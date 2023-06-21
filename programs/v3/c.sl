_for %i 2 10 _do
    "i = " _print
    %i _print
    ':' _println
    _for %j 0 %i _do
        %j %i 1 >> > _if
            _break
        _end
        %j 1 & _if
            _continue
        _end
        %j _println
    _end
_end
