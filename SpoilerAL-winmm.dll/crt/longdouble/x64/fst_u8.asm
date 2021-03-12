public _fst_u8

.code

_fst_u8 proc
	cmp     word ptr [rcx + 8], 403EH
	jge     L1
	fld     tbyte ptr [rcx]
	fistp   qword ptr [rsp + 8]
	mov     rax, qword ptr [rsp + 8]
	ret
L1:
	mov     rax, -1
	jne     L2
	mov     rax, qword ptr [rcx]
L2:
	db      0F3h
	ret
_fst_u8 endp

end
