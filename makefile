CC = gcc
LIBS = -lpthread
DEFINES = 
FLAGS = -g -Wall  
EXECS = philosopher_lock philosopher_no_lock

all: $(EXECS)

philosopher_lock: philosopher_lock.c
	$(CC) $(FLAGS) -o $@ $? $(LIBS)

philosopher_no_lock: philosopher_no_lock.c
	$(CC) $(FLAGS) -o $@ $? $(LIBS)

clean:
	$(RM) $(EXECS)
