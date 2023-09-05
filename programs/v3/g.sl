_def ?#f %a #a $a &b _begin
    #b
_end

_def ?g $a _begin
    $a _println
_end

_def ?#h #a _begin
    #a 2 *
_end

"abc" _println
123 ?#h
"bcd" ?g
