!__________________________________________________________________________________________________
!____________Программа записи файла с величиенным числом слоев шлаков______________________________
!__________________________________________________________________________________________________
subroutine DATAREWRITE(debug)
!DEC$ ATTRIBUTES DLLEXPORT ::  DATAREWRITE
!__________________________________________________________________________________________________
 use ShlakiDataModule
integer iLayer,iTVS,iString,lenghtofdir,lengthofname,lengthOld,debug
character*128 printOld

            INTERFACE
    SUBROUTINE Run_C() BIND(C,NAME='Run_C')
      use iso_c_binding
    END SUBROUTINE Run_C
  END INTERFACE
!__________________________________________________________________________________________________
If (nLayersNew<NZSHL) then
    deallocate(Shlaki,Sm,Pm)
    call Run_C()
    return
end if
open(7,file=ShlakiAdressNew)
 write(7,'(a6)')'&BURNF'
 write(7,'(1x,a4,f6.2,a1)')'TEF=',TEF,','
 write(7,'(1x,a7,f2.0,a1)')'SIMSHL=',SIMSHL,','
 write(7,'(1x,a6,i2,a1)')'NZSHL=',nLayersNew,','
!__________________________________________________________________________________________________
 write(7,'(2x,a7)')'SHLAKI='
 !
 iString = 0
 do iTVS=1,nTVS
  do iLayer=1,nLayersNew
    !
     iString = iString + 1
     if(iTVS==nTVS.and.iLayer==nLayersNew)then
         if(iString==1)write(7,'(1x,f6.3,a1)')ShlakiNew(iLayer,iTVS),','
         if(iString.ne.1)write(7,'(f6.3,a1)')ShlakiNew(iLayer,iTVS),','
         exit
     endif
     if(iString==1)write(7,'(1x,f6.3,a1\)')ShlakiNew(iLayer,iTVS),','
     if(iString.gt.1.and.iString.lt.10)write(7,'(f6.3,a1\)')ShlakiNew(iLayer,iTVS),','
     if(iString==10)then
         write(7,'(f6.3,a1)')ShlakiNew(iLayer,iTVS),','
         iString = 0
     endif    

    !
  enddo
 enddo
!__________________________________________________________________________________________________
  write(7,'(2x,a3)')'SM='
 !
 iString = 0
 
 do iTVS=1,nTVS
  do iLayer=1,nLayersNew
    !
     iString = iString + 1
    if(iTVS==nTVS.and.iLayer==nLayersNew)then
         if(iString==1)write(7,'(f6.3,a1)')SmNew(iLayer,iTVS),','
         if(iString.ne.1)write(7,'(f6.3,a1)')SmNew(iLayer,iTVS),','
         exit
     endif
     if(iString==1)write(7,'(1x,f6.3,a1\)')SmNew(iLayer,iTVS),','
     if(iString.gt.1.and.iString.lt.10)write(7,'(f6.3,a1\)')SmNew(iLayer,iTVS),','
     if(iString==10)then
         write(7,'(f6.3,a1)')SmNew(iLayer,iTVS),','
         iString = 0
     endif
    !
  enddo
 enddo
!__________________________________________________________________________________________________
   write(7,'(2x,a3)')'PM='
 !
 iString = 0
 do iTVS=1,nTVS
  do iLayer=1,nLayersNew
    !
     iString = iString + 1
    if(iTVS==nTVS.and.iLayer==nLayersNew)then
         if(iString==1)write(7,'(f6.3,a1)')PmNew(iLayer,iTVS),','
         if(iString.ne.1)write(7,'(f6.3,a1)')PmNew(iLayer,iTVS),','
         exit
     endif
     if(iString==1)write(7,'(1x,f6.3,a1\)')PmNew(iLayer,iTVS),','
     if(iString.gt.1.and.iString.lt.10)write(7,'(f6.3,a1\)')PmNew(iLayer,iTVS),','
     if(iString==10)then
         write(7,'(f6.3,a1)')PmNew(iLayer,iTVS),','
         iString = 0
     endif
    !
  enddo
 enddo
!__________________________________________________________________________________________________ 
 write(7,'(a4)')'&END'
close(7)
!
!
deallocate(Shlaki,Sm,Pm,LayerCoordinateOld,LayerCoordinateNew,ShlakiNew,SmNew,PmNew)
call Run_C() 
 return
endsubroutine