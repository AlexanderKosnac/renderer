CXX = g++
CFLAGS += -g -O3 -DLINUX -Wall -Wextra

EXEC = renderer
SRCS = $(shell find ./src -name "*.cpp")
OBJS = $(SRCS:.cpp=.o)

LFLAGS += -L/usr/X11R6/lib -lX11 -lfmt
INCLUDES = -I. -Iinclude -Iglm -I/usr/X11R6/include

TEST_EXEC = runUnitTests
TEST_SRCS = $(shell find ./src ./tests -name "*.cpp" -a ! -name "main.cpp")
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

GTEST_LIBS = -lgtest -lgtest_main -lpthread
TEST_CFLAGS = $(CFLAGS) -I/usr/include/gtest

$(EXEC): $(OBJS)
	$(CXX) -o $(EXEC) $(OBJS) $(CFLAGS) $(LIBS) $(LFLAGS)

$(TEST_EXEC): $(TEST_OBJS)
	$(CXX) -o $(TEST_EXEC) $(TEST_OBJS) $(CFLAGS) $(GTEST_LIBS) $(LFLAGS)

%.o: %.cpp
	$(CXX) -c $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	rm -f $(EXEC) $(TEST_EXEC)
	find . -type f -name "*.o" -delete

test: $(TEST_EXEC)
	./$(TEST_EXEC)
