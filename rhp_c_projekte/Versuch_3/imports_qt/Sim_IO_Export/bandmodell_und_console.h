#ifndef BANDMODELL_UND_CONSOLE_H
#define BANDMODELL_UND_CONSOLE_H

//#include "slave_message_passing_via_file.h"
//#include "bandmodell_messages.h"

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
//####################### Das Bandmodell_und_Console ist kein typisches Device denn es kommunziert via File ##################//
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//

#define USE_SIM_IO_FOR_BANDMODELL	// Anhand diese defines kann bestimmt werden ob das Bandmodell 
									// 1.) an einen Port der SimIO "angeschlossen" wird (USE_SIM_IO ist dann zu einzukommentiern): 
									//	   Die weiteren defines dienen zur Definition wo genau (Port, Bit) angeschlossen wird.
									//     In der User-Applikation die diese Lib nutz um das Bandmodell anzusteuern 
									//     kann im Fall 1.) mit den ganz normalen in() und out() Funktionen auf das Bandmodell zugegriffen werden.
									//	   DIES IST DER NORMALE FALL
									//     In der Funkion synchronisiere_bandmodelldaten() kann das normale out NICHT verwendet werden, denn
									//     out() schreibt per Definition nicht auf einen RegIN weshalb das ever_out() verwendet wird
									// oder
									// 2.) alleine also ohne Anschluss an die Ports des SimIO betrieben wird. Die IO-Daten des Bandmodells
									//     werden dann in einem globalen Feld io_buf[3] gespeichert.
									//     In der Applikation die das Bandmodell ansteuert kann im Fall 2.) NICHT mit den ganz normalen in() und out()
									//     zugegriffen werden sondern es muessen die Funktionen out_to_io_buf() und in_from_io_buf() verwendte werden
									//     DIESER FALL WIRD NUR ZUR SICHERHEIT (fuer denFall dass man spaeter doch ohne SimIO arbeitet) NOCH DRIN GELASSEN 
									//
									// Die GET- und STORE-Makros kapseln steuern die Art wie auf das Bandmodell zugeggriffen wird

#ifdef USE_SIM_IO_FOR_BANDMODELL

// Makros fuer die Handhabung der IO-Daten
// Die Sensorbits (HPX, ESX, TX) des Bandmodell sind an PORT 1 (Bit 0 bis 7) angeschlossen. Sensordaten => Speicherung in RegIN
#define GET_SENSOR_BITS						in_GPIO_PORT1(GPIO_PORT1_RegIN)					// PORT 1   RegIN
#define STORE_SENSORE_BITS(val)				ever_out_GPIO_PORT1(GPIO_PORT1_RegIN, val)
#define MASKE_SENSOR_BITS					0xFF	// 8 Bits

// Die Aktorbits (M_An, M_Li, M_Re, LedR, LedG) sind an PORT 1 (Bit 8 bis 12) angeschlossen. Aktordaten => Speicherung in RegOUT
#define GET_AKTOR_BITS						in_GPIO_PORT1(GPIO_PORT1_RegOUT)					// PORT 1	RegOUT
#define STORE_AKTOR_BITS(val)				out_GPIO_PORT1(GPIO_PORT1_RegOUT, val)				// Auf RegOUT darf immer geschrieben werden deshalb hier out_GPIO... und  kein ever_out_GPIO..
#define SHIFT_AKTOREN_BANDMODELL	8		// M_An, M_Li,... werden hier in dem upper Bytealso ab Bit 8  gespeichert

// Der Zaehlstand des bandmodells ist am PORT 0 angeschlossen. Sensordatem => Speicherung in RegIN
#define GET_ZAEHLSTAND						in_GPIO_PORT0(GPIO_PORT0_RegIN)				// PORT 0	RegIN
#define STORE_ZAEHLSTAND(val)				ever_out_GPIO_PORT0(GPIO_PORT0_RegIN, val)

#else
// Feld als Ersatz fuer die Regsiter der SimIO
extern unsigned short io_buf[3];		// Nimmt die Bits zum und vom Bandmodell sowie den Zaehlstand auf
										// Ist somit ein Ersatz fuer die Registr des SimIO
// Indices fuer das Feld
#define BITS_ZUM_BANDMODELL		0		// M_An, M_Li,...
#define BITS_VOM_BANDMODELL		1		// HPL, ESR, T1, ...
#define ZAEHLSTAND_BANDMODELL	2		// Zaehlstand vom Bandmodell 
#define SHIFT_AKTOREN_BANDMODELL 8		// M_An, M_Li,... werden hier in dem upper Byte gespeichert


// Funktionen als "Ersatz" fuer die  Regsiterzugriffsfunktionen der SimIO 
// Die weiter unten aufgefuehrten Makros sehen sich dann sehr aehnlich
extern void out_to_io_buf(unsigned short registerAdr, unsigned short value); 
extern unsigned short in_from_io_buf(unsigned short registerAdr); 

// Makros fuer die Handhabung der IO-Daten
// Die Sensorbits (HPX, ESX, TX) des Bandmodell werden im Feldelement mit Index BITS_VOM_BANDMODELL gespeichert
#define GET_SENSOR_BITS						in_from_io_buf(BITS_VOM_BANDMODELL)					
#define STORE_SENSORE_BITS(val)				out_to_io_buf(BITS_VOM_BANDMODELL, val)
#define MASKE_SENSOR_BITS					0xFF	// 8 Bits

// Die Aktorbits (M_An, M_Li, M_Re, LedR, LedG) werden im Feldelement mit Index BITS_ZUM_BANDMODELL gespeichert
#define GET_AKTOR_BITS						in_from_io_buf(BITS_ZUM_BANDMODELL)					
#define STORE_AKTOR_BITS(val)				out_to_io_buf(BITS_ZUM_BANDMODELL, val)
#define SHIFT_AKTOREN_BANDMODELL	8		// M_An, M_Li,... werden hier in dem upper Bytealso ab Bit 8  gespeichert

// Der Zaehlstand des Bandmodells wird im Feldelement mit Index ZAEHLSTAND_BANDMODELL gespeichert
#define GET_ZAEHLSTAND						in_from_io_buf(ZAEHLSTAND_BANDMODELL)			
#define STORE_ZAEHLSTAND(val)				out_to_io_buf(ZAEHLSTAND_BANDMODELL, val)

#endif //USE_SIM_IO_FOR_BANDMODELL

// Die exportierten Funktionen
extern void init_bandmodell();
extern void synchronisiere_bandmodelldaten();
extern void putstring(char* string);
extern void getstring(char* string);

extern char with_bandmodell;
extern void sync_when_bm_is_used();

#endif //BANDMODELL_UND_CONSOLE_H
