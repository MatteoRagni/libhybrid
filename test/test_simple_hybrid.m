clear all
close all

compile()
run()

data = load('test.out', '-ascii');

figure();
  subplot(2, 1, 1);
  plot(data(:,2), data(:,4));
  subplot(2, 1, 2);
  plot(data(:,2), data(:,5));


function compile()
  !gcc -Wall -c -I../librk4 ../librk4/librk4.c -o librk4.o
  !gcc -Wall -c -I.. -I../librk4 ../libhybrid.c -o libhybrid.o
  !gcc -Wall -c -I.. -I../librk4 test_simple_hybrid.c -o test_simple_hybrid.o
  !gcc librk4.o libhybrid.o test_simple_hybrid.o -o test.bin
end 

function run()
  !test.bin > test.out
end