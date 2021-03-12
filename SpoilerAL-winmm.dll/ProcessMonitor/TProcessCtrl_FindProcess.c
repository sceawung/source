#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <mbstring.h>

#ifndef __BORLANDC__
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#else
#define string_empty(s)             (s)->empty()
#define string_length(s)            (s)->length()
#define string_begin(s)             (s)->begin()
#define string_end(s)               (s)->end()
#define string_assign_cstr(s, cstr) *(s) = (cstr);
#endif

#include "intrinsic.h"
#include "tlhelp32fix.h"

#include "ProcessMonitor.h"
#include "FindWindowExceptInvalids.h"
#include "GetFileTitlePointer.h"

extern HANDLE hHeap;

static char * __fastcall FindInvalidChar(const char *string)
{
	while (*string)
	{
		if (!__intrinsic_isleadbyte(*string))
		{
			switch (*string)
			{
			case '\\':
			case '/':
			case ':':
			case '*':
			case '?':
			case '"':
			case '<':
			case '>':
			case '|':
				return (char *)string;
			}
		}
		else
		{
			if (!*(++string))
				break;
		}
		string++;
	}
	return NULL;
}

static char ** __stdcall ParseArgument(const char *begin, const char *end, size_t *argc)
{
	while (*begin == ' ')
		begin++;
	if (end > begin)
	{
		size_t length, maxArgCount, capacity;
		char   **argv;

		length = end - begin;
		maxArgCount = (length + 1) / 2;
		capacity = maxArgCount * sizeof(char *) + (length + 1) * sizeof(char);
		argv = (char **)HeapAlloc(hHeap, 0, capacity);
		if (argv)
		{
			char    *p;
			BOOLEAN inDoubleQuote;
			size_t  index;

			p = (char *)(argv + maxArgCount);
			*(char *)(end = p + length) = '\0';
			memcpy(p, begin, length * sizeof(char));
			inDoubleQuote = FALSE;
			argv[index = 0] = p;
			while (*p)
			{
				if (!__intrinsic_isleadbyte(*p))
				{
					switch (*p)
					{
					case ' ':
						if (inDoubleQuote)
							break;
						*p = '\0';
						while (*(++p) == ' ');
						if (*p)
							argv[++index] = p;
						continue;
					case '"':
						inDoubleQuote = !inDoubleQuote;
						memcpy(p, p + 1, ((end--) - p) * sizeof(char));
						continue;
					case '\\':
						length = 0;
						while (p[++length] == '\\');
						if (p[length] == '"')
						{
							const char *src;

							length = (length + 1) / 2;
							src = p + length;
							memcpy(p, src, (end - src + 1) * sizeof(char));
							end -= length;
						}
						p += length;
						continue;
					}
				}
				else if (!*(++p))
					break;
				p++;
			}
			*argc = index + 1;
			return argv;
		}
	}
	*argc = 0;
	return NULL;
}

#ifdef __BORLANDC__
unsigned long TProcessCtrl::FindProcess(string *ProcessName, PROCESSENTRY32 *Entry)
#else
unsigned long __cdecl TProcessCtrl_FindProcess(LPVOID this, string *ProcessName, PROCESSENTRY32A *Entry)
#endif
{
	#define CLASSNAME_BRACKET_OPEN  '<'
	#define CLASSNAME_BRACKET_CLOSE '>'
	#define WINDOWNAME_PREFIX       '*'
	#define MODULENAME_PREFIX       ':'
	#define CMDLINEARG_PREFIX       '?'
	#define OPTION_PREFIX           '/'

	do	/* do { ... } while (0); */
	{
		DWORD           dwProcessId;
		HANDLE          hSnapshot;
		PROCESSENTRY32A pe;
		char          * end;

		if (string_empty(ProcessName))
			break;
		if ((dwProcessId = strtoul(string_begin(ProcessName), &end, 0), end) == string_end(ProcessName))
			;
		else if (!FindInvalidChar(string_begin(ProcessName)))
		{
			dwProcessId = FindProcessId(FALSE, string_begin(ProcessName), string_length(ProcessName), NULL, NULL);
		}
		else
		{
			char   **argv;
			size_t argc;
			BOOL   bIsRegex;
			LPSTR  lpProcessName;
			LPSTR  lpClassName;
			LPSTR  lpWindowName;
			LPSTR  lpModuleName;
			LPSTR  lpCmdLineArg;
			size_t length;

			argv = ParseArgument(string_begin(ProcessName), string_end(ProcessName), &argc);
			if (!argv)
				break;
			dwProcessId = 0;
			bIsRegex = FALSE;
			lpProcessName = NULL;
			lpClassName = NULL;
			lpWindowName = NULL;
			lpModuleName = NULL;
			lpCmdLineArg = NULL;
			for (size_t i = 0; i < argc; i++)
			{
				switch (*argv[i])
				{
				case CLASSNAME_BRACKET_OPEN:
					if (lpClassName)
						break;
					length = strlen(argv[i] + 1);
					if (!length || argv[i][length] != CLASSNAME_BRACKET_CLOSE)
						break;
					argv[i][length] = '\0';
					lpClassName = argv[i] + 1;
					break;
				case WINDOWNAME_PREFIX:
					if (!lpWindowName)
						lpWindowName = argv[i] + 1;
					break;
				case MODULENAME_PREFIX:
					if (!lpModuleName)
						lpModuleName = argv[i] + 1;
					break;
				case CMDLINEARG_PREFIX:
					if (!lpCmdLineArg)
						lpCmdLineArg = argv[i] + 1;
					break;
				case OPTION_PREFIX:
					if (_stricmp(argv[i] + 1, "regex") == 0)
						bIsRegex = TRUE;
					break;
				default:
					if (!lpProcessName)
						lpProcessName = argv[i];
					break;
				}
			}
			if (lpCmdLineArg && !bIsRegex)
				_mbslwr(lpCmdLineArg);
			if (lpProcessName || ((lpModuleName || lpCmdLineArg) && !lpClassName && !lpWindowName))
			{
				dwProcessId = FindProcessId(bIsRegex, lpProcessName, lpProcessName ? strlen(lpProcessName) : 0, lpModuleName, lpCmdLineArg);
			}
			else if (lpClassName || lpWindowName)
			{
				if (FindWindowExceptInvalids(&dwProcessId, bIsRegex, lpClassName, lpWindowName, lpModuleName, lpCmdLineArg))
					StopProcessMonitor();
			}
			HeapFree(hHeap, 0, argv);
		}
		if (!dwProcessId)
			break;
		hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnapshot == INVALID_HANDLE_VALUE)
			break;
		pe.dwSize = sizeof(PROCESSENTRY32A);
		if (Process32FirstA(hSnapshot, &pe))
		{
			DWORD const dwErrCode = GetLastError();
			do
			{
				if (pe.th32ProcessID != dwProcessId)
					continue;
				CloseHandle(hSnapshot);
				string_assign_cstr(ProcessName, GetFileTitlePointerA(pe.szExeFile));
				*Entry = pe;
				return 0;
			} while (Process32NextA(hSnapshot, &pe));
			SetLastError(dwErrCode);// discard ERROR_NO_MORE_FILES
		}
		CloseHandle(hSnapshot);
	} while (0);
	return 1;

	#undef OPTION_PREFIX
	#undef CMDLINEARG_PREFIX
	#undef MODULENAME_PREFIX
	#undef WINDOWNAME_PREFIX
	#undef CLASSNAME_BRACKET_CLOSE
	#undef CLASSNAME_BRACKET_OPEN
}
