##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/Abderhalden.png'

##### general settings #####
stats 'Abderhalden.semitonef0' using 2:3 nooutput
set xrange [STATS_min_x:STATS_max_x]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - Abderhalden (rmse=0.135798)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'
set datafile missing '0'

##### plot syllable bounds #####
set arrow from 0.397894,STATS_min_y-2 to 0.397894,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.571064,STATS_min_y-2 to 0.571064,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.733544,STATS_min_y-2 to 0.733544,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.053648,STATS_min_y-2 to 1.053648,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.358879,STATS_min_y-2 to 1.358879,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.397894,92.727384 to 0.571064,92.728371 nohead dt 4
set arrow from 0.571064,95.742619 to 0.733544,91.139499 nohead dt 4
set arrow from 0.733544,99.305203 to 1.053648,92.730894 nohead dt 4
set arrow from 1.053648,87.792500 to 1.358879,88.074211 nohead dt 4

###### plot curves from data points #####
plot 'Abderhalden.semitonef0' using 2:3 title 'Original F0' with linespoints lt -1 lc black lw 2 dt 1, 'Abderhalden.qTAf0' using 2:3 title 'Resynthesized F0' with linespoints lt -1 lc black lw 1 dt 2
