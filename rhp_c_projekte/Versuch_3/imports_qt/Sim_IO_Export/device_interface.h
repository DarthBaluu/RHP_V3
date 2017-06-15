
#ifndef DEVICE_INTERFACE_H
#define DEVICE_INTERFACE_H


#ifdef  __cplusplus
extern "C" {
#endif


// 12.) Alle Include-Dateien der benutzten Devices hier einfuegen
// FFFFEEEERRRRTTTTIIIIGGGG
#include "timer1.h"
#include "gpio_port1.h"
#include "interruptcontroller.h"
#include "steuerung.h"
#include "pattern.h"
#include "gpio_port0.h"
#include "spi1.h"
#include "spi2.h"
#include "bandmodell_und_console.h" // ACHTUNG: Dies ist kein typisches Device, denn es kommuniziert via File


// In dieser Datei stehen die wichtigen Konfigurationsdaten
// Diese Datei wurde nur erstellt, damit die "Device-Programmierer" nich in der device_interface.h-Datei
// herumsuchen muessen.
#include "system_configuration.h"


// X.)
// Definition der Struktur fuer das Lokalen-Shared-Memory und das Gloabeln-Shared-Memory
// Hier sind zu Anfang der Strukturdefinition einfach die Struckturen fuer die einzelenen Devices einzufuegen
typedef struct
{
	LISTE_DER_DEVICE_STRUKTUREN

    /**
    *	Kontroll-Flags für die einzelnen Prozesse
    *	Idee ist hier einzelne Bits fuer bestimmte Komponenten zu nutzen
    *	Bit 0 fuer die Timer1 Value, Bit 1 fuer die Timer1 Register, Bit 2 fuer die GPIO Register, Bit 3 fuer die Interrupt Register, Bit 4 Komponenten Steuerung
	*
    *	Ein Prozess liest anfangs nur die eigenen Kontroll-Flags und ueberprueft die einzelnen Bits,
    *	ob von einem anderen Prozess eine Komponente aktualisiert wurde (Bit = 1) und liest diese gegebenenfalls aus.
	*	Als Array angelegt, um eine spaetere Anpassung der Prozessanzahl zu vereinfachen.
	*	Bisher wird nur mit zwei Prozessen gearbeitet (je ein Prozeß auf jeder Seite des GlobalShared-Memory)
	*   
    */
	unsigned long int ctr_Prozess[maxAnzahlProzesse];		//Kontroll Flags der Prozesse (z.B. Bit 0 Timer1 Value, Bit 1 Timer1 Register, Bit 2 GPIO Register, Bit 3 Interrupt Controller Register
														// MAX_COUNT_SHARED_MEMORY_CONTROL_BITS_Changes

}STRC_SHARED_MEM;



// ToDo: Ab hier noch etwas mehr kommentieren
//#define DO_LOCAL_SYNC 1		// Hiermit wird bestimmt ob die out_device()-Funktionen auf das Zuruecksetzen des Crontrolflags
#define DO_LOCAL_SYNC 0			// durch den anderen lokalen Thread "Kommunikationsdauerlauefer_Simulationsseitig()" und die
								// darin aufgerufene entsprende Device-Setterfunktion warten oder nicht
								// ACHTUNG: Das Warten (also mit #define DO_LOCAL_SYNC 1) verlangsamt alles sehr.
								//			Insbesondere wird die Reaktion auf Interrupts sehr traege, 
								//			d.h. diese koennen verloren gehen da nicht lang genug
								// So wie es aussieht reicht es auch auf das globale Sync 
								// (siehe Kommentare "GLOBALES_SYNHCRONISATIONS_KONZEPT") ein bis zweimal zu warten.
								// Dieses globale Sync wird immer angeboten und kann (muss aber nicht) genutzt werden.
								// Wenn das globale Sync genutzt wird dann verlangsamt es natuerlich auch aber nicht so dramitsch.
								// Es kann aber in simuc.h durch die Studierenden aktivier/deaktiviert werden.
								// Fazit: Wenn nicht (zukuenftig) notwendig, dann immer DO_LOCAL_SYNC 0	belassen.

#define ERROR_CREATE_MUTEX	1

#define MAX_COUNT_DEVICES 100


#define MAX_COUNT_LOCAL_CONTROL_FLAGS	MAX_COUNT_SHARED_MEMORY_CONTROL_BITS		// Hier gibt es kein prinzipielle Grenze, das die Flags nicht als Bits in einer Variablen verwalet werden
												// sondern fuer jedes Flag eine Variable existiert.
												// Fuer jedes genutze Flag muss es eine Shared-Memory-Setter-Funktion geben
												// Die Setter-Funktionen "kennen" die global definierten Flags.
												// Da Setter- und Getter-Funktionen (gemaess dem Konzept eigentlich) paarweise auftreten (Ausnahme Kontrolle fuer An- Aus)
												// und die Anzahl der Getter-Funktionen auf MAX_COUNT_SHARED_MEMORY_CONTROL_BITS begrenz ist, macht es Sinn diese beiden
												// Werte gleich gross zu halten.


// Feld fuer die In-Funktionen der Devices
extern unsigned short (*array_with_device_specific_in_functions[MAX_COUNT_DEVICES])(unsigned short registerAdr); 

// Feld fuer die Out-Funktionen der Devices
extern void (*array_with_device_specific_out_functions[MAX_COUNT_DEVICES])(unsigned short registerAdr, unsigned short value); 

// Feld fuer die Out8-Funktionen der Devices
extern void (*array_with_device_specific_out8_functions[MAX_COUNT_DEVICES])(unsigned short registerAdr, unsigned char value); 

// Feld fuer Shared-Memory-GETTER-Funktionen. Ein Eintrag ist z.B.ein Zeiger sm_get_GPIOReg()  
extern void (*array_with_shared_memory_getter_functions[MAX_COUNT_SHARED_MEMORY_CONTROL_BITS])(unsigned short eigeneProzessNummer);

// Feld fuer Shared-Memory-SETTER-Funktionen. Ein Eintrag ist z.B.ein Zeiger sm_get_GPIOReg()  
extern unsigned char (*array_with_shared_memory_setter_functions[MAX_COUNT_LOCAL_CONTROL_FLAGS])(unsigned short eigeneProzessNummer);




/**
*	Kontrol-Flag, Mutexe  und Makros um einen gleichzeitigen Zugriff der unterschiedlichen Threads auf die globalen Variablen zu verhindern.
*	Fuer den Zugriff auf das Feld  ctr_Prozess[] (das die Kontroll-Flags) beinhaltet ein Mutex. Naeheres zu den
*	Kotroll-Flags ist weiter unten (bei der Struktur zur Definition der Shared-Memory-Inhalte) zu finden.
*
*	Fuer den internen Shared Memory Parameter gibt es keinen Mutex, da dieser nur durch den Kommunikations-Thread
*	beschrieben wird.
*/
//extern unsigned char ctr_komponenten_flag;
extern void* control_Mutex; 

#define __control_Mutex_lock			LockMutexOs(control_Mutex) 
#define __control_Mutex_release			UnlockMutexOs(control_Mutex) 

// Zyklischer Zaehler, der zur Synchronisationen von lokalen Prozesen
// mit denen auf der anderen Seite des Global-Shared_memory verwendet wird.
// Siehe GLOBALES_SYNHCRONISATIONS_KONZEPT
extern unsigned long int global_shared_memory_sync_counter;

//Kontroll-Flags fuer die Prozesse
unsigned long int gibtCtrProzess(int prozess); // Hilfsfunktion in der macro.c um Fehleingaben abzufangen
#define __ctr_Prozess_out(prozess, value)		if((prozess>0)&&(prozess<=PROZESSES_IN_WORK)) local_Shared_Mem_Copy.ctr_Prozess[prozess-1] = value
#define __ctr_Prozess_in(prozess)				gibtCtrProzess(prozess)


// siehe device_interface.c	
extern STRC_SHARED_MEM local_Shared_Mem_Copy;		

// siehe device_interface.c
extern STRC_SHARED_MEM *global_pointer_ShMem_Seg[];		

// siehe device_interface.c
extern unsigned char reset_simulation_function_was_executed ;

// Export von Funktionen
extern void reset_simulation_function(); //Wird fuer in der FreeRTOS-Lib und fuer die UI benoetigt
extern unsigned char do_wait_for_gsms;
extern void wait_for_gsms();


#ifdef  __cplusplus
}
#endif

#endif //DEVICE_INTERFACE_H