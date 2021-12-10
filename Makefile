CC = g++
IDIR = -I ./include
Default = day1 day2 day3 day4 day5 day6 day7 day8 day9 day10
#mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))

all : $(Default)
	
day% : lib/luka_string.o
	$(CC) $@/first.cpp $< -o $@/first
	$(CC) $@/second.cpp $< -o $@/second

lib/luka_string.o :
	$(CC) -c lib/luka_string.cpp $(IDIR) -o $@

clean :
	rm day*/first day*/second lib/luka_string.o
