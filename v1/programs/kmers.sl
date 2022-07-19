$a "AGCT" =
#i 0 =
#k 7 =
#ma 1 #k 1 << << =
#t #k 1 - 1 << =
_while #i #ma < _do
    #j 0 =
    #m 3 =
    _while #j #t <= _do
        $a #i #m & #j >> [] _print
        #j #j 2 + =
        #m #m 2 << =
    _end
    "" _println

    #j 0 =
    #m 3 =
    _while #j #t <= _do
        $a #i ~ #m & #j >> [] _print
        #j #j 2 + =
        #m #m 2 << =
    _end
    "" _println
    #i #i 1 + =
    "" _println
_end
