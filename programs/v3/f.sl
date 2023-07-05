@@%matrix 10 10 _alloc
_for %i 0 9 _do
    _for %j 0 9 _do
        @@%matrix %i [] %j [] %i 1 + %j 1 + * =
    _end
_end

_for %i 0 9 _do
    _for %j 0 9 _do
        @@%matrix %i [] %j [] _print
        ' ' _print
    _end
    '\n' _print
_end
