#ifndef EIOK_SHAREDMEMORY_H
#define EIOK_SHAREDMEMORY_H

#include <qsharedmemory.h>
#include <QBuffer>
#include <QObject>
#include <qDebug.h>
#include "device_interface.h"


class Eiok_SharedMemory : public QObject

{
	Q_OBJECT

public:
	Eiok_SharedMemory(char  *NamensString, unsigned short ProzessNummer, unsigned short index_exportzeigerfeld);
	~Eiok_SharedMemory();


public slots:
	void setMemInit();
	void pointerMemInit();
	void Kommunikationsdauerlauefer_Simulationsseitig();
	void Kommunikationsdauerlauefer_Userinterfaceseitig();


signals:					// Alle Signals sind ausschliesslich fuer das UI !!!!
	// Ein Signal fuer jedes der Control-Flags 
	// Maximale unterstuetzt werden  MAX_COUNT_SHARED_MEMORY_CONTROL_BITS
	// Die Bedeutung ergibt aus der  "Definition fuer die Lage (Index) der Kontroll-Flags im Feld ctr_Prozess[]"
	// siehe "3.)" in der Datei "system_configuration.h"
    
	void signal_for_ctr_flag_0();	
    void signal_for_ctr_flag_1();
    void signal_for_ctr_flag_2();
    void signal_for_ctr_flag_3();
	void signal_for_ctr_flag_4();
	void signal_for_ctr_flag_5();
	void signal_for_ctr_flag_6();
	void signal_for_ctr_flag_7();	
	void signal_for_ctr_flag_8();	
    void signal_for_ctr_flag_9();
    void signal_for_ctr_flag_10();
    void signal_for_ctr_flag_11();
    void signal_for_ctr_flag_12();
    void signal_for_ctr_flag_13();
	void signal_for_ctr_flag_14();
    void signal_for_ctr_flag_15();
    void signal_for_ctr_flag_16();
    void signal_for_ctr_flag_17();
	void signal_for_ctr_flag_18();
    void signal_for_ctr_flag_19();
    void signal_for_ctr_flag_20();
    void signal_for_ctr_flag_21();
	void signal_for_ctr_flag_22();
    void signal_for_ctr_flag_23();
    void signal_for_ctr_flag_24();
    void signal_for_ctr_flag_25();
	void signal_for_ctr_flag_26();
    void signal_for_ctr_flag_27();
    void signal_for_ctr_flag_28();
    void signal_for_ctr_flag_29();
	void signal_for_ctr_flag_30();
    void signal_for_ctr_flag_31();	//MAX_COUNT_SHARED_MEMORY_CONTROL_BITS_Changes

	// Weitere Signale die unabhängig von den Control-Flags genutzt werden
    void setText(QString);

private:
	QSharedMemory sharedMem_Segment;
	STRC_SHARED_MEM *pointer_ShMem_Seg;  
	unsigned short eigeneProzessNummer;
	unsigned short fuer_exportzeigerfeld_benutzer_index;
};

#endif // EIOK_SHAREDMEMORY_H
