!Программа записи файла user.set
subroutine WriteUserSet(FileNameUserSet,iCampaign,ErrorSOTV,debug)
!задаем библиотеки данных
use user_   !user.set
use master_ !master.set
use bippar_ !bippar file
use burboc_ !burboc file
Use FuelSortsModule
implicit none
!описываем переменные
!__________________
integer, allocatable:: SinglFuelSortsList(:)
real PK8_IR(20,3),D1_IR(20,3),PC3_IR(20,3)
integer SortsList(163),nFuelSorts,nSorta, &
        SingleMAPN,SingleKAMP,RealMAPN(163),iCampaign,ErrorSort,MapnDetailed(163),KampDetailed(163),lenght_sssc,FirstNumber,iFirstNumber,nStroki
!__________________
character*300 FileNameUserSet
character*13 stroka1,stroka2,stroka3,stroka4
character*30 namlib,ssscIR
character*128 NamconWork,NameShlString,NameLibString,NameNppString
integer NameShlStringLenght,NameLibStringLenght,StartPoinNameLib,Point1,Point2,NameLibLenght,NameNppStringLenght,debug
character*1 B1,CampaignNumber_Down10
CHARACTER*2 B2,CampaignNumber_Up10
CHARACTER(len=3) C
CHARACTER*5 D
character*1:: ForSorts = "'"    !кавычки(' ') для Sorts(i)
integer i,i_,k,rrr,j,l,ll,ii,iii,nn,MAPN_(163),M,key,jj,jjj,jjj_,nnn,jj_,l_,klen
integer  ErrorSOTV    !ошибка заполнения исходного массива SOTV. Недостаточное число элементов в массиве.   
!
INTERFACE
    SUBROUTINE PRINT_intUSER_C(Text1,num,Text2) BIND(C,NAME='PRINT_INTUSER')
        use iso_c_binding, only : C_CHAR, C_NULL_CHAR,C_INT
        integer(C_INT) :: num
        character(kind=C_CHAR), intent(in) :: Text1,Text2
    END SUBROUTINE PRINT_intUSER_C
END INTERFACE


!open(7,file='DifferentSimmetryTable.dat')
! write(7,'(3(a3,2x))')'360','120','60'
! write(7,*)
! do j=1,163
!  write(7,'(i3,2x,i3,2x,i3,2x)')DiffrentSimmetry(1,j),DiffrentSimmetry(2,j),DiffrentSimmetry(3,j)
! enddo
!close(7)

open(5,file=FileNameUserSet)!,delim='apostrophe') !создали файл userKxx.set
!
 write(5,'(a5)') '&USER' ! заголовой файлаю
!_________________________________________________! строка с адресом файла шлаков.
 NameShlString = "NAME_SHL= '.\SET\SHL\shlK0i',"           
 NameShlStringLenght = index(NameShlString,'i')  ! длина строки адреса шлаков до символа i.
 if(iCampaign.lt.10)then                         ! записываем номер кампании после символа K.
                    write(CampaignNumber_Down10,'(i1)')iCampaign
                    NameShlString(NameShlStringLenght:NameShlStringLenght) = CampaignNumber_Down10
                   else
                    write(CampaignNumber_Up10,'(i2)')iCampaign
                    NameShlString(NameShlStringLenght-1:NameShlStringLenght) = CampaignNumber_Up10
 endif
 write(5,'(/1x,a128)')NameShlString
!_________________________________________________
! NAMCON - адрес библиотеки в файле bippar	
! из адреса библиотеки выписываем имя файла без расширения
NamconWork = NAMCON
Point1  = index(NamconWork,' ')	                ! позиция пробела после имени файла
Point2 = index(NamconWork,'\')                  ! позиция первого "\"
do i = Point2,Point1                            ! цикл поиска позиция "\" перед именем файла, ищем от первого "\" до " "   
 if(NAMCON(i:i).eq.'\') then
     StartPoinNameLib = i                       !записываем позиции "\" начиная с первого. Последняя запись - искомый \ перед именем файла.
 endif
enddo
NameLibLenght = Point1 - StartPoinNameLib - 1   ! вычисляем длину имена файла библиотек.
                                                ! Формируем запись адреса библиотеки в файл.
NameLibString = "NAME_LIB= '.\SET\LIB\-"
NameLibStringLenght = index(NameLibString,'-')
NameLibString(NameLibStringLenght:NameLibStringLenght+NameLibLenght) = NamconWork(StartPoinNameLib+1:Point1-1) ! адрес библиотеки констант в программе ИР
NameLibString(NameLibStringLenght+NameLibLenght:NameLibStringLenght+NameLibLenght+1) = "',"
write(5,'(1x,a128)')NameLibString
!_________________________________________________
NameNppString = "NAME_NPP= '"
NameNppStringLenght = index(NameNppString,"'")
lenght_sssc=index(sssc,' ')
NameNppString(NameNppStringLenght+1:NameNppStringLenght + 1 + lenght_sssc)=sssc
NameNppString(NameNppStringLenght + lenght_sssc:NameNppStringLenght + lenght_sssc + 1)="',"
write(5,'(1x,a128)')NameNppString
!_________________________________________________
write(5,'(a12,i2,a1)') 'NOM_BLOCK= ',NB,','
write(5,'(a12,i2,a1)') 'NOM_CYCLE= ',iCampaign,','
write(5,'(a4,i1,a1)') 'T= ',T,','
write(5,'(a7,i2,a1)') 'NNRG= ',NRG,','
write(5,'(a8,i2,a1)') 'NNRG_= ',NRG-2,','
write(5,'(/1x,a6,i1,a1/)') 'IVERC=',IVERC,','
!________________________________________________________________________________________________________________
!_______________________________Формируем истинную загрузку______________________________________________________
!________________________________________________________________________________________________________________
! SIMSHL - индекс симметрии
! 1 - симметрия 360 градусов
! 3 - симметрия 120 градусов
! 6 - симметрия  60 градусов
!
! 1
if(SIMSHL.eq.1)then
    do i=1,163                                          ! перебираем кассеты с 1 по 163
       MapnDetailed(i) = MAPN(i)
       KampDetailed(i) = KAMP(i)
    enddo
endif
! 3
if(SIMSHL.eq.3)then
 do i=1,55
    do ii=1,163                                         ! перебираем кассеты с 1 по 163
     if(i.eq.DiffrentSimmetry(2,ii))then         
       MapnDetailed(DiffrentSimmetry(1,ii)) = MAPN(i)   ! разворачиваем топливную загрузку
       KampDetailed(DiffrentSimmetry(1,ii)) = KAMP(i)
     endif
    enddo
 enddo
endif
! 6
if(SIMSHL.eq.6)then
 do i=1,28
    do ii=1,163                                         ! перебираем кассеты с 1 по 163
     if(i.eq.DiffrentSimmetry(3,ii))then        
       MapnDetailed(DiffrentSimmetry(1,ii)) = MAPN(i)   ! разворачиваем топливную загрузку
       KampDetailed(DiffrentSimmetry(1,ii)) = KAMP(i)
       if(DiffrentSimmetry(1,ii).eq.116)then
        continue
       endif 
     endif
    enddo
 enddo
endif
do i=1,163                                  ! На основе данных массивов KampDetailed и NTOP формируем истинную загрузку
 SingleMAPN = MapnDetailed(i)                       ! в данной кампании - массив RealMAPN.    
 SingleKAMP = KampDetailed(i)
 do ii = 1,300
    if(NTOP(1,ii).eq.SingleMAPN)then
        RealMAPN(i) = NTOP(SingleKAMP,ii)
    endif
 enddo
enddo
!________________________________________________________________________________________________________________
j=1
do i=1,163
 SortsList(i) = 0                                                           ! Зануляем массив для избежания ошибок поиска сортов топлива.
enddo
do i=1,163
 do ii=1,163
  if(RealMAPN(i).eq.SortsList(ii))exit                                      ! Обрабатываем массив MAPN - массив сортов топлива.
  if(ii.eq.163)then
   SortsList(j) = RealMAPN(i)                                               ! Находим число разных типов ТВС и записываем их SortsList
   j = j + 1
  endif 
 enddo
enddo
nFuelSorts = 0
do i=1,163
 if(SortsList(i).ne.0)nFuelSorts = nFuelSorts + 1                           ! Подсчитываем число разлиичных сортов топлива
enddo                                                                       ! в данной кампании.
allocate(SinglFuelSortsList(nFuelSorts))                                    ! Задаем размерность массива сортов топлива данной кампании.
SinglFuelSortsList = 0.
                                                                            ! SinglFuelSortsList - это массив SortsList без нулей.            
!_____________________________________________________________________________Вывод числа сортов топлива в данной кампании.
if (debug==1) then
 write(997,'(1x,a8,1x,i2,1x,a17)')'There is',nFuelSorts,'varieties of TVS.'
end if
 call PRINT_intUSER_C('There is',nFuelSorts,'varieties of TVS.')
 print('(1x,a8,1x,i2,1x,a17)'),'There is',nFuelSorts,'varieties of TVS.'
do i=1,nFuelSorts                                                           ! Проверяем все ли сорта топлива данной кампании 
 do ii=1,DimentionOfSortsList                                               ! описаны в массиве SOTV.            
    if(SortsList(i).eq.FuelLibrary(1,ii))exit
    if(ii.eq.DimentionOfSortsList)then
        ErrorSort = SortsList(i)                                            ! сорт топлива не описанный в SOTV.
!_____________________________________________________________________________Ошибка. Сорт топлива не описан в массиве SOTV!        
        goto 1
    endif
 enddo
enddo
!---------------------- 
! Заполняем массив TOPL.
!---------------------- 
do i=1,nFuelSorts                                               ! для каждого сорта топлива
 SinglFuelSortsList(i) = SortsList(i)                           ! 
 do ii=1,163                                                    ! перебираем все ТВС
!---- 
  if(RealMAPN(ii).eq.SinglFuelSortsList(i))then                 ! находим ТВС соответствующую i типу топлива
 !- 
   do iii=1,DimentionOfSortsList            
    if(SinglFuelSortsList(i).eq.FuelLibrary(1,iii))then    ! находим сорту топлива КАСКАД в полном перечне сортов топлива
        TOPL(ii) = FuelLibrary(2,iii)                      ! присваеваем ему соответсвтвующий сорт топлива ИР
        exit
    endif    
   enddo 
 !-  
  endif
!----  
 enddo 
enddo
!                                                           ! упорядочиваем SinglFuelSortsList
do i=1,nFuelSorts
 FirstNumber  = SinglFuelSortsList(i)
 iFirstNumber = i
  do ii=i,nFuelSorts
      if(FirstNumber.gt.SinglFuelSortsList(ii))then
          FirstNumber  = SinglFuelSortsList(ii)
          iFirstNumber = ii
      endif
  enddo
  SinglFuelSortsList(iFirstNumber) = SinglFuelSortsList(i)
  SinglFuelSortsList(i) = FirstNumber
enddo
!
!--------------------------
! Обрабатываем массив SOTV
!---------------------------
do i=1,nFuelSorts                       ! Выбираем из массива SOTV номера записи в БКА для соответствующих типов ТВС. Шаг 1.
 do ii=1,600                            ! Записываем массивы для каждого сорта топлива первым номером БКА для исключения зануления
  if(SOTV(1,ii).eq.SortsList(i))then    ! в случае использования только 1 номера из БКА для описания сорта топлива    
   do j=1,3
    PK8_IR(i,j) = PK8(SOTV(4,ii))
    D1_IR(i,j) = D1(SOTV(4,ii))
    PC3_IR(i,j) = PC3(SOTV(4,ii))
   enddo
  endif 
 enddo
enddo
 
do i=1,nFuelSorts                       ! Выбираем из массива SOTV номера записи в БКА для соответствующих типов ТВС. Шаг 2.
! j = 1                                  ! Перезаписываем массивы в тех случаях, когда для описания сорта топлива используется
 do ii=1,DimentionOfSortsList                            ! 3 номера БКА    
!  if(SOTV(1,ii).eq.SortsList(i))then
  if(SinglFuelSortsList(i).eq.FuelLibrary(1,ii))then
 !   nSorta = FuelLibrary(2,ii)
   !if(j.eq.nNumbersConstantLibrary+1)exit
  do j=1,nNumbersConstantLibrary 
   PK8_IR(i,j) = PK8(FuelLibrary(j+2,ii))    !(SOTV(4,ii))
   D1_IR(i,j) = D1(FuelLibrary(j+2,ii))      !(SOTV(4,ii))
   PC3_IR(i,j) = PC3(FuelLibrary(j+2,ii))    !(SOTV(4,ii))
  enddo 
   !j = j + 1
  endif 
 enddo                
enddo
!________________________________________________________________________________________________________________
!_____________________________________ Запись в файл блока KTOP,PK8,D1,PC3_______________________________________
!________________________________________________________________________________________________________________
stroka1='KTOP(   ,:)= '
stroka2='PK8(   ,:)= '
stroka3='D1(   ,:)= '
stroka4='PC3(   ,:)= '
allocate (Sorta_Nomera_Strok(300))
do i=1,nFuelSorts                                               ! Перебираем все сорта топлива.
!
 do ii=1,DimentionOfSortsList
    if(SinglFuelSortsList(i).eq.FuelLibrary(1,ii))then          ! Находим в списке сорт топлива КАСКАД
        nSorta = ii !FuelLibrary(2,ii)                              ! и соответствующий ему тип топлива ИР    
        exit
    endif    
 enddo
! 
 write(C,'(i3)')FuelLibrary(2,ii)                                                 ! записываем номер сорта топлива ИР
 Sorta_Nomera_Strok(i)=FuelLibrary(2,ii)
 stroka1(6:8) = C
!______________________________________________________________________Записываем в файл массив KTOP 
 write(5,'(1x,a12\)')stroka1(1:12)                      
 do j=3,nNumbersConstantLibrary+1
 write (5,'(i3,a1\)')FuelLibrary(j,nSorta),','
 enddo 
 write (5,'(i3,a1)')FuelLibrary(nNumbersConstantLibrary+2,nSorta),','
! 
enddo       
write(5,*)                                                          ! конец цикла перебора сортов топлива
!
! 2.
!
do i=1,nFuelSorts                                               ! Перебираем все сорта топлива.
!
 do ii=1,DimentionOfSortsList
    if(SinglFuelSortsList(i).eq.FuelLibrary(1,ii))then          ! Находим в списке сорт топлива КАСКАД
        nSorta = FuelLibrary(2,ii)                              ! и соответствующий ему тип топлива ИР    
        exit
    endif    
 enddo
! 
 write(C,'(i2)')nSorta                                                 ! записываем номер сорта топлива ИР
 stroka2(6:7)=C
!______________________________________________________________________PK8
write(5,'(1x,a11\)')stroka2(1:11)
j=1
 do while( j<=nNumbersConstantLibrary)
    klen=0
    if ((j+klen+1)>nNumbersConstantLibrary) exit
    do while (PK8_IR(i,j)==PK8_IR(i,j+klen+1)) 
        klen = klen + 1
        if ((j+klen+1)>nNumbersConstantLibrary) exit
    end do
if (klen>0) then
    write (5,'(i1,a1,f6.3,a1\)')klen+1,'*',PK8_IR(i,j),','
else
    write (5,'(f6.3,a1\)')PK8_IR(i,j),','
end if
    j=j+klen+1
 enddo 
 write(5,*)
!______________________________________________________________________
! 
enddo
write(5,*)
!
! 3.
!
do i=1,nFuelSorts                                               ! Перебираем все сорта топлива.
!
 do ii=1,DimentionOfSortsList
    if(SinglFuelSortsList(i).eq.FuelLibrary(1,ii))then          ! Находим в списке сорт топлива КАСКАД
        nSorta = FuelLibrary(2,ii)                              ! и соответствующий ему тип топлива ИР    
        exit
    endif    
 enddo
! 
write(C,'(i2)')nSorta                                                 ! записываем номер сорта топлива ИР
 stroka3(6:7)=C
!______________________________________________________________________D1
write(5,'(1x,a11\)')stroka3(1:11)
 !______________________________________________________________________
j=1
 do while( j<=nNumbersConstantLibrary)
    klen=0
    if ((j+klen+1)>nNumbersConstantLibrary) exit
    do while (D1_IR(i,j)==D1_IR(i,j+klen+1)) 
        klen = klen + 1
        if ((j+klen+1)>nNumbersConstantLibrary) exit
    end do
if (klen>0) then
    write (5,'(i1,a1,f3.1,a1\)')klen+1,'*',D1_IR(i,j),','
else
    write (5,'(f3.1,a1\)')D1_IR(i,j),','
end if
    j=j+klen+1
 enddo 
 write(5,*)

enddo
write(5,*)
!
! 4.
!
do i=1,nFuelSorts                                               ! Перебираем все сорта топлива.
!
 do ii=1,DimentionOfSortsList
    if(SinglFuelSortsList(i).eq.FuelLibrary(1,ii))then          ! Находим в списке сорт топлива КАСКАД
        nSorta = FuelLibrary(2,ii)                              ! и соответствующий ему тип топлива ИР    
        exit
    endif    
 enddo
! 
 write(C,'(i2)')nSorta                                                 ! записываем номер сорта топлива ИР
 stroka4(6:7)=C
!______________________________________________________________________PC3
write(5,'(1x,a11\)')stroka4(1:11)
 j=1
 do while( j<=nNumbersConstantLibrary)
    klen=0
    if ((j+klen+1)>nNumbersConstantLibrary) exit
    do while (PC3_IR(i,j)==PC3_IR(i,j+klen+1)) 
        klen = klen + 1
        if ((j+klen+1)>nNumbersConstantLibrary) exit
    end do
if (klen>0) then
    write (5,'(i1,a1,f12.9,a1\)')klen+1,'*',PC3_IR(i,j),','
else
    write (5,'(f12.9,a1\)')D1_IR(i,j),','
end if
    j=j+klen+1
 enddo 
 write(5,*)

enddo    
!________________________________________________________________________________________________________________
!____________________________________Запись массива TOPL в файл user.set_________________________________________
!________________________________________________________________________________________________________________
!
call WriteTOPL
!________________________________________________________________________________________________________________

write(5,*)
do ii=1,nFuelSorts
 do iii=1,DimentionOfSortsList
    if(SinglFuelSortsList(ii).eq.FuelLibrary(1,iii))then          ! Находим в списке сорт топлива КАСКАД
        nSorta = iii !FuelLibrary(2,iii)                              ! и соответствующий ему тип топлива ИР    
        exit
    endif    
 enddo
write(C,'(i2)') Sorta_Nomera_Strok(ii)
SORTS(ii)(5:6)=C
  do iii=1,300
    if(Sorta_Nomera_Strok(ii).eq.int(NTPK(1,iii)))then         
        nStroki = iii !FuelLibrary(2,iii)  
        print*,iii
        exit
    endif    
 enddo
stroka1='SORTS(  )='
stroka1(7:8)=C
write(5,'(1x,a)',advance='no') stroka1
write(5,'(a1,a5,a1,a1)') "'", NAMSO(iii), "'", ','
end do
write(5,'(1x,a,a)') 'DT_Prot=5.0,  DT_Arc=720.0,  DT_Print=0.0',','
write(5,'(/1x,a,a)') 'Tstep_ext=2.0',','
write(5,'(1x,a,a)') 'Tfin_ext=30.0',','
write(5,'(1x,a,a)') 'Pstep_ext=7.0',','
write(5,'(1x,a,a)') 'Pfin_ext=75.0',','
write(5,'(1x,a)')'StationaryOffset= '
do ii=1,20
 if(ii.le.19)write(5,'(1x,a)')'-3.0,'
 if(ii.eq.20)write(5,'(1x,a)')'-3.0,'
enddo 
write(5,'(a)') '&END'
close(5)
deallocate(Sorta_Nomera_Strok)
goto 2                                                                                  ! Обход блока вывода ошибок.
!_______________________________________________________________________________________  Ошибки.

1 if (debug==1) then
  write(997,'(1x,a11,1x,i3,1x,a20)')'There is no',ErrorSort,'sort of fuel in SOTV'    ! Сорт топлива не описан в массиве SOTV 
  end if
    write(5,'(1x,a11,1x,i3,1x,a20)')'There is no',ErrorSort,'sort of fuel in SOTV'
    ErrorSOTV=1
    print('(1x,a11,1x,i3,1x,a20)'),'There is no',ErrorSort,'sort of fuel in SOTV'
    call PRINT_intUSER_C('There is no',ErrorSort,'sort of fuel in SOTV')
!_______________________________________________________________________________________    
2 continue
 return
end