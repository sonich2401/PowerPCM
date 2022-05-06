testString: "Hello there!"
testInt: 1234
.macro testMacro: 213


main:
  li r3, testString
  bl puts
  
  li r3, testInt
  lw r3, 0(r3)
  bl puti
  li r3, 10
  sc 0
  
  li r3, testMacro
  bl puti
  li r3, 10
  sc 0
  
  lw r3, testMacro(r0)
  
  end 0
