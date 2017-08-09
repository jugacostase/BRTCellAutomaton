set view map
set object 1 rect from graph 0, graph 0 to graph 1, graph 1 back
set object 1 rect fc rgb "black" fillstyle solid 1.0
set term gif animate delay 30
set output "car.gif"
stats "datos.dat" nooutput
unset key
set xrange [0:20]
set yrange [-9:9]
set size ratio 1
unset colorbox
do for [i=1:int(STATS_blocks-1)]{splot "datos.dat" index (i-1) using 1:2:3 with points pointtype 5 pointsize 1.5 palette linewidth 6.9}
