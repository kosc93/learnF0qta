##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/ähnlich.png'

##### general settings #####
set datafile missing '0'
stats 'ähnlich.qtaf0sampled' using 2:3 nooutput
set xrange [0.301184:0.966397]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - ähnlich (rmse=0.866362)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'

##### plot syllable bounds #####
set arrow from 0.301184,STATS_min_y-2 to 0.301184,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.567204,STATS_min_y-2 to 0.567204,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.966397,STATS_min_y-2 to 0.966397,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.301184,95.258325 to 0.567204,92.598120 nohead dt 4
set arrow from 0.567204,85.152910 to 0.966397,89.144834 nohead dt 4

###### plot curves from data points #####
plot 'ähnlich.semitonef0' using 2:3 title 'Original F0' with points lt 3 lc '#0000FF' lw 1,\
 'ähnlich.qtaf0' using 2:3 title 'Resynthesized F0' with points lt 1 lc black lw 2,\
 'ähnlich.qtaf0sampled' using 2:3 notitle with linespoints lt -1 lc black lw 1
