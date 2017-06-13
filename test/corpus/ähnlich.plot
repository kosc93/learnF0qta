##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/ähnlich.png'

###### general settings #####
stats 'ähnlich.semitonef0' using 2:3 nooutput
set xrange [STATS_min_x:STATS_max_x]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - ähnlich'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'
set datafile missing '0'

##### plot syllable bounds #####
set arrow from 0.301184,STATS_min_y-2 to 0.301184,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.567204,STATS_min_y-2 to 0.567204,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.966397,STATS_min_y-2 to 0.966397,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.301184,98.921702 to 0.567204,91.611906 nohead dt 4
set arrow from 0.567204,87.328469 to 0.966397,86.642901 nohead dt 4

###### plot curves from data points #####
plot 'ähnlich.semitonef0' using 2:3 title 'Original F0' with linespoints lt -1 lc black lw 2 dt 1, 'ähnlich.qTAf0' using 2:3 title 'Resynthesized F0' with linespoints lt -1 lc black lw 1 dt 2
