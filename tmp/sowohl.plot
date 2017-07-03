##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/sowohl.png'

##### general settings #####
set datafile missing '0'
stats 'sowohl.qtaf0sampled' using 2:3 nooutput
set xrange [0.258237:1.13159]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - sowohl (rmse=1.29072)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'

##### plot syllable bounds #####
set arrow from 0.258237,STATS_min_y-2 to 0.258237,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.544560,STATS_min_y-2 to 0.544560,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.131590,STATS_min_y-2 to 1.131590,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.258237,88.289817 to 0.544560,91.153045 nohead dt 4
set arrow from 0.544560,78.741938 to 1.131590,84.612241 nohead dt 4

###### plot curves from data points #####
plot 'sowohl.semitonef0' using 2:3 title 'Original F0' with points lt 3 lc '#0000FF' lw 1,\
 'sowohl.qtaf0' using 2:3 title 'Resynthesized F0' with points lt 1 lc black lw 2,\
 'sowohl.qtaf0sampled' using 2:3 notitle with linespoints lt -1 lc black lw 1
