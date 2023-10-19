all: main.cpp unixTime.o fake_receiver.o data_converter.o data_processing.o check_statuses.o state_functions.o thread_functions.o types.h common.h
	g++ main.cpp unixTime.o fake_receiver.o data_converter.o data_processing.o check_statuses.o state_functions.o thread_functions.o -o main

fake_receiver.o: fake_receiver.c
	g++ -c fake_receiver.c

unixTime.o: unixTime.cpp
	g++ -c unixTime.cpp

data_processing.o: data_converter.o types.h data_processing.cpp
	g++ -c data_processing.cpp

data_converter.o: data_converter.cpp
	g++ -c data_converter.cpp

state_functions.o: state_functions.cpp common.h thread_functions.o check_statuses.o
	g++ -c state_functions.cpp

thread_functions.o: thread_functions.cpp data_processing.o fake_receiver.o common.h types.h
	g++ -c thread_functions.cpp

check_statuses.o: check_statuses.cpp types.h
	g++ -c check_statuses.cpp

clean:
	rm -rf *.o main *.csv *.txt