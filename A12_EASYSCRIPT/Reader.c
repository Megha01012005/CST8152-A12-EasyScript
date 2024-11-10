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
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: May 01 2024
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */

#include <ctype.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

 /*
 ***********************************************************
 * Function name: readerCreate
 * Purpose: Creates the buffer reader according to capacity, increment
	 factor and operational mode ('f', 'a', 'm')
 * Author: Svillen Ranev / Paulo Sousa
 * History/Versions: S22
 * Called functions: calloc(), malloc()
 * Parameters:
 *   size = initial capacity
 *   increment = increment factor
 *   mode = operational mode
 * Return value: bPointer (pointer to reader)
 * Algorithm: Allocation of memory according to inicial (default) values.
 * TODO ......................................................
 *	- Adjust datatypes for your LANGUAGE.
 *   - Use defensive programming
 *	- Check boundary conditions
 *	- Check flags.
 *************************************************************
 */

BufferPointer readerCreate(EasyScript_int size, EasyScript_int increment, EasyScript_char mode) {
	BufferPointer readerPointer;
	EasyScript_int count = 0;
	/* TO_DO: Defensive programming */

	/* If size is zero or negative, use the default size */
	if (size <= 0) {
		size = READER_DEFAULT_SIZE;
	}

	/* If increment is zero or negative, use the default increment */
	if (increment <= 0) {
		increment = READER_DEFAULT_INCREMENT;
	}

	/* Validate mode: If mode is invalid, default to MODE_FIXED */
	if (mode != MODE_FIXED && mode != MODE_ADDIT && mode != MODE_MULTI) {
		mode = MODE_FIXED;
	}

	readerPointer = (BufferPointer)calloc(1, sizeof(Buffer));

	if (!readerPointer)
		return EasyScript_INVALID;

	readerPointer->content = (EasyScript_string)malloc(size);

	if (!readerPointer->content) {
		/* If memory allocation for the content fails, free the reader and return an error */
		free(readerPointer);
		return EasyScript_INVALID;
	}
	/* TO_DO: Defensive programming */
	/* TO_DO: Initialize the histogram */
	for (EasyScript_int i = 0; i < NCHAR; i++) {
		readerPointer->histogram[i] = 0;  /* Histogram tracks character occurrences */
	}
	/* TO_DO: Initialize errors */
	readerPointer->numReaderErrors = 0;

	readerPointer->mode = mode;
	readerPointer->size = size;
	readerPointer->increment = increment;

	/* TO_DO: Initialize flags */
	readerPointer->flags.isEmpty = EasyScript_TRUE;  /* Initially, the buffer is empty */
	readerPointer->flags.isFull = EasyScript_FALSE;  /* The buffer is not full at the beginning */
	readerPointer->flags.isMoved = EasyScript_FALSE;  /* No memory reallocation yet */
	readerPointer->flags.isRead = EasyScript_FALSE;  /* No content has been read yet */

	/* Initialize positions */
	readerPointer->positions.wrte = 0;  /* Write position starts at 0 */
	readerPointer->positions.read = 0;  /* Read position starts at 0 */
	readerPointer->positions.mark = 0;  /* Mark position starts at 0 */

	/* TO_DO: Default checksum */
	readerPointer->checksum = 0;

	return readerPointer;
}


/*
***********************************************************
* Function name: readerAddChar
* Purpose: Adds a char to buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   ch = char to be added
* Return value:
*	readerPointer (pointer to Buffer Reader)
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

BufferPointer readerAddChar(BufferPointer readerPointer, EasyScript_char ch) {
	EasyScript_string tempReader = EasyScript_INVALID;
	EasyScript_int newSize = 0;
	EasyScript_char tempChar = ' ';
	/* TO_DO: Defensive programming */

	if (readerPointer == EasyScript_INVALID || readerPointer->content == EasyScript_INVALID) {
		return EasyScript_INVALID;
	}

	/* Check if buffer is full: wrte position >= size */
	if (readerPointer->positions.wrte >= readerPointer->size) {
		/* Buffer is full, handle based on mode */
		switch (readerPointer->mode) {
		case MODE_FIXED:
			/* Fixed mode: buffer size cannot be changed, terminate with EOF */
			readerPointer->flags.isFull = EasyScript_TRUE;
			readerPointer->content[readerPointer->positions.wrte - 1] = READER_TERMINATOR;
			return readerPointer;

		case MODE_ADDIT:
			/* Additive mode: increase buffer size by increment */
			newSize = readerPointer->size + readerPointer->increment;
			if (newSize <= 0) {  /* Check for overflow */
				return EasyScript_INVALID;
			}
			break;

		case MODE_MULTI:
			/* Multiplicative mode: multiply buffer size by increment */
			newSize = readerPointer->size * readerPointer->increment;
			if (newSize <= readerPointer->size || newSize <= 0) {  /* Check for overflow */
				return EasyScript_INVALID;
			}
			break;

		default:
			/* Invalid mode */
			return EasyScript_INVALID;
		}

		/* Try to reallocate memory for the buffer */
		tempReader = (EasyScript_string)realloc(readerPointer->content, newSize);

		if (tempReader == EasyScript_INVALID) {
			/* Memory reallocation failed */
			return EasyScript_INVALID;
		}

		/* Memory reallocation succeeded, update buffer size */
		readerPointer->content = tempReader;
		readerPointer->size = newSize;
		readerPointer->flags.isMoved = EasyScript_TRUE;
	}

	/* Add the character to the buffer and update write position */
	readerPointer->content[readerPointer->positions.wrte++] = ch;

	/* Update flags */
	readerPointer->flags.isEmpty = EasyScript_FALSE;  /* Buffer is no longer empty */
	readerPointer->flags.isFull = (readerPointer->positions.wrte == readerPointer->size);  /* Check if buffer is full */

	/* Update the histogram for the character */
	if ((EasyScript_int)ch >= 0 && (EasyScript_int)ch < NCHAR) {
		readerPointer->histogram[(EasyScript_int)ch]++;  /* Increment the count for this character */
	}

	return readerPointer;

	}
	

	/*
	***********************************************************
	* Function name: readerClear
	* Purpose: Clears the buffer reader
	* Parameters:
	*   readerPointer = pointer to Buffer Reader
	* Return value:
	*	Boolean value about operation success
	* TO_DO:
	*   - Use defensive programming
	*	- Check boundary conditions
	*	- Adjust for your LANGUAGE.
	*************************************************************
	*/

	EasyScript_bool readerClear(BufferPointer const readerPointer) {
		
		/* TO_DO: Defensive programming */
		if (readerPointer == EasyScript_INVALID || readerPointer->content == EasyScript_INVALID) {
			return EasyScript_FALSE;  /* Return false if invalid */
		}

		/* TO_DO: Adjust the write, mark and read to zero */
		readerPointer->positions.wrte = 0;  /* Reset write position */
		readerPointer->positions.read = 0;  /* Reset read position */
		readerPointer->positions.mark = 0;  /* Reset mark position */

		/* TO_DO: Adjust flags */
		readerPointer->flags.isEmpty = EasyScript_TRUE;  /* Buffer is now empty */
		readerPointer->flags.isFull = EasyScript_FALSE;  /* Buffer is not full */
		readerPointer->flags.isMoved = EasyScript_FALSE;  /* Buffer content hasn't been reallocated after clear */

		return EasyScript_TRUE;
	}

	/*
	***********************************************************
	* Function name: readerFree
	* Purpose: Releases the buffer address
	* Parameters:
	*   readerPointer = pointer to Buffer Reader
	* Return value:
	*   Boolean value about operation success
	* TO_DO:
	*   - Use defensive programming
	*   - Check boundary conditions
	*   - Adjust for your LANGUAGE.
	*************************************************************
	*/

	EasyScript_bool readerFree(BufferPointer const readerPointer) {
		/* TO_DO: Defensive programming */
		if (readerPointer == EasyScript_INVALID) {
			return EasyScript_FALSE;  /* Return false if readerPointer is invalid */
		}

		/* TO_DO: Free pointers */
		if (readerPointer->content != EasyScript_INVALID) {
			free(readerPointer->content);  /* Free the content buffer */
			readerPointer->content = EasyScript_INVALID;  /* Avoid dangling pointers */
		}

		free(readerPointer);  /* Free the readerPointer itself */

		return EasyScript_TRUE;
	}
	

	/*
	***********************************************************
	* Function name: readerIsFull
	* Purpose: Checks if buffer reader is full
	* Parameters:
	*   readerPointer = pointer to Buffer Reader
	* Return value:
	*	Boolean value about operation success
	* TO_DO:
	*   - Use defensive programming
	*	- Check boundary conditions
	*	- Adjust for your LANGUAGE.
	*************************************************************
	*/
	EasyScript_bool readerIsFull(BufferPointer const readerPointer) {
		/* TO_DO: Defensive programming */
		if (readerPointer == EasyScript_INVALID || readerPointer->content == EasyScript_INVALID) {
			return EasyScript_FALSE;  /* Return false if readerPointer is invalid */
		}

		/* TO_DO: Check flag if buffer is FUL */
		if (readerPointer->flags.isFull == EasyScript_TRUE) {
			return EasyScript_TRUE;  /* Return true if the buffer is full */
		}

		/* Otherwise, return false if buffer is not full */
		return EasyScript_FALSE;
	}
	

	/*
	***********************************************************
	* Function name: readerIsEmpty
	* Purpose: Checks if buffer reader is empty.
	* Parameters:
	*   readerPointer = pointer to Buffer Reader
	* Return value:
	*	Boolean value about operation success
	* TO_DO:
	*   - Use defensive programming
	*	- Check boundary conditions
	*	- Adjust for your LANGUAGE.
	*************************************************************
	*/
	EasyScript_bool readerIsEmpty(BufferPointer const readerPointer) {
		/* TO_DO: Defensive programming */
		if (readerPointer == EasyScript_INVALID || readerPointer->content == EasyScript_INVALID) {
			return EasyScript_FALSE;  /* Return false if readerPointer or content is invalid */
		}

		/* TO_DO: Check flag if buffer is EMP */
		if (readerPointer->flags.isEmpty == EasyScript_TRUE) {
			return EasyScript_TRUE;  /* Return true if the buffer is marked as empty */
		}

		/* Otherwise, return false */
		return EasyScript_FALSE;
	}

/*
***********************************************************
* Function name: readerSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
EasyScript_bool readerSetMark(BufferPointer const readerPointer, EasyScript_int mark) {
	/* TO_DO: Defensive programming */
	if (readerPointer == EasyScript_INVALID || readerPointer->content == EasyScript_INVALID) {
		return EasyScript_FALSE;  /* Return false if readerPointer or content is invalid */
	}

	/* TO_DO: Adjust mark */
	if (mark < 0 || mark >= readerPointer->positions.wrte) {
		return EasyScript_FALSE;  /* Return false if the mark is out of bounds */
	}

	readerPointer->positions.mark = mark;  /* Set the mark position */

	return EasyScript_TRUE;  /* Return success */
}


/*
***********************************************************
* Function name: readerPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
EasyScript_int readerPrint(BufferPointer const readerPointer) {
	EasyScript_int cont = 0;
	EasyScript_char c;
	
	/* TO_DO: Defensive programming (including invalid chars) */
	if (readerPointer == EasyScript_INVALID || readerPointer->content == EasyScript_INVALID) {
		return EasyScript_ERROR;  /* Return error if readerPointer or content is invalid */
	}

	if (readerPointer->flags.isEmpty == EasyScript_TRUE) {
		return 0;  /* If buffer is empty, return 0 since there is nothing to print */
	}

	c = readerGetChar(readerPointer);

	while (cont < readerPointer->positions.wrte) {
		if (c == READER_TERMINATOR) {
			break;  /* Stop printing if terminator is reached */
		}

		cont++;
		printf("%c", c);
		c = readerGetChar(readerPointer);
	}
	return cont;
}

/*
***********************************************************
* Function name: readerLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   fileDescriptor = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
EasyScript_int readerLoad(BufferPointer readerPointer, FILE* const fileDescriptor) {
	EasyScript_int size = 0;
	EasyScript_char c;

	/* TO_DO: Defensive programming */
	if (readerPointer == EasyScript_INVALID || readerPointer->content == EasyScript_INVALID) {
		printf("Error: Invalid buffer or file\n");
		return EasyScript_ERROR;
	}
	if (fileDescriptor == NULL) {
		printf("Error: Could not open file\n");
		return EasyScript_ERROR;
	}


	/* TO_DO: Defensive programming */
	while ((c = (EasyScript_char)fgetc(fileDescriptor)) != EOF) {
		if (!readerAddChar(readerPointer, c)) {
			printf("Error: Failed to add character to buffer\n");
			return EasyScript_ERROR;
		}
		size++;
		/*printf("Added character: %c\n", c);  // Debug: Check characters being added*/
	}

	return size;  // Return the number of characters read
}

/*
***********************************************************
* Function name: readerRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
EasyScript_bool readerRecover(BufferPointer const readerPointer) {

	/* TO_DO: Defensive programming */
	if (readerPointer == EasyScript_INVALID || readerPointer->content == EasyScript_INVALID) {
		return EasyScript_FALSE;  /* Return false if readerPointer or content is invalid */
	}

	/* TO_DO: Recover positions: read and mark must be zero */
	readerPointer->positions.read = 0;  /* Reset the read position to the start */
	readerPointer->positions.mark = 0;  /* Reset the mark position to the start */

	/* TO_DO: Update flags */
	readerPointer->flags.isEmpty = (readerPointer->positions.wrte == 0) ? EasyScript_TRUE : EasyScript_FALSE;
	readerPointer->flags.isFull = EasyScript_FALSE;  /* Buffer can't be full if we reset read and mark */

	return EasyScript_TRUE;
}


/*
***********************************************************
* Function name: readerRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
EasyScript_bool readerRetract(BufferPointer const readerPointer) {
	
	/* TO_DO: Defensive programming */
	if (readerPointer == EasyScript_INVALID || readerPointer->content == EasyScript_INVALID) {
		return EasyScript_FALSE;  /* Return false if readerPointer or content is invalid */
	}
	
	/* TO_DO: Retract (return 1 pos read) */
	if (readerPointer->positions.read > 0) {
		readerPointer->positions.read--;  /* Move the read position one step back */
		return EasyScript_TRUE;  /* Return success */
	}	
	return EasyScript_FALSE;
}


/*
***********************************************************
* Function name: readerRestore
* Purpose: Resets the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
EasyScript_bool readerRestore(BufferPointer const readerPointer) {
	
	/* TO_DO: Defensive programming */
	if (readerPointer == EasyScript_INVALID || readerPointer->content == EasyScript_INVALID) {
		return EasyScript_FALSE;  /* Return false if readerPointer or content is invalid */
	}

	/* TO_DO: Restore positions (read to mark) */
	if (readerPointer->positions.mark >= 0 && readerPointer->positions.mark <= readerPointer->positions.wrte) {
		readerPointer->positions.read = readerPointer->positions.mark;  /* Set the read position to the mark */
		return EasyScript_TRUE;  /* Return success */
	}
	return EasyScript_FALSE;  /* Return false if mark position is out of bounds */
}


/*
***********************************************************
* Function name: readerGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
EasyScript_char readerGetChar(BufferPointer const readerPointer) {
	
	/* TO_DO: Defensive programming */
	if (readerPointer == EasyScript_INVALID || readerPointer->content == EasyScript_INVALID) {
		return READER_TERMINATOR;  /* Return terminator if readerPointer or content is invalid */
	}

	/* TO_DO: Check condition to read/wrte */
	if (readerPointer->positions.read >= readerPointer->positions.wrte) {
		return READER_TERMINATOR;  /* Return terminator if read position is beyond the write position */
	}
	return readerPointer->content[readerPointer->positions.read++];
}


/*
***********************************************************
* Function name: readerGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
EasyScript_string readerGetContent(BufferPointer const readerPointer, EasyScript_int pos) {
	
	/* TO_DO: Defensive programming */
	if (readerPointer == EasyScript_INVALID || readerPointer->content == EasyScript_INVALID) {
		return EasyScript_INVALID;  /* Return invalid if readerPointer or content is invalid */
	}

	/* TO_DO: Check boundary conditions */
	if (pos < 0 || pos >= readerPointer->positions.wrte) {
		return EasyScript_INVALID;  /* Return invalid if the position is out of bounds */
	}

	return readerPointer->content + pos;
}


/*
***********************************************************
* Function name: readerGetPosRead
* Purpose: Returns the value of getCPosition.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The read position offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
EasyScript_int readerGetPosRead(BufferPointer const readerPointer) {
	
	/* TO_DO: Defensive programming */
	if (readerPointer == EasyScript_INVALID || readerPointer->content == EasyScript_INVALID) {
		return EasyScript_ERROR;  /* Return error if readerPointer or content is invalid */
	}

	/* TO_DO: Return read */
	return readerPointer->positions.read;
}


/*
***********************************************************
* Function name: readerGetPosWrte
* Purpose: Returns the position of char to be added
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Write position
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
EasyScript_int readerGetPosWrte(BufferPointer const readerPointer) {
	
	/* TO_DO: Defensive programming */
	if (readerPointer == EasyScript_INVALID || readerPointer->content == EasyScript_INVALID) {
		return EasyScript_ERROR;  /* Return error if readerPointer or content is invalid */
	}

	/* TO_DO: Return wrte */
	return readerPointer->positions.wrte;
}


/*
***********************************************************
* Function name: readerGetPosMark
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Mark position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
EasyScript_int readerGetPosMark(BufferPointer const readerPointer) {
	
	/* TO_DO: Defensive programming */
	if (readerPointer == EasyScript_INVALID || readerPointer->content == EasyScript_INVALID) {
		return EasyScript_ERROR;  /* Return error if readerPointer or content is invalid */
	}

	/* TO_DO: Return mark */
	return readerPointer->positions.mark;
}


/*
***********************************************************
* Function name: readerGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Size of buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
EasyScript_int readerGetSize(BufferPointer const readerPointer) {
	
	/* TO_DO: Defensive programming */
	if (readerPointer == EasyScript_INVALID || readerPointer->content == EasyScript_INVALID) {
		return EasyScript_ERROR;  /* Return error if readerPointer or content is invalid */
	}

	/* TO_DO: Return size */
	return readerPointer->size;
}

/*
***********************************************************
* Function name: readerGetInc
* Purpose: Returns the buffer increment.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The Buffer increment.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
EasyScript_int readerGetInc(BufferPointer const readerPointer) {
	
	/* TO_DO: Defensive programming */
	if (readerPointer == EasyScript_INVALID || readerPointer->content == EasyScript_INVALID) {
		return EasyScript_ERROR;  /* Return error if readerPointer or content is invalid */
	}

	/* TO_DO: Return increment */
	return readerPointer->increment;
}

/*
***********************************************************
* Function name: readerGetMode
* Purpose: Returns the operational mode
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Operational mode.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
EasyScript_char readerGetMode(BufferPointer const readerPointer) {
	
	/* TO_DO: Defensive programming */
	if (readerPointer == EasyScript_INVALID || readerPointer->content == EasyScript_INVALID) {
		return READER_TERMINATOR;  /* Return terminator if readerPointer or content is invalid */
	}

	/* TO_DO: Return mode */
	return readerPointer->mode;
}

/*
***********************************************************
* Function name: readerShowStat
* Purpose: Shows the char statistic.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value: (Void)
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
EasyScript_void readerPrintStat(BufferPointer const readerPointer) {
	
	/* TO_DO: Defensive programming */
	if (readerPointer == EasyScript_INVALID || readerPointer->content == EasyScript_INVALID) {
		printf("Error: Invalid buffer pointer.\n");
		return;  /* Exit if the reader pointer or content is invalid */
	}

	/* TO_DO: Updates the histogram */
	printf("Reader statistics...\n");
	for (EasyScript_int i = 0; i < NCHAR; i++) {
		if (readerPointer->histogram[i] > 0) {
			/* Print in the required format: B[<character>]=<occurrences> */
			printf("B[%c]=%d, ", (i >= 32 && i <= 126) ? i : ' ', readerPointer->histogram[i]);

			/* Newline for every 8 characters to keep the format readable */
			if ((i + 1) % 8 == 0) {
				printf("\n");
			}
		}
	}
	printf("\n");  /* Ensure a newline at the end */
}

/*
***********************************************************
* Function name: readerNumErrors
* Purpose: Returns the number of errors found.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of errors.
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
EasyScript_int readerGetNumErrors(BufferPointer const readerPointer) {
	
	/* TO_DO: Defensive programming */
	if (readerPointer == EasyScript_INVALID || readerPointer->content == EasyScript_INVALID) {
		return EasyScript_ERROR;  /* Return error if readerPointer or content is invalid */
	}

	/* TO_DO: Returns the number of errors */
	return readerPointer->numReaderErrors;
}

/*
***********************************************************
* Function name: readerCalcChecksum
* Purpose: Calculates the checksum of the reader (8 bits).
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	[None]
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

EasyScript_void readerCalcChecksum(BufferPointer readerPointer) {
	
	/* TO_DO: Defensive programming */
	if (readerPointer == EasyScript_INVALID || readerPointer->content == EasyScript_INVALID) {
		printf("Error: Invalid buffer pointer.\n");
		return;  /* Exit if the readerPointer or content is invalid */
	}

	/* TO_DO: Calculate checksum */
	readerPointer->checksum = 0;  /* Initialize checksum to 0 */
	for (EasyScript_int i = 0; i < readerPointer->positions.wrte; i++) {
		readerPointer->checksum += readerPointer->content[i];  /* Add the ASCII value of each character */
	}
	readerPointer->checksum %= 256;  /* Ensure the checksum is an 8-bit value (0-255) */
}

/*
***********************************************************
* Function name: readerPrintFlags
* Purpose: Sets the checksum of the reader (4 bits).
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	[None]
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

EasyScript_bool readerPrintFlags(BufferPointer readerPointer) {
	
	/* TO_DO: Defensive programming */
	if (readerPointer == EasyScript_INVALID || readerPointer->content == EasyScript_INVALID) {
		printf("Error: Invalid buffer pointer.\n");
		return EasyScript_FALSE;  /* Return false if readerPointer or content is invalid */
	}

	/* TO_DO: Shows flags */
	printf("Buffer Flags:\n");
	printf("isEmpty: %s\n", readerPointer->flags.isEmpty ? "TRUE" : "FALSE");
	printf("isFull: %s\n", readerPointer->flags.isFull ? "TRUE" : "FALSE");
	printf("isRead: %s\n", readerPointer->flags.isRead ? "TRUE" : "FALSE");
	printf("isMoved: %s\n", readerPointer->flags.isMoved ? "TRUE" : "FALSE");

	return EasyScript_TRUE;
}
