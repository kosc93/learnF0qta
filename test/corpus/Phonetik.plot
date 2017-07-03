##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/Phonetik.png'

##### general settings #####
set datafile missing '0'
stats 'Phonetik.qtaf0sampled' using 2:3 nooutput
set xrange [0.283985:1.12733]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - Phonetik (rmse=0.393738)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'

##### plot syllable bounds #####
set arrow from 0.283985,STATS_min_y-2 to 0.283985,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.472836,STATS_min_y-2 to 0.472836,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.669093,STATS_min_y-2 to 0.669093,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.127334,STATS_min_y-2 to 1.127334,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.283985,92.465595 to 0.472836,94.354104 nohead dt 4
set arrow from 0.472836,94.223108 to 0.669093,96.041026 nohead dt 4
set arrow from 0.669093,83.140140 to 1.127334,87.722552 nohead dt 4

###### plot curves from data points #####
plot 'Phonetik.semitonef0' using 2:3 title 'Original F0' with points lt 3 lc '#0000FF' lw 1,\
 'Phonetik.qtaf0' using 2:3 title 'Resynthesized F0' with points lt 1 lc black lw 2,\
 'Phonetik.qtaf0sampled' using 2:3 notitle with linespoints lt -1 lc black lw 1
