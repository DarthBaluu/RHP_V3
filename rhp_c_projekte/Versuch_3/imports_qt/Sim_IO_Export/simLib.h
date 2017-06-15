/*************************************************************************************************************//**
 *	SEEM 
 *	simLib.h
 *
 *	Diese Datei soll von den Studiernenden eingebunden werden. Somit muessen alle für die Studierenden
 *	wichtigen Funktion von hieraus erreichbar sein.
 *
 *	Hauptaugenmerk sind hier die Funktionen in() und out().
 *	Es muss noch eine Funktion zum Eintragen der ISR hinzugefuegt werden.
 *	
 *	Author: Ma Kroe
 *
 *	zuletzt geaendert:
 *	12.05.14
 *
 ****************************************************************************************************************/

#ifndef SIMLIB_H
#define SIMLIB_H

#include "port_extension.h"
#include "device_interface.h"



#ifdef  __cplusplus
extern "C" {
#endif

/*!
*	Funktion in(registerAdr) um den Wert des Registers mit angegebener Registeradresse 
*	zurueckzugeben.
*
*	@param registerAdr unsigned short fuer die Adresse des gewuenschten Registers
*
*	@return unsigned short aktueller Wert des Registers
*	
*/
unsigned short in(unsigned short registerAdr); // Liest 2 Byte aber nur ab geraden (durch 2 teilbaren) Adressen.
unsigned char in8(unsigned short registerAdr); // Liest 1 Byte ab jeder Adresse

/*!
*	Funktion out(registerAdr, value) um einem Register mit der angegebenen Register Adresse einen
*	Wert zuzuweisen.
*
*	@param registerAdr	unsigned int fuer die Adresse des gewuenschten Registers
*	@param value		unsigned int fuer den Wert, welcher in das Register geschrieben werden soll
*	
*/
void out(unsigned short registerAdr, unsigned short value);
void out8(unsigned short registerAdr, unsigned char value);

#ifdef  __cplusplus
}
#endif

#endif //SIMLIB_H