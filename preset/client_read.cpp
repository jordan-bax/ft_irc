#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include "bircd.h"


// int	ft_memcmp(const void *s1, const void *s2, size_t n)
// {
// 	unsigned int	count;

// 	count = 0;
// 	while (count < n && (*(unsigned char *)s1) == (*(unsigned char *)s2))
// 	{
// 		s1++;
// 		s2++;
// 		count++;
// 	}
// 	if (count >= n)
// 		return (0);
// 	return ((*(unsigned char *)s1) - (*(unsigned char *)s2));
// }

static bool commands(t_env *e, int cs){
	t_fd client = e->fds[cs];
	if (strlen(client.buf_read) > 6 && !memcmp(e->fds[cs].buf_read, "LOGIN ", 6))
	{
		std::string temp(client.buf_read+6);
		client.user->set_fullname(temp);
		client.user->set_nickname(temp);
		return false;
	}
	return true;
}

void	client_read(t_env *e, int cs)
{
  int	r;
  int	i;

  r = recv(cs, e->fds[cs].buf_read, BUF_SIZE, 0);
  if (r <= 0)
	{
	  close(cs);
	  clean_fd(&e->fds[cs]);
	  printf("client #%d gone away\n", cs);
	}
  else if (commands(e,cs))
	{
	  i = 0;
	  while (i < e->maxfd)
		{
		if ((e->fds[i].type == FD_CLIENT) && (i != cs))
			send(i, e->fds[cs].user->massege(e->fds[cs].buf_read).c_str(),e->fds[cs].user->massege(e->fds[cs].buf_read).size() , 0);
		i++;
		}
	}
}
