subroutine WriteMasterSet(FileNameMasterSet)
!задаем библиотеки данных
use user_   !user.set
use master_ !master.set
use bippar_ !bippar file
use burboc_ !burboc file
implicit none
!описываем переменные
character*300 FileNameMasterSet
integer i,ii,j,jj
!________________________________________________________________________________________________________________
!________________________Присваиваем переменным начальные значения_______________________________________________
!________________________________________________________________________________________________________________
dAOmg = 0.1
!________________________________________________________________________________________________________________
open(3,file=FileNameMasterSet)
write(3,'(a7/)') '&MASTER'
!---Sm
!write(3,'(a5,f5.1,a1\)')   'PSm=',PSm,','
!write(3,'(1x,a5,f5.1,a1\)')'SmT1=',SmT1,','
!write(3,'(1x,a5,f5.1,a1\)')'SmT2=',SmT2,','
!write(3,'(1x,a5,f5.1,a1\)')'SmT3=',SmT3,','
!write(3,'(1x,a5,f5.1,a1\)')'SmT4=',SmT4,','
!write(3,'(1x,a5,f5.1,a1\)')'SmP1=',SmP1,','
!write(3,'(1x,a5,f5.1,a1\)')'SmP2=',SmP2,','
!---BurBor
!write(3,'(1x,a9,f3.1,a1\)')'BurBor1=',BurBor1,','
!write(3,'(1x,a9,f6.4,a1\)')'BurBor2=',BurBor2,','
!write(3,'(1x,a9,f5.1,a1/)')'BurBor3=',BurBor3,','
!----
write(3,'(1x,a7,f3.1,a1)') 'dhstep=',0.5,','
KEYGWASTENULL=1
write(3,'(1x,a14,i1,a1)') 'KEYGWASTENULL=',KEYGWASTENULL,','
errCH3BO3=0
write(3,'(1x,a10,i1,a1)') 'errCH3BO3=',errCH3BO3,','
DTin_dNt=(Tnom-TS)/100
write(3,'(1x,a9,f5.3,a1)') 'DTin_dNt=',DTin_dNt,','
Concentrate=40
write(3,'(1x,a12,f4.1,a1/)') 'Concentrate=',Concentrate,','

KEY_NGU=1
write(3,'(1x,a8,i1,a1)') 'KEY_NGU=',KEY_NGU,','
KEY_ANdpz=1
write(3,'(1x,a10,i1,a1)') 'KEY_ANdpz=',KEY_ANdpz,','
write(3,'(1x,a9,i1,a1)') 'KEYKVLIM=',KEYKVLIM,','
write(3,'(1x,a13,f5.1,a1)') 'H_UP_OVERLAP=',H_UP_OVERLAP,','
write(3,'(1x,a13,f4.1,a1)') 'H_DW_OVERLAP=',H_DW_OVERLAP,','
write(3,'(1x,a12,i1,a1)') 'KEY_OVERLAP=',KEY_OVERLAP,','
write(3,'(1x,a23,f3.1,a1)') 'ADD_CH3BO3_UNDER_START=',ADD_CH3BO3_UNDER_START,','
write(3,'(1x,a16,f4.1,a1)') 'HGR_UNDER_START=',HGR_UNDER_START,','
write(3,'(1x,a18,i1,a1)') 'Key_Mode_Transfer=',Key_Mode_Transfer,','
KEY_DEFINE_SIGN_AO=1;KEY_DEFINE_LIM_KQ=1;KEY_DEFINE_LIM_KV=1
write(3,'(1x,a19,i1,a1)') 'KEY_DEFINE_SIGN_AO=',KEY_DEFINE_SIGN_AO,','
write(3,'(1x,a18,i1,a1)') 'KEY_DEFINE_LIM_KQ=',KEY_DEFINE_LIM_KQ,','
write(3,'(1x,a18,i1,a1)') 'KEY_DEFINE_LIM_KV=',KEY_DEFINE_LIM_KV,','
write(3,'(1x,a18,i1,a1/)') 'Key_AO_Nastr_CBPK=',Key_AO_Nastr_CBPK,','

write(3,'(1x,a5,i2,a1)') 'KDPZ=',54,','
write(3,'(1x,a6,i2,a1)') 'KSDPZ=',KSDPZ,','
!---HDPZ
write(3,'(1x,a6,f6.2,a1\)')'HDPZZ=',HDPZZ(1),','
do i = 2,6
 write(3,'(f6.2,a1\)')HDPZZ(i),','
enddo
write(3,'(f6.2,a1)') HDPZZ(7),','
!---NomDpz
write(3,'(1x,a7)')'NomDpz='
write(3,'(1x,i3,a1\)')NomDpz(1),','
do i=2,53
!---1 stroka
 if(i.le.9) write(3,'(i3,a1\)')NomDpz(i),','
 if(i.eq.10)write(3,'(i3,a1)')NomDpz(i),','
!---2 stroka 
 if(i.eq.11)write(3,'(1x,i3,a1\)')NomDpz(i),','
 if(i.ge.12.and.i.le.19)write(3,'(i3,a1\)')NomDpz(i),','
 if(i.eq.20)write(3,'(i3,a1)')NomDpz(i),','
!---3 stroka 
 if(i.eq.21)write(3,'(1x,i3,a1\)')NomDpz(i),','
 if(i.ge.22.and.i.le.29)write(3,'(i3,a1\)')NomDpz(i),','
 if(i.eq.30)write(3,'(i3,a1)')NomDpz(i),','
!---4 stroka 
 if(i.eq.31)write(3,'(1x,i3,a1\)')NomDpz(i),','
 if(i.ge.32.and.i.le.39)write(3,'(i3,a1\)')NomDpz(i),','
 if(i.eq.40)write(3,'(i3,a1)')NomDpz(i),','
!---5 stroka
 if(i.eq.41)write(3,'(1x,i3,a1\)')NomDpz(i),','
 if(i.ge.42.and.i.le.49)write(3,'(i3,a1\)')NomDpz(i),','
 if(i.eq.50)write(3,'(i3,a1)')NomDpz(i),','
!---6 stroka 
 if(i.eq.51)write(3,'(1x,i3,a1\)')NomDpz(i),','
 if(i.eq.52)write(3,'(i3,a1\)')NomDpz(i),','
 if(i.eq.53)write(3,'(i3,a1\)')NomDpz(i),','
enddo
write(3,'(i3,a1)')NomDpz(54),','
!---DHDPZ
!write(3,'(a6/,10(i3,1x))') 'DHDPZ=',(DHDPZ(i),i=1,64)
write(3,'(a13)')'DHDPZ= 54*0.0'

!---
write(3,'(/1x,a3,i2,a1)') 'NZ=',NZ,','
DH=50
write(3,'(1x,a3,f4.1,a1)') 'DH=',DH,','
write(3,'(1x,a8,f4.1,a1)') 'DHORSUZ=',DHORSUZ,','
!---NRST
!write(3,'(a5/,12(i3,1x))') 'NRST=',(NRST(:,j),j=1,14)
write(3,'(a5)') 'NRST='
ii = 0
jj = 0
 do j=1,20
 do i=1,14
  ii = ii + 1
  if(jj.eq.12)goto 1
   if(ii.eq.12)then
                    write(3,'(i4,a1)')NRST(i,j),','
                    ii = 0
                    jj = jj + 1
                else
                    write(3,'(i4,a1\)')NRST(i,j),','
   endif
enddo
enddo
!---
1write(3,'(/1x,a6,f3.1,a1)') 'dAOmg=',dAOmg,','
write(3,'(1x,a2,f5.1,a1)') 'H=',H,','
HST=H;TST=H
!write(3,'(4x,a4,20(f5.1,1x))') 'HST=',HST
!---HST
write(3,'(1x,a8,f5.1)')'HST= 12*',H

!write(3,'(4x,a4,20(f5.1,1x))') 'TST=',TST
!---TST
	if (NRG.eq.10) write(3,'(1x,a11,a1)') 'HST(10)=-90.',','
	if (NRG.eq.12) write(3,'(1x,a11,a1)') 'HST(12)=-90.',','
	if (NRG.eq.14) write(3,'(1x,a11,a1)') 'HST(14)=-90.',','
write(3,'(1x,a7,f4.1,a1)') 'Pnastr=',Pnastr,','
write(3,'(1x,a2,f7.1,a1)') 'G=',G,','
write(3,'(1x,a7,f5.1,a1)') 'VKONT1=',VKONT1,','
TIN=TNOM
write(3,'(1x,a4,f5.1,a1)') 'TIN=',TIN,','
XKBT=WKBT;KBT=WKBT
write(3,'(1x,a5,f9.0,a1)') 'XKBT=',XKBT,','
write(3,'(1x,a4,f9.0,a1)') 'KBT=',KBT,','
write(3,'(1x,a4,f5.1,a1)') 'PAP=',PAP,','
write(3,'(1x,a6,f5.2,a1)') 'DHNGU=',DHNGU,','
write(3,'(1x,a6,f5.2,a1)') 'DHKH =',DHKH,','
write(3,'(1x,a6,f6.2,a1)') 'DHKB =',DHKB,','

write(3,'(/1x,a6,f5.2,a1)') 'POCB1=',POCB1,','
TDGOR=DGOR;TDGOZB=DGOZB;TDGOZH=DGOZH
!---TDGOR
write(3,'(1x,a7\)')'tDGOR = '
do i=1,5
 if(i.le.4)write(3,'(f5.1,a1\)'),TDGOR(i),','
 if(i.eq.5)write(3,'(f5.1,a1)'),TDGOR(i),','
enddo
!----TDGOZB
write(3,'(1x,a8\)')'tDGOZB = '
do i=1,5
 if(i.le.4)write(3,'(f5.1,a1\)'),TDGOZB(i),','
 if(i.eq.5)write(3,'(f5.1,a1)'),TDGOZB(i),','
enddo
!---TDGOZH
write(3,'(1x,a8\)')'tDGOZH = '
do i=1,5
 if(i.le.4)write(3,'(f5.1,a1\)'),TDGOZH(i),','
 if(i.eq.5)write(3,'(f5.1,a1)'),TDGOZH(i),','
enddo
!---
TDR=DR;TDR2=DR2;TDTOR=DTOR;TDTOZB=DTOZB;TDTOZH=DTOZH
!---TDR
write(3,'(1x,a6\)')'tDR = '
do i=1,5
 if(i.le.4)write(3,'(f5.1,a1\)'),TDR(i),','
 if(i.eq.5)write(3,'(f5.1,a1)'),TDR(i),','
enddo
!---TDR2
write(3,'(1x,a7\)')'tDR2 = '
do i=1,5
 if(i.le.4)write(3,'(f5.1,a1\)'),TDR2(i),','
 if(i.eq.5)write(3,'(f5.1,a1)'),TDR2(i),','
enddo
!---TDTOR
write(3,'(1x,a8\)')'tDTOR = '
do i=1,5
 if(i.le.4)write(3,'(f10.5,a1\)'),TDTOR(i),','
 if(i.eq.5)write(3,'(f10.5,a1)'),TDTOR(i),','
enddo
!---TDTOZB
write(3,'(1x,a8\)')'tDTOZB = '
do i=1,5
 if(i.le.4)write(3,'(f10.5,a1\)'),TDTOZB(i),','
 if(i.eq.5)write(3,'(f10.5,a1)'),TDTOZB(i),','
enddo
!---TDTOZH
write(3,'(1x,a8\)')'tDTOZH = '
do i=1,5
 if(i.le.4)write(3,'(f10.5,a1\)'),TDTOZH(i),','
 if(i.eq.5)write(3,'(f10.5,a1)'),TDTOZH(i),','
enddo
!---
TDZ=DZ;TDZLOR=DZLOR;TDZUPR=DZUPR
!---TDZ
write(3,'(1x,a6\)')'tDZ = '
do i=1,5
 if(i.le.4)write(3,'(f5.1,a1\)'),TDZ(i),','
 if(i.eq.5)write(3,'(f5.1,a1)'),TDZ(i),','
enddo
!---TDZLOR
write(3,'(1x,a9\)')'tDZLOR = '
do i=1,5
 if(i.le.4)write(3,'(f5.1,a1\)'),TDZLOR(i),','
 if(i.eq.5)write(3,'(f5.1,a1)'),TDZLOR(i),','
enddo
!---TDZUPR
write(3,'(1x,a8\)')'tDZUPR = '
do i=1,5
 if(i.le.4)write(3,'(f5.1,a1\)'),TDZUPR(i),','
 if(i.eq.5)write(3,'(f5.1,a1)'),TDZUPR(i),','
enddo
TGAMMA=GAMMA;TP1=P1;TP1DZ=P1DZ;TP2=P2;TP2DZ=P2DZ;
!---TGAMMA
write(3,'(1x,a8\)')'tGAMMA = '
do i=1,5
 if(i.le.4)write(3,'(f5.1,a1\)'),TGAMMA(i),','
 if(i.eq.5)write(3,'(f6.1,a1)'),TGAMMA(i),','
enddo
!---TP1
write(3,'(1x,a6\)')'tP1 = '
do i=1,5
 if(i.le.4)write(3,'(f6.2,a1\)'),TP1(i),','
 if(i.eq.5)write(3,'(f6.2,a1)'),TP1(i),','
enddo
!---TP1DZ
write(3,'(1x,a8\)')'tP1DZ = '
do i=1,5
 if(i.le.4)write(3,'(f6.2,a1\)'),TP1DZ(i),','
 if(i.eq.5)write(3,'(f6.2,a1)'),TP1DZ(i),','
enddo
!---TP2
write(3,'(1x,a6\)')'tP2 = '
do i=1,5
 if(i.le.4)write(3,'(f6.2,a1\)'),TP2(i),','
 if(i.eq.5)write(3,'(f6.2,a1)'),TP2(i),','
enddo
!---TP2DZ
write(3,'(1x,a8\)')'tP2DZ = '
do i=1,5
 if(i.le.4)write(3,'(f6.2,a1\)'),TP2DZ(i),','
 if(i.eq.5)write(3,'(f6.2,a1)'),TP2DZ(i),','
enddo
!---PDR
!write(3,'(8x,a5,2x10(f5.3,1x))') 'PDR =',(PDR(i),i=1,10)
write(3,'(1x,a6\)')'PDR = '
do i=1,10
 if(i.le.9)write(3,'(f5.3,a1\)'),PDR(i),','
 if(i.eq.10)write(3,'(f5.3,a1)'),PDR(i),','
enddo
!write(3,'(8x,a5,2x10(f5.3,1x))') 'PDR2=',(PDR2(i),i=1,10)
!---PDR2
write(3,'(1x,a7\)')'PDR2 = '
do i=1,10
 if(i.le.9)write(3,'(f5.3,a1\)'),PDR2(i),','
 if(i.eq.10)write(3,'(f5.3,a1)'),PDR2(i),','
enddo
!write(3,'(8x,a6,5(f5.1,1x))') 'TRHRO=',TRHRO
!---TRHRO
write(3,'(1x,a8\)')'TRHRO = '
do i=1,5
 if(i.le.4)write(3,'(f5.1,a1\)'),TRHRO(i),','
 if(i.eq.5)write(3,'(f5.1,a1)'),TRHRO(i),','
enddo
VAR=(/5,8,0,50,100,150,200,250,300,350,575,720,924,1100,1270,1410,1540,1670,0,0,0/)
!write(3,'(8x,a4,4(f4.0,1x),9(f5.0,1x),5(f6.0,1x),3(f4.0,1x))') 'VAR=',VAR
!----VAR
write(3,'(1x,a6\)')'VAR = '
do i=1,21
 if(i.le.4)write(3,'(f4.0,a1\)'),VAR(i),','
 if(i.ge.5.and.i.le.13)write(3,'(f5.0,a1\)'),VAR(i),','
 if(i.ge.14.and.i.le.18)write(3,'(f6.0,a1\)'),VAR(i),','
 if(i.ge.19.and.i.le.20)write(3,'(f4.0,a1\)'),VAR(i),','
 if(i.eq.21)write(3,'(f4.0,a1)'),VAR(i),','
enddo
!----
write(3,'(a4)') '&END'
close(3)
!
 return
end