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
    #cnt 0 =
    _while #temp 0 > _do
        #temp #temp 10 / =
        #cnt ++
    _end
    #cnt _println
_end

903 2 ?print_fp
