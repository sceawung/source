__declspec(naked) void __cdecl _chkesp()
{
	__asm
	{
		jz      L1
		int     3
	L1:
		rep ret
	}
}
