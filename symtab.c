/****************************************************/
/* File: symtab.c                                   */
/* Symbol table implementation for the TINY compiler*/
/* (allows only one symbol table)                   */
/* Symbol table is implemented as a chained         */
/* hash table                                       */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#include "globals.h"

/* SIZE is the size of the hash table */
#define SIZE 211

/* SHIFT is the power of two used as multiplier
   in hash function  */
#define SHIFT 4

/* the hash function */
static int hash ( char * key )
{ int temp = 0;
  int i = 0;
  while (key[i] != '\0')
  { temp = ((temp << SHIFT) + key[i]) % SIZE;
    ++i;
  }
  return temp;
}

/* the list of line numbers of the source 
 * code in which a variable is referenced
 */
typedef struct LineListRec
   { int lineno;
     struct LineListRec * next;
   } * LineList;

/* The record in the bucket lists for
 * each variable, including name, 
 * assigned memory location, and
 * the list of line numbers in which
 * it appears in the source code
 */


//typedef enum {Fun,Var} IDType;
//typedef enum {Integer, Void} ExpType;

typedef struct BucketListRec
   { char * name;
     LineList lines;
     char * scope;
     IDType idType;
     ExpType valType;
     struct BucketListRec * next;
   } * BucketList;

/* the hash table */
static BucketList hashTable[SIZE];

/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */
void st_insert( char * name, int lineno, char *scope,IDType idType, ExpType valType)
{ int h = hash(name);

  BucketList l =  hashTable[h];
  BucketList lGlob = NULL;
  while (l != NULL){
    if(strcmp(name,l->name) == 0){
      if(strcmp(l->scope,"Global")==0)
        lGlob=l;
      if(strcmp(scope,l->scope)==0)
        break;
    }
    l = l->next;
  }

  if(l==NULL)
    l=lGlob;
  if (l == NULL) /* variable not yet in table */
  { 
    l = (BucketList) malloc(sizeof(struct BucketListRec));
    l->name = name;
    l->lines = (LineList) malloc(sizeof(struct LineListRec));
    l->lines->lineno = lineno;
    l->scope = scope;
    l->valType=valType;
    l->idType=idType;
    l->lines->next = NULL;
    l->next = hashTable[h];
    hashTable[h] = l; }
  else /* found in table, so just add line number */
  { 
    
    LineList t = l->lines;
    while (t->next != NULL) t = t->next;
    t->next = (LineList) malloc(sizeof(struct LineListRec));
    t->next->lineno = lineno;
    t->next->next = NULL;
  }
} /* st_insert */

int validate_decl( char * name, int lineno, char *scope,IDType idType, ExpType valType)
{ int h = hash(name);
  BucketList l =  hashTable[h];
  BucketList lGlob = NULL;
  while (l != NULL){
    if(strcmp(name,l->name) == 0){
      if(strcmp(l->scope,"Global")==0)
        lGlob=l;
      if(strcmp(scope,l->scope)==0)
        break;
    }
    l = l->next;
  }

  if(l==NULL)
    l=lGlob;
  if (l == NULL) /* variable not yet in table */{
    if(idType)
      printf("Erro Semantico: uso de variavel nao declarada : %s. Linha: %d\n",name,lineno);
    else
      printf("Erro Semantico: chamada de funcao nao declarada: %s. Linha: %d\n",name,lineno);
    return -1;
  }
  else /* found in table, so just add line number */
  { //Check scope and type
    // if((strcmp("Global",l->scope)!=0)&&(strcmp(scope,l->scope)!=0)){
    //   if(idType)
    //     printf("Erro Semantico: uso da variavel %s nao declarada no escopo %s, Linha: %d\n",name,scope,lineno);
    //   else
    //     printf("Erro Semantico: chamada da funcao %s nao declarada no escopo %s, Linha: %d\n",name,scope,lineno);
    //   return -1;
    // }
    if(idType!=l->idType){
      if(idType)
        printf("Erro Semantico: funcao %s eh usada como variavel, Linha: %d\n",name,lineno);
      else
        printf("Erro Semantico: variavel %s eh usada como funcao, Linha: %d\n",name,lineno);
      return -1;
    }
  }
  return 1;
}

/* Function st_lookup returns the memory 
 * location of a variable or -1 if not found
 */
int check_not_exist ( char * name ,char * scope, IDType idType)
{ int h = hash(name);
  BucketList l =  hashTable[h];
  BucketList lGlob = NULL;
  while (l != NULL){
    if(strcmp(name,l->name) == 0){
      if(strcmp(l->scope,"Global")==0)
        lGlob=l;
      if(strcmp(scope,l->scope)==0)
        break;
    }
    l = l->next;
  }

  if(l==NULL)
    l=lGlob;
  if (l == NULL) return 1;
  if (l->idType==idType){
    if(l->idType){
      if(strcmp(scope,l->scope)==0)
        printf("Erro Semantico: variavel %s ja havia sido declarada neste escopo (%s). Erro na Linha: %d\n",name,scope,lineno);
    }
    else
       printf("Erro Semantico: funcao %s ja havia sido declarada. Erro na Linha: %d\n",name,lineno);
  }
  else{
    if(idType)
      printf("Erro Semantico: declaracao invalida. %s ja havia sido declarada como funcao. Erro na Linha: %d\n",name,lineno);
    else if(strcmp(l->scope,"Global")==0)
     printf("Erro Semantico: declaracao invalida. %s ja havia sido declarada como variavel. Erro na Linha: %d\n",name,lineno);
  } 
  
  //Checar escopo
  return -1;
}

int search_main ()
{ int h = hash("main");
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp("main",l->name) != 0))
    l = l->next;
  if (l == NULL){
    printf("\nErro Semantico: funcao main nao declarada.\n");
    return -1;
  }
  else return 1;
}

/* Procedure printSymTab prints a formatted 
 * listing of the symbol table contents 
 * to the listing file
 */
void printSymTab(FILE * listing)
{ int i;
  fprintf(listing,"Variable Name      Scope       ID Type  Val Type  Line Numbers\n");
  fprintf(listing,"-------------  --------------  -------  --------  ------------\n");
  for (i=0;i<SIZE;++i)
  { if (hashTable[i] != NULL)
    { BucketList l = hashTable[i];
      while (l != NULL)
      { LineList t = l->lines;
        fprintf(listing,"%-14s  ",l->name);
        fprintf(listing,"%-14s   ",l->scope);
        if(l->idType)
          fprintf(listing," %-7s ","var");
        else
          fprintf(listing," %-7s ","func");
        if(l->valType)
          fprintf(listing," %-6s","int");
        else
          fprintf(listing," %-6s","void");
        while (t != NULL)
        { fprintf(listing,"%4d ",t->lineno);
          t = t->next;
        }
        fprintf(listing,"\n");
        l = l->next;
      }
    }
  }
  fprintf(listing, "\n" );
} /* printSymTab */
