$lookup "AGCT" =
#i 0 =
#length 7 =
#ma 1 #length 1 << << =
#length 2 << _println
#t #length 1 - 1 << =
_while #i #ma < _do
    #offset 0 =
    #mask 3 =
    _while #offset #t <= _do
        $lookup #i #mask & #offset >> [] _print
        #offset 2 +=
        #mask #mask 2 << =
    _end
    "" _println

    #offset 0 =
    #mask 3 =
    _while #offset #t <= _do
        $lookup #i ~ #mask & #offset >> [] _print
        #offset 2 +=
        #mask #mask 2 << =
    _end
    "" _println
    #i #i 1 + =
    "" _println
_end
