##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/Abbild2.png'

##### general settings #####
set datafile missing '0'
stats 'Abbild2.qtaf0sampled' using 2:3 nooutput
set xrange [0.276041:1.00621]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - Abbild2 (rmse=0.79198)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'

##### plot syllable bounds #####
set arrow from 0.276041,STATS_min_y-2 to 0.276041,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.506783,STATS_min_y-2 to 0.506783,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.006211,STATS_min_y-2 to 1.006211,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.276041,97.310759 to 0.506783,95.003341 nohead dt 4
set arrow from 0.506783,87.956430 to 1.006211,88.272499 nohead dt 4

###### plot curves from data points #####
plot 'Abbild2.semitonef0' using 2:3 title 'Original F0' with points lt 3 lc '#0000FF' lw 1,\
 'Abbild2.qtaf0' using 2:3 title 'Resynthesized F0' with points lt 1 lc black lw 2,\
 'Abbild2.qtaf0sampled' using 2:3 notitle with linespoints lt -1 lc black lw 1
