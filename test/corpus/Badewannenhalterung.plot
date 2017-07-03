##### output file options #####
set terminal pngcairo size 700,254 enhanced font 'Verdana,10'
set output '../plots/Badewannenhalterung.png'

##### general settings #####
set datafile missing '0'
stats 'Badewannenhalterung.qtaf0sampled' using 2:3 nooutput
set xrange [0.448765:1.79959]
set yrange [STATS_min_y-2:STATS_max_y+2]
set title 'F0 - Badewannenhalterung (rmse=0.628373)'
set xlabel 'Time [sec]'
set ylabel 'Frequency [st]'

##### plot syllable bounds #####
set arrow from 0.448765,STATS_min_y-2 to 0.448765,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.620413,STATS_min_y-2 to 0.620413,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.754672,STATS_min_y-2 to 0.754672,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 0.883833,STATS_min_y-2 to 0.883833,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.041672,STATS_min_y-2 to 1.041672,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.279813,STATS_min_y-2 to 1.279813,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.425969,STATS_min_y-2 to 1.425969,STATS_max_y+2 nohead dt 3 lt -1
set arrow from 1.799593,STATS_min_y-2 to 1.799593,STATS_max_y+2 nohead dt 3 lt -1

###### plot targets #####
set arrow from 0.448765,98.344099 to 0.620413,96.627619 nohead dt 4
set arrow from 0.620413,90.655130 to 0.754672,89.312536 nohead dt 4
set arrow from 0.754672,91.344752 to 0.883833,90.053143 nohead dt 4
set arrow from 0.883833,88.874503 to 1.041672,89.549728 nohead dt 4
set arrow from 1.041672,70.000000 to 1.279813,67.618589 nohead dt 4
set arrow from 1.279813,91.943195 to 1.425969,90.481637 nohead dt 4
set arrow from 1.425969,87.651129 to 1.799593,90.018862 nohead dt 4

###### plot curves from data points #####
plot 'Badewannenhalterung.semitonef0' using 2:3 title 'Original F0' with points lt 3 lc '#0000FF' lw 1,\
 'Badewannenhalterung.qtaf0' using 2:3 title 'Resynthesized F0' with points lt 1 lc black lw 2,\
 'Badewannenhalterung.qtaf0sampled' using 2:3 notitle with linespoints lt -1 lc black lw 1
