##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/Augsburg.png'

##### general settings #####
set datafile missing '0'
stats 'Augsburg.qtaf0sampled' using 2:3 nooutput
set xrange [0.275167:1.20584]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - Augsburg (rmse=0.247516)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'

##### plot syllable bounds #####
set arrow from 0.275167,STATS_min_y-2 to 0.275167,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.647565,STATS_min_y-2 to 0.647565,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.205838,STATS_min_y-2 to 1.205838,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.275167,96.251545 to 0.647565,92.527561 nohead dt 4
set arrow from 0.647565,82.094264 to 1.205838,87.676990 nohead dt 4

###### plot curves from data points #####
plot 'Augsburg.semitonef0' using 2:3 title 'Original F0' with points lt 3 lc '#0000FF' lw 1,\
 'Augsburg.qtaf0' using 2:3 title 'Resynthesized F0' with points lt 1 lc black lw 2,\
 'Augsburg.qtaf0sampled' using 2:3 notitle with linespoints lt -1 lc black lw 1
