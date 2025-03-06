#pragma once

extern bool g_saveQuit;
// inisiate the signal quit and interupt to set saveQuit to false
// so we can savely quit and close all the fds in the main loop that has the data
void init_signalHandling();

// void	clean_client(t_fd *fd);
// void	server_close(t_env *e);