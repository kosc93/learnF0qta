##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/Abderhalden.png'

###### general settings #####
stats 'Abderhalden.semitonef0' using 2:3 nooutput
set xrange [STATS_min_x:STATS_max_x]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - Abderhalden'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'
set datafile missing '0'

##### plot syllable bounds #####
set arrow from 0.427894,STATS_min_y-2 to 0.427894,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.601064,STATS_min_y-2 to 0.601064,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.763544,STATS_min_y-2 to 0.763544,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.083648,STATS_min_y-2 to 1.083648,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.388879,STATS_min_y-2 to 1.388879,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.427894,92.766024 to 0.601064,92.709966 nohead dt 4
set arrow from 0.601064,62.566877 to 0.763544,80.439697 nohead dt 4
set arrow from 0.763544,96.263761 to 1.083648,91.959524 nohead dt 4
set arrow from 1.083648,88.000591 to 1.388879,88.223203 nohead dt 4

###### plot curves from data points #####
plot 'Abderhalden.semitonef0' using 2:3 title 'Original F0' with linespoints lt -1 lc black lw 2 dt 1, 'Abderhalden.qTAf0' using 2:3 title 'Resynthesized F0' with linespoints lt -1 lc black lw 1 dt 2
