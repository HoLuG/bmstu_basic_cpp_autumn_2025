CC      = g++
FLAGS   = -std=c++20 -Wall -Werror -Wextra -Wno-unused-variable

GTEST_DIR  = D:/googletest/googletest
GTEST_SRC  = $(GTEST_DIR)/src/gtest-all.cc
GTEST_OBJ  = gtest-all.o
INCLUDES   = -I$(GTEST_DIR)/include -I$(GTEST_DIR)

TARGET = 06
OBJS   = tests.o vector.o $(GTEST_OBJ)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(FLAGS) -o $@ $(OBJS)

tests.o: tests.cpp vector.hpp
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

vector.o: vector.cpp vector.hpp
	$(CC) $(FLAGS) -c $< -o $@

$(GTEST_OBJ): $(GTEST_SRC)
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

test: $(TARGET)
	./$(TARGET)

clean:
	rm -rf *.o $(TARGET)
