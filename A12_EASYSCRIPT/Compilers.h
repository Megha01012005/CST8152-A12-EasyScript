/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2024
* Author: TANVI/MEGHA
* Professors: Paulo Sousa
************************************************************
#
# ECHO "  =---------------------------------------=	  "
# ECHO "  |  COMPILERS - ALGONQUIN COLLEGE (F24)  |	  "
# ECHO "  =---------------------------------------=	  "
# ECHO "  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    "
# ECHO "  @@									@@    "
# ECHO "  @@   @@@@@@@@@@@@@       @@@@@@@      @@    "
# ECHO "  @@   @@@@@@@@@@@@@    @@@@@@@@@@@@    @@    "
# ECHO "  @@   @@@@			  @@@@@      @@@@   @@    "
# ECHO "  @@   @@@@            @@@@@	   @@	@@    "
# ECHO "  @@   @@@@              @@@@@          @@    "
# ECHO "  @@   @@@@@@@@@@         @@@@@@@       @@    "
# ECHO "  @@   @@@@@@@@@@             @@@@@@    @@    "
# ECHO "  @@   @@@@                     @@@@@@  @@    "
# ECHO "  @@   @@@@            @@        @@@@@  @@    "
# ECHO "  @@   @@@@		       @@@@     @@@@@@  @@    "
# ECHO "  @@   @@@@@@@@@@@@@     @@@@@@@@@@@@   @@    "
# ECHO "  @@   @@@@@@@@@@@@@       @@@@@@@		@@    "
# ECHO "  @@									@@    "
# ECHO "  @@         EasyScript					@@    "
# ECHO "  @@									@@    "
# ECHO "  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    "
# ECHO "											  "
# ECHO "[READER SCRIPT .........................]	  "
# ECHO "											  "
*/

/*
************************************************************
* File name: Compilers.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12, A22, A32.
* Date: Sep 01 2024
* Professor: Paulo Sousa
* Purpose: This file defines the functions called by main function.
* Function list: mainReader(), mainScanner(), mainParser().
*************************************************************/

#ifndef COMPILERS_H_
#define COMPILERS_H_

#define DEBUG 0

/*
 * ............................................................................
 * ADVICE 1:
 * Please check the "TODO" labels to develop your activity.
 *
 * ADVICE 2: This file must be updated according to each assignment
 * (see the progression: reader > scanner > parser).
 * ............................................................................
 */

/* TO_DO: Adjust your language (cannot be "EasyScript") */

/* TO_DO: Language name */
#define STR_LANGNAME	"EasyScript"

/* TO_DO: Logical constants - adapt for your language */
#define EasyScript_TRUE		   1
#define EasyScript_FALSE	   0
#define EasyScript_ERROR	  (-1)	/* General error message */

#define EasyScript_INVALID	  NULL

/*
------------------------------------------------------------
Data types definitions
NOTE: Some types may not be directly used by your language,
		but they can be necessary for conversions.
------------------------------------------------------------
*/


/* TO_DO: Define your typedefs */
typedef char			EasyScript_char;
typedef char*			EasyScript_string;
typedef int				EasyScript_int;
typedef float			EasyScript_float;
typedef void			EasyScript_void;

typedef char			EasyScript_bool;
typedef unsigned char	EasyScript_byte;

typedef long			EasyScript_long;
typedef double			EasyScript_double;

/*
------------------------------------------------------------
Programs:
1: Reader - invokes MainReader code
2: Scanner - invokes MainScanner code
3: Parser - invokes MainParser code
------------------------------------------------------------
*/
enum PROGRAMS {
	PGM_READER  = '1',
	PGM_SCANNER = '2',
	PGM_PARSER  = '3'
};

/*
------------------------------------------------------------
Main functions signatures
(Code will be updated during next assignments)
------------------------------------------------------------
*/
EasyScript_int mainReader(EasyScript_int argc, EasyScript_string* argv);
EasyScript_int mainScanner(EasyScript_int argc, EasyScript_string* argv);
/* TO_DO: Remove comment in next assignments */
/*
EasyScript_int mainParser(EasyScript_int argc, EasyScript_string* argv);
*/

EasyScript_void printLogo();

#endif
