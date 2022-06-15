SHELL = /bin/sh
.PHONY: all clean fclean re
CC = gcc
FLAGS = -c -Wall -Werror -Wextra
GOAL = tlv_application_parser
ADVICE = hex_to_binary

NAMES = appUtils parseDir tlv
OBJS = $(addsuffix .o, $(NAMES))
HDRS = $(addsuffix .h, $(NAMES))

OBJSDIR = objs
vpath %.h hdrs toBinary
vpath %.c srcs toBinary

all: $(GOAL) $(ADVICE)
	bash ./test.sh

$(GOAL): main.o $(OBJS) | $(HDRS)
	$(CC) $(addprefix ./$(OBJSDIR)/, $(OBJS) $<)  -o $@

$(ADVICE): hex_to_binary.o | hex_to_binary.h
	$(CC) $(addprefix ./$(OBJSDIR)/, $<) -o $@

main.o: main.c | $(OBJSDIR)
	$(CC) $(FLAGS) $< -Ihdrs -o ./$(OBJSDIR)/$@

%.o: %.c %.h | $(OBJSDIR)
	$(CC) $(FLAGS) $< -Ihdrs -ItoBinary -o ./$(OBJSDIR)/$@

$(OBJSDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJSDIR)

fclean: clean
	rm $(GOAL) $(ADVICE)

re: fclean all