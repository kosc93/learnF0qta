##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/Wust.png'

##### general settings #####
set datafile missing '0'
stats 'Wust.qtaf0sampled' using 2:3 nooutput
set xrange [0.177553:0.914586]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - Wust (rmse=1.90289)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'

##### plot syllable bounds #####
set arrow from 0.177553,STATS_min_y-2 to 0.177553,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.914586,STATS_min_y-2 to 0.914586,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.177553,96.293790 to 0.914586,88.923458 nohead dt 4

###### plot curves from data points #####
plot 'Wust.semitonef0' using 2:3 title 'Original F0' with points lt 3 lc '#0000FF' lw 1,\
 'Wust.qtaf0' using 2:3 title 'Resynthesized F0' with points lt 1 lc black lw 2,\
 'Wust.qtaf0sampled' using 2:3 notitle with linespoints lt -1 lc black lw 1
