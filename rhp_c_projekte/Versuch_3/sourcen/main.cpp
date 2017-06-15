
#include <QtWidgets/QApplication>
#include "qthread.h"
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "../../Gemeinsam_genutzte_Sourcen/qt_sharedmemory.h"
#include "device_interface.h"
#include "user_conf.h"


#pragma comment(lib,"../imports/OS_Support_Export/Debug/OS_Support_lib_sta.lib")
#pragma comment(lib,"../imports/Free_RTOS_Export/Debug/Free_RTOS_Kern_lib_sta.lib")
#pragma comment(lib,"../imports/SIM_IO_Export/Debug/Sim_IO_Kern_lib_sta.lib")



/*!
*	Einbinden der C-Funktion frtosemain aus der emain.c, damit sie von hier aus aufgerufen werden kann.
*/
extern "C" {
#ifdef USER_PROG_1
	void USER_PROG_1(void* arg); 
#endif
#ifdef USER_PROG_2
	void USER_PROG_2(void* arg); 
#endif

	// Zweite FRTOS-Task als Beispiel
	// Kann so nach emain() verschoeben werden
	void task0(void* arg)
	{
		int i;
        //char* dummy; dummy=(char*) arg;

		while (1) {
			for(i=0; i<100000; i++) { }; 
			Sleep(30);		//30
			vTaskDelay(6); //6
		}
	}
}



int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);       //CHANGED_FOR_QT
    QCoreApplication a(argc, argv);

	// Instanziieren und starten eien Qt-Shared-Memory-Threads
	// Dieser Thread hier erhaelt die Prozessnumme PROZESS1
	// Das Shared-Memory ist (bisher ungetestet) so ausgelegt, dass mehrere Instanzen (Thread, Prozesse) von der gleichen Seite
	// darauf zu greifen koennen und sich gegenseitig ueber 
	
	QThread* kommunikations_thread = new QThread;								/**< erstellen eines QThreads */

#ifdef OLD
    // Die Uebergabe von konstanten Springs bewirkt unter Qt einen Warnung
    Eiok_SharedMemory* sharedMem_Object = new Eiok_SharedMemory(NAME_GLOBAL_SHARED_MEMORY_0	 ,PROZESS1, EXPORTZEIGERFELD_INDEX_FUER_GLOBAL_SHARED_MEMORY_0);		/**< erstellen eines Objekts der Eiok_SharedMemory Klasse */
#else
    // Deshalb nun unter Nutzung von buffer
    Eiok_SharedMemory* sharedMem_Object;
    {
        char buffer[100];
        sprintf(buffer,"%s",NAME_GLOBAL_SHARED_MEMORY_0);
        sharedMem_Object = new Eiok_SharedMemory(buffer	 ,PROZESS1, EXPORTZEIGERFELD_INDEX_FUER_GLOBAL_SHARED_MEMORY_0);		/**< erstellen eines Objekts der Eiok_SharedMemory Klasse */

    }
#endif //OLD



    sharedMem_Object->moveToThread(kommunikations_thread);						/**< verschieben dieses Objekts in den erstellten Thread */
	QObject::connect(kommunikations_thread, SIGNAL(started()), sharedMem_Object, SLOT( Kommunikationsdauerlauefer_Simulationsseitig() ));
	QObject::connect(kommunikations_thread, SIGNAL(finished()), kommunikations_thread, SLOT(deleteLater()));
	kommunikations_thread->start();												/**< starten des Threads */

	/* Eine "unsichtbare" FRTOS-TASK die eine Grundlast erzeugt */
	static unsigned char Param_Task_0=0;
	xTaskHandle Handle_Task_0;
	xTaskCreate(task0, ((const char *) "task0"), 128, &Param_Task_0, tskIDLE_PRIORITY, &Handle_Task_0);

#ifdef USER_PROG_1
	/* FRTOS-TASK user_prog_1() fuer den Studierenden-Bereich erstellen, es wird (in der Regel) die Funktion emain im Task gestartet */
	static unsigned char Param_Task_1=1;
	xTaskHandle Handle_Task_1;
	xTaskCreate(USER_PROG_1, ((const char *) "USER_PROG_1"), 128, &Param_Task_1, tskIDLE_PRIORITY, &Handle_Task_1);
#endif	


#ifdef USER_PROG_2
	/* FRTOS-TASK user_prog_2() fuer den Studierenden-Bereich erstellen, es wird z.B. emain_sender() im Task gestartet */
	static unsigned char Param_Task_2=2;
	xTaskHandle Handle_Task_2;
	xTaskCreate(USER_PROG_2, ((const char *) "USER_PROG_2"), 128, &Param_Task_2, tskIDLE_PRIORITY, &Handle_Task_2);
#endif	

	//Aufruf (Start) aller FRTOS-TASKs
	vTaskStartScheduler();

	return a.exec();
}



