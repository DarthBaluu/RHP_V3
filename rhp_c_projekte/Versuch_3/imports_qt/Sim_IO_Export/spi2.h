
#ifndef SPI2_H
#define SPI2_H

#include "os_support.h" 

#ifdef  __cplusplus
extern "C" {
#endif

// 1.)
// Definition der Makros mit denen die Studierenden auf die Register des Devices und die Register-Bits zugreifen koennen
// Die Adresse der Register berechnet sich aus einen Basis-Adresse des Devices und eines direkt angegeben Offsets (im Falle von SPI2_CONF_REG mit dem Wert 0x0010).
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

//Configuration Register 
#define SPI2_CONF_REG		(SPI2_BASEADDRESS + 0)		
#define	SPI2_CONF_REG0			0			
#define SPI2_CONF_REG1			1
#define SPI2_CONF_REG2			2
#define SPI2_CONF_REG3			3
#define SPI2_CONF_REG4			4
#define SPI2_CONF_REG5			5
#define SPI2_CONF_REG6			6
#define SPI2_CONF_REG7			7


//Data Register 
#define SPI2_DATA_REG		(SPI2_BASEADDRESS + 2)		
#define	SPI2_DATA_REG0		0			
#define SPI2_DATA_REG1		1
#define SPI2_DATA_REG2		2
#define SPI2_DATA_REG3		3
#define SPI2_DATA_REG4		4
#define SPI2_DATA_REG5		5
#define SPI2_DATA_REG6		6
#define SPI2_DATA_REG7		7

//Status Register 
#define SPI2_STAT_REG		(SPI2_BASEADDRESS + 4)		
#define	SPI2_STAT_REG0		0			
#define SPI2_STAT_REG1		1
#define SPI2_STAT_REG2		2
#define SPI2_STAT_REG3		3
#define SPI2_STAT_REG4		4
#define SPI2_STAT_REG5		5
#define SPI2_STAT_REG6		6
#define SPI2_STAT_REG7		7

// 2a.)
// Definition mit welchem Globalen-Shared-Memory(-Segemnt) dieses Device arbeitet.
// Die Definition erfolgt ueber den zu verwendenden Index fuer das Export-Zeigers-Feld 
// Details zum zu Grunde liegenden Konzept finden sich in device_interface.c  // C.) 
// Das Wichtigste ist wie folgt:
//			SPI2_EXPORTZEIGERFELD_INDEX  wird innerhalb des ganzen SPI2.c verwendet
//			EXPORTZEIGERFELD_INDEX_FUER_GLOBAL_SHARED_MEMORY_0	ist in device.h definiert
//			EXPORTZEIGERFELD_INDEX_FUER_GLOBAL_SHARED_MEMORY_0	wird dem Konstruktor fuer das Globel-Shared-Memory uebergeben (siehe main.cpp)
//			EXPORTZEIGERFELD_INDEX_FUER_GLOBAL_SHARED_MEMORY_0	gehoert zu  NAME_GLOBAL_SHARED_MEMORY_0	 (was in device_interface.h als "SEEM"
//																definiert ist. "SEEM" ist der Name des Global-Shared-Memroy-(Segmentes)
// GPIO_PORT1 arbeit also mit "SEEM"
#define SPI2_EXPORTZEIGERFELD_INDEX	EXPORTZEIGERFELD_INDEX_FUER_GLOBAL_SHARED_MEMORY_0 // also "SEEM"



// 2b.)
// Definition der Device-Struktur die als Teil der Struktur fuer das Lokale-Shared-Memory und das Globale-Shared-Memory
// verwendet wird.
typedef struct
{
    unsigned short SPI2_CONF_REG_str;		
	unsigned short SPI2_DATA_REG_str;
	unsigned short SPI2_STAT_REG_str;
}STRC_SPI2;



// 3a.)
// Export von Funktionen, da diese in der Datei device_interface.c in die "Zeiger-Felder" eingetragen werden, ...
// ... bzw (im Falle von init_SPI2()) in der Funktion reset_simulation_function() aufgerufen werden
unsigned short			in_SPI2(unsigned short registerAdr);
void					out_SPI2(unsigned short registerAdr, unsigned short value);
void					out8_SPI2(unsigned short registerAdr, unsigned char value);
extern void				SPI2_Simulation();
extern void				sm_get_SPI2Reg(unsigned short eigeneProzessNummer);
extern unsigned char	sm_set_SPI2Reg(unsigned short eigeneProzessNummer);
extern unsigned char	init_SPI2();



// 3b.) Export weiterer Flags
extern unsigned char SPI2_An_Aus;



// 4.)
// Export weiterer (eigentlich) "Dinge", die vornehmlich bei der UI extern benoetigt werden.
void setNewSPI2_CONF_REG(unsigned short value);	//Hilfsfunktion in macro.c um automatische Einstellungen zu uebernehmen
#define __SPI2_CONF_REG_out(value)	setNewSPI2_CONF_REG((unsigned short) value)
#define __SPI2_CONF_REG_in			local_Shared_Mem_Copy.SPI2_shared.SPI2_CONF_REG_str

void setNewSPI2_DATA_REG(unsigned short value);	//Hilfsfunktion in macro.c um automatische Einstellungen zu uebernehmen
#define __SPI2_DATA_REG_out(value)	setNewSPI2_DATA_REG((unsigned short) value)
#define __SPI2_DATA_REG_in			local_Shared_Mem_Copy.SPI2_shared.SPI2_DATA_REG_str

void setNewSPI2_STAT_REG(unsigned short value);	//Hilfsfunktion in macro.c um automatische Einstellungen zu uebernehmen
#define __SPI2_STAT_REG_out(value)	setNewSPI2_STAT_REG((unsigned short) value)
#define __SPI2_STAT_REG_in			local_Shared_Mem_Copy.SPI2_shared.SPI2_STAT_REG_str

extern void* SPI2_cntr_Mutex; 
#define __SPI2_cntr_Mutex_lock		LockMutexOs(SPI2_cntr_Mutex) 
#define __SPI2_cntr_Mutex_release	UnlockMutexOs(SPI2_cntr_Mutex) 

extern unsigned char ctr_SPI2_flag;
#define	__ctr_SPI2_flag_out(value)	ctr_SPI2_flag = (unsigned char)value
#define	__ctr_SPI2_flag_in			ctr_SPI2_flag

extern unsigned char spi2_data_transfer_aktiv_flag;


#ifdef  __cplusplus
}
#endif

#endif //SPI2
