#include "stdinc.h"

services_info_t services_info;

int main(int argc, char *argv[])
{
  memset(&services_info, 0, sizeof(services_info));

  libio_init(FALSE);

  read_services_conf(TRUE);

  printf("Services starting with name %s description %s sid %s\n",
      services_info.name, services_info.description, services_info.sid);

  init_db();

  for(;;)
      ;

  return 0;
}
