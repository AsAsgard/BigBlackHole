!------------------------------------------------------------------------------------------------------------------------------------------
!--------------------------------Подпрограмма записи массива TOPL в файл user.set----------------------------------------------------------
!------------------------------------------------------------------------------------------------------------------------------------------
subroutine WriteTOPL
use user_                                       ! передается массив TOPL
implicit none
integer M                                       ! переменная цикла
!__________________________________________________________________________________________________________________________________________
write(5,'(2/a6)') ' TOPL='
! 15 stroka
write(5,'(1x,i2,a1\)') TOPL(1), ','
do M=2,5
write(5,'(i2,a1\)') TOPL(M), ','
enddo
write(5,'(i2,a1)') TOPL(6), ','
! 14 stroka
write(5,'(1x,i2,a1\)') TOPL(7), ','
do M=8,14
write(5,'(i2,a1\)') TOPL(M), ','
enddo
write(5,'(i2,a1)') TOPL(15), ','
! 13 stroka
write(5,'(1x,i2,a1\)') TOPL(16), ','
do M=17,24
write(5,'(i2,a1\)') TOPL(M), ','
enddo
write(5,'(i2,a1)') TOPL(25), ','
! 12 stroka
write(5,'(1x,i2,a1\)') TOPL(26), ','
do M=27,35
write(5,'(i2,a1\)') TOPL(M), ','
enddo
write(5,'(i2,a1)') TOPL(36), ','
! 11 stroka
write(5,'(1x,i2,a1\)') TOPL(37), ','
do M=38,47
write(5,'(i2,a1\)') TOPL(M), ','
enddo
write(5,'(i2,a1)') TOPL(48), ','
! 10 stroka
write(5,'(1x,i2,a1\)') TOPL(49), ','
do M=50,60
write(5,'(i2,a1\)') TOPL(M), ','
enddo
write(5,'(i2,a1)') TOPL(61), ','
! 9 stroka
write(5,'(1x,i2,a1\)') TOPL(62), ','
do M=63,74
write(5,'(i2,a1\)') TOPL(M), ','
enddo
write(5,'(i2,a1)') TOPL(75), ','
! 8 stroka
write(5,'(1x,i2,a1\)') TOPL(76), ','
do M=77,87
write(5,'(i2,a1\)') TOPL(M), ','
enddo
write(5,'(i2,a1)') TOPL(88), ','
! 7 stroka
write(5,'(1x,i2,a1\)') TOPL(89), ','
do M=90,101
write(5,'(i2,a1\)') TOPL(M), ','
enddo
write(5,'(i2,a1)') TOPL(102), ','
! 6 stroka
write(5,'(1x,i2,a1\)') TOPL(103), ','
do M=104,114
write(5,'(i2,a1\)') TOPL(M), ','
enddo
write(5,'(i2,a1)') TOPL(115), ','
! 5 stroka
write(5,'(1x,i2,a1\)') TOPL(116), ','
do M=117,126
write(5,'(i2,a1\)') TOPL(M), ','
enddo
write(5,'(i2,a1)') TOPL(127), ','
! 4 stroka
write(5,'(1x,i2,a1\)') TOPL(128), ','
do M=129,137
write(5,'(i2,a1\)') TOPL(M), ','
enddo
write(5,'(i2,a1)') TOPL(138), ','
! 3 stroka
write(5,'(1x,i2,a1\)') TOPL(139), ','
do M=140,147
write(5,'(i2,a1\)') TOPL(M), ','
enddo
write(5,'(i2,a1)') TOPL(148), ','
! 2 stroka
write(5,'(1x,i2,a1\)') TOPL(149), ','
do M=150,156
write(5,'(i2,a1\)') TOPL(M), ','
enddo
write(5,'(i2,a1)') TOPL(157), ','
! 1 stroka
write(5,'(1x,i2,a1\)') TOPL(158), ','
do M=159,162
write(5,'(i2,a1\)') TOPL(M), ','
enddo
write(5,'(i2,a1)') TOPL(163), ','

!
 return
end