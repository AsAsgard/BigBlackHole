! Задание переменных для ИР !

 Module user_

 character*80 Name_shl_pool
 character(20) Name_shl
 character(40) Name_lib	
 character*17 SORTS(20)
 character(4) :: Name_NPP='KLN3      '		!по умолчанию

 integer  Nom_BLOCK,Nom_CYCLE
 integer T,NNRG,NNRG_,KTOP(20,3),TOPL(163),AK,RESULTNEW(15),M_Reload(241),IVERC
 integer , DIMENSION(18) :: Shifr_SVP=(/(0,ijkl=1,18)/)
 integer(4) ::	KSORT

 real :: DT_Prot=0.0, DT_ARC=360.0, DT_Print=0.0
 real :: Tstep_ext=2.0, Tfin_ext=30.0, Pstep_ext=5.0, Pfin_ext=80.0
 REAL, DIMENSION(100) :: StationaryOffset=(/(-3.0,ijkl=1,100)/)
 !real PC3(20,3),PK8(20,3),D1(20,3)
          
 end Module user_    

!-----------------------------------------------------------!
 Module master_

 integer UPR,NK,NR,NJ(23),NP,ML,NRL,NR3,HE,HW,KD,NS,NST,SDV,PSPUSK,PXE,KTN,KR,I10,NACHXE, &
         KEY_DEFINE_SIGN_AO,KEY_DEFINE_LIM_KV,KEY_DEFINE_LIM_KQ,KEY_ANdpz, KEY_NGU, KEYFLOWRATE, KEYGWASTENULL, &
		 KEYBACKSPACE,KEYMICROFIELD,KEYSTARTCONTROL1,KEY_PROGNOS_OFFSET,KEYMICROFIELD_F9,KEYKVLIM, &
		 Irgm(10),NZ
 Integer :: N_fuel_rod_in_TWS=312
 Integer :: Key_AO_Nastr_CBPK = 1 
 Integer :: KEY_OVERLAP=0
 INTEGER :: KTBC=163, &        ! Количество ТВС
            KDpz=64,  &        ! Количество ТВС с ДПЗ
            KsDpz=7 
 INTEGER, DIMENSION(241) ::  NomDpz=(/  &
      83, 81, 80, 54,131, 33,146, 77, 23, 88,   &
      150,  8, 98, 93, 43, 71,134, 65, 45,124,   &
      92, 32,163,  6,154, 50, 35,137,104, 12,   &
      162,  3, 96, 69, 84,108,135, 29, 87,141,   &
      18,156, 76, 14,122, 66, 57,111, 99,119,   &
      30,132, 40, 72,  1,158,114,129, 10,152,   &
      27, 60,159,  4, (0,ijkl=1,177)/)
 Integer	NRST(14,20)
 INTEGER :: KEY_UNDER_START=1
 Integer (2) Time_break_refresh
 Integer :: KEY_ADMISSION=0
 INTEGER :: KEY_CENTR_AO=1 ! ключ: 1 - автоматическая настройка, 0 - по массиву  StationaryOffset
 INTEGER :: LENGTH_CENTRAO_STACK1 = 280 ! длина стека  1го уровняF
 INTEGER :: LENGTH_CENTRAO_STACK2 = 10  ! длина стека  2го уровня
 Integer :: DT_CONTROL1=0
 INTEGER ::  Key_Mode_transfer=1  ! Ключ передачи данных СВРК: 0 - через ir.dat; 1 - общ память  27.02.2009
 INTEGER :: micro_KSST=8    !Кол. комбинаций погружения групп ОР СУЗ 
 INTEGER :: micro_KTWL=331  !Кол. ячеек в ТВС
 INTEGER :: micro_NUPZ=4    !Ном. группы УПЗ
 INTEGER :: micro_NGR=3     !Кол. управляющих групп, которые учитываются в расчетах микрополей 


 real DTeffday,TKK,TPCH,DTPCH,TPF,DTPF,H,HR,SIM,HTOPL(2,10),KBT,W,EPS(10),RODOP,WDOP,CBMAX,TDOP,KFF,DKB,CB,DCB,M2, &
      BO(10),AI(8,10),BI(6,10),C1(20),TST(20),RLI(20),PKR,DRDT,PDT,PDRDN,RPOL(50),H1,H2,NSPG, &
	  DLOGST(4),TXE,DTXE,GJOD,GXE,ALXE,TKXE,TTEK,XKBT,TSM,RXEL(8),HDSTN(20),HST1(20),SIGXE(20),GPM,GSM,ALPM,ALSM,TKONSM, &
      SIGSM(20),GN(350),AAA(10),TPK(350),TIN,PAP,D2SL(2,4), &
	  PPSI,PSS,PODR,PDLOG(4),DDR(151),DZUPPR,DZLOWR,DZLOWS,DZUPPS, &
      PKK(350)

REAL, DIMENSION(21) :: &
	 VAR=(/5,8,0,50,100,150,200,250,300,350,575,720,924,1100,1270,1410,1540,1670,0,0,0/)
 
 REAL, DIMENSION(9) :: &
     P1DZ_X=(/-00.00008046,-15.85410999,+20.84453877,(0.0,ijkl=1,6)/),  &
     P2DZ_X=(/-00.00002707,-04.17648552,+18.13823177,(0.0,ijkl=1,5),0.1/)
 REAL :: Tnastr=10.0
 REAL :: DHEFF=0.0, Pnastr=0.0, DPnastr0=0.1
 REAL :: TTN=5.
 REAL, DIMENSION(5) :: &     
!	 TRHRO=(/302.,280.0,200.0,120.0,27.0/), &
     TGAMMA=(/724., 766., 875., 951., 1003.5/),  &
     TDR=(/25.5,24.5,23.0,22.2,21.6/),       &
     TDZ=(/17.5, 16.5, 15.2, 14.4, 13.9/),    &
     TP1=(/-0.31,-0.31,-0.31,-0.31,-0.31/),     &
     TP2=(/0.00,0.00,0.00,0.00,0.00/),          &
     TDR2=(/11.2, 10.8, 9.7, 9.1, 8.6/),          &
     TDZUPR=(/-12.0, -14.0, -12.6, -11.6, -10.0/),  &
     TDZLOR=(/-13.0, -13.0, -12.0, -11.0, -9.2/),  &
     TP1DZ=(/0.95,0.95,0.95,0.95,0.95/),          &
     TP2DZ=(/1.05,1.05,1.05,1.05,1.05/),          &
     TDGOR=(/-9.1,-8.4,-6.5,-5.6,-5.0/),           &
     TDGOZH=(/-8.7,-7.9,-6.1,-5.2,-4.6/),            &
     TDGOZB=(/-9.5,-7.5,-5.9,-4.9,-4.4/),               &
     TDTOR=(/-0.0006,-0.0006,-0.0006,-0.00062,-0.0008/),  &
     TDTOZH=(/0.0001,0.0001,0.00016,0.00025,0.0004/),  &
     TDTOZB=(/0.0032,0.0032,0.0032,0.0032,0.0036/)
 REAL :: POCB1=1.0, POCB2=0.0
 REAL :: TUS1=-0.01653, TUS2=0.000308, TUS3=-1.3E-07
 REAL :: H_UP_OVERLAP=100.0, H_DW_OVERLAP=50.0
 REAL, DIMENSION(30) :: HDpzz=(/41.03,085.78,128.53,172.28,216.03,259.78,303.53, (0.0,ijkl=1,23)/)
 REAL, DIMENSION(241) :: DHDpz=(/(0.0,ijkl=1,241)/)
 Real LimKv_CBPK(30,241),LimDPZnom(30)           
 Real, dimension (20)  :: WEIGHT_G=(/1.,1.,1.,1.,1.,1.,1.,1.5,1.6666,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1./)
 REAL :: VKONT1=280.0   
 REAL dTin_dNt,DH,HST(20),dAOmg
 REAL :: DHORSUZ=6.0   
 REAL :: DEPTH_UNDER_START=0.   
 REAL :: ADD_CH3BO3_UNDER_START=0.3
 REAL :: CH3BO3_UNDER_START=99.9999
 REAL :: HGR_UNDER_START=60.
 REAL :: TIN_UNDER_START=280.
 Real :: LIM_CORRECT_CBPK=10.0
 Real :: DHNGU=7.25, DHKH=17.25, DHKB=369.25  ! Положения выключателей для групп ОР   27.02.2009
 Real :: Screen_Gate_N =2.0, Screen_Gate_T=1.0, Screen_Gate_P=1.0, Screen_Gate_G=2.0, Screen_Gate_H=2.0
 REAL :: DHSTEP=2.0    
 Real :: FTC=0.97
 Real :: maxKq=1.35, maxKv=1.9, minKv=1.35, Hcross=45.0
 Real ErrCH3BO3,Concentrate,Distillate,ErrorNT,ErrorTINT, ErrorOP, ErrorG
 real :: PSm  = 1.0
 real :: SmT1 = 0.0   ! конец 1го интервала (снижение доли Sm: SmP1>FTR> 1)
 real :: SmT2 = 60.0  ! конец 2го интервала (снижение доли Sm: FTR <1)
 real :: SmT3 = 180.0 ! конец 3го интервала (постоянная  доля Sm: FTR =SmP2)
 real :: SmT4 = 250.0 ! конец 4го интервала (увеличение доли Sm: FTR <=1, начало интервала FTR  =1)
 real :: SmP1 = 1.0   ! максимальная доля самария > 1
 real :: SmP2 = 1.0   ! минимальная доля самария < 1  
 real :: PGAMI   = 1.0   ! доля выхода Йода
 real :: PSIGXE=1.0      ! доля сечения поглощения Ксенона
 real :: DTIME_FOR_WST   = 5.0   ! шаг заполнения стека, мин	!Параметры управления стеком времени и концентрации БК для расчета ж.о. (subr. RG_Control)
 real :: DELTA_TIME_FOR_WST   = 30.0 ! база для дифференциала конц.БК, мин
 REAL :: DT_CENTRAO = 1    ! шаг времени для изменения стеков (час)    
 Real(4) :: dcalcSPND
 Real(4), dimension(60) :: LimSHLAKI
 real LimDeltaTime,DAO_Prognos,lambda_x,lambda_i,sigma_x,Array_factor_of_flow_neutron(2,20),DT_Prognos_adjusting,Ras_3GCN,Ras_2GCN, &
      NTlim_4GCN,NTlim_3GCN,NTlim_2GCN
 real :: BurBor1=0.0 
 real :: BurBor2=0.0005
 real :: BurBor3=120.0
 REAL :: DWINTERPOLATE=5.0  !    шаг по мощности для повторной интерполяции констант в BIPR_WORK  22.07.2009
 Real IniLimQl(3,3)

 end Module master_

  
 
      


      





