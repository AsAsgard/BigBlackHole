!__________________________________________________________________________________________________
!
!__________________________________________________________________________________________________
subroutine Shlaki_Normalization
 use ShlakiDataModule
 
        INTERFACE
    SUBROUTINE error_C(string) BIND(C,NAME='error_C')
      use iso_c_binding, only : C_CHAR
      character ( kind = C_CHAR ) :: string ( * )
    END SUBROUTINE error_C
    END INTERFACE
           

    
 real SumShlakiOld(nTVS),SumShlakiNew(nTVS),SumSmOld(nTVS),SumPmOld(nTVS),SumSmNew(nTVS),SumPmNew(nTVS), &
        NormalizationKoeffShlaki(nTVS),NormalizationKoeffSm(nTVS),NormalizationKoeffPm(nTVS),DeltaShlaki(nTVS),FirstDelta,maxDelta
 integer iTVS,iLayer,iFirstDelta,maxDeltaTVS
!__________________________________________________________________________________________________
!
!                                                   ! находим сумму шлаков (старые) по реактору.
SumShlakiOld = 0.;SumSmOld = 0.;SumPmOld = 0.       ! зануляем изначальные массивы.
!
do iTVS = 1,nTVS
!                                                   ! находим сумму шлаков (старые) для каждой ТВС.
 do iLayer = 1,nLayersOld  
     SumShlakiOld(iTVS) = SumShlakiOld(iTVS) + Shlaki(iLayer,iTVS)
     SumSmOld(iTVS)     = SumSmOld(iTVS) + Sm(iLayer,iTVS)
     SumPmOld(iTVS)     = SumPmOld(iTVS) + Pm(iLayer,iTVS)
 enddo
 !                                                   ! усредняем сумму для каждой ТВС по числу слоев  (старые).
 SumShlakiOld(iTVS) = SumShlakiOld(iTVS)  / nLayersOld
 SumSmOld(iTVS)     = SumSmOld(iTVS) / nLayersOld
 SumPmOld(iTVS)     = SumPmOld(iTVS) / nLayersOld 
enddo
!
!
SumShlakiNew = 0.;SumSmNew = 0.;SumPmNew = 0.
!
do iTVS = 1,nTVS
!                                                   ! находим сумму шлаков (новые) для каждой ТВС.
 do iLayer = 1,nLayersNew  
     SumShlakiNew(iTVS) = SumShlakiNew(iTVS) + ShlakiNew(iLayer,iTVS)
     SumSmNew(iTVS)     = SumSmNew(iTVS) + SmNew(iLayer,iTVS)
     SumPmNew(iTVS)     = SumPmNew(iTVS) + PmNew(iLayer,iTVS)
 enddo
 !                                                   ! усредняем сумму для каждой ТВС по числу слоев  (новые).
 SumShlakiNew(iTVS) = SumShlakiNew(iTVS)  / nLayersNew
 SumSmNew(iTVS)     = SumSmNew(iTVS) / nLayersNew
 SumPmNew(iTVS)     = SumPmNew(iTVS) / nLayersNew 
enddo
!
!
do iTVS = 1,nTVS
!                                                   ! вводим нормировочный коэффициент для выполнения условия сохранения числа шлаков в АкЗ.     
 if(SumShlakiNew(iTVS).ne.0)NormalizationKoeffShlaki(iTVS) = SumShlakiOld(iTVS) / SumShlakiNew(iTVS)
 if(SumSmNew(iTVS).ne.0)    NormalizationKoeffSm(iTVS)     = SumSmOld(iTVS) / SumSmNew(iTVS)
 if(SumPmNew(iTVS).ne.0)    NormalizationKoeffPm(iTVS)     = SumPmOld(iTVS) / SumPmNew(iTVS)
enddo
!
!                                                   ! нормируем новые шлаки на полученный коэффициент.
do iTVS = 1,nTVS
 do iLayer = 1,nLayersNew
    if(SumShlakiNew(iTVS).ne.0)ShlakiNew(iLayer,iTVS) = NormalizationKoeffShlaki(iTVS) * ShlakiNew(iLayer,iTVS)
    if(SumSmNew(iTVS).ne.0)SmNew(iLayer,iTVS)        = NormalizationKoeffSm(iTVS) * SmNew(iLayer,iTVS)
    if(SumPmNew(iTVS).ne.0)PmNew(iLayer,iTVS)        = NormalizationKoeffPm(iTVS) * PmNew(iLayer,iTVS)
 enddo
enddo
!                                                   ! рассчитываем сумму еще раз для проверки.
!
SumShlakiNew = 0.;SumSmNew = 0.;SumPmNew = 0.
!
do iTVS = 1,nTVS
!                                                   ! находим сумму шлаков (новые) для каждой ТВС.
 do iLayer = 1,nLayersNew  
     SumShlakiNew(iTVS) = SumShlakiNew(iTVS) + ShlakiNew(iLayer,iTVS)
     SumSmNew(iTVS)     = SumSmNew(iTVS) + SmNew(iLayer,iTVS)
     SumPmNew(iTVS)     = SumPmNew(iTVS) + PmNew(iLayer,iTVS)
 enddo
 !                                                   ! усредняем сумму для каждой ТВС по числу слоев  (новые).
 SumShlakiNew(iTVS) = SumShlakiNew(iTVS)  / nLayersNew
 SumSmNew(iTVS)     = SumSmNew(iTVS) / nLayersNew
 SumPmNew(iTVS)     = SumPmNew(iTVS) / nLayersNew 
enddo
!                                                   ! находим различия в суммах новых и старых шлаков.
do iTVS = 1,nTVS
    if(SumShlakiNew(iTVS).ne.0) DeltaShlaki(iTVS) = (( SumShlakiNew(iTVS) - SumShlakiOld(iTVS) ) / SumShlakiNew(iTVS) ) * 100. !,%
    if(SumShlakiNew(iTVS)==0.and.SumShlakiNew(iTVS).ne.0)then
        call error_C('Ошибка при вычислении SumShlakiNew.')
        if (debug==1) then
             write(999,'(1x,a34)')'Error in calculation SumShlakiNew.'
        end if
    endif
    if(SumShlakiNew(iTVS)==0.and.SumShlakiNew(iTVS)==0)DeltaShlaki(iTVS) = 0.     
enddo
!                                                   ! находим наибольшее различие в суммах новых и старых шлаков.
FirstDelta = DeltaShlaki(1)
do iTVS = 1,nTVS
 if(abs(DeltaShlaki(iTVS)).gt.FirstDelta)then
     FirstDelta = DeltaShlaki(iTVS)
     iFirstDelta = iTVS
 endif
enddo
maxDelta = FirstDelta
maxDeltaTVS = iFirstDelta
if (debug==1) then
     write(999,'(1x,a35,1x,f5.3,1x,a2,1x,i3,1x,a4/)')'Maximal normalizatoin koefficient =',NormalizationKoeffShlaki(maxDeltaTVS),'in',maxDeltaTVS,'TVS.'
end if
 return
endsubroutine