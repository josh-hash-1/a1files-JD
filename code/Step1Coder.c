/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2025
* Author: Josh & Daryl
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (F25)  |"
# ECHO "=---------------------------------------="
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    "
# ECHO "    @@                             @@    "
# ECHO "    @@           %&@@@@@@@@@@@     @@    "
# ECHO "    @@       @%% (@@@@@@@@@  @     @@    "
# ECHO "    @@      @& @   @ @       @     @@    "
# ECHO "    @@     @ @ %  / /   @@@@@@     @@    "
# ECHO "    @@      & @ @  @@              @@    "
# ECHO "    @@       @/ @*@ @ @   @        @@    "
# ECHO "    @@           @@@@  @@ @ @      @@    "
# ECHO "    @@            /@@    @@@ @     @@    "
# ECHO "    @@     @      / /     @@ @     @@    "
# ECHO "    @@     @ @@   /@/   @@@ @      @@    "
# ECHO "    @@     @@@@@@@@@@@@@@@         @@    "
# ECHO "    @@                             @@    "
# ECHO "    @@         S O F I A           @@    "
# ECHO "    @@                             @@    "
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    "
# ECHO "                                         "
# ECHO "[CODER SCRIPT ..........................]"
# ECHO "                                         "
*/

/*
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: Sep 01 2025
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif

/*
************************************************************
* Function name: vigenereFile
* Purpose: Reads an input file, applies the Vigenere cipher
*          (encode or decode), and writes the result to an
*          output file.
*
* HOW THE CIPHER WORKS:
*   Each visible character (ASCII 32-126) gets shifted by an
*   amount determined by the current key letter:
*     shift = key[keyIndex % keyLength] - ASCII_START
*
*   Encode: ((char - ASCII_START) + shift) % ASCII_RANGE + ASCII_START
*   Decode: ((char - ASCII_START) - shift + ASCII_RANGE) % ASCII_RANGE + ASCII_START
*
*   Non-visible characters (newline, tab) pass through unchanged.
*   keyIndex only advances for visible characters.
*
* Parameters:
*   inputFileName  - file to read from
*   outputFileName - file to write result into
*   key            - cipher keyword (e.g. "JD")
*   encode         - CYPHER(1) to encode, DECYPHER(0) to decode
************************************************************
*/
void vigenereFile(const jd_strg inputFileName, const jd_strg outputFileName, const jd_strg key, jd_intg encode) {
	FILE* inputFIle;
	FILE* outPutFIle;

	/* Defensive programming: check all inputs before opening files */
	if (key == NULL) {
		printf("Key Not Found.");
		return;
	}

	inputFIle = fopen(inputFileName, "r");
	if (inputFIle == NULL) {
		printf("The file is not opened.");
		return;
	}

	outPutFIle = fopen(outputFileName, "w");
	if (outPutFIle == NULL) {
		printf("The file is not opened.");
		return;
	}

	jd_intg character;          /* must be int (not char) — fgetc returns EOF (-1) at end of file */
	jd_intg keylenght;          /* pre-computed key length so we don't call strlen inside the loop */
	keylenght = strlen(key);
	jd_intg keyIndex;           /* tracks our position in the key — cycles via % keylenght */
	keyIndex = 0;
	jd_intg shiftOne;           /* shift amount for the current character based on the key letter */
	shiftOne = 0;

	/* Read one character at a time until EOF */
	while ((character = fgetc(inputFIle)) != EOF ) {

		/* Only apply cipher to visible ASCII characters (space=32 to ~=126).
		 * Characters below 32 (newline, tab) pass through unchanged. */
		if (character >= ASCII_START && character <= ASCII_END) {

			/* key[keyIndex % keylenght] cycles through the key letters.
			 * Subtracting ASCII_START normalizes the key letter to a 0-based shift. */
			shiftOne = (key[keyIndex % keylenght] - ASCII_START );

			if (encode == CYPHER)
				character = ((character - ASCII_START) + shiftOne) % ASCII_RANGE + ASCII_START; /* Encode */
			else
				character = ((character - ASCII_START) - shiftOne + ASCII_RANGE) % ASCII_RANGE + ASCII_START; /* Decode — +ASCII_RANGE prevents negative numbers */
			keyIndex++;
		}
		fputc(character, outPutFIle);
	}

	fclose(inputFIle);
	fclose(outPutFIle);
}

/*
************************************************************
* Function name: vigenereMem
* Purpose: Same cipher logic as vigenereFile(), but instead
*          of writing to a file, it returns the result as a
*          string in memory (malloc'd).
*
*          Used by Main1Coder.c to VERIFY the cipher worked —
*          after encoding to CODED.txt, it decodes CODED.txt
*          in memory and prints it so you can confirm the
*          round-trip gives back the original text.
*
* Parameters:
*   inputFileName - file to read from
*   key           - cipher keyword (e.g. "JD")
*   encode        - CYPHER(1) to encode, DECYPHER(0) to decode
*
* Return value:
*   A malloc'd string with the cipher output.
*   Returns NULL if inputs are invalid or memory fails.
*   Caller should free() the result when done.
************************************************************
*/
jd_strg vigenereMem(const jd_strg inputFileName, const jd_strg key, jd_intg encode) {

	jd_intg character;
	jd_intg keylenght = strlen(key);
	jd_intg keyIndex  = 0;
	jd_intg shiftOne  = 0;
	jd_intg outIndex  = 0;  /* position we are writing to in the output buffer */

	jd_strg output = NULL;

	/* Defensive programming: check inputs before doing anything */
	if (inputFileName == NULL || key == NULL)
		return NULL;

	/* Get the file size so we know how much memory to allocate.
	 * We need size+1 bytes: one per character, plus one for the null terminator '\0'. */
	jd_intg size = getSizeOfFile(inputFileName);
	if (size == 0)
		return NULL;

	/* malloc allocates a block of memory on the heap to hold the output string */
	output = (jd_strg)malloc(size + 1);
	if (output == NULL)
		return NULL;

	FILE* inputFile = fopen(inputFileName, "r");
	if (inputFile == NULL)
		return NULL;

	/* Same cipher loop as vigenereFile — see comments there for explanation */
	while ((character = fgetc(inputFile)) != EOF) {

		if (character >= ASCII_START && character <= ASCII_END) {
			shiftOne = (key[keyIndex % keylenght] - ASCII_START);

			if (encode == CYPHER)
				character = ((character - ASCII_START) + shiftOne) % ASCII_RANGE + ASCII_START;
			else
				character = ((character - ASCII_START) - shiftOne + ASCII_RANGE) % ASCII_RANGE +
				ASCII_START;
			keyIndex++;
		}

		/* Write to the output buffer in memory instead of a file */
		output[outIndex] = (jd_char)character;
		outIndex++;
	}

	output[outIndex] = '\0';  /* null-terminate so the string works with printf, etc. */
	fclose(inputFile);

	return output;
}

// Function to encode (cypher)
void cypher(const jd_strg inputFileName, const jd_strg outputFileName, const jd_strg key) {
    vigenereFile(inputFileName, outputFileName, key, CYPHER);
}

// Function to decode (decypher)
void decypher(const jd_strg inputFileName, const jd_strg outputFileName, const jd_strg key) {
    vigenereFile(inputFileName, outputFileName, key, DECYPHER);
}

/*
************************************************************
* Function name: getSizeOfFile
* Purpose: Returns the size (in bytes) of a file.
*          Used by vigenereMem() to know how much memory
*          to allocate before reading the file.
*
* HOW IT WORKS:
*   fseek(fptr, 0, SEEK_END) moves the cursor to the end.
*   ftell(fptr) then returns the byte position = file size.
*
* Parameters:
*   filename - path to the file
* Return value:
*   File size in bytes, or 0 if file can't be opened.
************************************************************
*/
jd_intg getSizeOfFile(const jd_strg filename) {
	jd_intg size = 0;   /* default — returned if file can't be opened */
	FILE* fptr;
	fptr = fopen(filename, "r");

	if (fptr == NULL) {
		printf("The file is not opened.");
		return 0;
	}
	else {
			fseek(fptr, 0, SEEK_END);  /* move cursor to end of file */
			size = ftell(fptr);        /* cursor position = file size in bytes */
	}
		fclose(fptr);
	return size;
}
