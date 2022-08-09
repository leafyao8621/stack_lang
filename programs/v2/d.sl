#var_a 90 =
#var_a _print
#bc #var_a 1 + =
#bc _print

#var_a #bc > _if
    1 _print
_else
    #var_a 90 >= _if
        2 _print
    _else
        #var_a 80 > _if
            3 _print
        _else
            4 _print
        _end
    _end
_end
