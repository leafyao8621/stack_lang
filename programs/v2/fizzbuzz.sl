#i 1 =
"Value of n: " _print
#n _input
_while #i #n < _do
    #var_x #i 3 % ! =
    #y #i 5 % ! =
    #var_x #y || _if
        #var_x _if
            "fizz" _print
        _end
        #y _if
            "buzz" _print
        _end
    _else
        #i _print
    _end
    "" _println
    #i #i 1 + =
_end
