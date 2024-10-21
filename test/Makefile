NAME= program.exe

SRC= main.o 
SRC+= 

HEADERS= 
CC= c++

val = -ggdb3
CPPFLAGS +=	-Wall -Werror -Wextra
CPPFLAGS += -g $(val)
CPPFLAGS +=	-std=c++98

all:$(NAME)

$(NAME): $(SRC)
	$(CC) $(CPPFLAGS) -o $@ $^
%.o:%.cpp $(HEADERS)
	$(CC) -c $(CPPFLAGS) $< -o $@

clean:
	rm -rf $(SRC)
fclean:
	rm -rf $(SRC) $(NAME)

re:
	@$(MAKE) fclean --no-print-directory
	@$(MAKE) all --no-print-directory

vgrind: all
	valgrind --leak-check=full ./$(NAME)

r: all
	./$(NAME)

.PHONY: all clean fclean re r vgrind