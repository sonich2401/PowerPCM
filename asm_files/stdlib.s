; STDLIB: This file implements functions from C into PowerPC

;FUNCTIONS PROVIDED
; strlen(char* str)
; puts(char* str)
; put(char* str)                                                   ; Prints a string without a newline
; scanf(char* format)                                        ; works the same way as C scanf because it is being passed through via sc
; puti(unsigned int num)                                  ; prints a unsigned int as a string
; itoa(char* buffer, unsigned int num)           ; converts a unsigned int into a string
; memcpy(void* dest, void* src, unsigned int size)
; realloc(void* ptr)
; malloc(unsigned int size)
; free(void* ptr)

__puti__string__buffer: "OOB MEM OOB MEM OOB MEM"


strlen:
  subi r0, r0, 8
  sw r4, 0(r0)
  sw r5, 4(r0)
  
  ;Main code
  mr r4, r3
  li r3, 0
;if str[0] == 0 then the addi later will correct the strlen to 0
  subi r3, r3, 1
  __strlen_loop:
    lb r5, 0(r4)
    addi r3, r3, 1
    addi r4, r4, 1
    cmpi r5, 0
    bne __strlen_loop
  ;;;;;;;;;;
  
  lw r4, 0(r0)
  lw r5, 4(r0)
  addi r0, r0, 8
  blr

  
puts:
  subi r0, r0, 8
  sw r4, 0(r0)
  mflr r4
  sw r4, 4(r0)
  bl put
  ;print a new line
  li r3, 10
  sc 0
  
  ;Exit
  lw r4, 4(r0)
  mtlr r4
  lw r4, 0(r0)
  addi r0, r0, 8
  blr

put:
  subi r0, r0, 8
  sw r4, 0(r0)
  mflr r4
  sw r4, 4(r0)
  mr r4, r3
  li r3, 0
  put_loop:
    lb r3, 0(r4)
    sc 0
    addi r4, r4, 1
    cmpi r3, 0
  bne put_loop
;Cleanup
  lw r4, 4(r0)
  mtlr r4
  lw r4, 0(r0)
  addi r0, r0, 8
  blr
  
scanf:
  subi r0, r0, 8
  sw r5, 0(r0)
  mflr r5
  sw r5, 4(r0)
  sc 1
  lw r5, 4(r0)
  mtlr r5
  lw r5, 0(r0)
  addi r0, r0, 8
  blr
  
; Convert a 'int' into a string.
; Does not affect LR
; Changes registers 14, 15, 16 with restore
; NOTE: BUFFER MUST BE MORE THAN THE MAX AMOUNT OF DIGITS A INT CAN HOLD
; void itoa(unsigned int num, char* buffer)
itoa:
  subi r0, r0, 12
  sw r14, 0(r0)
  sw r15, 4(r0)
  sw r16, 8(r0)

  ; REGISTER USAGE
  ; r14 = num. type = unsigned int
  ; r15 = end_str. Type = char*
  ; r16 = tmp_var. Type = unsigned int



  mr r14, r3    ; int num = arg_1
  mr r15, r4
  __tostring__lenloop:
    ; num /= 10;
    divi r14, r14, 10
    ; len++;
    addi r15, r15, 1
    ;if (num != 0 ) goto __tostring2__lenloop;
    cmpi r14, 0
    bne __tostring__lenloop
  
  ; len is stored in r15
  li r16, 0
  sb r16, 0(r15)      ; str[len] = 0
  subi r15, r15, 1    ; len--;
  mr r14, r3    ; Restore the number we are grabbing
  __tostring__stringwrite__loop:
    ; num %  10
    mr r16, r14
    divi r14, r14, 10
    muli r14, 14, 10
    sub r16, 16, r14
    ; num /= 10
    divi r14, r14, 10
    ;Save char
    addi r16, r16, 48
    sb r16, 0(r15)
    ; str--;
    subi r15, r15, 1
    cmp r15, r4
    bge __tostring__stringwrite__loop
  
  lw r14, 0(r0)
  lw r15, 4(r0)
  lw r16, 8(r0)
  addi r0, r0, 12
  blr
  
puti:
  ;Backup this crap
  subi r0, r0, 12
  sw r3, 8(r0)
  sw r4, 4(r0)
  mflr r4
  sw r4, 0(r0)
  ; call the stupid function
  li r4, __puti__string__buffer
  bl itoa
  li r3, __puti__string__buffer
  bl put         ;Print this crap to the screen
  lw r4, 0(r0)
  mtlr r4
  lw r4, 4(r0)
  lw r3, 8(r0)
  addi r0, r0, 12
  blr
  
memcpy:
  subi r0, r0, 8
  sw r14, 0(r0)
  sw r3,  4(r0)        ; Backup function argument for later return value

  __memcpy__loop:
    lb r14, 0(r4)
    sb r14, 0(r3)
    addi r3, r3, 1
    addi r4, r4, 1
    subi r5, r5, 1
    cmpi r5, 0
    bne __memcpy__loop
  lb r14, 0(r4)
  sb r14, 0(r3)    
  
  lw r14, 0(r0)
  lw r3,  4(r0)
  addi r0, r0, 8
  blr

;; MALLOC IMPLEMENTATION

.macro struct_malloc_ptr: 0
.macro struct_malloc_size: 4
.macro struct_malloc_free: 8

___malloc__entry__construct:
  sw r4, struct_malloc_ptr(r3)
  sw r5, struct_malloc_size(r3)
  sb r6, struct_malloc_free(r3)
  blr
  
___malloc__merge__:
  subi r0, r0, 4
  addi r0, r0, 4
  blr

___malloc__get__empty:
  subi r0, r0, 16
  sw r14, 0(r0)
  mflr r14
  sw r14, 4(r0)
  sw r15, 8(r0)
  sw r16, 12(r0)

  li r14, ___MALLOC__BLOCK_COUNT
  lw r14, 0(r14)
  subi r14, r14, 1
  muli r14, 14, 9
  li r15, ___MALLOC__BLOCK__
  lw r15, 0(r15)
  add r14, r14, r15
  ___malloc__get__empty__loop:
    lw r16, struct_malloc_ptr(r15)
    cmpi r16, 0
    bne ___malloc__get__empty__loop__continue
    mr r3, r15
    b ___malloc__get__cleanup
    ___malloc__get__empty__loop__continue:
    addi r15, r15, 9
    cmp r15, r14
    ble ___malloc__get__empty__loop
  
  ___malloc__get__cleanup:
  ;Do we not need to merge blocks?
  cmp r15, r3
  beq ___malloc__if__valid__retrun__value
  ;Not a valid return value. Merge blocks and try again
    ;Backup register 17 and 18
      subi r0, r0, 8
      sw r17, 0(r0)
      sw r18, 4(r0)
    li r14, ___MALLOC__BLOCK_COUNT
    lw r14, 0(r14)
    subi r14, r14, 1
    muli r14, 14, 9
    li r15, ___MALLOC__BLOCK__
    lw r15, 0(r15)
    add r14, r14, r15
    ; Move r15 back so that the loop does not need extra logic
    subi r15, r15, 9
    ___malloc__merge__loop:
      addi r15, r15, 9
      cmp r15, r14
      ; dont go over block amounts
      bgt ___malloc__merge__cleanup
      
      lb r16, struct_malloc_free(r15)
      cmpi r16, 1
      bne ___malloc__merge__loop   ;free != 1 so continue
      addi r17, r15, 9        ; Check to next block
      lb r16, struct_malloc_free(r17)
      cmpi r16, 1
      bne ___malloc__merge__loop   ;free != 1 so continue
      ;;Two adjactent free nodes found. Move memory into 'i' and delete 'i + 1'
        lw r16, struct_malloc_size(r17)     ;Get size of i+1
        lw r18, struct_malloc_size(r15)     ;Get size of i
        add r16, r18, r16     ; malloc[i].size += malloc[i + 1].size
        sw r16, struct_malloc_size(r15)    ;Set size of i
        ;;Delete i + 1
          li r16, 0
          sw r16, struct_malloc_ptr(r15)
        ; Return the null node
          mr r3, r17
    ___malloc__merge__cleanup:
    ; Restore register 17
      lw r17, 0(r0)
      lw r18, 4(r0)
      addi r0, r0, 8
  
  ___malloc__if__valid__retrun__value:
  lw r14, 4(r0)
  mtlr r14
  lw r14, 0(r0)
  lw r15, 8(r0)
  lw r16, 12(r0)
  addi r0, r0, 16
  blr



malloc:
  cmpi r3, 0
  bne __malloc__passed__null__check
  ; size = 0
  li r3, 0
  blr             ; return NULL;
  __malloc__passed__null__check:
  subi r0, r0, 32
  sw r14, 0(r0)
  mflr r14
  sw r14, 4(r0)
  sw r15, 8(r0)
  sw r16, 12(r0)
  sw r17, 16(r0)
  sw r18, 20(r0)
  sw r19, 24(r0)
  
  li r14, ___MALLOC__BLOCK__
  lw r14, 0(r14)
  ;Get end ptr
  li r15, ___MALLOC__BLOCK_COUNT
  lw r15, 0(r15)
  muli r15, r15, 9
  add r15, r15, r14
  ;Start loop
  ___malloc__search__block__loop:
    lb r16, struct_malloc_free(14)
    cmpi r16, 1
    bne __malloc__search__block__loop__continue
    ; Check for free has passed. Now get size
    lw r16, struct_malloc_size(r14)
    cmp r3, r16
    bgt __malloc__search__block__loop__continue
    ; All checks have passed. Lets use this block
    ; Remaining_bytes = table[i].size - size;
    sub r17, r16, r3
    ;If remaining_bytes == 0
    cmpi r17, 0
    beq ___malloc__search__block__set__free
    ; All checks have passed. Get a new free node and allocate the memory and return it
    mr r15, r3
    ;node* empty = get_empty();
    bl ___malloc__get__empty
    lw r16, struct_malloc_ptr(r14)        ; void* tmp_ptr = table[i].ptr
    add r16, r16, r15     ; tmp_ptr += size
    sw r16, struct_malloc_ptr(r3)         ; empty->ptr = tmp_ptr
    sw r17, struct_malloc_size(r3)         ; empty->size = remaining_bytes
    ; empty->free = 1
    li r17, 1
    sb r17, struct_malloc_free(r3)
    ; table[i].size = size
    sw r15, struct_malloc_size(r14)
     
    ___malloc__search__block__set__free:
    ; table[i].free = 0
    li r16, 0
    sb r16, struct_malloc_free(r14)
    ; break;
    b ___malloc__cleanup
    __malloc__search__block__loop__continue:
    addi r14, r14, 9
    cmp r14, r15
    bne ___malloc__search__block__loop
  
  
  ___malloc__cleanup:
  ;Get return value
  lw r3, 0(r14)
  
  lw r14, 4(r0)
  mtlr r14
  lw r14, 0(r0)
  lw r15, 8(r0)
  lw r16, 12(r0)
  lw r17, 16(r0)
  lw r18, 20(r0)
  lw r19, 24(r0)
  addi r0, r0, 32
  blr
  
free:
  li r4, ___MALLOC__BLOCK_COUNT
  lw r4, 0(r4)
  mr r6, r4
  muli r4, r4, 9
  li r7, ___MALLOC__BLOCK__
  lw r7, 0(r7)
  add r4, r4, r7
  
  __free__loop:
    subi r4, r4, 9
    cmp r4, r7
    bge __free__loop__if__pass
    ;ptr was not found!
    end 1
    __free__loop__if__pass:
    lw r5, struct_malloc_ptr(r4)
    cmp r5, r3
    bne __free__loop
  ;Found node is stored in r4
  ;node.free = true
  li r3, 1
  sb r3, 8(r4)
  blr


printHeapMessage1: "Entry: "
printHeapMessage2: "Size: "
printHeapMessage3: "Free: "
printHeapMessage4: "PTR: "

print_heap:
  subi r0, r0, 32
  sw r14, 0(r0)
  sw r15, 4(r0)
  sw r16, 8(r0)
  sw r17, 12(r0)
  sw r18, 16(r0)
  sw r19, 20(r0)
  mflr r14
  sw r14, 24(r0)
  
  li r14, 200
  li r15, 0
  li r16, ___MALLOC__BLOCK__
  lw r16, 0(r16)
  ___print_heap_loop:
    cmpi r15, r14
    bge ___print_heap__cleanup
    
    lw r17, struct_malloc_ptr(r16)
    cmpi r17, 0
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
  lw r14, 24(r0)
  mtlr r14
  lw r14, 0(r0)
  lw r15, 4(r0)
  lw r16, 8(r0)
  lw r17, 12(r0)
  lw r18, 16(r0)
  lw r19, 20(r0)
  addi r0, r0, 32
  blr


realloc:
  subi r0, r0, 16
  sw r14, 0(r0)
  mflr r14
  sw r14, 4(r0)
  sw r15, 8(r0)
  sw r16, 12(r0)
  
  li r14, ___MALLOC__BLOCK_COUNT
  lw r14, 0(r14)
  li r15, ___MALLOC__BLOCK__
  lw r15, 0(r15)
  
  __realloc__loop:
    lw r16, 0(r15)
    
    ; Is the pointer the same?
    cmp r16, r3
    bne __realloc__loop__continue
    
    ; Block was found. Grab the size of the block
    lw r14, 4(r15)
    b __realloc__save__new
    
    __realloc__loop__continue:
    addi r15, r15, 9
    subi r14, r14, 1
    cmpi r14, 0
    bne __realloc__loop
    ; If you reach this point, then the block is not found. Return NULL and Cleanup
    li r3, 0
    b __realloc__cleanup
    
  __realloc__save__new:
  mr r15, r3      ; Move the address of the block to be deleted into a backup regsiter
  mr r16, r4      ; Move size of new block into r16 for backup for future memcpy call 
  mr r3, r4       ; Move size of new block into r3 for the malloc call
  bl malloc
    cmpi r3, 0
    bne __realloc__if__null__pass
    ; The address malloc returned was null. Cleanup and return NULL as well
    li r3 0
    b __realloc__cleanup
    
  __realloc__if__null__pass:
  mr r4, r15       ; Move old memory block pointer into r4 for memcpy call
  mr r5, r16       ; Move old size into r5 for memcpy call
  ; r3 already contains the destination pointer due to the malloc call
  bl memcpy
  mr r16, r3       ; Backup pointer of new memory 
  
  ; Memory has been copied. Free old memory block
  mr r3, r15
  bl free
  
  ; Restore pointer to memory for return
  mr r3, r16
  
  ; Exit function
  __realloc__cleanup:
  lw r14, 4(r0)
  mtlr r14
  lw r14, 0(r0)
  lw r15, 8(r0)
  lw r16, 12(r0)
  addi r0, r0, 16
  blr

; Four chars give us a word to play with
___MALLOC__START__: "///"
___MALLOC__BLOCK__: "///"
___MALLOC__BLOCK_COUNT: "///"  
___MALLOC__SIZE__: "///"

;struct malloc_entry{
;  void* ptr;
;  unsigned int size;
;  char free;
;}











  
start:
  addi r3, r0, 1
  li r2, ___MALLOC__BLOCK__
  sw r3, 0(r2)
  
  ;Number of blocks allowed
  li r3, 80
  li r2, ___MALLOC__BLOCK_COUNT
  sw r3, 0(r2)
  
  ;Get start ptr
  li r6, 9           ; sizeof(malloc_entry)
  mul r3, r3, r6
  
  ;Get ___MALLOC__BLOCK__ and add sizeof(malloc_entry) * 200
  add r3, r3, r0
  addi r3, r3, 1
  li r2, ___MALLOC__START__
  sw r3, 0(r2)
  
  ; Allowed space to play with
  ;li r3, 2000
  li r3, 50000
  sub r3, r3, r0
  subi r3, r3 1000
  li r2, ___MALLOC__SIZE__
  sw r3, 0(r2)
  
  ;;;Init all malloc blocks
  ; i = ___MALLOC__BLOCK_COUNT
  li r2, ___MALLOC__BLOCK_COUNT
  lw r5, 0(r2)
  subi r5, r5, 1
  ; Registers are used as values to set things to
  li r3, 0
  li r4, 1
  li r2, ___MALLOC__BLOCK__
  lw r2, 0(r2)
  __malloc__block__init__loop:
    sw r3, 0(r2)
    sw r3, 4(r2)
    sb r4, 8(r2)
    ;ptr += sizeof(malloc_entry)
    add r2, r2, r6
    subi r5, r5, 1
    cmpi r5, 0
    bne __malloc__block__init__loop
  
  ;;Set root node
  li r2, ___MALLOC__BLOCK__
  lw r2, 0(r2)
  li r3, ___MALLOC__START__
  lw r3, 0(r3)
  li r4, ___MALLOC__SIZE__
  lw r4, 0(r4)
  ; node.ptr = malloc_start;
  ; node.size = malloc_size
  sw r3, 0(r2)
  sw r4, 4(r2)
  
  
  
  
  b main
