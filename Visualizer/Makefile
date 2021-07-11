CC = g++
CFLAGS = -w
LFLAGS = -lm -lSDL2 -lSDL2_ttf
INC = -I include -L lib -l SDL2-2.0.0


SRCS := $(wildcard *.cpp)
BINS := $(SRCS:%.cpp=%)

all: ${BINS}

%: %.o
		@echo "Checking.."
		${CC} ${LFLAGS} $< -o $@

%.o: %.cpp
		@echo "Creating object.."
		${CC} ${CFLAGS} -c $< ${INC}

clean:
		@echo "Cleaning up..."
		rm -rvf *.o ${BINS}