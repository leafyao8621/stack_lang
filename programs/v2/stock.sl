_def ?print_fp X #x #decimal_places _begin
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

_def ?multiply_fp # #x #y #decimal_places _begin
    #divisor 1 =
    #i 0 =
    _while #i #decimal_places < _do
        #divisor #divisor 10 * =
        #i ++
    _end
    #res #x #y * #divisor / =
    #res
_end

_def ?check_price # #start_price #up_prob #rate #threshold #time _begin
    #i 0 =
    #cond 1 =
    #price #start_price =
    _while #i #time < #cond && _do
        #r _rand 100 % =
        #r #up_prob 1 - > _if
            #price #price 1000 #rate + 3 ?multiply_fp =
        _else
            #price #price 1000 #rate - 3 ?multiply_fp =
        _end
        #price #threshold < _if
            #cond 0 =
        _end
        #i ++
    _end
    #price
_end

_def ?simulate #
    #start_price #up_prob #rate #threshold #time #iter #verbose _begin
    #i 0 =
    #cnt 0 =
    _while #i #iter < _do
        #price #start_price #up_prob #rate #threshold #time ?check_price =
        #verbose _if
            #price 3 ?print_fp
            #price #threshold >= _if
                " success" _println
            _else
                " failure" _println
            _end
        _end
        #price #threshold >= _if
            #cnt ++
        _end
        #i ++
    _end
    #cnt
_end

"Seed: " _print
#seed _input
"Start price in 1/10 cents: " _print
#start_price _input
"Up probability in %: " _print
#up_prob _input
"Change in 1/1000: " _print
#percentage _input
"Threshold price in 1/10 cents: " _print
#threshold _input
"Time span: " _print
#time _input
"Number of iterations: " _print
#iter _input
"Verbose: " _print
#verbose _input

#seed _srand
#success
    #start_price #up_prob #percentage #threshold #time #iter #verbose
    ?simulate =
#success _print
"/" _print
#iter _print
" success" _println
