#include "simuc.h"
#include "user_conf.h"
///////////////////////////////////////////////////////////////////////////////////////
// Bedingte Compilierung 
// Es darf immer nur ein "define" aktive, d.h. nicht auskommentiert, sein.
//
#define V3_Aufgabe_1
//#define V3_Aufgabe_2_und_3
//#define nachrichtenempfang_ueber_ports
//#define timer_als_taktsignalgenerator
//
///////////////////////////////////////////////////////////////////////////////////////
typedef struct {
    unsigned char hh;
    unsigned char mm;
    unsigned char ss;
} uhrzeit;

uhrzeit akt_zeit;
uhrzeit hoch_zeit;
uhrzeit runter_zeit;

#ifdef V3_Aufgabe_1

/*
    ES GELTEN DIE BEI "rollosteuerung_moore" AN GLEICHER STELLE VORANGESTELLTEN KOMMENTARE
*/

// Einige Defines zum leichteren Wiederfinden
#define BIT_POS_IST_OBEN           0
#define BIT_POS_IST_UNTEN          1
#define BIT_POS_NACH_OBEN          4
#define BIT_POS_NACH_UNTEN         5
#define BIT_POS_FAHRE_NACH_OBEN    9
#define BIT_POS_MOTOR_AN           8
#define BIT_POS_FAHRE_NACH_UNTEN  10

#define ZT_MAXW  3  //20
#define SS_MAXW 60  //60
#define MM_MAXW  2  //60
#define HH_MAXW  1  //24

typedef enum {steht=0, hoch, runter} STATE;    // Datentyp für den Zustand das Automaten.
// Vergleichbar mit "TYPE .... IS" in VHDL.
typedef unsigned short int USHORT;            // Eigener Datentyp zur Abkuerzung


void timer1_init()
{
    /*
     * Zur Berechnung der Werte fuer den Prescaler und den Compare-Match:
     * Bei einer Frequenz von 4 Mhz zaehlt der Timer mit einem Takt von 0,25us.
     * Er kann mit seinen 65536 moeglichen Zaehlstaenden eine maximale Interrupt-Periode
     * von 65536 * 0,25us = 16384us realisieren.
     * Dies ist zu schnell. => Der Zaheler muss mittels des Prescalers langsamer eingestellt werden.
     * Die ideale Untersetzung waere 50000us / 16384us = 3,0517.
     * Da es diese Unterssetzung (Prescaler-Wert) nicht gibt waehlen wir den naechst groesseren Wert also 8.
     * Der Zaehler zaehlt jetzt mit einem Takt vom 2us. => Die Interrupts haben bei
     * einem Compare-Match-Wert von 65535 eine Periode von 131072 us.Der Compare-Match-Wert
     * muss auf 50000us/131072us*65536us = 25000 eingestellt werden.
    **/
    unsigned short buf = 0;
    // TCRA1
    // Clock Source auf intern/8 Prescaler
    // Timer Modus Clear-Timer-On-Compare-Match
    buf = (1 << PS11) | (1 << TM10);
    io_out16(TCRA1, buf);
    // TCRB1
    // Counter Enable
    buf = (1 << CE1);
    io_out16(TCRB1, buf);
    // TIMR1
    // Compare Match Interrupt enable
    buf = (1 << OCIE1);
    io_out16(TIMR1, buf);
    // CMPA1
    // Compare Match Register auf ...
    buf = 5000; //25000;
    io_out16(CMPA1, buf);
}

//Interrupt Service Routine
void timer1_oco1_isr()
{
    static int zt = 0;
    //unsigned char stringbuf[100];
    unsigned short int buf;
    zt++;
    if(zt == ZT_MAXW){
        zt = 0;
        akt_zeit.ss++;
    }
    if(akt_zeit.ss == SS_MAXW){
        akt_zeit.ss = 0;
        akt_zeit.mm++;
    }
    if(akt_zeit.mm == MM_MAXW){
        akt_zeit.mm = 0;
        akt_zeit.hh++;
    }
    if(akt_zeit.hh == HH_MAXW){
        akt_zeit.hh = 0;
    }
    /*sprintf(stringbuf, "zt=%d Uhrzeit: %d:%d:%d\n", zt, akt_zeit.hh, akt_zeit.mm, akt_zeit.ss);
    putstring(stringbuf); // Bei der Simulation des SimuC fuehrt der Aufruf von putstring() innerhalb einer ISR
    // ggf. zu einem Dead-Lock . Dies liegt aber nur am Simulator.
    // Zuruecksetzen des Interrupt-Flags*/
    buf = io_in16(TIFR1);
    buf = buf & ~(1 << OCIF1);
    io_out16(TIFR1, buf);
}

void steuerungsfunktion(USHORT  ist_oben,
                        USHORT  ist_unten,
                        USHORT  nach_oben,
                        USHORT  nach_unten,
                        USHORT  nach_oben_wegen_zeit,
                        USHORT  nach_unten_wegen_zeit,
                        USHORT* p_fahre_nach_oben,
                        USHORT* p_fahre_nach_unten,
                        STATE*  p_state)
{
    // A1(a) Erweiterung der Steuerungsfunktion des Automaten
    nach_oben = nach_oben | nach_oben_wegen_zeit;
    nach_unten = nach_unten | nach_unten_wegen_zeit;

    // 5.)    switch-case-Konstrukt zur "Verwaltung" der Zustaende
    switch (*p_state) {

    // 6.)    Ein CASE je Zustand
    case steht:

        // 7a.)  .... Ausgabesignale bestimmen
        *p_fahre_nach_unten=0; *p_fahre_nach_oben=0;

        // 8.)    Eingabesignale auswerten und Zustandswechsel herbei fuehren
        //         Ein IF je Pfeil
        if (  (ist_unten == 0) && (nach_unten == 1) && (nach_oben == 0)) {
            *p_state = runter; // Wechsel in den Zustand "runter"
        }
        if (  (ist_oben == 0) && (nach_oben == 1) ){
            *p_state = hoch;  // Wechsel in den Zustand "hoch"
        }

        // Diese if-Anweisung kann entfallen, da sie cstate nicht veraendert.
        if ( !(    ((ist_unten == 0) && (nach_unten == 1) && (nach_oben == 0))
                   || ((ist_oben == 0) && (nach_oben == 1)) )      ) {
            *p_state = steht; // Bleibe im Zustand "steht"
        }
        break;

    case runter:

        // 7a.)  Ausgabesignale bestimmen
        *p_fahre_nach_unten=1; *p_fahre_nach_oben=0;


        // 8.)    Eingabesignale auswerten und Zustandswechsel herbei fuehren
        if (ist_unten == 1) {
            *p_state = steht; // Wechsel in den Zustand "steht"
        }

        // Diese if-Anweisung kann entfallen, da sie cstate nicht veraendert.
        if (ist_unten == 0) {
            *p_state = runter; // Bleibe im Zustand "runter"
        }

        break;

    case hoch:

        // 7a.)  Ausgabesignale bestimmen
        *p_fahre_nach_unten=0; *p_fahre_nach_oben=1;

        // 8.)    Eingabesignale auswerten und Zustandswechsel herbei fuehren
        if (ist_oben == 1){
            *p_state = steht; // Wechsel in den Zustand "steht"
        }

        // Diese if-Anweisung kann entfallen, da sie cstate nicht veraendert.
        if (ist_oben == 0){
            *p_state = hoch; // Bleibe im Zustand "hoch"
        }
        break;

        // 9.) nicht erlaubte Zustaende "abfangen"
    default:
        *p_fahre_nach_unten=0; *p_fahre_nach_oben=0;
        *p_state = runter;
        break;

    } // end switch
} // end steuerungsfunktion()


/**
 * @brief ZeitAreEqual
 * @param zeit1
 * @param zeit2
 * @return 1=Are Equal; otherwise 0
 */
int ZeitAreEqual(uhrzeit* zeit1, uhrzeit* zeit2) {
    if (zeit1 == zeit2 || (zeit1->hh == zeit2->hh && zeit1->mm == zeit2->mm && zeit1->ss == zeit2->ss)) {
        return 1;
    }
    return 0;
}


void m_emain()
{
    STATE cstate;        // Variable für den Zustand das Automaten.

    // Variablen für die Eingabesignale. Eine Variable fuer jedes Signal.
    USHORT      nach_oben;
    USHORT      nach_unten;
    USHORT      ist_oben;
    USHORT      ist_unten;

    // Variablen für die Ausgabesignale. Eine Variable fuer jedes Signal.
    USHORT      fahre_nach_oben;
    USHORT      fahre_nach_unten;

    USHORT      input, output, last_output;

    unsigned short buf = 0;
    //unsigned char stringbuf[100];

    INIT_BM_WITH_REGISTER_UI;    // Nur fuer Simulation

    // 1.)    Hardware konfigurieren
    io_out16(DIR1, 0xFF00); // Ausgang: Bits 15 bis 8   Eingang: Bits 7 bis 0

    // Zeiten vorbelegen
    akt_zeit.hh = 0;
    akt_zeit.hh = 0;
    akt_zeit.hh = 0;
    hoch_zeit.hh =  0;
    hoch_zeit.mm =  1;
    hoch_zeit.ss = 55;
    runter_zeit.hh =  0;
    runter_zeit.mm =  1;
    runter_zeit.ss =  0;

    // Zur Sicherheit vor Initialisierung den Interupt des PIC generell deaktivieren
    buf = io_in16(PICC);
    buf = buf & ~(1 << PICE);
    io_out16(PICC, buf);

    // Timer 1 initialisieren
    timer1_init();

    // ISR registrieren
    setInterruptHandler(IVN_OC1, timer1_oco1_isr);

    // Uhrzeiterfragen ohne weitere Ueberpruefung
    //putstring("Bitte die aktuelle Uhrzeit im Format hh:mm:ss eingeben\n");
    //getstring(stringbuf);
    //sscanf(stringbuf, "%d:%d:%d", &hh, &mm, &ss);

    // Interrupt des PIC jetzt zulassen
    buf = buf | (1 << PICE);
    io_out16(PICC, buf);

    // 2.)     Definition des Startzustandes. Entspricht dem asynchronen Reset in VHDL.
    cstate = steht; // runter;

    // 3.) Unendliche Schleife. Ein Schleifendurchlauf entspricht einem Zyklus des Automaten
    while (1) {

        SYNC_SIM; // Nur fuer Simulation

        // 4.)    Einlesen der Eingabesignale einmal je Zyklus
        input = io_in16(IN1);

        // extrahieren von "ist_oben" (BIT_POS_IST_OBEN)
        ist_oben = (input >> BIT_POS_IST_OBEN) & 0x01;

        // extrahieren von "ist_unten" (BIT_POS_IST_UNTEN)
        ist_unten = (input >> BIT_POS_IST_UNTEN) & 0x01;

        // extrahieren von "nach_oben" (BIT_POS_NACH_OBEN)
        nach_oben = (input >> BIT_POS_NACH_OBEN) & 0x01;

        // extrahieren von "nach_unten" (BIT_POS_NACH_UNTEN)
        nach_unten = (input >> BIT_POS_NACH_UNTEN) & 0x01;

        // Aufruf der Steuerungsfunktion
        steuerungsfunktion(ist_oben, ist_unten, nach_oben, nach_unten,
                           ZeitAreEqual(&akt_zeit, &hoch_zeit), ZeitAreEqual(&akt_zeit, &runter_zeit),
                           &fahre_nach_oben, &fahre_nach_unten, &cstate);


        // 7b.) Ausgabesignale ausgeben
        output = fahre_nach_unten << BIT_POS_FAHRE_NACH_UNTEN;
        output = output | (fahre_nach_oben << BIT_POS_FAHRE_NACH_OBEN);


        // Nur wirklich ausgeben wenn notwendig
        // Optimierung mittels bedigter Portausgabe
        if (last_output != output) {
            output=output | (1 << BIT_POS_MOTOR_AN);   // Nur fuer Bandmodell
            io_out16(OUT1, io_in16(OUT1) & 0x00FF);
            io_out16(OUT1, io_in16(OUT1) |  output);
            last_output = output;
        }
    } // end while
} // end main

#endif

#ifdef V3_Aufgabe_2_und_3


//#define A2
#define A3





// Vorgegebene globale Variablen und Makros
// Diese sind zum Teil auch in den Loesungen zu den relevanten Uebungsaufgaben schon vorhanden
#ifdef A3
#define DATA_LEGTH    9
#else
#define DATA_LEGTH  127  // default from template
#endif

#ifdef A3
#define X(c)  ((c)-'0')
#define MAX_MESSAGE_SIZE  100
#define STARTBYTE         '#'
#define ENDBYTE             0   // '\0'

typedef enum {b_akt_zeit='A', b_hoch_zeit, b_runter_zeit}  Befehlskennung;
typedef enum {warte_auf_start_byte, warte_auf_end_byte}    STATE_N;
#endif

#ifdef A2
unsigned char        byte_received;
#endif
#ifdef A3
unsigned char        nachricht[MAX_MESSAGE_SIZE];
unsigned char        flag_ready=1;
uhrzeit              akt_zeit, hoch_zeit, runter_zeit;
STATE_N              comstate = warte_auf_start_byte; // set default
unsigned long int    byte_counter;
#endif


#ifdef A3
void steuerungsfunktion(unsigned char        byte_received,
                        unsigned long int*   byte_zaehler,
                        unsigned char*       empfangene_nachricht,
                        unsigned char*       ready,
                        STATE_N*             state)
{
    switch (*state) {
    case warte_auf_start_byte:
        // Uebergang nach warte_auf_end_byte
        if (byte_received == STARTBYTE) {
            // Etwas tun am Uebergang.
            *byte_zaehler = 0;
            empfangene_nachricht[*byte_zaehler] = byte_received;
            (*byte_zaehler)++;

            // Zustandswechsel
            *state = warte_auf_end_byte;
        }
        break;

    case warte_auf_end_byte:
        // Uebergang nach warte_auf_start_byte
        if (byte_received == ENDBYTE) {
            // Etwas tun am Uebergang.
            empfangene_nachricht[*byte_zaehler] = byte_received;
            (*byte_zaehler)++;
            *ready = 1;

            // Zustandwechsel
            *state = warte_auf_start_byte;
        }

        if (*byte_zaehler == MAX_MESSAGE_SIZE-1) {
            // Die Nachricht ist zu lang und kann dahr nicht gueltig sein!
            // Zustandwechsel
            *state = warte_auf_start_byte;
        }

        // Uebergang auf sich selbst nur damit etwas getan wird.
        if (byte_received == STARTBYTE) {
            // Etwas tun am Uebergang.
            *byte_zaehler = 0;
            empfangene_nachricht[*byte_zaehler]=byte_received;
            (*byte_zaehler)++;

            // Zustandwechsel
            *state = warte_auf_end_byte; // Ist ueberfluessing dient aber hoffentlich dem Verstaendnis
        }

        // Uebergang auf sich selbst nur damit etwas getan wird.
        if ((byte_received != STARTBYTE) && (byte_received != ENDBYTE)
                && (*byte_zaehler < MAX_MESSAGE_SIZE-1)) {
            // Etwas tun am Uebergang.
            empfangene_nachricht[*byte_zaehler]=byte_received;
            (*byte_zaehler)++;

            // Zustandwechsel
            *state = warte_auf_end_byte; // Ist ueberfluessing dient aber hoffentlich dem Verstaendnis
        }
        break;

    default: // 9.) nicht erlaubte Zustaende "abfangen"
        *state = warte_auf_start_byte;
    } // switch
}


void do_param(unsigned char* msg, uhrzeit* akt, uhrzeit* hoch, uhrzeit* runter ) {
    char string[300];
    // Diese Funktion muss noch mit Leben gefuellt werden.
    // ...
    /* ****
     * Nachrichten Struktur:
     * Length: 9-bytes
     *
     *  0  |  1  | 2 | 3 | 4 | 5 | 6 | 7 |  8
     * '#' | cmd | h | h | m | m | s | s | '\0'
     *
     * '#'  start byte    define:  STARTBYTE
     * cmd  command       enum:    Befehlskennung
     * '0'  end byte      define:  ENDBYTE
    **/

    uhrzeit* time;
    switch (msg[1]) {  // check cmd
    case b_akt_zeit:    time = akt;      break;
    case b_hoch_zeit:   time = hoch;     break;
    case b_runter_zeit: time = runter;   break;
    }

    // handle msg data
    time->hh = (X(msg[2]) * 10) + X(msg[3]);
    time->mm = (X(msg[4]) * 10) + X(msg[5]);
    time->ss = (X(msg[6]) * 10) + X(msg[7]);

}
#endif


// slave
void init_spi2(){
    // Diese Funktion muss noch mit Leben gefuellt werden.
    // ...
    // SPIE2   SPI interrupt enable
    // SPE2    SPI enable
    // notSS2  not slave selecte
    // MSTR2   master select
    // CPOL2   clock polaritz
    // CPHA2   clock phase
    // SPR21   SPI clock rate 1
    // SPR20   SPI clock rate 0
    //   SPR11 | SPR10 | Takt von SCK
    //     0   |   0   |   clk/4
    //     0   |   1   |   clk/16
    //     1   |   0   |   clk/64
    //     1   |   1   |   clk/128
    io_out8(SPCR2, ((1<<SPIE2)|(1<<SPE2)));
}

void spi_isr() {
#ifdef A3
    unsigned char byte_received; // local variable in A3; otherwise global variable
#endif
    //unsigned short int help = 0;

    // Warte auf Uebtragungsende.
    while (!(io_in8(SPSR2) & (1<<SPIF2)));

    // Daten empfangen
    byte_received = io_in8(SPDR2);

#ifdef A3
    // Aufruf der Steuerungsfunktion
    steuerungsfunktion(byte_received, &byte_counter, &(nachricht[0]), &flag_ready, &comstate);
    if (flag_ready==1) do_param(&(nachricht[0]), &akt_zeit, &hoch_zeit, &runter_zeit );
#endif

    //Zureucksetzen des Interrupt-Flags
    //help = io_in16(SPSR1) & ~(1<<SPIF1);
    //io_out16(SPSR1, help);
}


void m_emain()
{
#ifdef A3
    char string[300];
#endif
    unsigned short buf = 0;

    INIT_BM_WITH_REGISTER_UI;

    // Hier die notwendigen Initialisierungen einfuegen.
    // ...
    // Set zero aus time default.
    akt_zeit.hh =  0;
    akt_zeit.mm =  0;
    akt_zeit.ss =  0;
    hoch_zeit.hh =  0;
    hoch_zeit.mm =  0;
    hoch_zeit.ss =  0;
    runter_zeit.hh =  0;
    runter_zeit.mm =  0;
    runter_zeit.ss =  0;

    // Zur Sicherheit vor Initialisierung den Interupt des PIC generell deaktivieren
    buf = io_in16(PICC);
    buf = buf & ~(1 << PICE);
    io_out16(PICC, buf);

    init_spi2();
    setInterruptHandler(IVN_SPI2, spi_isr);


    // Interrupt des PIC jetzt zulassen
    buf = buf | (1 << PICE);
    io_out16(PICC, buf);


    while(1) {
#ifndef USER_PROG_2
        putstring("Sie haben USER_PROG_2 nicht definiert\n");
#endif
#ifdef A3
        if(flag_ready == 1){
            putstring((char*)nachricht);
            putstring("\t");

            sprintf(string, "Akt:%02d:%02d:%02d  Hoch:%02d:%02d:%02d  Runter:%02d:%02d:%02d\n",
                    akt_zeit.hh,    akt_zeit.mm,    akt_zeit.ss,
                    hoch_zeit.hh,   hoch_zeit.mm,   hoch_zeit.ss,
                    runter_zeit.hh, runter_zeit.mm, runter_zeit.ss);

            putstring(string);
            flag_ready = 0;
        }
#endif
    }

}



//################AB HIER STEHT ALLES FUER DAS SENDER-PROGRAMM #################################################

// master
void init_spi1(){
    // Diese Funktion muss noch mit Leben gefuellt werden.
    // ...
    // SPIE1   SPI interrupt enable
    // SPE1    SPI enable
    // notSS1  not slave selecte
    // MSTR1   master select
    // CPOL1   clock polaritz
    // CPHA1   clock phase
    // SPR11   SPI clock rate 1
    // SPR10   SPI clock rate 0
    //   SPR11 | SPR10 | Takt von SCK
    //     0   |   0   |   clk/4
    //     0   |   1   |   clk/16
    //     1   |   0   |   clk/64
    //     1   |   1   |   clk/128
    io_out8(SPCR1, ((1<<SPIE1)|(1<<SPE1)|(1<<MSTR1)));
}

void spi_send_data(unsigned char data) {
    // Starte Uebertagung
    io_out(SPDR1, data);

    // Warte auf Uebertragungsende
    while(!(io_in8(SPSR1)&(1<<SPIF1)));
}

void emain_sender(void* arg)
{
    unsigned char i;
#ifdef A3
    unsigned char  parametriere_akt_zeit[]     =    "#A000005";
    unsigned char  parametriere_hoch_zeit[]    =    "#B000105";
    unsigned char  parametriere_runter_zeit[]  =    "#C000159";

    unsigned char* param;
    //param = parametriere_akt_zeit;
    //param = parametriere_hoch_zeit;
    param = parametriere_runter_zeit;

    // remove unintended unused warnings
    (void)parametriere_akt_zeit;
    (void)parametriere_hoch_zeit;
    (void)parametriere_runter_zeit;
#endif

    (void) arg;

    // Hier die notwendigen Initialisierungen einfuegen.
    // ...
    init_spi1();


    while(1) {
        i=0;
        do  {

            // Hier den Code fuer das Versenden eines Bytes einfuegen
            // ...
#ifdef A2
            spi_send_data(i); //(i%26)+'A'
#endif
#ifdef A3
            spi_send_data(param[i]);
#endif

            // Damit der Empfaenger genuegen Zeit zum Reagieren (Einlesen des Bytes) hat
            // muss hier geweartet werden.
            ms_wait(10);

            i++;

        } while(i<DATA_LEGTH);

    }

}

#endif

#ifdef nachrichtenempfang_ueber_ports
// Sinnvoll zu nutzende Makros
#define COM_SIGNAl_PIN              0    // Pin ueber den der Interrupts ausgeloest wird
#define COM_DATA_IN_REGISTER      IN0    // Register ueber den das Byte eingelesen wird
#define MAX_MESSAGE_SIZE          100    // Maximale Laenge einer Nachricht
#define STARTBYTE                0xFE    // Wert des Start-Bytes
#define ENDBYTE                  0xEF    // Wert des Ende-Bytes


typedef enum {warte_auf_start_byte, warte_auf_end_byte} STATE_N;


// Globale Variablen fuer die ISR
unsigned char        nachricht[MAX_MESSAGE_SIZE];
unsigned char        flag_ready;
STATE_N              comstate = warte_auf_start_byte;
unsigned long int    byte_counter;



void init_gpio_0_1()
{
    unsigned short hilfe = 0;

    // ### PORT 1
    // Interrupt fuer Bit 0 von PORT1 enable
    hilfe = io_in16(EIE1) | (1 << COM_SIGNAl_PIN);
    io_out16(EIE1, hilfe);

    // Das Bit 0 von PORT 1 ist Eingang
    hilfe = io_in16(DIR1) & ~(1 << COM_SIGNAl_PIN);
    io_out16(DIR1, hilfe);


    // ### PORT 0
    // Die unter 8 Bit von PORT0 sind Eingang
    hilfe = io_in16(DIR0) & 0xFF00;

    io_out16(DIR1, hilfe);
}


void steuerungsfunktion(unsigned char       byte_received,
                        unsigned long int*  byte_zaehler,
                        unsigned char*      empfangene_nachricht,
                        unsigned char*      ready,
                        STATE_N*            state)
{
    switch (*state) {

    case warte_auf_start_byte:
        // Uebergang nach warte_auf_end_byte
        if (byte_received == STARTBYTE) {
            // Etwas tun am Uebergang.
            *byte_zaehler = 0;
            empfangene_nachricht[*byte_zaehler] = byte_received;
            *byte_zaehler = *byte_zaehler + 1;

            // Zustandswechsel
            *state = warte_auf_end_byte;
        }
        break;

    case warte_auf_end_byte:
        // Uebergang nach warte_auf_start_byte
        if (byte_received == ENDBYTE)  {
            // Etwas tun am Uebergang.
            empfangene_nachricht[*byte_zaehler = byte_received;
            *byte_zaehler = *byte_zaehler + 1;

            *ready = 1;

            // Zustandwechsel
            *state = warte_auf_start_byte;
        }

        if (*byte_zaehler == MAX_MESSAGE_SIZE-1) {
            // Die Nachricht ist zu lang und kann dahr nicht gueltig sein!
            // Zustandwechsel
            *state = warte_auf_start_byte;
        }

        // Uebergang auf sich selbst nur damit etwas getan wird.
        if (byte_received == STARTBYTE) {
            // Etwas tun am Uebergang.
            *byte_zaehler = 0;
            empfangene_nachricht[*byte_zaehler] = byte_received;
            *byte_zaehler = *byte_zaehler + 1;

            // Zustandwechsel
            *state = warte_auf_end_byte;    // Ist ueberfluessing dient aber hoffentlich
            // dem Verstaendnis
        }

        // Uebergang auf sich selbst nur damit etwas getan wird.
        if ((byte_received != STARTBYTE) && (byte_received != ENDBYTE)
                && (*byte_zaehler < MAX_MESSAGE_SIZE - 1)) {
            // Etwas tun am Uebergang.
            empfangene_nachricht[*byte_zaehler] = byte_received;
            *byte_zaehler = *byte_zaehler + 1;

            // Zustandwechsel
            *state = warte_auf_end_byte;    // Ist ueberfluessing dient aber hoffentlich
            // dem Verstaendnis
        }

        break;

        // 9.) nicht erlaubte Zustaende "abfangen"
    default:
        *state = warte_auf_start_byte;
    } // switch
}


void ISR_EXT_INT0()
{
    unsigned short int hilfe = 0;
    unsigned char buf;

    // Einlesen des Datenbytes
    buf=(unsigned char) (io_in16(COM_DATA_IN_REGISTER) & 0x00FF);

    // Aufruf der Steuerungsfunktion
    steuerungsfunktion(buf, &byte_counter, &(nachricht[0]), &flag_ready, &comstate);

    // Zureucksetzen des Interrupt-Flags
    hilfe = io_in16(EIF1) & ~(1<<COM_SIGNAl_PIN);
    io_out16(EIF1, hilfe);
    return;
}


void m_emain()
{
    unsigned short int buf;

    INIT_REGISTER_UI_WITHOUT_BM

    // Zur Sicherheit vor Initialisierung den Interrupt des PIC generell deaktivieren
    buf = io_in16(PICC);
    buf = buf &  ~(1 << PICE);
    io_out16(PICC, buf);

    // Initialisieren der Ports
    init_gpio_0_1();

    // Registrieren der ISRs in der Interupt-Vektor-Tabelle
    setInterruptHandler(IVN_EI100, ISR_EXT_INT0);

    // Interrupt des PIV jetzt zulassen.
    buf = buf | (1 << PICE);
    io_out16(PICC, buf);

    while(1);
}

#endif

#ifdef timer_als_taktsignalgenerator


// Globale Variablen zur Kommunikation mit der ISR
unsigned long int zt=0, hh=0, mm=0, ss=0;

void timer1_init()
{
    /*    Zur Berechnung der Werte fuer den Prescaler und den Compare-Match:
        Bei einer Frequenz von 4 Mhz zaehlt der Timer mit einem Takt von 0,25us.
        Er kann mit seinen 65536 moeglichen Zaehlstaenden eine maximale Interrupt-Periode von 65536 * 0,25us = 16384us realisieren.
        Dies ist zu schnell. => Der Zaheler muss mittels des Prescalers langsamer eingestellt werden.
        Die ideale Untersetzung waere  50000us / 16384us = 3,0517.
        Da es diese Unterssetzung (Prescaler-Wert) nicht gibt waehlen wir den naechst groesseren Wert also 8.
        Der Zaehler zaehlt jetzt mit einem Takt vom 2us. => Die Interrupts haben bei einem Compare-Match-Wert von 65535
        eine Periode von 131072 us.Der Compare-Match-Wert muss auf 50000us/131072us*65536us = 25000 eingestellt werden.
    */

    unsigned short buf = 0;

    // TCRA1
    // Clock Source auf intern/8 Prescaler
    // Timer Modus Clear-Timer-On-Compare-Match
    buf = (1 << PS11) | (1 << TM10);
    io_out16(TCRA1, buf);

    // TCRB1
    // Counter Enable
    buf = (1 <<CE1);
    io_out16(TCRB1, buf);

    // TIMR1
    // Compare Match Interrupt enable
    buf = (1 << OCIE1);
    io_out16(TIMR1, buf);

    // CMPA1
    // Compare Match Register auf ...
    buf = 25000;
    io_out16(CMPA1, buf);
}

// Makros damit man beim Testen nicht so lange warten muss
// Die korrekten Werte stehen als Kommentare jeweils dahinter
#define ZT_MAXW    3    //20
#define SS_MAXW    3    //60
#define MM_MAXW    3    //60
#define HH_MAXW    3    //24

// Interrupt Service Routine
void timer1_oco1_isr()
{
    unsigned char stringbuf[100];
    unsigned short int buf;

    zt++;

    if(zt==ZT_MAXW){
        zt=0;
        ss++;
    }

    if(ss==SS_MAXW){
        ss=0;
        mm++;
    }

    if(mm==MM_MAXW){
        mm=0;
        hh++;
    }
    if(hh==HH_MAXW){
        hh=0;
    }

    sprintf(stringbuf,"zt=%d  Uhrzeit: %d:%d:%d\n",zt,hh,mm,ss);
    // Achtung die Verwendung von putstring() kann in der Simulationsumgebung
    // zu einem Deadlock fuehren, d.h. alles bleibt einfach stehen.
    // Dahr ist dies hier (zur Sicherheit) auskommentiert.
    // putstring(stringbuf);

    // Zuruecksetzen des Interrupt-Flags
    buf = io_in16(TIFR1);
    buf = buf & ~(1 << OCIF1);
    io_out16(TIFR1, buf);

}


void m_emain()
{
    unsigned short buf = 0;
    unsigned char stringbuf[100];

    INIT_BM_WITH_REGISTER_UI; // Nur zur Simulation

    // Zur Sicherheit vor Initialisierung den Interupt des PIC generell deaktivieren
    buf = io_in16(PICC);
    buf = buf &  ~(1 << PICE);
    io_out16(PICC, buf);

    // Timer 1 initialisieren
    timer1_init();

    // ISR registrieren
    setInterruptHandler(IVN_OC1, timer1_oco1_isr);

    // Uhrzeiterfragen ohne weitere Ueberpruefung
    putstring("Bitte die aktuelle Uhrzeit im Format hh:mm:ss eingeben\n");
    getstring(stringbuf);
    sscanf(stringbuf,"%d:%d:%d",&hh, &mm, &ss);

    // Interrupt des PIC jetzt zulassen
    buf = buf | (1 << PICE);
    io_out16(PICC, buf);

    while(1);
}

#endif


void emain(void* arg) {
    (void) arg; // remove unused warning of arg
    m_emain();
}
