0 #a =
_while #a 100 < _do
    #a 10 < _if
        1 _print
    _else
        #a 20 < _if
            2 _print
        _else
            #a _print
        _end
    _end
    #a 1 + #a =
_end
