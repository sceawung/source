.686
.model flat

if 1
_MSC_VER equ 1
endif

public _wmemset

extern memsetAVX512BW_entry: near
extern memsetAVX512F_entry: near
extern memsetAVX_entry: near
extern memsetSSE2_entry: near
extern GetMemsetCacheLimit: near
extern _InstructionSet: near
extern _Store256BitIsFaster: near

.data

wmemsetDispatch dd wmemsetCPUDispatch
wmemsetEntry    dd ?

.code

; Function entry:
align 16
_wmemset proc near
	jmp     dword ptr [wmemsetDispatch]                 ; Go to appropriate version, depending on instruction set
_wmemset endp

; AVX512BW version
; AVX512F version
; AVX version
; SSE2 Version
align 16
wmemsetANY proc near
	mov     ecx, dword ptr [esp + 12]                   ; load count
	mov     eax, dword ptr [esp + 4]                    ; load dest
	add     ecx, ecx                                    ; count * 2 = number of bytes
	jnc     has_not_carry                               ; jump if has not carry
	int     3                                           ; break point

	align   16
has_not_carry:
	mov     edx, eax                                    ; copy dest
	jz      count_equal_zero                            ; jump if count == 0
	test    eax, 1                                      ; is aligned to word?
	jnz     unaligned_word                              ; jump if not aligned
	movzx   eax, word ptr [esp + 8]                     ; load c
	imul    eax, 00010001H                              ; broadcast c into all words of eax
	jmp     dword ptr [wmemsetEntry]

	align   16
unaligned_word:
	sub     ecx, 2                                      ; decrease count
	jz      store_one_word                              ; jump if count == 0
	movzx   eax, word ptr [esp + 8]                     ; load c
	mov     byte ptr [edx], al                          ; store first byte
	inc     edx                                         ; dest = dest + 1 byte
	mov     byte ptr [edx + ecx], ah                    ; store last byte
	imul    eax, 00010001H                              ; broadcast c into all words of eax
	ror     eax, 8                                      ; rotate dword
	jmp     dword ptr [wmemsetEntry]

	align   16
store_one_word:
	mov     cx, word ptr [esp + 8]                      ; load c
	mov     word ptr [eax], cx                          ; store one word
count_equal_zero:
	db      0F3h                                        ; rep
	ret                                                 ; __cdecl return
wmemsetANY endp

; 80386 Version
align 16
wmemset386 proc near
	mov     eax, dword ptr [esp + 4]                    ; load dest
	xor     edx, edx                                    ; load c
	mov     dx, word ptr [esp + 8]                      ;
	mov     ecx, dword ptr [esp + 12]                   ; load count
	test    ecx, ecx                                    ; if count == 0?
	jz      count_equal_zero                            ; jump if count == 0
	push    edi                                         ; preserve edi
	mov     edi, eax                                    ; copy dest
	test    eax, 1                                      ; is aligned to word?
	jnz     unaligned_word                              ; jump if not aligned
	mov     eax, edx                                    ; edx = 0/0/H/L (L = low, H = high)
	shl     eax, 16                                     ; eax = H/L/0/0
	jmp     merge_dword

	align   16
unaligned_word:
	mov     byte ptr [eax], dl                          ; store first byte
	dec     ecx                                         ; decrease count
	mov     byte ptr [eax + ecx * 2 + 1], dh            ; store last byte
	jz      epilog                                      ; jump if count == 0
	mov     eax, edx                                    ; copy c
	inc     edi                                         ; dest = dest + 1 byte
	ror     edx, 8                                      ; edx = L/0/0/H (L = low, H = high)
	shl     eax, 8                                      ; eax = 0/H/L/0
	jmp     merge_dword

	align   16
merge_dword:
	or      eax, edx                                    ; merge dword
	mov     edx, edi                                    ; copy dest
	and     edx, 2                                      ; is aligned to dword?
	jz      repeat_store_dwords                         ; jump if aligned
	mov     word ptr [edi], ax                          ; store one word
	add     edi, 2                                      ; increase dest
	dec     ecx                                         ; decrease count
	jz      return_dest                                 ; jump if count == 0
repeat_store_dwords:
	mov     edx, ecx                                    ; copy count
	shr     ecx, 1                                      ; number of dword = count / 2
	jz      remaining_word                              ; jump if count / 2 = 0
if not _MSC_VER
	cld                                                 ; clear direction flag
endif
	rep     stosd                                       ; store 4 bytes at a time
	and     edx, 1                                      ; has remaining word?
	jz      return_dest                                 ; jump if has not remaining word
remaining_word:
	mov     word ptr [edi], ax                          ; store remaining word
return_dest:
	mov     eax, dword ptr [esp + 8]                    ; return dest
epilog:
	pop     edi                                         ; restore edi
count_equal_zero:
	db      0F3h                                        ; rep
	ret                                                 ; __cdecl return
wmemset386 endp

; CPU dispatching for wmemset. This is executed only once
align 16
wmemsetCPUDispatch proc near
	pushad
	call    GetMemsetCacheLimit                         ; calculate cache limit
	call    _InstructionSet                             ; get supported instruction set
	mov     ebx, eax
	; Point to generic version of wmemset
	mov     dword ptr [wmemsetDispatch], offset wmemset386
	cmp     eax, 4                                      ; check SSE2
	jb      Q100
	; SSE2 supported
	; Point to SSE2 version of wmemset
	mov     dword ptr [wmemsetDispatch], offset wmemsetANY
	mov     dword ptr [wmemsetEntry], offset memsetSSE2_entry
	call    _Store256BitIsFaster                        ; check if 256-bit stores are available and faster
	test    eax, eax
	jz      Q100
	mov     dword ptr [wmemsetEntry], offset memsetAVX_entry
	cmp     ebx, 15
	jb      Q100
	mov     dword ptr [wmemsetEntry], offset memsetAVX512F_entry
	cmp     ebx, 16
	jb      Q100
	mov     dword ptr [wmemsetEntry], offset memsetAVX512BW_entry

Q100:
	popad
	; Continue in appropriate version of wmemset
	jmp     dword ptr [wmemsetDispatch]
wmemsetCPUDispatch endp

end
