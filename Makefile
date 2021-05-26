CXXFLAGS = -Wall 

ifeq ($(DEBUG), 1)
CXXFLAGS += -g
endif

CXXFLAGS += `pkg-config --cflags ncurses`
CXXLIBS += `pkg-config --libs ncurses`

target = main
SRCS= $(notdir $(wildcard *.cpp))
OBJS = $(SRCS:.cpp=.o)
HEADER = $(SRCS:.cpp=.h) 
GCHES = $(SRCS:.cpp=.h.gch)

all : $(target)

%.o : %.cpp  $(HEADER)
	$(CXX) $(CXXFLAGS) $(CXXLIBS)-c $^

$(target) : $(OBJS)
	$(CXX) $(CXXFLAGS) $(CXXLIBS) $^ -o $@ 

.PHONY: clean
clean : 
	rm -f $(OBJS) $(target) $(GCHES)