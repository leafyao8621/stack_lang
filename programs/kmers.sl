#i 0 =
_while #i 256 < _do
    #j 0 =
    #m 3 =
    _while #j 5 < _do
        #i #m & #j >> _print
        #j #j 2 + =
        #m #m 2 << =
    _end
    #i #m & #j >> _println
    #i #i 1 + =
_end
