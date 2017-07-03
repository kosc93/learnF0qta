##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/Geburt.png'

##### general settings #####
set datafile missing '0'
stats 'Geburt.qtaf0sampled' using 2:3 nooutput
set xrange [0.12888:0.857399]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - Geburt (rmse=1.33436)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'

##### plot syllable bounds #####
set arrow from 0.128880,STATS_min_y-2 to 0.128880,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.223295,STATS_min_y-2 to 0.223295,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.857399,STATS_min_y-2 to 0.857399,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.128880,88.947717 to 0.223295,89.891866 nohead dt 4
set arrow from 0.223295,95.133730 to 0.857399,88.792696 nohead dt 4

###### plot curves from data points #####
plot 'Geburt.semitonef0' using 2:3 title 'Original F0' with points lt 3 lc '#0000FF' lw 1,\
 'Geburt.qtaf0' using 2:3 title 'Resynthesized F0' with points lt 1 lc black lw 2,\
 'Geburt.qtaf0sampled' using 2:3 notitle with linespoints lt -1 lc black lw 1
