INTEGER FUNCTION TWIC (adress,len_of_adress)
!DEC$ ATTRIBUTES DLLEXPORT ::  TWIC
    CHARACTER(128) :: adress,Support
    CHARACTER(256) :: Put
    INTEGER :: CSTAT, ESTAT, lenght, i, system,len_of_adress
    Support = ''
    do i=1,len_of_adress
        Support(i:i) = adress(i:i)
    end do
    Put = 'start '// Support
    print*, 'fortran'
    print*, Put
    call  execute_command_line(Put)
END FUNCTION TWIC
