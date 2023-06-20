%a 0 =
_while %a 100 < _do
    %a ++
    &cond_3 %a 3 % ! =
    &cond_5 %a 5 % ! =
    &cond_3 &cond_5 || _if
        &cond_3 _if
            "fizz" _print
        _end
        &cond_5 _if
            "buzz" _print
        _end
    _else
        %a _print
    _end
    '\n' _print
_end
