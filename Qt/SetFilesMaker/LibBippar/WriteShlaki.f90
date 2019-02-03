! Программа записи файла шлаков. Файл шлаков пишется в той же директории, что и файл user.set.
subroutine WriteShlaki(FileNameShlaki,iCampaign)
!задаем библиотеки данных
use user_   !user.set
use master_ !master.set
use bippar_ !bippar file
use burboc_ !burboc file
Use FuelSortsModule
implicit none
!описываем переменные
 real,allocatable:: ShlakiIRset(:),PmIRset(:),SmIRset(:) 
 character*300 FileNameShlaki
 integer i,ii,iii,j,iCampaign,nShlaki,nTVS,KeyNewString,NewTVSInShlaki,NewTVSInShlakiIRset
 !
nTVS=163                        ! число ТВС в активной зоне
nShlaki=NZSHL*nTVS              ! размерность массива шлаков
allocate(ShlakiIRset(nShlaki))  ! задаем размерность массива шлаков
allocate(PmIRset(nShlaki))
allocate(SmIRset(nShlaki))
! 1
if(SIMSHL.eq.1)then
                    do i=1,nShlaki
                       ShlakiIRset(i)=Shlaki(i)
                       PmIRset(i)=Pm(i)
                       SmIRset(i)=Sm(i)
                    enddo
endif
! 3
if(SIMSHL.eq.3)then
 do i=1,55                                              ! перебираем 55 кассет в симметрии 120 градусов
    NewTVSInShlaki = (i-1)*NZSHL                        ! находим строку со шлаками для кассеты i в массиве Shlaki
    do ii=1,163                                         ! перебираем кассеты с 1 по 163
     if(i.eq.DiffrentSimmetry(2,ii))then
       NewTVSInShlakiIRset = (ii-1)*NZSHL                 ! находим строку со шлаками для кассеты i в массиве ShlakiIRset  
       do iii=1,NZSHL
        ShlakiIRset(NewTVSInShlakiIRset+iii)=Shlaki(NewTVSInShlaki+iii)                         ! разворачиваем загрузку шлаков
        PmIRset(NewTVSInShlakiIRset+iii)=Pm(NewTVSInShlaki+iii)
        SmIRset(NewTVSInShlakiIRset+iii)=Sm(NewTVSInShlaki+iii)
       enddo 
     endif
    enddo
 enddo
endif
! 6
if(SIMSHL.eq.6)then                                     
 do i=1,28                                              ! перебираем 28 кассет в симметрии 60 градусов
     NewTVSInShlaki = (i-1)*NZSHL                       ! находим строку со шлаками для кассеты i в массиве Shlaki
    do ii=1,163                                         ! перебираем кассеты с 1 по 163
     if(i.eq.DiffrentSimmetry(3,ii))then        
       NewTVSInShlakiIRset = (ii-1)*NZSHL                 ! находим строку со шлаками для кассеты i в массиве ShlakiIRset  
       do iii=1,NZSHL
        ShlakiIRset(NewTVSInShlakiIRset+iii)=Shlaki(NewTVSInShlaki+iii)                         ! разворачиваем загрузку шлаков
        PmIRset(NewTVSInShlakiIRset+iii)=Pm(NewTVSInShlaki+iii)
        SmIRset(NewTVSInShlakiIRset+iii)=Sm(NewTVSInShlaki+iii)
       enddo
     endif
    enddo
 enddo
endif
 open(6,file=FileNameShlaki)
  write(6,'(a6)') '&BURNF'
  write(6,'(a4,f6.2,a1)')'TEF=',TEF,','
  write(6,'(a7,f2.0,a1)')'SIMSHL=',SIMSHL,','
  write(6,'(a6,i2,a1)')'NZSHL=',NZSHL,','
 !shlaki 
  write(6,'(a7)')'SHLAKI='
  KeyNewString=1
  do i=1,nShlaki
    if(KeyNewString.eq.10)then
     write(6,'(f6.3,a1)')ShlakiIRset(i),','
     KeyNewString=1
     goto 1
    endif    
     write(6,'(f6.3,a1\)')ShlakiIRset(i),','   
    KeyNewString = KeyNewString + 1
1   continue 
  enddo    
 !Sm
   write(6,'(a3)')'Sm='
  KeyNewString=1
  do i=1,nShlaki
    if(KeyNewString.eq.10)then
     write(6,'(f6.3,a1)')SmIRset(i),','
     KeyNewString=1
     goto 3
    endif    
     write(6,'(f6.3,a1\)')SmIRset(i),','   
    KeyNewString = KeyNewString + 1
3   continue 
  enddo  
 !Pm
   write(6,'(a3)')'Pm='
  KeyNewString=1
  do i=1,nShlaki
    if(KeyNewString.eq.10)then
     write(6,'(f6.3,a1)')PmIRset(i),','
     KeyNewString=1
     goto 2
    endif    
     write(6,'(f6.3,a1\)')PmIRset(i),','   
    KeyNewString = KeyNewString + 1
2   continue 
  enddo 

  write(6,'(/a4)') '&END'
 close(6)
 return
endsubroutine