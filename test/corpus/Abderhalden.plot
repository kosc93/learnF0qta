##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/Abderhalden.png'

##### general settings #####
set datafile missing '0'
stats 'Abderhalden.qtaf0sampled' using 2:3 nooutput
set xrange [0.427894:1.38888]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - Abderhalden (rmse=0.338034)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'

##### plot syllable bounds #####
set arrow from 0.427894,STATS_min_y-2 to 0.427894,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.601064,STATS_min_y-2 to 0.601064,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.763544,STATS_min_y-2 to 0.763544,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.083648,STATS_min_y-2 to 1.083648,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.388879,STATS_min_y-2 to 1.388879,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.427894,91.138604 to 0.601064,92.870301 nohead dt 4
set arrow from 0.601064,88.685209 to 0.763544,90.310011 nohead dt 4
set arrow from 0.763544,95.633789 to 1.083648,92.432746 nohead dt 4
set arrow from 1.083648,87.432517 to 1.388879,88.488363 nohead dt 4

###### plot curves from data points #####
plot 'Abderhalden.semitonef0' using 2:3 title 'Original F0' with points lt 3 lc '#0000FF' lw 1,\
 'Abderhalden.qtaf0' using 2:3 title 'Resynthesized F0' with points lt 1 lc black lw 2,\
 'Abderhalden.qtaf0sampled' using 2:3 notitle with linespoints lt -1 lc black lw 1
