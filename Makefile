
VPATH= HEADER:SRC

NAME= irc_server

CLASSES+= connection client server term_reader env error_log rock_bot



SRC= client_input.cpp client_output.cpp client_commands.cpp \
	Messages.cpp channel.cpp

SRC+= main.cpp #x.cpp \
	# init_env.cpp clean_fd.cpp get_opt.cpp main_loop.cpp \
	# init_fd.cpp do_select.cpp check_fd.cpp \
	# srv_create.cpp srv_accept.cpp client_read.cpp client_write.cpp 
SRC+= User_data.cpp signals.cpp 
SRC+= $(CLASSES:%=%.cpp)

OBF_DIR= OBF

OBF= $(SRC:%.cpp=$(OBF_DIR)/%.o)

HEADER= bircd.h User_data.hpp signal.hpp
HEADER= $(CLASSES:%=%.hpp)
CC= c++

val = -ggdb3
VALOPT = --leak-check=full
# VALOPT += --show-leak-kinds=all
VALOPT += -s
CPPFLAGS +=	-Wall -Werror -Wextra
CPPFLAGS += -g $(val)
PORT= 8000
# CPPFLAGS +=	-std=c++98

SAN= -fsanitize=address

OO= -O3

all: $(NAME)

$(NAME): $(OBF_DIR) $(OBF)
	$(CC) -g $(CFLAGS) -o $@ $(OBF) $(RLINE) 

$(OBF_DIR)/%o: %cpp $(HEADER)
	$(CC) -g -c $(CFLAGS) -o $@ $< 

$(OBF_DIR):
	mkdir $(OBF_DIR)

clean:
	@rm -rf $(OBF) $(OBF_DIR)

fclean:
	@rm -rf $(OBF) $(OBF_DIR) $(NAME)

f: fclean
 
v:$(NAME)
	valgrind $(VALOPT) ./$(NAME) $(PORT)

r:$(NAME)
	./$(NAME) $(PORT)
re:
	@$(MAKE) fclean
	@$(MAKE) all

.PHONY: all re fclean clean f r v
