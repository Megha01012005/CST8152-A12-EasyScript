:: ---------------------------------------------------------------------
:: COMPILERS COURSE - SCRIPT                                           -
:: SCRIPT A22 - CST8152 - Fall 2023                                    -
:: ---------------------------------------------------------------------

CLS
SET COMPILER=Compiler.exe

SET FILE1=INPUT0_Empty
SET FILE2=INPUT1_Hello
SET FILE3=INPUT2_Volume
SET FILE4=INPUT3_Factorial
SET FILE5=INPUT4_Datatypes
SET FILE6=INPUT5_General

SET ASSIGNMENT=A22
SET EXTENSION=sof
SET OUTPUT=out
SET ERROR=err

SET PARAM=s

:: ---------------------------------------------------------------------
:: Begin of Tests (A22 - F23) ------------------------------------------
:: ---------------------------------------------------------------------

@echo off

ECHO "=---------------------------------------="
ECHO "|  COMPILERS - ALGONQUIN COLLEGE (F23)  |"
ECHO "=---------------------------------------="
ECHO "|              ....                     |"
ECHO "|          ........::.::::::.           |"
ECHO "|        .:........::.:^^^~~~:          |"
ECHO "|        :^^::::::^^^::^!7??7~^.        |"
ECHO "|       .:^~~^!77777~~7?YY?7??7^.       |"
ECHO "|       :.^~!??!^::::^^~!?5PY??!~.      |"
ECHO "|       ~!!7J~.:::^^^^~!!~~?G5J?~       |"
ECHO "|       :^~?!~7?Y57^^?PP5YJ!J5Y?:       |"
ECHO "|       .~!!.:^!7!:.:7JYYJ7~7Y7^        |"
ECHO "|       .~77..    . .~^:^^^~7?:         |"
ECHO "|       .^!^~:::.:^!7?~^~!77J:          |"
ECHO "|        ^^!Y~^^^^~?YJ77??7JJ^          |"
ECHO "|       .^7J?~^~~^~7??7??7JY?~:         |"
ECHO "|        ::^^~^7?!^~~!7???J?J7~:.       |"
ECHO "|         ^~~!.^7YPPPP5Y?7J7777~.       |"
ECHO "|        ..:~..:^!JPP5YJ?!777!^.        |"
ECHO "| .~?JJJJJJJJJJYYYYYPPPPPPPPPPPP5PPYY~  |"
ECHO "|  :!Y5GGG.___ YYYYYY__._.PPGGGGGG5!.   |"
ECHO "|   :!Y5G / __| ___ / _(_)__ _ PGP5.    |"
ECHO "|    :~75 \__ \/ _ \  _| / _` | 5?.     |"
ECHO "|     7~7 |___/\___/_| |_\__,_| Y5?.    |"
ECHO "|    .^~!~.....................P5YY7.   |"
ECHO "|   .:::::::::::::?JJJJYYYYYYYYYJJJJ7.  |"
ECHO "|                                       |"
ECHO "=---------------------------------------="
ECHO "                                   "
ECHO "[READER SCRIPT .........................]"
ECHO "                                   "

ren *.exe %COMPILER%

::
:: BASIC TESTS  ----------------------------------------------------------
::
:: Basic Tests (A22 - W22) - - - - - - - - - - - - - - - - - - - - - -

%COMPILER% %PARAM% %FILE1%.%EXTENSION%	> %FILE1%-%ASSIGNMENT%.%OUTPUT%	2> %FILE1%-%ASSIGNMENT%.%ERROR%
%COMPILER% %PARAM% %FILE2%.%EXTENSION%	> %FILE2%-%ASSIGNMENT%.%OUTPUT%	2> %FILE2%-%ASSIGNMENT%.%ERROR%
%COMPILER% %PARAM% %FILE3%.%EXTENSION%	> %FILE3%-%ASSIGNMENT%.%OUTPUT%	2> %FILE3%-%ASSIGNMENT%.%ERROR%
%COMPILER% %PARAM% %FILE4%.%EXTENSION%	> %FILE4%-%ASSIGNMENT%.%OUTPUT%	2> %FILE4%-%ASSIGNMENT%.%ERROR%
%COMPILER% %PARAM% %FILE5%.%EXTENSION%	> %FILE5%-%ASSIGNMENT%.%OUTPUT%	2> %FILE5%-%ASSIGNMENT%.%ERROR%
%COMPILER% %PARAM% %FILE6%.%EXTENSION%	> %FILE6%-%ASSIGNMENT%.%OUTPUT%	2> %FILE6%-%ASSIGNMENT%.%ERROR%

:: SHOW OUTPUTS - - - - - - - - - - - - - - - - - - - - - - - - - - -
DIR *.OUT
DIR *.ERR

:: ---------------------------------------------------------------------
:: End of Tests (A22 - F22) --------------------------------------------
:: ---------------------------------------------------------------------
