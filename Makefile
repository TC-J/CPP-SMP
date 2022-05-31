default: lockfree_demo

lockfree_demo: lockfree_demo.cpp
	@g++ -D DEVMODE -o $@ $< -lpthread

clean: 
	@rm lockfree_demo