CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

SRCS = main.cpp sched_manage.cpp
OBJS = $(SRCS:.cpp=.o)

all: my_program

my_program: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

main.o: main.cpp sched_manage.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

sched_manage.o: sched_manage.cpp sched_manage.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) my_program
