AI_pathFind
===========

AI path finder using a recursive function

Per ogni nodo:
  - controlla che sia l'uscita

  - calcola i pesi del nodo

  - per ogni diramazione valida
    a partire dal nodo, la percorre

  - controlla che il nodo attuale non
    faccia parte della strada esatta

  - stampa la posizione del nodo nel caso
    il passo precedente sia verificato

FINE.

L'algoritmo cerca solo nei nodi (SOPRA,SOTTO,DESTRA,SINISTRA)
evitando di cercare nei nodi vicini diagonalmente al nodo attuale,
si potrebbe implementare ma l'algoritmo resterebbe lo stesso, solo
piu' lungo, quindi e' stato evitato.
