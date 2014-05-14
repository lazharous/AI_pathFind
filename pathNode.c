// **************************************
// *
// * Autore: Mainieri Paolo
// * Ultimo aggiornamento: 19/04/2010
// *
// * Il seguente codice sorgente può essere
// * ridistribuito a patto di lasciare intatto
// * il qui presente commento e non nascondere
// * mai la paternità di questo codice sorgente
// *
// **************************************

#include <stdio.h>
#include <stdlib.h>

// riga e colonna della matrice
#define ROW 12
#define COL 12

// possibili posizioni
#define LEFT 0
#define UPPER 1
#define RIGHT 2
#define BOTTOM 3

// condizione per un nodo
// di essere cancellato
#define VALID 0
#define ERASED 1
#define FOUND  7 /* 7 in onore di Lena <3 */

// struttura che contine il percorso
typedef struct
{
  int i, j;
} _path;

// struttura che contiene i valori di un nodo
typedef struct
{
  // numero di consensi alla cancellazione
  // se tutti sono positivi viene cancellato
  // default: non valido
  int p[ 4 ];
} _node;

// prototipi delle funzioni
int load_m(char (*m)[COL]);
int start_found(char (*m)[COL], char s, _path *p);
void print_m(char (*m)[COL]);


int load_m(char (*m)[COL])
{
  int i, j;
  char file_name[256];
  
  printf("File da caricare: ");
  scanf("%s", &file_name);

  FILE *fp = fopen(file_name, "rt");

  if (!fp)
  {
    printf("\nErrore: Apertura file.\n");
    return 0;
  }

  for(i=0; i<ROW; i++)
  {
    for(j=0; j<COL-1; j++)
      fscanf(fp, "%c", &m[i][j]);
      
    // ultimo elemento letto insieme al
    // carattere di ritorno
    fscanf(fp, "%c\n", &m[i][j]);
  }

  fclose(fp);
  
  return 1;
}


int start_found(char (*m)[COL], char s, _path *p)
{  
  for (p->i=0; p->i<ROW; (p->i)++)
    for (p->j=0; p->j<COL; (p->j)++)
      if (m[p->i][p->j] == s)
      {
        // partenza trovata, usciamo
        printf("\nPartenza trovata a [%d][%d]\n", p->i, p->j);
        return 1;
      }
  
  // non trovata, errore
  return 0;
}

// funzione principale del programma, trova la via per uscire dal
// labirinto, fondamentalmente crea una matrice 3x3 intorno alla posizione
// attuale ed elimina la cella attuale, poi analizza
// tutte le altre dando dei pesi in base al numero di uscite che ogni nodo ha
// se il peso e' 0 non ci sono strade, il nodo viene cancellato, un nodo con piu'
// diramazioni prima di essere cancellato deve avere il consenso dei suoi nodi
// vicini, uno di essi in fatti potrebbe portare all'uscita
int path_find(char (*m)[COL], _path step, char end, char walkable, char sign, int from)
{
  // nodo attuale
  _node n;
  
  // FASE 0: controllo dell'uscita
  if (m[step.i][step.j] == end)
  {
    printf("Uscita trovata a [%d][%d] "
           "facciamo i passi a ritroso ora\n", step.i, step.j);
    return FOUND;
  }
  
  // FASE 1: calcolo del peso del nodo attuale e dei consensi dello stesso
  // basati sulla provenienza del nodo (controlla che sia percorribile o sia il traguardo)
  if ((step.i-1>=0) && (from != UPPER) &&
     (m[step.i-1][step.j]==walkable || m[step.i-1][step.j]==end)) { n.p[ UPPER ]  = VALID; }
  else
    n.p[ UPPER ] = ERASED;
  
  if ((step.j-1>=0) && (from != LEFT) &&
     (m[step.i][step.j-1]==walkable || m[step.i][step.j-1]==end)) { n.p[ LEFT ]   = VALID; }
  else
    n.p[ LEFT ] = ERASED;
      
  if ((step.i+1<=ROW-1) && (from != BOTTOM) &&
     (m[step.i+1][step.j]==walkable || m[step.i+1][step.j]==end)) { n.p[ BOTTOM ] = VALID; }
  else
    n.p[ BOTTOM ] = ERASED;
    
  if ((step.j+1<=COL-1) && (from != RIGHT) &&
     (m[step.i][step.j+1]==walkable || m[step.i][step.j+1]==end)) { n.p[ RIGHT ]  = VALID; }
  else
    n.p[ RIGHT ] = ERASED;
  
  // FASE 2: avanzamento del percorso e controllo dell'uscita
  if ( n.p[ LEFT ] == VALID )
    {
      _path p;
      p.i = step.i;
      p.j = step.j-1;
      // segue il cammino e aggiorna i consensi
      // alla cancellazione di questo nodo
      n.p[ LEFT ] = path_find( m, p, end, walkable, sign, RIGHT );
      // controlla se si tratta di un nodo facente parte della
      // retta via :P
      if (n.p[ LEFT ] == FOUND)
      {
        printf("Passo: [%d][%d]\n", step.i, step.j);
        m[step.i][step.j] = sign;
        return FOUND;
      }
    }

  if ( n.p[ RIGHT ] == VALID )
    {
      _path p;
      p.i = step.i;
      p.j = step.j+1;
      // segue il cammino e aggiorna i consensi
      // alla cancellazione di questo nodo
      n.p[ RIGHT ] = path_find( m, p, end, walkable, sign, LEFT );
      // controlla se si tratta di un nodo facente parte della
      // retta via :P
      if (n.p[ RIGHT ] == FOUND)
      {
        printf("Passo: [%d][%d]\n", step.i, step.j);
        m[step.i][step.j] = sign;
        return FOUND;
      }
    }

  if ( n.p[ UPPER ] == VALID )
    {
      _path p;
      p.i = step.i-1;
      p.j = step.j;
      // segue il cammino e aggiorna i consensi
      // alla cancellazione di questo nodo
      n.p[ UPPER ] = path_find( m, p, end, walkable, sign, BOTTOM );
      // controlla se si tratta di un nodo facente parte della
      // retta via :P
      if (n.p[ UPPER ] == FOUND)
      {
        printf("Passo: [%d][%d]\n", step.i, step.j);
        m[step.i][step.j] = sign;
        return FOUND;
      }
    }
    
  if ( n.p[ BOTTOM ] == VALID )
    {
      _path p;
      p.i = step.i+1;
      p.j = step.j;
      // segue il cammino e aggiorna i consensi
      // alla cancellazione di questo nodo
      n.p[ BOTTOM ] = path_find( m, p, end, walkable, sign, UPPER );
      // controlla se si tratta di un nodo facente parte della
      // retta via :P
      if (n.p[ BOTTOM ] == FOUND)
      {
        printf("Passo: [%d][%d]\n", step.i, step.j);
        m[step.i][step.j] = sign;
        return FOUND;
      }
    }
}


void print_m(char (*m)[COL])
{
  int i, j;
  
  for(i=0; i<ROW; i++)
  {
    for(j=0; j<COL; j++)
      printf("%c", m[i][j]);
      
    printf("\n");
  }
}


int main(void)
{
  // matrice contenente il labirinto
  char m[ROW][COL];
  // conterra' il percorso svolto
  _path start;

  while (!load_m( m ));
  
  // stampa la matrice
  print_m( m );
  
  // cerca la posizione iniziale
  if (!start_found( m, '@', &start ))
  {
    printf("Errore: Partenza non trovata\n");
    return -1;
  }
  
  int f = path_find( m, start, 'E', '.', 'x', -1 );
  
  printf("\n");
  print_m( m );
  
  system("pause");
  
  return 0;
}
