/*------------------------------------------------------------------------
  Eurotech SpA 
  http://www.eurotech.com
  email:  support-it@eurotech.com
--------------------------------------------------------------------------
  Filename:      apc_defines.cpp
--------------------------------------------------------------------------
  Project:       <PCN-1001>
  Description:   <socket image client>
--------------------------------------------------------------------------
  Compiler:      Borland C++ Builder 6 Pro
-------------------------------------------------------------------------*/
#define NOMINMAX
#define PORT 5400
#define UDP_PORT 5402
#define MAX_STR_LENGTH 256
#define SCAN_ATTEMPTS  10000
#define NX      160
#define NY      120
#define NN      NX*NY

#define MODE_START                      0x0000
#define MODE_STOP                       0x1000
#define MODE_FPN                        0x0084
#define MODE_FPN_ODC                    0x1088
#define MODE_DISP_FPN_ODC               0x2001
#define MODE_DISP_MEDIAN_FPN_ODC        0x3004
#define MODE_DISP_MEDIAN_FPN_ODC_TRACK  0x4104
#define MODE_DISP_FPN_ODC_SOBEL         0x2002
#define MODE_TABLE_SIZE                 6

#define B300    0x7
#define B600    0x8
#define B1200   0x9
#define B2400   0xb
#define B4800   0xc
#define B9600   0xd
#define B19200  0xe
#define B38400  0xf
#define B57600  0x1001
#define B115200 0x1002
#define B230400 0x1003
#define B460800 0x1004
#define B921600 0x1007

#define NONE    0
#define EVEN    0x100
#define ODD     0x200

#define ONEBIT  0
#define TWOBITS 0x40


#define CS7     0x20
#define CS8     0x30

#define NUM_STEP 16
#define STEP0_225 12
#define STEP0_240 29
