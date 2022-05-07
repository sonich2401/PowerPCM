printHeapMessage1: "Entry: "
printHeapMessage2: "Size: "
printHeapMessage3: "Free: "
printHeapMessage4: "PTR: "

print_heap:
  subi r0, r0, 20
  sw r14, 0(r0)
  sw r15, 4(r0)
  sw r16, 8(r0)
  sw r17, 12(r0)
  mflr r14
  sw r14, 16(r0)
  
  li r14, 200
  li r15, 0
  li r16, ___MALLOC__BLOCK__
  ___print_heap_loop:
    cmpi r15, r14
    bge ___print_heap__cleanup
    
    lw r17, struct_malloc_ptr(r16)
    cmpi r17, NULL
    beq ___print_heap_loop__conitune

    li r3, printHeapMessage1
    bl put
    
    mr r3, 15
    bl puti
    
    li r3, 10
    sc 0
    
    li r3, printHeapMessage2
    bl put
    
    lw r3, struct_malloc_size(r16)
    bl puti
    
    li r3, 10
    sc 0
    
    li r3, printHeapMessage3
    bl put
    
    lb r3, struct_malloc_free(r16)
    bl puti
    
    li r3, 10
    sc 0
    
    li r3, printHeapMessage4
    bl put
    
    lw r3, struct_malloc_ptr(r16)
    bl puti
    
    li r3, 10
    sc 0
    sc 0

    ___print_heap_loop__conitune:
    addi r15, r15, 1
    addi r16, r16, 9
    b ___print_heap_loop
    
  ___print_heap__cleanup:
  lw r14, 16(r0)
  mtlr r14
  lw r14, 0(r0)
  lw r15, 4(r0)
  lw r16, 8(r0)
  lw r17, 12(r0)
  addi r0, r0, 20
  blr
