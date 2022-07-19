#i 1 =
_while #i 1000 < _do
    #x #i 3 % ! =
    #y #i 5 % ! =
    #x #y || _if
        #x _if
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
