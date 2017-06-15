
#ifndef STEUERUNG_H
#define STEUERUNG_H

#include "os_support.h" 

#ifdef  __cplusplus
extern "C" {
#endif


// An- Aus-Register
#define ESTG		(STEUERUNG_BASEADDRESS + 0x10)		
#define	ESTG0		0			
#define ESTG1		1
#define ESTG2		2
#define ESTG3		3
#define ESTG4		4
#define ESTG5		5
#define ESTG6		6
#define ESTG7		7
#define ESTG8		8
#define ESTG9		9
#define ESTG10		10
#define ESTG11		11
#define ESTG12		12
#define ESTG13		13
#define ESTG14		14
#define ESTG15		15


extern unsigned char Flag_das_das_Steuerungmodul_aktiviert;	// Aktiviert (bei Wert 1)  die Steuerung (insbesondere auch die Moeglichkeit das System einzifrieren FreezeSystem
																			// also das Ein- und Ausschalten der Simulationen der Module und insbesondere auch das FreezeSystem 
																			// von der UI aus.
																			// Wenn man ohne UI arbeitet (was selte nist) dann muss dieses Flag auf 0 gesetzt werden
																			// ansonsten geht das System in einen Freeze und es finden FreeRTOS-Schedulings nicht mehr statt
																			// Ein vTaskDelay(10) kommt dann nie zurück.

/**
*	Mutexe um einen gleichzeitigen Zugriff der unterschiedlichen Threads auf die globalen Variablen zu verhindern.
*	Je Device ein Mutex. 
*/
extern void* steuerung_cntr_Mutex; 

#define __steuerung_cntr_Mutex_lock		LockMutexOs(steuerung_cntr_Mutex) 
#define __steuerung_cntr_Mutex_release	UnlockMutexOs(steuerung_cntr_Mutex)


/**
*	Struktur für die exemplarischen Interrupt Controller Register
*/
typedef struct
{
    unsigned short ESTG_str;		//Steuerregister fuer An- Aus der einzelnen Devices
}STRC_STEUERUNG;


/*!
*	Kontroll-Flags zur Aenderungsmitteilung zwischen dem Simulations-Thread (Simulations-Funktion) und dem Thread fuer die 
*	Shared-Memory-Kommunikation (also ob sich in einer Komponente ein Wert geaendert hat).
*/
extern unsigned char ctr_steuerung_flag;

// 2a.)
// Definition mit welchem Globalen-Shared-Memory dieses Device arbeitet
// Die Definition erfolgt ueber den zu verwendenden Index fuer das Export-Zeigers-Feld (siehe device_interface.c  C.))
#define STEUERUNG_EXPORTZEIGERFELD_INDEX	EXPORTZEIGERFELD_INDEX_FUER_GLOBAL_SHARED_MEMORY_0 // also "SEEM"


unsigned short			in_steuerung(unsigned short registerAdr);
void					out_steuerung(unsigned short registerAdr, unsigned short value);
void					out8_steuerung(unsigned short registerAdr, unsigned char value);
extern void				Steuerung_Simulation();
extern void				sm_get_SteuerungReg(unsigned short eigeneProzessNummer);
extern unsigned char	sm_set_SteuerungReg(unsigned short eigeneProzessNummer);
extern unsigned char	init_steuerung();



//Makros fuer den Zugriff aufs Lokale-Shared-Memory
void setNewESTG(unsigned short value);	//Hilfsfunktion in macro.c um automatische Einstellungen zu uebernehmen
#define __ESTG_out(value)	setNewESTG((unsigned short) value)
#define __ESTG_in			local_Shared_Mem_Copy.steuerung_shared.ESTG_str


//Makros fuer den Zugriff auf interne Kontroll-Flags
#define	__ctr_steuerung_flag_out(value)	ctr_steuerung_flag = (unsigned char)value
#define	__ctr_steuerung_flag_in			ctr_steuerung_flag


extern void sm_get_SteuerungReg(unsigned short eigeneProzessNummer);
extern unsigned char sm_setSteuerungReg(unsigned short eigeneProzessNummer);

extern unsigned char init_steuerung();

#ifdef  __cplusplus
}
#endif

#endif //STEUERUNG
