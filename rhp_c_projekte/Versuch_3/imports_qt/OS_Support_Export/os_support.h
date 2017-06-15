#ifndef PORT_OS_SUPPORT_H
#define PORT_OS_SUPPORT_H


#define		FOR_WINDOWS
//#define		FOR_LINUX

#ifdef FOR_WINDOWS

	#include <Windows.h>

	typedef unsigned long int		uint32_os;
	typedef unsigned short			uint16_os;
	typedef void far				*void_far_pointer_os;
	typedef int						bool_os;


	#define PREFIX_STANDARD_CALL			__stdcall
	#define PREFIX_API						WINAPI

#endif //  FOR_WINDOWS



#ifdef FOR_LINUX


	typedef unsigned long int		uint32_os;
	typedef unsigned short			uint16_os;
	typedef signed char				int8_os;
	typedef void* 					void_far_pointer_os;
	typedef int						bool_os;


	#define PREFIX_STANDARD_CALL	
	#define PREFIX_API			

#endif //  FOR_LINUX


typedef enum {idle_pri, low_pri, normal_pri, high_pri} threadpriority;   

unsigned long GetMinimumTimeSlot();


/*!
*	eiok_sleep wartet bis eine gewisse Prozesszeit(TotalDuration) vergangen ist. Die Funktion ueberprueft in einer Schleife
*	die verbrauchte Prozesszeit und wartet zwischen den Überprüfungen eine angegebene Zeit(Deltasleep).
*
*	Die Idee hierbei ist, dass nicht mit der Windows-Systemzeit (quasi Echtzeit) alle z.B. 200 ms der Scheduler aufgerufen wird
*	sondern abhaengig von der von dieser Gesamtanwendung (d.h. inkl. all ihrer Threads) benutzten Zeit (Usertime). 
*	Diese Usertime bleibt (im Gegensatz zur Echtzeit naemlich) stehen wenn die Ausfuehrung des Programms 
*	(z.B. bei einem Breakpoint) angehalten wird.
*	Ebenso wenn die Gesamtanwendung durch andere (parallel laufende) Anwendungen ausgebremst wird. 
*
*
*	@param TotalDuration	unsigned long welche die Zeit angibt, die an Prozesszeit gewartet wird
*	@param DeltaSleep		unsigned long welche die Zeit angibt, die zwischen den Überprüfungen der Prozesszeit gewartet wird
*
*	@return		unsigned long gibt die verstrichene Prozesszeit zurueck
*	
*/
extern void SleepOs(uint32_os ms);
extern unsigned long eiok_sleep(unsigned long TotalDuration, unsigned long DeltaSleep, unsigned long* time_to_be_updated);


extern void* CreateThreadOs( void* address_of_start_function, void* parameter_list );
extern void  SuspendThreadOs( void* thread );
extern void  SetThreadPriorityOs( void* thread,  threadpriority prio);
extern void* GetCurrentThreadOs();
extern void  ResumeThreadOs(void* thread);
extern void  TerminateThreadOs(void* thread);
extern void  CloseThreadHandleOs(void* thread);
extern void  ExitThreadOs();

extern void* CreateMutexOs();
extern void  LockMutexOs(void* mutex);
extern void  UnlockMutexOs(void* mutex);
extern void  DestroyMutexOs(void* mutex);

extern void* CreateEventFlagOs();
extern void  SetEventFlagOs (void* EventFlag );
extern void  WaitForEventFlagOs (void* EventFlag);


#endif //PORT_OS_SUPPORT_H
