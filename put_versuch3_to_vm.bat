:: Kopiert die RHP-Simulatoren sowie das Beispielprojekt auf die bereitgestellte VM
:: Bereits existierende Dateien werden ueberschrieben

xcopy /s /i /y 	.\rhp_c_projekte  	c:\rhp_c_projekte

:: xcopy /s /i /y 	.\rhp_simulators  	c:\rhp_simulators

:: copy c:\rhp_simulators\*.bat 	%USERPROFILE%\Desktop\

pause


