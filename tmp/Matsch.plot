##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/Matsch.png'

##### general settings #####
set datafile missing '0'
stats 'Matsch.qtaf0sampled' using 2:3 nooutput
set xrange [0.198497:0.710839]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - Matsch (rmse=0.424487)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'

##### plot syllable bounds #####
set arrow from 0.198497,STATS_min_y-2 to 0.198497,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.710839,STATS_min_y-2 to 0.710839,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.198497,92.010345 to 0.710839,97.133767 nohead dt 4

###### plot curves from data points #####
plot 'Matsch.semitonef0' using 2:3 title 'Original F0' with points lt 3 lc '#0000FF' lw 1,\
 'Matsch.qtaf0' using 2:3 title 'Resynthesized F0' with points lt 1 lc black lw 2,\
 'Matsch.qtaf0sampled' using 2:3 notitle with linespoints lt -1 lc black lw 1
