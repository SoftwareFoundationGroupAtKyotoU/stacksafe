unset key
set terminal pdf monochrome enhanced font "Helvetica,16"
set title 'Analysis cost'
set logscale y
set logscale x
set xlabel 'Number of instructions'
set ylabel 'Execution time [ms]'
set nomxtics
set nomytics
set size ratio 0.5
FILE='insts-time.dat'
natlog(x)=log(x)/log(10)
bin(x,width)=width*floor(natlog(x)/width)+width/2.0
binwidth=0.1
plot [0.5:20000] FILE using 1:2 with points pt 1 ps 0.5
