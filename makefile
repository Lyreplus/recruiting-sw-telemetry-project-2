all: main.cpp unixTime.o fake_receiver.o types.h data_converter.o data_processing.o
	g++ main.cpp fake_receiver.o unixTime.o data_converter.o data_processing.o -o main

fake_receiver.o: fake_receiver.c
	g++ -c fake_receiver.c

unixTime.o: unixTime.cpp
	g++ -c unixTime.cpp

data_processing.o: data_converter.o types.h data_processing.cpp
	g++ -c data_processing.cpp

data_converter.o: data_converter.cpp
	g++ -c data_converter.cpp

clean:
	rm -rf *.o main *.csv *.txt