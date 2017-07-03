##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/Thomas.png'

##### general settings #####
set datafile missing '0'
stats 'Thomas.qtaf0sampled' using 2:3 nooutput
set xrange [0.464321:1.18738]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - Thomas (rmse=1.27674)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'

##### plot syllable bounds #####
set arrow from 0.464321,STATS_min_y-2 to 0.464321,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.670363,STATS_min_y-2 to 0.670363,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.187381,STATS_min_y-2 to 1.187381,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.464321,93.364955 to 0.670363,95.425382 nohead dt 4
set arrow from 0.670363,87.697932 to 1.187381,86.773588 nohead dt 4

###### plot curves from data points #####
plot 'Thomas.semitonef0' using 2:3 title 'Original F0' with points lt 3 lc '#0000FF' lw 1,\
 'Thomas.qtaf0' using 2:3 title 'Resynthesized F0' with points lt 1 lc black lw 2,\
 'Thomas.qtaf0sampled' using 2:3 notitle with linespoints lt -1 lc black lw 1
