CFLAGS = -Wall -Werror -g3
OBJ = main.o logging.o message_list.o list.o smtp_fsm.o programm_manager.o read_message.o message.o envelope.o maildir.o smtp.o

%.o: %.c *.h
	gcc -c $< $(CFLAGS)


client: $(OBJ)
	gcc -L/usr/lib/ -o $@ $^ -lfiredns -lrt

clean:
	rm client $(OBJ)

.PHONY: clean

