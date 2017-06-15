
#ifndef GPIO_PORT0_H
#define GPIO_PORT0_H

#include "os_support.h" 

#ifdef  __cplusplus
extern "C" {
#endif

// 1.)
// Definition der Makros mit denen die Studierenden auf die Register des Devices und die Register-Bits zugreifen koennen
// Die Adresse der Register berechnet sich aus einen Basis-Adresse des Devices und eines direkt angegeben Offsets (im Falle von GPIO_PORT0_RegDIR mit dem Wert 0x0010).
// Die Basisadressen der verschiedenen Devices duerfen nicht kollidieren.
// Deshalb werden diese Basis-Adressen an einer zentralen Stelle (in device_interface.h) vorgenommen. 
//Register A
#define GPIO_PORT0_RegIN		(GPIO_PORT0_BASEADDRESS + 0)		
#define	GPIO_PORT0_RegIN0		0			
#define GPIO_PORT0_RegIN1		1
#define GPIO_PORT0_RegIN2		2
#define GPIO_PORT0_RegIN3		3
#define GPIO_PORT0_RegIN4		4
#define GPIO_PORT0_RegIN5		5
#define GPIO_PORT0_RegIN6		6
#define GPIO_PORT0_RegIN7		7
#define GPIO_PORT0_RegIN8		8
#define GPIO_PORT0_RegIN9		9
#define GPIO_PORT0_RegIN10		10
#define GPIO_PORT0_RegIN11		11
#define GPIO_PORT0_RegIN12		12
#define GPIO_PORT0_RegIN13		13
#define GPIO_PORT0_RegIN14		14
#define GPIO_PORT0_RegIN15		15

#define GPIO_PORT0_RegOUT		(GPIO_PORT0_BASEADDRESS + 2)		
#define	GPIO_PORT0_RegOUT0		0			
#define GPIO_PORT0_RegOUT1		1
#define GPIO_PORT0_RegOUT2		2
#define GPIO_PORT0_RegOUT3		3
#define GPIO_PORT0_RegOUT4		4
#define GPIO_PORT0_RegOUT5		5
#define GPIO_PORT0_RegOUT6		6
#define GPIO_PORT0_RegOUT7		7
#define GPIO_PORT0_RegOUT8		8
#define GPIO_PORT0_RegOUT9		9
#define GPIO_PORT0_RegOUT10		10
#define GPIO_PORT0_RegOUT11		11
#define GPIO_PORT0_RegOUT12		12
#define GPIO_PORT0_RegOUT13		13
#define GPIO_PORT0_RegOUT14		14
#define GPIO_PORT0_RegOUT15		15

#define GPIO_PORT0_RegDIR		(GPIO_PORT0_BASEADDRESS + 4)		
#define	GPIO_PORT0_RegDIR0		0			
#define GPIO_PORT0_RegDIR1		1
#define GPIO_PORT0_RegDIR2		2
#define GPIO_PORT0_RegDIR3		3
#define GPIO_PORT0_RegDIR4		4
#define GPIO_PORT0_RegDIR5		5
#define GPIO_PORT0_RegDIR6		6
#define GPIO_PORT0_RegDIR7		7
#define GPIO_PORT0_RegDIR8		8
#define GPIO_PORT0_RegDIR9		9
#define GPIO_PORT0_RegDIR10		10
#define GPIO_PORT0_RegDIR11		11
#define GPIO_PORT0_RegDIR12		12
#define GPIO_PORT0_RegDIR13		13
#define GPIO_PORT0_RegDIR14		14
#define GPIO_PORT0_RegDIR15		15
// 2a.)
// Definition mit welchem Globalen-Shared-Memory(-Segemnt) dieses Device arbeitet.
// Die Definition erfolgt ueber den zu verwendenden Index fuer das Export-Zeigers-Feld 
// Details zum zu Grunde liegenden Konzept finden sich in device_interface.c  // C.) 
// Das Wichtigste ist wie folgt:
//			GPIO_PORT0_EXPORTZEIGERFELD_INDEX  wird innerhalb des ganzen GPIO_PORT0.c verwendet
//			EXPORTZEIGERFELD_INDEX_FUER_GLOBAL_SHARED_MEMORY_0	ist in device.h definiert
//			EXPORTZEIGERFELD_INDEX_FUER_GLOBAL_SHARED_MEMORY_0	wird dem Konstruktor fuer das Globel-Shared-Memory uebergeben (siehe main.cpp)
//			EXPORTZEIGERFELD_INDEX_FUER_GLOBAL_SHARED_MEMORY_0	gehoert zu  NAME_GLOBAL_SHARED_MEMORY_0	 (was in device_interface.h als "SEEM"
//																definiert ist. "SEEM" ist der Name des Global-Shared-Memroy-(Segmentes)
// GPIO_PORT1 arbeit also mit "SEEM"
#define GPIO_PORT0_EXPORTZEIGERFELD_INDEX	EXPORTZEIGERFELD_INDEX_FUER_GLOBAL_SHARED_MEMORY_0 // also "SEEM"



// 2b.)
// Definition der Device-Struktur die als Teil der Struktur fuer das Lokale-Shared-Memory und das Globale-Shared-Memory
// verwendet wird.
typedef struct
{
    unsigned short GPIO_PORT0_RegDIR_str;		//Hat nur ein Register
	unsigned short GPIO_PORT0_RegOUT_str;		//Hat nur ein Register
	unsigned short GPIO_PORT0_RegIN_str;		//Hat nur ein Register
}STRC_GPIO_PORT0;



// 3a.)
// Export von Funktionen, da diese in der Datei device_interface.c in die "Zeiger-Felder" eingetragen werden, ...
// ... bzw (im Falle von init_GPIO_PORT0()) in der Funktion reset_simulation_function() aufgerufen werden
unsigned short			in_GPIO_PORT0(unsigned short registerAdr);
void					out_GPIO_PORT0(unsigned short registerAdr, unsigned short value);
void					out8_GPIO_PORT0(unsigned short registerAdr, unsigned char value);
void					ever_out_GPIO_PORT0(unsigned short registerAdr, unsigned short value); // Damit die Bndmodellsimulation in RegIn schreiben kann
extern void				GPIO_PORT0_Simulation();
extern void				sm_get_GPIO_PORT0Reg(unsigned short eigeneProzessNummer);
extern unsigned char	sm_set_GPIO_PORT0Reg(unsigned short eigeneProzessNummer);
extern unsigned char	init_GPIO_PORT0();




// 3b.) Export weiterer Flags
extern unsigned char GPIO_PORT0_An_Aus;



// 4.)
// Export weiterer (eigentlich) "Dinge", die vornehmlich bei der UI extern benoetigt werden.
void setNewGPIO_PORT0_RegDIR(unsigned short value);	//Hilfsfunktion in macro.c um automatische Einstellungen zu uebernehmen
#define __GPIO_PORT0_RegDIR_out(value)	setNewGPIO_PORT0_RegDIR((unsigned short) value)
#define __GPIO_PORT0_RegDIR_in			local_Shared_Mem_Copy.GPIO_PORT0_shared.GPIO_PORT0_RegDIR_str

void setNewGPIO_PORT0_RegOUT(unsigned short value);	//Hilfsfunktion in macro.c um automatische Einstellungen zu uebernehmen
#define __GPIO_PORT0_RegOUT_out(value)		setNewGPIO_PORT0_RegOUT((unsigned short) value)
#define __GPIO_PORT0_RegOUT_in				local_Shared_Mem_Copy.GPIO_PORT0_shared.GPIO_PORT0_RegOUT_str

void setNewGPIO_PORT0_RegIN(unsigned short value);	//Hilfsfunktion in macro.c um automatische Einstellungen zu uebernehmen
#define __GPIO_PORT0_RegIN_out(value)		setNewGPIO_PORT0_RegIN((unsigned short) value)
#define __GPIO_PORT0_RegIN_in				local_Shared_Mem_Copy.GPIO_PORT0_shared.GPIO_PORT0_RegIN_str

extern void* GPIO_PORT0_cntr_Mutex; 
#define __GPIO_PORT0_cntr_Mutex_lock		LockMutexOs(GPIO_PORT0_cntr_Mutex) 
#define __GPIO_PORT0_cntr_Mutex_release		UnlockMutexOs(GPIO_PORT0_cntr_Mutex) 

extern unsigned char ctr_GPIO_PORT0_flag;
#define	__ctr_GPIO_PORT0_flag_out(value)	ctr_GPIO_PORT0_flag = (unsigned char)value
#define	__ctr_GPIO_PORT0_flag_in			ctr_GPIO_PORT0_flag


#ifdef  __cplusplus
}
#endif

#endif //GPIO_PORT0
