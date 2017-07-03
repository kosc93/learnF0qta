##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/boshaft.png'

##### general settings #####
set datafile missing '0'
stats 'boshaft.qtaf0sampled' using 2:3 nooutput
set xrange [0.491143:1.31539]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - boshaft (rmse=0.475359)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'

##### plot syllable bounds #####
set arrow from 0.491143,STATS_min_y-2 to 0.491143,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.807278,STATS_min_y-2 to 0.807278,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.315385,STATS_min_y-2 to 1.315385,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.491143,95.364009 to 0.807278,93.408637 nohead dt 4
set arrow from 0.807278,89.564326 to 1.315385,86.946757 nohead dt 4

###### plot curves from data points #####
plot 'boshaft.semitonef0' using 2:3 title 'Original F0' with points lt 3 lc '#0000FF' lw 1,\
 'boshaft.qtaf0' using 2:3 title 'Resynthesized F0' with points lt 1 lc black lw 2,\
 'boshaft.qtaf0sampled' using 2:3 notitle with linespoints lt -1 lc black lw 1
