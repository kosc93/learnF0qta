##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/Abbild.png'

###### general settings #####
stats 'Abbild.semitonef0' using 2:3 nooutput
set xrange [STATS_min_x:STATS_max_x]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - Abbild'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'
set datafile missing '0'

##### plot syllable bounds #####
set arrow from 0.356213,STATS_min_y-2 to 0.356213,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.583906,STATS_min_y-2 to 0.583906,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.053610,STATS_min_y-2 to 1.053610,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.356213,95.426756 to 0.583906,94.900026 nohead dt 4
set arrow from 0.583906,88.457958 to 1.053610,89.913004 nohead dt 4

###### plot curves from data points #####
plot 'Abbild.semitonef0' using 2:3 title 'Original F0' with linespoints lt -1 lc black lw 2 dt 1, 'Abbild.qTAf0' using 2:3 title 'Resynthesized F0' with linespoints lt -1 lc black lw 1 dt 2
