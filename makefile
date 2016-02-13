CC = gcc
CFLAGS = -ggdb -DDEBUG -I./ -I/usr/include/csound/ -I/usr/local/include/csound/ -lm -lcsound64
DEPS = vocli.h cmu.h cmds.h voice_types.h voice_edit.h voice_cmds.h sound.h mystring.h arpabet.h
OBJ = main.o vocli.o cmu.o cmds.o voice_edit.o voice_cmds.o sound.o mystring.o arpabet.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

vocli: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean: $(OBJ)
	\rm $^
