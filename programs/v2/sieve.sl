#n 10000 =
@arr[10000]
#cnt 0 =
#i 2 =
_while #i #n < _do
    #i _println
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
#cnt _println
