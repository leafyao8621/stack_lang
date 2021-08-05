#n 100 =
@a #n _alloc
#su 0 =
#i 2 =
_while #i #n < _do
    #i _println
    #j #i =
    _while #j #n < _do
        @a #j [] 1 =
        #j #j #i + =
    _end
    #c 1 =
    _while #c _do
        #c 0 =
        #i #i 1 + =
        #i #n < _if
            @a #i [] _if
                #c 1 =
            _end
        _end
    _end
    #su #su 1 + =
_end
#su _println
