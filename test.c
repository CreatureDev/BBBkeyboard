#include "kbrd_gpio.h"
#include <stdio.h>
#include <unistd.h>
#include "crbrd.h"

int main(int argc, char **argv)
{
	unsigned int i;
	if(init_kbrd())
	{
		free_kbrd();
		return -1;
	}
	init_crbrd();
	while(1)
	{
		scan_crbrd();
		p_keys();
		usleep(100);
	}
	free_kbrd();
	return 0;
}
