##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/Souvenir_var.png'

##### general settings #####
set datafile missing '0'
stats 'Souvenir_var.qtaf0sampled' using 2:3 nooutput
set xrange [0.243663:1.1147]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - Souvenir_var (rmse=0.991445)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'

##### plot syllable bounds #####
set arrow from 0.243663,STATS_min_y-2 to 0.243663,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.482534,STATS_min_y-2 to 0.482534,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.628857,STATS_min_y-2 to 0.628857,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.114701,STATS_min_y-2 to 1.114701,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.243663,70.000000 to 0.482534,67.611298 nohead dt 4
set arrow from 0.482534,94.533577 to 0.628857,93.070341 nohead dt 4
set arrow from 0.628857,94.388097 to 1.114701,89.529657 nohead dt 4

###### plot curves from data points #####
plot 'Souvenir_var.semitonef0' using 2:3 title 'Original F0' with points lt 3 lc '#0000FF' lw 1,\
 'Souvenir_var.qtaf0' using 2:3 title 'Resynthesized F0' with points lt 1 lc black lw 2,\
 'Souvenir_var.qtaf0sampled' using 2:3 notitle with linespoints lt -1 lc black lw 1
