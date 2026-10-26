# Esercitazione 03 — Git senza privilegi amministrativi sul PC di scuola

## Obiettivo

Installare **Git for Windows per il solo utente corrente**, usando il normale
installer dal proprio account scolastico senza privilegi amministrativi.
Avviare poi Git Bash, configurare l'identità e verificare un commit locale.
PortableGit rimane un'alternativa se il laboratorio ne prevede l'uso.

L'installer ufficiale gestisce anche l'installazione da utenti non
amministratori. Non tutte le versioni mostrano una scelta esplicita “solo per
me”: l'ambito dipende anche dall'account e dai privilegi con cui si avvia il
Setup. Il [codice ufficiale dell'installer](https://github.com/git-for-windows/build-extra/blob/main/installer/install.iss)
prevede impostazioni per l'utente corrente quando l'installazione non viene
eseguita come amministratore.

Questa procedura prepara la postazione scolastica per
[Git e GitHub per processi](01-Git_e_GitHub.md). Se Git Bash è già disponibile
e funzionante, puoi riutilizzarlo e documentare le verifiche.

## Prima di iniziare: documenta il lavoro

Nel documento Google dell'esercitazione principale aggiungi la sezione
**`Esercitazione 03 - Git senza amministratore a scuola`**. Se manca, crea il
documento **`Cognome Nome - Classe - Git e GitHub per processi`**.

Indica data, sistema operativo e postazione. Per ogni passo raccogli screenshot
leggibili con una didascalia che descriva il risultato. Non mostrare password,
passphrase o contenuto delle chiavi private. Le prove richieste sono elencate
alla fine dell'esercitazione.

## Prerequisiti

- PC Windows con architettura compatibile con il pacchetto scelto, x64 o ARM64.
- Account personale e cartella in cui puoi scrivere ed eseguire programmi,
  secondo le indicazioni del docente o del laboratorio.
- Connessione al sito ufficiale per il download oppure pacchetto ufficiale
  fornito dal docente.
- Browser, editor di testo e strumento per acquisire screenshot.
- Accesso al documento Google della relazione.

Verifica con il docente se la cartella personale viene conservata al logout
o al riavvio. **Non servono credenziali amministrative**, ma il laboratorio
deve consentire l'installazione di applicazioni nel profilo dell'utente
oppure l'esecuzione di applicazioni portabili. Se una regola del PC
blocca il programma, segnala il messaggio al docente: questa procedura non
richiede di disattivare protezioni o aggirare restrizioni.

Su un PC scolastico Linux o macOS usa Git già predisposto dal laboratorio;
se manca, chiedi al docente la modalità prevista. I pacchetti Setup e
PortableGit qui descritti sono per Windows.

## Passo 1 — Controllare la postazione

Cerca Git Bash nel menu Start. Se esiste, aprilo e prova:

```bash
git --version
command -v git
```

Se funziona, passa al Passo 5. Altrimenti apri le informazioni di sistema di
Windows e verifica l'architettura del processore per scegliere il pacchetto.
Tutti i comandi di questa esercitazione vanno eseguiti **localmente sul PC di
scuola**, non nella sessione SSH Web4Student.

## Passo 2 — Scaricare il normale installer

Apri la [pagina ufficiale di Git per Windows](https://git-scm.com/install/windows).
Nella sezione **Standalone Installer** scegli **Setup x64** oppure **Setup
ARM64**, in base al PC. Scarica la versione stabile proposta dal sito.

Il file ha un nome del tipo `Git-<versione>-64-bit.exe` oppure
`Git-<versione>-arm64.exe`. Per questa procedura usa il normale **Setup**:
non occorre scaricare PortableGit. Se il docente ha già fornito il pacchetto
ufficiale adatto, usa quello.

## Passo 3 — Installare soltanto per l'utente corrente

### 3.1 Avviare il Setup dal proprio account

1. Accedi con il tuo **account scolastico standard**, non con quello di un amministratore.
2. Apri il file Setup con un doppio clic normale, senza usare **Esegui come amministratore**.
3. Se la versione presenta la scelta tra tutti gli utenti e l'utente corrente,
   scegli **solo per me / utente corrente**.
4. Se non compare questa scelta, prosegui nell'account standard e controlla
   la destinazione al punto successivo: l'assenza della schermata non significa
   che servano privilegi amministrativi.

Se viene richiesta una password amministrativa, annulla quella richiesta e
segnalalo al docente. Non inserire credenziali di altri utenti. Una precedente
installazione di sistema o le regole del laboratorio possono richiedere una
gestione diversa. Potrai usare il Passo 4 bis se PortableGit è consentito.

### 3.2 Scegliere una cartella personale

Alla schermata della destinazione, scegli una cartella scrivibile del tuo
profilo, per esempio:

```text
C:\Users\Mario\AppData\Local\Programs\Git
```

Sostituisci `Mario` con il nome effettivo della cartella del tuo account.
Per individuarla puoi aprire Esplora file, digitare `%LOCALAPPDATA%` nella
barra degli indirizzi e ricavare il percorso completo. Nel Setup usa quel
percorso seguito da `\Programs\Git`, creandolo tramite la procedura guidata.

Se il Setup propone già una destinazione nel tuo profilo, puoi mantenerla.
Non scegliere `C:\Program Files`, `C:\Windows` o cartelle di altri utenti.
Se rileva una precedente installazione di sistema e non consente di scegliere
la destinazione personale, fermati e verifica con il docente.

**Attenzione:** cambiare soltanto la cartella non rende un'installazione
“per utente” se il Setup è stato avviato come amministratore. Occorrono sia
l'esecuzione dall'account standard sia una destinazione nel proprio profilo.

### 3.3 Completare le opzioni del Setup

| Impostazione | Scelta per questa esercitazione |
|---|---|
| Componenti ed editor | Mantieni i componenti proposti e scegli un editor disponibile che sai usare. |
| Branch iniziale | Imposta `main`, se viene richiesto. |
| PATH | Scegli **Use Git from Git Bash only**: svolgeremo il lavoro in Git Bash senza aggiungere Git al PATH di Windows. |
| SSH | Scegli **Use bundled OpenSSH**, per utilizzare il client incluso. |
| Altre opzioni | Mantieni le impostazioni proposte, salvo indicazioni del laboratorio. |

Le opzioni sono documentate nella
[guida ufficiale alle schermate del Setup](https://gitforwindows.org/mapping-between-git-installer-gui-settings-and-command-line-arguments.html).
Completa l'installazione e annota la cartella scelta nella relazione.

**Verifica:** il Setup termina senza credenziali amministrative e nella
cartella personale scelta è presente `git-bash.exe`.

## Passo 4 — Avviare Git Bash installato per l'utente

Cerca **Git Bash** nel menu Start del tuo account. In alternativa apri
`git-bash.exe` direttamente dalla cartella di installazione personale.
Puoi creare un collegamento sul tuo desktop per le lezioni successive.

Esegui i comandi dell'attività in questa finestra di **Git Bash**. Avendo
scelto “Git Bash only”, è normale che PowerShell non riconosca `git`:
Git Bash prepara il proprio ambiente e permette comunque di lavorare.
Prosegui al Passo 5; non serve eseguire anche la procedura portabile.

## Passo 4 bis — Alternativa con PortableGit

Usa questa alternativa se il docente la preferisce o se l'installazione per
utente non è disponibile e l'esecuzione di applicazioni portabili è consentita.
Non occorre completare entrambe le procedure.

1. Dalla [pagina ufficiale](https://git-scm.com/install/windows) scarica
   **Portable ("thumbdrive edition")** per x64 o ARM64.
2. Il file ha un nome come `PortableGit-<versione>-64-bit.7z.exe` oppure
   `PortableGit-<versione>-arm64.7z.exe`: è un archivio autoestraente.
3. Apri `%USERPROFILE%` in Esplora file e crea la cartella `Strumenti`.
4. Avvia l'archivio con un doppio clic normale ed estrailo in
   `Strumenti\PortableGit` dentro il tuo profilo.
5. Apri `git-bash.exe` dalla cartella estratta e passa alle verifiche del Passo 5.

Mantieni l'intera struttura estratta, comprese le sottocartelle `cmd` e `usr`.
Non basta copiare soltanto `git-bash.exe` o `git.exe`. Se la destinazione
contiene già una copia funzionante, riutilizzala oppure estrai la nuova
versione in una cartella distinta.

Se anche il programma portabile è bloccato, mostra il messaggio al docente
per usare l'ambiente previsto dal laboratorio.

## Passo 5 — Verificare strumenti e cartella personale

Da Git Bash:

```bash
git --version
command -v git
ssh -V
command -v ssh-keygen
command -v scp
cd ~
pwd
cygpath -w ~
```

**Verifica:** i comandi mostrano versioni e percorsi. `cygpath -w ~` mostra
la cartella personale nel formato Windows. `command -v git` può mostrare un
percorso interno come `/ucrt64/bin/git`: non è la cartella in cui devi salvare
i progetti.

Per vedere il percorso Windows dell'eseguibile scelto, con entrambi i metodi:

```bash
cygpath -w "$(command -v git)"
```

Controlla che corrisponda alla copia che intendi usare. Distingui queste posizioni:

| Posizione | Contenuto |
|---|---|
| Cartella personale scelta nel Setup, per esempio `AppData\Local\Programs\Git` | Programmi e file installati per il tuo utente. |
| `Strumenti\PortableGit`, se usi l'alternativa | Programmi e file estratti del pacchetto portabile. |
| `~/.gitconfig` | Configurazione Git dell'utente, nel caso usuale. |
| `~/.ssh` | Configurazione e chiavi SSH dell'utente. |
| `~/processi` | Futuro clone del repository del corso. |

**Installazione e dati personali hanno posizioni distinte.** Controlla dove
si trova `~`: configurazione, chiavi e repository sono nel tuo ambiente di
lavoro, non necessariamente nella cartella del programma. Anche usando
PortableGit, spostare la cartella del programma non trasferisce automaticamente
questi dati.

## Passo 6 — Configurare Git nel proprio account

Con il tuo account personale Windows, sostituisci i dati di esempio:

```bash
git config --global user.name "Nome Cognome"
git config --global user.email "tua.email@example.com"
git config --global init.defaultBranch main
git config --global --show-origin --get user.name
git config --global --show-origin --get user.email
git config --global --get init.defaultBranch
```

`--show-origin` indica anche da quale file proviene il valore. `--global`
scrive nella configurazione dell'utente corrente e non richiede di modificare
la configurazione di sistema; il significato dei livelli è spiegato nella
[guida ufficiale alla configurazione](https://git-scm.com/book/en/v2/Getting-Started-First-Time-Git-Setup).

Se il laboratorio usa un unico account Windows condiviso, chiedi al docente
come separare il lavoro. Per il solo repository di prova puoi impostare
`user.name` e `user.email` con `git config --local` dopo `git init`, anziché
modificare i valori globali di altri studenti. Questo non rende riservati
i file o le chiavi SSH: per l'attività autenticata serve l'ambiente personale
previsto dal laboratorio.

## Passo 7 — Creare un commit di prova senza amministratore

Scegli una cartella nuova. Se `~/prova-git-scuola` esiste, cambia il nome nei
comandi oppure riutilizza la prova già completata. Esegui una riga alla volta,
proseguendo soltanto se la precedente è riuscita:

```bash
cd ~
mkdir prova-git-scuola
cd prova-git-scuola
git init -b main
printf 'Prova di Git senza amministratore sul PC di scuola.\n' > README.md
git add README.md
git commit -m "Verifica Git sul PC di scuola"
git log --oneline -1
git status
```

Se hai concordato la configurazione locale, inserisci dopo `git init` e prima
del commit `git config --local user.name "Nome Cognome"` e
`git config --local user.email "tua.email@example.com"` con i tuoi dati.

**Verifica:** compare un commit e lo stato finale non presenta modifiche
da registrare. Questa prova locale non richiede GitHub, chiavi SSH o push.
Non creare il repository di prova dentro la cartella di installazione di Git
o di estrazione di PortableGit.

## Passo 8 — Prepararsi al lavoro nelle lezioni successive

Chiudi Git Bash, riaprilo dal menu Start o dallo stesso `git-bash.exe` usato
in precedenza e controlla:

```bash
cd ~/prova-git-scuola
git log --oneline -1
git status
```

Se hai usato un nome diverso, adatta il percorso. **Verifica:** la cronologia
è ancora presente. Questa prova verifica la riapertura del terminale; per la
conservazione dei dati dopo logout o riavvio valgono le regole del laboratorio.

Torna a [Git e GitHub per processi](01-Git_e_GitHub.md): al Passo 11 prepara
la postazione scolastica con la chiave distinta e protetta del **Passo 4 bis**,
poi segui il Passo 13 per clonare in `~/processi`. Installare Git per l'utente
o usare PortableGit non richiede di copiare sul PC di scuola la chiave privata
di Web4Student.

Alla fine della lezione fai commit e push del lavoro del corso, quando il
collegamento GitHub sarà configurato, quindi esci dagli account. Se il profilo
è temporaneo, il solo commit locale non preserva il lavoro dopo la cancellazione
del profilo. Non salvare chiavi private nel repository o nella relazione.

## Problemi frequenti

| Problema | Come procedere |
|---|---|
| Il Setup propone `Program Files` | Controlla di averlo avviato dall'account standard e scegli la destinazione personale. |
| Il Setup chiede una password amministrativa | Annulla la richiesta e verifica con il docente; usa PortableGit se previsto dal laboratorio. |
| Non compare la scelta “solo per me” | Il Setup può determinare l'ambito dai privilegi dell'account; segui il Passo 3 senza elevazione e verifica la destinazione. |
| Il programma è bloccato dalle regole del laboratorio | Mostra il messaggio al docente e usa l'ambiente autorizzato. |
| `git` non funziona in PowerShell | Con “Git Bash only” o PortableGit è previsto: usa Git Bash. |
| Il collegamento sul desktop non funziona più | Controlla il percorso di `git-bash.exe`; con PortableGit verifica se la cartella è stata spostata. |
| Il commit usa il nome di un altro studente | Controlla l'account e l'origine della configurazione prima di proseguire. |
| Il giorno dopo mancano i file | Verifica con il docente se il profilo è temporaneo e ripristina il lavoro del corso dal repository remoto. |

## Consegna e autovalutazione

Completa la sezione del documento Google e consegna il link al docente con
permesso di lettura, attraverso il canale indicato per il corso.

| Prova | Screenshot richiesto |
|---|---|
| Pacchetto | Pagina ufficiale e nome del Setup scelto, oppure PortableGit se hai seguito l'alternativa. |
| Ambito e destinazione | Eventuale scelta “solo per me” e percorso di installazione nel profilo personale. Se la scelta non compare, spiegalo nella didascalia. |
| Completamento | Esito del Setup e cartella contenente `git-bash.exe`; per PortableGit mostra invece la cartella estratta. |
| Avvio e strumenti | Git Bash, versioni di Git/SSH e percorsi verificati al Passo 5. |
| Identità e posizione dei dati | Configurazione con origine dei valori e percorso della cartella personale. |
| Commit di prova | Cronologia e stato del repository dopo aver riaperto Git Bash. |

Rispondi con parole tue:

1. Qual è la differenza tra installazione per l'utente corrente, installazione per tutti gli utenti e pacchetto PortableGit?
2. Perché puoi lavorare senza modificare il PATH di sistema?
3. Perché Git può funzionare in Git Bash e non essere riconosciuto in PowerShell?
4. Dove si trovano programma, repository, configurazione e chiavi? Quali percorsi hai verificato?
5. Che cosa cambia tra `git config --global` e `git config --local`?
6. Perché un commit locale non basta a conservare il lavoro se il profilo scolastico viene cancellato?
7. Come hai verificato che il programma usato è quello installato o estratto nella tua cartella personale?
8. Perché la chiave del PC di scuola deve seguire il Passo 4 bis dell'esercitazione principale?
9. Perché scegliere una cartella personale non basta se il Setup è stato avviato come amministratore?

---

[Installazione sul PC di casa](02-installazione-di-git.md) · [Git e GitHub per processi](01-Git_e_GitHub.md)
