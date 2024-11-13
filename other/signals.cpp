#include <iostream>
#include <csignal>
#include <sys/socket.h>
#include "signal.hpp"
bool g_saveQuit = true;

// send end of file to client and add stdin to the fd_set

// Signal handler function
void signalHandler(int signum) {
    if (signum == SIGINT) {
        std::cout << "Interrupt signal (SIGINT) received." << std::endl;
		g_saveQuit = false;
    }
	else if (signum == SIGQUIT) {
        std::cout << "Quit signal (SIGQUIT) received." << std::endl;
		g_saveQuit = false;
    }
    // Exit program if needed
    // std::exit(signum); // Uncomment to exit on signal
}

// Function to initialize signal handling for SIGINT and SIGQUIT
void init_signalHandling() {
    // Set up signal handling for SIGINT (Interrupt from keyboard, Ctrl+C)
    if (std::signal(SIGINT, signalHandler) == SIG_ERR) {
        std::cerr << "Error setting up SIGINT handler" << std::endl;
    }
    // Set up signal handling for SIGQUIT (Quit from keyboard, Ctrl+\)
    if (std::signal(SIGQUIT, signalHandler) == SIG_ERR) {
        std::cerr << "Error setting up SIGQUIT handler" << std::endl;
    }
}

void	server_close(t_env *e)
{
	int	i;
	int	srv_fd;

	i = 0;
	e->max = 0;
	while (i < e->maxfd)
	{
		if (e->fds[i].type == FD_SERV)
			srv_fd = i;
		if (e->fds[i].type == FD_CLIENT)
		{
			std::string bye(FG_RED"server is closing\n");
			send(i, bye.c_str(), bye.size(), 0);
			send(i, "\0", 1, 0);
			clean_client(&e->fds[i]);
			close(i);
			e->max = MAX(e->max, i);
		}
		i++;
	}
	clean_fd(&e->fds[srv_fd]);
	close(srv_fd);
	free(e->fds);
	exit(0);
}

void	clean_client(t_fd *fd)
{
	if (fd->user)
		delete fd->user;
	clean_fd(fd);
}