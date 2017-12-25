# Makefile for Project 1 of CS485, Spring 2015.
# This project is to manipulate binary data in C.


#This code was taken from http://www.cs.uky.edu/~raphael/courses/CS485/proj1/Makefile
GOAL = combineFiles
CFLAGS = -Wall
FILES = $(wildcard *.data)
OUTPUT = result.out
GOODPROG = goodCombineFiles

default: $(GOAL)

$(GOODPROG):
	wget http://www.cs.uky.edu/~raphael/courses/CS485/proj1/$(GOODPROG)
	chmod +x $(GOODPROG)

run: $(GOAL) $(FILES)
	./$(GOAL) $(FILES) > $(OUTPUT)

clean:
	rm -f $(GOAL) $(OUTPUT)

submit: $(GOAL).c Makefile README.txt
	tar -czf $(GOAL).tgz $(GOAL).c Makefile README.txt

