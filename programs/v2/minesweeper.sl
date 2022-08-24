_def ?initialize @board _begin
    #i 0 =
    _while #i 10 < _do
        #cond 1 =
        _while #cond _do
            #idx _rand 63 & =
            #cond @board #idx [] 16 & =
        _end
        @board #idx [] 16 =
        #row #idx 3 >> =
        #col #idx 7 & =
        #r #row 1 - =
        _while #r #row 2 + < _do
            #c #col 1 - =
            _while #c #col 2 + < _do
                #r 0 >=
                #r 8 < &&
                #c 0 >= &&
                #c 8 < && _if
                    #cur #r 3 << #c | =
                    @board #cur [] ++
                _end
                #c ++
            _end
            #r ++
        _end
        #i ++
    _end
_end

_def ?show @board #reveal _begin
    #idx 0 =
    #i 0 =
    _while #i 8 < _do
        #j 0 =
        _while #j 8 < _do
            #cur @board #idx [] =
            #reveal _if
                #cur 16 & _if
                    "* " _print
                _else
                    #cur 15 & _print
                    " " _print
                _end
            _end
            #j ++
            #idx ++
        _end
        "" _println
        #i ++
    _end
_end

@board[64]
@board ?initialize
@board 1 ?show
