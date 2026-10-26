# Esercitazione 02 — Installazione di Git sul PC di casa

## Obiettivo

Installare Git sul PC di casa, aprire il terminale corretto, configurare
l'identità dell'autore e verificare il funzionamento con un primo commit locale.
Al termine potrai proseguire con
[Git e GitHub per la cartella processi](01-Git_e_GitHub.md).

La procedura principale riguarda **Windows con Git for Windows e Git Bash**.
Se usi macOS o Linux, segui il Passo 2 bis e poi le verifiche comuni.
Git gestisce le versioni dei file sul computer; GitHub ospita repository online.
Installare Git non crea un account GitHub e non configura automaticamente SSH.

## Prima di iniziare: documenta il lavoro

Nel documento Google dell'esercitazione Git e GitHub aggiungi la sezione
**`Esercitazione 02 - Installazione di Git a casa`**. Se non lo hai ancora
creato, crea un documento intitolato
**`Cognome Nome - Classe - Git e GitHub per processi`**.

Indica data, sistema operativo e architettura del PC. Raccogli gli screenshot
durante il lavoro, aggiungendo per ciascuno il passo e una didascalia che
spieghi il risultato. La sezione finale elenca le prove da consegnare.

## Prerequisiti

- Accesso al tuo account personale sul PC di casa e a una cartella scrivibile.
- Connessione Internet e browser per scaricare Git dal sito ufficiale.
- Possibilità di installare programmi: se il sistema richiede credenziali
  amministrative, deve intervenire il proprietario del PC. Se non sono
  disponibili su Windows, usa la [procedura per utente corrente o portabile](03-installazione-git-no-admin.md).
- Account Google per la relazione e strumento di acquisizione screenshot.
- Account GitHub per la successiva esercitazione; non serve per il commit locale di prova.

Esegui i comandi **sul PC di casa**, non nella sessione SSH Web4Student.
Se sei collegato al server, usa `exit` per tornare al terminale locale.
Python non è necessario per questa prova di installazione e non viene
installato insieme a Git: servirà successivamente per eseguire `hello.py`.

## Passo 1 — Verificare se Git è già disponibile

Su Windows cerca **Git Bash** nel menu Start. Se è presente, aprilo ed esegui:

```bash
git --version
command -v git
```

Su macOS o Linux usa il terminale. Su Windows puoi anche provare
`git --version` in PowerShell: se il comando non è riconosciuto, Git potrebbe
comunque essere presente ma disponibile soltanto in Git Bash.

**Verifica:** se compare `git version ...`, annota la versione. Se Git Bash e
gli strumenti richiesti sono già funzionanti, passa al Passo 3. Non occorre
reinstallare Git soltanto per produrre gli screenshot.

## Passo 2 — Scaricare e installare Git su Windows

### 2.1 Scegliere il pacchetto

Apri **Impostazioni → Sistema → Informazioni** e controlla il tipo di sistema:
scegli **x64** per un PC con processore x64 oppure **ARM64** per Windows su ARM.

Apri la [pagina ufficiale di installazione per Windows](https://git-scm.com/install/windows)
e scegli il pacchetto **Standalone Installer / Setup** compatibile con il PC.
Usa la versione stabile proposta dal sito: il numero di versione cambia nel tempo.
L’installazione per utente corrente senza amministratore e la versione Portable sono trattate nell’esercitazione 03.

### 2.2 Eseguire la procedura guidata

Avvia il file scaricato. Se Windows richiede autorizzazione amministrativa,
fai completare la richiesta al proprietario del PC. Leggi le schermate e
segui queste scelte per il laboratorio; alcune opzioni possono cambiare
posizione o essere assenti nella versione installata:

| Impostazione | Scelta per l'esercitazione |
|---|---|
| Cartella di installazione e componenti | Mantieni le impostazioni proposte, salvo esigenze del PC. |
| Editor predefinito | Scegli un editor che sai usare e che è presente sul PC, per esempio Nano se proposto. |
| Nome iniziale del branch | Imposta `main`, se la schermata consente di sceglierlo. Lo configurerai anche al Passo 4. |
| Disponibilità di Git nel PATH | Scegli l'opzione che rende Git disponibile dalla riga di comando e da altri programmi, normalmente “Git from the command line and also from 3rd-party software”. |
| Client SSH | Scegli il client OpenSSH incluso, normalmente “Use bundled OpenSSH”. |
| Altre opzioni | Mantieni i valori proposti per questa prima installazione. |

Le opzioni sono descritte nella
[documentazione dell'installer di Git for Windows](https://gitforwindows.org/mapping-between-git-installer-gui-settings-and-command-line-arguments.html).
Completa l'installazione, chiudi i terminali già aperti e avvia **Git Bash**
dal menu Start. È il terminale con cui seguirai i comandi del corso:
Git for Windows include l'ambiente Bash illustrato nella
[presentazione ufficiale del progetto](https://gitforwindows.org/).

**Verifica:** si apre una finestra con un prompt in cui puoi digitare comandi.
Prosegui al Passo 3.

## Passo 2 bis — Alternativa per macOS e Linux

Segui soltanto la procedura del tuo sistema operativo.

### macOS

Se Git non è già disponibile, apri Terminale ed esegui:

```bash
xcode-select --install
```

Completa la procedura per i Command Line Tools, che includono Git, poi riapri
il terminale. Se risultano già installati, verifica direttamente `git --version`.
Riferimento: [installazione di Git su macOS](https://git-scm.com/install/mac).

### Ubuntu o Debian

Dal terminale del PC di casa, con un account autorizzato a usare `sudo`:

```bash
sudo apt update
sudo apt install git openssh-client
```

`git` fornisce il controllo di versione; `openssh-client` fornisce gli strumenti
SSH necessari all'esercitazione successiva. Per distribuzioni diverse usa
il relativo gestore di pacchetti, come indicato nella
[guida ufficiale per Linux](https://git-scm.com/install/linux).

## Passo 3 — Controllare Git e gli strumenti SSH

Apri Git Bash su Windows o il terminale su macOS/Linux ed esegui:

```bash
git --version
command -v git
ssh -V
command -v ssh-keygen
command -v scp
cd ~
pwd
```

**Verifica:** Git e SSH mostrano la versione; gli altri comandi mostrano
il percorso degli eseguibili. `pwd`, dopo `cd ~`, identifica la tua cartella
personale. In Git Bash un percorso Windows come `C:\Users\Mario` può apparire
come `/c/Users/Mario`.

`~` indica la cartella personale; la directory che contiene il programma Git
è una posizione diversa. Salverai i progetti nella tua cartella di lavoro,
non dentro la cartella di installazione di Git.

## Passo 4 — Configurare l'identità dell'autore

Sostituisci i dati di esempio con i tuoi:

```bash
git config --global user.name "Nome Cognome"
git config --global user.email "tua.email@example.com"
git config --global init.defaultBranch main
git config --global --get user.name
git config --global --get user.email
git config --global --get init.defaultBranch
```

Usa l'email associata a GitHub o l'indirizzo `noreply` fornito dal tuo account.
Nome ed email identificano l'autore dei commit; non sono credenziali di accesso.
`--global` riguarda l'utente corrente del PC, non tutti gli utenti e non il
server Web4Student. Riferimento:
[configurazione iniziale di Git](https://git-scm.com/book/en/v2/Getting-Started-First-Time-Git-Setup).

**Verifica:** le tre letture restituiscono i valori impostati.

## Passo 5 — Provare un repository locale

Usa una cartella nuova, separata dal futuro clone `~/processi`. Se
`~/prova-git-casa` esiste già, scegli un altro nome e adattalo nei comandi.
Esegui una riga alla volta e prosegui solo se la precedente è riuscita:

```bash
cd ~
mkdir prova-git-casa
cd prova-git-casa
git init -b main
printf 'Prova di Git sul PC di casa.\n' > README.md
git status
git add README.md
git commit -m "Verifica Git sul PC di casa"
git log --oneline -1
git status
```

`git init` crea il repository, `git add` prepara il file e `git commit` ne
registra una versione. **Verifica:** la cronologia mostra il commit e l'ultimo
`git status` indica che non ci sono modifiche da registrare.

Non serve un push: questa prova verifica Git sul PC e funziona anche senza
account GitHub. Conserva la cartella per mostrare il risultato al docente.

## Passo 6 — Riprendere l'esercitazione su GitHub

Torna a [Git e GitHub per processi](01-Git_e_GitHub.md). Se il repository
remoto è già pronto, completa il Passo 11 per la postazione di casa,
seguendo il **Passo 4 ter** per riutilizzare la chiave Web4Student, quindi
il Passo 12 per clonare in `~/processi`.

La cartella di prova non sostituisce il repository del corso. Se GitHub non è
ancora stato configurato, completa prima i passi su Web4Student indicati
nell'esercitazione principale.

## Problemi frequenti

| Problema | Come procedere |
|---|---|
| `git` non riconosciuto dopo l'installazione | Chiudi e riapri il terminale; prova Git Bash dal menu Start. |
| SSH o `scp` non disponibili in PowerShell | Esegui i controlli in Git Bash e verifica che l'installazione sia completa. |
| Il commit chiede nome ed email | Ripeti la configurazione del Passo 4 nel terminale e nell'account correnti. |
| Non puoi installare con privilegi amministrativi | Su Windows usa la procedura per utente corrente dell’esercitazione 03, oppure PortableGit. |
| Non sai dove sono stati salvati i file | Esegui `pwd` nella cartella del repository e osserva il percorso. |

## Consegna e autovalutazione

Completa la sezione dedicata nel documento Google e consegna il link secondo
le indicazioni del docente, con permesso di lettura per il suo account.

| Prova | Screenshot richiesto |
|---|---|
| Sistema utilizzato | Sistema operativo e architettura, senza dati personali non necessari. |
| Installazione | Pacchetto scelto e completamento della procedura, oppure verifica dell'installazione già presente. |
| Strumenti | Versioni e percorsi dei comandi del Passo 3. |
| Configurazione | Nome, email e branch iniziale letti con `git config`. |
| Prova locale | Commit, cronologia e stato finale del repository di prova. |

Rispondi con parole tue:

1. Quale differenza c'è tra Git, Git Bash e GitHub?
2. Come hai verificato di essere sul PC di casa e non su Web4Student?
3. Che cosa significa rendere Git disponibile nel PATH?
4. Che cosa configura `--global` e perché nome ed email non permettono l'accesso a GitHub?
5. Perché hai potuto fare un commit senza collegarti a GitHub?
6. Quali risultati dimostrano che l'installazione è pronta per l'esercitazione successiva?

---

[Git e GitHub per processi](01-Git_e_GitHub.md) · [Git senza amministratore a scuola](03-installazione-git-no-admin.md)
