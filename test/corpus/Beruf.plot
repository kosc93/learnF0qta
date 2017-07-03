##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/Beruf.png'

##### general settings #####
set datafile missing '0'
stats 'Beruf.qtaf0sampled' using 2:3 nooutput
set xrange [0.365705:1.05304]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - Beruf (rmse=1.36626)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'

##### plot syllable bounds #####
set arrow from 0.365705,STATS_min_y-2 to 0.365705,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.465372,STATS_min_y-2 to 0.465372,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.053044,STATS_min_y-2 to 1.053044,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.365705,89.291877 to 0.465372,90.288538 nohead dt 4
set arrow from 0.465372,93.647313 to 1.053044,87.770590 nohead dt 4

###### plot curves from data points #####
plot 'Beruf.semitonef0' using 2:3 title 'Original F0' with points lt 3 lc '#0000FF' lw 1,\
 'Beruf.qtaf0' using 2:3 title 'Resynthesized F0' with points lt 1 lc black lw 2,\
 'Beruf.qtaf0sampled' using 2:3 notitle with linespoints lt -1 lc black lw 1
