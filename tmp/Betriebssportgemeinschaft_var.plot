##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/Betriebssportgemeinschaft_var.png'

##### general settings #####
set datafile missing '0'
stats 'Betriebssportgemeinschaft_var.qtaf0sampled' using 2:3 nooutput
set xrange [0.281842:2.05353]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - Betriebssportgemeinschaft_var (rmse=0.42356)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'

##### plot syllable bounds #####
set arrow from 0.281842,STATS_min_y-2 to 0.281842,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.352395,STATS_min_y-2 to 0.352395,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.742395,STATS_min_y-2 to 0.742395,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.116799,STATS_min_y-2 to 1.116799,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.222955,STATS_min_y-2 to 1.222955,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.486303,STATS_min_y-2 to 1.486303,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 2.053525,STATS_min_y-2 to 2.053525,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.281842,88.546687 to 0.352395,89.252209 nohead dt 4
set arrow from 0.352395,93.034584 to 0.742395,89.134584 nohead dt 4
set arrow from 0.742395,92.957845 to 1.116799,96.701887 nohead dt 4
set arrow from 1.116799,88.850587 to 1.222955,89.912145 nohead dt 4
set arrow from 1.222955,90.061120 to 1.486303,89.042568 nohead dt 4
set arrow from 1.486303,92.119701 to 2.053525,86.447477 nohead dt 4

###### plot curves from data points #####
plot 'Betriebssportgemeinschaft_var.semitonef0' using 2:3 title 'Original F0' with points lt 3 lc '#0000FF' lw 1,\
 'Betriebssportgemeinschaft_var.qtaf0' using 2:3 title 'Resynthesized F0' with points lt 1 lc black lw 2,\
 'Betriebssportgemeinschaft_var.qtaf0sampled' using 2:3 notitle with linespoints lt -1 lc black lw 1
