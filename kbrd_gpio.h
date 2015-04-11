#ifndef KBRD_GPIO_H
#define KBRD_GPIO_H

int init_kbrd(void);
int free_kbrd(void);
int enable_kbrd_dir(void);
int enable_gpio(void);
void set_col(char);
void clr_col(char);
unsigned char get_rows();
void clr_gpio();
void pdata();
void shit();

#define GPIO_SIZE	0x1000
#define GPIO0_ADD	0x44e07000
#define GPIO1_ADD	0x4804c000
#define GPIO2_ADD	0x481ac000
#define GPIO3_ADD	0x481ae000

#define WKUP_SIZE	0x1000
#define WKUP_ADD	0x44e00000
#define GPIO0_WKUP	0x408
#define GPIO1_WKUP	0x0ac
#define GPIO2_WKUP	0x0b0
#define GPIO3_WKUP	0x0b4

#define GPIO_OUTE		0x134
#define GPIO_DATIN		0x138
#define GPIO_DATOUT		0x13c
#define GPIO_SETDATOUT	0x194
#define GPIO_CLRDATOUT	0x190

#endif
