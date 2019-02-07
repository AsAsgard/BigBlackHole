!__________________________________________________________________________________________________
!________________Программа перерасчета шлаков с увеличенным числом слоев___________________________
!__________________________________________________________________________________________________
subroutine LISTCONV(debug)
!DEC$ ATTRIBUTES DLLEXPORT ::  LISTCONV
!__________________________________________________________________________________________________
use ArrayOf10Numbers
use ShlakiDataModule
use iso_c_binding, only : C_CHAR, C_NULL_CHAR
!
IMPLICIT NONE
character*1 NumberMassive(10)
character*2 nLayersLetter
character*128 TVSnumberString,SinglLetterSHL
integer i,j,iOld,iTVS,iLayer,Lenght_ShlakiAdressNew1,TVSnumber,Lenght_TVSnumberString,NumberScore,YesNoLetter,nomerTVS,DownLayeKey,NZ,ii,jj,i1,i2,kj,ki,ost,G_index1,G_index2,debug
real Height_LayerOld,Height_LayerNew,kShlaki,kSm,kPm,nomerTVSreal, Value1, Value2,C,CC,D,S
Character B*80,  T*8, G*80
real,allocatable :: C1(:), H1(:),A2(:),H2(:),C2(:),HD2(:),N2(:),SHLData(:)

!__________________________________________________________________________________________________

  INTERFACE
    SUBROUTINE Read_Old(str,FileAdress) BIND(C, NAME='ReadOld_C')
      use iso_c_binding, only : C_INT,C_CHAR
      integer ( C_INT ) :: str
      character(kind=C_CHAR), dimension(128), intent(out) :: FileAdress 
    END SUBROUTINE Read_Old
  END INTERFACE   

            INTERFACE
    SUBROUTINE mistake_C() BIND(C,NAME='mistake_C')
      use iso_c_binding, only : C_CHAR
    END SUBROUTINE mistake_C
            END INTERFACE

if (debug==1) then
    open(999,file='ShlakiProtokol.dat', position="append") 
end if

nLayersOld = NZSHL
ShlakiAdressNew=''
call Read_Old(nLayersNew,ShlakiAdressNew)
print*,'fortran',nLayersNew
print*,'fortran',ShlakiAdressNew
If (NZSHL>nLayersNew) then
      Open (10,File=ShlakiAdress) 
      Open (11,File=ShlakiAdressNew) 
      allocate( C1(NZSHL), H1(NZSHL),A2(nLayersNew*163),H2(nLayersNew),C2(nLayersNew),HD2(nLayersNew*2),N2(nLayersNew),SHLData(NZSHL*163))
      Do i=1,2
          Read (10,100) B         !    &BURNF
          Write(11,100) B         !     TEF
100       Format(A80)
      End Do
          Read (10,100) B         !     SIMSHL
          Write(11,100) B
          Read (10,'(a)') G      !     NZSHL
          G_index1=index(G,'=')
          G_index2=index(G,',')
          if (G_index2==0) G_index2=30
          NZ=nLayersNew
          print*,G(1:G_index1)
          Write(11,108) G(1:G_index1),NZ,','
108       Format(A10,I2,A1)
105       Format(A8,I2)
          
          H1(1)=100./(NZSHL*2.)  !60*2=120
             Do i=2,NZSHL
             H1(i)=H1(i-1)+100./NZSHL
             End Do
                H2(1)=100./(NZ*2.)
                Do j=2,NZ
                H2(j)=H2(j-1)+100./NZ
                End Do
                     i=1
                     j=1
200       If(H2(j) >= H1(i)) i=i+1
          If(H2(j) >= H1(i)) i=i+1
          If(H2(j) >= H1(i)) i=i+1
          If(H2(j) >= H1(i)) i=i+1
          HD2(2*j-1)=H2(j)-H1(i-1) !dH перед выбр.сл.
          HD2(2*j)  =H1(i)-H2(j)   !dH после выбр.сл.
          
                       N2(j)=i                  !выбранные слои из 60
                       j=j+1
                       If (j <= NZ) Go To 200
                       
    Do ii=1,3      !!!!       sh,sm,pm     !!!!!!!!!!!!!!!!!!!!!!!!!
        Read (10,100) B   !  SHLAKI   SM    PM
        Write(11,100) B
        ki=0
        ost=0
        SHLData=0
        do kj=1,(163*NZSHL)/10
            Read (10,101) (SHLData((kj-1)*10+i1),i1=1,10) 
        end do
        if (((163*NZSHL)/10)*10 .ne. (163*NZSHL)) then
            Read (10,101) (SHLData(((163*NZSHL)/10)*10+i1),i1=1,163*NZSHL-((163*NZSHL)/10)*10)
        end if
        S=0
        do kj=1,163*NZSHL
            S=S+SHLData(kj)
        end do
              Do jj=1,163   !---------------------------------Исправить!
                Do ki=1,NZSHL
                    C1(ki)=SHLData(ost+ki)
                end do
                ost=ost+NZSHL
             !================================
                    C=0.0
                    Do i2=1,NZSHL
                    C=C1(i2)+C
                    End Do
                    C=C/NZSHL   ! вначале 2сл и в конце 1сл исключаем
                          CC=0.0
                          Do j=1,NZ
      
                            Value1 =  C1(N2(j)-1)*HD2(2*j)/(HD2(2*j)+HD2(2*j-1))
                            Value2 = C1(N2(j))*HD2(2*j-1)/(HD2(2*j)+HD2(2*j-1))
                            C2(j) = Value1  +  Value2
                          CC=C2(j)+CC
                          End Do
                          CC=CC/NZ
                             D=0.0
                             Do j=1,NZ
                             If (CC >= 0.0) C2(j)=C2(j)*C/CC
                             If (CC == 0.0) C2(j)=CC
                             A2(j+NZ*(jj-1))=C2(j)
                             D=D+C2(j)
                             End Do
                             D=D/NZ
              End Do        !---------------------------------
               Write(11,102) A2
               S=0
               do kj=1,nLayersNew*163
                   S=S+A2(kj)
               end do
102            Format((1x,10(F6.3,',')))
101            Format(10(1x,F6.3))


           End Do    
      
      Close (10)
      Close (11)
      return
end if

!
!__________________________________________________________________________________________________
iOld = 1
Height_LayerOld = Height_ActiveZone / nLayersOld    ! высота слоя по высоте (старая).
Height_LayerNew = Height_ActiveZone / nLayersNew    ! высота слоя по высоте (новая).
!
allocate(LayerCoordinateOld(nLayersOld),LayerCoordinateNew(nLayersNew))
LayerCoordinateOld = 0.;LayerCoordinateNew = 0.
!
do i=1,nLayersOld                                            ! заполняем массив старых координат слоев.
 if(i.eq.1)LayerCoordinateOld(i) = 0.5 * Height_LayerOld     ! начало оси координат внизу АкЗ.
 if(i.gt.1)LayerCoordinateOld(i) = LayerCoordinateOld(i - 1) + Height_LayerOld 
enddo
do j=1,nLayersNew                                            ! заполняем массив новых координат слоев.
 if(j.eq.1)LayerCoordinateNew(j) = 0.5 * Height_LayerNew     ! начало оси координат внизу АкЗ.
 if(j.gt.1)LayerCoordinateNew(j) = LayerCoordinateNew(j - 1) + Height_LayerNew 
enddo
!__________________________________________________________________________________________________
allocate(ShlakiNew(nLayersNew,nTVS),SmNew(nLayersNew,nTVS),PmNew(nLayersNew,nTVS))
ShlakiNew = 0.;SmNew = 0.;PmNew = 0.
                                                             ! интерполяция шлаков.
do iTVS=1,nTVS
iOld = 1
kShlaki = ( Shlaki(iOld+1,iTVS) - Shlaki(iOld,iTVS)) / ( LayerCoordinateOld(iOld+1) - LayerCoordinateOld(iOld) )    ! y = k*x + b
kSm     = (     Sm(iOld+1,iTVS) -     Sm(iOld,iTVS)) / ( LayerCoordinateOld(iOld+1) - LayerCoordinateOld(iOld) )
kPm     = (     Pm(iOld+1,iTVS) -     Pm(iOld,iTVS)) / ( LayerCoordinateOld(iOld+1) - LayerCoordinateOld(iOld) )
!
 do iLayer=1,nLayersNew
!     
  if(LayerCoordinateNew(iLayer).gt.LayerCoordinateOld(iOld))then
     iOld = iOld + 1
     if(iOld.ge.nLayersOld)iOld = nLayersOld
     kShlaki = ( Shlaki(iOld,iTVS) - Shlaki(iOld-1,iTVS)) / ( LayerCoordinateOld(iOld) - LayerCoordinateOld(iOld-1) )
     kSm     = ( Sm(iOld,iTVS) - Sm(iOld-1,iTVS)) / ( LayerCoordinateOld(iOld) - LayerCoordinateOld(iOld-1) )
     kPm     = ( Pm(iOld,iTVS) - Pm(iOld-1,iTVS)) / ( LayerCoordinateOld(iOld) - LayerCoordinateOld(iOld-1) )
  endif
 !-----------------
  ShlakiNew(iLayer,iTVS) = Shlaki(iOld,iTVS) - kShlaki * (LayerCoordinateOld(iOld) - LayerCoordinateNew(iLayer))
  SmNew(iLayer,iTVS)     = Sm(iOld,iTVS) - kSm * (LayerCoordinateOld(iOld) - LayerCoordinateNew(iLayer))
  PmNew(iLayer,iTVS)     = Pm(iOld,iTVS) - kPm * (LayerCoordinateOld(iOld) - LayerCoordinateNew(iLayer))
 !------------------
 continue
!
 enddo
enddo
!
if (debug==1) then
    write(999,'(1x,a35,1x,i3,1x,a4/)')'Data files successfully created for',TVSnumber,'TVS.'
end if

call Shlaki_Normalization
close (999)
 return
endsubroutine
