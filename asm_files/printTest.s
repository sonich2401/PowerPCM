question: "What is your age? "
answer: "You are "
answer2: "? Cool! Me too!"
Format: "%u"

main:
  li r3, question
  bl put
  
  ;Get strack space for the saved value
  subi r0, r0, 4
  mr r4, r0 ;Set address of string
  li r3, Format ; %u
  bl scanf
  li r3, answer
  bl put
  
  lw r3, 0(r0)       ;Get number that was enerted
  bl puti
  
  ;Prints rest of meesage
  li r3, answer2
  bl puts
  
  ;Exit
  addi r0, r0, 4
  end 0
  
