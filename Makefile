CXXFLAGS = -Wall 
CXX=g++

ifeq ($(DEBUG), 1)
CXXFLAGS += -g
endif

CXXFLAGS += `pkg-config --cflags ncurses`
CXXLIBS += `pkg-config --libs ncurses`

SRC_DIR := src

EXE := snake

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=%.o)
DEP := $(OBJ:%.o=%.d)


.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ) 
	$(CXX) $(CXXFLAGS) $^ $(CXXLIBS) -o $@

%.o: $(SRC_DIR)/%.cpp  Makefile
	$(CXX) $(CXXFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(DEP)

clean:
	rm $(OBJ) $(EXE) $(DEP)