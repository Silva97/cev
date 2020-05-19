CFLAGS=-I include \
	-O2 \
	-Wall \
	-Werror \
	-pedantic-errors \
	-std=c11

SRCDIR=src
OBJDIR=obj
BIN=cev
INSTPATH=/usr/local/bin

src2obj = $(subst .c,.o, \
	$(subst \
		$(SRCDIR), \
		$(OBJDIR), \
		$(1) \
	) \
)

SRC=$(wildcard src/cev/*.c)
OBJ=$(call src2obj,$(SRC))
	
all: release
debug: CFLAGS += -ggdb
debug: release

release: starting $(OBJ) main.o
	$(CC) $(CFLAGS) $(OBJ) $(OBJDIR)/main.o -o $(BIN)

lib: starting $(OBJ)
	ar r libcev.a $(OBJ)

starting:
	mkdir -p obj/cev

doc:
	doxygen

clean:
	rm -rf obj/

install:
	cp $(BIN) $(INSTPATH)/$(BIN)

uninstall:
	rm $(INSTPATH)/$(BIN)

main.o: src/main.c
	$(CC) $(CFLAGS) -c $< -o $(OBJDIR)/$@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

test: starting $(OBJ)
	$(CC) $(CFLAGS) -DANSI_COLORS -c tests/test_$(name).c -o $(OBJDIR)/test_$(name).o
	$(CC) $(CFLAGS) $(OBJ) $(OBJDIR)/test_$(name).o -o test
	./test
	rm ./test
