#define CM_ESCP		0
#define CM_SALE		1
#define CM_INFO		2
#define CM_POFF		3
#define CM_REBT		4
#define CM_STAT		5

struct pos position(int d,int ax);
int c_menu(int ogr);
void quit_prompt(int ogr);
void clean_c_menu(int ogr);
