
#ifndef PATTERN_H
#define PATTERN_H

#include "os_support.h" 

#ifdef  __cplusplus
extern "C" {
#endif

// 1.)
// Definition der Makros mit denen die Studierenden auf die Register des Devices und die Register-Bits zugreifen koennen
// Die Adresse der Register berechnet sich aus einen Basis-Adresse des Devices und eines direkt angegeben Offsets (im Falle von PATTERN_REG_A mit dem Wert 0x0010).
// Die Basisadressen der verschiedenen Devices duerfen nicht kollidieren.
// Deshalb werden diese Basis-Adressen an einer zentralen Stelle (in device_interface.h) vorgenommen. 
//
// Der Adress-Abstand zwischen den Registern muss so gross (oder groesser) sein als die Registerbreite, denn 
// wir wollen einen "echten" IO-Adressraum simulieren der auf Basis von BYTE-Adressen (also Adressen die Bytes adressieren) aufgebaut ist.
// Hierzu muss man dann die folgende Konvention einhalten:
//		1.) Die Adressen von Registern sind immer gerade (ohne Rest durch zwei teilbar), auch dann wenn es nur ein 8-Bit-Register ist.
//		2.) Mit in() und out() greift man auf 16 Bit also auf zwei aufeinander folgende Byte-Adressen zu.
//		3.) Die Funktionen in(addr) und out(addr) funktionieren nur bei geraden addr, ansonsten wird 0 zurueck gegeben.
//		3.) Mit in8(addr) liest man im Falle einer GERADEN addr die 16 Bit (2 Byte) ab addr und liefert davon das niederwertigte Byte 
//			also (wegen little endian) das Byte an der niederwertigsten Adresse zurück
//		4.) Mit in8(addr) liest man im Falle einer UNGERADEN addr die 16 Bit (2 Byte) ab addr-1 und liefert davon das hoeherwertige Byte 
//			also (wegen little endian) das Byte an der hoeherwertigsten Adresse zurück.

//Register A
#define PATTERN_REG_A		(PATTERN_BASEADDRESS + 0x10)		
#define	PATTERN_REG_A0		0			
#define PATTERN_REG_A1		1
#define PATTERN_REG_A2		2
#define PATTERN_REG_A3		3
#define PATTERN_REG_A4		4
#define PATTERN_REG_A5		5
#define PATTERN_REG_A6		6
#define PATTERN_REG_A7		7
#define PATTERN_REG_A8		8
#define PATTERN_REG_A9		9
#define PATTERN_REG_A10		10
#define PATTERN_REG_A11		11
#define PATTERN_REG_A12		12
#define PATTERN_REG_A13		13
#define PATTERN_REG_A14		14
#define PATTERN_REG_A15		15


// 2a.)
// Definition mit welchem Globalen-Shared-Memory(-Segemnt) dieses Device arbeitet.
// Die Definition erfolgt ueber den zu verwendenden Index fuer das Export-Zeigers-Feld 
// Details zum zu Grunde liegenden Konzept finden sich in device_interface.c  // C.) 
// Das Wichtigste ist wie folgt:
//			PATTERN_EXPORTZEIGERFELD_INDEX  wird innerhalb des ganzen pattern.c verwendet
//			EXPORTZEIGERFELD_INDEX_FUER_GLOBAL_SHARED_MEMORY_0	ist in device.h definiert
//			EXPORTZEIGERFELD_INDEX_FUER_GLOBAL_SHARED_MEMORY_0	wird dem Konstruktor fuer das Globel-Shared-Memory uebergeben (siehe main.cpp)
//			EXPORTZEIGERFELD_INDEX_FUER_GLOBAL_SHARED_MEMORY_0	gehoert zu  NAME_GLOBAL_SHARED_MEMORY_0	 (was in device_interface.h als "SEEM"
//																definiert ist. "SEEM" ist der Name des Global-Shared-Memroy-(Segmentes)
// GPIO_PORT1 arbeit also mit "SEEM"
#define PATTERN_EXPORTZEIGERFELD_INDEX	EXPORTZEIGERFELD_INDEX_FUER_GLOBAL_SHARED_MEMORY_0 // also "SEEM"



// 2b.)
// Definition der Device-Struktur die als Teil der Struktur fuer das Lokale-Shared-Memory und das Globale-Shared-Memory
// verwendet wird.
typedef struct
{
    unsigned short PATTERN_REG_A_str;		//Hat nur ein Register
}STRC_PATTERN;



// 3a.)
// Export von Funktionen, da diese in der Datei device_interface.c in die "Zeiger-Felder" eingetragen werden, ...
// ... bzw (im Falle von init_PATTERN()) in der Funktion reset_simulation_function() aufgerufen werden
unsigned short			in_PATTERN(unsigned short registerAdr);
void					out_PATTERN(unsigned short registerAdr, unsigned short value);
void					out8_PATTERN(unsigned short registerAdr, unsigned char value);
extern void				PATTERN_Simulation();
extern void				sm_get_PATTERNReg(unsigned short eigeneProzessNummer);
extern unsigned char	sm_set_PATTERNReg(unsigned short eigeneProzessNummer);
extern unsigned char	init_PATTERN();



// 3b.) Export weiterer Flags
extern unsigned char PATTERN_An_Aus;



// 4.)
// Export weiterer (eigentlich) "Dinge", die vornehmlich bei der UI extern benoetigt werden.
void setNewPATTERN_REG_A(unsigned short value);	//Hilfsfunktion in macro.c um automatische Einstellungen zu uebernehmen
#define __PATTERN_REG_A_out(value)	setNewPATTERN_REG_A((unsigned short) value)
#define __PATTERN_REG_A_in			local_Shared_Mem_Copy.PATTERN_shared.PATTERN_REG_A_str

extern void* PATTERN_cntr_Mutex; 
#define __PATTERN_cntr_Mutex_lock		LockMutexOs(PATTERN_cntr_Mutex) 
#define __PATTERN_cntr_Mutex_release	UnlockMutexOs(PATTERN_cntr_Mutex) 

extern unsigned char ctr_PATTERN_flag;
#define	__ctr_PATTERN_flag_out(value)	ctr_PATTERN_flag = (unsigned char)value
#define	__ctr_PATTERN_flag_in			ctr_PATTERN_flag


#ifdef  __cplusplus
}
#endif

#endif //PATTERN
