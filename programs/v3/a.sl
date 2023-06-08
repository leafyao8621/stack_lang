'\1' '\1' - _if
    "a" _println
_else
    1 1.0 - _if
        "b" _println
    _else
        "c" _println
    _end
_end

1 1 + _if
    $a "aa" =
_else
    $a "bb" =
_end
$a _println
$b $a =
$b _println
$a _println
$a _printhex
'\n' _print
2 3.0 * _println

3 2 / _println
3.0 2 / _println
3.0 2.0 / _println
3 2.0 / _println
