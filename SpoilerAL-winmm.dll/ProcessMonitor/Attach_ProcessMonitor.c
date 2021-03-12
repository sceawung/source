#include <windows.h>
#include <dhcpsapi.h>
#define _NO_CRT_STDIO_INLINE
#include <stdio.h>
#include "ShowErrorMessage/ErrorMessage.h"
#define USING_NAMESPACE_BCB6_STD
#include "TProcessCtrl.h"
#include "TMainForm.h"
#include "TCanvas.h"
#include "TProcessSearchReportListnerBase.h"

EXTERN_C DWORD_DWORD __fastcall TProcessAddForm_ReLoadBtnClick_GetFirstModule(LPVOID, LPVOID, LPVOID);
EXTERN_C DWORD_DWORD __fastcall TProcessAddForm_ReLoadBtnClick_ProcessDGrid_Repaint(LPVOID, LPVOID);
EXTERN_C void __cdecl TSearchReportListner_OnReport_InvokeDrawProgress(WPARAM searchForm, unsigned long Pos);
EXTERN_C void __cdecl TSearchForm_AdjustValToString_GetStart(LPVOID activeElement, LPVOID ssgCtrl);
EXTERN_C void __fastcall TSearchForm_DrawCanvas(TProcessSearchReportListnerBase *reportListner, long ImageWidth, unsigned long Pos, TCanvas *Canv);
EXTERN_C void __cdecl TSearchForm_AddressLBoxDblClick_SubjectAccess(TMainForm* mainForm, TSSGSubject* SelectS);
EXTERN_C void __cdecl LoadHeapList(TProcessCtrl *this);

#define USE_INTERNAL_SPECIFICATION_OF_HEAP_ID 1

#if USE_INTERNAL_SPECIFICATION_OF_HEAP_ID
EXTERN_C BOOL __cdecl VerifyInternalSpecificationOfHeapID();
#endif

static const char *const _Format = "%d";

static __declspec(naked) LPCVOID /* bcc */__fastcall TMainForm_M_ProcessAddClick_ProcessNameEdit_setText()
{
	extern const DWORD vcl_System_AnsiString_ctor_sz;
	__asm {
		sub  esp, 0x80
		mov  ecx, esp
		push [edi]TMainForm.ssgCtrl.processCtrl.entry.th32ProcessID
		push _Format
		push 0x80
		push ecx
		call _snprintf
		add  esp, 0x10
		mov  edx, esp
		lea  eax, [ebp - 0x1C]// AnsiString
		call vcl_System_AnsiString_ctor_sz
		add  esp, 0x80
		ret
	}
}

static size_t __fastcall TMainForm_M_ProcessAddClick_mrOk(
	TMainForm *const this,
	string    *const tmpS,
	string    *const __position)
{
	extern void __cdecl OnProcessDetach(TProcessCtrl * proc);
	// New process id specified, clear previous information.
	char *end;
	vector_string *const tmpV = &this->ssgCtrl.processNameVec;
	for (string *desc = vector_begin(tmpV); desc < vector_end(tmpV); )
		if ((strtoul(string_begin(desc), &end, 0), end) == string_end(desc))
			memmove(desc, desc + 1, (size_t)--vector_end(tmpV) - (size_t)desc);
		else
			desc++;
	OnProcessDetach(&this->ssgCtrl.processCtrl);
	return __position - vector_begin(tmpV);
}

static __declspec(naked) pstring __cdecl TProcessAddForm_ProcessDGridClick_GetBack()
{
	__asm {
		lea  ecx, [esp + 0x10]// Token
		call string_dtor
		mov  eax, size PROCESSENTRY32A
		mul  edi// ProcessNum
		mov  edx, [ebx + 0x03A0]// processVec._M_start
		sub  esp, 0x80
		mov  ecx, esp
		push [edx + eax]PROCESSENTRY32A.th32ProcessID
		push _Format
		push 0x80
		push ecx
		call _snprintf
		add  esp, 0x10
		mov  ecx, [esp + 0x80 + size LPCVOID]
		mov  edx, esp
		push eax
		call string_ctor_assign_cstr_with_length
		add  esp, 0x80
		ret
	}
}

static __declspec(naked) uintptr_t TProcessCtrl_GetProcessVec_SkipSystemProcess(void)
{
	static uintptr_t const _M_insert_overflow = 0x004A3605;
	__asm {
		lea   ecx, [ebp - size bcb6_std_vector]// Vec
		mov   esi, [ecx]bcb6_std_vector._M_finish

		mov   eax, 0x004A3621
		cmp   [ebp - 0x01A8]PROCESSENTRY32A.th32ParentProcessID, 0
		je    SKIP

		mov   eax, 0x004A35E5
		cmp   esi, [ecx]bcb6_std_vector._M_end_of_storage
		cmove eax, _M_insert_overflow

	SKIP:
		rep ret
	}
}

static HANDLE __stdcall TProcessCtrl_Open_OpenProcess(DWORD dwDesiredAccess, BOOL bInheritHandle, long dwProcessId)
{
	if (dwProcessId > 0)
	{
		HANDLE const hProcess = OpenProcessWithSaveError(dwDesiredAccess | SYNCHRONIZE, bInheritHandle, dwProcessId);
		if (hProcess)
		{
			if (WaitForSingleObject(hProcess, 0) == WAIT_TIMEOUT)
				return hProcess;
			else
				CloseHandle(hProcess);
		}
	}
	return NULL;
}

#define PUSH_EDI  (BYTE )0x57
#define CALL_REL  (BYTE )0xE8
#define JMP_REL32 (BYTE )0xE9
#define NOP       (BYTE )0x90
#define NOP_X4    (DWORD)0x00401F0F

EXTERN_C void __cdecl Attach_ProcessMonitor()
{
	// TMainForm::M_ProcessAddClick
	*(LPDWORD)0x0044C6BE = (DWORD)TMainForm_M_ProcessAddClick_ProcessNameEdit_setText - (0x0044C6BE + sizeof(DWORD));

	*(LPBYTE )0x0044C76D =         0xFF   ;// push
	*(LPBYTE )0x0044C76E =         0x75   ;// dword ptr [ebp + ...]
	*(LPWORD )0x0044C770 = BSWAP16(0x8BCF);// mov   ecx, edi
	*(LPWORD )0x0044C772 = BSWAP16(0x8D55);// lea   edx,[ebp + ...]
	*(LPWORD )0x0044C774 = CALL_REL << 8 | 0xE8;
	*(LPDWORD)0x0044C776 = (DWORD)TMainForm_M_ProcessAddClick_mrOk - (0x0044C776 + sizeof(DWORD));

	// TProcessAddForm::ReLoadBtnClick
	*(LPBYTE )0x004873B7 = JMP_REL32;
	*(LPDWORD)0x004873B8 = 0x0048748F - (0x004873B8 + sizeof(DWORD));
	*(LPBYTE )0x004873BC = NOP;

	*(LPBYTE )0x00487495 = PUSH_EDI;
	*(LPBYTE )0x00487499 = 0x8B;// mov        eax,
	*(LPBYTE )0x0048749A = 0x85;// dword ptr [ebp + ...]
	*(LPDWORD)0x0048749B = -0x000002B8;
	*(LPBYTE )0x0048749F = 0x8D;// lea        ecx,
	*(LPBYTE )0x004874A0 = 0x88;// dword ptr [eax + ...]
	*(LPDWORD)0x004874A1 = +0x000003A0;
	*(LPBYTE )0x004874A5 = CALL_REL;
	*(LPDWORD)0x004874A6 = (DWORD)TProcessAddForm_ReLoadBtnClick_GetFirstModule - (0x004874A6 + sizeof(DWORD));
	*(LPWORD )0x004874AA = BSWAP16(0x85C0);// test eax, eax
	*(LPWORD )0x004874AC = BSWAP16(0x0F84);// jz   near
	*(LPDWORD)0x004874AE = 0x004873A5 - (0x004874AE + sizeof(DWORD));
	*(LPWORD )0x004874B2 = BSWAP16(0xFFC0);// inc  eax

	*(LPDWORD)0x004874C9 = SHGFI_ICON | SHGFI_USEFILEATTRIBUTES | SHGFI_SMALLICON;
	
	*(LPDWORD)0x004879C4 = BSWAP32(0x8B56FC8D);// mov edx, dword ptr [this]
	*(LPBYTE )0x004879C8 =         0x8A       ;// lea ecx, [edx]TProcessAddForm.processVec
	*(LPDWORD)0x004879C9 = 0x000003A0;
	*(LPBYTE )0x004879CD = CALL_REL;
	*(LPDWORD)0x004879CE = (DWORD)TProcessAddForm_ReLoadBtnClick_ProcessDGrid_Repaint - (0x004879CE + sizeof(DWORD));

	// TProcessAddForm::ProcessDGridClick
	*(LPDWORD)0x0048855E = (DWORD)TProcessAddForm_ProcessDGridClick_GetBack - (0x0048855E + sizeof(DWORD));

	// TSearchReportListner::OnReport
	//	must call from same thread
	*(LPDWORD)0x00490706 = (DWORD)TSearchReportListner_OnReport_InvokeDrawProgress - (0x00490706 + sizeof(DWORD));

	// TSearchReportListner::OnFindAddeess
	//	This->GetActiveElement()->GetStart(*This->GetSSGCtrl()) => this.start
	*(LPWORD )0x004907AE = 0x9066;
	*(LPWORD )0x004907B0 = 0x478B;
	*(LPBYTE )0x004907B2 =   0x14;

	// TSearchForm::UpdateStruct
	//	normalize calculating order
	*(LPDWORD)0x00494175 = 0x006218A0;
	*(LPDWORD)0x00494188 = 0x004D1858 - (0x00494188 + sizeof(DWORD));
	*(LPDWORD)0x004941B4 = 0x334;

	*(LPDWORD)0x00494219 = 0x0062188E;
	*(LPDWORD)0x0049422C = 0x004D187C - (0x0049422C + sizeof(DWORD));
	*(LPDWORD)0x00494258 = 0x32C;

	*(LPDWORD)0x004942BD = 0x00621897;
	*(LPDWORD)0x004942D0 = 0x004D18A0 - (0x004942D0 + sizeof(DWORD));
	*(LPDWORD)0x004942FC = 0x330;

	// TSearchForm::AdjustValToString
	//	activeElement->GetStart(*ssgCtrl) => (searchMode == 1 ? reportListner.start : activeElement->GetStart(*ssgCtrl))
	*(LPDWORD)0x004946F5 = (DWORD)TSearchForm_AdjustValToString_GetStart - (0x004946F5 + sizeof(DWORD));

	// TSearchForm::DrawProgress
#ifndef USE_ORIGINAL
	/*
		push    esi                                     ; 0049492D _ 56
		lea     ecx, [ebx + 400H]                       ; 0049492E _ 8D. 8B, 00000400
		push    dword ptr [ebp + 0CH]                   ; 00494934 _ FF. 75, 0C
		call    TSearchForm_DrawCanvas                  ; 00494937 _ E8, ????????
		jmp     00494B39H                               ; 0049493C _ E9, 000001F8
	*/
	*(LPBYTE )0x0049492D =       0x56;
	*(LPWORD )0x0049492E =     0x8B8D;
	*(LPDWORD)0x00494930 = 0x00000400;
	*(LPDWORD)0x00494934 = 0xE80C75FF;
	*(LPDWORD)0x00494938 = (DWORD)TSearchForm_DrawCanvas - (0x00494938 + sizeof(DWORD));
	*(LPBYTE )0x0049493C = JMP_REL32;
	*(LPDWORD)0x0049493D = 0x00494B39 - (0x0049493D + sizeof(DWORD));
#else
	//	activeElement->GetStart(*ssgCtrl) => reportListner.start
	*(LPBYTE )0x00494941 =       0x8D;
	*(LPWORD )0x00494942 =     0x008B;
	*(LPDWORD)0x00494944 = 0x83000004;
	*(LPDWORD)0x00494948 = 0x418B04EC;
	*(LPBYTE )0x0049494C =       0x14;
	//	activeElement->GetMin(*ssgCtrl)   => reportListner.min
	*(LPBYTE )0x00494973 =       0x8D;
	*(LPDWORD)0x00494974 = 0x0004008B;
	*(LPDWORD)0x00494978 = 0x04EC8300;
	*(LPWORD )0x0049497C =     0x418B;
	*(LPBYTE )0x0049497E =       0x18;
	//	activeElement->GetMax(*ssgCtrl)   => reportListner.max
	*(LPDWORD)0x00494A3C = 0x04008B8D;
	*(LPDWORD)0x00494A40 = 0xEC830000;
	*(LPDWORD)0x00494A44 = 0x1C418B04;
#endif

	// TSearchForm::SerachStartToolBtnClick
	//	normalize calculating order
	*(LPDWORD)0x00495026 = 0x004D1858 - (0x00495026 + sizeof(DWORD));
	*(LPDWORD)0x0049502F = -0x104;

	*(LPDWORD)0x0049505E = 0x004D18A0 - (0x0049505E + sizeof(DWORD));
	*(LPDWORD)0x00495064 = -0xFC;

	// TSearchForm::SerachStopToolBtnClick
	//	if(searchMode==0) return; => if(searchMode!=1 || reportListner.reserveMode==prcsCtrl::rsEND) return;
	*(LPDWORD)0x004955B4 = 0x0003CC05;
	*(LPDWORD)0x004955B8 = 0x01388000;
	*(LPDWORD)0x004955BC = 0x78830975;
	*(LPDWORD)0x004955C0 = 0x03740338;
	*(LPDWORD)0x004955C4 = 0xC30300C6;

	// TSearchForm::AddressLBoxDblClick
	*(LPDWORD)0x00497048 = (DWORD)TSearchForm_AddressLBoxDblClick_SubjectAccess - (0x00497048 + sizeof(DWORD));

	// TProcessCtrl::Clear
	//   entry.th32ProcessID=NULL; => entry.th32ProcessID=-1;
	*(LPWORD)  0x004A34A6      = BSWAP16(NOP << 8 | 0x5F);
	*(LPWORD )(0x004A34A8 + 0) = BSWAP16(0x838B);
	*(LPBYTE )(0x004A34A8 + 6) = -1;

	// TProcessCtrl::GetProcessVec
	*(LPBYTE )0x004A35DD = CALL_REL;
	*(LPDWORD)0x004A35DE = (DWORD)TProcessCtrl_GetProcessVec_SkipSystemProcess - (0x004A35DE + sizeof(DWORD));
	*(LPWORD )0x004A35E2 = BSWAP16(0xFFE0);
	*(LPBYTE )0x004A35E4 = 0xCC;

	// TProcessCtrl::LoadHeapList
#if USE_INTERNAL_SPECIFICATION_OF_HEAP_ID
	if (VerifyInternalSpecificationOfHeapID())
#endif
	{
		*(LPBYTE )0x004A3980 = JMP_REL32;
		*(LPDWORD)0x004A3981 = (DWORD)LoadHeapList - (0x004A3981 + sizeof(DWORD));
		*(LPDWORD)0x004A3985 = NOP_X4;
	}
	*(LPBYTE )(0x004A39B3 + 6) = (BYTE)INVALID_HANDLE_VALUE;

	// TProcessCtrl::FindProcess
	*(LPBYTE )0x004A5ACC = JMP_REL32;
	*(LPDWORD)0x004A5ACD = (DWORD)TProcessCtrl_FindProcess - (0x004A5ACD + sizeof(DWORD));
	*(LPDWORD)0x004A5AD1 = NOP_X4;

	// TProcessCtrl::Attach(string ProcessName)
	*(LPBYTE )0x004A5FF0 = JMP_REL32;
	*(LPDWORD)0x004A5FF1 = (DWORD)TProcessCtrl_AttachByProcessName - (0x004A5FF1 + sizeof(DWORD));
	*(LPDWORD)0x004A5FF5 = NOP_X4;

	// TProcessCtrl::Attach(void)
	*(LPBYTE )0x004A610C = JMP_REL32;
	*(LPDWORD)0x004A610D = (DWORD)TProcessCtrl_Attach - (0x004A610D + sizeof(DWORD));
	*(LPBYTE )0x004A6111 = NOP;

	// TProcessCtrl::Open
	*(LPDWORD)(0x004A61A1 + 1) = (DWORD)TProcessCtrl_Open_OpenProcess - (0x04A61A1 + 1 + sizeof(DWORD));

	// TProcessCtrl::LoadHeapList
	//   (Snapshot==NULL) => (Snapshot==INVALID_HANDLE_VALUE)
	*(LPWORD )0x004A66D5 = BSWAP16(0xFFC0);

	// TProcessCtrl::SearchFunc
	//	missing Listner->OnSearchEnd()
	*(LPBYTE )0x004A6B16 = JMP_REL32;
	*(LPDWORD)0x004A6B17 = 0x004A6D9E - (0x004A6B17 + sizeof(DWORD));
	//	missing Listner->OnSearchEnd()
	*(LPBYTE )0x004A6C73 = JMP_REL32;
	*(LPDWORD)0x004A6C74 = 0x004A6B9A - (0x004A6C74 + sizeof(DWORD));

	// TSSBundleList::Read
	//   SSGC.processCtrl.Open => SSGC.Open
	*(LPBYTE )(0x004BF377 + 0) = 0xFF;
	*(LPBYTE )(0x004BF377 + 1) = 0xB5;
	*(LPDWORD)(0x004BF377 + 2) = 8;
	*(LPDWORD)(0x004BF37E + 1) = 0x0051C338 - (0x004BF37E + 1 + sizeof(DWORD));
	*(LPBYTE )(0x004BF383 + 2) = 12;

	// TSSTrace::Write
	//   SSGC.processCtrl.Open => SSGC.Open
	*(LPBYTE )(0x0052CE9E + 0) = 0x68;
	*(LPDWORD)(0x0052CE9E + 1) = PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION;
	*(LPBYTE ) 0x0052CEA3      = 0x53;
	*(LPBYTE )(0x0052CEAD + 0) = 0x8B;
	*(LPBYTE )(0x0052CEAD + 1) = 0xC6;
	*(LPDWORD)(0x0052CEB0 + 1) = 0x0051C338 - (0x0052CEB0 + 1 + sizeof(DWORD));
	*(LPBYTE )(0x0052CEB5 + 2) = 12;
}
