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

.global main

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
  
.macro puti_temp_string_size: 30
puti:
  ;Backup this crap
  subi r0, r0, 12
  sw r3, 8(r0)
  sw r4, 4(r0)
  mflr r4
  sw r4, 0(r0)
  ; call the stupid function
  subi r0, r0, puti_temp_string_size     ;Alloc 30 bytes for the string
  mr r4, r0
  bl itoa
  mr r3, r0    ;Get addr of string
  bl put         ;Print this crap to the screen
  addi r0, r0, puti_temp_string_size   ;Dealloc temp string
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
;#########################################################################################
; How does it work?

; This implementation of malloc has a table of blocks of memory. Each table stores the pointer, size, and free state.
; The first node contains all of the memory in the RAM. The 'free' state of the node is marked as free.

; When you call malloc(), it searches the nodes for a freed node. If it finds one that it can fit into, it will find a node that has not be used.
; The node that was found to have free memory will now have a size of the newly malloced object. The remaing bytes that were not used by the
; newly malloced object will be moved to a new node that contains a pointer to the location in memory after the newly malloced object.
; It's state is kept set to free so that other future malloc calls can use it. The newly malloced object is then set to false so that free() knows to not delete it
; Here is an example

; === STEP 0 === Inital state after 'start' is done executing
; table[0] = {
;  ptr = 4000,
;  size = 3000,
;  free = true
;}
;table[1] = {
;  ptr = NULL,
;  size = NULL,
;  free = NULL
;}

; === STEP 1 === User calls malloc(30);    the old size of the block is stored temporality in 'old_size'
;new_size = 30;
;old_size = 3000;

; table[0] = {      ; The table is now marked as not free and has a size of 30 now. Notice the ptr has not changed as well
;  ptr = 4000,
;  size = 30,
;  free = false
;}
;table[1] = {        ; The program has also found this node to be NULL so it saves the index to copy the remaining bytes into
;  ptr = NULL,
;  size = NULL,
;  free = NULL
;}

; === STEP 2 ===    Orignal node is copied over with the remaing_bytes not used by table[0]
; table[0] = {    
;  ptr = 4000,
;  size = 30,
;  free = false
;}
;table[1] = {
;  ptr = table[0].ptr + table[0].size,
;  size = old_size - table[0],
;  free = true
;}

; === FINAL RESULT ===
; table[0] = {    
;  ptr = 4000,
;  size = 30,
;  free = false
;}
;table[1] = {  ; This block has now become 30 bytes less than what it orginally was 
;  ptr = 4030,
;  size = 2970,
;  free = true
;}

; Because all memory is assumed to be 0 at boot, there is no need to initalize anything other than the first node


; Macro table sizes and start locations
;___MALLOC__BLOCK__  is a symbol defined by the assembler by default
.macro ___MALLOC__BLOCK_COUNT: 500
.macro ___MALLOC__SIZE__: 10000

.macro struct_malloc_ptr: 0
.macro struct_malloc_size: 4
.macro struct_malloc_free: 8
.macro sizeof_struct_malloc: 9


;struct malloc_entry{
;  void* ptr;
;  unsigned int size;
;  char free;
;}
;##########################################################################################



___malloc__get__empty:
  subi r0, r0, 20
  sw r14, 0(r0)
  mflr r14
  sw r14, 4(r0)
  sw r15, 8(r0)
  sw r16, 12(r0)
  sw r17, 16(r0)

  li r14, ___MALLOC__BLOCK_COUNT
  subi r14, r14, 1
  muli r14, 14, sizeof_struct_malloc
  li r15, ___MALLOC__BLOCK__
  add r14, r14, r15
  ___malloc__get__empty__loop:
    lw r16, struct_malloc_ptr(r15)
    cmpi r16, 0
    bne ___malloc__get__empty__loop__continue
    mr r3, r15
    b ___malloc__get__cleanup
    ___malloc__get__empty__loop__continue:
    addi r15, r15, sizeof_struct_malloc
    cmp r15, r14
    ble ___malloc__get__empty__loop
  
  ___malloc__get__cleanup:
  ;Do we not need to merge blocks?
  cmp r15, r3
  beq ___malloc__if__valid__retrun__value
  ;Not a valid return value. Merge blocks and try again
    li r15, ___MALLOC__BLOCK__
    ; Move r15 back so that the loop does not need extra logic
    subi r15, r15, sizeof_struct_malloc
    ___malloc__merge__loop:
      addi r15, r15, sizeof_struct_malloc
      cmp r15, r14
      ; dont go over block amounts
      bgt ___malloc__if__valid__retrun__value
      
      lb r16, struct_malloc_free(r15)
      cmpi r16, true
      bne ___malloc__merge__loop     ;free != 1 so continue
      addi r17, r15, sizeof_struct_malloc        ; Check to next block
      lb r16, struct_malloc_free(r17)
      cmpi r16, true
      bne ___malloc__merge__loop   ;free != 1 so continue
  ;;Two adjactent free nodes found. Move memory into 'i' and delete 'i + 1'
  lw r16, struct_malloc_size(r17)     ;Get size of i+1
  lw r14, struct_malloc_size(r15)     ;Get size of i
  add r16, r14, r16     ; malloc[i].size += malloc[i + 1].size
  sw r16, struct_malloc_size(r15)    ;Set size of i
  ;;Delete i + 1
    li r16, 0
    sw r16, struct_malloc_ptr(r15)
  ; Return the null node
    mr r3, r17
  
  ___malloc__if__valid__retrun__value:
  lw r14, 4(r0)
  mtlr r14
  lw r14, 0(r0)
  lw r15, 8(r0)
  lw r16, 12(r0)
  lw r17, 16(r0)
  addi r0, r0, 20
  blr



malloc:
  cmpi r3, 0
  bne __malloc__passed__null__check
  ; size = 0
  li r3, 0
  blr             ; return NULL;
  __malloc__passed__null__check:
  subi r0, r0, 24
  sw r14, 0(r0)
  mflr r14
  sw r14, 4(r0)
  sw r15, 8(r0)
  sw r16, 12(r0)
  sw r17, 16(r0)
  
  li r14, ___MALLOC__BLOCK__
  ;Get end ptr
  li r15, ___MALLOC__BLOCK_COUNT
  muli r15, r15, sizeof_struct_malloc
  add r15, r15, r14
  ;Start loop
  ___malloc__search__block__loop:
    lb r16, struct_malloc_free(14)
    cmpi r16, true
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
    li r17, true
    sb r17, struct_malloc_free(r3)
    ; table[i].size = size
    sw r15, struct_malloc_size(r14)
     
    ___malloc__search__block__set__free:
    ; table[i].free = false
    li r16, 0
    sb r16, struct_malloc_free(r14)
    ; break;
    b ___malloc__cleanup
    __malloc__search__block__loop__continue:
    addi r14, r14, sizeof_struct_malloc
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
  addi r0, r0, 24
  blr
  


___double__free__message: "Double free: LR = "
___double__free__message1: ", PTR = "
free:
  li r4, ___MALLOC__BLOCK_COUNT
  muli r4, r4, sizeof_struct_malloc
  li r7, ___MALLOC__BLOCK__
  add r4, r4, r7
  
  __free__loop:
    subi r4, r4, sizeof_struct_malloc
    cmp r4, r7
    bge __free__loop__if__pass
    ;ptr was not found!
    end 1
    __free__loop__if__pass:
    lw r5, struct_malloc_ptr(r4)
    cmp r5, r3
    bne __free__loop
  ;Found node is stored in r4

  
  ;Has the node been double freed?
  lb r3, struct_malloc_free(r4)
  cmpi r3, true
  bne ___free__double__free__pass
    ;DOUBLE FREE DETECTED
    lw r29, struct_malloc_ptr(r4)
    mflr r30
    li r3, ___double__free__message
    bl put

    
    ;Print LR
    mr r3, r30
    bl puti
    
    li r3, ___double__free__message1
    bl put
    
    ;Print PTR
    mr r3, r29
    bl puti
    li r3, 10
    sc 0
    end 1
  
  ___free__double__free__pass:
    ;node.free = true
  li r3, 1
  sb r3, struct_malloc_free(r4)
  blr

  

realloc:
  subi r0, r0, 16
  sw r14, 0(r0)
  mflr r14
  sw r14, 4(r0)
  sw r15, 8(r0)
  sw r16, 12(r0)
  
  li r14, ___MALLOC__BLOCK_COUNT
  li r15, ___MALLOC__BLOCK__
  
  __realloc__loop:
    lw r16, struct_malloc_ptr(r15)
    
    ; Is the pointer the same?
    cmp r16, r3
    bne __realloc__loop__continue
    
    ; Block was found. Grab the size of the block
    lw r14, struct_malloc_size(r15)
    b __realloc__save__new
    
    __realloc__loop__continue:
    addi r15, r15, sizeof_struct_malloc
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






;  ==================   ENTRY   =========================
  
start:
  ;;Set root node because we can assume that all memory is NULL when boot happens
  li r2, ___MALLOC__BLOCK__
      ;Calculate end of malloc block section to get the beggining of the heap
  li r3, ___MALLOC__BLOCK_COUNT
  addi r3, r3, 1 ; Add another NULL block so that malloc doesnt need extra lodgic to figure out the end of the table
  muli r3, r3, sizeof_struct_malloc
  addi r3, r3, ___MALLOC__BLOCK__
  
  li r4, ___MALLOC__SIZE__
  ; node.ptr = malloc_start;
  ; node.size = malloc_size
  sw r3, struct_malloc_ptr(r2)
  sw r4, struct_malloc_size(r2)
  ; node.free = 1;  
  li r4, 1
  sb r4, struct_malloc_free(r2)
  
  b main
