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

3 2 % _println
%a 3 =
%b 2 =
%a 2 % _println
3 %b % _println
%a %b % _println

0x80 1 << _printhex
'\n' _print
%a 0x80 =
%b 1 =
%a 1 << _printhex
'\n' _print
0x80 %b << _printhex
'\n' _print
%a %b << _printhex
'\n' _print

'\x40' '\1' << _printhex
'\n' _print
&a '\x40' =
&b '\1' =
&a '\1' << _printhex
'\n' _print
'\x40' &b << _printhex
'\n' _print
&a &b << _printhex
'\n' _print

2.0 _printhex
'\n' _print
2.0 1 << _printhex
'\n' _print
#a 2 =
%b 1 =
#a 1 << _printhex
'\n' _print
2.0 %b << _printhex
'\n' _print
#a %b << _printhex
'\n' _print

0x8000000000000000 1 >> _printhex
'\n' _print
%a 0x8000000000000000 =
%b 1 =
%a 1 >> _printhex
'\n' _print
0x8000000000000000 %b >> _printhex
'\n' _print
%a %b >> _printhex
'\n' _print

'\x80' '\1' >> _printhex
'\n' _print
&a '\x80' =
&b '\1' =
&a '\1' >> _printhex
'\n' _print
'\x80' &b >> _printhex
'\n' _print
&a &b >> _printhex
'\n' _print

2.0 _printhex
'\n' _print
2.0 1 >> _printhex
'\n' _print
#a 2 =
%b 1 =
#a 1 >> _printhex
'\n' _print
2.0 %b >> _printhex
'\n' _print
#a %b >> _printhex
'\n' _print

0x8000000000000000 1 >>> _printhex
'\n' _print
%a 0x8000000000000000 =
%b 1 =
%a 1 >>> _printhex
'\n' _print
0x8000000000000000 %b >>> _printhex
'\n' _print
%a %b >>> _printhex
'\n' _print

'\x80' '\1' >>> _printhex
'\n' _print
&a '\x80' =
&b '\1' =
&a '\1' >>> _printhex
'\n' _print
'\x80' &b >>> _printhex
'\n' _print
&a &b >>> _printhex
'\n' _print

2.0 _printhex
'\n' _print
2.0 1 >>> _printhex
'\n' _print
#a 2 =
%b 1 =
#a 1 >>> _printhex
'\n' _print
2.0 %b >>> _printhex
'\n' _print
#a %b >>> _printhex
'\n' _print

0xF00FF00FF00FF00F ~ _printhex
'\n' _print
%a 0xF00FF00FF00FF00F =
%a ~ _printhex
'\n' _print

2.0 _printhex
'\n' _print
2.0 ~ _printhex
'\n' _print
#a 2 =
#a ~ _printhex
'\n' _print

'\xF0' ~ _printhex
'\n' _print
&a '\xF0' =
&a ~ _printhex
'\n' _print
