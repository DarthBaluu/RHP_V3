#------------------------------------------------------------------------------------------------------
# Bei unseren Projekten werden recht intensiv Exports und Imports verwendet.
# Der Import geschieht dabei ueber den u.a. Block "Library Import".
# Der dabei verwendete Mechanismus benutzt sogenannte *.pri-Dateien (PRI-Dateien).
# Diese werden ausgewertet und die Projektskrutur (auch im linken Projekt-Fenster)
# wird dabei entsprechend angepasst.
# Die PRI-Dateien werden im Rahmen der Veraenderung der Projekte auch immer mal
# wieder angepasst und erweitert. Die in den exports_qt und imports_qt-Verzeichnissen
# zu findenden Pri-Dateien sind dabei nur Kopien die durch POSTBUILD.BAT und PREBUILD.BAT
# aus den Original-Pri-Dateien dort hin kopiert werden.
# Die Original-Pri-Dateien finden sich wie folgt:
#  1.) Zu jedem Export(zu jeder exportierten Lib) findet sich die entsprechende Pri-Datei direkt neben
#      der Pro-Datei des Projektes, das den Export(die Lib)erzeugt. Die Pri-Datei hat dabei
#      einen gleichen oder sehr aehnlichen Namen wie die Pro-Datei.
#  2.) Zu jedem Projekt das etwas importiert findet sich die Pri-Datei direkt neben der Pro-Datei des
#      Projektes. Der Name dieser Pri-Datei ist immer libs.pri.
#      In dieser Pri-Datei stehen die Verweise auf ALLE zu importierenden Libs.
#
# ACHTUNG:
# Prinzipiell sind die Verzeichnisse exports_qt und imports_qt so angelegt, dass diese immer
# auch von Hand geloescht werden duerfen. Die Batchjobs POSTBUILD.BAT und PREBUILD.BAT bauen diese
# immer wieder neu auf.
# Hat man nun ein imports_qt-Verzeichnis von Hand geloescht und oeffnet dann das entsprechende Projekt
# so findet Qt die Verweise nicht und loescht diese auch aus der Projektstruktur.
# Folglich werden dann beim Compileiren Dateien (die eigentlich nach dem PREBUILD.BAT-Aufruf vorhandene
# sind) nicht gefunden.
# IN DIESEM FALL MUSS DIE PRO-DATEI NEU GESPEICHERT WERDEN DAMIT DIE PROJEKTSTRUKTUR NEU GEBAUT WIRD.
#------------------------------------------------------------------------------------------------------


## Original Anfang
# QT       += core gui
# greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
# TARGET = Applikation_und_Simulation
# TEMPLATE = app
## Original Ende

## Neu Anfang
QT      += core
QT      -= gui
TARGET = Applikation_und_Simulation
CONFIG   += console
Config   -= app_bundle
TEMPLATE = app
## Neu Ende


SOURCES += \
    ../sourcen/main.cpp \
    ../sourcen/emain.c \
    ../sourcen/io_teiber.c \
    ../../Gemeinsam_genutzte_Sourcen/qt_sharedmemory.cpp



HEADERS  += \
    ../sourcen/io_treiber.h \
    ../sourcen/simuc.h \
    ../sourcen/user_conf.h \
    ../../Gemeinsam_genutzte_Sourcen/qt_sharedmemory.h



  </ItemGroup>

EXE_DIR = $$PWD/../../../EXE_Projekte
INCLUDEPATH += \
        $$EXE_DIR/Gemeinsam_genutzte_Sourcen

################################# Library Import ###############################
IMP_DIR = $$PWD/../imports_qt
# Dateiendung abhängig vom Betriebssystem
unix:LIB_FILE_EXT = a
win32:LIB_FILE_EXT = a

# Unterscheidung bzgl. des Build (Debug/Release)
Debug: BUILD_DEBUG_RELEASE = Debug
Release: BUILD_DEBUG_RELEASE = Release

exists($${IMP_DIR}/libs.pri) {
	include($${IMP_DIR}/libs.pri)
}
################################################################################
