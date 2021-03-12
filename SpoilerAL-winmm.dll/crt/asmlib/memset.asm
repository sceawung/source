.686
.xmm
.model flat

if 1
_MSC_VER equ 1
endif

public _memset
public memsetAVX512BW_entry
public memsetAVX512F_entry
public memsetAVX_entry
public memsetSSE2_entry
public GetMemsetCacheLimit

extern _Store256BitIsFaster: near
extern _InstructionSet: near
extern _DataCacheSize: near

.const

; Jump table for count from 0 to 16:
MemsetJTab label dword
	dd M00, M01, M02, M03, M04, M05, M06, M07
	dd M08, M09, M10, M11, M12, M13, M14, M15, M16

.data

memsetDispatch dd memsetCPUDispatch

MemsetCacheLimit dd 0

.code

; Define return from this function
RETURNM macro
	mov     eax, dword ptr [esp + 4]                    ; return dest
	ret
	align   16
endm

; Function entry:
align 16
_memset proc near
	jmp     dword ptr [memsetDispatch]                  ; Go to appropriate version, depending on instruction set
_memset endp

; AVX512BW version. Use zmm register
align 16
memsetAVX512BW proc near
	mov     edx, dword ptr [esp + 4]                    ; dest
	movzx   eax, byte ptr [esp + 8]                     ; c
	imul    eax, 01010101H                              ; Broadcast c into all bytes of eax
	mov     ecx, dword ptr [esp + 12]                   ; count

memsetAVX512BW_entry label near
	push    edi

	mov     edi, edx                                    ; save dest
	vpbroadcastd zmm0, eax                              ; Broadcast further into 64 bytes

if 0
	cmp     ecx, 40H
	jbe     L520
	cmp     ecx, 80H
	jbe     L500                                        ; Use simpler code if count <= 128

else
	cmp     ecx, 80H
	ja      L050                                        ; jump if count > 128
	cmp     ecx, 40H
	ja      L500                                        ; jump if count > 64, use simpler code
	test    ecx, ecx
	jnz     L520                                        ; jump if count != 0
	vzeroupper
	mov     eax, edi                                    ; return dest
	pop     edi
	ret

	align   16
endif
L050 label near
	; Common code for memsetAVX512BW and memsetAVX512F:

	; count > 80H
	; store first 40H bytes
	vmovdqu64 zmmword ptr [edx], zmm0

	; find first 40H boundary
	add     edx, 40H
	and     edx, -40H

	; find last 40H boundary
	lea     eax, [edi + ecx]
	and     eax, -40H
	sub     edx, eax                                    ; negative count from last 40H boundary
	; Check if count very big
	cmp     ecx, dword ptr [MemsetCacheLimit]
	ja      L200                                        ; Use non-temporal store if count > MemsetCacheLimit

	align   16
L100:
	; main loop, aligned
	vmovdqa64 zmmword ptr [eax + edx], zmm0
	add     edx, 40H
	jnz     L100

L110:
	; remaining 0 - 3FH bytes
	; overlap previous write
	vmovdqu64 zmmword ptr [edi + ecx - 40H], zmm0

	vzeroupper                                          ; is this needed?
	mov     eax, edi                                    ; return dest
	pop     edi
	ret

	align   16
L200:
	; loop with non-temporal stores
	vmovntdq zmmword ptr [eax + edx], zmm0
	add     edx, 40H
	jnz     L200
	sfence
	jmp     L110

	align   16
	; short versions, memsetAVX512BW only:
L500:
	; count = 41H - 80H
	vmovdqu64 zmmword ptr [edx], zmm0
	add     edx, 40H
	sub     ecx, 40H

L520:
	; count = 00H - 40H
	or      eax, -1                                     ; if count = 1-31: |  if count = 32-63:
	bzhi    eax, eax, ecx                               ; -----------------|--------------------
	kmovd   k1, eax                                     ;       count 1's  |  all 1's
	xor     eax, eax                                    ;
	sub     ecx, 32                                     ;
	cmovb   ecx, eax                                    ;               0  |  count-32
	dec     eax                                         ;
	bzhi    eax, eax, ecx                               ;
	kmovd   k2, eax                                     ;               0  |  count-32 1's
	kunpckdq k3, k2, k1                                 ; low 32 bits from k1, high 32 bits from k2. total = count 1's
	vmovdqu8 zmmword ptr [edx]{k3}, zmm0
	vzeroupper
	mov     eax, edi                                    ; return dest
	pop     edi
	ret
memsetAVX512BW endp

; AVX512F version
align 16
memsetAVX512F proc near
	mov     edx, dword ptr [esp + 4]                    ; dest
	movzx   eax, byte ptr [esp + 8]                     ; c
	mov     ecx, dword ptr [esp + 12]                   ; count
	imul    eax, 01010101H                              ; Broadcast c into all bytes of eax

memsetAVX512F_entry label near
	cmp     ecx, 80H
	jbe     B010                                        ; Use memsetAVX code if count <= 128
	push    edi
	mov     edi, edx                                    ; save dest
	vpbroadcastd zmm0, eax                              ; Broadcast further into 64 bytes
	jmp     L050                                        ; Use preceding code
memsetAVX512F endp

; AVX version. Use ymm register
align 16
memsetAVX proc near
	mov     edx, dword ptr [esp + 4]                    ; dest
	movzx   eax, byte ptr [esp + 8]                     ; c
	mov     ecx, dword ptr [esp + 12]                   ; count
	imul    eax, 01010101H                              ; Broadcast c into all bytes of eax

memsetAVX_entry label near
B010 label near
	; entry from AVX512F version
	cmp     ecx, 16
	ja      B100

B050 label near
	; count <= 16, both SSE2 and AVX version
	jmp     dword ptr [MemsetJTab + ecx * 4]

	align   16
; Separate code for each count from 0 to 16:
M16 label near
	mov     dword ptr [edx + 12], eax
M12 label near
	mov     dword ptr [edx + 8], eax
M08 label near
	mov     dword ptr [edx + 4], eax
M04 label near
	mov     dword ptr [edx], eax
M00 label near
	RETURNM

M15 label near
	mov     dword ptr [edx + 11], eax
M11 label near
	mov     dword ptr [edx + 7], eax
M07 label near
	mov     dword ptr [edx + 3], eax
M03 label near
	mov     word ptr [edx + 1], ax
M01 label near
	mov     byte ptr [edx], al
	RETURNM

M14 label near
	mov     dword ptr [edx + 10], eax
M10 label near
	mov     dword ptr [edx + 6], eax
M06 label near
	mov     dword ptr [edx + 2], eax
M02 label near
	mov     word ptr [edx], ax
	RETURNM

M13 label near
	mov     dword ptr [edx + 9], eax
M09 label near
	mov     dword ptr [edx + 5], eax
M05 label near
	mov     dword ptr [edx + 1], eax
	mov     byte ptr [edx], al
	RETURNM

B100:
	; count > 16.
	movd    xmm0, eax
	pshufd  xmm0, xmm0, 0                               ; Broadcast c into all bytes of xmm0
	lea     eax, [edx + ecx]                            ; point to end

	cmp     ecx, 20H
	jbe     K600                                        ; faster to use xmm registers if small

	; Store the first possibly unaligned 16 bytes
	; It is faster to always write 16 bytes, possibly overlapping
	; with the subsequent regular part, than to make possibly mispredicted
	; branches depending on the size of the first part.
	movups  oword ptr [edx], xmm0

	; store another 16 bytes, aligned
	add     edx, 10H
	and     edx, -10H
	movaps  oword ptr [edx], xmm0

	; go to next 32 bytes boundary
	add     edx, 10H
	and     edx, -20H

	; Check if count very big
	cmp     ecx, dword ptr [MemsetCacheLimit]
	ja      K300                                        ; Use non-temporal store if count > MemsetCacheLimit

	; find last 32 bytes boundary
	mov     ecx, eax
	and     ecx, -20H

	; - size of 32-bytes blocks
	sub     edx, ecx
	jnb     K200                                        ; Jump if not negative

	; extend value to 256 bits
	vinsertf128 ymm0,ymm0,xmm0,1

	align   16
K100:
	; Loop through 32-bytes blocks
	; ecx = end of 32-bytes blocks part
	; edx = negative index from the end, counting up to zero
	vmovaps ymmword ptr [ecx + edx], ymm0
	add     edx, 20H
	jnz     K100
	vzeroupper

K200:
	; the last part from ecx to eax is < 32 bytes. write last 32 bytes with overlap
	movups  xmmword ptr [eax - 20H], xmm0
	movups  xmmword ptr [eax - 10H], xmm0
	RETURNM

K300:
	; Use non-temporal moves, same code as above:

	; find last 32 bytes boundary
	mov     ecx, eax
	and     ecx, -20H

	; - size of 32-bytes blocks
	sub     edx, ecx
	jnb     K500                                        ; Jump if not negative

	; extend value to 256 bits
	vinsertf128 ymm0,ymm0,xmm0,1

	align   16
K400:
	; Loop through 32-bytes blocks
	; ecx = end of 32-bytes blocks part
	; edx = negative index from the end, counting up to zero
	vmovntps ymmword ptr [ecx + edx], ymm0
	add     edx, 20H
	jnz     K400
	sfence
	vzeroupper

K500:
	; the last part from ecx to eax is < 32 bytes. write last 32 bytes with overlap
	movups  xmmword ptr [eax - 20H], xmm0
	movups  xmmword ptr [eax - 10H], xmm0
	RETURNM

K600:
	; 16 < count <= 32
	movups  xmmword ptr [edx], xmm0
	movups  xmmword ptr [eax - 10H], xmm0
	RETURNM
memsetAVX endp

; SSE2 Version
align 16
memsetSSE2 proc near
	mov     edx, dword ptr [esp + 4]                    ; dest
	movzx   eax, byte ptr [esp + 8]                     ; c
	mov     ecx, dword ptr [esp + 12]                   ; count
	imul    eax, 01010101H                              ; Broadcast c into all bytes of eax

memsetSSE2_entry label near
	cmp     ecx, 16
	jna     B050                                        ; small counts: same as AVX version
if 1
	push    ecx                                         ; count
	push    edx                                         ; dest
endif
	movd    xmm0, eax
	pshufd  xmm0, xmm0, 0                               ; Broadcast c into all bytes of xmm0

	; Store the first unaligned part.
	; The size of this part is 1 - 16 bytes.
	; It is faster to always write 16 bytes, possibly overlapping
	; with the subsequent regular part, than to make possibly mispredicted
	; branches depending on the size of the first part.
	movq    qword ptr [edx], xmm0
	movq    qword ptr [edx + 8], xmm0

	; Check if count very big
	cmp     ecx, dword ptr [MemsetCacheLimit]
	ja      M500                                        ; Use non-temporal store if count > MemsetCacheLimit

	; Point to end of regular part:
	; Round down dest+count to nearest preceding 16-bytes boundary
	lea     ecx, [edx + ecx - 1]
	and     ecx, -10H

	; Point to start of regular part:
	; Round up dest to next 16-bytes boundary
	add     edx, 10H
	and     edx, -10H

	; -(size of regular part)
	sub     edx, ecx
	jnb     M300                                        ; Jump if not negative

	align   16
M200:
	; Loop through regular part
	; ecx = end of regular part
	; edx = negative index from the end, counting up to zero
	movdqa  xmmword ptr [ecx + edx], xmm0
	add     edx, 10H
	jnz     M200

M300:
	; Do the last irregular part
	; The size of this part is 1 - 16 bytes.
	; It is faster to always write 16 bytes, possibly overlapping
	; with the preceding regular part, than to make possibly mispredicted
	; branches depending on the size of the last part.
if 0
	mov     eax, dword ptr [esp + 4]                    ; dest
	mov     ecx, dword ptr [esp + 12]                   ; count
else
	pop     eax                                         ; dest
	pop     ecx                                         ; count
endif
	movq    qword ptr [eax + ecx - 10H], xmm0
	movq    qword ptr [eax + ecx - 8], xmm0
	RETURNM

M500:
	; Use non-temporal moves, same code as above:
	; End of regular part:
	; Round down dest+count to nearest preceding 16-bytes boundary
	lea     ecx, [edx + ecx - 1]
	and     ecx, -10H

	; Start of regular part:
	; Round up dest to next 16-bytes boundary
	add     edx, 10H
	and     edx, -10H

	; -(size of regular part)
	sub     edx, ecx
	jnb     M700                                        ; Jump if not negative

	align   16
M600:
	; Loop through regular part
	; ecx = end of regular part
	; edx = negative index from the end, counting up to zero
	movntdq [ecx + edx], xmm0
	add     edx, 10H
	jnz     M600
	sfence

M700:
	; Do the last irregular part (same as M300)
if 0
	mov     eax, dword ptr [esp + 4]                    ; dest
	mov     ecx, dword ptr [esp + 12]                   ; count
else
	pop     eax                                         ; dest
	pop     ecx                                         ; count
endif
	movq    qword ptr [eax + ecx - 10H], xmm0
	movq    qword ptr [eax + ecx - 8], xmm0
	RETURNM
memsetSSE2 endp

; 80386 Version
align 16
memset386 proc near
if 0
	mov     edx, dword ptr [esp + 4]                    ; dest
	xor     eax, eax
	mov     al, byte ptr [esp + 8]                      ; c
	mov     ecx, dword ptr [esp + 12]                   ; count
	imul    eax, 01010101H                              ; Broadcast c into all bytes of eax
	push    edi
	mov     edi, edx
	cmp     ecx, 4
	jb      N400

N200:
	test    edi, 3
	jz      N300

N210:
	; unaligned
	mov     byte ptr [edi], al                          ; store 1 byte until edi aligned
	inc     edi
	dec     ecx
	test    edi, 3
	jnz     N210

N300:
	; aligned
	mov     edx, ecx
	shr     ecx, 2
	cld
	rep     stosd                                       ; store 4 bytes at a time
	mov     ecx, edx
	and     ecx, 3

N400:
	rep     stosb                                       ; store any remaining bytes
	pop     edi
	RETURNM
else
	mov     eax, dword ptr [esp + 4]                    ; load dest
	xor     edx, edx                                    ; load c
	mov     dl, byte ptr [esp + 8]                      ;
	mov     ecx, dword ptr [esp + 12]                   ; load count
	test    ecx, ecx                                    ; if count == 0 ?
	jz      count_equal_zero                            ; jump if count == 0
	push    edi                                         ; preserve edi
	mov     edi, edx                                    ; make word
	shl     edx, 8                                      ;
	or      edx, edi                                    ;
	test    eax, 3                                      ; is aligned to dword?
	jz      repeat_store_dwords                         ; jump if aligned
	cmp     ecx, 2                                      ; if count > 2 ?
	ja      is_aligned_to_word                          ; jump if count > 2
	jb      store_unaligned_byte                        ; jump if count < 2
	jmp     store_unaligned_word                        ; jump if count == 2

	align   16
is_aligned_to_word:
	test    eax, 1                                      ; is aligned to word?
	jz      store_unaligned_word                        ; jump if aligned
store_unaligned_byte:
	mov     byte ptr [eax], dl                          ; store unaligned byte
	inc     eax                                         ; increase dest
	dec     ecx                                         ; decrease count
	jz      return_dest                                 ; jump if count == 0
	test    eax, 2                                      ; is aligned to dword?
	jz      repeat_store_dwords                         ; jump if aligned
store_unaligned_word:
	mov     word ptr [eax], dx                          ; store unaligned word
	add     eax, 2                                      ; increase dest
	sub     ecx, 2                                      ; decrease count
	jz      return_dest                                 ; jump if count == 0
repeat_store_dwords:
	mov     edi, eax                                    ; copy dest
	mov     eax, edx                                    ; make dword
	shl     edx, 16                                     ;
	or      eax, edx                                    ;
	mov     edx, ecx                                    ; copy count
	shr     ecx, 2                                      ; number of dword = count / 4
	jz      store_remaining                             ; jump if count / 4 == 0
if not _MSC_VER
	cld                                                 ; clear direction flag
endif
	rep     stosd                                       ; store 4 bytes at a time
store_remaining:
	and     edx, 3                                      ; has remaining?
	jz      return_dest                                 ; jump if has not remaining
	cmp     edx, 2                                      ; if count < 2 ?
	jae     store_remaining_2_or_3_byte                 ; jump if count >= 2
	mov     byte ptr [edi], al                          ; store remaining byte
	jmp     return_dest

store_remaining_2_or_3_byte:
	mov     word ptr [edi], ax                          ; store remaining word
	je      return_dest                                 ; jump if count == 2
	mov     byte ptr [edi + 2], al                      ; store remaining byte
return_dest:
	mov     eax, dword ptr [esp + 8]                    ; return dest
	pop     edi                                         ; restore edi
count_equal_zero:
	db      0F3h                                        ; rep
	ret                                                 ; __cdecl return
endif
memset386 endp

; CPU dispatching for memset. This is executed only once
align 16
memsetCPUDispatch proc near
	pushad
	call    GetMemsetCacheLimit                         ; calculate cache limit
	call    _InstructionSet                             ; get supported instruction set
	mov     ebx, eax
	; Point to generic version of memset
	mov     dword ptr [memsetDispatch], offset memset386
	cmp     eax, 4                                      ; check SSE2
	jb      Q100
	; SSE2 supported
	; Point to SSE2 version of memset
	mov     dword ptr [memsetDispatch], offset memsetSSE2
	call    _Store256BitIsFaster                        ; check if 256-bit stores are available and faster
	test    eax, eax
	jz      Q100
	mov     dword ptr [memsetDispatch], offset memsetAVX
	cmp     ebx, 15
	jb      Q100
	mov     dword ptr [memsetDispatch], offset memsetAVX512F
	cmp     ebx, 16
	jb      Q100
	mov     dword ptr [memsetDispatch], offset memsetAVX512BW

Q100:
	popad
	; Continue in appropriate version of memset
	jmp     dword ptr [memsetDispatch]
memsetCPUDispatch endp

align 16
GetMemsetCacheLimit proc near
	push    ebx
	mov     ebx, offset MemsetCacheLimit
	mov     eax, dword ptr [ebx]
	test    eax, eax
	jnz     U200
	; Get half the size of the largest level cache
	push    0                                           ; 0 means largest level cache
	call    _DataCacheSize                              ; get cache size
	pop     ecx
	shr     eax, 1                                      ; half the size
	jnz     U100
	mov     eax, 400000H                                ; cannot determine cache size. use 4 Mbytes

U100:
	mov     dword ptr [ebx], eax

U200:
	pop     ebx
	ret
GetMemsetCacheLimit endp

end
