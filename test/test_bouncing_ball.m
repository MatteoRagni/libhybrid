clear all
close all

compile()
run()

data = load('test.out', '-ascii');

figure();
  plot3(data(:,1), data(:,2), data(:,3), '.');
  grid on;
  xlabel('t');
  ylabel('j');
  zlabel('x1');


function compile()
  !gcc -Wall -c -I../librk4 ../librk4/librk4.c -o librk4.o
  !gcc -Wall -c -I.. -I../librk4 ../libhybrid.c -o libhybrid.o
  !gcc -Wall -c -I.. -I../librk4 test_bouncing_ball.c -o test_bouncing_ball.o
  !gcc librk4.o libhybrid.o test_bouncing_ball.o -o test.bin
end 

function run()
  !test.bin > test.out
end