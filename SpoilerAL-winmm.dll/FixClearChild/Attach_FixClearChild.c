#include <windows.h>
#include <dhcpsapi.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_operator.h"
#include "bcb6_std_stack.h"
#include "TMainForm.h"
#include "SSGSubjectProperty.h"
#include "SubjectStringOperator.h"

extern unsigned long __cdecl Parsing(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const string *Src, ...);
extern UINT  ExtensionTSSDir;
extern UINT  MultiStageAttrs;
extern DWORD RepeatDepth;
extern void __stdcall repeat_ReadSSRFile(
	TSSGCtrl     *this,
	stack        *ParentStack,
	LPVOID       ADJElem,
	const string *LineS,
	DWORD        RepeatIndex,
	DWORD        OuterRepeat,
	TSSGSubject  *SSGS);

static unsigned seqElement = MAXDWORD;

static __declspec(naked) uint64_t __cdecl TSSGAttributeSelector_StartElementCheck_new_attributeSetMap(size_t const __n)
{
	__asm {
#pragma region TSSGAttributeSelector.AESet
		mov  ecx, size bcb6_std_set_node
#if OPTIMIZE_ALLOCATOR
		call node_alloc_allocate
#else
		push ecx
		call node_alloc_allocate
		pop  ecx
#endif
		xor  edx, edx
		lea  ecx, [esi]TSSGAttributeSelector.AESet
		mov  [eax]bcb6_std_set_node._M_color , dl
		mov  [eax]bcb6_std_set_node._M_parent, edx
		mov  [eax]bcb6_std_set_node._M_left  , eax
		mov  [eax]bcb6_std_set_node._M_right , eax
		mov  [ecx]bcb6_std_set._M_header     , eax
		mov  [ecx]bcb6_std_set._M_node_count , edx
#pragma endregion
		xor  eax, eax
		mov  seqElement, eax
		mov  edx, [esi]TSSGAttributeSelector.attributeSetMap
		test edx, edx
		jnz  REVERT

		mov  ecx, dword ptr [esp + 4]
#if OPTIMIZE_ALLOCATOR
		call bcb6_operator_new
#else
		push ecx
		call bcb6_operator_new
		pop  ecx
#endif
		xor  edx, edx

	REVERT:
		rep ret
	}
}

static BOOL __fastcall TSSGAttributeSelector_EndElementCheck_delete_attributeSetMap(TSSGAttributeSelector *const this, BOOL const clear)
{// delete attributeSetMap if return TRUE
	static void(__cdecl *const tree_ptr_M_erase)(struct _Rb_tree *this, struct _Rb_tree_node *__x) = (void *)0x004D399C;

	if (this->nowAttributeList)
	{
		if (this->AESet._M_node_count)
		{
			tree_ptr_M_erase(&this->AESet, set_end(&this->AESet)->_M_parent);
			this->AESet._M_node_count = 0;
		}
		node_alloc_deallocate(
			set_end(&this->AESet)
#if !OPTIMIZE_ALLOCATOR
			, sizeof(bcb6_std_set_node)
#endif
		);
		set_end(&this->AESet) = NULL;
	}
	if (clear)
		return TRUE;
	else if (ExtensionTSSDir)
		return FALSE;
	else
	{
		unsigned discard = 0;
		for (list_iterator It = list_end(&this->allAtteributeVecList);
			 list_iterator_decrement(It) != list_end(&this->allAtteributeVecList);
			 )
		{
			vector *const AttrV = *(vector *const *)It->_M_data;
			if (AttrV->allocator_type[0])
			{
				It = list_erase(It);
				vector_dtor(AttrV);
				bcb6_operator_delete(AttrV);
				discard++;
			}
		}
		if (TMainForm_GetUserMode(MainForm) > 1)
		{
			EXTERN_C int __fastcall GuidePrint(const char *format, ...);

			GuidePrint("Discard vector<TSSGAttributeElement *>[%u]\n", discard);
		}
		return TRUE;
	}
}

static void *__cdecl TSSGAttributeSelector_AddElement_MakeOnlyOneAtteribute(TSSGAttributeSelector *const this, TAdjustmentAttribute *AElem)
{
	static void *(__cdecl *const TSSGAttributeSelector_MakeOnlyOneAtteribute)(TSSGAttributeSelector *, void *) = (void *)0x004D5764;

	AElem = TSSGAttributeSelector_MakeOnlyOneAtteribute(this, AElem);
	AElem->seqElement = TSSGAttributeElement_GetType(AElem) == atDEFINE
		? string_length(&((TDefineAttribute *)AElem)->inputCode)
		: seqElement++// renumbering when redeclared too
		;// store into allocator_type[1], except atSCOPE
	return AElem;
}

void __fastcall TSSGAttributeSelector_AESet_erase(TSSGAttributeSelector *const this, const TAdjustmentAttribute *const AElem)
{
	set_iterator const it = set_find(&this->AESet, (LPDWORD)&AElem);
	if (it != set_end(&this->AESet))
		node_alloc_deallocate(
			tree_erase_sans_delete(&this->AESet, it)
#if !OPTIMIZE_ALLOCATOR
			, sizeof(bcb6_std_set_node) + sizeof(DWORD)
#endif
		);
}

static void __fastcall TSSGAttributeSelector_PushElement_AESet_erase(
	TSSGAttributeSelector      *const this,
	TAdjustmentAttribute const *const AElem,
	list_iterator               const VIt)
{
	const TAdjustmentAttribute **const data = (void *)VIt->_M_data;
	TSSGAttributeSelector_AESet_erase(this, *data);
	*data = AElem;
}

static void __declspec(naked) TSSGAttributeSelector_PushElement_assign_AElem_into_VIt()
{
	__asm {
		mov edx, dword ptr [ebp + 0x0C]
		mov ecx, ebx
		jmp TSSGAttributeSelector_PushElement_AESet_erase
	}
}

static DWORD_DWORD __fastcall TSSGAttributeSelector_PopElement_AESet_erase(TSSGAttributeSelector *const this, const deque *const second)
{
	TSSGAttributeSelector_AESet_erase(this, *deque_back(second, void *));
	return (DWORD_DWORD) { (DWORD)second, (DWORD)deque_end(second)._M_cur };
}

static list_iterator __declspec(naked) TSSGAttributeSelector_PopElement_MIt_second_pop()
{
	__asm {// MIt->second
		lea edx, [eax + size DOUBLE]bcb6_std_map_node._M_value_field
		mov ecx, dword ptr [ebp + 8]
		jmp TSSGAttributeSelector_PopElement_AESet_erase
	}
}

static int __cdecl cmpElement(LPCVOID const A, LPCVOID const B)
{
	const TAdjustmentAttribute *const a = *(TAdjustmentAttribute **)A;
	const TAdjustmentAttribute *const b = *(TAdjustmentAttribute **)B;
	signed diff = a->type - b->type;
	return diff ? diff : TSSGAttributeElement_GetType(a) == atDEFINE
		? strcmp(a->c_str, b->c_str)
		: a->seqElement - b->seqElement
		;
}

#pragma intrinsic(_BitScanForward)
void __stdcall TSSGAttributeSelector_MakeNowAttributeVec_attributeSetMap_insert(const struct
{
	map_iterator first;
	bool         second;
} *const retVal, const map *const attributeSetMap, const struct
{
	set     AESet;
	vector *NewVec;
} *const __v, const list_iterator It)
{
	COORD coords[0x20] = { { 0, 0 } };
	vector const index = { coords, coords + _countof(coords) - 1, ._M_end_of_storage = coords };

	vector *const NewVec = __v->NewVec;
	qsort(vector_begin(NewVec), vector_size_by_type(NewVec, void *), sizeof(void *), cmpElement);

	for (TSSGAttributeElement **VIt = vector_begin(NewVec);
		 VIt < (TSSGAttributeElement **)vector_end(NewVec);
		 VIt++)
	{
		DWORD Index;
		_BitScanForward(&Index, TSSGAttributeElement_GetType(*VIt));
		if (coords[Index].X++ == 0)
			coords[Index].Y = VIt - (TSSGAttributeElement **)vector_begin(NewVec);
	}

	NewVec->allocator_type[0] = retVal->first;
	NewVec->allocator_type[1] = NULL;
	NewVec->lutIndex = SubjectStringTable_insert((string *)&index);
}

static_assert(sizeof(CONSOLE_FONT_INFO) <= 8, "CONSOLE_FONT_INFO is greater than 8 bytes.");
CONSOLE_FONT_INFO __fastcall TSSGAttributeElement_GetViaCoord(AeType const Type, const vector *const AttrV)
{
	TSSGAttributeElement const *AElem;
	DWORD Index;
	_BitScanForward(&Index, Type);
	COORD coord = vector_type_at(&vector_at(&SubjectStringTable_array, AttrV->lutIndex), COORD, Index);
	return coord.X ?// Give consideration to had been added attribute by TMainForm::AutoDialogAdjustment.
		(CONSOLE_FONT_INFO) { vector_type_at(AttrV, DWORD, coord.Y), coord } :
		TSSGAttributeElement_GetType(AElem = vector_type_at(
			AttrV,
			TSSGAttributeElement *,
			Index = vector_size_by_type(AttrV, void *) - 1
		)) != Type ? (CONSOLE_FONT_INFO) { 0, coord } : (CONSOLE_FONT_INFO) { (DWORD)AElem, { 1, (WORD)Index } };
}

static void __fastcall TSSDir_GetSubjectVec_onOpen(TSSGSubject *const SSGS, TSSGCtrl *const SSGC)
{
	static void(__cdecl *const stack_ptr_ctor)(pdeque this, rsize_t __n) = (void *)0x004E49B0;
	static void(__cdecl *const stack_ptr_dtor)(pdeque this, BOOL delete) = (void *)0x004E4FA8;
#define ABBREV_SELECT 1
	string Token;
	vector_string tmpV = { NULL };
	const string *Code = SubjectStringTable_GetString(&SSGS->code);
	if (string_empty(Code))
		return;
	else if (TStringDivision_List(
		&SSGC->strD, Code, *string_ctor_assign_char(&Token, ','), &tmpV, FALSE
#if 0
		| dtNEST
#endif
	) > 1)
	{
		TSSDir *const this = (void *)SSGS;
		vector *const attr = TSSGSubject_GetAttribute(SSGS);
		AeType const multi = atREPLACE | MultiStageAttrs | atENABLED
#if !ABBREV_SELECT
			| atDEFINE
#endif
			| atSCOPE | atFORMAT;
		if (SSGS->stable != MAXWORD)
		{
			TSSGSubject **discard = &vector_at(&this->childVec, SSGS->stable);
			for (TSSGSubject **it = discard; it < vector_end(&this->childVec); it++)
			{
				if ((*it)->isLocked)
					TSSGCtrl_SetLock(SSGC, FALSE, *it, NULL);
				delete_TSSGSubject(*it);
			}
			vector_end(&this->childVec) = discard;
		}
		else
			SSGS->stable = (WORD)vector_size(&this->childVec);
		TSSGAttributeSelector_StartElementCheck(TSSGCtrl_GetAttributeSelector(SSGC));
		for (TAdjustmentAttribute *const *pos = &vector_type_at(attr, TAdjustmentAttribute *, 1);
			 pos < (TAdjustmentAttribute **)vector_end(attr);
			 pos++)
		{
			TAdjustmentAttribute *const AElem = *pos;
#if ABBREV_SELECT
			TSSGAttributeSelector_PushStack(TSSGCtrl_GetAttributeSelector(SSGC), AElem);
			list_push_back(TSSGCtrl_GetAttributeSelector(SSGC)->nowAttributeList, pos);
			if (TSSGAttributeElement_GetType(AElem) & multi && AElem->seqElement >= seqElement)
				seqElement = AElem->seqElement + 1;
#else
			for (list_iterator LIt = list_begin(&TSSGCtrl_GetAttributeSelector(SSGC)->allAtteributeList);
				 LIt != list_end(&TSSGCtrl_GetAttributeSelector(SSGC)->allAtteributeList);
				 list_iterator_increment(LIt))
			{
				if (TSSGAttributeElement_IsEqual(AElem, *(TSSGAttributeElement **)LIt->_M_data))
				{
					list_erase(LIt);// prevent delete in TSSGAttributeSelector::MakeOnlyOneAtteribute
					break;
				}
			}
			if (TSSGAttributeElement_GetType(AElem) & multi)
				TSSGAttributeSelector_AddElement (TSSGCtrl_GetAttributeSelector(SSGC), AElem);
			else
				TSSGAttributeSelector_PushElement(TSSGCtrl_GetAttributeSelector(SSGC), AElem);
#endif
		}
		{
			TDirAttribute * const NewAElem = new_TDirAttribute();
			TDirAttribute_Setting(NewAElem, TSSGCtrl_GetDirLevel(SSGC, SSGS) + 1);
			TSSGAttributeSelector_PushElement(TSSGCtrl_GetAttributeSelector(SSGC), NewAElem);
		}
		{
			stack_ptr ParentStack;
			const TSSGSubjectProperty *const prop = GetSubjectProperty(SSGS);

			stack_ptr_ctor(&ParentStack, 0);
			stack_ptr_push(&ParentStack, &SSGS);
			if (prop)
			{
				RepeatDepth = prop->RepeatDepth;
				repeat_ReadSSRFile(SSGC, &ParentStack, NULL, Code, prop->RepeatIndex, SSGS->propertyIndex, SSGS);
			}
			else
			{
				RepeatDepth = 0;
				repeat_ReadSSRFile(SSGC, &ParentStack, NULL, Code, 0, MAXDWORD, SSGS);
			}
			stack_ptr_dtor(&ParentStack, FALSE);
		}
		TSSGAttributeSelector_EndElementCheck(TSSGCtrl_GetAttributeSelector(SSGC));
	}
	else
		Parsing(SSGC, SSGS, Code, 0);
	vector_string_dtor(&tmpV);
}

static void __declspec(naked) TSSGCtrl_ChangeDirectorySubject_GetSubjectVec(TSSDir *this, long Mode, pvector_PTSSGSubject Vec)
{
	static DWORD const TSSDir_GetSubjectVec = 0x004C2D64;
	__asm {// ecx is TSSDir* already
		cmp  ExtensionTSSDir, 1
		jne  PASS
		mov  edx, edi
		call TSSDir_GetSubjectVec_onOpen
	PASS:
		jmp  dword ptr [TSSDir_GetSubjectVec]
		ud2
	}
}

#define INC_ECX   (BYTE)0x41
#define PUSH_EAX  (BYTE)0x50
#define JNZ_SHORT (BYTE)0x75
#define NOP       (BYTE)0x90
#define NOP_X8          0x0000000000841F0Full
#define JECXZ     (BYTE)0xE3
#define CALL_REL  (BYTE)0xE8
#define JMP_REL32 (BYTE)0xE9
#define JMP_SHORT (BYTE)0xEB


EXTERN_C void __cdecl Attach_FixClearChild()
{
	// TSSDir::ClearChild
	*(LPBYTE )0x004C2CCC = JMP_REL32;
	*(LPDWORD)0x004C2CCD = (DWORD)TSSDir_ClearChild - (0x004C2CCD + sizeof(DWORD));
	*(LPBYTE )0x004C2CD1 = NOP;

	// TSSGAttributeSelector::StartElementCheck
	//   attributeSetMap!=NULL => nowAttributeList!=NULL
	*(LPBYTE )(0x004D2D6D + 2) = offsetof(TSSGAttributeSelector, nowAttributeList);

	*(LPDWORD)(0x004D2D85 + 1) = (DWORD)TSSGAttributeSelector_StartElementCheck_new_attributeSetMap - (0x004D2D85 + 1 + sizeof(DWORD));
	*(LPDWORD)(0x004D2D90 + 2) = 0x004D2E25 - (0x004D2D90 + 2 + sizeof(DWORD));// EAX is NULL then use EDX

	// TSSGAttributeSelector::EndElementCheck
	//   retain attributeSetMap if caller isn't TSSGAttributeSelector::Clear
	*(LPWORD )0x004D3686 = BSWAP16(0x8BCB    );// mov    ecx, ebx
	*(LPDWORD)0x004D3688 = BSWAP32(0x8B45042D);// mov    eax, dword ptr [_AddressOfReturnAddress]
	*(LPDWORD)0x004D368C =         0x004D2D15 ;// sub    eax, TSSGAttributeSelector::Clear
	*(LPDWORD)0x004D3690 = BSWAP32(0xF7D81BD2);// neg    eax; sbb edx, edx
	*(LPDWORD)0x004D3694 = BSWAP32(0x83C201E8);// add    edx, 1
	*(LPDWORD)0x004D3698 = (DWORD)TSSGAttributeSelector_EndElementCheck_delete_attributeSetMap - (0x004D3698 + sizeof(DWORD));
	*(LPDWORD)0x004D369C = BSWAP32(0x83632800);// and    dword ptr [nowAttributeVec], 0
	*(LPWORD )0x004D36A0 = BSWAP16(0x85C0    );// test   eax, eax
	*(LPBYTE )0x004D36A3 =         0x4B       ;// eax => ecx
	*(LPBYTE )0x004D36A6 =         0x4D       ;// eax => ecx
	*(LPDWORD)0x004D36A8 = BSWAP32(0x0F44C890);// cmovz  ecx, eax
	*(LPBYTE )0x004D36AC = JECXZ;
	*(LPBYTE )0x004D36AD = 0x004D3722 - (0x004D36AD + sizeof(BYTE));

	// TSSGAttributeSelector::PushElement
	*(LPBYTE ) 0x004D3D2B      = PUSH_EAX;
	*(LPBYTE )(0x004D3D2C + 0) = CALL_REL;
	*(LPDWORD)(0x004D3D2C + 1) = (DWORD)TSSGAttributeSelector_PushElement_assign_AElem_into_VIt - (0x004D3D2C + 1 + sizeof(DWORD));

	*(UINT64 *)0x004D3D4D = NOP_X8;
	*(UINT64 *)0x004D3D5D = NOP_X8;
	*(LPBYTE )(0x004D3D65 + 0) = JNZ_SHORT;

	// TSSGAttributeSelector::AddElement
	*(LPDWORD)(0x004D3DC9 + 1) = (DWORD)TSSGAttributeSelector_AddElement_MakeOnlyOneAtteribute - (0x004D3DC9 + 1 + sizeof(DWORD));

	// TSSGAttributeSelector::PopElement
	*(LPBYTE ) 0x004D3EE0      = NOP;
	*(LPBYTE )(0x004D3EE1 + 0) = CALL_REL;
	*(LPDWORD)(0x004D3EE1 + 1) = (DWORD)TSSGAttributeSelector_PopElement_MIt_second_pop - (0x004D3EE1 + 1 + sizeof(DWORD));

	*(UINT64 *)0x004D3FEA = NOP_X8;

	// TSSGAttributeSelector::EraseElement
	//   reverse iterate
	*(LPWORD )(0x004D414F + 1) = BSWAP16(0x4204);// mov eax, nowAttributeList->_M_node->_M_prev
	*(LPWORD )(0x004D4152 + 0) = BSWAP16(0x8945);// mov SIt, eax
	*(LPWORD )(0x004D4152 + 2) = BSWAP16(0xF0 << 8 | NOP);
	*(LPBYTE )(0x004D4156 + 1) =         0x55;// eax => edx

	*(LPWORD )(0x004D41CD + 1) = BSWAP16(0x5104);// mov eax, SIt->_M_prev
	*(LPBYTE ) 0x004D41D0      = NOP;
	*(LPBYTE )(0x004D41D4 + 1) = 0x5D;// eax => ebx

	*(UINT64 *)0x004D41DD = NOP_X8;

	*(LPBYTE )(0x004D41A9 + 0) = CALL_REL;
	*(LPDWORD)(0x004D41A9 + 1) = (DWORD)TSSGAttributeSelector_PopElement_MIt_second_pop - (0x004D41A9 + 1 + sizeof(DWORD));
	*(LPBYTE ) 0x004D41AE      = NOP;

	// TSSGAttributeSelector::ToRootElement
	*(LPBYTE )(0x004D42E3 + 0) = CALL_REL;
	*(LPDWORD)(0x004D42E3 + 1) = (DWORD)TSSGAttributeSelector_PopElement_MIt_second_pop - (0x004D42E3 + 1 + sizeof(DWORD));
	*(LPBYTE ) 0x004D42E8      = NOP;

	*(UINT64 *)0x004D438F = NOP_X8;

	// TSSGAttributeSelector::MakeOnlyOneAtteribute
	*(UINT64 *)0x004D57F1 = NOP_X8;

	// TSSGAttributeSelector::MakeNowAttributeVec
	//   AESet = this->AESet;
	*(LPBYTE )(0x004D5891 + 0) =         0x8D;// lea eax, [esi]TSSGAttributeSelector.AESet
	*(LPWORD )(0x004D5891 + 1) = BSWAP16(0x46 << 8 | offsetof(TSSGAttributeSelector, AESet));

	*(LPDWORD) 0x004D58A0      = BSWAP32(0x8D55E0F3);// lea    edx, AESet
	*(LPDWORD) 0x004D58A4      = BSWAP32(0x0F6F00F3);// movdqu xmm0, xmmword ptr [eax]
	*(LPWORD ) 0x004D58A8      = BSWAP16(0x0F7F    );// movdqu xmmword ptr [edx], xmm0
	*(LPBYTE ) 0x004D58AA      =         0x02;
	*(LPWORD ) 0x004D58AE      = INC_ECX << 8 | INC_ECX;
	*(LPDWORD) 0x004D58B0      = BSWAP32(0xF30F6F04);// movdqu xmm0, xmmword ptr [eax + ecx*8]
	*(LPDWORD) 0x004D58B4      = BSWAP32(0xC8F30F7F);// movdqu xmmword ptr [edx + ecx*8], xmm0
	*(LPWORD ) 0x004D58B8      = BSWAP16(0x04CA);
	*(LPBYTE ) 0x004D58BA      = JMP_SHORT;
	*(LPBYTE ) 0x004D58BB      = 0x004D58D6 - (0x004D58BB + sizeof(BYTE));

	*(UINT64 *)0x004D597E = NOP_X8;
	*(UINT64 *)0x004D59CE = NOP_X8;

	//   omit dtor AESet
	*(LPBYTE )(0x004D59F2 + 0) = JMP_SHORT;
	//   this->AESet = AESet;
	*(LPBYTE ) 0x004D5A2D = NOP;
	*(LPWORD ) 0x004D5A2E = BSWAP16(0x8D56    );// lea edx, [esi]TSSGAttributeSelector.AESet
	*(LPBYTE ) 0x004D5A30 = offsetof(TSSGAttributeSelector, AESet);
	*(LPBYTE ) 0x004D5A37 =         0x89       ;// mov [edx]bcb6_std_set._M_header, ecx
	*(LPDWORD) 0x004D5A38 = BSWAP32(0x4A088B4D);// mov ecx, AESet._M_node_count
	*(LPDWORD) 0x004D5A3C = BSWAP32(0xF0894A10);// mov [edx]bcb6_std_set._M_node_count, ecx

	*(UINT64 *)0x004D5C60 = NOP_X8;

	// push allAtteributeVecList iterator
	*(LPBYTE )(0x004D5CF1 + 0) =         0x83   ;// add  dword ptr [ebx + 0x1C], 2
	*(LPWORD )(0x004D5CF1 + 3) = BSWAP16(0x0257);// push edi
	*(LPBYTE ) 0x004D5CF6      = NOP;

	*(LPBYTE )(0x004D5DFB + 0) = CALL_REL;
	*(LPDWORD)(0x004D5DFB + 1) = (DWORD)TSSGAttributeSelector_MakeNowAttributeVec_attributeSetMap_insert - (0x004D5DFB + 1 + sizeof(DWORD));
	*(LPBYTE ) 0x004D5E00      = NOP;
	*(LPBYTE )(0x004D5E01 + 0) =         0x83   ;// sub
	*(LPBYTE )(0x004D5E01 + 1) =         0x6B   ;// dword ptr
	*(LPWORD )(0x004D5E01 + 3) = BSWAP16(0x0366);// [ebx + ...], 3
	*(LPBYTE ) 0x004D5E06      = NOP;

	//   omit dtor AESet
	*(LPBYTE )(0x004D5E6E + 0) = JMP_SHORT;
	//   this->AESet = AESet;
	*(LPBYTE ) 0x004D5E9E = NOP;
	*(LPBYTE ) 0x004D5E9F =         0x8D       ;// lea edx, [esi]TSSGAttributeSelector.AESet
	*(LPDWORD) 0x004D5EA0 = BSWAP32(0x56 << 24 | offsetof(TSSGAttributeSelector, AESet) << 16 | 0x897A);
	*(LPDWORD) 0x004D5EA4 = BSWAP32(0x088B4DF0);// mov [edx]bcb6_std_set._M_header, edi
	*(LPWORD ) 0x004D5EA8 = BSWAP16(0x894A    );// mov ecx, AESet._M_node_count
	*(LPBYTE ) 0x004D5EAA =         0x10       ;// mov [edx]bcb6_std_set._M_node_count, ecx

	// TSSGCtrl::ReadADJFile
	//   attributeSelector.IsElementCheckMode()
	*(LPBYTE )(0x004FD49A + 2) = offsetof(TSSGCtrl, attributeSelector.nowAttributeList);

	// TSSGCtrl::ChangeDirectorySubject
	*(LPDWORD)0x00502A40 = (DWORD)TSSGCtrl_ChangeDirectorySubject_GetSubjectVec - (0x00502A40 + sizeof(DWORD));
}
