##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/substantivisch.png'

##### general settings #####
set datafile missing '0'
stats 'substantivisch.qtaf0sampled' using 2:3 nooutput
set xrange [0.527172:1.71445]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - substantivisch (rmse=1.52294)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'

##### plot syllable bounds #####
set arrow from 0.527172,STATS_min_y-2 to 0.527172,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.817661,STATS_min_y-2 to 0.817661,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.028344,STATS_min_y-2 to 1.028344,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.274307,STATS_min_y-2 to 1.274307,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.714450,STATS_min_y-2 to 1.714450,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.527172,93.955933 to 0.817661,91.051045 nohead dt 4
set arrow from 0.817661,92.296720 to 1.028344,90.189890 nohead dt 4
set arrow from 1.028344,96.221898 to 1.274307,93.762273 nohead dt 4
set arrow from 1.274307,120.000000 to 1.714450,124.401435 nohead dt 4

###### plot curves from data points #####
plot 'substantivisch.semitonef0' using 2:3 title 'Original F0' with points lt 3 lc '#0000FF' lw 1,\
 'substantivisch.qtaf0' using 2:3 title 'Resynthesized F0' with points lt 1 lc black lw 2,\
 'substantivisch.qtaf0sampled' using 2:3 notitle with linespoints lt -1 lc black lw 1
