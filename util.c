/****************************************************/
/* File: util.c                                     */
/* Utility function implementation                  */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "util.h"

/* Procedure printToken prints a token 
 * and its lexeme to the listing file
 */
void printToken( TokenType token, const char* tokenString ){
 switch (token)
  { 
   case IF: fprintf(listing,"if"); break;
    case ELSE: fprintf(listing,"else"); break;
    case ATT: fprintf(listing,"=\n"); break;
    case MAIOR: fprintf(listing,">\n"); break;
    case ABCH: fprintf(listing,"{\n"); break;
    case FECCH: fprintf(listing,"}\n"); break;
    case ABCO: fprintf(listing,"[\n"); break;
    case FCCO: fprintf(listing,"]\n"); break;
    case VIRG: fprintf(listing,",\n"); break;
    case DIF: fprintf(listing,"!=\n"); break;
    case MAIIG: fprintf(listing,">=\n"); break;
    case MENOR: fprintf(listing,"<=\n"); break;
    case MENIG: fprintf(listing,")\n"); break;
    case IGLIGL: fprintf(listing,"==\n"); break;
    case ABP: fprintf(listing,"(\n"); break;
    case FCP: fprintf(listing,")\n"); break;
    case PEV: fprintf(listing,";\n"); break;
    case SOM: fprintf(listing,"+\n"); break;
    case SUB: fprintf(listing,"-\n"); break;
    case MUL: fprintf(listing,"*\n"); break;
    case DIV: fprintf(listing,"/\n"); break;
    case INT: fprintf(listing,"int\n"); break;
    case RETURN: fprintf(listing,"return\n"); break;
    case VOID: fprintf(listing,"void\n"); break;
    case WHILE: fprintf(listing,"while\n"); break;
    case 0: fprintf(listing,"EOF\n"); break;
    case NUM:
      fprintf(listing,
          "NUM = %s\n",tokenString);
      break;
    case ID:
      fprintf(listing,
          "ID = %s\n",tokenString);
      break;
    case ERROR:
      fprintf(listing,
          "ERROR: %s\n",tokenString);
      break;
    default: /* should never happen */
      fprintf(listing,"Unknown token: %d\n",token);
  }
}

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode * newDeclNode(DeclKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = DeclK;
    t->kind.decl = kind;
    t->lineno = lineno;
  }
  return t;
}
TreeNode * newStmtNode(StmtKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = StmtK;
    t->kind.stmt = kind;
    t->lineno = lineno;
  }
  return t;
}

/* Function newExpNode creates a new expression 
 * node for syntax tree construction
 */
TreeNode * newExpNode(ExpKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = ExpK;
    t->kind.exp = kind;
    t->lineno = lineno;
    t->type = Void;
  }
  return t;
}

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char * copyString(char * s)
{ int n;
  char * t;
  if (s==NULL) return NULL;
  n = strlen(s)+1;
  t = malloc(n);
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else strcpy(t,s);
  return t;
}

/* Variable indentno is used by printTree to
 * store current number of spaces to indent
 */
static indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno+=2
#define UNINDENT indentno-=2

/* printSpaces indents by printing spaces */
static void printSpaces(void)
{ int i;
  for (i=0;i<indentno;i++)
    fprintf(listing," ");
}

/* procedure printTree prints a syntax tree to the 
 * listing file using indentation to indicate subtrees
 */
void printTree( TreeNode * tree )
{ int i;
  INDENT;
  while (tree != NULL) {
    printSpaces();
    if (tree->nodekind==StmtK)
    { switch (tree->kind.stmt) {
        case IfK:
          fprintf(listing,"If\n");
          break;
        case IterK:
          fprintf(listing,"While\n");
          break;
        case AssignK:
          fprintf(listing,"Assign\n");
          break;
        case IfElseK:
          fprintf(listing,"If else\n");
          break;
        case RetK:
          fprintf(listing, "Return\n" );
          break;
        case AtivK:
          fprintf(listing, "Chamada Funcao: %s\n",tree->attr.name);
          break;
        case CompostK:
          fprintf(listing, "Sequencia de Statements\n" );
          break;
        default:
          fprintf(listing,"Unknown StmtNode kind\n");
          break;
      }
    }
    else if (tree->nodekind==ExpK)
    { switch (tree->kind.exp) {
        case OpK:
          fprintf(listing,"Op: ");
          printToken(tree->attr.op,"\0");
          break;
        case ConstK:
          fprintf(listing,"Const: %s\n",tree->attr.name);
          break;
        case IdK:
          fprintf(listing,"Id: %s\n",tree->attr.name);
          break;
        case ArrayK:
          fprintf(listing, "Array: %s\n", tree->attr.name);
          break;
        default:
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
    }
    else if (tree->nodekind==DeclK)
    { switch (tree->kind.decl) {
        case FunK:
          if(tree->type)
            fprintf(listing,"Function Declaration: %s, type: Integer\n ",tree->attr.name);
          else
            fprintf(listing,"Function Declaration: %s, type: Void\n ",tree->attr.name);
          break;
        case VarK:
          fprintf(listing,"Variable Declaration: %s\n",tree->attr.name);
          break;
        case VarArrayK:
          fprintf(listing,"Var Array Declaration: %s\n",tree->attr.name);
          break;
          case ParamK:
          fprintf(listing,"Parameter Declaration: %s\n",tree->attr.name);
          break;
        case ParamArrayK:
          fprintf(listing,"Parameter Array Declaration: %s\n",tree->attr.name);
          break;
        default:
          fprintf(listing,"Unknown DeclNode kind\n");
          break;
      }
    }
    else fprintf(listing,"Unknown node kind\n");
    for (i=0;i<MAXCHILDREN;i++)
         printTree(tree->child[i]);
    tree = tree->sibling;
  }
  UNINDENT;
}
