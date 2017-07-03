##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/ab.png'

##### general settings #####
set datafile missing '0'
stats 'ab.qtaf0sampled' using 2:3 nooutput
set xrange [0.206406:0.509569]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - ab (rmse=0.135609)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'

##### plot syllable bounds #####
set arrow from 0.206406,STATS_min_y-2 to 0.206406,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.509569,STATS_min_y-2 to 0.509569,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.206406,95.146409 to 0.509569,92.114781 nohead dt 4

###### plot curves from data points #####
plot 'ab.semitonef0' using 2:3 title 'Original F0' with points lt 3 lc '#0000FF' lw 1,\
 'ab.qtaf0' using 2:3 title 'Resynthesized F0' with points lt 1 lc black lw 2,\
 'ab.qtaf0sampled' using 2:3 notitle with linespoints lt -1 lc black lw 1
