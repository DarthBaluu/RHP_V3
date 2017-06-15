#ifndef IO_TREIBER_H
#define IO_TREIBER_H

/* --- Typedefs --- */

typedef unsigned char BYTE;
typedef struct BHandle_Data BHandle;
typedef BHandle* DSCB;


/* --- Global Variables --- */

extern DSCB GlobalBoardHandle;

/* --- Macros --- */

#define PA 0
#define PB 1
#define PC 2
#define PD 3

/* --- Function Headers --- */

BYTE Init(DSCB BoardHandle, unsigned long int Steuerwort);

BYTE InputByte(DSCB BoardHandle, BYTE Port, BYTE *DigitalValue);

BYTE OutputByte(DSCB BoardHandle, BYTE Port, BYTE DigialValue);

BYTE Free(DSCB BoardHandle);

#endif
