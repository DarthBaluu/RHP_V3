
#ifndef GPIO_PORT1_H
#define GPIO_PORT1_H

#include "os_support.h" 

#ifdef  __cplusplus
extern "C" {
#endif



// 1.)
// Definition der Makros mit denen die Studierenden auf die Register des Devices und die Register-Bits zugreifen koennen
// Die Adresse der Register berechnet sich aus einen Basis-Adresse des Devices und eines direkt angegeben Offsets (im Falle von GPIO_PORT1_RegIN mit dem Wert 0x0010).
// Die Basisadressen der verschiedenen Devices duerfen nicht kollidieren.
// Deshalb werden diese Basis-Adressen an einer zentralen Stelle (in device_interface.h) vorgenommen. 
#define GPIO_PORT1_RegIN		(GPIO_PORT1_BASEADDRESS + 0)		//Exemplary Input Register 0
#define GPIO_PORT1_RegIN0		0
#define GPIO_PORT1_RegIN1		1
#define GPIO_PORT1_RegIN2		2
#define GPIO_PORT1_RegIN3		3
#define GPIO_PORT1_RegIN4		4
#define GPIO_PORT1_RegIN5		5
#define GPIO_PORT1_RegIN6		6
#define GPIO_PORT1_RegIN7		7
#define GPIO_PORT1_RegIN8		8
#define GPIO_PORT1_RegIN9		9
#define GPIO_PORT1_RegIN10		10
#define GPIO_PORT1_RegIN11		11
#define GPIO_PORT1_RegIN12		12
#define GPIO_PORT1_RegIN13		13
#define GPIO_PORT1_RegIN14		14
#define GPIO_PORT1_RegIN15		15

#define GPIO_PORT1_RegOUT		(GPIO_PORT1_BASEADDRESS + 2)		//Exemplary Output Register 0
#define GPIO_PORT1_RegOUT0		0
#define GPIO_PORT1_RegOUT1		1
#define GPIO_PORT1_RegOUT2		2
#define GPIO_PORT1_RegOUT3		3
#define GPIO_PORT1_RegOUT4		4
#define GPIO_PORT1_RegOUT5		5
#define GPIO_PORT1_RegOUT6		6
#define GPIO_PORT1_RegOUT7		7
#define GPIO_PORT1_RegOUT8		8
#define GPIO_PORT1_RegOUT9		9
#define GPIO_PORT1_RegOUT10		10
#define GPIO_PORT1_RegOUT11		11
#define GPIO_PORT1_RegOUT12		12
#define GPIO_PORT1_RegOUT13		13
#define GPIO_PORT1_RegOUT14		14
#define GPIO_PORT1_RegOUT15		15

#define GPIO_PORT1_RegDIR		(GPIO_PORT1_BASEADDRESS + 4)		//Exemplary Data Direction Register 0
#define GPIO_PORT1_RegDIR0		0			// 0 : Input
#define GPIO_PORT1_RegDIR1		1			// 1 : Output
#define GPIO_PORT1_RegDIR2		2
#define GPIO_PORT1_RegDIR3		3
#define GPIO_PORT1_RegDIR4		4
#define GPIO_PORT1_RegDIR5		5
#define GPIO_PORT1_RegDIR6		6
#define GPIO_PORT1_RegDIR7		7
#define GPIO_PORT1_RegDIR8		8
#define GPIO_PORT1_RegDIR9		9
#define GPIO_PORT1_RegDIR10		10
#define GPIO_PORT1_RegDIR11		11
#define GPIO_PORT1_RegDIR12		12
#define GPIO_PORT1_RegDIR13		13
#define GPIO_PORT1_RegDIR14		14
#define GPIO_PORT1_RegDIR15		15

#define GPIO_PORT1_RegPUL		(GPIO_PORT1_BASEADDRESS + 6)      //Exemplary Pull-Up Control Register 0
#define GPIO_PORT1_RegPUL0		0           //Exemplary Pull-Up Enable    (0 : disabled / 1 : enabled)
#define GPIO_PORT1_RegPUL1		1
#define GPIO_PORT1_RegPUL2		2
#define GPIO_PORT1_RegPUL3		3
#define GPIO_PORT1_RegPUL4		4
#define GPIO_PORT1_RegPUL5		5
#define GPIO_PORT1_RegPUL6		6
#define GPIO_PORT1_RegPUL7		7
#define GPIO_PORT1_RegPUL8		8
#define GPIO_PORT1_RegPUL9		9
#define GPIO_PORT1_RegPUL10		10
#define GPIO_PORT1_RegPUL11		11
#define GPIO_PORT1_RegPUL12		12
#define GPIO_PORT1_RegPUL13		13
#define GPIO_PORT1_RegPUL14		14
#define GPIO_PORT1_RegPUL15		15

#define GPIO_PORT1_RegIE		(GPIO_PORT1_BASEADDRESS + 8)      //Exemplary Interrupt Mask Register 0
#define GPIO_PORT1_RegIE0		0           //Exemplary Interrupt Enable    (0 : disabled / 1 : enabled)
#define GPIO_PORT1_RegIE1		1
#define GPIO_PORT1_RegIE2		2
#define GPIO_PORT1_RegIE3		3
#define GPIO_PORT1_RegIE4		4
#define GPIO_PORT1_RegIE5		5
#define GPIO_PORT1_RegIE6		6
#define GPIO_PORT1_RegIE7		7
#define GPIO_PORT1_RegIE8		8
#define GPIO_PORT1_RegIE9		9
#define GPIO_PORT1_RegIE10		10
#define GPIO_PORT1_RegIE11		11
#define GPIO_PORT1_RegIE12		12
#define GPIO_PORT1_RegIE13		13
#define GPIO_PORT1_RegIE14		14
#define GPIO_PORT1_RegIE15		15

#define GPIO_PORT1_RegIF		(GPIO_PORT1_BASEADDRESS + 10)      //Exemplary Interrupt Flag Register 0
#define GPIO_PORT1_RegIF0		0
#define GPIO_PORT1_RegIF1		1
#define GPIO_PORT1_RegIF2		2
#define GPIO_PORT1_RegIF3		3
#define GPIO_PORT1_RegIF4		4
#define GPIO_PORT1_RegIF5		5
#define GPIO_PORT1_RegIF6		6
#define GPIO_PORT1_RegIF7		7
#define GPIO_PORT1_RegIF8		8
#define GPIO_PORT1_RegIF9		9
#define GPIO_PORT1_RegIF10		10
#define GPIO_PORT1_RegIF11		11
#define GPIO_PORT1_RegIF12		12
#define GPIO_PORT1_RegIF13		13
#define GPIO_PORT1_RegIF14		14
#define GPIO_PORT1_RegIF15		15


// 2a.)
// Definition mit welchem Globalen-Shared-Memory(-Segemnt) dieses Device arbeitet.
// Die Definition erfolgt ueber den zu verwendenden Index fuer das Export-Zeigers-Feld 
// Details zum zu Grunde liegenden Konzept finden sich in device_interface.c  // C.) 
// Das Wichtigste ist wie folgt:
//			GPIO_PORT1_EXPORTZEIGERFELD_INDEX  wird innerhalb des ganzen gpio.c verwendet
//			EXPORTZEIGERFELD_INDEX_FUER_GLOBAL_SHARED_MEMORY_0	ist in device.h definiert
//			EXPORTZEIGERFELD_INDEX_FUER_GLOBAL_SHARED_MEMORY_0	wird dem Konstruktor fuer das Globel-Shared-Memory uebergeben (siehe main.cpp)
//			EXPORTZEIGERFELD_INDEX_FUER_GLOBAL_SHARED_MEMORY_0	gehoert zu  NAME_GLOBAL_SHARED_MEMORY_0	 (was in device_interface.h als "SEEM"
//																definiert ist. "SEEM" ist der Name des Global-Shared-Memroy-(Segmentes)
// GPIO_PORT1 arbeit also mit "SEEM"
#define GPIO_PORT1_EXPORTZEIGERFELD_INDEX	EXPORTZEIGERFELD_INDEX_FUER_GLOBAL_SHARED_MEMORY_0 


// 2b.)
// Definition der Device-Struktur die als Teil der Struktur fuer das Lokale-Shared-Memory und das Globale-Shared-Memory
// verwendet wird.
typedef struct
{
    unsigned short GPIO_PORT1_RegIN_str;		//Exemplary Input Register 0
    unsigned short GPIO_PORT1_RegOUT_str;		//Exemplary Output Register 0
    unsigned short GPIO_PORT1_RegDIR_str;		//Exemplary Data Direction Register 0
    unsigned short GPIO_PORT1_RegPUL_str;       //Exemplary Pull Up Control Register 0
    unsigned short GPIO_PORT1_RegIE_str;       //Exemplary Interrupt Mask Register 0
    unsigned short GPIO_PORT1_RegIF_str;       //Exemplary Interrupt Flag Register 0
}STRC_GPIO;



// 3a.)
// Export von Funktionen, da diese in der Datei device_interface.c in die "Zeiger-Felder" eingetragen werden, ...
// ... bzw (im Falle von init_gio0()) in der Funktion reset_simulation_function() aufgerufen werden
extern unsigned short	in_GPIO_PORT1(unsigned short registerAdr);
extern void				out_GPIO_PORT1(unsigned short registerAdr, unsigned short value);
extern void				out8_GPIO_PORT1(unsigned short registerAdr, unsigned char value);
extern void				ever_out_GPIO_PORT1(unsigned short registerAdr, unsigned short value);
extern void				GPIO_PORT1_Simulation();
extern void				sm_get_GPIO_PORT1Reg(unsigned short eigeneProzessNummer);
extern unsigned char	sm_set_GPIO_PORT1Reg(unsigned short eigeneProzessNummer);
extern unsigned char	init_GPIO_PORT1();

// 3b.) Export weiterer Flag
extern unsigned char GPIO_PORT1_An_Aus; // Wird von dem "Device" Steuerung veraendert => muss exportiert werden



// 4.)
// Export weiterer (eigentlich) "Dinge", die nicht nur intern (hier) sondern auch extern bei der UI benoetigt werden.

void setNewGPIO_PORT1_RegDIR(unsigned short value);	//Hilfsfunktion um automatische Einstellungen zu uebernehmen
#define __GPIO_PORT1_RegDIR_out(value)	setNewGPIO_PORT1_RegDIR((unsigned short)value)
#define __GPIO_PORT1_RegDIR_in			local_Shared_Mem_Copy.gpio_shared.GPIO_PORT1_RegDIR_str

void setNewGPIO_PORT1_RegIN(unsigned short value);	//Hilfsfunktion um automatische Einstellungen zu uebernehmen
#define __GPIO_PORT1_RegIN_out(value)	setNewGPIO_PORT1_RegIN((unsigned short)value)
#define __GPIO_PORT1_RegIN_in			local_Shared_Mem_Copy.gpio_shared.GPIO_PORT1_RegIN_str

void setNewGPIO_PORT1_RegOUT(unsigned short value);
#define __GPIO_PORT1_RegOUT_out(value)	setNewGPIO_PORT1_RegOUT((unsigned short)value)      //local_Shared_Mem_Copy.gpio_shared.GPIO_PORT1_RegOUT_str = (__GPIO_PORT1_RegDIR_in & (unsigned short)value)  //verunded mit dem Data Direction Register, so ist das beschreiben nur bei gesetztem Ausgang moeglich
#define __GPIO_PORT1_RegOUT_in			local_Shared_Mem_Copy.gpio_shared.GPIO_PORT1_RegOUT_str

void setNewGPIO_PORT1_RegPUL(unsigned short value);
#define __GPIO_PORT1_RegPUL_out(value)	setNewGPIO_PORT1_RegPUL((unsigned short)value)
#define __GPIO_PORT1_RegPUL_in			local_Shared_Mem_Copy.gpio_shared.GPIO_PORT1_RegPUL_str

#define __GPIO_PORT1_RegIE_out(value)	local_Shared_Mem_Copy.gpio_shared.GPIO_PORT1_RegIE_str = (unsigned short)value
#define __GPIO_PORT1_RegIE_in			local_Shared_Mem_Copy.gpio_shared.GPIO_PORT1_RegIE_str

#define __GPIO_PORT1_RegIF_out(value)	local_Shared_Mem_Copy.gpio_shared.GPIO_PORT1_RegIF_str = (unsigned short)value
#define __GPIO_PORT1_RegIF_in			local_Shared_Mem_Copy.gpio_shared.GPIO_PORT1_RegIF_str


extern void* gpio_Mutex; 
#define __gpio_Mutex_lock				LockMutexOs (gpio_Mutex) 
#define __gpio_Mutex_release			UnlockMutexOs (gpio_Mutex) 

extern unsigned char ctr_gpio_flag;	
#define	__ctr_gpio_flag_out(value)		ctr_gpio_flag = (unsigned char)value
#define	__ctr_gpio_flag_in				ctr_gpio_flag


#ifdef  __cplusplus
}
#endif

#endif //GPIO_PORT1_H