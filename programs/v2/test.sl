_def ?sum_up #n _begin
    #i 0 =
    #out 0 =
    _while #i #n < _do
        #i ++
        #out #i +=
    _end
    #out
_end

_def ?sq #n _begin 
    #n #n *
_end

#n ?sq ?sum_up _println
