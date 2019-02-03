!__________________________________________________________________________________________________
module ShlakiDataModule
!__________________________________________________________________________________________________
 !                                                  ! параметры
 integer,parameter :: nTVS = 163                    ! число ТВС
 real, parameter :: Height_ActiveZone = 374.0       ! высота АкЗ
 !
 character*128 ShlakiAdressNew,OldShlakiName,NewShlakiName,ShlakiAdress,directivetosave,printNew,ShlakiDirect,SmDirect,PmDirect,directive
 integer NZSHL,nLayersNew,nLayersOld,Lenght_OldShlakiName,lengthNew,leghtofdirect,ExIndex2
 real TEF,SIMSHL
 real,allocatable :: Shlaki(:,:),Sm(:,:),Pm(:,:),LayerCoordinateOld(:),LayerCoordinateNew(:),ShlakiNew(:,:),SmNew(:,:),PmNew(:,:)
endmodule