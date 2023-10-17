all: main.cpp unixTime.o fake_receiver.o
	g++ main.cpp fake_receiver.o unixTime.o -o main

fake_receiver.o: fake_receiver.c
	g++ -c fake_receiver.c

unixTime.o: unixTime.cpp
	g++ -c unixTime.cpp

clean:
	rm -rf *.o main