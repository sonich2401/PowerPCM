; 0 = dat
; 4 = elements
; 8 = buffer_health
; 12 = tsize
; 16 = decon

.macro struct_vector_dat: 0
.macro struct_vector_size: 4
.macro struct_vector_buffer: 8
.macro struct_vector_type: 12
.macro struct_vector_decon: 16
.macro sizeof_vector: 20

vector_alloc:
       subi r0, r0, 8
       sw r14, 0(r0)
       mflr r14
       sw r14, 4(r0)
       ;Main code
       
       ; Shift all arguments over
       mr r5, r4
       mr r4, r3
       
       ; Alloc new vector
       li r3, sizeof_vector
       bl malloc
       bl vector_init
       ; r3 has the pointer of the new vector
       
       ;Cleanup
       lw r14, 4(r0)
       mtlr r14
       lw r14, 0(r0)
       addi r0, r0, 8
       blr

vector_init:
       subi r0, r0, 8
       sw r14, 0(r0)
       mflr r14
       sw r14, 4(r0)

       ;Main code
       sw r4, struct_vector_type(r3)     ;vec->type_size = type_size
       sw r5, struct_vector_decon(r3)     ;vec->decon = decon
       mr r5, r4          ; Backup register
       li r4, 30
       sw r4, struct_vector_buffer(r3)     ;vec->buffer_health = 30;
       mr r14, r3         ;Backup vec*
       mul r3, r4, r5  ;unsigned int size = buffer_health * type_size

       ;Get new block of memory
       bl malloc
       
       ;Save ptr to vec*
       sw r3, struct_vector_dat(r14)
       mr r3, r14
       
       ;Cleaup
       lw r14, 4(r0)
       mtlr r14
       lw r14, 0(r0)
       addi r0, r0, 8
       blr
       
       
__vector__add__check:
       subi r0, r0, 20
       sw r14, 0(r0)
       mflr r14
       sw r14, 4(r0)
       sw r15, 8(r0)
       ; Backup args
       sw r3, 12(r0)
       sw r4, 16(r0)
       ; Main

       lw r14, struct_vector_buffer(r3)  ;Get buffer health
       lw r15, struct_vector_size(r3)  ;Get element count
       addi r15, r15, 1
       cmp r14, r15
       bgt __vector__add__check__cleanup
       
       ; We have passed the buffer overflow. Allocate more memory
       addi r14, r14, 20
       lw r15, struct_vector_type(r3)   ; Get tsize
       
       mul r4, r14, r15 ; buffer_health * type_size
       sw r14, struct_vector_buffer(r3) ; Save new health
       mr r15, r3 ; Backup vec*
       lw r3, struct_vector_dat(r3) ; dat = *vec.dat
       
       bl realloc
       
       sw r3, 0(r15) ; Store dat back into vec*.dat
       
       ; Cleanup
       __vector__add__check__cleanup:
       ;Restore args
       lw r3, 12(r0)
       lw r4, 16(r0)
       ;
       lw r14, 4(r0)
       mtlr r14
       lw r14, 0(r0)
       lw r15, 8(r0)
       addi r0, r0, 20
       blr

vector_push_back:
       subi r0, r0, 12
       sw r14, 0(r0)
       mflr r14
       sw r14, 4(r0)
       sw r15, 8(r0)
       mr r14, r4  ;Backup data pointer for branch call
       mr r15, r3  ;Backup dat pointer for function call
       
       bl __vector__add__check

       ; Restore
       mr r3, r15
       mr r4, r14
       
       ; Prepare memcpy
       lw r4, struct_vector_size(r3) ; elemetns
       bl vector_index
       
       
       lw r5, 12(r15) ; type_size
       mr r4, r14 ; void* data pointer
       bl memcpy

       ; Get vec* again
       mr r3, r15
       ; Add the total data counter
       lw r14, struct_vector_size(r3)
       addi r14, r14, 1
       sw r14, struct_vector_size(r3)

       ; Cleanup
       lw r14, 4(r0)
       mtlr r14
       lw r14, 0(r0)
       lw r15, 8(r0)
       addi r0, r0, 12
       blr
       
vector_index:
       subi r0, r0, 4
       sw r14, 0(r0)


       lw r14, struct_vector_type(r3) ;type_size
       lw r3, struct_vector_dat(r3)  ;Dat
       mul r14, r14, r4  ;index * type_size
       add r3, r14, r3   ; dat + index * type_size
       
       lw r14, 0(r0)
       addi r0, r0, 4
       blr
       
vector_pop_back:
       subi r0, r0, 4
       sw r14, 0(r0)
       
       lw r14, struct_vector_size(r3)
       subi r14, r14, 1
       sw r14, struct_vector_size(r3)
       
       lw r14, struct_vector_decon(r3)  ;;Get decon
       cmpi r14, NULL
       beq __vector__pop__back__decon_null
       
       ; Call deconstructor
       mtctr r14
       lw r14, struct_vector_dat(r3)
       lw r3, struct_vector_size(r3)
       muli r3, r3, sizeof_vector
       add r3, r3, r14
       
       ; r3 now holds the pointer 
       bctrl
       
       __vector__pop__back__decon_null:
       ; total_elements--;
       
       ;Cleanup
       lw r14, 0(r0)
       addi r0, r0, 4
       blr

vector_deconstruct:
       subi sp, sp, 16
       sw r14, 0(r0)
       mflr r14
       sw r14, 4(r0)
       sw r15, 8(r0)
       sw r16, 12(r0)
       ; Backup vec* dat
       mr r16, r3
       
       lw r14, struct_vector_decon(r3)   ; Get vector deconstruct 
       cmpi r14, NULL
       beq __vector__deconstruct__cleanup:
       
       ;Valid deconstruct funciton was found
       mtctr r14

       lw r15, struct_vector_size(r3)       ; Get size
       lw r14, struct_vector_dat(r3)        ; Get ptr
       
       __vector__deconstruct__loop:
              cmpi r15, 0
              ble __vector__deconstruct__cleanup
              
              mr r3, r14
              bctrl   ; ctr holds the deconstructor
              
              subi r15, r15, 1
              addi r14, 14, sizeof_vector
              b __vector__deconstruct__loop
       
       __vector__deconstruct__cleanup:
       ;free(vec->dat);
       lw r3, struct_vector_dat(r16)
       bl free
       
       lw r14, 4(r0)
       mflr r14
       lw r14, 0(r0)
       lw r15, 8(r0)
       lw r16, 12(r0)
       addi sp, sp, 16
       blr
       
vector_size:
       lw r3, struct_vector_size(r3)
       blr

vector_health:
       lw r3, struct_vector_buffer(r3)
       blr

vector_type_size:
       lw r3, struct_vector_type(r3)
       blr


