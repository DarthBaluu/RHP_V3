/*************************************************************************************************************//**
 *	SEEM
 *	port_extension.h
 *
 *	Die port_extension dient als Erweiterung des Windows-Ports von FRTOS und enthaelt die fuer die Simulation
 *	benoetigten hinzugefuegten Komponenten (z.B. eiok_sleep, simulations_Thread) des Ports.
 *
 *	Ebenfalls wurden bereits existierende Teile des Ports hierher umgelegt, um sie in anderen Bereichen der
 *	Simulation benutzen zu koennen. (z.B. vPortSetInterruptHandler)
 *	
 *	Author: Ma Kroe / LT
 *
 *	zuletzt geaendert:
 *	20.06.14
 *
 ****************************************************************************************************************/

#ifndef PORT_EXTENSION_H
#define PORT_EXTENSION_H


#include "FreeRTOS.h"
#include "os_support.h"


#ifdef  __cplusplus
extern "C" {
#endif

//Einstellungen für den eiok_sleep-Aufruf in der eiok_SimulatedPeripheral_TickTimer
//DeltaSleep
#define MS_TICKTIMER_SLEEPS_BETWEEN_PROCESSTIME_CHECK				1		// ms in Echtzeit die sich der eiok_sleep schlafen legt zwischen den Prozesszeit-Überprüfungen
//TotalDuration
#define MS_OF_PROCESSTIME_BETWEEN_TICKINTERRUPTS					( portTICK_RATE_MS )	/*!< portTICK_RATE_MS = portTICK_PERIOD_MS = ( ( TickType_t ) 1000 / configTICK_RATE_HZ ),
																								 configTICK_RATE_HZ in FreeRTOSconfig.h = 1000 */

//Einstellungen für den eiok_sleep-Aufruf im simulations_Thread
//DeltaSleep
#define MS_SIMULATION_SLEEPS_BETWEEN_PROCESSTIME_CHECK				1		// ms in Echtzeit die sich der eiok_sleep schlafen legt zwischen den Prozesszeit-Überprüfungen
//TotalDuration
#define MS_OF_PROCESSTIME_BETWEEN_SIMULATION_ACTUALISATION			1		// ms in Simulated_Time (Prozesszeit) die (mind.) vergehen bis der Zaehlstand der Timers erhoeht wird.


/*!
*	Das Interrupt-Flag-Register von FreeRTOS, jedes Bit einen Interrupt darstellt, es sind z.Z. eine maximale Anzahl von 32 Interrupts zulaessig
*	Ueber das eiok_int_InterruptFlagRegister erkennt der FRTOS-Interrupt-Controller welche Interrupts aufgetreten sind und arbeitet sie ab.
*/
extern volatile unsigned int eiok_int_InterruptFlagRegister;

/*!
* Flags fuer die Verwaltung des Freeze 
*/
/*Flag mit dem das gesamte System (also der Simulations-Thread) und der (FreeRTOS-Sceduler-Thread) angehalten 
(d.h. jeweils in eine Wartschleife des Nichtstuens versetzt) werden kann. */
extern unsigned char FreezeSystem;




/*!
*	Thread welcher die zu simulierenden Komponenten des exemplarischen Mikrocontrollers aufnehmen soll.
*	Die Komponenten werden ueber den eiok_sleep() in ihrer Zeitbasis gesteuert und werden nacheinander
*	ausgefuehrt.
*/
//extern DWORD WINAPI simulation_Thread( LPVOID lpParameter );
extern uint32_os PREFIX_API simulation_Thread( void_far_pointer_os lpParameter );

/*
 * Raise a simulated interrupt represented by the bit mask in ulInterruptMask.
 * Each bit can be used to represent an individual interrupt - with the first
 * two bits being used for the Yield and Tick interrupts respectively.
*/
extern void vPortGenerateSimulatedInterrupt( uint32_os ulInterruptNumber );

/*
 * Install an interrupt handler to be called by the simulated interrupt handler
 * thread.  The interrupt number must be above any used by the kernel itself
 * (at the time of writing the kernel was using interrupt numbers 0, 1, and 2
 * as defined above).  The number must also be lower than 32.
 *
 * Interrupt handler functions must return a non-zero value if executing the
 * handler resulted in a task switch being required.
 */
extern void vPortSetInterruptHandler( uint32_os ulInterruptNumber, uint32_os (*pvHandler)( void ) );



//######################## Interface to the simulation-functions ###################################################
#define MAX_COUNT_SIMULATIONSFUNCTIONS 100

/* Shows the used processortime in ms */
extern unsigned long int  delta_ms; 


/* Global variable with simulated system time that can be debugged from every-where */
extern unsigned long int simtime;


/*	Simulation-Function-Array.
	Array that contians all pointers to the simulation functions which are called in simulation_Thread() 
	Array-Entries which are not use must contain the value NULL.
	The Array is analyzed starting at index 0 up to the first NULL-value
*/
extern void (*array_with_pointer_to_simumationsfunctions[MAX_COUNT_SIMULATIONSFUNCTIONS])(); 

/*
*	Function that simulates the processors start-up-reset. Must be used by the "developer" of the simulation-functions
*	for initialisation of the Simulation-Function-Array
*/
extern void reset_simulation_function();
// ##################################################################################################################




#ifdef  __cplusplus
}
#endif

#endif //PORT_EXTENSION_H