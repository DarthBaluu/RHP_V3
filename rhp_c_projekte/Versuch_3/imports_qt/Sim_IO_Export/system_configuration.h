

// Nach der Erstellung/Programmierung der einzelnen Devices ist hier das Gesamtsystem zu konfigurieren:


// 1.) 
//	Definition der Belegung der Interruptvektortabelle
//	Aufteilung der Interrupts. D.h. Definierung der Position des Interrupt Vektors der einzelnen Interruptquellen und
//	die Position im eiok_int_InterruptFlagRegister.
// 
// Derzeit sind max 32 Interruptquellen moeglich (siehe portMAX_INTERRUPTS im FreeRTOS-Port)
//
//	Bit:				Quelle:
//	0					portINTERRUPT_YIELD		(beide portINTERRUPTs sind von FreeRTOS und sind fuer den Ablauf
//	1					portINTERRUPT_TICK		 des Ports fuer Windows wichtig und muessen an erster Position bleiben!)
//	2					E_TIMER1_OVERFLOW
//	3					E_TIMER1_COMPARE_MATCH_A
//	4					E_EXTERN_GPIO_PORT1_RegIN0
//
//  ...
//  31					noch nicht belegt

#define E_TIMER1_OVERFLOW						( 2UL )
#define E_TIMER1_COMPARE_MATCH_A				( 3UL )
#define E_EXTERN_GPIO_PORT1_RegIN0				( 4UL )
#define E_EXTERN_GPIO_PORT1_RegIN1				( 5UL )
#define E_EXTERN_GPIO_PORT1_RegIN2				( 6UL )
#define E_EXTERN_GPIO_PORT1_RegIN3				( 7UL )
#define E_EXTERN_GPIO_PORT1_RegIN4				( 8UL )
#define E_EXTERN_GPIO_PORT1_RegIN5				( 9UL )
#define E_EXTERN_GPIO_PORT1_RegIN6				( 10UL )
#define E_EXTERN_GPIO_PORT1_RegIN7				( 11UL )
#define E_SPI1									( 12UL )
#define E_SPI2									( 13UL )



// 2.)
// Definition der belegung des I/O-Adressraums, d.h. der Basis-Adressen der Devices
// Einfache Regel. Jedes Device bekommt einen Adressbereich mit 100 Adressen (MAX_DEVICE_ADDRESS_SPACE) auch dann wenn es weniger benoetigt.
// Alle Adressbereiche liegen hintereinander im Speicher, d.h. die Basisadresen sind in 100ter-Schritten hochzuzaehlen

#define MAX_DEVICE_ADDRESS_SPACE			100		// Nicht veraendern!!!

#define TIMER1_BASEADDRESS					0		// Bei 0 beginnen
#define GPIO_PORT1_BASEADDRESS				100		// Basisadressen in 100ter-Schritten hochzaehlen ...
#define INTERRUPT_CONTROLLER_BASEADDRESS	200		// ... ohne Luecken
#define GPIO_PORT0_BASEADDRESS				300
#define STEUERUNG_BASEADDRESS				400
#define PATTERN_BASEADDRESS					500
#define SPI1_BASEADDRESS					600
#define SPI2_BASEADDRESS					700



//3a.) 
// Definition fuer die Lage (Index) der Kontroll-Flags im Feld ctr_Prozess[]
// Fuer jedes Paar von Getter-Setter-Funktionen gibt es ein Bit.
// Die Position dieses Bits ist eindeutig zu definieren.
// ACHTUNGT: Es stehen nur MAX_COUNT_SHARED_MEMORY_CONTROL_BITS zur Verfuegung.

// Wenn MAX_COUNT_SHARED_MEMORY_CONTROL_BITS erheoht wird dann sind folgende Stelle mit zu beruecksichtigen die mit
// dem Kommnetar "MAX_COUNT_SHARED_MEMORY_CONTROL_BITS_Changes" gekennzeichnet sind.
// Diese finden sich sowohl hier im Projekt als auch in den "Gemeinsam genutzte Dateien" (als Teil der die EXE-Projekte).
// Bei dem erstellten SW-Rahmen gehe ich davon aus, dass "unsigned long int" NICHT GROESSER als 32 Bit ist.
// Kleiner darf es sein jedoch dürfen dann nur entsprechen weniger Bits wirklich genutzt werden.
#define MAX_COUNT_SHARED_MEMORY_CONTROL_BITS	((sizeof(unsigned long int)) << 3)	// 4*8 = 32 Bit, Shiften ist schneller als Multiplikation
														// Prinzipiell maximal moegliche Anzahl nutzbarer Shared-Memory-Kontrollbits, Derzeit leider nur 8.
														// Fuer jedes genutzte Shared-Memory-Control-Bit muss es eine Shared-Memory-Getter- und/oder
														// Setter-Funktion geben.

// ACHTUNG !!! 
// Die folgende Nummerierung ist auch im UI-Projekt in der Device-spezifischen Datei XY_ui.cpp, (z.B. PATTERN_ui.cpp)
// unter 4b.) zu aendern (also dort wo  die DEvice-spezifische Vierbindung zwischen den Signals zu Slots definiert wird.
// Der erlaubte Wertebereich geht von 0 bis MAX_COUNT_SHARED_MEMORY_CONTROL_BITS
#define TIMER1_VAL				0
#define	TIMER1_REG				1
#define	GPIO_PORT1				2
#define INTERRUPT				3
#define STEUERUNG				4

#define PATTERN					6
#define GPIO_PORT0				7
#define SPI1					8
#define SPI2					9

//3b.) 
// Definition der UI-Ausgabe (unten im Fenster) wenn ein Register in der UI editiert wurde.
// Es gilt die Nummierungs aus 3a.)

#define SETTEXT0	"Timer1 Value uebertragen"
#define SETTEXT1	"Timer1 Register ohne Counter Value uebertragen"
#define SETTEXT2	"GPIO Register uebertragen"
#define SETTEXT3	"Interrupt Register uebertragen"
#define SETTEXT4	"Steuerung wurde genutzt"
#define SETTEXT5	"Diese Meldung sollte nicht erscheinen"
#define SETTEXT6	"PATTERN wurde editiert"
#define SETTEXT7	"GPIO_PORT0 wurde editiert"	
#define SETTEXT8	"SPI1 wurde editiert"
#define SETTEXT9	"SPI2 wurde editiert"
#define SETTEXT10	"Diese Meldung sollte nicht erscheinen"
#define SETTEXT11	"Diese Meldung sollte nicht erscheinen"
#define SETTEXT12	"Diese Meldung sollte nicht erscheinen"
#define SETTEXT13	"Diese Meldung sollte nicht erscheinen"
#define SETTEXT14	"Diese Meldung sollte nicht erscheinen"
#define SETTEXT15	"Diese Meldung sollte nicht erscheinen"
#define SETTEXT16	"Diese Meldung sollte nicht erscheinen"
#define SETTEXT17	"Diese Meldung sollte nicht erscheinen"
#define SETTEXT18	"Diese Meldung sollte nicht erscheinen"
#define SETTEXT19	"Diese Meldung sollte nicht erscheinen"
#define SETTEXT20	"Diese Meldung sollte nicht erscheinen"
#define SETTEXT21	"Diese Meldung sollte nicht erscheinen"
#define SETTEXT22	"Diese Meldung sollte nicht erscheinen"
#define SETTEXT23	"Diese Meldung sollte nicht erscheinen"
#define SETTEXT24	"Diese Meldung sollte nicht erscheinen"
#define SETTEXT25	"Diese Meldung sollte nicht erscheinen"
#define SETTEXT26	"Diese Meldung sollte nicht erscheinen"
#define SETTEXT27	"Diese Meldung sollte nicht erscheinen"
#define SETTEXT28	"Diese Meldung sollte nicht erscheinen"
#define SETTEXT29	"Diese Meldung sollte nicht erscheinen"
#define SETTEXT30	"Diese Meldung sollte nicht erscheinen"
#define SETTEXT31	"Diese Meldung sollte nicht erscheinen" //"MAX_COUNT_SHARED_MEMORY_CONTROL_BITS_Changes"



// 4.)
// Definition bzgl. der Prozesse die auf des selbe Global-Shared-Memory(-Segment) (z.B. "SEEM") zugreifen.
// Derzeit wird nur mit zwei Prozessen gearbeitet, naemlich einer fuer die Simulationsseite und einer fuer die
// User-Interface-Seite
#define maxAnzahlProzesse		4	
#define PROZESSES_IN_WORK 2		//bisher wird mit nur mit zwei Prozessen gearbeitet
#define PROZESS1	1				
#define PROZESS2	2
//#define PROZESS3	3
//#define PROZESS4	4





// 5.)
// Definition der Namen der verschiedenen benoetigten Global-Shared-Memory(-Segmente)
// Hier wird zwar alles fuer zwei Global-Shared-Memory (naemlich SEEM und SAAM) definiert
// Tatsaechlich genutzt (und instanziiert) wird nur SEEM (siehe main.cpp).
// Alles zu SAAM versteht sich also nur als ein Beispiel .
// Siehe auch Kommentar zu "_SHARED_MEM *global_pointer_ShMem_Seg[" in der Datei "device_interface.c".
// BISHER WURDE NUR MIT EINEM GLOBAL-SHARED-MEMORY (NAEMLICH SEEM) GEARBEITET.
// DIE PARALLELE NUTZUNG MEHRERER GLOBAL-SHARED-MEMORY IST NOCH VOLLKOMMEN UNGETESTET.
// ToDo: Noch wirklich testen und dann den vorangestellten Kommentar raus nehmen
#define MAX_ANZAHL_GLOBAL_SHARED_MEMORY 2 

#define NAME_GLOBAL_SHARED_MEMORY_0							"SEEM"
#define EXPORTZEIGERFELD_INDEX_FUER_GLOBAL_SHARED_MEMORY_0		0

#define NAME_GLOBAL_SHARED_MEMORY_1							"SAAM"
#define EXPORTZEIGERFELD_INDEX_FUER_GLOBAL_SHARED_MEMORY_1		1



// 6.) 
// Definition der Lage der Bits zum Ein-/Ausschalten einzelner Devices.
// ACHTUNG: Der Maximalwert ist 15 (also werte von 0 bis 15 sind erlaubt)
// Wird (bis auf FREEZE_AN_AUS_BIT) nur dann benoetigt wenn die Devices so "programmiert" sind, dass diese ein- und ausgeschaltet werden koennen.
// Dafuer muss der "Device-Progammierer" voll und ganz selber sorgen. Wie dies geht zeigt die Pattern-Vorlage.

#define FREEZE_AN_AUS_BIT		15	 // Diese Definition nicht loeschen, denn die wird fuer das Freeze des gesamten Systems 
								 // (also FreeRTOS-Scheduler-Thread und Simulations-Thread)
#define TIMER1_AN_AUS_BIT		0
#define GPIO_PORT1_AN_AUS_BIT	1
#define PATTERN_AN_AUS_BIT		2
#define GPIO_PORT0_AN_AUS_BIT	3
#define SPI1_AN_AUS_BIT			4
#define SPI2_AN_AUS_BIT			5



// 7.) 
// Definition welche Device-Strukturen in die Shared-Memorie Struktur in welcher Reihenfolge
// abgelegt sind.
// ACHTIUNG: Die "\" am Ende jeder Zeile muessen stehen gelassen werden.
#define LISTE_DER_DEVICE_STRUKTUREN			STRC_GPIO				gpio_shared;	\
											STRC_TIMER				timer1_shared; \
											STRC_INTERRUPT_CNTR		intcntr_shared; \
											STRC_STEUERUNG			steuerung_shared; \
											STRC_PATTERN			PATTERN_shared; \
											STRC_GPIO_PORT0			GPIO_PORT0_shared; \
											STRC_SPI1				SPI1_shared; \
											STRC_SPI2				SPI2_shared;


// 8.)
// WEITER GEHTS in der Datei system_configuration.c bei 9.)