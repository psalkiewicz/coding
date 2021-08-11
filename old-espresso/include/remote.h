#define MYPORT 3924    /* the port users will be sending to */
#define CODE1  0x66
#define CODE2  0x19

#define REM_DISABLE	0x10
#define REM_POWEROFF	0x11
#define REM_REBOOT	0x12

int rem_control(long ip,char action);
