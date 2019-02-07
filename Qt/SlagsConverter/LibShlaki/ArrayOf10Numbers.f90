module ArrayOf10Numbers
 character*1 ArrayOfNumbers(10)

contains

 subroutine ArrayOf_10Numbers
 !
  ArrayOfNumbers(1) = '1'
  ArrayOfNumbers(2) = '2'
  ArrayOfNumbers(3) = '3'
  ArrayOfNumbers(4) = '4'
  ArrayOfNumbers(5) = '5'
  ArrayOfNumbers(6) = '6'
  ArrayOfNumbers(7) = '7'
  ArrayOfNumbers(8) = '8'
  ArrayOfNumbers(9) = '9'
  ArrayOfNumbers(10) ='0'
 !
  return
 endsubroutine
endmodule
