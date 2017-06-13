##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/Abbild2.png'

###### general settings #####
stats 'Abbild2.semitonef0' using 2:3 nooutput
set xrange [STATS_min_x:STATS_max_x]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - Abbild2'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'
set datafile missing '0'

##### plot syllable bounds #####
set arrow from 0.276041,STATS_min_y-2 to 0.276041,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.506783,STATS_min_y-2 to 0.506783,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.006211,STATS_min_y-2 to 1.006211,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.276041,96.399732 to 0.506783,93.872970 nohead dt 4
set arrow from 0.506783,88.824012 to 1.006211,87.738907 nohead dt 4

###### plot curves from data points #####
plot 'Abbild2.semitonef0' using 2:3 title 'Original F0' with linespoints lt -1 lc black lw 2 dt 1, 'Abbild2.qTAf0' using 2:3 title 'Resynthesized F0' with linespoints lt -1 lc black lw 1 dt 2
