NAME = trafficmgr
SRC = trafficmgr.c monitor.c q.c
OBJ = $(SRC:.c=.o)
RM = rm -f
TARBALL = nmeni-$(NAME).tar.gz

all: $(OBJ)
	gcc $(OBJ) -o $(NAME) -lpthread

clean:
	-$(RM) *.o
	-$(RM) *.swp
	-$(RM) *.c~
	-$(RM) *.h~
	-$(RM) \#*
	-$(RM) *.core
	-$(RM) Makefile~

fclean: clean
	-$(RM) $(NAME)

re: fclean all

submit: fclean
	tar cvzf $(TARBALL) *
