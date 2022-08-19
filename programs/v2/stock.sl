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
    #remainder _print
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

_def ?check_price #start_price #up_prob #percentage #threshold #time _begin
    #i 0 =
    #cond 1 =
    #price #start_price =
    _while #i #time < #cond && _do
        #r _rand 100 % =
        #r #up_prob 1 - > _if
            #price #price 100 #percentage + 2 ?multiply_fp =
        _else
            #price #price 100 #percentage - 2 ?multiply_fp =
        _end
        #price #threshold < _if
            #cond 0 =
        _end
        #i ++
    _end
    #price
_end

_def ?simulate #start_price #up_prob #percentage #threshold #time #iter #verbose _begin
    #i 0 =
    #cnt 0 =
    _while #i #iter < _do
        #price #start_price #up_prob #percentage #threshold #time ?check_price =
        #verbose _if
            #price 2 ?print_fp
            #price #threshold >= _if
                " success" _println
            _else
                " fail" _println
            _end
        _end
        #price #threshold >= _if
            #cnt ++
        _end
        #i ++
    _end
    #cnt
_end


; 10000 2 ?print_fp
10000 80 2 9000 10 1000 1 ?simulate _println
