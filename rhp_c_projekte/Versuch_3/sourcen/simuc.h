#ifndef SIMUC_H
#define SIMUC_H

// ****************************************************************************************************
// Diese Datei buendelt die Includes und Defines die fuer eine Applikation fuer Studierende notwendig
// und hilfreich sind.
// Diese Datei muss nur in emain() also dem Quasi-Hauptprogramm-File der Studierenden included werden.
// ****************************************************************************************************

#include "simLib.h"
#include "task.h"	
#include "stdio.h"



//#pragma warning( disable : 4996 ) 		// Deaktiviert die Veralterungswarnung


// Hiermit wird zur Laufzeit bestimmt ob die io_out()-Funktionen auf eine Synchronisation mit dem UI (User-Interface)
// warten. Wenn ja dann sieht man die (durch io_out() hervorgerufenen) Registeraenderungen 
// direkt nachdem man ueber diese Codezeile hinweg gedebugged hat auch in der UI. Wenn nein, dann erfolgt
// die Visualisierung der Registeraenderung einige Code-Zeilen spaeter.
// Da eine aktivierte Synchronisation alles verlangsamt bietet es sich an bei schrittweiser Codeausfuehrung
// mit der Synchronisation zu arbeiten und danach, also bei der "Echtzeitausfuehrung", diese zu deaktivieren.
// ACHTUNG: Eine Aktivierung der Synchronisationen erfordert eine User_interface.exe mit der Vers. 1.3 oder hoeher.
//		do_wait_for_gsms=0;												// keine Synchronisation aber schnell
//		do_wait_for_gsms=1;												// mit Synchronisation aber langsam
//  DIESE INITIALISIERUNG DER GLOBLEN VARIBLEN WIRD IN DEN INIT_XY DEFINES GEMACHT


// INIT-Makros so dass hier auch noch was "nachgeschoben" werden kann ( char* dummy; dummy=(char*) arg )
#define INIT_BM_WITHOUT_REGISTER_UI					{ Flag_das_das_Steuerungmodul_aktiviert=  0  ; init_bandmodell(); with_bandmodell=1;						}
#define INIT_BM_WITH_REGISTER_UI__USING_SYNC		{ Flag_das_das_Steuerungmodul_aktiviert=  1  ; init_bandmodell(); with_bandmodell=1; do_wait_for_gsms=1;	}
//#define INIT_REGISTER_UI_WITHOUT_BM__USING_SYNC		{char* dummy; dummy=(char*) arg; Flag_das_das_Steuerungmodul_aktiviert=  1  ;				   ; with_bandmodell=0; do_wait_for_gsms=1;	}
#define INIT_REGISTER_UI_WITHOUT_BM__USING_SYNC		{ Flag_das_das_Steuerungmodul_aktiviert=  1  ;				   ; with_bandmodell=0; do_wait_for_gsms=1;	}

#define INIT_BM_WITH_REGISTER_UI__NO_SYNC			{ Flag_das_das_Steuerungmodul_aktiviert=  1  ; init_bandmodell(); with_bandmodell=1; do_wait_for_gsms=0;	}
#define INIT_REGISTER_UI_WITHOUT_BM__NO_SYNC		{ Flag_das_das_Steuerungmodul_aktiviert=  1  ;				   ; with_bandmodell=0; do_wait_for_gsms=0;	}

#define INIT_BM_WITH_REGISTER_UI		INIT_BM_WITH_REGISTER_UI__USING_SYNC		// Aus Kompatibilitaetsgruenden fuer aeltere Versionen
#define INIT_REGISTER_UI_WITHOUT_BM		INIT_REGISTER_UI_WITHOUT_BM__USING_SYNC		// Aus Kompatibilitaetsgruenden fuer aeltere Versionen


// Makro fuer die Beendigung des Applikations-Programms
// Da spaetesten bei den Automaten-Applikationen auch Dauerlaefer entstehen sollte die Variante mir der Warnung verwendet werden
#define END					{putstring("\nENDE: ##### Nicht vergessen das Debuggen zu beenden! #####\n");}						
//#define END					{putstring("\nENDE: ##### Das Programm wird beendet #####\n");exit(0)}						



// Define fuer die rudimentaren Stringoperationen
// Diese sind hier auf string.h Operationen abgebildet koennen fuer Kleinst-uC
// aber auch auf wirklich rudimetaere (eigene) Funktionen "umgemapped" werden
#define copystring(A,B)		{strcpy(B,A);}
#define appendstring(A,B) 	{strcat(B,A);}
#define itostring(A,B) 		{sprintf(B,"%d",(int)A);};
#define ftostring(A,B) 		{sprintf(B,"%f",(double)A);};
#define stringtoi atoi
#define stringtof atof
#define wait_for_return() 	{char temp[100]; getstring(temp);}

//--> Defines fuer eine verkuerzende Bennenung der SimIO-Register
#define IN0		GPIO_PORT0_RegIN
#define DIR0	GPIO_PORT0_RegDIR
#define OUT0	GPIO_PORT0_RegOUT

#define IN1		GPIO_PORT1_RegIN
	#define INT0	GPIO_PORT1_RegIN0		//Extern Interrupt 0
	#define INT1	GPIO_PORT1_RegIN1
	#define INT2	GPIO_PORT1_RegIN2
	#define INT3	GPIO_PORT1_RegIN3
	#define INT4	GPIO_PORT1_RegIN4
	#define INT5	GPIO_PORT1_RegIN5
	#define INT6	GPIO_PORT1_RegIN6
	#define INT7	GPIO_PORT1_RegIN7		//Extern Interupt 7

#define DIR1	GPIO_PORT1_RegDIR
#define OUT1	GPIO_PORT1_RegOUT
	#define OCO1	PWM_WF_Bit				//Timer1  On Compare Output 
#define PUL1	GPIO_PORT1_RegPUL	// Pullt Up Register
	#define PUE100	GPIO_PORT1_RegPUL0
	#define PUE101	GPIO_PORT1_RegPUL1
	#define PUE102	GPIO_PORT1_RegPUL2
	#define PUE103	GPIO_PORT1_RegPUL3
	#define PUE104	GPIO_PORT1_RegPUL4
	#define PUE105	GPIO_PORT1_RegPUL5
	#define PUE106	GPIO_PORT1_RegPUL6
	#define PUE107	GPIO_PORT1_RegPUL7
	#define PUE108	GPIO_PORT1_RegPUL8
	#define PUE109	GPIO_PORT1_RegPUL9
	#define PUE110	GPIO_PORT1_RegPUL10
	#define PUE111	GPIO_PORT1_RegPUL11
	#define PUE112	GPIO_PORT1_RegPUL12
	#define PUE113	GPIO_PORT1_RegPUL13
	#define PUE114	GPIO_PORT1_RegPUL14

#define EIE1	GPIO_PORT1_RegIE	//Extern Interupt Enable (Mask) Register
	#define EIE100	GPIO_PORT1_RegIE0		//Extern Interupt Enable 0
	#define EIE101	GPIO_PORT1_RegIE1
	#define EIE102	GPIO_PORT1_RegIE2
	#define EIE103	GPIO_PORT1_RegIE3
	#define EIE104	GPIO_PORT1_RegIE4
	#define EIE105	GPIO_PORT1_RegIE5
	#define EIE106	GPIO_PORT1_RegIE6
	#define EIE107	GPIO_PORT1_RegIE7		//Extern Interupt Enable 7

#define EIF1	GPIO_PORT1_RegIF	//Extern Interrupt Flag Register
	#define EIF100	GPIO_PORT1_RegIF0		//Extern Interupt Flag 0
	#define EIF101	GPIO_PORT1_RegIF1
	#define EIF102	GPIO_PORT1_RegIF2
	#define EIF103	GPIO_PORT1_RegIF3
	#define EIF104	GPIO_PORT1_RegIF4
	#define EIF105	GPIO_PORT1_RegIF5
	#define EIF106	GPIO_PORT1_RegIF6
	#define EIF107	GPIO_PORT1_RegIF7		//Extern Interupt Flag 7

#define	IVN_TOV1 (E_TIMER1_OVERFLOW + BASE_INTERRUPT_VECTOR_NUMBER)
#define	IVN_OC1 (E_TIMER1_COMPARE_MATCH_A + BASE_INTERRUPT_VECTOR_NUMBER)
#define	IVN_EI100 (E_EXTERN_GPIO_PORT1_RegIN0 + BASE_INTERRUPT_VECTOR_NUMBER)
#define	IVN_EI101 (E_EXTERN_GPIO_PORT1_RegIN1 + BASE_INTERRUPT_VECTOR_NUMBER)
#define	IVN_EI102 (E_EXTERN_GPIO_PORT1_RegIN2 + BASE_INTERRUPT_VECTOR_NUMBER)
#define	IVN_EI103 (E_EXTERN_GPIO_PORT1_RegIN3 + BASE_INTERRUPT_VECTOR_NUMBER)
#define	IVN_EI104 (E_EXTERN_GPIO_PORT1_RegIN4 + BASE_INTERRUPT_VECTOR_NUMBER)
#define	IVN_EI105 (E_EXTERN_GPIO_PORT1_RegIN5 + BASE_INTERRUPT_VECTOR_NUMBER)
#define	IVN_EI106 (E_EXTERN_GPIO_PORT1_RegIN6 + BASE_INTERRUPT_VECTOR_NUMBER)
#define	IVN_EI107 (E_EXTERN_GPIO_PORT1_RegIN7 + BASE_INTERRUPT_VECTOR_NUMBER)
#define	IVN_SPI1  (E_SPI1 + BASE_INTERRUPT_VECTOR_NUMBER)
#define	IVN_SPI2  (E_SPI2 + BASE_INTERRUPT_VECTOR_NUMBER)

#define PICC	EGIC	//Vectored Interrupt Controller Control-Register
	#define	PICE	EGIE //Vectored Interrupt Controller Enable

/*

*/
// Timer 1
#define	TCRA1	ETCLKC1				//Timer Control Register A
	#define		PS12	ECLK2				//Prescaler
	#define		PS11	ECLK1
	#define		PS10	ECLK0
	#define		TM11	ETM1				//Timer Mode
	#define		TM10	ETM0
#define TCRB1	ETCNTC1				//Timer Control Register B
	#define	WFE1	EWF						//Wave Form Enable
	#define CR1		ECNTR					//Counter Reset
	#define CE1		ECNT					//Counter Enable
#define	CNTV1	ECNTV1				//Counter Value Register
#define	CMPA1	ECMRA1				//Compare Match Register
#define	TIMR1	ETIMR1				//Timer Interrupt Mask Register
	#define	OCIE1	ECM1E					//On Compare Match Interrupt Enable
	#define TOVIE1	ECO1E					//Timer Overflow Interrupt Enable
#define TIFR1	ETIFR1				//Timer Interrupt Flag Register
	#define OCIF1	ECM1F					//On Compare Match Interrupt Flag
	#define TOVIF1	ECO1F					//Timer Overflow Interrupt Flag

// SPI1
// Status-Register
//#define SPI1_STAT	SPI1_STAT_REG	
#define SPSR1		SPI1_STAT_REG	// Alternative Bezeichnung zur Kompatibilitaet mit dem ATmega32	
	#define SPIF1	SPI1_STAT_REG7
	#define WCOL1	SPI1_STAT_REG6
// Konfigurations-Register alias Control-Register
//#define SPI1_CONF	SPI1_CONF_REG	
#define SPCR1		SPI1_CONF_REG	// Alternative Bezeichnung zur Kompatibilitaet mit dem ATmega32	
	#define SPR10	SPI1_CONF_REG0
	#define SPR11	SPI1_CONF_REG1
	#define CPHA1	SPI1_CONF_REG2
	#define CPOL1	SPI1_CONF_REG3
	#define MSTR1	SPI1_CONF_REG4
	#define notSS1	SPI1_CONF_REG5
	#define SPE1	SPI1_CONF_REG6
	#define	SPIE1	SPI1_CONF_REG7
// Daten-Register
//#define SPI1_DATA	SPI1_DATA_REG	
#define SPDR1		SPI1_DATA_REG	// Alternative Bezeichnung zur Kompatibilitaet mit dem ATmega32	

// SPI2
// Status-Register
//#define SPI2_STAT	SPI2_STAT_REG	
#define SPSR2		SPI2_STAT_REG	// Alternative Bezeichnung zur Kompatibilitaet mit dem ATmega32	
	#define SPIF2	SPI2_STAT_REG7
	#define WCOL2	SPI2_STAT_REG6
// Konfigurations-Register alias Control-Register
//#define SPI2_CONF	SPI2_CONF_REG
#define SPCR2		SPI2_CONF_REG	// Alternative Bezeichnung zur Kompatibilitaet mit dem ATmega32	
	#define SPR20	SPI2_CONF_REG0
	#define SPR21	SPI2_CONF_REG1
	#define CPHA2	SPI2_CONF_REG2
	#define CPOL2	SPI2_CONF_REG3
	#define MSTR2	SPI2_CONF_REG4
	#define notSS2	SPI2_CONF_REG5
	#define SPE2	SPI2_CONF_REG6
	#define	SPIE2	SPI2_CONF_REG7
// Daten-Register
//#define SPI2_DATA	SPI2_DATA_REG	// SPDR2
#define SPDR2		SPI2_DATA_REG	// Alternative Bezeichnung zur Kompatibilitaet mit dem ATmega32	
//<--


#define m_delay_ms(A)	{vTaskDelay(A);} 

#ifdef TESTS
#define FIX_ANZAHL_SYNC_PER_CYCLE 50
short int anzahl_bm_syncs=0;

// Erweiterete in() und out() Funktionen mit integriertem ...
// 1.) ... notwendigem "Schlafen legen" damit auch die Simulation oft genug "dran kommt"
// 2.) ... Warten bei Freeze so dass immer eine Editierung der GPIO-IN-Register in der UI moeglich ist
// 3.) ... Synchronieren der Daten mit der Bandmodellsimulation fuer den Fall, dass diese auch "angeschlossen ist". Dies ist 
//         erkennbar am Flag "with_bandmodell", das in den INIT_XY-Makros (siehe oben in dierser Datei) entsprechend gesetzt wird.
unsigned short io_in162(unsigned short Adr){
	//vTaskDelay(0);
	//{int i; for(i=0; i<1000;i++) {};}
	//Sleep(10);
	//wait(10);
	//while ( ((in(ESTG)>>15) & 0x01) == 1) { // Warten bei (solange) Freeze == 1
		//vTaskDelay(1); 
	//} 

	if(with_bandmodell==1) {
		synchronisiere_bandmodelldaten();
		anzahl_bm_syncs++;
	}
	return (in(Adr));
}

void io_out162(unsigned short Adr,unsigned short Val){
	out(Adr,Val); 
	if(with_bandmodell==1) {
		synchronisiere_bandmodelldaten();
		anzahl_bm_syncs++;
	}
	//vTaskDelay(0);
	//{int i; for(i=0; i<1000;i++) {};}
	//Sleep(10);
	//wait(10);
	//while ( ((in(ESTG)>>15) & 0x01) == 1) { // Warten bei (solange) Freeze == 1
		//vTaskDelay(1); 
	//}
};


void complete_syncs(){
	
	while(anzahl_bm_syncs < FIX_ANZAHL_SYNC_PER_CYCLE) {
		synchronisiere_bandmodelldaten();
		anzahl_bm_syncs++;
	}

	anzahl_bm_syncs=0;
};


char shared_memory_sync_done=0;

void sync_and_wait_for_shared_memory_process(){
	synchronisiere_bandmodelldaten();
	shared_memory_sync_done=0;
	while(shared_memory_sync_done==0) {
		
	}

};
#endif //TESTS

// Makro (und kleine Funktion) fuer die zyklische Synchronisation der Daten zwischen Bandmodell und Applikation
// Dieses muss zyklisch aufgerufen werden damit die Bandmodellsimulation funktioniert
// Fuer die Nutzung von Text-Out und Text-In ist die nicht notwendig
// Damit SYNC unabhaengig davon ob man mit oder ohne dem Bandmodell arbeitet immer "drin bleiben darf"
// wird die Funktion sync_when_bm_is_used() verwendet.


#define BANDSPEEDERSATZ 3	// Bestimmt wie lange ein SYNC_SIM schlaeft und damit indirekt (ueber Thread-Prozessorzuteilung) die 
							// Relation von Bandsimulationszeit und TimerCounter-Simulationszeit und somit
							// wieviel Bandzaehlercounts ca. (wirklich nur ungefaehr) auf einen Timercout kommen.


// Regel:	Setzen Sie einen Breakpoint auf SYNC_SIM und nehmen sie Aenderungen
//			(manuelle Aktionen wie z.B. T1 betaetigen) 
//			dann vor wenn die Software an diesem Breakpoitn haelt.
#define SYNC_SIM								{sync_when_bm_is_used(); /*Sleep(30)*/; vTaskDelay(BANDSPEEDERSATZ);}  //sync_when_bm_is_used synchronisiert 2 x


#define io_out(Adr,Val)			{out((Adr),(Val))  ;wait_for_gsms();}
#define io_out16(Adr,Val)		{out((Adr),(Val))  ;wait_for_gsms();} 
#define io_out8(Adr,Val)		{out8((Adr),(Val)) ;wait_for_gsms();}

#define io_in(Adr)			in((Adr))
#define io_in8(Adr)			in8((Adr))
#define io_in16(Adr)		in((Adr))

//#define io_out16(Adr,Val)		io_out162((Adr),(Val))
//#define io_in16(Adr)			io_in162((Adr))					

#define ms_wait(A)	vTaskDelay(A)

#endif //SIMUC_H
