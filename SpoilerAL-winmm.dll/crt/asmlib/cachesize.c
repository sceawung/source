#include <stddef.h>

extern void __cdecl CpuType(int *vendor, int *family, int *model);

#pragma pack(push, 1)

#define numlevels 4                                         // max level

typedef struct {
	unsigned long ok;
	unsigned long level1;
	unsigned long level2;
	unsigned long level3;
	unsigned long level4;
} data_layout;

typedef struct {                                            // record for table of cache descriptors
	unsigned char d_key;                                    // key from cpuid instruction
	unsigned char d_level;                                  // cache level
	unsigned char d_sizem;                                  // size multiplier
	unsigned char d_2pow;                                   // power of 2. size = d_sizem << d_2pow
} descriptor_record;

#pragma pack(pop)

__declspec(align(4)) static data_layout dataref = {         // reference point
	0,                                                      // 1 when values are determined
	0,                                                      // level 1 data cache size
	0,                                                      // level 2 data cache size
	0,                                                      // level 3 data cache size
	0,                                                      // level 4 data cache size
};

#define descriptortable_length 61

__declspec(align(4)) static const descriptor_record descriptortable[descriptortable_length] = {
	{ 0x0A, 1, 1, 13 },                                     //   8 kb L1 data cache
	{ 0x0C, 1, 1, 14 },                                     //  16 kb L1 data cache
	{ 0x0D, 1, 1, 14 },                                     //  16 kb L1 data cache
	{ 0x21, 2, 1, 18 },                                     // 256 kb L2 data cache
	{ 0x22, 3, 1, 19 },                                     // 512 kb L3 data cache
	{ 0x23, 3, 1, 20 },                                     //   1 Mb L3 data cache
	{ 0x25, 3, 1, 21 },                                     //   2 Mb L3 data cache
	{ 0x29, 3, 1, 22 },                                     //   4 Mb L3 data cache
	{ 0x2C, 1, 1, 15 },                                     //  32 kb L1 data cache
	{ 0x39, 2, 1, 17 },                                     // 128 kb L2 data cache
	{ 0x3A, 2, 3, 16 },                                     // 192 kb L2 data cache
	{ 0x3B, 2, 1, 17 },                                     // 128 kb L1 data cache
	{ 0x3C, 2, 1, 18 },                                     // 256 kb L1 data cache
	{ 0x3D, 2, 3, 17 },                                     // 384 kb L2 data cache
	{ 0x3E, 2, 1, 19 },                                     // 512 kb L2 data cache
	{ 0x41, 2, 1, 17 },                                     // 128 kb L2 data cache
	{ 0x42, 2, 1, 18 },                                     // 256 kb L2 data cache
	{ 0x43, 2, 1, 19 },                                     // 512 kb L2 data cache
	{ 0x44, 2, 1, 20 },                                     //   1 Mb L2 data cache
	{ 0x45, 2, 1, 21 },                                     //   2 Mb L2 data cache
	{ 0x46, 3, 1, 22 },                                     //   4 Mb L3 data cache
	{ 0x47, 3, 1, 23 },                                     //   8 Mb L3 data cache
	{ 0x48, 2, 3, 20 },                                     //   3 Mb L2 data cache
	{ 0x49, 2, 1, 22 },                                     //   4 Mb L2 or L3 data cache
	{ 0x4A, 3, 3, 21 },                                     //   6 Mb L3 data cache
	{ 0x4B, 3, 1, 23 },                                     //   8 Mb L3 data cache
	{ 0x4C, 3, 3, 22 },                                     //  12 Mb L3 data cache
	{ 0x4D, 3, 1, 24 },                                     //  16 Mb L3 data cache
	{ 0x4E, 2, 3, 21 },                                     //   6 Mb L2 data cache
	{ 0x60, 1, 1, 14 },                                     //  16 kb L1 data cache
	{ 0x66, 1, 1, 13 },                                     //   8 kb L1 data cache
	{ 0x67, 1, 1, 14 },                                     //  16 kb L1 data cache
	{ 0x68, 1, 1, 15 },                                     //  32 kb L1 data cache
	{ 0x78, 2, 1, 20 },                                     //   1 Mb L2 data cache
	{ 0x79, 2, 1, 17 },                                     // 128 kb L2 data cache
	{ 0x7A, 2, 1, 18 },                                     // 256 kb L2 data cache
	{ 0x7B, 2, 1, 19 },                                     // 512 kb L2 data cache
	{ 0x7C, 2, 1, 20 },                                     //   1 Mb L2 data cache
	{ 0x7D, 2, 1, 21 },                                     //   2 Mb L2 data cache
	{ 0x7F, 2, 1, 19 },                                     // 512 kb L2 data cache
	{ 0x82, 2, 1, 18 },                                     // 256 kb L2 data cache
	{ 0x83, 2, 1, 19 },                                     // 512 kb L2 data cache
	{ 0x84, 2, 1, 20 },                                     //   1 Mb L2 data cache
	{ 0x85, 2, 1, 21 },                                     //   2 Mb L2 data cache
	{ 0x86, 2, 1, 19 },                                     // 512 kb L2 data cache
	{ 0x87, 2, 1, 20 },                                     //   1 Mb L2 data cache
	{ 0xD0, 3, 1, 19 },                                     // 512 kb L3 data cache
	{ 0xD1, 3, 1, 20 },                                     //   1 Mb L3 data cache
	{ 0xD2, 3, 1, 21 },                                     //   2 Mb L3 data cache
	{ 0xD6, 3, 1, 20 },                                     //   1 Mb L3 data cache
	{ 0xD7, 3, 1, 21 },                                     //   2 Mb L3 data cache
	{ 0xD8, 3, 1, 22 },                                     //   4 Mb L3 data cache
	{ 0xDC, 3, 3, 19 },                                     // 1.5 Mb L3 data cache
	{ 0xDD, 3, 3, 20 },                                     //   3 Mb L3 data cache
	{ 0xDE, 3, 3, 21 },                                     //   6 Mb L3 data cache
	{ 0xE2, 3, 1, 21 },                                     //   2 Mb L3 data cache
	{ 0xE3, 3, 1, 22 },                                     //   4 Mb L3 data cache
	{ 0xE4, 3, 1, 23 },                                     //   8 Mb L3 data cache
	{ 0xEA, 3, 3, 22 },                                     //  12 Mb L3 data cache
	{ 0xEB, 3, 9, 21 },                                     //  18 Mb L3 data cache
	{ 0xEC, 3, 3, 23 },                                     //  24 Mb L3 data cache
};

// Function entry:
__declspec(naked) size_t __cdecl DataCacheSize(int level)
{
	static void __cdecl IntelNewMethod();
	static void __cdecl IntelOldMethod();
	static void __cdecl AMDMethod();

	__asm
	{
		push    ebx
		push    esi
		mov     esi, dword ptr [esp + 12]                   // level

		// check if called before
		cmp     dataref.ok, 1
		je      D800

		// find cpu vendor
		push    0
		mov     eax, esp
		push    0
		push    0
		push    eax
		call    CpuType
		mov     eax, dword ptr [esp + 12]                   // eax = vendor
		add     esp, 16
		dec     eax
		jz      Intel
		dec     eax
		jz      AMD
		dec     eax
		jz      VIA

		// unknown vendor, try all methods
		call    IntelNewMethod
		jnc     D800                                        // not carry = success
		call    AMDMethod
		jnc     D800                                        // not carry = success
		call    IntelOldMethod
		jmp     D800                                        // return whether success or not

	Intel:
		call    IntelNewMethod
		jnc     D800                                        // not carry = success
		call    IntelOldMethod
		jmp     D800                                        // return whether success or not

	AMD:
	VIA:
		// AMD and VIA use same method
		call    AMDMethod

	D800:
		// cache data known, get desired return value
		xor     eax, eax
		cmp     esi, numlevels
		ja      D900
		test    esi, esi
		jz      D820

		// level = 1 .. numlevels
		mov     eax, dword ptr [dataref + esi * 4]          // size of selected cache
		jmp     D850

	D820:
		// level = 0. Get size of largest level cache
		mov     eax, dataref.level3
		mov     ecx, dataref.level2
		test    eax, eax
		jnz     D850
		or      eax, ecx
		jnz     D850
		mov     eax, dataref.level1

	D850:
		mov     dataref.ok, 1                               // remember called, whether success or not

	D900:
		pop     esi
		pop     ebx
		ret
	}
}

// Determine cache sizes by CPUID function 4
// output: values returned in dataref + level1, level2, level3
// carry flag = 0 on succes
__declspec(naked) static void __cdecl IntelNewMethod()
{
	__asm
	{
		xor     eax, eax
		cpuid                                               // get number of CPUID functions
		cmp     eax, 4
		jb      I900                                        // fail
		xor     ebp, ebp                                    // loop counter

	I100:
		mov     eax, 4
		mov     ecx, ebp
		cpuid                                               // get cache parameters
		mov     edx, eax
		and     edx, 11111B                                 // cache type
		jz      I500                                        // no more caches
		cmp     edx, 2
		je      I200                                        // code cache, ignore
		inc     ecx                                         // sets
		mov     edx, ebx
		shr     edx, 22
		inc     edx                                         // ways
		imul    ecx, edx
		mov     edx, ebx
		shr     edx, 12
		and     edx, 1111111111B
		inc     edx                                         // partitions
		imul    ecx, edx
		and     ebx, 111111111111B
		inc     ebx                                         // line size
		imul    ecx, ebx                                    // calculated cache size
		shr     eax, 5
		and     eax, 111B                                   // cache level
		cmp     eax, numlevels
		jna     I180
		mov     eax, numlevels                              // limit higher levels

	I180:
		mov     dword ptr [dataref + eax * 4], ecx          // store size of data cache level eax

	I200:
		inc     ebp
		cmp     ebp, 100H                                   // avoid infinite loop
		jb      I100                                        // next cache

	I500:
		// loop finished
		// check if OK
		mov     eax, dataref.level1
		cmp     eax, 1024

	I900:
		rep ret                                             // carry flag set if fail
	}
}

// Determine cache sizes by CPUID function 2
// output: values returned in dataref + level1, level2, level3
// carry flag = 0 on succes
__declspec(naked) static void __cdecl IntelOldMethod()
{
	__asm
	{
		xor     eax, eax
		cpuid                                               // get number of CPUID functions
		cmp     eax, 2
		jb      J900                                        // fail
		mov     eax, 2
		xor     ecx, ecx
		cpuid                                               // get 16 descriptor bytes in eax, ebx, ecx, edx
		mov     al, 0                                       // al does not contain a descriptor
		push    eax                                         // save all descriptors
		push    ebx
		push    ecx
		push    edx                                         // now esp points to descriptors
		mov     edx, 15                                     // loop counter

		// loop to read 16 descriptor bytes
	J100:
		mov     al, byte ptr [esp + edx]

		// find in table
		mov     ebx, descriptortable_length - 1             // loop counter

		// loop to search in descriptortable
	J200:
		cmp     al, descriptortable[ebx * size descriptor_record].d_key
		jne     J300

		// descriptor found
		movzx   eax, descriptortable[ebx * size descriptor_record].d_sizem
		mov     cl,  descriptortable[ebx * size descriptor_record].d_2pow
		shl     eax, cl                                     // compute size
		movzx   ecx, descriptortable[ebx * size descriptor_record].d_level

		// check that level = 1-3
		cmp     ecx, 3
		ja      J300
		mov     dword ptr [dataref + ecx * 4], eax          // store size eax of data cache level ecx

	J300:
		dec     ebx
		jns     J200                                        // inner loop
		dec     edx
		jns     J100                                        // outer loop
		add     esp, 16                                     // remove from stack

		// check if OK
		mov     eax, dataref.level1
		cmp     eax, 1024

	J900:
		rep ret                                             // carry flag set if fail
	}
}

// Determine cache sizes by CPUID function 80000005H - 80000006H
// output: values returned in dataref
// carry flag = 0 on succes
__declspec(naked) static void __cdecl AMDMethod()
{
	__asm
	{
		mov     eax, 80000000H
		cpuid                                               // get number of CPUID functions
		cmp     eax, 6
		jb      K900                                        // fail
		mov     eax, 80000005H
		cpuid                                               // get L1 cache size
		shr     ecx, 24                                     // L1 data cache size in kbytes
		shl     ecx, 10                                     // L1 data cache size in bytes
		mov     dataref.level1, ecx                         // store L1 data cache size
		mov     eax, 80000006H
		cpuid                                               // get L2 and L3 cache sizes
		shr     ecx, 16                                     // L2 data cache size in kbytes
		shl     ecx, 10                                     // L2 data cache size in bytes
		mov     dataref.level2, ecx                         // store L2 data cache size
		mov     ecx, edx
		shr     ecx, 18                                     // L3 data cache size / 512 kbytes
		shl     ecx, 19                                     // L3 data cache size in bytes

	//K100:
		mov     dataref.level3, ecx                         // store L3 data cache size

		// check if OK
		mov     eax, dataref.level1
		cmp     eax, 1024

	K900:
		rep ret                                             // carry flag set if fail
	}
}
