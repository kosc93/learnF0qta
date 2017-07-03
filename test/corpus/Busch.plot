##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/Busch.png'

##### general settings #####
set datafile missing '0'
stats 'Busch.qtaf0sampled' using 2:3 nooutput
set xrange [0.215089:0.61843]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - Busch (rmse=1.2833)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'

##### plot syllable bounds #####
set arrow from 0.215089,STATS_min_y-2 to 0.215089,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.618430,STATS_min_y-2 to 0.618430,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.215089,92.862947 to 0.618430,96.896355 nohead dt 4

###### plot curves from data points #####
plot 'Busch.semitonef0' using 2:3 title 'Original F0' with points lt 3 lc '#0000FF' lw 1,\
 'Busch.qtaf0' using 2:3 title 'Resynthesized F0' with points lt 1 lc black lw 2,\
 'Busch.qtaf0sampled' using 2:3 notitle with linespoints lt -1 lc black lw 1
