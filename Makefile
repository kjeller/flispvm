# flispvm make
# Karl Strålman

# Directories
BUILD_DIR=obj
SRC_DIR=src
INC_DIR=include
TEST_DIR=test/src
GTEST_DIR=test/googletest/googletest/src
GMOCK_DIR=test/googletest/googlemock/src
GTEST_INC_DIR=test/googletest/googletest/include/ test/googletest/googletest/
GMOCK_INC_DIR=test/googletest/googlemock/include/

# Object files 
OBJ=main.o file_utils.o flispvm.o
TEST_OBJ=flisp_test.o
GTEST_OBJ=gtest_main.o gtest.o

# Environment
CC=gcc
C_FLAGS=
L_FLAGS=
COMP=${CC} -I${INC_DIR} -c ${C_FLAGS}
LINK=${CC} ${L_FLAGS}

.PHONY: all test clean

${BUILD_DIR}/%.o: ${SRC_DIR}/%.c
	${COMP} $^ -o $@

${BUILD_DIR}/%.o: ${TEST_DIR}/%.cc
	${COMP} $^ -o $@

${BUILD_DIR}/%.o: ${GTEST_DIR}/%.cc
	${COMP} $^ -o $@ ${addprefix -I, ${GTEST_INC_DIR}}		

all: ${addprefix ${BUILD_DIR}/, ${OBJ}}
	@mkdir -p ${BUILD_DIR}
	${LINK} $^ -o flispvm

test: ${addprefix ${BUILD_DIR}/, ${TEST_OBJ}} ${addprefix ${BUILD_DIR}/, ${GTEST_OBJ}}
	${MAKE} all
	${LINK} $^ ${addprefix ${BUILD_DIR}/, ${OBJ}}
 
clean:
	rm -f ${addprefix ${BUILD_DIR}/, ${OBJ}}
