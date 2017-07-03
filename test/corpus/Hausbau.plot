##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/Hausbau.png'

##### general settings #####
set datafile missing '0'
stats 'Hausbau.qtaf0sampled' using 2:3 nooutput
set xrange [0.179475:0.940548]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - Hausbau (rmse=0.182646)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'

##### plot syllable bounds #####
set arrow from 0.179475,STATS_min_y-2 to 0.179475,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.537845,STATS_min_y-2 to 0.537845,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.940548,STATS_min_y-2 to 0.940548,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.179475,93.445307 to 0.537845,93.083932 nohead dt 4
set arrow from 0.537845,85.633163 to 0.940548,89.660202 nohead dt 4

###### plot curves from data points #####
plot 'Hausbau.semitonef0' using 2:3 title 'Original F0' with points lt 3 lc '#0000FF' lw 1,\
 'Hausbau.qtaf0' using 2:3 title 'Resynthesized F0' with points lt 1 lc black lw 2,\
 'Hausbau.qtaf0sampled' using 2:3 notitle with linespoints lt -1 lc black lw 1
