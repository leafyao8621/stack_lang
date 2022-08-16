_def ?f #n _begin
    #i 0 =
    _while #i #n < _do 
        #i _println
        #i ++
    _end
    #i 0 =
    _while #i #n < _do 
        #i _println
        #i ++
    _end
    #n 10 < _if 
        "abc" _println
    _else
        "def" _println
    _end
    #n 2 < _if 
        "xxx" _println
    _else
        "zzz" _println
    _end
_end

3 ?f
