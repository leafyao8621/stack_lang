_def ?print_fp #x #decimal_places _begin
    #divisor 1 =
    #i 0 =
    _while #i #decimal_places < _do
        #divisor #divisor 10 * =
        #i ++
    _end
    #x #divisor / _print
    "." _print
    #remainder #x #divisor % =
    #temp #remainder =
    #temp _if 
        #cnt 0 =
        _while #temp 0 > _do
            #temp #temp 10 / =
            #cnt ++
        _end
    _else
        #cnt 1 =
    _end
    _while #cnt #decimal_places < _do 
        0 _print
        #cnt ++
    _end
    #remainder _println
_end

_def ?multiply_fp #x #y #decimal_places _begin 
    #divisor 1 =
    #i 0 =
    _while #i #decimal_places < _do
        #divisor #divisor 10 * =
        #i ++
    _end
    #res #x #y * #divisor / =
    #res
_end


; #x 100 =
{aklsdfj
askl;dfj'}
#x -200 =
#a #x 200 2 ?multiply_fp =
#a 2 ?print_fp
