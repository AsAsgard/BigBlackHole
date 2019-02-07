!__________________________________________________________________________________________________
!______________Программа для считвания исходных шлаков по заданному адресу_________________________
!__________________________________________________________________________________________________
subroutine SHLAKIOLD(debug)
!__________________________________________________________________________________________________
!
!DEC$ ATTRIBUTES DLLEXPORT ::  SHLAKIOLD

use ShlakiDataModule
use iso_c_binding, only : C_CHAR, C_NULL_CHAR
!
character*128  ss,ShlakiAdress2,a
character*250 initial
character*128 SinglLetter,TEFtemp,SIMSHLtemp,NZSHLtemp
integer LenghtSingleString,iSHLAKI,iSm,iBURNF,Lenght_ShlakiAdress,Point1,Point2,Slash1,Slash2,jjSHL,kkSHL,jjSM,kkSM,jjPM,kkPM,Deallocateindex,debug
integer i,j,YesNoOr,SIMSHLindex1,SIMSHLindex2,NZSHLindex1,NZSHLindex2,TEFindex1,TEFindex2,ExIndex,iter,WSHLindex,WPMindex,WSMindex
real OneSHL, OnePM, OneSM

!__________________________________________________________________________________________________
   
     INTERFACE
    SUBROUTINE ReadAdress(FileAdress,SizeFileAdress,Deallocateindex) BIND(C,NAME='ReadAdress')
      use iso_c_binding
      integer(C_INT) :: SizeFileAdress ,Deallocateindex
     character(kind=C_CHAR), dimension(128), intent(out) :: FileAdress 
    END SUBROUTINE ReadAdress
  END INTERFACE
        
          INTERFACE
    SUBROUTINE param(NZL,ex) BIND(C,NAME='Param')
      use iso_c_binding
      integer(C_INT) :: NZL ,ex
    END SUBROUTINE param
          END INTERFACE
          
        
            INTERFACE
    SUBROUTINE mistakeadress_C() BIND(C,NAME='mistakeadress_C')
      use iso_c_binding, only : C_CHAR
    END SUBROUTINE mistakeadress_C
            END INTERFACE
            
           INTERFACE
    SUBROUTINE namelis() BIND(C,NAME='NameList')
      use iso_c_binding
    END SUBROUTINE namelis
        END INTERFACE
            
iBURNF = 0  ! индикатор
iSHLAKI = 0 ! индикатор записи записи Slaki.dat
iSm = 0     ! индикатор записи Sm.dat
iPm = 0     ! индикатор записи Pm.dat
! 1.
!Узнаем дерикторию программы
if (debug==1) then
    open(999,file='ShlakiProtokol.dat') 
    write(999,'(/1x,a37)')'ShlakiConversion program has started.'
end if
iter=1
do While  (iter>0) 
   ShlakiAdress=''
   Lenght_OldShlakiName=0
   SIMSHLtemp=''
   NZSHLtemp=''
   TEFtemp=''
   iBURNF=0
   iSHLAKI = 0 
   iSm = 0    
   iPm = 0 
 call ReadAdress(ShlakiAdress,Lenght_OldShlakiName,Deallocateindex)
 print*,ShlakiAdress
 if (Deallocateindex/=0) then
     deallocate(Shlaki,Sm,Pm)
 end if
! Исходное имя файла со шлаками - символы от последнего "/" до пробела в конце строки.
 call namelis()
 
jjSHL=1
kkSHL=1

jjSM=1
kkSM=1

jjPM=1
kkPM=1

open(2,file=ShlakiAdress)
!


do while(.true.)                      ! цикл считывания символьных данных из файла. Файл читается от начала до конца.
!*
 read(2,'(a)', end=999)SinglLetter! считываем i-ую строку.
 LenghtSingleString = len_trim(SinglLetter) ! длина i-ой строки без завершающего пробела.
!__________________________________________________________________________________________________BURNF
do j=1,LenghtSingleString
 if(SinglLetter(j:j)=='B'.and.SinglLetter(j+1:j+1)=='U'.and.SinglLetter(j+2:j+2)=='R'.and. SinglLetter(j+3:j+3)=='N'.and.SinglLetter(j+4:j+4)=='F')then
  iBURNF = iBURNF + 1   ! считываем только первый неймлист BURNF. 
    if(iBURNF==2)then 
        if (debug==1) then
             write(999,'(1x,a56)')'There is much then one BURNF namelists! Used only first.'
        end if
     goto 2            ! если есть второй неймлист BURNF, останавливаем чтение файла.
    endif
 endif    
enddo
!
 do j=1,LenghtSingleString  ! завершение чтения строки при обнаружении символа открытия/закрытия неймлиста "&".
    if(SinglLetter(j:j)=='&')goto 1
 enddo
!
!
 do j=1,LenghtSingleString  ! начало цикла поиска в считанной строке.
!__________________________________________________________________________________________________ищем TEF     
  if(SinglLetter(j:j)=='T'.and.SinglLetter(j+1:j+1)=='E'.and.SinglLetter(j+2:j+2)=='F')then
    TEFtemp = SinglLetter
          TEFindex1=index(TEFtemp,'=') 
          TEFindex2=index(TEFtemp,',')
          read(TEFtemp(TEFindex1+1:TEFindex2-1),*) TEF
    exit
  endif
!__________________________________________________________________________________________________ищем SIMSHL.
 if(SinglLetter(j:j)=='S'.and.SinglLetter(j+1:j+1)=='I'.and.SinglLetter(j+2:j+2)=='M'.and. SinglLetter(j+3:j+3)=='S'.and.SinglLetter(j+4:j+4)=='H'.and.SinglLetter(j+5:j+5)=='L')then
    SIMSHLtemp = SinglLetter
         SIMSHLindex=index(SIMSHLtemp,'=') 
         SIMSHLindex2=index(SIMSHLtemp,',')
         read(SIMSHLtemp(SIMSHLindex+1:SIMSHLindex2-1),*) SIMSHL
    exit
  endif
!__________________________________________________________________________________________________ищем NZSHL.
  if(SinglLetter(j:j)=='N'.and.SinglLetter(j+1:j+1)=='Z'.and.SinglLetter(j+2:j+2)=='S'.and. SinglLetter(j+3:j+3)=='H'.and.SinglLetter(j+4:j+4)=='L')then
    NZSHLtemp = SinglLetter
         NZSHLindex1=index(NZSHLtemp,'=') 
         NZSHLindex2=index(NZSHLtemp,',')
         if (NZSHLindex2==0) NZSHLindex2=30
         read(NZSHLtemp(NZSHLindex1+1:NZSHLindex2-1),*) NZSHL
         allocate(Shlaki(NZSHL,nTVS))
         allocate(Sm(NZSHL,nTVS))
         allocate(Pm(NZSHL,nTVS))
    exit
  endif
!__________________________________________________________________________________________________ищем SHLAKI.
  if(SinglLetter(j:j)=='S'.and.SinglLetter(j+1:j+1)=='H'.and.SinglLetter(j+2:j+2)=='L'.and. SinglLetter(j+3:j+3)=='A'.and.SinglLetter(j+4:j+4)=='K'.and.SinglLetter(j+5:j+5)=='I')then
    SinglLetter=''
    iSHLAKI = 1
    iSm = 0
    iPm = 0
    exit
  endif   
!__________________________________________________________________________________________________ищем Sm.
  if(SinglLetter(j:j)=='S'.and.SinglLetter(j+1:j+1)=='M')then
    SinglLetter=''
    iSHLAKI = 0
    iSm = 1
    iPm = 0
    exit
  endif  
  if(SinglLetter(j:j)=='S'.and.SinglLetter(j+1:j+1)=='m')then
    SinglLetter=''
    iSHLAKI = 0
    iSm = 1
    iPm = 0
    exit
  endif  
!__________________________________________________________________________________________________ищем Pm.
  if(SinglLetter(j:j)=='P'.and.SinglLetter(j+1:j+1)=='M')then
    SinglLetter=''
    iSHLAKI = 0
    iSm = 0
    iPm = 1
    exit
  endif  
    if(SinglLetter(j:j)=='P'.and.SinglLetter(j+1:j+1)=='m')then
    SinglLetter=''
    iSHLAKI = 0
    iSm = 0
    iPm = 1
    exit
  endif  
!__________________________________________________________________________________________________ищем End.
  if(SinglLetter(j:j)=='&'.and.SinglLetter(j+1:j+1)=='E'.and.SinglLetter(j+2:j+2)=='N'.and.SinglLetter(j+3:j+3)=='D')then
    iSHLAKI = 0
    iSm = 0
    iPm = 0
    exit
  endif    
!
 enddo  ! конец цикла поиска в считанной строке.
!
! 
 ! if(iSHLAKI==1.and.iSm==0.and.iPm==0)write(4,'(a128)')SinglLetter  ! запись строки для Shlaki.dat
 ! if(iSHLAKI==0.and.iSm==1.and.iPm==0)write(5,'(a128)')SinglLetter  ! запись строки для Sm.dat   
 ! if(iSHLAKI==0.and.iSm==0.and.iPm==1)write(6,'(a128)')SinglLetter  ! запись строки для Pm.dat
 ! Запись в массив шлаки
 !print*,SinglLetter
 if(iSHLAKI==1.and.iSm==0.and.iPm==0) then
    WSHLindex=index(SinglLetter,',')
    if (WSHLindex>0) then
    a=SinglLetter(1:WSHLindex-1)
    read(a,*) OneSHL
    Shlaki(jjSHL,kkSHL)=OneSHL
    jjSHL=jjSHL+1
        if (jjSHL>NZSHL) then
            kkSHL=kkSHL+1
            jjSHL=1
        end if
    end if
    do i=1,9
        SinglLetter=SinglLetter(WSHLindex+1:)
        WSHLindex=index(SinglLetter,',')
        if (WSHLindex>0) then
        a=SinglLetter(1:WSHLindex-1)
        read(a,*) OneSHL
        Shlaki(jjSHL,kkSHL)=OneSHL
        jjSHL=jjSHL+1
             if (jjSHL>NZSHL) then
                  kkSHL=kkSHL+1
                  jjSHL=1
             end if
        end if
    end do
 end if
 ! Запись в массив сомарий
 if(iSHLAKI==0.and.iSm==1.and.iPm==0) then
    WSMindex=index(SinglLetter,',')
    if (WSMindex>0) then
    a=SinglLetter(1:WSMindex-1)
    read(a,*) OneSM
    Sm(jjSM,kkSM)=OneSM
    jjSM=jjSM+1
        if (jjSM>NZSHL) then
            kkSM=kkSM+1
            jjSM=1
        end if
    end if
    do i=1, 9
        SinglLetter=SinglLetter(WSMindex+1:)
        WSMindex=index(SinglLetter,',')
        if (WSMindex>0) then
        a=SinglLetter(1:WSMindex-1)
        read(a,*) OneSM
        Sm(jjSM,kkSM)=OneSM
        jjSM=jjSM+1
            if (jjSM>NZSHL) then
               kkSM=kkSM+1
               jjSM=1
            end if
        end if
    end do
 end if
 ! Запись в массив прометий
 if(iSHLAKI==0.and.iSm==0.and.iPm==1) then
    WPMindex=index(SinglLetter,',')
    if (WPMindex>0) then
    a=SinglLetter(1:WPMindex-1)
    read(a,*) OnePM
    Pm(jjPM,kkPM)=OnePM
    jjPM=jjPM+1
        if (jjPM>NZSHL) then
            kkPM=kkPM+1
            jjPM=1
        end if
    end if
    do i=1, 9
        SinglLetter=SinglLetter(WPMindex+1:)
        WPMindex=index(SinglLetter,',')
        if (WPMindex>0) then
        a=SinglLetter(1:WPMindex-1)
        read(a,*) OnePM
        Pm(jjPM,kkPM)=OnePM
        jjPM=jjPM+1
            if (jjPM>NZSHL) then
               kkPM=kkPM+1
               jjPM=1
            end if
        end if
    end do
 end if
1 continue
!*
!*
enddo   ! конец цикла по считыванию исходного файла.
999 continue
2 close(2)
  

  
!  SIMSHLindex=index(SIMSHLtemp,'=') 
!  SIMSHLindex2=index(SIMSHLtemp,',')
!  read(SIMSHLtemp(SIMSHLindex+1:SIMSHLindex2-1),*) SIMSHL
!  NZSHLindex1=index(NZSHLtemp,'=') 
!  NZSHLindex2=index(NZSHLtemp,',')
!  read(NZSHLtemp(NZSHLindex1+1:NZSHLindex2-1),*) NZSHL
!  TEFindex1=index(TEFtemp,'=') 
!  TEFindex2=index(TEFtemp,',')
!  read(TEFtemp(TEFindex1+1:TEFindex2-1),*) TEF
  
 ExIndex=0
 print*,'Hello'
 call param(NZSHL,ExIndex)
 print*,'Hello'
 if (ExIndex/=1) then 
     iter=-1
 end if
end do
close(999)
 return
endsubroutine
