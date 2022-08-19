_def ?f #n _begin
    #i 0 =
    _while #i #n < _do
        _rand _println
        #i ++
    _end
_end

1000 _srand
10 ?f
