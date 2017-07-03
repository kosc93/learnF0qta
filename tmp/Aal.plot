##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/Aal.png'

##### general settings #####
set datafile missing '0'
stats 'Aal.qtaf0sampled' using 2:3 nooutput
set xrange [0.240627:0.704564]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - Aal (rmse=0.545794)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'

##### plot syllable bounds #####
set arrow from 0.240627,STATS_min_y-2 to 0.240627,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.704564,STATS_min_y-2 to 0.704564,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.240627,81.943921 to 0.704564,86.583288 nohead dt 4

###### plot curves from data points #####
plot 'Aal.semitonef0' using 2:3 title 'Original F0' with points lt 3 lc '#0000FF' lw 1,\
 'Aal.qtaf0' using 2:3 title 'Resynthesized F0' with points lt 1 lc black lw 2,\
 'Aal.qtaf0sampled' using 2:3 notitle with linespoints lt -1 lc black lw 1
