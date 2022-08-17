_def ?sieve @arr #n #verbose _begin
    #cnt 0 =
    #i 2 =
    _while #i #n < _do
        #verbose _if
            #i _println
        _end
        #j #i =
        _while #j #n < _do
            @arr #j [] 1 =
            #j #i +=
        _end
        #cont 1 =
        _while #cont _do
            #cont 0 =
            #i ++
            #i #n < _if
                @arr #i [] _if
                    #cont 1 =
                _end
            _end
        _end
        #cnt ++
    _end
    #cnt
_end

@arr[1000000]
"Value of n: " _print
#n _input
"Value of verbose: " _print
#verbose _input
#valid 1 =
#n 1000000 > _if
    "Invalid input" _println
    #valid 0 =
_end

#valid _if
    @arr #n #verbose ?sieve _println
_end

