#ifndef BIRCD_H_
# define BIRCD_H_

# include <sys/select.h>

# define FD_FREE	0
# define FD_SERV	1
# define FD_CLIENT	2

# define BUF_SIZE	4096

# define Xv(err,res,str)	(x_void(err,res,str,__FILE__,__LINE__))
# define X(err,res,str)		(x_int(err,res,str,__FILE__,__LINE__))
# define MAX(a,b)	((a > b) ? a : b)

# define USAGE		"Usage: %s port\n"

typedef struct	s_fd
{
  int	type;
  void	(*fct_read)(); // = client read
  void	(*fct_write)(); // = client write
  char	buf_read[BUF_SIZE + 1];
  char	buf_write[BUF_SIZE + 1];
}		t_fd;

typedef struct	s_env
{
  t_fd		*fds; // malloc * maxfd
  int		port; // the port to lissen
  int		maxfd; // set resource soft limits
  int		max;
  int		r; // select returned 
  fd_set	fd_read; // a select functionaliteid
  fd_set	fd_write;
}		t_env;

// prepare's fd's in env struct
void	init_env(t_env *e);// 127.0.0.1
// gets the port from argv
void	get_opt(t_env *e, int ac, char **av);
// loops and uses init_fd do_select and check_fd
void	main_loop(t_env *e);
// makes the server and first client 
void	srv_create(t_env *e, int port);
// makes and adds the client to fd struct
void	srv_accept(t_env *e, int s);
void	client_read(t_env *e, int cs);
void	client_write(t_env *e, int cs);
// null all t_fd
void	clean_fd(t_fd *fd);
// checks err == res true > gives error message and exits
int	  x_int(int err, int res, char *str, char *file, int line);
// checks err == res true > gives error message and exits
void	*x_void(void *err, void *res, char *str, char *file, int line);
//-----main loop-----

// looks if a fds write holds something and set fd_read/write
void	init_fd(t_env *e);
// ueses select and returns to env-r
void	do_select(t_env *e);
// ?? check the fd struct and uses the fucntions
void	check_fd(t_env *e);

#endif /* !BIRCD_H_ */
