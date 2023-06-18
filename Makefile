all:
	g++ main.cpp -I/usr/include/i386-linux-gnu/c++/4.8 -I/usr/include/c++/4.8 -I/usr/include  -std=c++11 -lstdc++ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o ./confetti
run:
	./app
