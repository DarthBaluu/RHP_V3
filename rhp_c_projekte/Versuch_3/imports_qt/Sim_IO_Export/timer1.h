
#ifndef TIMER1_H
#define TIMER1_H

#include "os_support.h" 

#ifdef  __cplusplus
extern "C" {
#endif



//###########  Makros für die Timer1-Simulation => timer1.h
#define CLOCKRATE	4194304/16										/*!< (2^22 Hz/16)dient zur Berechnung des Wertes add_to_cnt (Zweierpotenz um einen runden Wert bei Teilung durch den Prescaler zu erhalten).
																		 Dies stellt ca. einen Sechzehntel der Geschwindigkeit des Atmel aus dem Praktikum da, dieser lief auf 4 MHz. */
#define PRESCALER	(unsigned int)(__EPrescaler_in)					//Lesen des automatisch gesetzten Perscalerwertes
#define COUNTERTOP	65535
#define FASTPWMTOP	0xFFFF //vormals 0x01FF (wraum genau ist fraglich)

/*!
*	Exemplarische Timer Modi
*/
#define	NORMALMODUS		0
#define CTCMODUS		1
#define FASTPWMMODUS	2

//	Bit 14 (15) des GPIO 0 fungiert als PWM (Waveform) Ausgabe.
#define PWM_WF_Bit	15
#define PWMBit		PWM_WF_Bit // Beider Ausgaben erfolgen jetzt durch das gleiche Signa auf dem gleichen Pin und 
#define WFBit		PWM_WF_Bit



	//Timer1
#define ETCLKC1         (TIMER1_BASEADDRESS + 0)		//Exemplary Timer/Counter 1 Clock Control Register
#define	ETM0		0               //Exemplary Timer Modus Bit [1:0]
#define	ETM1		1               //ETM1   ETM0
#define	ETCLKC12		2           // 0      0     Normaler Modus
#define	ETCLKC13		3           // 0      1     Clear Timer On Compare Match
#define	ETCLKC14		4
#define	ETCLKC15		5
#define	ETCLKC16		6
#define	ETCLKC17		7
#define	ETCLKC18		8
#define	ETCLKC19		9
#define	ETCLKC110		10
#define ECLK0           13			//Exemplary Clock Source Control
#define ECLK1           14			//ECLK2  ECLK1  ECLK0
#define ECLK2           15			//  0      0      1		internal clk/1 (no prescaling)
                                    //  0      1      0		internal clk/8 from prescaler
                                    //  0      1      1		internal clk/64 from prescaler
                                    //  1      0      0		internal clk/256 from prescaler
                                    //  1      0      1		internal clk/1024 from prescaler
                                    //  1      1      1		external clk   (bisher nicht in Simulation umgesetzt)

#define ETCNTC1         (TIMER1_BASEADDRESS + 2)		//Exemplary Counter Control Register
#define ECNT        	0			//Exemplary Counter Enable		(0 : disabled / 1 : enabled)
#define ECNTR       	1           //Exemplary Counter Reset       (0 : disabled / 1 : enabled)
#define ETCNTC12		2
#define ETCNTC13		3
#define ETCNTC14		4
#define ETCNTC15		5
#define ETCNTC16		6
#define ETCNTC17		7
#define ETCNTC18		8
#define ETCNTC19		9
#define ETCNTC110		10
#define ETCNTC111		11
#define ETCNTC112		12
#define EWF             13			//Exemplary Waveform (PWM) Enable		(0 : disabled / 1 : enabled)
#define ETCNTC114       14
#define ETCNTC115		15

#define ECNTV1          (TIMER1_BASEADDRESS + 4)		//Exemplary Timer/Counter 1 Value Register
#define ECNTV10         0
#define ECNTV11         1
#define ECNTV12         2
#define ECNTV13         3
#define ECNTV14         4
#define ECNTV15         5
#define ECNTV16         6
#define ECNTV17         7
#define ECNTV18         8
#define ECNTV19         9
#define ECNTV110		10
#define ECNTV111		11
#define ECNTV112		12
#define ECNTV113		13
#define ECNTV114		14
#define ECNTV115		15

#define ECMRA1          (TIMER1_BASEADDRESS + 6)		//Exemplary Compare Match Register A Timer/Counter 1
#define ECMRA10         0
#define ECMRA11         1
#define ECMRA12         2
#define ECMRA13         3
#define ECMRA14         4
#define ECMRA15         5
#define ECMRA16         6
#define ECMRA17         7
#define ECMRA18         8
#define ECMRA19         9
#define ECMRA110		10
#define ECMRA111		11
#define ECMRA112		12
#define ECMRA113		13
#define ECMRA114		14
#define ECMRA115		15

#define ETIMR1          (TIMER1_BASEADDRESS + 8)		//Exemplary Timer/Counter 1 Interrupt Mask Register
#define ECO1E           0			//Exemplary Counter Overflow Interrupt Enable	(0 : disabled / 1 : enabled)
#define ECM1E           1			//Exemplary Compare Match 1 Interrupt Enable	(0 : disabled / 1 : enabled)
#define ETIMR12         2
#define ETIMR13         3
#define ETIMR14         4
#define ETIMR15         5
#define ETIMR16         6
#define ETIMR17         7
#define ETIMR18         8
#define ETIMR19         9
#define ETIMR110		10
#define ETIMR111		11
#define ETIMR112		12
#define ETIMR113		13
#define ETIMR114		14
#define ETIMR115		15

#define ETIFR1          (TIMER1_BASEADDRESS + 10)		//Exemplary Timer/Counter 1 Interrupt Flag Register
#define ECO1F           0			//Exemplary Counter Overflow Interrupt Flag
#define ECM1F           1			//Exemplary Compare Match 1 Interrupt Flag
#define ETIFR12         2
#define ETIFR13         3
#define ETIFR14         4
#define ETIFR15         5
#define ETIFR16         6
#define ETIFR17         7
#define ETIFR18         8
#define ETIFR19         9
#define ETIFR110		10
#define ETIFR111		11
#define ETIFR112		12
#define ETIFR113		13
#define ETIFR114		14
#define ETIFR115		15

/**
*	Mutexe um einen gleichzeitigen Zugriff der unterschiedlichen Threads auf die globalen Variablen zu verhindern.
*	Je Device ein Mutex. Fuer den Timer Value gibt es einen extra Mutex, da dieser
*	staerker frequentiert ist und nicht von Registerzugriffen blockiert sein soll.
*/
extern void* timer1_Register_Mutex; 
extern void* timer1_CntValue_Mutex;

#define __timer1_Register_Mutex_lock	LockMutexOs(timer1_Register_Mutex) 
#define __timer1_Register_Mutex_release	UnlockMutexOs(timer1_Register_Mutex) 
#define __timer1_CntValue_Mutex_lock	LockMutexOs(timer1_CntValue_Mutex) 
#define __timer1_CntValue_Mutex_release	UnlockMutexOs (timer1_CntValue_Mutex) 


/*!
*	Kontroll-Flags zur Aenderungsmitteilung zwischen dem Simulations-Thread (Simulations-Funktion) und dem Thread fuer die 
*	Shared-Memory-Kommunikation (also ob sich in einer Komponente ein Wert geaendert hat).
*/
extern unsigned char ctr_timer1_CntValue_flag;	//internes Kontroll-Flag um eine Aenderung der Timer1 Counter Value Register zu melden
extern unsigned char ctr_timer1_register_flag;	//internes Kontroll-Flag um eine Aenderung der Timer1 Register zu melden

// 2a.)
// Definition mit welchem Globalen-Shared-Memory dieses Device arbeitet
// Die Definition erfolgt ueber den zu verwendenden Index fuer das Export-Zeigers-Feld (siehe device_interface.c  C.))
#define TIMER1_EXPORTZEIGERFELD_INDEX	EXPORTZEIGERFELD_INDEX_FUER_GLOBAL_SHARED_MEMORY_0 // also "SEEM"


/**
*	Struktur für die exemplarischen 16 Bit Timer Register
*/
typedef struct
{
    unsigned short ETCLKC1_str;		//Exemplary Clock Control Register
    unsigned short ETCNTC1_str;		//Exemplary Counter Control Register
    unsigned short ECNTV1_str;		//Exemplary Counter Value Register
    unsigned short ECMRA1_str;		//Exemplary Compare Match Register 1
    unsigned short ETIMR1_str;		//Exemplary Interrupt Mask Register
    unsigned short ETIFR1_str;		//Exemplary Interrupt Flag Register
    unsigned short EPrescaler;      //Exemplary Prescaler Value
}STRC_TIMER;


unsigned short	in_timer1(unsigned short registerAdr);
void			out_timer1(unsigned short registerAdr, unsigned short value);
void			out8_timer1(unsigned short registerAdr, unsigned char value);
void			Timer1_Simulation();




//Timer1-Makros fuer den Zugriff aufs Shared-Memory
#define __EPrescaler_out(value)		local_Shared_Mem_Copy.timer1_shared.EPrescaler = (unsigned short)value
#define __EPrescaler_in				local_Shared_Mem_Copy.timer1_shared.EPrescaler

void einstellenTCLKC1(unsigned short value);	//Hilfsfunktion in macro.c um automatische Einstellungen zu uebernehmen
#define __ETCLKC1_out(value)	einstellenTCLKC1((unsigned short)value)
#define __ETCLKC1_in			local_Shared_Mem_Copy.timer1_shared.ETCLKC1_str

void setNewTCNTC1(unsigned short value);	//Hilfsfunktion in macro.c um automatische Einstellungen zu uebernehmen
#define __ETCNTC1_out(value)	setNewTCNTC1((unsigned short)value)
#define __ETCNTC1_in			local_Shared_Mem_Copy.timer1_shared.ETCNTC1_str

#define __ECNTV1_out(value)	local_Shared_Mem_Copy.timer1_shared.ECNTV1_str = (unsigned short)value
#define __ECNTV1_in			local_Shared_Mem_Copy.timer1_shared.ECNTV1_str

void setNewECMRA1(unsigned short value);	//Hilfsfunktion in macro.c um automatische Einstellungen zu uebernehmen
#define __ECMRA1_out(value)	setNewECMRA1((unsigned short) value)
#define __ECMRA1_in			local_Shared_Mem_Copy.timer1_shared.ECMRA1_str

void setNewTIMR1(unsigned short value);	//Hilfsfunktion in macro.c um automatische Einstellungen zu uebernehmen
#define __ETIMR1_out(value)	setNewTIMR1((unsigned short)value)
#define __ETIMR1_in			local_Shared_Mem_Copy.timer1_shared.ETIMR1_str

void setNewTIFR1(unsigned short value);	//Hilfsfunktion in macro.c um automatische Einstellungen zu uebernehmen
#define __ETIFR1_out(value)	setNewTIFR1((unsigned short)value)
#define __ETIFR1_in			local_Shared_Mem_Copy.timer1_shared.ETIFR1_str

#define __toggleWF			__GPIO_PORT1_RegOUT_out(__GPIO_PORT1_RegOUT_in ^ (1<< WFBit))			//toggle Waveform-Signal
#define __setPWM			__GPIO_PORT1_RegOUT_out(__GPIO_PORT1_RegOUT_in | (1<< PWMBit))
#define __unsetPWM			__GPIO_PORT1_RegOUT_out(__GPIO_PORT1_RegOUT_in & ~(1<< PWMBit))


//Makros fuer den Zugriff auf interne Kontroll-Flags
#define	__ctr_timer1_CntValue_flag_out(value)	ctr_timer1_CntValue_flag = (unsigned char)value
#define	__ctr_timer1_CntValue_flag_in           ctr_timer1_CntValue_flag


#define	__ctr_timer1_register_flag_out(value)	ctr_timer1_register_flag = (unsigned char)value
#define	__ctr_timer1_register_flag_in           ctr_timer1_register_flag





extern void sm_get_Timer1CntVal(unsigned short eigeneProzessNummer);
extern void sm_get_Timer1Reg(unsigned short eigeneProzessNummer);

extern unsigned char sm_set_Timer1CntVal(unsigned short eigeneProzessNummer);
extern unsigned char sm_set_Timer1Reg(unsigned short eigeneProzessNummer);

extern unsigned char init_timer1();

extern unsigned char Timer1_An_Aus;



#ifdef  __cplusplus
}
#endif

#endif //TIMER1_H