$a " bottles of beer on the wall, " =
$b " bottles of beer." =
$c "Take one down and pass it around, " =
$d " bottles of beer on the wall." =
#i 99 =
_while #i 1 > _do
    #i _print
    $a _print
    #i _print
    $b _println
    $c _print
    #i --
    #i _print
    $d _println
_end

"1 bottle of beer on the wall, 1 bottle of beer." _println
"Take one down and pass it around, no more beer on the wall!" _println
