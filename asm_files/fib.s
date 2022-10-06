.global start:
li 3, 20
bl 16
end 0

subi 0, 0, 8
sw 4, 4(0) 
sw 5, 0(0) 
li 4, 1
li 5, 0
add 4, 4, 5
sub 5, 4, 5
subi 3, 3, 1

cmpi 3, 0
bne 36
mr 3, 4
lw 5 0(0)
lw 4 4(0)
addi 0, 0, 8
blr
