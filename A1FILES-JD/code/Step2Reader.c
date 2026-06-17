/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2025
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (S25)  |"
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
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: Jan 01 2025
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
#include <string.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif

#ifndef READER_H_
#include "Step2Reader.h"
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

/* readerCreate — allocates memory and sets up a fresh buffer ready to store characters
 * size = how big it starts, factor = how much it grows when full
 * Structs used: Buffer (creates it), Flag (sets all flags), Position (not set here, zeroed by calloc) */
BufferPointer readerCreate(jd_intg size, jd_real factor) {
	BufferPointer readerPointer = NULL;             /* start with no buffer — will be assigned below */
	if (size <= 0)                                  /* if size is 0 or negative it is invalid */
		size = READER_DEFAULT_SIZE;                 /* replace it with the safe default value of 100 */
	if (factor <= 0)                                /* if factor is 0 or negative it is invalid */
		factor = READER_DEFAULT_FACTOR;             /* replace it with the safe default value of 0.5 */
	readerPointer = calloc(1, sizeof(Buffer));      /* create the Buffer struct in memory and set all fields to 0 */

	if (!readerPointer)                             /* if calloc returned NULL it means the computer ran out of memory */
		return NULL;                                /* stop here and return nothing — cannot continue without memory */
	jd_strg content = malloc(size);                 /* create a separate block of memory to hold the actual characters */
	if (readerPointer!=NULL && content!=NULL) {     /* make sure both the struct and the character array were created */
		readerPointer->content = content;           /* link the character array to the buffer struct */
	}

	if (!content) {                                 /* if malloc returned NULL the character array could not be created */
		free(readerPointer);                        /* free the struct we already made so we do not leak memory */
		return NULL;                                /* stop here and return nothing */
	}

	readerPointer->size = size;                     /* save 100 into the struct so the buffer knows its starting size */
	readerPointer->factor = factor;                 /* save 0.5 into the struct so the buffer knows how much to grow */

	readerPointer->numReaderErrors = 0;             /* set error count to 0 because no errors have happened yet */
	readerPointer->checkSum = 0;                    /* set checksum to 0 because no characters have been added yet */

	readerPointer->flags.isEmpty = JD_TRUE;         /* set isEmpty to true because the buffer has no characters yet */
	readerPointer->flags.isFull = JD_FALSE;         /* set isFull to false because the buffer is empty not full */
	readerPointer->flags.isRead = JD_FALSE;         /* set isRead to false because nothing has been read yet */
	readerPointer->flags.isMoved = JD_FALSE;        /* set isMoved to false because realloc has not been called yet */
	return readerPointer;                           /* return the finished buffer ready to use */
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

/* readerAddChar — adds one character to the buffer
 * If there is no room, grows the buffer with realloc first then adds the character
 * Structs used: Buffer (content, size, factor), Position (wrte), Flag (isFull, isMoved, isEmpty) */
BufferPointer readerAddChar(BufferPointer const readerPointer, jd_char ch) {
	jd_strg tempReader = NULL;          /* temporary variable to hold the new memory address after realloc */
	jd_intg newSize = 0;                /* temporary variable to hold the calculated new size when growing */
	if (!readerPointer)                 /* if the buffer does not exist there is nothing to add to */
		return NULL;                    /* stop here and return nothing */

	if (readerPointer->position.wrte * (jd_intg)sizeof(jd_char) < readerPointer->size) {   /* check if write position is still within the allocated size */
		readerPointer->flags.isFull = JD_FALSE;                                             /* there is still room so mark the buffer as not full */
	}
	else {                                                                                   /* write position has reached the end so the buffer is full */
		readerPointer->flags.isFull = JD_FALSE;                                             /* reset the full flag before we grow it */
		newSize = (jd_intg)(readerPointer->size + readerPointer->size * readerPointer->factor); /* new size = current size + (current size x 0.5) for example 100 becomes 150 */

		if (newSize <= readerPointer->size)             /* if the new size is not bigger than current something went wrong */
			return NULL;                                /* stop here and return nothing */
		tempReader = realloc(readerPointer->content, newSize);  /* ask the computer to resize the character array to the new size */
		if (!tempReader)                                /* if realloc returned NULL the computer could not give us more memory */
			return NULL;                                /* stop here and return nothing */
		if (tempReader != readerPointer->content)       /* if the memory address changed realloc moved it to a new location */
			readerPointer->flags.isMoved = JD_TRUE;    /* set isMoved to true so we know the memory was relocated */
		readerPointer->content = tempReader;            /* update content to point to the new memory location */
		readerPointer->size = newSize;                  /* update size to the new bigger value */
	}
	readerPointer->content[readerPointer->position.wrte++] = ch;   /* place the character at the current write position then move write cursor forward by 1 */
	readerPointer->histogram[(unsigned char)ch]++;                  /* add 1 to the count for this character in the histogram array */
	readerPointer->flags.isEmpty = JD_FALSE;                        /* set isEmpty to false because we just added a character */
	return readerPointer;                                           /* return the buffer with the new character in it */
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
/* readerClear — resets all positions to 0 so the buffer looks empty
 * Memory is kept, the content is just ignored. Use readerFree to fully destroy it.
 * Structs used: Position (wrte, read, mark), Flag (isEmpty, isFull, isRead, isMoved) */
jd_boln readerClear(BufferPointer const readerPointer) {
	if (!readerPointer)                         /* if the buffer does not exist we cannot clear it */
		return JD_FALSE;                        /* stop and return false to tell the caller it failed */
	readerPointer->position.wrte = 0;           /* move the write cursor back to position 0 so new chars overwrite from the start */
	readerPointer->position.read = 0;           /* move the read cursor back to position 0 so reading starts from the beginning */
	readerPointer->position.mark = 0;           /* move the bookmark back to position 0 */
	readerPointer->flags.isEmpty = JD_TRUE;     /* set isEmpty to true because the buffer is being treated as empty now */
	readerPointer->flags.isFull = JD_FALSE;     /* set isFull to false because an empty buffer cannot be full */
	readerPointer->flags.isRead = JD_FALSE;     /* set isRead to false because nothing has been read in the cleared buffer */
	readerPointer->flags.isMoved = JD_FALSE;    /* set isMoved to false to reset the relocation flag */
	return JD_TRUE;                             /* return true to tell the caller the clear worked */
}

/*
***********************************************************
* Function name: readerFree
* Purpose: Releases the buffer address
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
/* readerFree — releases all memory used by the buffer
 * Call this when you are completely done with it, otherwise memory leaks.
 * Structs used: Buffer (frees the whole thing and content inside) */
jd_boln readerFree(BufferPointer const readerPointer) {
	if (!readerPointer)                 /* if the buffer does not exist there is nothing to free */
		return JD_FALSE;                /* stop and return false */
	free(readerPointer->content);       /* free the character array first — this is the 100+ bytes holding the text */
	free(readerPointer);                /* free the buffer struct itself — this held size, factor, flags, positions etc */
	return JD_TRUE;                     /* return true to tell the caller the memory was freed successfully */
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
/* readerIsFull — checks if the buffer is full (returns true or false)
 * Structs used: Flag (isFull) */
jd_boln readerIsFull(BufferPointer const readerPointer) {
	if (!readerPointer)                         /* if the buffer does not exist we cannot check if it is full */
		return JD_FALSE;                        /* return false as a safe default */
	return readerPointer->flags.isFull;         /* return the isFull flag — 1 means full, 0 means still has space */
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
/* readerIsEmpty — checks if the buffer has nothing in it (returns true or false)
 * Structs used: Flag (isEmpty) */
jd_boln readerIsEmpty(BufferPointer const readerPointer) {
	if (!readerPointer)                         /* if the buffer does not exist we cannot check if it is empty */
		return JD_FALSE;                        /* return false as a safe default */
	return readerPointer->flags.isEmpty;        /* return the isEmpty flag — 1 means empty, 0 means has content */
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
/* readerSetMark — saves a position as a bookmark so you can jump back to it later
 * Structs used: Position (mark, wrte) */
jd_boln readerSetMark(BufferPointer const readerPointer, jd_intg mark) {
	if (!readerPointer)                                         /* if the buffer does not exist we cannot set a mark */
		return JD_FALSE;                                        /* return false to tell the caller it failed */
	if (mark < 0 || mark > readerPointer->position.wrte)       /* mark must be between 0 and the last written position */
		return JD_FALSE;                                        /* if out of range return false — invalid position */
	readerPointer->position.mark = mark;                        /* save the given position as the bookmark */
	return JD_TRUE;                                             /* return true to tell the caller the bookmark was saved */
}


/*
***********************************************************
* Function name: readerPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   
* Return value:
*	Number of chars printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
/* readerPrint — prints everything in the buffer to the screen one character at a time
 * Structs used: Buffer (content via readerGetChar) */
jd_intg readerPrint(BufferPointer const readerPointer) {
	if (!readerPointer)                                             /* if the buffer does not exist there is nothing to print */
		return 0;                                                   /* return 0 meaning zero characters were printed */
	jd_intg count = 0;                                             /* this variable keeps track of how many characters we printed */
	jd_char ch;                                                     /* this variable holds the current character being printed */
	while ((ch = readerGetChar(readerPointer)) != READER_TERMINATOR) { /* call readerGetChar and keep looping until it returns '\0' meaning the end */
		printf("%c", ch);                                           /* print the current character to the screen */
		count++;                                                    /* add 1 to the count each time we print a character */
	}
	return count;                                                   /* return the total number of characters that were printed */
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
/* readerLoad — the bridge between A1 and A2
 * Calls vigenereMem (from A1) to decode the file, then feeds every character
 * into readerAddChar one by one to fill the buffer. Returns how many chars were loaded.
 * Structs used: Buffer (content via readerAddChar), Position (wrte moves forward each add) */
jd_intg readerLoad(BufferPointer const readerPointer, jd_strg fileName) {
	if (!readerPointer || !fileName)                        /* if the buffer or file name is missing we cannot load anything */
		return READER_ERROR;                                /* return -1 to signal an error */

	jd_strg decoded = vigenereMem(fileName, STR_LANGNAME, DECYPHER); /* call A1's vigenereMem to decode the file using key "JD" and return the plain text in memory */

	if (!decoded)                                           /* if vigenereMem returned NULL the decoding failed */
		return READER_ERROR;                                /* return -1 to signal an error */

	jd_intg count = 0;                                     /* this variable counts how many characters were loaded into the buffer */
	jd_intg i = 0;                                         /* this index walks through the decoded string one character at a time */
	while (decoded[i] != '\0') {                           /* keep going until we reach the end of the decoded string */
		if (!readerAddChar(readerPointer, decoded[i]))      /* add the current character to the buffer using readerAddChar */
			return READER_ERROR;                            /* if readerAddChar failed return -1 to signal an error */
		count++;                                            /* add 1 to count because we successfully loaded one character */
		i++;                                               /* move to the next character in the decoded string */
	}
	free(decoded);                                          /* free the decoded string from memory since all characters are now in the buffer */
	return count;                                           /* return the total number of characters loaded into the buffer */
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
/* readerRecover — rewinds all the way back to the beginning of the buffer
 * Structs used: Position (read, mark) */
jd_boln readerRecover(BufferPointer const readerPointer) {
	if (!readerPointer)                     /* if the buffer does not exist we cannot rewind it */
		return JD_FALSE;                    /* return false to tell the caller it failed */
	readerPointer->position.read = 0;       /* set the read cursor back to 0 so reading starts from the very first character again */
	readerPointer->position.mark = 0;       /* set the bookmark back to 0 as well */
	return JD_TRUE;                         /* return true to tell the caller the rewind worked */
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
/* readerRetract — goes back one character in the buffer
 * Structs used: Position (read) */
jd_boln readerRetract(BufferPointer const readerPointer) {
	if (!readerPointer)                         /* if the buffer does not exist we cannot go back */
		return JD_FALSE;                        /* return false to tell the caller it failed */
	if (readerPointer->position.read <= 0)      /* if the read cursor is already at position 0 we cannot go back further */
		return JD_FALSE;                        /* return false because there is nowhere to retract to */
	readerPointer->position.read--;             /* subtract 1 from the read cursor to go back one character */
	return JD_TRUE;                             /* return true to tell the caller the retract worked */
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
/* readerRestore — jumps the read cursor back to wherever the bookmark was saved
 * Structs used: Position (read, mark) */
jd_boln readerRestore(BufferPointer const readerPointer) {
	if (!readerPointer)                                             /* if the buffer does not exist we cannot restore */
		return JD_FALSE;                                            /* return false to tell the caller it failed */
	readerPointer->position.read = readerPointer->position.mark;   /* set the read cursor to wherever the bookmark was saved */
	return JD_TRUE;                                                 /* return true to tell the caller the restore worked */
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
/* readerGetChar — returns the next character from the buffer and moves the cursor forward
 * Structs used: Position (read, wrte), Flag (isRead), Buffer (content) */
jd_char readerGetChar(BufferPointer const readerPointer) {
	if (!readerPointer)                                                     /* if the buffer does not exist we cannot read from it */
		return '\0';                                                        /* return an empty character as a safe default */
	if (readerPointer->position.read >= readerPointer->position.wrte) {    /* if the read cursor has reached or passed the last written position we are done */
		readerPointer->flags.isRead = JD_TRUE;                             /* set isRead to true to signal that all characters have been read */
		return READER_TERMINATOR;                                           /* return '\0' to tell the caller there are no more characters */
	}
	return readerPointer->content[readerPointer->position.read++];          /* return the character at the current read position then move the read cursor forward by 1 */
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
/* readerGetContent — returns a pointer to a specific position in the buffer
 * Structs used: Buffer (content), Position (wrte) */
jd_strg readerGetContent(BufferPointer const readerPointer, jd_intg pos) {
	if (!readerPointer)                                         /* if the buffer does not exist there is no content to return */
		return NULL;                                            /* return NULL as a safe default */
	if (pos < 0 || pos > readerPointer->position.wrte)         /* pos must be between 0 and the last written position or it is invalid */
		return NULL;                                            /* return NULL because the position is out of range */
	return readerPointer->content + pos;                        /* return a pointer that points to the character at position pos inside the buffer */
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
/* readerGetPosRead — returns where the read cursor currently is
 * Structs used: Position (read) */
jd_intg readerGetPosRead(BufferPointer const readerPointer) {
	if (!readerPointer)                         /* if the buffer does not exist we cannot return a read position */
		return READER_ERROR;                    /* return -1 to signal an error */
	return readerPointer->position.read;        /* return the current read cursor position — this is which character would be read next */
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
/* readerGetPosWrte — returns how many characters have been written into the buffer
 * Structs used: Position (wrte) */
jd_intg readerGetPosWrte(BufferPointer const readerPointer) {
	if (!readerPointer)                         /* if the buffer does not exist we cannot return a write position */
		return READER_ERROR;                    /* return -1 to signal an error */
	return readerPointer->position.wrte;        /* return the write cursor position — this is how many characters are currently in the buffer */

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
/* readerGetPosMark — returns where the bookmark is currently saved
 * Structs used: Position (mark) */
jd_intg readerGetPosMark(BufferPointer const readerPointer) {
	if (!readerPointer)                         /* if the buffer does not exist we cannot return a mark position */
		return READER_ERROR;                    /* return -1 to signal an error */
	return readerPointer->position.mark;        /* return the saved bookmark position */
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
/* readerGetSize — returns total memory allocated, not how many chars are in it
 * Structs used: Buffer (size) */
jd_intg readerGetSize(BufferPointer const readerPointer) {
	if (!readerPointer)                         /* if the buffer does not exist we cannot return a size */
		return READER_ERROR;                    /* return -1 to signal an error */
	return readerPointer->size;                 /* return the total memory allocated — this includes empty space not just used characters */
}

/*
***********************************************************
* Function name: readerGetFlags
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Flags from Buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
#define FLAGS_
#undef FLAGS_
#ifndef FLAGS_
/* readerPrintFlags — prints all 4 status flags, each showing 0 (false) or 1 (true)
 * Structs used: Flag (isEmpty, isFull, isMoved, isRead) */
jd_void readerPrintFlags(BufferPointer const readerPointer) {
	if (!readerPointer)                                                     /* if the buffer does not exist there are no flags to print */
		return;
	printf("Flag.isEmpty = %d\n", readerPointer->flags.isEmpty);           /* print isEmpty — 1 means nothing in buffer, 0 means has content */
	printf("Flag.isFull = %d\n", readerPointer->flags.isFull);             /* print isFull — 1 means buffer is full, 0 means still has space */
	printf("Flag.isMoved = %d\n", readerPointer->flags.isMoved);           /* print isMoved — 1 means realloc moved memory to new location, 0 means stayed same */
	printf("Flag.isRead = %d\n", readerPointer->flags.isRead);             /* print isRead — 1 means finished reading all characters, 0 means still reading */
}
#else
#define bGetFlags(readerPointer) ((readerPointer)?(readerPointer->flags):(RT_FAIL_1))
#endif

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
/* readerPrintStat — prints how many times each character appeared in the buffer
 * Structs used: Buffer (histogram) */
jd_void readerPrintStat(BufferPointer const readerPointer) {
	if (!readerPointer)                                             /* if the buffer does not exist there are no stats to print */
		return;
	jd_intg i;                                                      /* index used to loop through all 128 possible ASCII characters */
	for (i = 0; i < NCHAR; i++) {                                  /* loop from 0 to 127 checking every ASCII character */
		if (readerPointer->histogram[i] > 0)                        /* only print this character if it appeared at least once in the buffer */
			printf("B[%c]=%d, ", (jd_char)i, readerPointer->histogram[i]); /* print the character and how many times it appeared */
	}
	printf("\n");                                                   /* print a new line after all the stats */
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
/* readerNumErrors — returns how many errors happened while loading the buffer
 * Structs used: Buffer (numReaderErrors) */
jd_intg readerNumErrors(BufferPointer const readerPointer) {
	if (!readerPointer)                             /* if the buffer does not exist we cannot return an error count */
		return READER_ERROR;                        /* return -1 to signal an error */
	return readerPointer->numReaderErrors;          /* return the total number of errors that happened while loading the buffer */
}

/*
***********************************************************
* Function name: readerChecksum
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

/* readerChecksum — adds up the ASCII value of every character to verify nothing got corrupted
 * Structs used: Buffer (content, checkSum), Position (wrte) */
jd_intg readerChecksum(BufferPointer readerPointer) {
	if (!readerPointer)                                             /* if the buffer does not exist there is nothing to calculate */
		return 0;                                                   /* return 0 as a safe default */
	jd_intg i;                                                      /* index used to loop through every character in the buffer */
	jd_intg checksum = 0;                                           /* this variable holds the running total and starts at 0 */
	for (i = 0; i < readerPointer->position.wrte; i++)             /* loop through every character from position 0 to the last written position */
		checksum += (unsigned char)readerPointer->content[i];       /* add the ASCII value of the current character to the running total */
	readerPointer->checkSum = checksum;                             /* save the final total into the struct so it can be accessed later */
	return checksum;                                                /* return the final total to the caller */
}
