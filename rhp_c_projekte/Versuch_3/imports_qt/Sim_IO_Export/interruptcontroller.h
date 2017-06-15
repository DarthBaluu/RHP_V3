
#ifndef INTERRUPTCONTROLLER_H
#define INTERRUPTCONTROLLER_H

#include "os_support.h" 

#ifdef  __cplusplus
extern "C" {
#endif


//Interrupt Controller
#define EGIC		(INTERRUPT_CONTROLLER_BASEADDRESS + 0)		//Exemplary General Interrupt Control Register
#define	EGIE		0			//Exemplary Global Interrupt Enable				(0 : disabled / 1 : enabled)
#define EGIC1		1
#define EGIC2		2
#define EGIC3		3
#define EGIC4		4
#define EGIC5		5
#define EGIC6		6
#define EGIC7		7
#define EGIC8		8
#define EGIC9		9
#define EGIC10		10
#define EGIC11		11
#define EGIC12		12
#define EGIC13		13
#define EGIC14		14
#define EGIC15		15



/*!
*	Makros um unter FreeRTOS einen InterruptHandler (ISR) einem Interrupt zuzuweisen.
*/
#define BASE_INTERRUPT_VECTOR_NUMBER	0x30 // Offset bzgl. der Interruptvektornummer damit beim IA-32 die Interupt-Vektor-Nummer oberhalb
											 // von 32 und damit ausserhalb des erlaubten Bereiches liegt.
											 // Der Wert 0x30 ist kein versehen sondern angelehnt an Windows, denn hier gilt für IRQ0 der Wert 0x30

// Die Simulation eines Interrupts im FreeRTOS-Windows-Port sieht vor, dass die ISR einen Returnwert hat.
// Ueber diesen Returnwert kann bestimmt werden ob nach Beendigung der ISR ein Taskwechsel ausgefuehrt werden soll 
// (Rueckgabewert !=0) oder nicht (Rueckgabewert == 0).
// Fuer den normalen Nutzer (die Studierenden) ist es nicht sinnvoll, dass eine ISR einen Returnwert haben muss.
// Deshalb arbeiten wir mit ISRs ohne Returnwert.
// Da der Returnwert im EAX-Register des Prozessors zurueck gegeben wird entscheidet
// bei "unseren ISRs" (also ISRs ohne Returnwert) der Inhalt dieses EAX-Register (eher zufaellig) 
// ob ein Taskwechsel ausgeführt wird oder nicht. 
// Damit kann man leben.
// Deshalb wird in dem folgenden Makro ein Casten der ISR-Schnittstelle auf (uint32_t (*)( void )) vorgenommen, also mit uint32_t Returnwert.
// Ohne dieses Casten tuts auch aber es gibt eine Compilerwarnung.

#define	setInterruptHandler(interruptNumber, ISR)		vPortSetInterruptHandler((interruptNumber-BASE_INTERRUPT_VECTOR_NUMBER), (uint32_os (*)( void )) (ISR) )



/**
*	Mutexe um einen gleichzeitigen Zugriff der unterschiedlichen Threads auf die globalen Variablen zu verhindern.
*	Je Device ein Mutex. 
*/
extern void* interrupt_cntr_Mutex; 

#define __interrupt_cntr_Mutex_lock		LockMutexOs(interrupt_cntr_Mutex)
#define __interrupt_cntr_Mutex_release		UnlockMutexOs(interrupt_cntr_Mutex)


/**
*	Struktur für die exemplarischen Interrupt Controller Register
*/
typedef struct
{
    unsigned short EGIC_str;		//Exemplary General Interrupt Control Register
}STRC_INTERRUPT_CNTR;


/*!
*	Kontroll-Flags zur Aenderungsmitteilung zwischen dem Simulations-Thread (Simulations-Funktion) und dem Thread fuer die 
*	Shared-Memory-Kommunikation (also ob sich in einer Komponente ein Wert geaendert hat).
*/
extern unsigned char ctr_interrupt_flag;

// 2a.)
// Definition mit welchem Globalen-Shared-Memory dieses Device arbeitet
// Die Definition erfolgt ueber den zu verwendenden Index fuer das Export-Zeigers-Feld (siehe device_interface.c  C.))
#define INTERRUPT_EXPORTZEIGERFELD_INDEX	EXPORTZEIGERFELD_INDEX_FUER_GLOBAL_SHARED_MEMORY_0 // also "SEEM"


unsigned short	in_interruptcontroller(unsigned short registerAdr);
void			out_interruptcontroller(unsigned short registerAdr, unsigned short value);
void			out8_interruptcontroller(unsigned short registerAdr, unsigned char value);

//Interrupt Controller-Makros fuer den Zugriff aufs Shared-Memory
void setNewGIC(unsigned short value);	//Hilfsfunktion in macro.c um automatische Einstellungen zu uebernehmen
#define __EGIC_out(value)	setNewGIC((unsigned short) value)
#define __EGIC_in			local_Shared_Mem_Copy.intcntr_shared.EGIC_str


//Makros fuer den Zugriff auf interne Kontroll-Flags
#define	__ctr_interrupt_flag_out(value)	ctr_interrupt_flag = (unsigned char)value
#define	__ctr_interrupt_flag_in			ctr_interrupt_flag


extern void sm_get_InterruptReg(unsigned short eigeneProzessNummer);
extern unsigned char sm_setInterruptReg(unsigned short eigeneProzessNummer);

extern unsigned char init_interruptcontroller();

#ifdef  __cplusplus
}
#endif

#endif //INTERRUPTCONTROLLER
