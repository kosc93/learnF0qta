##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/Abbild.png'

##### general settings #####
set datafile missing '0'
stats 'Abbild.qtaf0sampled' using 2:3 nooutput
set xrange [0.356213:1.05361]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - Abbild (rmse=0.308453)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'

##### plot syllable bounds #####
set arrow from 0.356213,STATS_min_y-2 to 0.356213,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.583906,STATS_min_y-2 to 0.583906,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.053610,STATS_min_y-2 to 1.053610,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.356213,96.516392 to 0.583906,94.239461 nohead dt 4
set arrow from 0.583906,88.592099 to 1.053610,89.027768 nohead dt 4

###### plot curves from data points #####
plot 'Abbild.semitonef0' using 2:3 title 'Original F0' with points lt 3 lc '#0000FF' lw 1,\
 'Abbild.qtaf0' using 2:3 title 'Resynthesized F0' with points lt 1 lc black lw 2,\
 'Abbild.qtaf0sampled' using 2:3 notitle with linespoints lt -1 lc black lw 1
