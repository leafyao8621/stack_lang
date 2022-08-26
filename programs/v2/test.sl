_def ?sum # @arr #len _begin
    #i 0 =
    #out 0 =
    _while #i #len < _do
        #out @arr #i [] +=
        #i ++
    _end
    #out
_end

@arr[100]
#i 0 =
_while #i 100 < _do
    @arr #i [] #i 1 + =
    #i ++
_end

@arr 100 ?sum _println
