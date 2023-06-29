%n 200000 =
@&arr %n _alloc
%cnt 0 =
%i 2 =
_while %i %n < _do
    %i _println
    %cnt ++
    %j %i =
    _while %j %n < _do
        @&arr %j [] '\1' =
        %j %i +=
    _end
    %i ++
    _while %i %n < _do
        @&arr %i [] ! _if
            _break
        _end
        %i ++
    _end
_end
%cnt _println
