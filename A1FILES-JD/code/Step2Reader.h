/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2025
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (F25)  |"
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
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12.
* Date: Jan 01 2025
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
#include <stdlib.h>
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Constants about controls (not need to change) */
#define READER_ERROR		(-1)		/* General error message */
#define READER_TERMINATOR	'\0'		/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME .................................. */

/* You should add your own constant definitions here */
#define READER_MAX_SIZE		INT_MAX-1	/* maximum capacity */ 

#define READER_DEFAULT_SIZE		100			/* default initial buffer reader capacity */
#define READER_DEFAULT_FACTOR	0.5f		/* default factor */

/* TO_DO: Add your bit-masks constant definitions here */
/*
// BITS                             (    76543210)
#define READER_DEFAULT_FLAG 0x00 	// (0b00000000) = (0x00)_16 = (000)_10
// TO_DO: BIT 3: END = End of buffer flag
#define READER_SET_FLAG_END 0x08	// (0b00001000) = (0x08)_16 = (008)_10
// TO_DO: BIT 2: REL = Rellocation memory flag
#define READER_SET_FLAG_REL 0x04	// (0b00000100) = (0x04)_16 = (004)_10
// TO_DO: BIT 1: EMP = Buffer empty flag
#define READER_SET_FLAG_FUL 0x02	// (0b00000010) = (0x02)_16 = (002)_10
// TO_DO: BIT 0: FUL = Buffer full flag
#define READER_SET_FLAG_EMP 0x01	// (0b00000001) = (0x01)_16 = (001)_10
*/

#define NCHAR				128			/* Chars from 0 to 127 */

#define CHARSEOF			(-1)		/* EOF Code for Reader */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME .................................. */

/* TODO: Adjust datatypes */

/* Offset declaration */
typedef struct position {
	jd_intg wrte;					/* the offset to the add chars (in chars) */
	jd_intg read;					/* the offset to the get a char position (in chars) */
	jd_intg mark;					/* the offset to the mark position (in chars) */
} Position;

/* Flags declaration */
typedef struct flag {
	jd_boln isEmpty;					/* checks if there is no content */
	jd_boln isFull;					/* the content is using all size */
	jd_boln isRead;					/* all content was read */
	jd_boln isMoved;					/* the content was moved in reallocation */
} Flag;

/* Buffer structure */
typedef struct bufferReader {
	jd_strg		content;			/* pointer to the beginning of character array (character buffer) */
	jd_intg		size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	jd_real		factor;				/* factor for increase the buffer */
	Flag			flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Position		position;			/* Offset / position field */
	jd_intg		histogram[NCHAR];	/* Statistics of chars */
	jd_intg		numReaderErrors;	/* Number of errors from Reader */
	jd_intg		checkSum;			/* Sum of bytes(chars) */
} Buffer, * BufferPointer;

/* FUNCTIONS DECLARATION:  .................................. */

/* General Operations */
BufferPointer	readerCreate(jd_intg, jd_real);
BufferPointer	readerAddChar(BufferPointer const, jd_char);
jd_boln		readerClear(BufferPointer const);
jd_boln		readerFree(BufferPointer const);
jd_boln		readerIsFull(BufferPointer const);
jd_boln		readerIsEmpty(BufferPointer const);
jd_boln		readerSetMark(BufferPointer const, jd_intg);
jd_intg		readerPrint(BufferPointer const);
jd_intg		readerLoad(BufferPointer const, jd_strg);
jd_boln		readerRecover(BufferPointer const);
jd_boln		readerRetract(BufferPointer const);
jd_boln		readerRestore(BufferPointer const);
jd_intg		readerChecksum(BufferPointer const);
/* Getters */
jd_char		readerGetChar(BufferPointer const);
jd_strg		readerGetContent(BufferPointer const, jd_intg);
jd_intg		readerGetPosRead(BufferPointer const);
jd_intg		readerGetPosWrte(BufferPointer const);
jd_intg		readerGetPosMark(BufferPointer const);
jd_intg		readerGetSize(BufferPointer const);
jd_void		readerPrintFlags(BufferPointer const);
jd_void		readerPrintStat(BufferPointer const);
jd_intg		readerNumErrors(BufferPointer const);

#endif
