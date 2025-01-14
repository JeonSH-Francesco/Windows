
newTargetFunc		PROTO C

.code

replacedTargetFunc PROC
	;targetFunc() 프롤로그
	mov			dword ptr [rsp+10h],edx
	mov			dword ptr [rsp+8],ecx
	sub			rsp, 38h

	call newTargetFunc

	;targetFunc() 프롤로그 다음

	mov rax, 00000001400010Ech
	jmp rax

replacedTargetFunc  ENDP

END
