CC      = g++
FLAGS   = -std=c++20 -Wall -Werror -Wextra -Wno-unused-variable

GTEST_DIR  = D:/googletest/googletest
GTEST_SRC  = $(GTEST_DIR)/src/gtest-all.cc
GTEST_OBJ  = gtest-all.o
INCLUDES   = -I$(GTEST_DIR)/include -I$(GTEST_DIR)

TARGET = 02
OBJS   = tests.o allocator.o $(GTEST_OBJ)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(FLAGS) -o $@ $(OBJS)

tests.o: tests.cpp allocator.hpp
	$(CC) $(FLAGS) $(INCLUDES) -c $<

allocator.o: allocator.cpp allocator.hpp
	$(CC) $(FLAGS) -c $<

$(GTEST_OBJ): $(GTEST_SRC)
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf *.o $(TARGET)
