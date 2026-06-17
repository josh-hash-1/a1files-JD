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
# ECHO "[COMPILER SCRIPT .......................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: Compilers.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A1-A5.
* Date: Sep 01 2025
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

/* Language name — we changed this from "Sofia" to "JD" (Josh & Daryl).
 * This string is also used as the default key for the Vigenere cipher. */
#define STR_LANGNAME	"JD"

/* Logical constants — renamed prefix from sofia_ to jd_ to match our language name */
#define JD_TRUE  1
#define JD_FALSE 0

#define INVALID NULL
#define EOF_CHAR '\0'

/*
------------------------------------------------------------
Data types definitions
NOTE: Some types may not be directly used by your language,
		but they can be necessary for conversions.
------------------------------------------------------------
*/

/* Custom type aliases — we renamed all of these from sofia_* to jd_*
 * to match our language name "JD". The underlying C types are unchanged.
 *   jd_char = single character
 *   jd_strg = string (already a pointer — char* — so no need to write jd_strg*)
 *   jd_intg = integer
 *   jd_real = float (decimal number)
 *   jd_void = void (functions that return nothing)
 *   jd_boln = boolean (0 or 1), stored as 1 byte
 *   jd_byte = raw byte (0-255)
 *   jd_long = long integer
 *   jd_doub = double (high-precision decimal) */
typedef char			jd_char;
typedef char*			jd_strg;
typedef int				jd_intg;
typedef float			jd_real;
typedef void			jd_void;

typedef unsigned char	jd_boln;
typedef unsigned char	jd_byte;

typedef long			jd_long;
typedef double			jd_doub;

/*
------------------------------------------------------------
Programs:
1: Coder - invokes Main1Coder code
2: Reader - invokes Main2Reader code
3: Scanner - invokes Main3Scanner code
4: Parser - invokes Main4Parser code
5: Writer - invokes Main5Writer code
------------------------------------------------------------
*/
enum PROGRAMS {
	PGM_CDR = '1', // Coder:	05pt
	PGM_RDR = '2', // Reader:	05pt
	PGM_SCN = '3', // Scanner:	15pt
	PGM_PSR = '4', // Parser:	15pt
	PGM_WRT = '5', // Writer:	10pt
};

/*
------------------------------------------------------------
Main functions signatures
(Code will be updated during next assignments)
------------------------------------------------------------
*/
jd_intg main1Coder	(jd_intg argc, jd_strg* argv); // update the return value name with are name of are language
jd_intg main2Reader	(jd_intg argc, jd_strg* argv); 
jd_void printLogo(); // update the return value name with are name of are language
jd_void errorPrint(jd_strg fmt, ...); // update the return value name with are name of are language

#endif

/*
	  "args": [
		1,
		1,
		"README.txt",
		"CODED.txt"
	  ]
	  "args": [
		4,
		"CODED.txt"
	  ]
*/
