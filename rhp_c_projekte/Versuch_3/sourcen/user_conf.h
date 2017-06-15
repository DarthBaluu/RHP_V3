#ifndef USER_CONF_H
#define USER_CONF_H

// ****************************************************************************************************
// Diese Datei buendelt die durch den User zur Compilierzeit einstellbaren Parameter
// ****************************************************************************************************

// Parametrierung der parallel laufenden User-Programme
// Es koennen derzeit maximal zwei parallel laufende User-Programme definiert werden.
// Wird ein Userprogramm nicht benoetigt, dann muss das ensprechende Define auskommentiert werden.
// ACHTUNG: Nach der Aenderung dieser Konfiguration koennen beim Compilieren folgende Fehler gemeldet werden:
//			Fehlermeldung 1: "Der Package-Server funktioniert nicht mehr. Soll er beendet werden."
//				und/oder
//			Fehlermeldung 2: "  error MSB3073: :VCEnd" wurde mit dem Code 4 beendet."
//	Beide Fehler liegen beim Visual-Studio/Visual-Express und ruehren wohl daher, dass die geaenderte Datenbasis 
//  fuer das Compilieren noch nicht konsistent aufgebaut wurde.
//	Man muss es dann einfach noch mal versuchen. 
#define USER_PROG_1		emain
//#define USER_PROG_2		emain_sender

#endif //USER_CONF_H
