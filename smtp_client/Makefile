CFSM_DIR = cfsm_lib
CFLAGS = -Wall -Werror -g3
OBJ = main.o logging.o message_list.o list.o smtp_fsm.o programm_manager.o read_message.o message.o envelope.o maildir.o smtp.o fsm.o run.o

client: $(OBJ)
	gcc -L/usr/lib/ -o $@ $^ -lrt -lconfig

fsm.o: fsm.c
	gcc -c fsm.c -o fsm.o

fsm.c: fsm.fsm
	cd $(CFSM_DIR) && make --silent
	$(CFSM_DIR)/cfsm/cfsm -t $(CFSM_DIR)/cfsm/ -d fsm.fsm -o fsm.c
	$(CFSM_DIR)/cfsm/cfsm -t $(CFSM_DIR)/cfsm/ -g fsm.fsm -o fsm.dot

%.o: %.c *.h
	gcc -c $< $(CFLAGS)

clean:
	rm -f client $(OBJ)
	cd $(CFSM_DIR) && make clean --silent
	rm -f fsm.c fsm.h fsm.dot

.PHONY: clean
