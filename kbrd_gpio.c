#include "kbrd_gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>

#define PRDWR (PROT_READ|PROT_WRITE)

unsigned int gpio_add[] = {GPIO0_ADD, GPIO1_ADD, GPIO2_ADD, GPIO3_ADD};

int memb;
void *gpio_ptr[4];
void *wkup;

char row_gpio[] = {2, 2, 1, 0, 1, 2};
char col_gpio[] = {2, 2, 1, 0, 1, 0, 0, 1, 1, 
				1, 1, 1, 1, 1, 2, 0, 0};

unsigned int row_bit[] = {1<<3, 1<<4, 1<<12, 1<<26, 1<<14, 1<<1};
unsigned int col_bit[] = {1<<2, 1<<5, 1<<13, 1<<23, 1<<15, 1<<27, 
						1<<22, 1<<30, 1<<4, 1<<0, 1<<29, 1<<1,
						1<<5, 1<<31, 1<<25, 1<<11, 1<<10};



int init_kbrd(void)
{
	int i;
	
	if(memb)
	{
		printf("memb already mapped\n");
		return -1;
	}
	memb = open("/dev/mem", O_RDWR);

	for(i = 0; i < 4; i++)
	{
		gpio_ptr[i] = mmap(0, GPIO_SIZE, PRDWR, MAP_SHARED, memb, gpio_add[i]);
		if(gpio_ptr[i] == MAP_FAILED)
		{
			printf("Failed to map GPIO %d\n", i);
			return -1;
		}
	}
	wkup = mmap(0, WKUP_SIZE, PRDWR, MAP_SHARED, memb, WKUP_ADD);
	if(enable_gpio())
	{
		printf("Unable to enable GPIO registers\n");
		return -1;
	}
	if(enable_kbrd_dir())
	{
		printf("Unable to set GPIO direction\n");
		return -1;
	}
	return 0;
}

void clr_gpio()
{
	void *gp;
	unsigned int i;
	unsigned int *val;
	for(i = 0; i < 4; i++)
	{
		gp = gpio_ptr[i];
		val = (unsigned int *) (gp + GPIO_OUTE);
		*val = 0;
		val = (unsigned int *) (gp + GPIO_CLRDATOUT);
		*val = ~0;
	}
}

int free_kbrd(void)
{
	if(memb)
	{
		close(memb);
	}
	return 0;
}

void set_col(char c)
{
	if(c < 0 || c > 16)
		return;
	void *gp = gpio_ptr[col_gpio[c]];
	unsigned int *val = (unsigned int *) (gp + GPIO_SETDATOUT);
	*val |= col_bit[c];
}

void clr_col(char c)
{
	if(c < 0 || c > 16)
		return;
	void *gp = gpio_ptr[col_gpio[c]];	
	unsigned int *val = (unsigned int *) (gp + GPIO_CLRDATOUT);
	*val |= col_bit[c];
}

void pdata()
{
	void *gp;
	unsigned int *val;
	unsigned int i;
	for(i = 0; i < 4; i++)
	{
		gp = gpio_ptr[i];
		val = (unsigned int *) (gp + GPIO_DATIN);
		printf("%u : ", *val);
		val = (unsigned int *) (gp + GPIO_DATOUT);
		printf("%u\n", *val);
	}
	printf("\n");
}

unsigned char get_rows()
{
	void *gp;
	unsigned char a, r = 0;
	for(a = 0; a < 6; a++)
	{
		gp = gpio_ptr[row_gpio[a]];
		if(*(unsigned int *) (gp + GPIO_DATIN) & row_bit[a])
			r |= 1<<a;
	}
	return r;
}

int enable_gpio()
{
	if(!memb)
	{
		return -1;
	}
	unsigned int *val;
	val = (unsigned int *) (wkup + GPIO0_WKUP);
	*val |= 0x2;
	val = (unsigned int *) (wkup + GPIO1_WKUP);
	*val |= 0x2;
	val = (unsigned int *) (wkup + GPIO2_WKUP);
	*val |= 0x2;
	val = (unsigned int *) (wkup + GPIO3_WKUP);
	*val |= 0x2;
	return 0;
}

int enable_kbrd_dir()
{
	if(!memb)
	{
		return -1;
	}
	void *gp;
	unsigned int *val;
	unsigned int i;
	for(i = 0; i < 17; i++)
	{
		gp = gpio_ptr[col_gpio[i]];
		val = (unsigned int *) (gp + GPIO_OUTE);
		*val &= ~col_bit[i];
	}
	for(i = 5; i > -1; i++)
    {
        gp = gpio_ptr[row_gpio[i]];
        val = (unsigned int *) (gp + GPIO_OUTE);
        *val |= row_bit[i];
    }
	return 0;
}
