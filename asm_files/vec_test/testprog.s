; DEPENDS
; vector.s
; stdlib.s 


.macro sizeof_int: 4

.global main:

       ; Allocate some memory
       li r3, sizeof_int
       bl malloc
       
       ; Store value in new memory block
       li r4, 1234
       sw r4, 0(r3)
       
       ; Get value from memory block and print it
       mr r30, r3   ; Backup memory address
       lw r3, 0(r3)
       bl puti
       li r3, 10
       sc 0
       
       
       mr r3, r30
       bl free
       
       
       li r3, 20
       bl malloc
       bl free
       
       li r3, 10
       bl malloc
       bl free
       
       li r3, 10
       bl malloc
       bl free

li r3, 10
       bl malloc
       bl free       
       ;bl print_heap
       
       
       ;bl memleak_check

       end 0
