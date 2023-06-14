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

0xA00AA00AA00AA00A 0xC00CC00CC00CC00C & _printhex
'\n' _print
%a 0xA00AA00AA00AA00A =
%b 0xC00CC00CC00CC00C =
%a 0xC00CC00CC00CC00C & _printhex
'\n' _print
0xA00AA00AA00AA00A %b & _printhex
'\n' _print
%a %b & _printhex
'\n' _print

'\xA0' '\xC0' & _printhex
'\n' _print
&a '\xA0' =
&b '\xC0' =
&a '\xC0' & _printhex
'\n' _print
'\xA0' &b & _printhex
'\n' _print
&a &b & _printhex
'\n' _print

5.0 _printhex
'\n' _print
5.0 0xfff0000000000000 & _printhex
'\n' _print
#a 5 =
%b 0xfff0000000000000 =
#a 0xfff0000000000000 & _printhex
'\n' _print
5.0 %b & _printhex
'\n' _print
#a %b & _printhex
'\n' _print

0xA00AA00AA00AA00A 0x4880488048804880 | _printhex
'\n' _print
%a 0xA00AA00AA00AA00A =
%b 0x4880488048804880 =
%a 0x4880488048804880 | _printhex
'\n' _print
0xA00AA00AA00AA00A %b | _printhex
'\n' _print
%a %b | _printhex
'\n' _print

'\xA0' '\x48' | _printhex
'\n' _print
&a '\xA0' =
&b '\x48' =
&a '\x48' | _printhex
'\n' _print
'\xA0' &b | _printhex
'\n' _print
&a &b | _printhex
'\n' _print

5.0 _printhex
'\n' _print
5.0 0x000000000000000F | _printhex
'\n' _print
#a 5 =
%b 0x000000000000000F =
#a 0x000000000000000F | _printhex
'\n' _print
5.0 %b | _printhex
'\n' _print
#a %b | _printhex
'\n' _print

0xA00AA00AA00AA00A 0xFFFFFFFFFFFFFFFF ^ _printhex
'\n' _print
%a 0xA00AA00AA00AA00A =
%b 0xFFFFFFFFFFFFFFFF =
%a 0xFFFFFFFFFFFFFFFF ^ _printhex
'\n' _print
0xA00AA00AA00AA00A %b ^ _printhex
'\n' _print
%a %b ^ _printhex
'\n' _print

'\xA0' '\xFF' ^ _printhex
'\n' _print
&a '\xA0' =
&b '\xFF' =
&a '\xFF' ^ _printhex
'\n' _print
'\xA0' &b ^ _printhex
'\n' _print
&a &b ^ _printhex
'\n' _print

5.0 _printhex
'\n' _print
5.0 0xFFFFFFFFFFFFFFFF ^ _printhex
'\n' _print
#a 5 =
%b 0xFFFFFFFFFFFFFFFF =
#a 0xFFFFFFFFFFFFFFFF ^ _printhex
'\n' _print
5.0 %b ^ _printhex
'\n' _print
#a %b ^ _printhex
'\n' _print

0xF00FF00FF00FF00F ! _printhex
'\n' _print
%a 0xF00FF00FF00FF00F =
%a ! _printhex
'\n' _print

0x0 ! _printhex
'\n' _print
%a 0x0 =
%a ! _printhex
'\n' _print

2.0 _printhex
'\n' _print
2.0 ! _printhex
'\n' _print
#a 2 =
#a ! _printhex
'\n' _print

0.0 _printhex
'\n' _print
0.0 ! _printhex
'\n' _print
#a 0 =
#a ! _printhex
'\n' _print

'\0' ! _printhex
'\n' _print
&a '\0' =
&a ! _printhex
'\n' _print

2 0 && _printhex
'\n' _print
%a 2 =
%b 0 =
%a 0 && _printhex
'\n' _print
2 %b && _printhex
'\n' _print
%a %b && _printhex
'\n' _print

2 1 && _printhex
'\n' _print
%a 2 =
%b 1 =
%a 1 && _printhex
'\n' _print
2 %b && _printhex
'\n' _print
%a %b && _printhex
'\n' _print

2.0 1 && _printhex
'\n' _print
#a 2 =
%b 1 =
#a 1 && _printhex
'\n' _print
2.0 %b && _printhex
'\n' _print
#a %b && _printhex
'\n' _print

0.0 1 && _printhex
'\n' _print
#a 0 =
%b 1 =
#a 1 && _printhex
'\n' _print
0.0 %b && _printhex
'\n' _print
#a %b && _printhex
'\n' _print

2.0 1.0 && _printhex
'\n' _print
#a 2 =
#b 1.0 =
#a 1.0 && _printhex
'\n' _print
2.0 #b && _printhex
'\n' _print
#a #b && _printhex
'\n' _print

'\2' '\1' && _printhex
'\n' _print
&a '\2' =
&b '\1' =
&a '\1' && _printhex
'\n' _print
'\2' &b && _printhex
'\n' _print
&a &b && _printhex
'\n' _print

'\2' '\0' && _printhex
'\n' _print
&a '\2' =
&b '\0' =
&a '\0' && _printhex
'\n' _print
'\2' &b && _printhex
'\n' _print
&a &b && _printhex
'\n' _print

0 0 || _printhex
'\n' _print
%a 0 =
%b 0 =
%a 0 || _printhex
'\n' _print
0 %b || _printhex
'\n' _print
%a %b || _printhex
'\n' _print

2 1 || _printhex
'\n' _print
%a 2 =
%b 1 =
%a 1 || _printhex
'\n' _print
2 %b || _printhex
'\n' _print
%a %b || _printhex
'\n' _print

2.0 1 || _printhex
'\n' _print
#a 2 =
%b 1 =
#a 1 || _printhex
'\n' _print
2.0 %b || _printhex
'\n' _print
#a %b || _printhex
'\n' _print

0.0 0 || _printhex
'\n' _print
#a 0 =
%b 0 =
#a 0 || _printhex
'\n' _print
0.0 %b || _printhex
'\n' _print
#a %b || _printhex
'\n' _print

2.0 1.0 || _printhex
'\n' _print
#a 2 =
#b 1.0 =
#a 1.0 || _printhex
'\n' _print
2.0 #b || _printhex
'\n' _print
#a #b || _printhex
'\n' _print

'\2' '\1' || _printhex
'\n' _print
&a '\2' =
&b '\1' =
&a '\1' || _printhex
'\n' _print
'\2' &b || _printhex
'\n' _print
&a &b || _printhex
'\n' _print

'\0' '\0' || _printhex
'\n' _print
&a '\0' =
&b '\0' =
&a '\0' || _printhex
'\n' _print
'\0' &b || _printhex
'\n' _print
&a &b || _printhex
'\n' _print

%a 10 =
%a 10 == _if
    "TEN" _println
_else %a 9 == _if
    "NINE" _println
_else %a 8 == _if
    "EIGHT" _println
_end
_end
_end

%a 9 =
%a 10 == _if
    "TEN" _println
_else %a 9 == _if
    "NINE" _println
_else %a 8 == _if
    "EIGHT" _println
_end
_end
_end

#a 8 =
#a 10 == _if
    "TEN" _println
_else #a 9 == _if
    "NINE" _println
_else #a 8 == _if
    "EIGHT" _println
_end
_end
_end

&a '\8' =
&a '\x0A' == _if
    "TEN" _println
_else &a '\9' == _if
    "NINE" _println
_else &a '\8' == _if
    "EIGHT" _println
_end
_end
_end

1 2 != _if
    "NEQ" _println
_else
    "EQ" _println
_end

1 1 != _if
    "NEQ" _println
_else
    "EQ" _println
_end

%a 11 =
%a 10 > _if
    "TEN" _println
_else %a 9 > _if
    "NINE" _println
_else %a 8 > _if
    "EIGHT" _println
_end
_end
_end

%a 10 =
%a 10 > _if
    "TEN" _println
_else %a 9 > _if
    "NINE" _println
_else %a 8 > _if
    "EIGHT" _println
_end
_end
_end

#a 9 =
#a 10 > _if
    "TEN" _println
_else #a 9 > _if
    "NINE" _println
_else #a 8 > _if
    "EIGHT" _println
_end
_end
_end

&a '\9' =
&a '\x0A' > _if
    "TEN" _println
_else &a '\9' > _if
    "NINE" _println
_else &a '\8' > _if
    "EIGHT" _println
_end
_end
_end

%a 7 =
%a 8 < _if
    "TEN" _println
_else %a 9 < _if
    "NINE" _println
_else %a 10 < _if
    "EIGHT" _println
_end
_end
_end

%a 8 =
%a 8 < _if
    "TEN" _println
_else %a 9 < _if
    "NINE" _println
_else %a 10 < _if
    "EIGHT" _println
_end
_end
_end

#a 9 =
#a 8 < _if
    "TEN" _println
_else #a 9 < _if
    "NINE" _println
_else #a 10 < _if
    "EIGHT" _println
_end
_end
_end

&a '\9' =
&a '\8' < _if
    "TEN" _println
_else &a '\9' < _if
    "NINE" _println
_else &a '\x0A' < _if
    "EIGHT" _println
_end
_end
_end

%a 8 =
%a 8 <= _if
    "TEN" _println
_else %a 9 <= _if
    "NINE" _println
_else %a 10 <= _if
    "EIGHT" _println
_end
_end
_end

%a 9 =
%a 8 <= _if
    "TEN" _println
_else %a 9 <= _if
    "NINE" _println
_else %a 10 <= _if
    "EIGHT" _println
_end
_end
_end

#a 10 =
#a 8 <= _if
    "TEN" _println
_else #a 9 <= _if
    "NINE" _println
_else #a 10 <= _if
    "EIGHT" _println
_end
_end
_end

&a '\x0A' =
&a '\8' <= _if
    "TEN" _println
_else &a '\9' <= _if
    "NINE" _println
_else &a '\x0A' <= _if
    "EIGHT" _println
_end
_end
_end

%a 11 =
%a 10 >= _if
    "TEN" _println
_else %a 9 >= _if
    "NINE" _println
_else %a 8 >= _if
    "EIGHT" _println
_end
_end
_end

%a 9 =
%a 10 >= _if
    "TEN" _println
_else %a 9 >= _if
    "NINE" _println
_else %a 8 >= _if
    "EIGHT" _println
_end
_end
_end

#a 8 =
#a 10 >= _if
    "TEN" _println
_else #a 9 >= _if
    "NINE" _println
_else #a 8 >= _if
    "EIGHT" _println
_end
_end
_end

&a '\8' =
&a '\x0A' >= _if
    "TEN" _println
_else &a '\9' >= _if
    "NINE" _println
_else &a '\8' >= _if
    "EIGHT" _println
_end
_end
_end

%a 1 =
%a ++
%a _println

&a '\1' =
&a ++
&a _printhex
'\n' _print

%a 1 =
%a --
%a _println

&a '\1' =
&a --
&a _printhex
'\n' _print

%a 1 =
%a 2 +=
%a _println
#b 2 =
%a #b +=
%a _println
%a 2.0 +=
%a _println

#a 1 =
#a 2 +=
#a _println
%b 2 =
#a %b +=
#a _println
#a 2.0 +=
#a _println

&a '\1' =
&a '\2' +=
&a _printhex
'\n' _print
&b '\2' =
&a &b +=
&a _printhex
'\n' _print

%a 1 =
%a 2 -=
%a _println
#b 2 =
%a #b -=
%a _println
%a 2.0 -=
%a _println

#a 1 =
#a 2 -=
#a _println
%b 2 =
#a %b -=
#a _println
#a 2.0 -=
#a _println

&a '\1' =
&a '\2' -=
&a _printhex
'\n' _print
&b '\2' =
&a &b -=
&a _printhex
'\n' _print

%a 1 =
%a 2 *=
%a _println
#b 2 =
%a #b *=
%a _println
%a 2.0 *=
%a _println

#a 1 =
#a 2 *=
#a _println
%b 2 =
#a %b *=
#a _println
#a 2.0 *=
#a _println
