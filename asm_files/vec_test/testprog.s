; DEPENDS
; vector.s
; stdlib.s

main:

       ; Get a new vector
       li r3, 4         ;sizeof(int)
       li r4, 0         ;decon_func_ptr = null
       bl vector_alloc
       mr r14, r3 ;Backup new pointer
       bl puti
       li r3, 10
       sc 0
       mr r3, r14
       
       subi r0, r0, 4
       li r15, 1234
       sw r15, 0(r0)
       mr r4, r0  ;Set up data argument
       
       bl vector_push_back
       addi r0, r0, 4
       
       mr r3, r14 ;Grab pointer
       bl puti
       li r3, 10
       sc 0
       mr r3, r14 ;Grab pointer
       li r4, 0
       bl vector_index
       
       lw r3, 0(r3)
       bl puti
       li r3, 10
       sc 0
       
       mr r3, r14 ;;Get vec* again
       bl vector_deconstruct
       
       bl print_heap
       ;Cleanup
       mr r3, r14
       bl free

       end 0
