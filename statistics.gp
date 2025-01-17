reset
set title "fib big number"
set xlabel "fib(n)"
set ylabel "time(ns)"
set key top left
set terminal png size 1024,720
set terminal png font "Times_New_Roman,12"
set output "fib_bn.png"
set xtics 0,20,10000000

plot [-10:][-2000:] \
'bn_fib_plot' using 1:2 with linespoints linewidth 2 title "user", \
'bn_fib_plot' using 1:3 with linespoints linewidth 2 title "kernel", \
'bn_fib_plot' using 1:4 with linespoints linewidth 2 title "kernel to user"
