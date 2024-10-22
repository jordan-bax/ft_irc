
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "bircd.h"
#include <string.h>

static void	*ft_memcpy(void *dest, const void *stc, size_t n)
{
	unsigned int	count;

	count = 0;
	if (!dest && !stc)
	{
		return (dest);
	}
	while (count < n)
	{
		((char *)dest)[count] = ((char *)stc)[count];
		count++;
	}
	return (dest);
}

void			srv_accept(t_env *e, int s)
{
  int			cs;
  struct sockaddr_in	csin;
  socklen_t		csin_len;

  csin_len = sizeof(csin);
  cs = X(-1, accept(s, (struct sockaddr*)&csin, &csin_len), "accept");
  printf("New client #%d from %s:%d\n", cs,
	 inet_ntoa(csin.sin_addr), ntohs(csin.sin_port));
  clean_fd(&e->fds[cs]);
  e->fds[cs].type = FD_CLIENT;
  e->fds[cs].fct_read = client_read;
  e->fds[cs].fct_write = client_write;
  e->fds[cs].user = new User_data(cs);
  ft_memcpy(e->fds[cs].buf_write, WELCOME,strlen(WELCOME));
}
