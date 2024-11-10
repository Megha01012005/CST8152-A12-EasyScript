/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2024
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (F24)  |"
# ECHO "=---------------------------------------="
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    �
# ECHO "    @@                             @@    �
# ECHO "    @@           %&@@@@@@@@@@@     @@    �
# ECHO "    @@       @%% (@@@@@@@@@  @     @@    �
# ECHO "    @@      @& @   @ @       @     @@    �
# ECHO "    @@     @ @ %  / /   @@@@@@     @@    �
# ECHO "    @@      & @ @  @@              @@    �
# ECHO "    @@       @/ @*@ @ @   @        @@    �
# ECHO "    @@           @@@@  @@ @ @      @@    �
# ECHO "    @@            /@@    @@@ @     @@    �
# ECHO "    @@     @      / /     @@ @     @@    �
# ECHO "    @@     @ @@   /@/   @@@ @      @@    �
# ECHO "    @@     @@@@@@@@@@@@@@@         @@    �
# ECHO "    @@                             @@    �
# ECHO "    @@         S O F I A           @@    �
# ECHO "    @@                             @@    �
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    �
# ECHO "                                         "
# ECHO "[READER SCRIPT .........................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: Scanner.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2024
* Purpose: This file contains all functionalities from Scanner.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

 /* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
  * to suppress the warnings about using "unsafe" functions like fopen()
  * and standard sting library functions defined in string.h.
  * The define does not have any effect in Borland compiler projects.
  */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

  /* #define NDEBUG to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/*
----------------------------------------------------------------
TO_DO: Global vars definitions
----------------------------------------------------------------
*/

/* Global objects - variables */
/* This buffer is used as a repository for string literals. */
extern BufferPointer stringLiteralTable;	/* String literal table */
EasyScript_int line;								/* Current line number of the source code */
extern EasyScript_int errorNumber;				/* Defined in platy_st.c - run-time error number */

extern EasyScript_int stateType[NUM_STATES];
extern EasyScript_string keywordTable[KWT_SIZE];

extern PTR_ACCFUN finalStateTable[NUM_STATES];
extern EasyScript_int transitionTable[NUM_STATES][CHAR_CLASSES];

/* Local(file) global objects - variables */
static BufferPointer lexemeBuffer;			/* Pointer to temporary lexeme buffer */
static BufferPointer sourceBuffer;			/* Pointer to input source buffer */

/*
 ************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 ***********************************************************
 */
 /* TO_DO: Follow the standard and adjust datatypes */

EasyScript_int startScanner(BufferPointer psc_buf) {
	/* TO_DO: Start histogram */
	for (EasyScript_int i = 0; i < NUM_TOKENS; i++) {
		scData.scanHistogram[i] = 0;
	}

	/* Basic scanner initialization */
	/* in case the buffer has been read previously  */
	readerRecover(psc_buf);
	readerClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;

	return EXIT_SUCCESS; /*0*/
}

/*
 ************************************************************
 * Process Token
 *		Main function of buffer, responsible to classify a char (or sequence
 *		of chars). In the first part, a specific sequence is detected (reading
 *		from buffer). In the second part, a pattern (defined by Regular Expression)
 *		is recognized and the appropriate function is called (related to final states
 *		in the Transition Diagram).
 ***********************************************************
 */

Token tokenizer(EasyScript_void) {

	/* TO_DO: Follow the standard and adjust datatypes */

		Token currentToken = { 0 };
		EasyScript_char c;
		EasyScript_int state = 0;
		EasyScript_int lexStart;
		EasyScript_int lexEnd;
		EasyScript_int lexLength;
		EasyScript_string lexeme;

		lexeme = (EasyScript_string)malloc(VID_LEN * sizeof(EasyScript_char));
		if (!lexeme) return currentToken;  // Defensive check

		lexeme[0] = EOS_CHR;  // Initialize lexeme

		while (1) {
			c = readerGetChar(sourceBuffer);

			if (c < 0 || c >= NCHAR) return currentToken;  // Defensive check for EOF

		/* ------------------------------------------------------------------------
			Part 1: Implementation of token driven scanner.
			Every token is possessed by its own dedicated code
			-----------------------------------------------------------------------
		*/

		/* TO_DO: All patterns that do not require accepting functions */
			switch (c) {
			case SPC_CHR:
			case TAB_CHR:
				break;
			case NWL_CHR:
				line++;
				break;
			case SCL_CHR:
				currentToken.code = SEMI_T;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;
			case LPR_CHR:
				currentToken.code = LPR_T;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;
			case RPR_CHR:
				currentToken.code = RPR_T;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;
			case LBR_CHR:
				currentToken.code = LBR_T;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;
			case RBR_CHR:
				currentToken.code = RBR_T;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;
			case EOF_CHR:
				currentToken.code = SEOF_T;
				scData.scanHistogram[currentToken.code]++;
				currentToken.attribute.seofType = (c == EOS_CHR) ? SEOF_0 : SEOF_255;
				return currentToken;
			default:  // General case for finite state processing
				state = nextState(state, c);
				lexStart = readerGetPosRead(sourceBuffer) - 1;
				readerSetMark(sourceBuffer, lexStart);

				// Process characters until a final state is reached
				while (stateType[state] == NOFS) {
					c = readerGetChar(sourceBuffer);
					state = nextState(state, c);
				}

				if (stateType[state] == FSWR) {
					readerRetract(sourceBuffer);
				}

				lexEnd = readerGetPosRead(sourceBuffer);
				lexLength = lexEnd - lexStart;
				lexemeBuffer = readerCreate(lexLength + 2, 0, MODE_FIXED);

				if (!lexemeBuffer) {
					fprintf(stderr, "Scanner error: Cannot create buffer\n");
					exit(1);
				}

				readerRestore(sourceBuffer);
				for (EasyScript_int i = 0; i < lexLength; i++) {
					readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
				}
				readerAddChar(lexemeBuffer, READER_TERMINATOR);
				lexeme = readerGetContent(lexemeBuffer, 0);

				if (!lexeme) return currentToken;  // Defensive programming check

				currentToken = (*finalStateTable[state])(lexeme);
				readerRestore(lexemeBuffer);
				return currentToken;
			}
		}
}
			/* ------------------------------------------------------------------------
				Part 2: Implementation of Finite State Machine (DFA) or Transition Table driven Scanner
				Note: Part 2 must follow Part 1 to catch the illegal symbols
				-----------------------------------------------------------------------
			*/

			/* TO_DO: Adjust / check the logic for your language */

default: // general case
	state = nextState(state, c);
	lexStart = readerGetPosRead(sourceBuffer) - 1;
	readerSetMark(sourceBuffer, lexStart);
	int pos = 0;
	while (stateType[state] == NOFS) {
		c = readerGetChar(sourceBuffer);
		state = nextState(state, c);
		pos++;
	}
	if (stateType[state] == FSWR)
		readerRetract(sourceBuffer);
	lexEnd = readerGetPosRead(sourceBuffer);
	lexLength = lexEnd - lexStart;
	lexemeBuffer = readerCreate((EasyScript_int)lexLength + 2, 0, MODE_FIXED);
	if (!lexemeBuffer) {
		fprintf(stderr, "Scanner error: Can not create buffer\n");
		exit(1);
	}
	readerRestore(sourceBuffer);
	for (i = 0; i < lexLength; i++)
		readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
	readerAddChar(lexemeBuffer, READER_TERMINATOR);
	lexeme = readerGetContent(lexemeBuffer, 0);

	// Defensive programming check
	if (!lexeme) {
		return currentToken;
	}

	currentToken = (*finalStateTable[state])(lexeme);  // Get the token from final state function
	readerRestore(lexemeBuffer);  // Restore lexeme buffer for next token
	return currentToken;

}  // switch

}  // while

}  // tokenizer


/*
 ************************************************************
 * Get Next State
	The assert(int test) macro can be used to add run-time diagnostic to programs
	and to "defend" from producing unexpected results.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	(*) assert() is a macro that expands to an if statement;
	if test evaluates to false (zero) , assert aborts the program
	(by calling abort()) and sends the following message on stderr:
	(*) Assertion failed: test, file filename, line linenum.
	The filename and linenum listed in the message are the source file name
	and line number where the assert macro appears.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	If you place the #define NDEBUG directive ("no debugging")
	in the source code before the #include <assert.h> directive,
	the effect is to comment out the assert statement.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	The other way to include diagnostics in a program is to use
	conditional preprocessing as shown bellow. It allows the programmer
	to send more details describing the run-time problem.
	Once the program is tested thoroughly #define DEBUG is commented out
	or #undef DEBUG is used - see the top of the file.
 ***********************************************************
 */
 /* TO_DO: Just change the datatypes */

EasyScript_int nextState(EasyScript_int state, EasyScript_char c) {
	EasyScript_int col;
	EasyScript_int next;
	col = nextClass(c);
	next = transitionTable[state][col];

	if (DEBUG)
		printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
	assert(next != FS);
	if (DEBUG)
		if (next == FS) {
			printf("Scanner Error: Illegal state:\n");
			printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
			exit(1);
		}
	return next;
}

/*
 ************************************************************
 * Get Next Token Class
	* Create a function to return the column number in the transition table:
	* Considering an input char c, you can identify the "class".
	* For instance, a letter should return the column for letters, etc.
 ***********************************************************
 */
 /* TO_DO: Use your column configuration */

 /* Adjust the logic to return next column in TT */
 /*    [A-z],[0-9],    _,    &,   \', SEOF,    #, other
		L(0), D(1), U(2), M(3), Q(4), E(5), C(6),  O(7) */

EasyScript_int nextClass(EasyScript_char c) {
	EasyScript_int val = -1;
	switch (c) {
	case UND_CHR:
		val = 2;
		break;
	case AMP_CHR:
		val = 3;
		break;
	case QUT_CHR:
		val = 4;
		break;
	case HST_CHR:
		val = 6;
		break;
	case EOS_CHR:
	case EOF_CHR:
		val = 5;
		break;
	default:
		if (isalpha(c))
			val = 0;
		else if (isdigit(c))
			val = 1;
		else
			val = 7;
	}
	return val;
}

/*
 ************************************************************
 * Acceptance State Function COM
 *		Function responsible to identify COM (comments).
 ***********************************************************
 */
 /* TO_DO: Adjust the function for IL */

Token funcCMT(EasyScript_string lexeme) {
	Token currentToken = { 0 };
	EasyScript_int len = strlen(lexeme);

	currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);
	for (EasyScript_int i = 1; i < len - 1; i++) {
		if (lexeme[i] == NWL_CHR) line++;
	}
	currentToken.code = CMT_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}


/*
 ************************************************************
 * Acceptance State Function IL
 *		Function responsible to identify IL (integer literals).
 * - It is necessary respect the limit (ex: 2-byte integer in C).
 * - In the case of larger lexemes, error shoul be returned.
 * - Only first ERR_LEN characters are accepted and eventually,
 *   additional three dots (...) should be put in the output.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for IL */

Token funcIL(EasyScript_string lexeme) {
	Token currentToken = { 0 };
	EasyScript_long tlong = atol(lexeme);

	if (tlong >= 0 && tlong <= SHRT_MAX) {
		currentToken.code = INL_T;
		scData.scanHistogram[currentToken.code]++;
		currentToken.attribute.intValue = (EasyScript_int)tlong;
	}
	else {
		currentToken = funcErr(lexeme);
	}
	return currentToken;
}


/*
 ************************************************************
 * Acceptance State Function ID
 *		In this function, the pattern for IDs must be recognized.
 *		Since keywords obey the same pattern, is required to test if
 *		the current lexeme matches with KW from language.
 *	- Remember to respect the limit defined for lexemes (VID_LEN) and
 *	  set the lexeme to the corresponding attribute (vidLexeme).
 *    Remember to end each token with the \0.
 *  - Suggestion: Use "strncpy" function.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for ID */

Token funcID(EasyScript_string lexeme) {
	Token currentToken = { 0 };
	size_t length = strlen(lexeme);

	if (lexeme[length - 1] == AMP_CHR) {
		currentToken.code = MNID_T;
		strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
	}
	else {
		currentToken = funcKEY(lexeme);
	}
	return currentToken;
}

/*
************************************************************
 * Acceptance State Function SL
 *		Function responsible to identify SL (string literals).
 * - The lexeme must be stored in the String Literal Table
 *   (stringLiteralTable). You need to include the literals in
 *   this structure, using offsets. Remember to include \0 to
 *   separate the lexemes. Remember also to incremente the line.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for SL */

Token funcSL(EasyScript_string lexeme) {
	Token currentToken = { 0 };
	EasyScript_int len = strlen(lexeme);

	currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);
	for (EasyScript_int i = 1; i < len - 1; i++) {
		if (lexeme[i] == NWL_CHR) line++;
		if (!readerAddChar(stringLiteralTable, lexeme[i])) {
			currentToken.code = RTE_T;
			strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
			errorNumber = RTE_CODE;
			return currentToken;
		}
	}
	readerAddChar(stringLiteralTable, EOS_CHR);
	currentToken.code = STR_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}


/*
************************************************************
 * This function checks if one specific lexeme is a keyword.
 * - Tip: Remember to use the keywordTable to check the keywords.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for Keywords */

Token funcKEY(EasyScript_string lexeme) {
	Token currentToken = { 0 };

	for (EasyScript_int j = 0; j < KWT_SIZE; j++) {
		if (strcmp(lexeme, keywordTable[j]) == 0) {
			currentToken.code = KW_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.codeType = j;
			return currentToken;
		}
	}
	return funcErr(lexeme);
}


/*
************************************************************
 * Acceptance State Function Error
 *		Function responsible to deal with ERR token.
 * - This function uses the errLexeme, respecting the limit given
 *   by ERR_LEN. If necessary, use three dots (...) to use the
 *   limit defined. The error lexeme contains line terminators,
 *   so remember to increment line.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for Errors */

Token funcErr(EasyScript_string lexeme) {
	Token currentToken = { 0 };
	size_t len = strlen(lexeme);

	if (len > ERR_LEN) {
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		strcat(currentToken.attribute.errLexeme, "...");
	}
	else {
		strcpy(currentToken.attribute.errLexeme, lexeme);
	}

	for (EasyScript_int i = 0; i < len; i++) {
		if (lexeme[i] == NWL_CHR) line++;
	}

	currentToken.code = ERR_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}



/*
 ************************************************************
 * The function prints the token returned by the scanner
 ***********************************************************
 */

EasyScript_void printToken(Token t) {
	extern EasyScript_string keywordTable[]; /* link to keyword table in */
	switch (t.code) {
	case RTE_T:
		printf("RTE_T\t\t%s", t.attribute.errLexeme);
		printf("\n");
		break;
	case ERR_T:
		printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case MNID_T:
		printf("MNID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("STR_T\t\t%d\t ", (EasyScript_int)t.attribute.codeType);
		printf("%s\n", readerGetContent(stringLiteralTable, (EasyScript_int)t.attribute.codeType));
		break;
	case LPR_T:
		printf("LPR_T\n");
		break;
	case RPR_T:
		printf("RPR_T\n");
		break;
	case LBR_T:
		printf("LBR_T\n");
		break;
	case RBR_T:
		printf("RBR_T\n");
		break;
	case KW_T:
		printf("KW_T\t\t%s\n", keywordTable[t.attribute.codeType]);
		break;
	case CMT_T:
		printf("CMT_T\n");
		break;
	case EOS_T:
		printf("EOS_T\n");
		break;
	default:
		printf("Scanner error: invalid token code: %d\n", t.code);
	}
}

/*
 ************************************************************
 * The function prints statistics of tokens
 * Param:
 *	- Scanner data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */
EasyScript_void printScannerData(ScannerData scData) {
	/* Print Scanner statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	int cont = 0;
	for (cont = 0; cont < NUM_TOKENS; cont++) {
		if (scData.scanHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", tokenStrTable[cont], "]=", scData.scanHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}

/*
TO_DO: (If necessary): HERE YOU WRITE YOUR ADDITIONAL FUNCTIONS (IF ANY).
*/