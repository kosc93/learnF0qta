##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/Licht.png'

##### general settings #####
set datafile missing '0'
stats 'Licht.qtaf0sampled' using 2:3 nooutput
set xrange [0.203262:0.740599]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - Licht (rmse=0.897775)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'

##### plot syllable bounds #####
set arrow from 0.203262,STATS_min_y-2 to 0.203262,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.740599,STATS_min_y-2 to 0.740599,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.203262,92.707129 to 0.740599,98.080500 nohead dt 4

###### plot curves from data points #####
plot 'Licht.semitonef0' using 2:3 title 'Original F0' with points lt 3 lc '#0000FF' lw 1,\
 'Licht.qtaf0' using 2:3 title 'Resynthesized F0' with points lt 1 lc black lw 2,\
 'Licht.qtaf0sampled' using 2:3 notitle with linespoints lt -1 lc black lw 1
