; DEPENDS
; stdlib.s


___memleak__msg1: "LEAK: "
___memleak__msg2: "\nNO LEAKS\n"
memleak_check:
  subi r0, r0, 20
  sw r15, 0(r0)
  sw r16, 4(r0)
  sw r17, 8(r0)
  sw r18, 12(r0)
  mflr r15
  sw r15, 16(r0)
  
  li r18, false
  
  li r15, ___MALLOC__BLOCK__
  li r16, 0
  
  ___memleak__loop:
    lb r17, struct_malloc_free(r15)
    cmpi r17, true
    beq ___memleak__loop__continue
      ; if the block is not init then dont say that it is a leak
      lw r17, struct_malloc_ptr(r15)
      cmpi r17, NULL
      beq ___memleak__loop__continue
      
        li r3, ___memleak__msg1
        bl put
        lw r3, struct_malloc_ptr(r15)
        bl puti
        li r3, 10
        sc 0
        li r18, true  ; Set failure flag to true
    
    
    ___memleak__loop__continue:
    addi r15, r15, sizeof_struct_malloc
    addi r16, r16, 1
    cmpi r16, ___MALLOC__BLOCK_COUNT
    blt ___memleak__loop
  
  cmpi r18, false
  bne ___memleak__cleanup  ;Do not print "No Leaks" message if we found leaks
    li r3, ___memleak__msg2
    bl puts
  
  
  ___memleak__cleanup:
  lw r15, 16(r0)
  mtlr r15
  lw r15, 0(r0)
  lw r16, 4(r0)
  lw r17, 8(r0)
  lw r18, 12(r0)
  addi r0, r0, 24
  blr

