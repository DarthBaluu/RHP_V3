#include "simuc.h"
#include "io_treiber.h"

typedef struct BHandle_Data {
	BYTE Board_allocated;	// 1=allocated, 0=free
	BYTE Port_A_Direction;	// 1=Output, 0=Input
	BYTE Port_B_Direction;
	BYTE Port_C_Direction;
	BYTE Port_D_Direction;
} BHandle;


BHandle BoardHandle_Data;
DSCB GlobalBoardHandle = &BoardHandle_Data;



BYTE Init(DSCB BoardHandle, unsigned long int Steuerwort) {

	// Mit Leben fuellen
	
	return 0;
}

BYTE InputByte(DSCB BoardHandle, BYTE Port, BYTE *DigitalValue) {
	
	// Mit Leben fuellen

	return 0;
}

BYTE OutputByte(DSCB BoardHandle, BYTE Port, BYTE DigitalValue) {
	
	// Mit Leben fuellen

	return 0;
}

BYTE Free(DSCB BoardHandle) {

	// Mit Leben fuellen
	
	return 0;
}

