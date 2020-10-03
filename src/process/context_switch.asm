
global context_switch

section .text
;void context_switch(context* old_context, context* new_context);
context_switch:
	mov eax, [esp+4] ;eax=old_context
	mov edx, [esp+8];edx=new_context

	push esi
	push edi
	push ebp
	push ebx

	; switch stacks
	mov [eax], esp
	mov esp, edx

	pop ebx
	pop ebp
	pop edi
	pop esi

	ret


