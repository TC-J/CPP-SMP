.PHONEY: run

default: all

all: lockfree_demo channel_demo run

lockfree_demo: lockfree_demo.cpp smp.hpp
	@g++ -D DEVMODE -o $@ $< -lpthread

channel_demo: channel_demo.cpp channels.hpp
	@g++ -D DEVMODE -o $@ $< -lpthread

run: 
	@./channel_demo

clean: 
	@rm lockfree_demo channel_demo