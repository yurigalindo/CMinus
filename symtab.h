/****************************************************/
/* File: symtab.h                                   */
/* Symbol table interface for the TINY compiler     */
/* (allows only one symbol table)                   */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "globals.h"
/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */

typedef enum {Fun,Var} IDType;
void st_insert( char * name, int lineno, char * scope,IDType idType, ExpType valType);

int validate_decl( char * name, int lineno, char * scope,IDType idType, ExpType valType); //Confirma que foi declarada e valida os tipos




/* Function st_lookup returns the memory 
 * location of a variable or -1 if not found
 */
int check_not_exist ( char * name ,char *scope,IDType idType);

int search_main();
/* Procedure printSymTab prints a formatted 
 * listing of the symbol table contents 
 * to the listing file
 */
void printSymTab(FILE * listing);


#endif
