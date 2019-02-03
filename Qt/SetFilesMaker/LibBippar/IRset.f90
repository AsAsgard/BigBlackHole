subroutine GenerWriteProgram(debug)
!DEC$ ATTRIBUTES DLLEXPORT ::  GENERWRITEPROGRAM
!задаем библиотеки данных
use user_   !user.set
use master_ !master.set
use bippar_ !bippar file
use burboc_ !burboc file
use DataFilesModule ! 
use FuelSortsModule
implicit none
!описываем переменные
character*300 string,string1,& 
              ErrorBipparAdressFile, &                               !переменная для проверки наличия файла BipparAdress
              FileNameMasterSet,FileNameUserSet,FileNameShlaki,Help       !адрес папки для файла master.set и user.set, шлаков
CHARACTER*2 B
logical exists
integer i,i_,k,debug,key, &
        ErrorSOTV   !ошибка заполнения исходного массива SOTV.  

INTERFACE
    SUBROUTINE error_C(string) BIND(C,NAME='error_C')
        use iso_c_binding, only : C_CHAR
        character ( kind = C_CHAR ) :: string ( * )
    END SUBROUTINE error_C
END INTERFACE

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
    SUBROUTINE PRINT_NULL_C() BIND(C,NAME='PRINT_NULL')
    END SUBROUTINE PRINT_NULL_C
END INTERFACE
!namelist!
!bippar!
namelist /HIPI/ &
  AAA,AK,AKOJD,AKQDOP,AKODOP,AKRDOP,ALJOD,ALPM,ALXE,CBMAX,D1,DAKBT,DELB,DELW,DLKL,DW,EPSB,EPSF,EPSIN,EPSK,G,GN,H,HR,HZAZ,ITEMP,IVAR,IVERC,MAPSUZ,MAPTOP &
 ,MASGR,NAMCON,NAMSO,NB,NGR,NJ,NLIM,NPETEL,NR,NRG,NRST,NSST,NSUZ,NTOP,NTPK,NZ,PAP,PC3,PER,PK8,PKK,POCB1,POCB2,POPM2,SLAM,SMU,SOTV,SSSC,TDOP,TNOM,TS,TUS1,TUS2,TUS3,VAR,W,WDOP,WKBT,WLOW,WMIN,WUPP
namelist /FGRAN/ &
  CP,CP1,CP2,DGOR,DGOZB,DGOZH,DGSR,DGSZ,DLGST,DLOGG1,DLOGG2,DLOGS1,DLOGS2,DR,DR2,DS2L,DST,DTOR,DTOZB,DTOZH,DTSR,DTSZ,DZ,DZCBD,DZCBU,DZLOR,DZLOS,DZUPR,DZUPS,G1DR,G1DR2,G2DR &
 ,G2DR2,GAMMA,HPST,KTSR,KTSS,NDREF,P0DR,P1,P1DZ,P2,P2DZ,P3DZ,PDES,PDR,PDR2,PDST,Q1,Q2,S1DR,S1DR2,S2DR,S2DR2,TN,TRHRO
namelist /HIPI1/ &
  ITYPS,ITYPT,NUS,NUT,ULENS,ULENT
namelist /HIPI2/ &
  ECON,NCHUD
!burboc!
namelist /BURNF/ &
  TEF,simshl,NZSHL,SHLAKI,MAPN,KAMP,PM,SM
!user!
NAMELIST /USER/   &
       T,NNRG,NNRG_   &
      ,KTOP,TOPL,AK,PC3,D1,PK8   &
      ,RESULTNEW   &
      ,DT_Prot,DT_ARC,DT_Print   &
      ,M_Reload,Shifr_SVP   &
      ,Tstep_ext,Tfin_ext,Pstep_ext,Pfin_ext   &
      ,KSORT,SORTS   & 
      ,StationaryOffset  &
      ,IVERC 
!master!
NAMELIST /MASTER/   &
      UPR,DTeffday,TKK,TPCH,DTPCH,TPF,DTPF,NK,H,HR,   & 
      SIM,NR,NJ,NP,ML,NRL,NR3,HTOPL,KBT,G,   &
      HE,HW,W,DW,EPS,RODOP,WDOP,CBMAX,TDOP,KFF,KD,DKB,CB,DCB,M2,NS,   &
      BO,AI,BI,C1,Q1,Q2,TST,RLI,   &
      PKR,DRDT,PDT,PDRDN,RPOL,NST,SDV,H1,H2,PSPUSK,NSPG,DR,DZ,   &
      DLOGST,DST,P1,P2,PXE,TXE,DTXE,GJOD,GXE,ALJOD,ALXE,KTN,TKXE,   &
      TTEK,XKBT,TSM,RXEL,HDSTN,HST1,SIGXE,PSM,GPM,GSM,ALPM,ALSM,TKONSM,   &
      SIGSM,GN,KR,AAA,TPK,VAR,I10,TIN,PAP,   &
      DGOR,DGOZH,DGOZB,DGSR,DGSZ,DTOR,DTOZH,DTOZB,DTSR,DTSZ,D2SL,   &
      PPSI,PSS,PODR,P1DZ,P2DZ,P3DZ,PDST,PDLOG,PDR,KTSS,KTSR,HPST,   &
      DDR,DR2,DZUPPR,DZLOWR,DZLOWS,DZUPPS,PDR2,PDES,   &
      NACHXE,PKK,   &
      P1DZ_X,P2DZ_X,Pnastr,DPnastr0,Tnastr,   &
               TTN,TRHRO,TGAMMA,TDR,TDZ,TP1,TP2,   &
               TDR2,TDZUPR,TDZLOR,TP1DZ,TP2DZ,   &
               TDGOR,TDGOZH,TDGOZB,   &
               TDTOR,TDTOZH,TDTOZB,   &
               POCB1,POCB2,   &
              TUS1,TUS2,TUS3,   &
       N_fuel_rod_in_TWS,   &
       Key_AO_Nastr_CBPK,   &
       KEY_DEFINE_SIGN_AO,KEY_DEFINE_LIM_KV,KEY_DEFINE_LIM_KQ,   &
       H_UP_OVERLAP,H_DW_OVERLAP,KEY_OVERLAP,   &
       IRGM,   &
       KDPZ,KSDPZ,HDPZZ,NOMDPZ,DHDPZ,LimDPZnom,   &
     WEIGHT_G,   &
     VKONT1,   &
     dTin_dNt,   &
     DH,DHEFF,NRST,HST,DHORSUZ,dAOmg,NZ,   &
     KEY_UNDER_START,DEPTH_UNDER_START,   &
     ADD_CH3BO3_UNDER_START,HGR_UNDER_START,TIN_UNDER_START,   &
     LIM_CORRECT_CBPK,   &
     DHNGU,DHKH,DHKB,   &
     Screen_Gate_N,Screen_Gate_T,Screen_Gate_P,Screen_Gate_G,   &
     Screen_Gate_H,   &
     Time_break_refresh,   &
     DHSTEP,   &
     FTC,   &
     KEY_ADMISSION,   &
     KEYKVLIM,   &
     maxKq,maxKv,minKv,Hcross,  &
     KEY_ANdpz,  &
     KEY_NGU,  &
     ErrCH3BO3,   &
     Concentrate,Distillate,   &
     ErrorNT, ErrorTINT, ErrorOP, ErrorG,   &
     KEYFLOWRATE,   &
     SmT1,SmT2,SmT3,SmT4,SmP1,SmP2,   &
      PGAMI,PSIGXE,   &
     KEYGWASTENULL,   &
     DTIME_FOR_WST,   &
     DELTA_TIME_FOR_WST,   &
     DT_CENTRAO, KEY_CENTR_AO,    &
     LENGTH_CENTRAO_STACK1, LENGTH_CENTRAO_STACK2,   &
     dcalcSPND,   &
     DT_CONTROL1,   &
	 Key_Mode_transfer, &    
     LimSHLAKI, &  
	 LimDeltaTime, & 
     KEYBACKSPACE,  & 
	 DWINTERPOLATE,  &  
	 KEYMICROFIELD, &    
     DAO_Prognos, &    
      lambda_x, &   
      lambda_i, &   
      sigma_x, &   
      Array_factor_of_flow_neutron, & 
      DT_Prognos_adjusting, &            
      Ras_3GCN,Ras_2GCN, &              
      NTlim_4GCN,NTlim_3GCN,NTlim_2GCN, &  
      BurBor1,BurBor2,BurBor3, &  
      IniLimQl, &  
	  micro_KSST,micro_KTWL,micro_NUPZ,micro_NGR, &    
      KEYMICROFIELD_F9 
!__________________________________________________________________________________________________________________________________________           
!начальные данные
i   = 0
i_  = 1      
key = 0   ! key=1 - индикатор наличия файла bippar в папке компании i
if (debug==1) then
    open(997,file='ProtokolBippar.dat',position="append")
end if
        
!____________________________________________________________________________________________Считываем адрес папки с фалом bippar.
!
 string = BipparAdressData
 k=len_dir+1 !k - число символов в строке "адрес + пробел"; k-1 - число символов в строке адреса                 
 string(k:k+7) = '\bippar'                   !_дописываем к строке адреса имя bippar, начиная с символа k (то есть пробел заменяется на \)
INQUIRE(FILE=string(1:k+7),exist=exists)  !проверка существования файла bippar в кампании Ki
!____________________________________________________________________________________________Ошибка. Не найден файл bippar.
if(exists==.false.) then
    if (debug==1) then
        write(997,'(1x,a57)')'File bippar do not find in current folder! Program exit.'
    end if
 call error_C('File bippar do not find in current folder! Program exit.')
 print('(1x,a57)'),'File bippar do not find in current folder! Program exit.'
 goto 999
endif
Help = string(1:k+7)
!____________________________________________________________________________________________Ошибки при считывании bippar
123 open(85,file=Help,err=996)
        read(85,HIPI,err=994)                       !считываем неймлисты из файла bippar  
        read(85,FGRAN,err=993)
    close(85)
    if (key.eq.1) then                        ! key=1 - индикатор наличия файла bippar в папке компании i
	 open (22,file=string1(1:k+11),err=995)   !открыли bippar в кампании номер i
	  string1(k+4:k+16)='\master .set'        !обозначили masteri.set
	  string1(k+11:k+11)=B(2:2)               !номера файла masteri.set
      read(22,HIPI,err=994)                   !считываем неймлисты из файла bippar    
      read(22,FGRAN,err=993)
     close(22)
	end if
!____________________________________________________________________________________________Ошибки при считывании неймлистов файла bippar
 !read(2,HIPI,err=994)                       !считываем неймлисты из файла bippar    
 !read(2,FGRAN,err=993)
 !я read(2,HIPI1,err=992)
 !я read(2,HIPI2,err=991)
!close(2)
!____________________________________________________________________________________________Отчет о считывании файла bippar.
if(i.eq.0)then
 if (debug==1) then   
    write(997,'(/1x,a33)')'File bippar is successfully read.'
 end if
 call PRINT_NULL_C()
 call PRINT_C('File bippar is successfully read.')
 print('(/1x,a33)'),'File bippar is successfully read.'
endif
!____________________________________________________________________________________________Отчет о считывании файла bippar в кампании i.
if(i.ne.0)then
if (debug==1) then
 write(997,'(1x,a43,i2)')'File bippar is successfully read in company',i
end if
 call PRINT_int_C('File bippar is successfully read in company',i)
 print('(1x,a43,i2)'),'File bippar is successfully read in company',i
endif

string(k:k+11)='\master.set'    !создаем файл master.set! 10 символов. Записываем начиная с k символа
if(key.eq.0)then
    FileNameMasterSet = string(1:k+11)
!____________________________________________________________________________________________Отчет о записи файла master.set.    
if (debug==1) then
    write(997,'(/1x,a37)')'File Master.set open for writing data.'
end if
    call PRINT_NULL_C()
    call PRINT_C('File Master.set open for writing data.')
    print('(/1x,a37)'),'File Master.set open for writing data.'
!----------------------------------------------------------------------------------------------------------------------------------------    
    call WriteMasterSet(FileNameMasterSet)   !открыли для записи файл master.set
!----------------------------------------------------------------------------------------------------------------------------------------    
if (debug==1) then
    write(997,'(1x,a37)')'File Master.set successfully written.'
end if
    call PRINT_C('File Master.set successfully written.')
    print('(1x,a37)'),'File Master.set successfully written.'
endif    
if(key.eq.1)then
    FileNameMasterSet = string1(1:k+16)
!____________________________________________________________________________________________Отчет о записи файла masteri.set.
if (debug==1) then
    write(997,'(/1x,a50,1x,i2)')'File Masteri.set open for writing data in campaign',i
end if
    call PRINT_int_C('File Masteri.set open for writing data in campaign',i)
    print('(/1x,a50,1x,i2)'),'File Masteri.set open for writing data in campaign',i
!----------------------------------------------------------------------------------------------------------------------------------------    
    call WriteMasterSet(FileNameMasterSet)   !открыли для записи файл masteri.set
!----------------------------------------------------------------------------------------------------------------------------------------  
if (debug==1) then
    write(997,'(1x,a49,i2)')'File Masteri.set successfully written in campaign',i
end if
    call PRINT_int_C('File Masteri.set successfully written in campaign',i)
    print('(1x,a49,i2)'),'File Masteri.set successfully written in campaign',i
endif
!*****открываем файлы burboc и создаем user.set для каждой кампании*****!
write(B,'(a2)') '\K'
string (k:k+1)=B                                !создаем адресс папки с кампанией i (например, \K01)
!----------------------------------------------------------------------------------------------------------------------------------------    
do i=i_,50 !NumberOfCampaign                          !цикл перебора кампаний
!----------------------------------------------------------------------------------------------------------------------------------------    
 write(B,'(i2)') i
 string (k+2:k+3)=B                              !вписываем в адресс папки номер кампании
 if(i.lt.10) string(k+2:k+2)='0'
    string (k+4:k+11)='\burboc'	                !создаем ссылку на файл burboc в папке кампании
	string1=string
	string1 (k+4:k+11)='\bippar'                !ищем bippar в отдельной кампании
	INQUIRE(FILE=string1(1:k+11),exist=exists)  !проверка существования файла bippar в кампании Ki
	if(exists==.true..and.key.eq.0) then        !создаем masteri.set в кампании, в которой есть bippar
        if (debug==1) then
		    write(997,'(/1x,a34,1x,i2)')'File bippar exists in the campaign',i
        end if
        call PRINT_int_C('File bippar exists in the campaign',i)
        print('(/1x,a34,1i2)'),'File bippar exists in the campaign',i
        key=1
        goto 123
	endif
 i_=i_+1
 key=0                                           !изменяем ключ записи файла master.set в кампании i на 0 для поиска в следующей кампании
 print*,Help(1:k+7)
    open(85,file=Help,err=996)
        read(85,HIPI,err=994)                       !считываем неймлисты из файла bippar  
        read(85,FGRAN,err=993)
        close(85)
 string (k+12:k+16)='    '
 INQUIRE(FILE=string(1:k+11),exist=exists)       !проверка существования файла burboc в кампании Ki
 
 if(exists==.false.)cycle                                             !если нет, то смотрим следующую кампанию
 open(42,file=string(1:k+11),err=990)
!____________________________________________________________________________________________Ошибки при считывании неймлистов файла burboc
TEF=0.;simshl=0.;NZSHL=0.;SHLAKI=0.;MAPN=0.;KAMP=0.;PM=0.;SM=0.
  read(42,BURNF)
 close(42)
!
 string(k+5:k+10)='userK'                        !создаем файлы userKxx.set и shlKxx в папке кампании, где лежит текущий открытый burboc!
 string(k+10:k+11)=B
 if(i.lt.10) string(k+10:k+10)='0'
 string(k+12:k+16)='.set'
 FileNameUserSet = string(1:k+16)       ! адрес файла user.set
 string(k+5:k+16)='           '         ! создаем файл шлаков в той же папке, что и user.set
 string(k+5:k+9)='shlK'
 string(k+9:k+10)=B
 if(i.lt.10) string(k+9:k+9)='0'
 FileNameShlaki  = string(1:k+11)       ! адрес файла шлаков (записывается в той же папке, что и user.set)
 !____________________________________________________________________________________________Отчет о записи файла user.set.  
 if (debug==1) then
    write(997,'(/1x,a49,1x,i2)')'File UserKi.set open for writing data in campaign',i
 end if
 call PRINT_NULL_C()
 print('(/1x,a49,1x,i2)'),'File UserKi.set open for writing data in campaign',i
 call PRINT_int_C('File UserKi.set open for writing data in campaign',i)
 !----------------------------------------------------------------------------------------------------------------------------------------
 ErrorSOTV=0
 call WriteUserSet(FileNameUserSet,i,ErrorSOTV,debug)     !открыли для записи файл userKi.set
 if(ErrorSOTV.eq.1)then
  if (debug==1) then
    write(997,'(1x,a39,i2)')'File UserKi.set NOT written in campaign',i
  end if
  call PRINT_int_C('File UserKi.set NOT written in campaign',i)
  print('(1x,a39,i2)'),'File UserKi.set NOT written in campaign',i
  ErrorSOTV = 0
  goto 988
 endif   
 !----------------------------------------------------------------------------------------------------------------------------------------
 if (debug==1) then
    write(997,'(1x,a48,i2)')'File UserKi.set successfully written in campaign',i
 end if
 call PRINT_int_C('File UserKi.set successfully written in campaign',i)
 print('(1x,a48,i2)'),'File UserKi.set successfully written in campaign',i
  !____________________________________________________________________________________________Отчет о записи файла shlKinew 
 if (debug==1) then
     write(997,'(1x,a46,1x,i2)')'File shlKinew open for writing data in campaign',i
 end if
 call PRINT_int_C('File shlKinew open for writing data in campaign',i)
 print('(1x,a46,1x,i2)'),'File shlKinew open for writing data in campaign',i
 !----------------------------------------------------------------------------------------------------------------------------------------
 call WriteShlaki(FileNameShlaki,i)                                    ! записываем файл шлаков в ту же директорию, что и user.set
  !----------------------------------------------------------------------------------------------------------------------------------------
 if (debug==1) then
    write(997,'(1x,a46,i2)')'File shlKinew successfully written in campaign',i
 end if
 call PRINT_int_C('File shlKinew successfully written in campaign',i)
 print('(1x,a46,i2)'),'File shlKinew successfully written in campaign',i
988 continue 
enddo			!!!конец перебора кампаний!!!

!__________________________________________________________________________________________________________________________________________
!_________________________________Блок выдачи информации об ошибках при считывании данных из неймлистов____________________________________
!__________________________________________________________________________________________________________________________________________
!  write(997,'(/1x,a33)')'Number of compaigns is incorrect!'
!  print('(/1x,a33)'),'Number of compaigns is incorrect!'
  goto 999
!1
996 if (debug==1) then
        write(997,'(/1x,a26)')'ERROR opening file bippar!'                                         !ошибка при открытии файла bippar
    end if
    call error_C('ERROR opening file bippar!')
    print*,'ERROR opening file bippar!'
    goto 999
!2    
995 if (debug==1) then
        write(997,'(/1x,a40)')'ERROR opening file bippar of company i!'                            !ошибка при открытии файла bippar компании i
    end if
    call error_C('ERROR opening file bippar of company i!')
    print*,'ERROR opening file bippar of company i!!'
    goto 999
!3    
994 if (debug==1) then
        write(997,'(/1x,a28)')'ERROR reading HIPI namelist!'                                       !ошибка при считывании данных неймлиста HIPPI
    end if
    call error_C('ERROR reading HIPI namelist!'  )
    print*,'ERROR reading HIPI namelist!'
    goto 999
!4
993 if (debug==1) then
        write(997,'(/1x,a29)')'ERROR reading FGRAN namelist!'                                      !ошибка при считывании данных неймлиста FGRAN
    end if
    call error_C('ERROR reading FGRAN namelist!' )
    print*,'ERROR reading FGRAN namelist!'
    goto 999
!5
992 if (debug==1) then
        write(997,'(/1x,a29)')'ERROR reading HIPI1 namelist!'                                      !ошибка при считывании данных неймлиста HIPPI1
    end if
    call error_C('ERROR reading HIPI1 namelist!')
    print*,'ERROR reading HIPI1 namelist!'
    goto 999
!6
991 if (debug==1) then
        write(997,'(/1x,a29)')'ERROR reading HIPI2 namelist!'                                      !ошибка при считывании данных неймлиста HIPPI2
    end if
    call error_C('ERROR reading HIPI2 namelist!')
    print*,'ERROR reading HIPI2 namelist!'
    goto 999    
!7
990 if (debug==1) then
         write(997,'(/1x,a29)')'ERROR reading BURNF namelist!'                                      !ошибка при считывании данных неймлиста BURNF
    end if
    call error_C('ERROR reading BURNF namelist!')
    print*,'ERROR reading BURNF namelist!'
    goto 999       
!__________________________________________________________________________________________________________________________________________  
999 continue
    if (debug==1) then
        close(997)
    end if
deallocate(GlobalFuelSortsList,FuelSortsList_IR,FuelLibrary)
 return
end 








