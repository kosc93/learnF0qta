##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/Regenwassersammelbecken.png'

##### general settings #####
set datafile missing '0'
stats 'Regenwassersammelbecken.qtaf0sampled' using 2:3 nooutput
set xrange [0.216579:1.7065]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - Regenwassersammelbecken (rmse=0.632075)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'

##### plot syllable bounds #####
set arrow from 0.216579,STATS_min_y-2 to 0.216579,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.412333,STATS_min_y-2 to 0.412333,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.591797,STATS_min_y-2 to 0.591797,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.725288,STATS_min_y-2 to 0.725288,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.900269,STATS_min_y-2 to 0.900269,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.060819,STATS_min_y-2 to 1.060819,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.219565,STATS_min_y-2 to 1.219565,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.380115,STATS_min_y-2 to 1.380115,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.706495,STATS_min_y-2 to 1.706495,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.216579,102.672971 to 0.412333,100.715429 nohead dt 4
set arrow from 0.412333,92.849593 to 0.591797,91.054956 nohead dt 4
set arrow from 0.591797,91.584030 to 0.725288,90.249120 nohead dt 4
set arrow from 0.725288,92.550120 to 0.900269,90.800306 nohead dt 4
set arrow from 0.900269,91.649621 to 1.060819,90.044121 nohead dt 4
set arrow from 1.060819,91.544037 to 1.219565,89.956577 nohead dt 4
set arrow from 1.219565,91.304362 to 1.380115,89.698862 nohead dt 4
set arrow from 1.380115,91.872357 to 1.706495,89.915295 nohead dt 4

###### plot curves from data points #####
plot 'Regenwassersammelbecken.semitonef0' using 2:3 title 'Original F0' with points lt 3 lc '#0000FF' lw 1,\
 'Regenwassersammelbecken.qtaf0' using 2:3 title 'Resynthesized F0' with points lt 1 lc black lw 2,\
 'Regenwassersammelbecken.qtaf0sampled' using 2:3 notitle with linespoints lt -1 lc black lw 1
