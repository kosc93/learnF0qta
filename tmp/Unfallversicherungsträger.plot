##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/Unfallversicherungsträger.png'

##### general settings #####
set datafile missing '0'
stats 'Unfallversicherungsträger.qtaf0sampled' using 2:3 nooutput
set xrange [0.380158:2.12206]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - Unfallversicherungsträger (rmse=0.507549)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'

##### plot syllable bounds #####
set arrow from 0.380158,STATS_min_y-2 to 0.380158,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.530590,STATS_min_y-2 to 0.530590,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.793847,STATS_min_y-2 to 0.793847,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.956156,STATS_min_y-2 to 0.956156,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.076898,STATS_min_y-2 to 1.076898,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.229310,STATS_min_y-2 to 1.229310,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.529286,STATS_min_y-2 to 1.529286,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.815206,STATS_min_y-2 to 1.815206,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 2.122060,STATS_min_y-2 to 2.122060,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.380158,97.130999 to 0.530590,98.089695 nohead dt 4
set arrow from 0.530590,86.913381 to 0.793847,89.545952 nohead dt 4
set arrow from 0.793847,92.795252 to 0.956156,91.172163 nohead dt 4
set arrow from 0.956156,91.486071 to 1.076898,90.278651 nohead dt 4
set arrow from 1.076898,93.235943 to 1.229310,91.711823 nohead dt 4
set arrow from 1.229310,90.351564 to 1.529286,89.604987 nohead dt 4
set arrow from 1.529286,93.219903 to 1.815206,90.360704 nohead dt 4
set arrow from 1.815206,90.057729 to 2.122060,89.818352 nohead dt 4

###### plot curves from data points #####
plot 'Unfallversicherungsträger.semitonef0' using 2:3 title 'Original F0' with points lt 3 lc '#0000FF' lw 1,\
 'Unfallversicherungsträger.qtaf0' using 2:3 title 'Resynthesized F0' with points lt 1 lc black lw 2,\
 'Unfallversicherungsträger.qtaf0sampled' using 2:3 notitle with linespoints lt -1 lc black lw 1
