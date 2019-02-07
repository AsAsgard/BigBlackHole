!------------------------------------------------------------------------------------------------------------------------------------------
!--------------Программа формирования сортов топлива расчета ИР на основе сортов топлива комплекса КАСКАД----------------------------------
!--------------------------------[результат работы программы массив FuelLibrary(:,:)]_____-------------------------------------------------
!------------------------------------------------------------------------------------------------------------------------------------------
subroutine FUELSORTSLIBRARY(ExitCode,adress,len_dirC,debug)
!DEC$ ATTRIBUTES DLLEXPORT ::  FUELSORTSLIBRARY
!задаем библиотеки данных
use user_   !user.set
use master_ !master.set
use bippar_ !bippar file
use burboc_ !burboc file
Use FuelSortsModule !данные для формирования библиотеки сортов топлива ИР
use DataFilesModule
use iso_c_binding, only : C_CHAR, C_NULL_CHAR
!
implicit none
integer,allocatable:: GlobalFuelSortsListTemp(:)
integer j,i,i_,k,ii,jj,key,iCampaign, &         ! переменные цикла
       TempFuelSortsList(163),       &            ! временный массив для размещения сортов топлива. Размерность массива - число ТВС в АЗ.
       ExitCode,FirstNumber,iFirstNumber,len_dirC,debug
character*300 BipparAdress, ErrorBipparAdressFile,adress             ! индикатор наличия файла bippar
CHARACTER*2 B       
logical exists
!burboc!
INTERFACE
    SUBROUTINE PRINT_C(Text) BIND(C,NAME='PRINT')
        use iso_c_binding, only : C_CHAR, C_NULL_CHAR
        character(kind=C_CHAR), intent(in) :: Text 
    END SUBROUTINE PRINT_C
END INTERFACE

INTERFACE
    SUBROUTINE PRINT_int_C(Text,num) BIND(C,NAME='PRINT_INT')
        use iso_c_binding, only : C_CHAR, C_NULL_CHAR,C_INT
        integer(C_INT) :: num
        character(kind=C_CHAR), intent(in) :: Text 
    END SUBROUTINE PRINT_int_C
END INTERFACE

INTERFACE
    SUBROUTINE error_C(string) BIND(C,NAME='error_C')
        use iso_c_binding, only : C_CHAR
        character ( kind = C_CHAR ) :: string ( * )
    END SUBROUTINE error_C
END INTERFACE

INTERFACE
    SUBROUTINE error_C_bippar() BIND(C,NAME='error_C_bippar')
    END SUBROUTINE error_C_bippar
END INTERFACE

INTERFACE
    SUBROUTINE Clear() BIND(C,NAME='ClearDispley')
    use iso_c_binding
    END SUBROUTINE Clear
END INTERFACE

namelist /BURNF/ &
   TEF,simshl,NZSHL,SHLAKI,MAPN,KAMP,PM,SM
   
!bippar!
namelist /HIPI/ &
  AAA,AK,AKOJD,AKQDOP,AKODOP,AKRDOP,ALJOD,ALPM,ALXE,CBMAX,D1,DAKBT,DELB,DELW,DLKL,DW,EPSB,EPSF,EPSIN,EPSK,G,GN,H,HR,HZAZ,ITEMP,IVAR,IVERC,MAPSUZ,MAPTOP &
 ,MASGR,NAMCON,NAMSO,NB,NGR,NJ,NLIM,NPETEL,NR,NRG,NRST,NSST,NSUZ,NTOP,NTPK,NZ,PAP,PC3,PER,PK8,PKK,POCB1,POCB2,POPM2,SLAM,SMU,SOTV,SSSC,TDOP,TNOM,TS,TUS1,TUS2,TUS3,VAR,W,WDOP,WKBT,WLOW,WMIN,WUPP
namelist /FGRAN/ &
  CP,CP1,CP2,DGOR,DGOZB,DGOZH,DGSR,DGSZ,DLGST,DLOGG1,DLOGG2,DLOGS1,DLOGS2,DR,DR2,DS2L,DST,DTOR,DTOZB,DTOZH,DTSR,DTSZ,DZ,DZCBD,DZCBU,DZLOR,DZLOS,DZUPR,DZUPS,G1DR,G1DR2,G2DR &
 ,G2DR2,GAMMA,HPST,KTSR,KTSS,NDREF,P0DR,P1,P1DZ,P2,P2DZ,P3DZ,PDES,PDR,PDR2,PDST,Q1,Q2,S1DR,S1DR2,S2DR,S2DR2,TN,TRHRO
!namelist /HIPI1/ &
!  ITYPS,ITYPT,NUS,NUT,ULENS,ULENT
!namelist /HIPI2/ &
!  ECON,NCHUD  
!
!__________________________________________________________________________________________________________________________________________
!____________________________________________________________________________________________Проверяем наличие файла BipparAdress.dat
!ErrorBipparAdressFile = 'BipparAdress.dat'
!INQUIRE(FILE=ErrorBipparAdressFile,exist=exists)  !проверка существования файла BipparAdress
!if(exists==.true.)then
!                    write(997,'(1x,a27)')'BipparAdress file is found.'
!                    print*,'BipparAdress file is found.'
!                   else
!                    write(997,'(1x,a38)')'ERROR! BipparAdress file is not found!'
!                    print*,'ERROR! BipparAdress file is not found!'
!                    ExitCode = 1
!                    return
!endif           
!open(1,file='BipparAdress.dat')                ! считываем адресс файла bippar
 !write(997,'(1x,a25)')'BipparAdress file opened.'
! print*,'BipparAdress file opened.'
 !read (1,*) BipparAdressData

if (debug==1) then
    open(997,file='ProtokolBippar.dat')
end if
len_dir = len_dirC
print*,adress
BipparAdress=''
BipparAdressData=''
 do i=1,len_dir
    if (adress(i:i)=='/') then
        BipparAdress(i:i) = '\'
        BipparAdressData (i:i) = '\'
    else
        BipparAdress(i:i) = adress(i:i)
        BipparAdressData (i:i) = adress(i:i)
    end if
 end do
 print*,'BipparAdress1',BipparAdress
 call Clear()
 k=len_dir+1
 if(k.eq.0)then
!____________________________________________________________________________________________Проверяем задание ссылка на папку с файлом bippar.
                    if (debug==1) then
                        write(997,'(1x,a62)')'ERROR! Bippar adress not read! Error in BipparAdress.dat file!'
                    end if
                    print*,'ERROR! Bippar adress not read! Error in BipparAdress.dat file!'
                    call error_C('ERROR! Bippar adress not read! Error in BipparAdress.dat file!')
                    ExitCode = 1
                    return
                    else
                    if (debug==1) then
                         write(997,'(1x,a19)')'Bippar adress read.'
                    end if
                    print*,'Bippar adress  read.'
                    !call print_c('Bippar adress  read.')
endif  
BipparAdress(k:k+7) = '\bippar'                   !_дописываем к строке адреса имя bippar, начиная с символа k (то есть пробел заменяется на \)
INQUIRE(FILE=BipparAdress(1:k+7),exist=exists)  !проверка существования файла bippar в кампании Ki
if(exists==.false.) then
    if (debug==1) then
        write(997,'(1x,a57)')'File bippar do not find in current folder! Program exit.'
    end if
 call error_C_bippar()
 print('(1x,a57)'),'File bippar do not find in current folder! Program exit.'
 goto 999
endif
call print_c('Bippar adress read.')
open(21,file=BipparAdress)         !открываем bippar
 read(21,HIPI,err=994)                       !считываем неймлисты из файла bippar 
 read(21,FGRAN,err=993)   
! read(21,HIPI1)
! read(21,HIPI2)
close(21)
!
j = 1
BipparAdress(k:k+1)='\K'
!INQUIRE(FILE=BipparAdress(1:k+1),exist=exists)
!print*,exists
!if(exists==.false.) BipparAdress(k:k+1)='\B'
do i=1,163                                                                                              
 TempFuelSortsList(i) = 0                                                               ! зануляем массив TempFuelSortsList
enddo

!----------------------------------------------------------------------------------------------------------------------------------------    
do iCampaign=1,50 !NumberOfCampaign                          !цикл перебора кампаний
!----------------------------------------------------------------------------------------------------------------------------------------    
 write(B,'(i2)')iCampaign
 BipparAdress(k+2:k+3)=B                              !вписываем в адресс папки номер кампании
 if(iCampaign.lt.10) BipparAdress(k+2:k+2)='0'
    BipparAdress(k+4:k+11)='\burboc'	                !создаем ссылку на файл burboc в папке кампании
    INQUIRE(FILE=BipparAdress(1:k+11),exist=exists)       !проверка существования файла burboc в кампании Ki 
    if(exists==.false.)cycle                                             !если нет, то смотрим следующую кампанию
 open(41,file=BipparAdress(1:k+11))
!____________________________________________________________________________________________Ошибки при считывании неймлистов файла burboc
  read(41,BURNF)
 close(41)
!____________________________________________________________________________________________Отчет о считывании файла burboc в кампании i.
if (debug==1) then
    write(997,'(1x,a43,i2)')'File burboc is successfully read in company',iCampaign
end if
 print('(1x,a43,i2)'),'File burboc is successfully read in company',iCampaign 
 call print_int_c('File burboc is successfully read in company',iCampaign)
 !__________________________________________________________________________________________________________________________________________
!_____________________________________ Получаем список сортов топлива из массива SOTV______________________________________________________
!__________________________________________________________________________________________________________________________________________
do i=1,163                                                                              ! извлекаем из массива SOTV сорта топлива   
 do ii=1,163
  if(MAPN(i).eq.TempFuelSortsList(ii))exit
  if(ii.eq.163)then
   TempFuelSortsList(j) = MAPN(i)
   j = j + 1
  endif 
 enddo
enddo
!____________________________________________________________________________________________Отчет о считывании файла burboc в кампании i.

enddo
DimentionOfSortsList = 0
do i=1,163
 if(TempFuelSortsList(i).ne.0)DimentionOfSortsList = DimentionOfSortsList + 1           ! определяем число сортов топлива
enddo
allocate(GlobalFuelSortsList(DimentionOfSortsList),GlobalFuelSortsListTemp(DimentionOfSortsList))                                     ! задаем размерность массива GlobalFuelSortsList
GlobalFuelSortsList = 0.
do i=1,DimentionOfSortsList                                                             ! GlobalFuelSortsList - массив без нулевых элементов
  GlobalFuelSortsList(i) = TempFuelSortsList(i)
enddo
                                                                                        ! Упорядочиваем номера топлива в GlobalFuelSortsList
do i=1,DimentionOfSortsList
 FirstNumber  = GlobalFuelSortsList(i)
 iFirstNumber = i
  do ii=i,DimentionOfSortsList
      if(FirstNumber.gt.GlobalFuelSortsList(ii))then
          FirstNumber  = GlobalFuelSortsList(ii)
          iFirstNumber = ii
      endif
  enddo
  GlobalFuelSortsList(iFirstNumber) = GlobalFuelSortsList(i)
  GlobalFuelSortsList(i) = FirstNumber
enddo
nNumbersConstantLibrary = 3                                                                     ! число номеров БК для описания сорта топлива    
allocate(FuelSortsList_IR(nNumbersConstantLibrary,DimentionOfSortsList))                        ! задаем размерность массива FuelSortsList_IR
do i=1,nNumbersConstantLibrary                                                                  ! зануляем массив FuelSortsList_IR
 do ii=1,DimentionOfSortsList   
  FuelSortsList_IR(i,ii)=0
 enddo
enddo  
!__________________________________________________________________________________________________________________________________________
!_____________________________________________________Формируем список сортов топлива ИР___________________________________________________
!__________________________________________________________________________________________________________________________________________
do i=1,DimentionOfSortsList                                                             ! выбираем сорт топлива i
!
 j = 1   
 do ii=1,600   
  if(j.gt.nNumbersConstantLibrary)exit                                                          ! ищем сорт топлива по массиву SOTV
  if(SOTV(1,ii).eq.GlobalFuelSortsList(i))then
   FuelSortsList_IR(j,i) = SOTV(4,ii)                                                   ! записываем номера библиотеки констант
   j = j + 1
  endif
 enddo
! 
enddo                                                                       
!Формируем массив соответствия сортов топлива FuelLibrary
! 1 элемент строки - сорт топлива в КАСКАДе
! 2 элемент строки - сорт топлива в ИР
! 3 элемент строки и далее - номер в библиотеке констант
 allocate(FuelLibrary(nNumbersConstantLibrary+2,DimentionOfSortsList))
 FuelLibrary = 0.
 do i=1,DimentionOfSortsList
  FuelLibrary(1,i) = GlobalFuelSortsList(i)    ! сорт топлива в КАСКАДе
  FuelLibrary(2,i) = GlobalFuelSortsList(i)    ! сорт топлива в ИР
  do ii=3,nNumbersConstantLibrary+2
   FuelLibrary(ii,i)= FuelSortsList_IR(ii-2,i)    ! номер в библиотеке констант
   if(FuelLibrary(ii,i).eq.0)FuelLibrary(ii,i)=FuelLibrary(ii-1,i)           ! если сорту топлива в SOTV соответствует один номер из БК,
  enddo                                                                      ! остальные элементы массива FuelLibrary заполняем этим номером.                                                                          
 enddo

 return
!__________________________________________________________________________________________________________________________________________
!3    
994 if (debug==1) then
    write(997,'(/1x,a28)')'ERROR reading HIPI namelist!'                                       !ошибка при считывании данных неймлиста HIPPI
    end if
    print*,'ERROR reading HIPI namelist!'
    call error_C('ERROR reading HIPI namelist!')
    ExitCode = 1
    goto 1
!4
993 if (debug==1) then
        write(997,'(/1x,a29)')'ERROR reading FGRAN namelist!'                                      !ошибка при считывании данных неймлиста FGRAN
    end if
    print*,'ERROR reading FGRAN namelist!' 
    call error_C('ERROR reading FGRAN namelist!')
    ExitCode = 1
    goto 1
999 continue
    if (debug==1) then
        close(997)
    end if
1 return
end