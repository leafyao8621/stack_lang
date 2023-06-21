_for %a 0 10 _do
    %a 1 & _if
        _continue
    _end
    %a _println
    %a 5 == _if
        _break
    _end
_end
