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
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12.
* Date: May 01 2024
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * Please check the "TODO" labels to develop your activity.
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#define READER_H_

/* TIP: Do not change pragmas, unless necessary .......................................*/
/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */
/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Modes (used to create buffer reader) */
enum READER_MODE {
	MODE_FIXED = 'f', /* Fixed mode (constant size) */
	MODE_ADDIT = 'a', /* Additive mode (constant increment to be added) */
	MODE_MULTI = 'm'  /* Multiplicative mode (constant increment to be multiplied) */
};

/* Constants about controls (not need to change) */
#define READER_TERMINATOR	'\0'							/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME (EasyScript) .................................. */

/* You should add your own constant definitions here */
#define READER_MAX_SIZE	INT_MAX-1	/* maximum capacity */ 

#define READER_DEFAULT_SIZE			250		/* default initial buffer reader capacity */
#define READER_DEFAULT_INCREMENT	10		/* default increment factor */

#define NCHAR				128			/* Chars from 0 to 127 */

#define CHARSEOF			(-1)		/* EOF Code for Reader */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME (EasyScript) .................................. */

/* TODO: Adjust datatypes */

/* Offset declaration */
typedef struct flag {
	EasyScript_bool isEmpty;			/* indicates if the buffer is empty */
	EasyScript_bool isFull;			/* indicates if the buffer is full */
	EasyScript_bool isRead;			/* indicates if the buffer was completely read */
	EasyScript_bool isMoved;			/* indicates if the buffer memory was changed */
} Flag;

/* Offset declaration */
typedef struct position {
	EasyScript_int wrte;			/* the offset to the add chars (in chars) */
	EasyScript_int read;			/* the offset to the get a char position (in chars) */
	EasyScript_int mark;			/* the offset to the mark position (in chars) */
} Position;

/* Buffer structure */
typedef struct bufferReader {
	EasyScript_string	content;			/* pointer to the beginning of character array (character buffer) */
	EasyScript_int		size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	EasyScript_int		increment;			/* character array increment factor */
	EasyScript_char		mode;				/* operational mode indicator */
	Flag				flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Position			positions;			/* Offset / position field */
	EasyScript_int		histogram[NCHAR];	/* Statistics of chars */
	EasyScript_int		numReaderErrors;	/* Number of errors from Reader */
	EasyScript_byte		checksum;
} Buffer, * BufferPointer;

/* FUNCTIONS DECLARATION:  .................................. */
/* General Operations */
BufferPointer		readerCreate		(EasyScript_int, EasyScript_int, EasyScript_char);
BufferPointer		readerAddChar		(BufferPointer const, EasyScript_char);
EasyScript_bool		readerClear		    (BufferPointer const);
EasyScript_bool		readerFree		    (BufferPointer const);
EasyScript_bool		readerIsFull		(BufferPointer const);
EasyScript_bool		readerIsEmpty		(BufferPointer const);
EasyScript_bool		readerSetMark		(BufferPointer const, EasyScript_int);
EasyScript_int		readerPrint		    (BufferPointer const);
EasyScript_int		readerLoad			(BufferPointer const, FILE* const);
EasyScript_bool		readerRecover		(BufferPointer const);
EasyScript_bool		readerRetract		(BufferPointer const);
EasyScript_bool		readerRestore		(BufferPointer const);
EasyScript_void		readerCalcChecksum	(BufferPointer const);
EasyScript_bool		readerPrintFlags	(BufferPointer const);
EasyScript_void		readerPrintStat     (BufferPointer const);
/* Getters */
EasyScript_char		readerGetChar		(BufferPointer const);
EasyScript_string	readerGetContent	(BufferPointer const, EasyScript_int);
EasyScript_int		readerGetPosRead	(BufferPointer const);
EasyScript_int		readerGetPosWrte	(BufferPointer const);
EasyScript_int		readerGetPosMark	(BufferPointer const);
EasyScript_int		readerGetSize		(BufferPointer const);
EasyScript_int		readerGetInc		(BufferPointer const);
EasyScript_char		readerGetMode		(BufferPointer const);
EasyScript_int		readerGetNumErrors	(BufferPointer const);

#endif
