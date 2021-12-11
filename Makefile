CC = g++
IDIR = -I ./include
Default = day1 day2 day3 day4 day5 day6 day7 day8 day9 day10 day11
#mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))

all : $(Default)
	
day% : lib/luka_string.o
	$(CC) $@/first.cpp $< -o $@/first
	$(CC) $@/second.cpp $< -o $@/second

lib/luka_string.o : lib/luka_string.cpp
	$(CC) -c $< $(IDIR) -o $@

#lib/luka_string.cpp : include/luka_string.h


clean :
	rm day*/first day*/second lib/luka_string.o
