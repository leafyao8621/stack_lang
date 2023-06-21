&a '\xff' =
_while &a _do
    &a _printhex
    '\n' _print
    &a '\x0f' == _if
        _break
    _end
    &a '\x20' & _if
        &a '\1' >>>=
        _continue
    _end
    &a --
_end
