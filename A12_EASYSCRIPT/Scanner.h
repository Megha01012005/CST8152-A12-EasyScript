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
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@           %&@@@@@@@@@@@     @@    ”
# ECHO "    @@       @%% (@@@@@@@@@  @     @@    ”
# ECHO "    @@      @& @   @ @       @     @@    ”
# ECHO "    @@     @ @ %  / /   @@@@@@     @@    ”
# ECHO "    @@      & @ @  @@              @@    ”
# ECHO "    @@       @/ @*@ @ @   @        @@    ”
# ECHO "    @@           @@@@  @@ @ @      @@    ”
# ECHO "    @@            /@@    @@@ @     @@    ”
# ECHO "    @@     @      / /     @@ @     @@    ”
# ECHO "    @@     @ @@   /@/   @@@ @      @@    ”
# ECHO "    @@     @@@@@@@@@@@@@@@         @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@         S O F I A           @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "                                         "
# ECHO "[READER SCRIPT .........................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: Scanner.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2024
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1  /* Value for run-time error */

/* TO_DO: Define the number of tokens */
#define NUM_TOKENS 17

/* TO_DO: Define Token codes - Create your token classes */
/* Defined Token codes for EasyScript */
enum TOKENS {
	ERR_T,      /*  0: Error token */
	MNID_T,     /*  1: Method name identifier token (identifiers ending with &) */
	ID_T,       /*  2: Identifier token (variable names) */
	INL_T,      /*  3: Integer literal token */
	FPL_T,      /*  4: Floating-point literal token */
	STR_T,      /*  5: String literal token (surrounded by single quotes) */
	KW_T,       /*  6: Keyword token */
	CMT_T,      /*  7: Comment token (single-line and multi-line) */
	OP_T,       /*  8: Operator token (+, -, *, /, etc.) */
	LPR_T,      /*  9: Left parenthesis token */
	RPR_T,      /* 10: Right parenthesis token */
	LBR_T,      /* 11: Left brace token */
	RBR_T,      /* 12: Right brace token */
	SEMI_T,     /* 13: Semicolon token for end of statement */
	ASSIGN_T,   /* 14: Assignment operator token (=) */
	RTE_T,      /* 15: Run-time error token */
	SEOF_T      /* 16: Source end-of-file token */
};


/* TO_DO: Define the list of token strings */
static EasyScript_string tokenStrTable[NUM_TOKENS] = {
	"ERR_T",      /*  0: Error token */
	"MNID_T",     /*  1: Method name identifier token */
	"ID_T",       /*  2: Identifier token */
	"INL_T",      /*  3: Integer literal token */
	"FPL_T",      /*  4: Floating-point literal token */
	"STR_T",      /*  5: String literal token */
	"KW_T",       /*  6: Keyword token */
	"CMT_T",      /*  7: Comment token */
	"OP_T",       /*  8: Operator token */
	"LPR_T",      /*  9: Left parenthesis token */
	"RPR_T",      /* 10: Right parenthesis token */
	"LBR_T",      /* 11: Left brace token */
	"RBR_T",      /* 12: Right brace token */
	"SEMI_T",     /* 13: Semicolon token */
	"ASSIGN_T",   /* 14: Assignment operator token */
	"RTE_T",      /* 15: Run-time error token */
	"SEOF_T"      /* 16: Source end-of-file token */
};


/* TO_DO: Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT, OP_GE, OP_LE } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* TO_DO: Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	EasyScript_int codeType;                /* Integer attributes accessor */
	AriOperator arithmeticOperator;          /* Arithmetic operator attribute */
	RelOperator relationalOperator;          /* Relational operator attribute */
	LogOperator logicalOperator;             /* Logical operator attribute */
	EofOperator seofType;                    /* Source-end-of-file attribute */
	EasyScript_int intValue;                 /* Integer literal value */
	EasyScript_int keywordIndex;             /* Keyword index */
	EasyScript_int contentString;            /* Offset for string literals */
	EasyScript_float floatValue;             /* Floating-point literal value */
	EasyScript_char idLexeme[VID_LEN + 1];   /* Identifier token attribute */
	EasyScript_char errLexeme[ERR_LEN + 1];  /* Error token attribute */
} TokenAttribute;

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	EasyScript_byte flags;			/* Flags information */
	union {
		EasyScript_int intValue;				/* Integer value */
		EasyScript_float floatValue;			/* Float value */
		EasyScript_string stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	EasyScript_int code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

/* Scanner */
typedef struct scannerData {
	EasyScript_int scanHistogram[NUM_TOKENS];	/* Statistics of chars */
} ScannerData, * pScanData;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* TO_DO: Define lexeme FIXED classes */
/* EOF definitions */
#define EOS_CHR '\0'	// CH00
#define EOF_CHR 0xFF	// CH01
#define UND_CHR '_'		// CH02
#define AMP_CHR '&'		// CH03
#define QUT_CHR '\''	// CH04
#define HST_CHR '#'		// CH05
#define TAB_CHR '\t'	// CH06
#define SPC_CHR ' '		// CH07
#define NWL_CHR '\n'	// CH08
#define SCL_CHR ';'		// CH09
#define LPR_CHR '('		// CH10
#define RPR_CHR ')'		// CH11
#define LBR_CHR '{'		// CH12
#define RBR_CHR '}'		// CH13

/*  Special case tokens processed separately one by one in the token-driven part of the scanner:
 *  LPR_T, RPR_T, LBR_T, RBR_T, EOS_T, SEOF_T and special chars used for tokenis include _, & and ' */

 /* TO_DO: Error states and illegal state */
#define ESNR -1  // Error State No Retract
#define ESWR -2  // Error State With Retract
#define FSNR   1  // Final State No Retract
#define FSWR   2  // Final State With Retract
#define NOFS   0  // Not an accepting state

 /* TO_DO: State transition table definition */
#define NUM_STATES 11       // Total states in your transition table
#define TABLE_COLUMNS 10    // Total character classes (columns)

#define CHAR_CLASSES 8   // Ensure this is defined according to the number of character classes in your table
#define EOS_T 10         // Define token identifiers (e.g., SEMI_T, LPR_T, etc.) for each token type

/* TO_DO: Transition table - type of states defined in separate table */
static EasyScript_int transitionTable[NUM_STATES][TABLE_COLUMNS] = {
	{1, 6, 8, ESNR, 4, ESWR, ESWR, ESNR, ESWR, 1},  // S0: NOFS
	{2, 7, ESNR, ESNR, ESNR, ESNR, ESNR, ESNR, ESWR, ESNR},  // S1: NOFS
	{FSNR, FSNR, FSNR, FSNR, FSNR, FSNR, FSNR, FSNR, ESWR, FSNR},  // S2: FSNR (MID)
	{FSWR, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR, ESWR, FSWR},  // S3: FSWR (KEY)
	{ESNR, ESNR, ESNR, ESNR, 5, ESNR, ESNR, ESNR, FSNR, ESNR},  // S4: NOFS
	{FSNR, FSNR, FSNR, FSNR, FSNR, FSNR, FSNR, FSNR, FSNR, FSNR},  // S5: FSNR (SL)
	{ESNR, 7, ESNR, ESNR, ESNR, ESNR, ESNR, ESNR, ESWR, ESNR},  // S6: NOFS
	{FSNR, FSNR, FSNR, FSNR, FSNR, FSNR, FSNR, FSNR, FSNR, FSNR},  // S7: FSNR (NL)
	{ESNR, ESNR, 9, ESNR, ESNR, ESNR, ESNR, ESNR, FSNR, ESNR},  // S8: SEOF
	{ESNR, ESNR, ESNR, ESNR, 10, ESNR, ESNR, ESNR, ESWR, ESNR},  // S9: NOFS
	{FSWR, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR}  // S10: FSWR (CMT)
};

/* Define accepting states types */
#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */

/* TO_DO: Define list of acceptable states */
static EasyScript_int stateType[NUM_STATES] = {
	NOFS,  /* S0 */
	NOFS,  /* S1 */
	FSNR,  /* S2 (MNID) - Methods */
	FSWR,  /* S3 (KEY) */
	NOFS,  /* S4 */
	FSNR,  /* S5 (SL) */
	NOFS,  /* S6 */
	FSNR,  /* S7 (COM) */
	FSNR,  /* S8 (Err1 - no retract) */
	FSWR   /* S9 (Err2 - retract) */
};

/*
-------------------------------------------------
TO_DO: Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
EasyScript_int			startScanner(BufferPointer psc_buf);
static EasyScript_int	nextClass(EasyScript_char c);					/* character class function */
static EasyScript_int	nextState(EasyScript_int, EasyScript_char);		/* state machine function */
EasyScript_void			printScannerData(ScannerData scData);
Token				tokenizer(EasyScript_void);

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* TO_DO: Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(EasyScript_string lexeme);

/* Declare accepting states functions */
Token funcSL(EasyScript_string lexeme);
Token funcIL(EasyScript_string lexeme);
Token funcID(EasyScript_string lexeme);
Token funcCMT(EasyScript_string lexeme);
Token funcKEY(EasyScript_string lexeme);
Token funcErr(EasyScript_string lexeme);

/*
 * Accepting function (action) callback table (array) definition
 * If you do not want to use the typedef, the equvalent declaration is:
 */

 /* TO_DO: Define final state table */
static PTR_ACCFUN finalStateTable[NUM_STATES] = {
	NULL,		/* -    [00] */
	NULL,		/* -    [01] */
	funcID,		/* MNID	[02] */
	funcKEY,	/* KEY  [03] */
	NULL,		/* -    [04] */
	funcSL,		/* SL   [05] */
	NULL,		/* -    [06] */
	funcCMT,	/* COM  [07] */
	funcErr,	/* ERR1 [06] */
	funcErr		/* ERR2 [07] */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* TO_DO: Define the number of Keywords from the language */
#define KWT_SIZE 11

/* TO_DO: Define the list of keywords */
static EasyScript_string keywordTable[KWT_SIZE] = {
	"data",		/* KW00 */
	"code",		/* KW01 */
	"int",		/* KW02 */
	"float",		/* KW03 */
	"string",	/* KW04 */
	"if",		/* KW05 */
	"then",		/* KW06 */
	"else",		/* KW07 */
	"while",	/* KW08 */
	"do",		/* KW09 */
	"return"	/* KW10 */
};

/* NEW SECTION: About indentation */

/*
 * Scanner attributes to be used (ex: including: intendation data
 */

#define INDENT TAB_CHR  /* Tabulation */

 /* TO_DO: Should be used if no symbol table is implemented */
typedef struct languageAttributes {
	EasyScript_char indentationCharType;
	EasyScript_int indentationCurrentPos;
	/* TO_DO: Include any extra attribute to be used in your scanner (OPTIONAL and FREE) */
#define INDENT TAB_CHR  /* Tabulation character */

} LanguageAttributes;
	/* Number of errors */
	EasyScript_int numScannerErrors;

	/* Scanner data */
	ScannerData scData;

#endif
