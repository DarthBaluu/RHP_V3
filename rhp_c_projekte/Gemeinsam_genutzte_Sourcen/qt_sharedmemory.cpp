/*************************************************************************************************************//**
 *	SEEM
 *	eiok_sharedMemory.cpp
 *
 *	Die sharedMemory.cpp enthält alle fuer die Kommunikation mit dem Shared Memory noetigen Funktionen.
 *	
 *	Es liegen hier alle Getter- und Setter-Funktionen vor, die fuer die Exemplarischen Komponenten gebraucht werden.
 *	Zur Erweiterung müssen hier einfach die benoetigten Funtionen hinzugefuegt werden und in die 
 *	kommunikation-Funktion eingefügt werden.
 *
 *	Im Kommunikations-Thread wird die Funktion kommunikation gestartet und ueberprueft in einer Schleife ob 
 *	Aenderungen aus dem Shared Memory geholt werden oder ob lokale Aktualisierungen im eigenen Prozess in das
 *	Shared Memory geschrieben werden muessen.
 *
 *	Authors: Lo Thie, Ma Kroe
 *
 *	zuletzt geaendert:
 *	01.04.15
 *
 ****************************************************************************************************************/
#include "qt_sharedMemory.h"
#include "device_interface.h"


//Definition der Größe des Shared Memory Segments ueber die Groesse der globalen structur der gesamten exemplarischen Komponenten
#define memorySize				(int)sizeof(local_Shared_Mem_Copy)


/*!
*	Konstruktor der Klasse Eiok_SharedMemory.
*	Im Konstruktor wird das Shared Memory Segment mit dem Keyword "SEEM" angelegt.
*/
Eiok_SharedMemory::Eiok_SharedMemory(char *NamensString, unsigned short ProzessNummer, unsigned short index_exportzeigerfeld) : sharedMem_Segment(NamensString)
{
	eigeneProzessNummer = ProzessNummer;
	fuer_exportzeigerfeld_benutzer_index = index_exportzeigerfeld;

	//anlegen des Shared Memory Segments mit der oben definierten Groesse memorySize
	if (!sharedMem_Segment.create(memorySize))
	{
		//falls das Segment nicht angelegt werden konnte ueberpruefen ob es schon vorhanden ist
		//x == 0 compare return true
        int x = sharedMem_Segment.errorString().compare("QSharedMemory::create: already exists");
        if(x != 0)
		{
			//falls noch nicht vorhanden: Fehlerausgabe
            qDebug() << sharedMem_Segment.errorString();
            return;

        }else if(!sharedMem_Segment.attach())		//wenn Segment schon existiert an den Prozess binden
		{
			//wenn binden fehlschlaegt: Fehlerausgabe
			qDebug() << sharedMem_Segment.errorString();
			return;

		}else{
			//wenn Segment existiert und eingebunden werden konnte den Pointer auf das Segment holen
			pointerMemInit();
		}
    }else{
		//konnte das Segment angelegt werden, die structur der exemplarischen Komponenten in das Segment schreiben
        setMemInit();
    }

}


//Destruktor
Eiok_SharedMemory::~Eiok_SharedMemory(void)
{
	sharedMem_Segment.detach();
}


/*!
*	Beschreiben des erstellten Shared Memory Segments mit der Stucture der exemplarischen Komponenten (local_Shared_Mem_Copy)
*/
void Eiok_SharedMemory::setMemInit()	
{
	pointer_ShMem_Seg = NULL;
	//Shared Memory Semaphore holen
    sharedMem_Segment.lock();
	//Pointer auf das Shared Memory Segment holen
    char *to = (char*)sharedMem_Segment.data();
	//Pointer auf die lokale Struktur der exemplarischen Komponenten
	const char *from = (const char*)&local_Shared_Mem_Copy;
	//kopieren der Struktur in das Shared Memory mit der oben definierten Groesse (memorySize)
	memcpy(to, from, qMin(sharedMem_Segment.size(), memorySize));
	//Freigeben der Semaphore des Shared Memory
    sharedMem_Segment.unlock();
	//erstellen des Pointers auf die Struktur im Shared Memory ueber die Funktion pointerMemInit()
    pointerMemInit();
}

/*!
*	Erstellen eines Pointers auf die Struktur der exemplarischen Komponenten im Shared Memory.
*	So kann direkt auf einzelne Komponenten im Shared Memory zugegriffen werdem.
*	Von setMemInit() getrennt, da setMemInit bei schon bestehendem Shared Memory Segment nicht benötigt
*	wird.
*/
void Eiok_SharedMemory::pointerMemInit()
{
    void *memPointer = NULL;

	//eigeneProzessNummer = PROZESS1; // jetzt im Konstruktor
	//Shared Memory Semaphore holen
    sharedMem_Segment.lock();
	//Pointer des Shared Memory Segments in Hilfsvariable zwischenspeichern (ohne hat nicht funktioniert)
    memPointer = (void*)sharedMem_Segment.constData();
	//Umwandeln der Hilfsvariable in einen Pointer der Datenstruktur der exemplarischen Komponenten
	//so kann auf die direkten Speicherpositionen der einzelnen Variablen zugegriffen werden.
    pointer_ShMem_Seg = (STRC_SHARED_MEM*)memPointer;

	global_pointer_ShMem_Seg[fuer_exportzeigerfeld_benutzer_index] = pointer_ShMem_Seg;	// Auch einen globalen Zeiger darauf biegen, so dass auch ausserhalb der Klasse (Eiok_SharedMemory) 
																			// also aus plain-C heraus auf das Shared memory zugegriffen werden kann.
	//Freigeben der Semaphore des Shared Memory
    sharedMem_Segment.unlock();
	
}




/*!
*	Funktion die vom Kommunikations-Thread in Schleife ausgefuehrt wird.
*	Ueberprueft durch Pollen ob eine Aenderung durch die beiden anderen Prozesse im Shared Memory vorgenommen wurde
*	oder uebertraegt lokale Aktualisierungen ins Shared Memory.
*/
// Diese Methode kommt nur auf der Seite des Shared-Memory zum Einsatz auf der auch 
// die Simulation und die Studierenden-Applikation laueft.
void Eiok_SharedMemory::Kommunikationsdauerlauefer_Simulationsseitig()
{
	unsigned long int shMem_cntr = 0;
    unsigned short int i;
	unsigned long int flag_sum = 0;
	unsigned long int flag;

	while(1){ 
		if(reset_simulation_function_was_executed == 1){ //wait for all needed initialisations done in reset_simulation_function()
				/*!
				*	auslesen des Prozess Kontroll Flags aus dem Shared Memory
				*	Segment durch Semaphore geschuetzt.
				*/
				sharedMem_Segment.lock();
				__ctr_Prozess_out(eigeneProzessNummer, pointer_ShMem_Seg->ctr_Prozess[eigeneProzessNummer-1]);
				sharedMem_Segment.unlock();  

				//Kontroll Flags des Prozesses in Kontrollvariable abspeichern
				shMem_cntr = __ctr_Prozess_in(eigeneProzessNummer);
			
				/*!
				*	Durch die Reihenfolge der Ueberpruefung, dass erst nach neuen Daten im Shared Memory Segment
				*	geprüft wird und erst dann lokale Aenderungen geschrieben werden,
				*	wird verhindert das neue Daten von anderen Prozessen im Shared Memory ueberschrieben werden.
				*	Da allerdings bei einer Aktualisierung der globalen Parameter aus dem Shared Memory die eventuell
				*	vorliegenden lokalen Aenderungen ueberschrieben werden, kann es so zu einem Verlust kommen.
				*	Hier muss ueberlegt werden welche Prioritaet der eigene Prozess haben soll.
				*
				*	Durch das zuruecksetzen des lokalen kontroll Flags bei Auslesen aus dem Shared Memory wird ein rueckschreiben
				*	des gerade geholten Wertes verhindert, falls vorher eine lokale Aktualisierung vorlag.
				*/
				// Ueberpruefung ob in den anderen Prozessen einzelne Komponenten geaendert wurden
				// und gegebenenfalls das Auslesen der dementsprechenden Shared Memory Bereiches.
				// Die Funktionen zum Auslesen des Shared-Memory-Bereiches werden ueber Zeiger im Feld 
				// array_with_shared_memory_getter_functions aufgerufen.
				// Für jedes Bit eine Funktion (oder keine Funktion, dann NULL-Zeiger im Feld)
			
                for (i=0; i< ((unsigned short int) (MAX_COUNT_SHARED_MEMORY_CONTROL_BITS)); i++) {
					if(   ((shMem_cntr>>i)&0x1 ) && (array_with_shared_memory_getter_functions[i] != NULL) )
					{
						//Kontroll-Bit ist gesetzt und die entsprechende Funktion zum Auslesen existiert => dann die Funktion ausfuehren
						sharedMem_Segment.lock();
						array_with_shared_memory_getter_functions[i](eigeneProzessNummer);
						sharedMem_Segment.unlock();
					}
				}

				// Alle vorhandenen Setter-Funktionen aufrufen.
				// Jede Setter-Funktion ueberprueft anhand "seines" Flags ob dies sich geaendert hat und ob
				// dann etwas ins Shared Memory kopiert werden musss.
                for (i=0; i< ((unsigned short int) (MAX_COUNT_LOCAL_CONTROL_FLAGS)); i++) {
					if(   (array_with_shared_memory_setter_functions[i] != NULL) )
					{
						sharedMem_Segment.lock();
						array_with_shared_memory_setter_functions[i](eigeneProzessNummer);
						sharedMem_Segment.unlock();
					}
				}

				//--> GLOBALES_SYNHCRONISATIONS_KONZEPT
				// Warten bis alle anderen  am Global-Shared-memory "angeschlossenen"  Prozesse 
				// gemdeldet haben, dass sie die Aenderungen (die durch die oben aufgerufenen Setter-Funktionen bewirkt wurden)
				// "mitbekommen" (quittiert) haben.
				do{							
					flag_sum=0;
					for(i = 1; i <= PROZESSES_IN_WORK; i++)
					{
						if(i!=eigeneProzessNummer) {
							sharedMem_Segment.lock();
							flag =  global_pointer_ShMem_Seg[PATTERN_EXPORTZEIGERFELD_INDEX]->ctr_Prozess[i-1]; //PING2_EMPFANGEN plural
							sharedMem_Segment.unlock();
							flag_sum = flag_sum + flag;
						}
					}
				} while (flag_sum!=0);

				global_shared_memory_sync_counter++; // Wer auf eine Synchronisation wartet, der fragt diesen Counter einfach initial ab
								// und warten dann darauf, dass dessen Wert sich aendert.
								// Bei einem unsigned long int counter duerfte es wohl nicht vorkommen, dass
								// bei diesem Pollen ein Sync uebersehen wird weil der counter genau
								// 2hoch32 mal erhoeht wurde.
								// Sinnvoll ist es hierfuer eine Funktion wait_for_gsms() (GloabalSharedMemorySynchronisation) zu schreiben.
								// Diese kann dann in den out()-Funktionen ganz am Ende aufgerufen werden.
								// PONG2_SENDEN plural
				//<--
	
			}//if(...

			//kleine Wartezeit um die CPU nicht voll auszulasten
			SleepOs(50); //Sleep(50);

	}//while(1)
}//shared_Mem_Prozess_kommunikation()








/*!
*	Funktion die vom Kommunikations-Thread in Schleife ausgefuehrt wird.
*	Ueberprueft durch Pollen ob eine Aenderung durch die beiden anderen Prozesse im Shared Memory vorgenommen wurde
*	oder uebertraegt lokale Aktualisierungen ins Shared Memory.
*/
// Diese Methode kommt nur auf der Seite des Shared-Memory zum Einsatz auf der auch 
// das User-Interface laueft.
int kommSchleife = 1;	// Wird benoetigt, damit die "Endlausschleife" bei Beendigung der UI-App 
						// geregelt unterbrochen werden kann.
void Eiok_SharedMemory::Kommunikationsdauerlauefer_Userinterfaceseitig()
{
    emit signal_for_ctr_flag_0();
    emit signal_for_ctr_flag_1();
	emit signal_for_ctr_flag_2();
    emit signal_for_ctr_flag_3();
	emit signal_for_ctr_flag_4(); // ACHTUNG: Die meisten dieser Signale sind in der Regel mit keinem Slot verbunden...
	emit signal_for_ctr_flag_5(); // ... => ToDo: Klaerung ob dies zu Speicherfressern oder anderen Nebeneffekten fuehrt
	emit signal_for_ctr_flag_6();
	emit signal_for_ctr_flag_7();
	emit signal_for_ctr_flag_8();
	emit signal_for_ctr_flag_9();
	emit signal_for_ctr_flag_10();
	emit signal_for_ctr_flag_11();
	emit signal_for_ctr_flag_12();
	emit signal_for_ctr_flag_13();
	emit signal_for_ctr_flag_14();
	emit signal_for_ctr_flag_15();
	emit signal_for_ctr_flag_16();
	emit signal_for_ctr_flag_17();
	emit signal_for_ctr_flag_18();
	emit signal_for_ctr_flag_19();
	emit signal_for_ctr_flag_20();
	emit signal_for_ctr_flag_21();
	emit signal_for_ctr_flag_22();
	emit signal_for_ctr_flag_23();
	emit signal_for_ctr_flag_24();
	emit signal_for_ctr_flag_25();
	emit signal_for_ctr_flag_26();
	emit signal_for_ctr_flag_27();
	emit signal_for_ctr_flag_28();
	emit signal_for_ctr_flag_29();
	emit signal_for_ctr_flag_30();
	emit signal_for_ctr_flag_31();	//MAX_COUNT_SHARED_MEMORY_CONTROL_BITS_Changes

    unsigned long int shMem_cntr = 0;
    unsigned short int i;
	unsigned char ret;


    while(!kommSchleife==0){
            /*!
            *	auslesen des Prozess Kontroll Flags aus dem Shared Memory
            *	Segment durch Semaphore geschuetzt.
            */
            sharedMem_Segment.lock();
            __ctr_Prozess_out(eigeneProzessNummer, pointer_ShMem_Seg->ctr_Prozess[eigeneProzessNummer-1]);
            sharedMem_Segment.unlock();

            //Kontroll Flags des Prozesses in Kontrollvariable abspeichern
            shMem_cntr = __ctr_Prozess_in(eigeneProzessNummer);

			/*!
				*	Durch die Reihenfolge der Ueberpruefung, dass erst nach neuen Daten im Shared Memory Segment
				*	geprüft wird und erst dann lokale Aenderungen geschrieben werden,
				*	wird verhindert das neue Daten von anderen Prozessen im Shared Memory ueberschrieben werden.
				*	Da allerdings bei einer Aktualisierung der globalen Parameter aus dem Shared Memory die eventuell
				*	vorliegenden lokalen Aenderungen ueberschrieben werden, kann es so zu einem Verlust kommen.
				*	Hier muss ueberlegt werden welche Prioritaet der eigene Prozess haben soll.
				*
				*	Durch das zuruecksetzen des lokalen kontroll Flags bei Auslesen aus dem Shared Memory wird ein rueckschreiben
				*	des gerade geholten Wertes verhindert, falls vorher eine lokale Aktualisierung vorlag.
			*/
			// Ueberpruefung ob in den anderen Prozessen einzelne Komponenten geaendert wurden
			// und gegebenenfalls das Auslesen der dementsprechenden Shared Memory Bereiches.
			// Die Funktionen zum Auslesen des Shared-Memory-Bereiches werden ueber Zeiger im Feld 
			// array_with_shared_memory_getter_functions aufgerufen.
			// Für jedes Bit eine Funktion (oder keine Funktion, dann NULL-Zeiger im Feld)
			for (i=0; i< MAX_COUNT_SHARED_MEMORY_CONTROL_BITS; i++) {
				if(   ((shMem_cntr>>i)&0x1 ) && (array_with_shared_memory_getter_functions[i] != NULL) )
				{
					//Kontroll-Bit ist gesetzt und die entsprechende Funktion zum Auslesen existiert => dann die Funktion ausfuehren
					sharedMem_Segment.lock();
					array_with_shared_memory_getter_functions[i](eigeneProzessNummer);
					sharedMem_Segment.unlock();
					switch (i) {
							case 0:		emit	signal_for_ctr_flag_0();		break;
							case 1:		emit	signal_for_ctr_flag_1();		break;
							case 2:		emit	signal_for_ctr_flag_2();		break;
							case 3:		emit	signal_for_ctr_flag_3();		break;
							case 4:		emit	signal_for_ctr_flag_4();		break; 
							case 5:		emit	signal_for_ctr_flag_5();		break;
							case 6:		emit	signal_for_ctr_flag_6();		break;
							case 7:		emit	signal_for_ctr_flag_7();		break; 							
							case 8:		emit	signal_for_ctr_flag_8();		break;
							case 9:		emit	signal_for_ctr_flag_9();		break;
							case 10:	emit	signal_for_ctr_flag_10();		break;
							case 11:	emit	signal_for_ctr_flag_11();		break;
							case 12:	emit	signal_for_ctr_flag_12();		break;
							case 13:	emit	signal_for_ctr_flag_13();		break;
							case 14:	emit	signal_for_ctr_flag_14();		break;
							case 15:	emit	signal_for_ctr_flag_15();		break;
							case 16:	emit	signal_for_ctr_flag_16();		break;
							case 17:	emit	signal_for_ctr_flag_17();		break;
							case 18:	emit	signal_for_ctr_flag_18();		break;
							case 19:	emit	signal_for_ctr_flag_19();		break;
							case 20:	emit	signal_for_ctr_flag_20();		break;
							case 21:	emit	signal_for_ctr_flag_21();		break;
							case 22:	emit	signal_for_ctr_flag_22();		break;
							case 23:	emit	signal_for_ctr_flag_23();		break;
							case 24:	emit	signal_for_ctr_flag_24();		break;
							case 25:	emit	signal_for_ctr_flag_25();		break;
							case 26:	emit	signal_for_ctr_flag_26();		break;
							case 27:	emit	signal_for_ctr_flag_27();		break;
							case 28:	emit	signal_for_ctr_flag_28();		break;
							case 29:	emit	signal_for_ctr_flag_29();		break;
							case 30:	emit	signal_for_ctr_flag_30();		break;
							case 31:	emit	signal_for_ctr_flag_31();		break; //MAX_COUNT_SHARED_MEMORY_CONTROL_BITS_Changes
							default: ;
					}

				}
			}


			// Alle vorhandenen Setter-Funktionen aufrufen.
			// Jede Setter-Funktion ueberprueft anhand "seines" Flags ob dies sich geaendert hat und ob
			// dann etwas ins Shared Memory kopiert werden musss.
			for (i=0; i< MAX_COUNT_LOCAL_CONTROL_FLAGS; i++) {
				if(   (array_with_shared_memory_setter_functions[i] != NULL) )
				{
					sharedMem_Segment.lock();	// ACHTUNG: In der BA-Version wird erst __control_Mutex_lock; und danch sharedMem_Segment.lock(); ausgefuehrt
												// ToDo: Klaerung mit Hern Kroenke warum dies im UI-Projekt anders herumg ist
					ret=array_with_shared_memory_setter_functions[i](eigeneProzessNummer);
					sharedMem_Segment.unlock();
					if (ret == 1) {
						switch (i) {
							case 0:			emit	setText(SETTEXT0);		break;
							case 1:			emit	setText(SETTEXT1);		break;
							case 2:			emit	setText(SETTEXT2);		break;
							case 3:			emit	setText(SETTEXT3);		break;
							case 4:			emit	setText(SETTEXT4);		break;
							case 5:			emit	setText(SETTEXT5);		break;
							case 6:			emit	setText(SETTEXT6);		break;	
							case 7:			emit	setText(SETTEXT7);		break;
							case 8:			emit	setText(SETTEXT8);		break;
							case 9:			emit	setText(SETTEXT9);		break;
							case 10:		emit	setText(SETTEXT10);		break;
							case 11:		emit	setText(SETTEXT11);		break;
							case 12:		emit	setText(SETTEXT12);		break;
							case 13:		emit	setText(SETTEXT13);		break;
							case 14:		emit	setText(SETTEXT14);		break;	
							case 15:		emit	setText(SETTEXT15);		break;
							case 16:		emit	setText(SETTEXT16);		break;
							case 17:		emit	setText(SETTEXT17);		break;
							case 18:		emit	setText(SETTEXT18);		break;
							case 19:		emit	setText(SETTEXT19);		break;
							case 20:		emit	setText(SETTEXT20);		break;	
							case 21:		emit	setText(SETTEXT21);		break;
							case 22:		emit	setText(SETTEXT22);		break;
							case 23:		emit	setText(SETTEXT23);		break;
							case 24:		emit	setText(SETTEXT24);		break;
							case 25:		emit	setText(SETTEXT25);		break;
							case 26:		emit	setText(SETTEXT26);		break;	
							case 27:		emit	setText(SETTEXT27);		break;
							case 28:		emit	setText(SETTEXT28);		break;
							case 29:		emit	setText(SETTEXT29);		break;
							case 30:		emit	setText(SETTEXT30);		break;
							case 31:		emit	setText(SETTEXT31);		break; //MAX_COUNT_SHARED_MEMORY_CONTROL_BITS_Changes
							default: ;
						}
					}

				}
			}

            //kleine Wartezeit um die CPU nicht voll auszulasten
            SleepOs(1); //Sleep(1);
    }//while
    sharedMem_Segment.detach();
}//shared_Mem_Prozess_UI_kommunikation()





