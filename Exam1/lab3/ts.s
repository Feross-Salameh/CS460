!----------------- ts.s file -----------------------------------------------
        .globl _tswitch
        .globl _main,_running,_scheduler,_proc,_procSize   ! IMPORT these

start:
        mov     ax,cs                   ! establish segments 
        mov     ds,ax                   ! Let DS,SS,ES = CS=0x1000.
        mov     ss,ax                   
        mov     es,ax
	
	mov     sp,#_proc               ! sp -> proc[0]
	add     sp,_procSize            ! sp -> proc[0]'s HIGH END
	
        call _main                      ! call main() in C

dead:	jmp dead                        ! loop if main() ever returns

	
_tswitch:
SAVE:		

	push bp
	pushf
	push ds
	push ss
	push es
	
	mov  bx,_running
	mov  2[bx],sp

FIND:	call _scheduler

RESUME:
	mov  bx,_running
	mov  sp,2[bx]
	
	pop  es
	pop  ss
	pop  ds
	popf
	pop  bp


	ret

