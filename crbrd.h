#ifndef CRBRD_H
#define CRBRD_H


#define	CTRL 1
#define	SHFT 2
#define	ALT 4
#define	CPLK 8
#define	SUPR 16
#define	MENU 32

typedef struct brd
{
	unsigned char s;
	unsigned char key[6][17];
	//keyt key[6][17];
} brd;

void init_crbrd();
void scan_crbrd();
void p_keys();

#endif
