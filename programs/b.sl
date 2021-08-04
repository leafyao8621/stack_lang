#a 90 =
#a _print
#bc #a 1 + =
#bc _print

#a #bc > _if
    1 _print
_else
    #a 90 >= _if
        2 _print
    _else
        #a 80 > _if
            3 _print
        _else
            4 _print
        _end
    _end
_end
