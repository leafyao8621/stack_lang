#sum 0 =
#i 0 =
_while #i 1000000 < _do
    #sum #sum #i + =
    #i #i 1 + =
_end

#sum _println
