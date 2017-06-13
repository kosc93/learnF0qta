##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/Aal.png'

###### general settings #####
stats 'Aal.semitonef0' using 2:3 nooutput
set xrange [STATS_min_x:STATS_max_x]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - Aal'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'
set datafile missing '0'

##### plot syllable bounds #####
set arrow from 0.240627,STATS_min_y-2 to 0.240627,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.704564,STATS_min_y-2 to 0.704564,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.240627,16.817383 to 0.704564,67.850425 nohead dt 4

###### plot curves from data points #####
plot 'Aal.semitonef0' using 2:3 title 'Original F0' with linespoints lt -1 lc black lw 2 dt 1, 'Aal.qTAf0' using 2:3 title 'Resynthesized F0' with linespoints lt -1 lc black lw 1 dt 2
