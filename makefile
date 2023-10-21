all: main.cpp unixTime.o fake_receiver.o data_converter.o data_processing.o check_statuses.o state_functions.o thread_functions.o include/types.h include/common.h
	g++ main.cpp unixTime.o fake_receiver.o data_converter.o data_processing.o check_statuses.o state_functions.o thread_functions.o -o main

fake_receiver.o: fake_receiver.c
	g++ -c fake_receiver.c

unixTime.o: src/unixTime.cpp
	g++ -c src/unixTime.cpp

data_processing.o: data_converter.o include/types.h src/data_processing.cpp
	g++ -c src/data_processing.cpp

data_converter.o: src/data_converter.cpp
	g++ -c src/data_converter.cpp

state_functions.o: src/state_functions.cpp include/common.h thread_functions.o check_statuses.o
	g++ -c src/state_functions.cpp

thread_functions.o: src/thread_functions.cpp data_processing.o fake_receiver.o include/common.h include/types.h
	g++ -c src/thread_functions.cpp

check_statuses.o: src/check_statuses.cpp include/types.h
	g++ -c src/check_statuses.cpp

clean:
	rm -rf *.o *.csv file*.txt main