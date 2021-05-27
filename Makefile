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


.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ) 
	$(CXX) $(CXXFLAGS) $^ $(CXXLIBS) -o $@

%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	rm $(OBJ) $(EXE)