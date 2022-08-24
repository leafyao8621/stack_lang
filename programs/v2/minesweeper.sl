_def ?get_marked # #state _begin
    #out #state 3840 & 8 >> =
    #out
_end

_def ?inc_marked # #state _begin
    #state #state 127 & #state 3840 & 256 + 3840 & | =
    #state
_end

_def ?dec_marked # #state _begin
    #state #state 127 & #state 3840 & 256 - 3840 & | =
    #state
_end

_def ?get_rem # #state _begin
    #state #state 63 & =
    #state
_end

_def ?dec_rem # #state _begin
    #state --
    #state
_end

_def ?set_loss # #state _begin
    #state #state 4096 | =
    #state
_end

_def ?get_loss # #state _begin
    #state #state 4096 & =
    #state
_end

_def ?initialize # @board _begin
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
    #state 54 =
    #state
_end

_def ?show X @board #reveal _begin
    " " _print
    #i 0 =
    _while #i 8 < _do
        " " _print
        #i _print
        #i ++
    _end
    "" _println
    #idx 0 =
    #i 0 =
    _while #i 8 < _do
        #i _print
        #j 0 =
        _while #j 8 < _do
            #cur @board #idx [] =
            "|" _print
            #reveal _if
                #cur 16 & _if
                    "*" _print
                _else
                    #cur 15 & _print
                _end
            _else
                #cur 32 & _if
                    #cur 15 & _print
                _else
                    #cur 64 & _if
                        "f" _print
                    _else
                        "_" _print
                    _end
                _end
            _end
            #j ++
            #idx ++
        _end
        "" _println
        #i ++
    _end
_end

_def ?mark # @board #row #col #state _begin
    #idx #row 3 << #col + =
    #cur @board #idx [] 64 & =
    #cur _if
        #state #state ?dec_marked =
    _else
        #state #state ?inc_marked =
    _end
    @board #idx [] @board #idx [] 64 ^ =
    #state
_end

_def ?check # @board #row #col #state @stack _begin
    #idx #row 3 << #col + =
    #cur @board #idx [] =
    #cur 16 &
    #cur 64 & ! && _if
        #state #state ?set_loss =
    _end
    #state 4096 & _if
        #state
    _else
        @board #idx [] #cur 32 | =
        #cur 15 & ! _if
            #stack_ptr 0 =
            @board #stack_ptr [] #idx =
            #stack_ptr ++
            _while #stack_ptr _do
                #stack_ptr --
            _end
        _end
        #state
    _end
_end

_def ?show_game_state X @board #state _begin
    #state ?get_loss _if
        "Loss" _println
        @board 1 ?show
    _else
        @board 0 ?show
        "Marked: " _print
        #state ?get_marked _println
        "Remaining: " _print
        #state ?get_rem _println
    _end
_end
@board[64]
@stack[64]
#state @board ?initialize =
@board 1 ?show

#state @board 0 0 #state ?mark =
@board #state ?show_game_state

#state @board 0 0 #state ?mark =
@board #state ?show_game_state


#state @board 0 0 #state @stack ?check =
@board #state ?show_game_state
