.global _reverseTimeValueA

_reverseTimeValueA:
    pushl %ebp
    movl %esp, %ebp
    pushl %ebx    # save registers
    pushl %esi
    pushl %edi
    movl 8(%ebp), %eax # GET num
    xorl %edi, %edi    # reverseNum = 0
    cmpl $0, %eax      # compare num : 0
    jle end            # exit if num <= 0
    loop:
        movl $10, %esi    # 10
        imull %edi, %esi  # reverseNum*10
        movl $10, %ebx    # 10
        sarl $31, %edx
        idivl %ebx        # num%10
        addl %esi, %edx   # reverseNum*10 + num%10
        movl %edx, %edi   # reverseNum = reverseNum*10 + num%10
        # num = num/10 is implied (in %eax)
        cmpl $0, %eax     # compare num : 0
        jg loop           # jump if num > 0
    end:
    movl %edi, %eax
    popl %ebx    # restore registers
    popl %esi
    popl %edi
    movl %ebp, %esp
    popl %ebp
    ret
