##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/ähnlich.png'

##### general settings #####
stats 'ähnlich.semitonef0' using 2:3 nooutput
set xrange [STATS_min_x:STATS_max_x]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - ähnlich (rmse=0.163548)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'
set datafile missing '0'

##### plot syllable bounds #####
set arrow from 0.271184,STATS_min_y-2 to 0.271184,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.537204,STATS_min_y-2 to 0.537204,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.936397,STATS_min_y-2 to 0.936397,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.271184,100.581339 to 0.537204,92.734665 nohead dt 4
set arrow from 0.537204,87.245582 to 0.936397,86.695561 nohead dt 4

###### plot curves from data points #####
plot 'ähnlich.semitonef0' using 2:3 title 'Original F0' with linespoints lt -1 lc black lw 2 dt 1, 'ähnlich.qTAf0' using 2:3 title 'Resynthesized F0' with linespoints lt -1 lc black lw 1 dt 2
