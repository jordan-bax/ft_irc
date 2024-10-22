
#include "bircd.h"

void	main_loop(t_env *e)
{
  std::cout << "server is now listening to port "<< e->port<< std::endl;
  while (1)
    {
      init_fd(e);
      do_select(e);
      check_fd(e);
    }
}
