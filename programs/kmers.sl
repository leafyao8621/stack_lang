$a "AGCT" =
#i 0 =
#k 7 =
_while #i 1 #k 1 << << < _do
    #j 0 =
    #m 3 =
    _while #j #k 1 - 1 << <= _do
        $a #i #m & #j >> [] _print
        #j #j 2 + =
        #m #m 2 << =
    _end
    "" _println

    #j 0 =
    #m 3 =
    _while #j #k 1 - 1 <<  <= _do
        $a #i ~ #m & #j >> [] _print
        #j #j 2 + =
        #m #m 2 << =
    _end
    $a #i ~ #m & #j >> [] _println
    #i #i 1 + =
    "" _println
_end
