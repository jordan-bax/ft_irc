
#include "bircd.h"

int	main(int ac, char **av)
{
  t_env	e;

  init_env(&e);// get limit
  get_opt(&e, ac, av); // check arg number and get port
  srv_create(&e, e.port); //make server
  main_loop(&e);// main loop

  return (0);
}

