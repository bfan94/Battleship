# Basic makefile by John (Sean) Greenslade

# Source C/C++ files
SRC = main.cpp GameCore.cpp TUI.cpp net.cpp

# Other settings
OPTI = s

FLAGS = -g -Wall -Wno-reorder -std=c++11 \
-Wno-delete-non-virtual-dtor \
-lcurses -lboost_system -lpthread \
-O$(OPTI) \
-I/usr/include

# Binaries
COMP = g++
RM = rm -f
CP = cp


# Define object files
OBJ = $(SRC:.cpp=.o)


all: main

main: $(OBJ)
	$(COMP) $(FLAGS) -o main $(OBJ) -I.

%.o : %.cpp
	@echo Compiling $<
	$(COMP) -c $(FLAGS) -I. $< -o $@

clean:
	$(RM) $(OBJ)
	$(RM) main
