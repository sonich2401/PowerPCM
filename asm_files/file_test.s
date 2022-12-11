; REQUIRES STDLIB

.file myFile: "./fib.s"

.macro sizeof_size_t: 8

main:
    li r3, myFile
    bl fsize
    mr r5, r3 ; r5 now holds the size of the file

    li r3, myFile
    bl fptr
    mr r4, r3 ; r4 now holds the pointer to the file

    print_loop:
        lb r3, 0(r4) ; Loads byte from the file
        addi r4, r4, 1 ; Move to next character
        sc 0 ; Print character to screen
        subi r5, r5, 1
        cmpi r5, r5, 0
        bgt print_loop
    
    end 0