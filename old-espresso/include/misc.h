
#define W_RETURN		13
#define W_ESCAPE		27
#define W_DOL			258
#define W_GORA			259
#define W_LEWO			260
#define W_PRAWO			261
#define W_BACKSPACE		263

/* Lista jednokierunkowa - lista komputerow w cafe */
struct k {
    char name[20];
    unsigned long ip;
    int tariff;
    
    struct k *n;
};

/* Strukturka do okreslania pozycji */ 
struct pos {
	int x;
	int y;
};
