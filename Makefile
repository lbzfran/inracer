TARGET := r55

SRC_DIR=./src
BUILD_DIR=./build
OBJ_DIR=./src/obj
LIB_DIR=./src/lib
INC_DIR=./src/include

CC=gcc
CFLAGS=-Wall
LDFLAGS=-L$(LIB_DIR) -I$(SRC_DIR) -I$(INC_DIR) -lraylib -lm
FLAGS=$(CFLAGS) $(LDFLAGS)

install:
	$(CC) -o $(BUILD_DIR)/$(TARGET) $(SRC_DIR)/*.c $(FLAGS)

clean:
	rm -f $(TARGET)
