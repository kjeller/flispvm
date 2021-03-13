# flispvm make
# Karl Strålman

BUILD_DIR=obj
SRC_DIR=src
INC_DIR=include

OBJ=main.o

CC=gcc
C_FLAGS=-o flispvm
COMP=${CC} -I ${INC_DIR} -c
LINK=${CC} ${C_FLAGS}

${BUILD_DIR}/%.o: ${SRC_DIR}/%.c
	${COMP} $^ -o $@

all: ${addprefix ${BUILD_DIR}/, ${OBJ}}
	@mkdir -p ${BUILD_DIR}
	${LINK} $^

clean:
	rm -f ${addprefix ${BUILD_DIR}/, ${OBJ}}
