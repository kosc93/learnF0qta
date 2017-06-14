##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/ab.png'

##### general settings #####
stats 'ab.semitonef0' using 2:3 nooutput
set xrange [STATS_min_x:STATS_max_x]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - ab (rmse=0.152218)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'
set datafile missing '0'

##### plot syllable bounds #####
set arrow from 0.176406,STATS_min_y-2 to 0.176406,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.479569,STATS_min_y-2 to 0.479569,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.176406,93.546556 to 0.479569,93.915750 nohead dt 4

###### plot curves from data points #####
plot 'ab.semitonef0' using 2:3 title 'Original F0' with linespoints lt -1 lc black lw 2 dt 1, 'ab.qTAf0' using 2:3 title 'Resynthesized F0' with linespoints lt -1 lc black lw 1 dt 2
