%a 0 =
_while %a 100 < _do
    %a ++
    %a 3 % ! %a 5 % ! || _if
        %a 3 % ! _if
            "fizz" _print
        _end
        %a 5 % ! _if
            "buzz" _print
        _end
    _else
        %a _print
    _end
    '\n' _print
_end
