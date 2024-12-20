#!/bin/sh

BIN="r55"
LIBS="-L -lraylib -lm -ldl"

SRC_DIR="./src"
BUILD_DIR="./build"
OBJ_DIR="./src/obj"
LIB_DIR="./src/lib"
INC_DIR="./src/include"

CC="gcc"
CFLAGS="-Wall -Wextra -ggdb"
LDFLAGS="-L${LIB_DIR} -I${SRC_DIR} -I${INC_DIR} -lraylib -lm"
FLAGS="${CFLAGS} ${LDFLAGS}"

mkdir -p ./${BUILD_DIR}/
$CC $CFLAGS -o ./${BUILD_DIR}/libhotpot.so -fPIC -shared ./src/hotpot.c $LDFLAGS
$CC $CFLAGS -o ./${BUILD_DIR}/${BIN} ./src/main.c ./src/sds.c -L./build/ $LDFLAGS
