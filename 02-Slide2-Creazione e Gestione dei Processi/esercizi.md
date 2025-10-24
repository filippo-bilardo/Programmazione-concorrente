### Esercizio a

Il processo padre inizializza tre variabili intere `cateto1` e `cateto2` di un triangolo rettangolo e assegna loro dei valori. Viene creato il processo figlio, il processo padre calcola e stampa l'area del triangolo mentre il processo figlio deve stampare il valore del perimetro.

---

### Esercizio b

Il processo padre sta in ascolto di un comando da tastiera (`scanf`). In base al comando ricevuto, crea ed avvia un processo figlio. Prima di avviare il processo figlio, la variabile `led` viene impostata con il valore 1, 2 o 3 (1=lampeggio led verde, 2 rosso e 3 giallo). Il processo figlio, leggendo il valore della variabile `led`, farà lampeggiare il led corrispondente.

- Comando `'v'` → 10 lampeggi del led verde ad intervalli di 0.3 sec.
- Comando `'r'` → 10 lampeggi del led rosso ad intervalli di 0.7 sec.
- Comando `'g'` → 10 lampeggi del led giallo ad intervalli di 1.2 sec.
- Comando `'q'` → chiusura del programma.

Oltre a comandare il pin fisico, visualizzare un messaggio a schermo (es. "led verde acceso", "led verde spento", ...).

---

### Esercizio c

Il processo padre crea un processo figlio che esegue un conteggio da 1 a 10, stampando il numero corrente e il PID del processo figlio. Il processo padre, nel frattempo, attende la terminazione del processo figlio e poi stampa un messaggio di completamento.

---