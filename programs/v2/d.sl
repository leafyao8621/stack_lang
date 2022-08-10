#var_a 90 =
#var_a _println
#bc #var_a 1 + =
#bc _println

#var_a #bc < _if
    -1 _println
_end

#var_a #bc > _if
    1 _println
_else
    #var_a 90 >= _if
        2 _println
    _else
        #var_a 80 > _if
            3 _println
        _else
            4 _println
        _end
    _end
_end
