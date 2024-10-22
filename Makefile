
VPATH= preset : classes 

NAME= irc_server

SRC= main.cpp init_env.cpp clean_fd.cpp get_opt.cpp x.cpp main_loop.cpp \
	init_fd.cpp do_select.cpp check_fd.cpp \
	srv_create.cpp srv_accept.cpp \
	client_read.cpp client_write.cpp

SRC+= User_data.cpp

OBF_DIR= OBF

OBF= $(SRC:%.cpp=$(OBF_DIR)/%.o)

HEADER= bircd.h User_data.hpp

CC= c++

val = -ggdb3
CPPFLAGS +=	-Wall -Werror -Wextra
CPPFLAGS += -g $(val)
CPPFLAGS +=	-std=c++98

SAN= -fsanitize=address

OO= -O3

all: $(NAME)

$(NAME): $(OBF_DIR) $(OBF)
	$(CC) $(CFLAGS) -o $@ $(OBF) $(RLINE) 

$(OBF_DIR)/%o: %cpp $(HEADER)
	$(CC) -c $(CFLAGS) -o $@ $< 

$(OBF_DIR):
	mkdir $(OBF_DIR)

clean:
	@rm -rf $(OBF) $(OBF_DIR)

fclean:
	@rm -rf $(OBF) $(OBF_DIR) $(NAME)

f: fclean

r:$(NAME)
	./$(NAME) 8080
re:
	@$(MAKE) fclean
	@$(MAKE) all


.PHONY: all re fclean clean f r