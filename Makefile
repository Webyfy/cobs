CC=gcc
CFLAGS=-g -Wall -Wextra
RM=rm -rf

SRC=src
OBJ=obj
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

TEST=test
TESTBIN=$(TEST)/bin
TESTS=$(wildcard $(TEST)/*.c)
TESTBINS=$(patsubst $(TEST)/%.c, $(TESTBIN)/%, $(TESTS))

LIBDIR=lib
LIB=$(LIBDIR)/cobs.a

# all:$(LIB)
all: $(OBJS)

release: CFLAGS=-Wall -Wextra -O2 -DNDEBUG
release: clean
release: $(LIB)

$(LIB): $(OBJS)
	@mkdir -p $(@D)
	$(RM) -f $(LIB)
	ar -cvrs $(LIB) $(OBJS)

$(OBJ)/%.o: $(SRC)/%.c $(SRC)/%.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/%.o: $(SRC)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(TESTBIN)/%: $(TEST)/%.c $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $< $(OBJS) -o $@

test: $(TESTBINS)
	for test_item in $(TESTBINS); do ./$$test_item ; done

clean:
	$(RM) $(LIBDIR) $(TESTBIN) $(OBJ)

.PHONY: clean test