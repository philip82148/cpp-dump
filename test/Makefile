main := dump_test.cpp
ESCAPED_MAIN := $(shell echo $(main) | sed -e 's/ /\\ /g')
out := ./build/$(shell basename $(main) .cpp).out
ESCAPED_OUT := $(shell echo $(out) | sed -e 's/ /\\ /g')

COMPILER := g++
STD := -std=c++17
OPTIONS := -Wall -Wextra -Wfloat-equal -Werror
OUT := -o ${ESCAPED_OUT}
SRCS := $(ESCAPED_MAIN)

build:clean ${SRCS}
	$(COMPILER) $(STD) $(INC) $(OPTIONS) $(DEFINES) $(OUT) $(SRCS)

.PHONY:clean
clean:
	rm -rf $(out)
