##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/Cembalo.png'

##### general settings #####
set datafile missing '0'
stats 'Cembalo.qtaf0sampled' using 2:3 nooutput
set xrange [0.340929:1.07833]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - Cembalo (rmse=0.371508)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'

##### plot syllable bounds #####
set arrow from 0.340929,STATS_min_y-2 to 0.340929,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.622467,STATS_min_y-2 to 0.622467,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.754084,STATS_min_y-2 to 0.754084,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.078331,STATS_min_y-2 to 1.078331,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.340929,70.000000 to 0.622467,67.184622 nohead dt 4
set arrow from 0.622467,91.155974 to 0.754084,89.839807 nohead dt 4
set arrow from 0.754084,92.543071 to 1.078331,89.300592 nohead dt 4

###### plot curves from data points #####
plot 'Cembalo.semitonef0' using 2:3 title 'Original F0' with points lt 3 lc '#0000FF' lw 1,\
 'Cembalo.qtaf0' using 2:3 title 'Resynthesized F0' with points lt 1 lc black lw 2,\
 'Cembalo.qtaf0sampled' using 2:3 notitle with linespoints lt -1 lc black lw 1
