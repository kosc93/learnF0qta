##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/Souvenir.png'

##### general settings #####
set datafile missing '0'
stats 'Souvenir.qtaf0sampled' using 2:3 nooutput
set xrange [0.257964:1.05743]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - Souvenir (rmse=1.35155)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'

##### plot syllable bounds #####
set arrow from 0.257964,STATS_min_y-2 to 0.257964,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.449262,STATS_min_y-2 to 0.449262,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.586879,STATS_min_y-2 to 0.586879,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.057425,STATS_min_y-2 to 1.057425,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.257964,92.079469 to 0.449262,93.992456 nohead dt 4
set arrow from 0.449262,91.414700 to 0.586879,92.790868 nohead dt 4
set arrow from 0.586879,80.656094 to 1.057425,85.361556 nohead dt 4

###### plot curves from data points #####
plot 'Souvenir.semitonef0' using 2:3 title 'Original F0' with points lt 3 lc '#0000FF' lw 1,\
 'Souvenir.qtaf0' using 2:3 title 'Resynthesized F0' with points lt 1 lc black lw 2,\
 'Souvenir.qtaf0sampled' using 2:3 notitle with linespoints lt -1 lc black lw 1
