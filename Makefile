OBJS   = main.o metric_spaces.o distances.o hashtable.o misc_functions.o
SOURCE = main.cpp metric_spaces.cpp distances.cpp hashtable.cpp misc_functions.cpp
HEADER = metric_spaces.h distances.h hashtable.h misc_functions.h
OUT    = lsh
CC     = g++
FLAGS  = -g -c -std=c++11

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

main.o: main.cpp 
	$(CC) $(FLAGS) main.cpp
	
metric_spaces.o: metric_spaces.cpp
	$(CC) $(FLAGS) metric_spaces.cpp

distances.o: distances.cpp
	$(CC) $(FLAGS) distances.cpp

hashtable.o: hashtable.cpp
	$(CC) $(FLAGS) hashtable.cpp
	
misc_functions.o: misc_functions.cpp
	$(CC) $(FLAGS) misc_functions.cpp


clean:
	rm -f $(OBJS) $(OUT)

count:
	wc $(SOURCE) $(HEADER)
