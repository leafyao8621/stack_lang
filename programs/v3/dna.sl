$sequence "AAAAAAAA" =
$lookup "ATGC" =
_for %i 0 0xffff _do
    %msk 3 =
    _for %j 0 8 _do
        $sequence %j [] $lookup %i %msk & %j 1 << >> [] =
        %msk 2 <<=
    _end
    $sequence _println
_end
