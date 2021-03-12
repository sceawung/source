#include <windows.h>
#include <dhcpsapi.h>
#define USING_NAMESPACE_BCB6_STD
#include "TSSGCtrl.h"
#include "SSGSubjectProperty.h"

EXTERN_C UINT MultiStageAttrs;
EXTERN_C DWORD_DWORD __fastcall TSSGCtrl_ReadSSG_rootSubject_SetAttribute(void *, void *, void *, void *);
EXTERN_C unsigned long __cdecl Parsing(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const bcb6_std_string *Src, ...);
EXTERN_C double __cdecl ParsingDouble(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const string *Src, IN double Val);

static DWORD_DWORD __stdcall TSSGCtrl_AddressAttributeFilter_GetAddress(TReplaceAttribute const *const *const it, LPCVOID const Address, va_list fp)
{
	TSSGCtrl *SSGCtrl = (va_arg(fp, void *), va_arg(fp, void *), va_arg(fp, void *));
	TSSGSubject *SSGS = va_arg(fp, void *);
	switch ((*it)->displace)
	{
	case TRUE:
		if (SSGS && SSGS->folder)
			SSGS = &SSGS->folder->super;
		break;
	case TRUE + TRUE:
		*va_arg(fp, LPCVOID *) = Address;
		break;
	}
	return (DWORD_DWORD) { (DWORD)SSGCtrl, (DWORD)SSGS };
}

static TFunnelAttribute *__fastcall TSSGCtrl_CheckFunnel_MultiStage(va_list args, TSSGSubject *SSGS)
{
	if (SSGS->attribute)
	{
		const COORD coord = TSSGAttributeElement_GetViaCoord(atFUNNEL, SSGS->attribute).dwFontSize;
		if (coord.X)
		{
			TSSGCtrl *this = va_arg(args, void *);
			unsigned long Val = (va_arg(args, void *), va_arg(args, unsigned long));
			TOnErrorAttribute *err = TSSGCtrl_GetAttribute(this, SSGS, atON_ERROR);
			for (TFunnelAttribute **base = &vector_type_at(SSGS->attribute, TFunnelAttribute *, coord.Y),
					 **cur  = base + coord.X;
					 --cur >= base; )
				if (!TSSGCtrl_Funneling(this, SSGS, &(*cur)->fileName, Val) && !err)
					break;
		}
	}
	return NULL;// means processed
}

static __declspec(naked) TSSGAttributeElement *TSSGCtrl_CheckFunnel_GetAttribute(TSSGCtrl *this, TSSGSubject *SSGS, AeType Type)
{
	__asm {// edx is SSGS already
		lea  ecx, [ebp + 8]// "this" on frame
		test MultiStageAttrs, atFUNNEL
#pragma warning(suppress: 4414)
		jnz  TSSGCtrl_CheckFunnel_MultiStage
		jmp  TSSGCtrl_GetAttribute
		ud2
	}
}

static TIO_FEPAttribute *__fastcall TSSGCtrl_CheckIO_FEP_MultiStage(va_list args, TSSGSubject *SSGS)
{
	static const char VarName[] = "Val";
	if (SSGS->attribute)
	{
		const COORD coord = TSSGAttributeElement_GetViaCoord(atIO_FEP, SSGS->attribute).dwFontSize;
		if (coord.X)
		{
			TSSGCtrl *this = va_arg(args, void *);
			unsigned long *Val = (va_arg(args, void *), &va_arg(args, unsigned long));
			if (!va_arg(args, bool))
				for (TIO_FEPAttribute **cur  = &vector_type_at(SSGS->attribute, TIO_FEPAttribute *, coord.Y), **apex = cur + coord.X;
					 cur < apex;
					 cur++)
					*Val = Parsing(this, SSGS, TIO_FEPAttribute_GetOutputCode(*cur), _countof(VarName) - 1, VarName, (uint64_t)*Val, 0);
			else
				for (TIO_FEPAttribute **base = &vector_type_at(SSGS->attribute, TIO_FEPAttribute *, coord.Y),
					 **cur  = base + coord.X;
					 --cur >= base; )
					*Val = Parsing(this, SSGS, TIO_FEPAttribute_GetInputCode(*cur) , _countof(VarName) - 1, VarName, (uint64_t)*Val, 0);
		}
	}
	return NULL;// means processed
}

static __declspec(naked) TSSGAttributeElement *TSSGCtrl_CheckIO_FEP_GetAttribute(TSSGCtrl *this, TSSGSubject *SSGS, AeType Type)
{
	__asm {// edx is SSGS already
		lea  ecx, [ebp + 8]// "this" on frame
		test MultiStageAttrs, atIO_FEP
#pragma warning(suppress: 4414)
		jnz  TSSGCtrl_CheckIO_FEP_MultiStage
		jmp  TSSGCtrl_GetAttribute
		ud2
	}
}

static TIO_FEPAttribute *__fastcall TSSGCtrl_CheckIO_FEPDouble_MultiStage(va_list args, TSSGSubject *SSGS)
{
	if (SSGS->attribute)
	{
		const COORD coord = TSSGAttributeElement_GetViaCoord(atIO_FEP, SSGS->attribute).dwFontSize;
		if (coord.X)
		{
			TSSGCtrl *this = va_arg(args, void *);
			double *Val = (va_arg(args, void *), &va_arg(args, double));
			if (!va_arg(args, bool))
				for (TIO_FEPAttribute **cur  = &vector_type_at(SSGS->attribute, TIO_FEPAttribute *, coord.Y), **apex = cur + coord.X;
					 cur < apex;
					 cur++)
					*Val = ParsingDouble(this, SSGS, TIO_FEPAttribute_GetOutputCode(*cur), *Val);
			else
				for (TIO_FEPAttribute **base = &vector_type_at(SSGS->attribute, TIO_FEPAttribute *, coord.Y),
					 **cur  = base + coord.X;
					 --cur >= base; )
					*Val = ParsingDouble(this, SSGS, TIO_FEPAttribute_GetInputCode(*cur) , *Val);
		}
	}
	return NULL;// means processed
}

static __declspec(naked) TSSGAttributeElement *TSSGCtrl_CheckIO_FEPDouble_GetAttribute(TSSGCtrl *this, TSSGSubject *SSGS, AeType Type)
{
	__asm {// edx is SSGS already
		lea  ecx, [ebp + 8]// "this" on frame
		test MultiStageAttrs, atIO_FEP
#pragma warning(suppress: 4414)
		jnz  TSSGCtrl_CheckIO_FEPDouble_MultiStage
		jmp  TSSGCtrl_GetAttribute
		ud2
	}
}

static __declspec(naked) void __fastcall THeapAdjustmentAttribute_dtor_heapMap(THeapAdjustmentAttribute *this, struct _Rb_tree_node *root)
{
	extern void(__cdecl *const tree_string_double_M_erase)(struct _Rb_tree *this, struct _Rb_tree_node *__x);
	static void(__cdecl *const tree_ulong_pair_uu_M_erase)(struct _Rb_tree *this, struct _Rb_tree_node *__x) = (void *)0x004D0A80;

	__asm {
		mov   eax, tree_ulong_pair_uu_M_erase
		cmp   [ecx]TSSGAttributeElement.type, atSCOPE
		cmove eax, tree_string_double_M_erase
		jmp   eax
		ud2
	}
}

#define PUSH_EDX  (BYTE )0x52
#define PUSH_EBX  (BYTE )0x53
#define PUSH_EBP  (BYTE )0x55
#define JNZ_SHORT (BYTE )0x75
#define NOP       (BYTE )0x90
#define NOP_X4    (DWORD)0x00401F0F
#define CALL_REL  (BYTE )0xE8
#define JMP_REL32 (BYTE )0xE9

EXTERN_C void __cdecl Attach_EnumReadSSG()
{
	// TSSGCtrl::ReadSSG
	*(LPBYTE )(0x004E45C8 + 1) = sizeof(TDirAttribute);

	*(LPBYTE )(0x004E45F7 + 0) = 0x89;// mov [eax]
	*(LPBYTE )(0x004E45F7 + 1) = 0x08;// ecx
	*(LPBYTE )(0x004E45F9 + 0) = 0x8B;// mov edx, 
	*(LPBYTE )(0x004E45F9 + 1) = 0x93;// [ebx + ... 
	*(LPDWORD)(0x004E45F9 + 2) = offsetof(TSSGCtrl, rootSubject);
	*(LPBYTE )(0x004E4601 + 1) = 0x48;// edx => eax 
	*(LPWORD )(0x004E4604 + 0) = BSWAP16(0x8948);// mov [eax + ...], ecx
	*(LPBYTE )(0x004E4604 + 2) = offsetof(TDirAttribute, identity);
	*(LPBYTE )(0x004E4617 + 0) = 0x89;// mov [eax + ...]
	*(LPBYTE )(0x004E4617 + 1) = 0x50;// edx
	*(LPBYTE )(0x004E4617 + 2) = offsetof(TDirAttribute, arbitrary);

	*(LPWORD ) 0x004E462E      = BSWAP16(0x8BD0);// mov edx, nowAttributeVec 
	*(LPWORD ) 0x004E4630      = BSWAP16(0x8BCB);// mov ecx, this
	*(LPBYTE )(0x004E4632 + 0) = CALL_REL;
	*(LPDWORD)(0x004E4632 + 1) = (DWORD)TSSGCtrl_ReadSSG_rootSubject_SetAttribute - (0x004E4632 + 1 + sizeof(DWORD));

	// TSSGCtrl::EnumReadSSG
	*(LPBYTE )0x004E5090 = JMP_REL32;
	*(LPDWORD)0x004E5091 = (DWORD)TSSGCtrl_EnumReadSSG - (0x004E5091 + sizeof(DWORD));
	*(LPDWORD)0x004E5095 = BSWAP32(0xC8FC0F00);

	// TSSGCtrl::MakeSubjectClass
	*(LPDWORD)(0x004EC409 + 1) = (DWORD)TSSGCtrl_MakeSubjectType - (0x004EC409 + 1 + sizeof(DWORD));

	// TSSGCtrl::ReadADJFile
	*(LPBYTE )(0x004FD549 + 1) = sizeof(TDirAttribute);
	*(LPWORD )(0x004FD594 + 0) = BSWAP16(0x8948);// mov [eax + ...], ecx
	*(LPBYTE )(0x004FD594 + 2) = offsetof(TDirAttribute, identity);
	*(LPBYTE )(0x004FD597 + 0) = 0x89;// mov [eax + ...]
	*(LPBYTE )(0x004FD597 + 1) = 0x48;// ecx
	*(LPBYTE )(0x004FD597 + 2) = offsetof(TDirAttribute, arbitrary);

	// TSSGCtrl::GetAttribute
	*(LPWORD )0x005038FE = BSWAP16(0x8BCB);// mov ecx, ebx
	*(LPWORD )0x00503900 = BSWAP16(0x5B5D);// pop ebx
	*(LPBYTE )0x00503902 = JMP_REL32      ;// pop ebp
	*(LPDWORD)0x00503903 = (DWORD)TSSGAttributeElement_GetViaCoord - (0x00503903 + sizeof(DWORD));

	// TSSGCtrl::AddressAttributeFilter
	//   keeping Address initial value.
	*(LPWORD )(0x00504133 + 1) = BSWAP16(0xEC48);// sub  esp, 0x48
	*(LPDWORD) 0x0050414C      = BSWAP16(0x8B0F);// mov  ecx, dword ptr [Address]
	*(LPWORD ) 0x0050414E      = BSWAP16(0x85C0);// test eax, eax 
	*(LPBYTE )(0x00504150 + 0) = JNZ_SHORT;
	*(LPBYTE )(0x00504150 + 1) = 0x00504161 - (0x00504150 + 1 + sizeof(BYTE));
	*(LPBYTE )(0x00504169 + 0) =         0x89   ;// mov dword ptr
	*(LPWORD )(0x00504169 + 1) = BSWAP16(0x4DB8);// [ebp - 0x48], ecx
	*(LPBYTE )(0x0050416C + 0) = JMP_REL32;
	*(LPDWORD)(0x0050416C + 1) = 0x00504266 - (0x0050416C + 1 + sizeof(DWORD));
	*(LPBYTE ) 0x00504171      = NOP;
	//   GetAddress(SSGS, tmpAE->GetOffsetCode(), 0)
	*(LPBYTE )(0x005041BB + 3) = 0x08;
	*(LPBYTE ) 0x005041C3      = PUSH_EBP;
	*(LPWORD )(0x005041C4 + 0) = BSWAP16(0xFF75);// push
	*(LPWORD )(0x005041C4 + 2) =         0xB8   ;// dword ptr [ebp - 0x48]
	*(LPWORD ) 0x005041C7      = PUSH_EBX;
	*(LPBYTE )(0x005041C8 + 0) = CALL_REL;
	*(LPDWORD)(0x005041C8 + 1) = (DWORD)TSSGCtrl_AddressAttributeFilter_GetAddress - (0x005041C8 + 1 + sizeof(DWORD));
	*(LPBYTE ) 0x005041CD      = PUSH_EDX;

	// TSSGCtrl::CheckFunnel
	*(LPDWORD)(0x00510169 + 1) = (DWORD)TSSGCtrl_CheckFunnel_GetAttribute - (0x00510169 + 1 + sizeof(DWORD));

	// TSSGCtrl::CheckIO_FEP
	*(LPDWORD)(0x00510F69 + 1) = (DWORD)TSSGCtrl_CheckIO_FEP_GetAttribute - (0x00510F69 + 1 + sizeof(DWORD));

	// TSSGCtrl::CheckIO_FEPDouble
	*(LPDWORD)(0x005110B1 + 1) = (DWORD)TSSGCtrl_CheckIO_FEPDouble_GetAttribute - (0x005110B1 + 1 + sizeof(DWORD));

	// THeapAdjustmentAttribute::~THeapAdjustmentAttribute
#ifdef map_string_quad_insert
	*(LPDWORD)(0x00523531 + 1) = (DWORD)THeapAdjustmentAttribute_dtor_heapMap - (0x00523531 + 1 + sizeof(DWORD));
#endif

	// THeapAdjustmentAttribute::IsEqual
	//   if (type == atSCOPE) return TRUE;
	*(LPBYTE )0x005235CA = 0x0052363A - (0x005235CA + sizeof(BYTE));
	*(LPBYTE )0x005235CB = 0x81;// cmp dword ptr
	*(LPBYTE )0x005235CC = 0x7B;// [ebx + ... 
	*(LPBYTE )0x005235CD = offsetof(THeapAdjustmentAttribute, type);
	*(LPDWORD)0x005235CE = atSCOPE;
	*(LPBYTE )0x005235D3 = 0x0052363E - (0x005235D3 + sizeof(BYTE));

	// TChildRWAttribute::IsEqual
	//   Compare 'prohibit' as DWORD
	*(LPBYTE )0x005268D1 = 0x8B;// al => eax
	*(LPBYTE )0x005268D4 = 0x3B;// al => eax

	// TReplaceAttribute::IsEqual
	//   if (displace != X->displace) return FALSE;
	*(LPBYTE )0x00527250 = JNZ_SHORT;
	*(LPWORD )0x00527251 = 0x0052726A - (0x00527251 + sizeof(BYTE));
	*(LPWORD )0x00527252 = BSWAP16 (0x8B47);// mov eax, dword ptr [edi + ...
	*(LPBYTE )0x00527254 = offsetof(TReplaceAttribute, displace);
	*(LPBYTE )0x00527255 =          0x8B   ;// mov ecx, dword ptr [ebp +   
	*(LPWORD )0x00527256 = BSWAP16 (0x4D0C);//                           0x0C]
	*(LPWORD )0x00527258 = BSWAP16 (0x8B51);// mov edx, dword ptr [ecx + ...
	*(LPBYTE )0x0052725A = offsetof(TReplaceAttribute, displace);
	*(LPBYTE )0x0052725B =          0x3B   ;// cmp eax, edx  
	*(LPWORD )0x0052725C = BSWAP16 (0xC2 << 8 | JNZ_SHORT);

	// TDirAttribute::IsEqual
	//   if (identity != X->identity) return FALSE;
	*(LPBYTE )0x00527403 = JNZ_SHORT;
	*(LPBYTE )0x00527404 = 0x00527415 - (0x00527404 + sizeof(BYTE));
	*(LPBYTE )0x00527405 = 0x8B;// mov ecx, 
	*(LPBYTE )0x00527406 = 0x48;// [eax + ...
	*(LPBYTE )0x00527407 = offsetof(TDirAttribute, identity);
	*(LPWORD )0x00527408 = BSWAP16(0x3B4A);// cmp ecx, [edx + ..
	*(LPBYTE )0x0052740A = offsetof(TDirAttribute, identity);
	*(LPBYTE )0x0052740B = JNZ_SHORT;
}
