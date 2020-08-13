.global _galoislfsrina
.extern sprintfkeys

_galoislfsrina:
    pushl %ebp
    movl %esp, %ebp
    movl 8(%ebp), %ebx   # GET seedIN
    movl %ebx, %esi      # make a copy of seedIN
    movl 12(%ebp), %ecx  # GET charCount
    movl 16(%ebp), %edx  # GET &key[0]
    movl $0, %edi
    loop:
        andl $1, %ebx  # generate LSB (output)
        shrl $1, %esi  # apply shift (LFSR_STATE)
        cmpl $0, %ebx  # compare LSB : 0
        jle noMask     # jump if LSB is 1
        xorl $0xA3000000, %esi # apply taps
        noMask:
        incl %edi          # period++
        pushl %ecx
        pushl %esi         # pass LFSR_STATE as arg2
        pushl %edx         # pass keyPtr as arg1
        call _sprintfkeys   # sprintfkeys(keyPtr, LFSR_STATE)
        popl %edx          # clean up the stack
        popl %esi          # ...
        popl %ecx
        addl %eax, %edx # keyPtr += sprintfKeys(keyPtr, LFSR_STATE)
        movl %edi, %ebx # ...
        shll $2, %ebx   # 4*period
        cmpl %ecx, %ebx # compare 4*period : charCount
        jge end         # break if 4*period >= charCount
        movl %esi, %ebx # restore LFSR_STATE
        cmpl %esi, 16(%ebp) # compare startingState : LFSR_STATE
        jne loop
    end:
    movl $1, %eax # return 1
    movl %ebp, %esp
    popl %ebp
    ret
