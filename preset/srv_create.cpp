#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include "bircd.h"
#include "../classes/server.hpp"
#include "../classes/term_reader.hpp"

void			srv_create(t_env *e, int port)
{
		int opt=1;
		socklen_t optlen=sizeof(opt);
		int			s;
		struct sockaddr_in	sin;
		struct protoent	*pe;
		
		pe = (struct protoent*)Xv(NULL, getprotobyname("tcp"), "getprotobyname");
		s = X(-1, socket(PF_INET, SOCK_STREAM, pe->p_proto ), "socket");
		X( -1, setsockopt(s ,SOL_SOCKET, SO_REUSEADDR, &opt, optlen), "sockopt");//| SO_REUSEADDR
		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = INADDR_ANY;
		sin.sin_port = htons(port);
		X(-1, bind(s, (struct sockaddr*)&sin, sizeof(sin)), "bind");
		X(-1, listen(s, 42), "listen");
		// e->fds[s].type = FD_SERV;
		// e->fds[s].fct_read = srv_accept;
		// e->fds[0].fct_read = srv_read;
		e->connections.push_back(new term_reader(FD_IO, STDIN_FILENO));
		e->connections.push_back(new server(FD_SERV, s));
}
