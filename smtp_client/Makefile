CFSM_DIR = cfsm_lib
UNIT_EXE = test_smtp
TEST_DIR = test
CFLAGS = -Wall -Werror -g3
OBJ = main.o logging.o message_list.o smtp_fsm.o programm_manager.o read_message.o message.o envelope.o maildir.o smtp.o  run.o main_proc.o child_proc.o

client: $(OBJ)
	gcc -L/usr/lib/ -o $@ $^ -lrt -lconfig -lresolv


%.o: %.c *.h
	gcc -c $< $(CFLAGS)
test_style: apply_style
	diff -u <(cat *) <(clang-format -style=file ./*)

apply_style:
	clang-format -i -style=file ./*

clean:
	rm -f client $(OBJ)
	cd $(CFSM_DIR) && make clean --silent

TEST_SRC = $(wildcard $(TEST_DIR)/*.c) read_message.c smtp.c message.c envelope.c smtp_fsm.c main_proc.c run.c maildir.c main_proc.c logging.c programm_manager.c child_proc.c

test_units: $(UNIT_EXE)
	./$(UNIT_EXE)

$(UNIT_EXE): $(TEST_SRC)
	$(CC) -I. $(CFLAGS) $^ -lcunit -lrt -o $@

test_memory:
	echo "MAKE MEMORY TESTS"

test_system:
	echo "MAKE SYSTEM TESTS"


tests: test_units test_memory test_system
	echo "MAKE ALL TESTS"


.PHONY: clean
