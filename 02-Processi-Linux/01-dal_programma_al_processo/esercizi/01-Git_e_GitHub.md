# Git e GitHub per l'intera cartella processi

## Obiettivo

Creare un repository personale che contenga **l'intera cartella `processi`**, con
tutti i programmi, le sottocartelle e le risorse del corso di programmazione concorrente.
L'unità di lavoro è l'intera raccolta di esercizi, non soltanto il file Hello World.

Successivamente dovrai **collegarti a GitHub sia dal PC di casa sia dal PC di
scuola**, clonare il repository creato e continuare il lavoro da entrambe le
postazioni. In questa esercitazione imparerai quindi anche a creare una copia
locale in ciascun PC e a scambiare gli aggiornamenti tramite `git pull` e
`git push`. Avrai tre copie di lavoro: una su Web4Student, una a casa e una a
scuola, collegate allo stesso repository GitHub.

Il nome suggerito è **Programmazione concorrente**: nel campo del nome del
repository su GitHub usa **`Programmazione-concorrente`**, con il trattino al
posto dello spazio. La cartella locale resterà `processi`: il nome locale e
quello del repository remoto possono essere diversi.

## Prerequisiti e risultato finale

Prepara il tuo account GitHub e le credenziali di accesso a Web4Student.
Tutti i comandi, dopo il collegamento SSH, si eseguono nel terminale
**Web4Student**, salvo diversa indicazione. Per questo esercizio servono Git,
Python 3 e un editor di testo, per esempio `nano`.

Dal Passo 11 userai invece il **terminale locale del PC di casa o di scuola**.
Anche su queste postazioni dovranno essere disponibili Git e, per eseguire
l'esempio, Python 3. Su Windows puoi usare Git Bash per i comandi Git e SSH
riportati nell'esercitazione.

La struttura da ottenere è:

```text
~/processi/
├── .git/
├── .gitignore
├── README.md
├── tmp/                    file temporanei, esclusi da Git
├── es01b-hello-world/
│   └── hello.py
└── ... tutte le altre cartelle e risorse degli esercizi sui processi
```

Il repository viene inizializzato direttamente in `~/processi`: su GitHub
comparirà il suo contenuto, senza un ulteriore livello `processi`.
I programmi si eseguono dal terminale; questa cartella non è l'area del sito web.

## Passo 1 — Collegarsi e verificare gli strumenti

Dal computer locale:

```bash
ssh -p 2222 username@w4s.filippobilardo.it
```

Nel terminale remoto:

```bash
git --version
python3 --version
mkdir -p ~/processi
ls -la ~/processi
find ~/processi -name .git -print
```

`mkdir -p` crea la cartella se manca e conserva il contenuto se esiste già.
Il percorso guidato parte da una cartella che non contiene già repository Git.
Se l'ultimo comando elenca una `.git`, individua il repository esistente prima
di procedere. Se `~/processi` è già un repository, riutilizzalo; non creare
repository annidati nelle cartelle dei singoli esercizi.

## Passo 2 — Preparare tmp e il programma es01b-hello-world

Entra nella cartella `processi` del tuo account:

```bash
cd ~/processi
pwd
ls -la
```

Il percorso mostrato da `pwd` deve terminare con `/processi`.
Questa sarà la cartella di lavoro per gli esercizi del corso.

Crea la sottocartella per i file temporanei:

```bash
mkdir -p tmp
ls -ld tmp
```

Useremo `tmp` per dati di prova che non devono essere versionati. Al passo 5
creerai `.gitignore` per escluderne tutto il contenuto. Da ora lavora
direttamente sui sorgenti in `~/processi`.

### Creare il programma dell'esercizio b

Sempre da `~/processi`, crea la sottocartella e apri il nuovo file:

```bash
mkdir -p es01b-hello-world
nano es01b-hello-world/hello.py
```

Inserisci il codice seguente, sostituendo `Mario` con il tuo nome:

```python
#!/usr/bin/env python3
"""Hello World: dal programma Python al processo in esecuzione."""
import os

nome = 'Mario'
messaggio = "Hello World dell'esercizio b."

print('Hello, World!')
print(f'Studente: {nome}')
print(messaggio)
# Il sistema operativo assegna un PID al processo che esegue il programma.
print(f'PID del processo: {os.getpid()}')
```

Salva con `Ctrl+O`, conferma con `Invio` ed esci con `Ctrl+X`.
Controlla la sintassi ed esegui il programma:

```bash
python3 -m py_compile es01b-hello-world/hello.py
python3 es01b-hello-world/hello.py
```

**Verifica:** compaiono **Hello, World!**, il tuo nome, il messaggio
**Hello World dell'esercizio b.** e il PID del processo. Il nuovo programma
entrerà nel primo commit insieme a tutti gli altri sorgenti di `processi`.
`py_compile` può creare una cartella `__pycache__`, che escluderemo con `.gitignore`.

## Passo 3 — Configurare l'identità Git

Sul proprio account Web4Student, configura nome ed email per i commit:

```bash
git config --global user.name "Nome Cognome"
git config --global user.email "tua.email@example.com"
git config --global init.defaultBranch main
git config --global --get user.name
git config --global --get user.email
```

Usa l'email associata all'account GitHub oppure il tuo indirizzo `noreply`
fornito dalle impostazioni GitHub. Questi dati identificano l'autore dei commit;
non sono credenziali di accesso. Sono impostazioni globali: valgono per tutti i repository Git sul server Web4Student.

## Passo 4 — Preparare l'autenticazione SSH verso GitHub

**Scegli una delle due procedure alternative:** questo passo configura una
chiave senza passphrase, utilizzabile su un PC con **account personale protetto**
e accesso riservato ai propri file. Il **Passo 4 bis** configura invece una
chiave con passphrase, da preferire sul PC della scuola quando altre persone
potrebbero accedere al file della chiave privata. Dopo la procedura scelta,
prosegui con il Passo 5.

### Perché creare una chiave SSH?

Quando esegui `git push`, GitHub deve riconoscere il tuo account e verificare
che possa scrivere nel repository. Il nome e l'email configurati nel passo 3
identificano l'autore dei commit, ma **non permettono di accedere a GitHub**.

La chiave SSH serve ad autenticare la connessione tra Web4Student e GitHub.
Dopo la configurazione puoi usare comandi come `git push` e `git pull` con
l'URL SSH del repository, senza inserire ogni volta le credenziali GitHub.
`git commit`, invece, registra una versione locale e non richiede una connessione
a GitHub. Riferimento: [autenticazione SSH su GitHub](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/about-ssh).

### Una coppia di chiavi con due compiti diversi

`ssh-keygen` crea due file collegati tra loro:

| File | Dove rimane | A cosa serve |
|---|---|---|
| `w4s_github_ed25519` — chiave privata | Nella tua cartella `~/.ssh` su Web4Student | Permette al client SSH di dimostrare il possesso della chiave |
| `w4s_github_ed25519.pub` — chiave pubblica | Viene copiata nelle impostazioni del tuo account GitHub | Permette a GitHub di verificare l'autenticazione e associarla al tuo account |

La chiave privata non va incollata su GitHub né aggiunta al repository.
Registrare quella pubblica non concede nuovi permessi: puoi fare push soltanto
nei repository su cui il tuo account ha già il diritto di scrittura.

### Perché la chiave viene creata su Web4Student?

In questo esercizio i comandi Git vengono eseguiti sul server Web4Student:
è quindi quel server a dover presentare la tua chiave a GitHub. Una chiave
presente soltanto sul tuo computer non viene automaticamente usata dalla
sessione remota.

Le connessioni sono distinte:

| Collegamento | Account e porta |
|---|---|
| Computer → Web4Student | `username`, porta 2222 |
| Web4Student → GitHub | Utente SSH `git`, normalmente porta 22 |

Il primo accesso ti porta nel tuo account Web4Student; il secondo ti identifica
su GitHub tramite la chiave. Nell'indirizzo `git@github.com` l'utente SSH è
`git`: il tuo account GitHub viene riconosciuto dalla chiave registrata.

### Generare la coppia di chiavi

La cartella `~/.ssh` esiste già. Controlla le chiavi presenti:

```bash
ls -la ~/.ssh
```

Se non hai già una chiave adatta a GitHub, creala, il nome del file è libero:

```bash
ssh-keygen -t ed25519 -C "tua.email@example.com" -f ~/.ssh/w4s_github_ed25519 -N ""
```

| Opzione | Significato |
|---|---|
| `-t ed25519` | Sceglie l'algoritmo della coppia di chiavi |
| `-C "tua.email@example.com"` | Aggiunge un'etichetta per riconoscere la chiave; non esegue l'accesso con quell'email |
| `-f ~/.ssh/w4s_github_ed25519` | Sceglie il nome della chiave privata; la pubblica avrà lo stesso nome seguito da `.pub` |
| `-N ""` | Imposta una passphrase vuota, come previsto per il collegamento automatico di questo esercizio |

Non sovrascrivere una chiave già utilizzata. La
[guida GitHub alla generazione delle chiavi](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent)
approfondisce coppia di chiavi, nomi dei file e passphrase.

### Registrare la chiave pubblica su GitHub

Visualizza la chiave pubblica:

```bash
cat ~/.ssh/w4s_github_ed25519.pub
```

Il file con estensione **`.pub`** contiene la chiave pubblica da registrare su
GitHub. La chiave privata resta nel tuo ambiente.

**Copia l'intera chiave pubblica** (inizia con `ssh-ed25519...`) e aggiungila su GitHub:

1. Vai su [https://github.com/settings/keys](https://github.com/settings/keys)
2. Clicca su "New SSH key"
3. Dai un titolo (es: "Web4Student user")
4. Incolla la chiave nel campo "Key"
5. Clicca "Add SSH key"

### Indicare a SSH quale chiave usare

Nel terminale remoto apri `nano ~/.ssh/config` e inserisci questo blocco;
se esiste già un blocco per GitHub, integra quello esistente:

```sshconfig
Host github.com
    HostName github.com
    User git
    IdentityFile ~/.ssh/w4s_github_ed25519
    IdentitiesOnly yes
```

Salva e verifica:

> SSH userà direttamente la chiave indicata da `IdentityFile`, senza chiedere
una passphrase e senza richiedere l'avvio di `ssh-agent` a ogni sessione.
Qui “automatico” riguarda l'autenticazione: per inviare le modifiche dovrai
comunque creare i commit ed eseguire `git push`.

```bash
chmod 600 ~/.ssh/config ~/.ssh/w4s_github_ed25519
ssh -T git@github.com
```

Alla prima connessione confronta l'impronta presentata con le
[impronte ufficiali GitHub](https://docs.github.com/en/authentication/keeping-your-account-and-data-secure/githubs-ssh-key-fingerprints).
Il messaggio di successo deve identificare il tuo account e indicare
l'autenticazione riuscita; GitHub non fornisce una shell interattiva. In questo
test il codice di uscita 1 può accompagnare l'autenticazione riuscita, come
spiegato nella [guida al test SSH](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/testing-your-ssh-connection).


CONGRATULAZIONI! La connessione SSH tra Web4Student e GitHub funziona.
Se hai scelto questa procedura, puoi proseguire direttamente con il Passo 5.

## Passo 4 bis — Creare una chiave con passphrase per il PC della scuola

Questa procedura è **alternativa al Passo 4**, non un secondo collegamento
obbligatorio. Usala quando lavori su un PC della scuola o in un ambiente in cui
altri potrebbero vedere o copiare i file del tuo account.

### Perché proteggere la chiave con una password?

La chiave privata permette di autenticarsi su GitHub. Se non ha una passphrase,
chi ne ottiene una copia può usarla senza dover sbloccare il file. La passphrase
cifra invece la chiave privata salvata: per utilizzarla serve anche conoscere
questa password. Scegli una frase lunga, difficile da indovinare e personale.

La passphrase non è la password di GitHub o di Web4Student: serve a sbloccare
**localmente il file della chiave privata**. Il nome del file e la chiave pubblica
possono essere conosciuti da altri; è la chiave privata che deve restare riservata.
Vedi la [guida GitHub alle passphrase SSH](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/working-with-ssh-key-passphrases).

### 1. Scegliere il terminale e un nome riconoscibile

Esegui i comandi nell'ambiente che userà Git:

- Se Git viene eseguito sul PC della scuola, usa il suo terminale, per esempio Git Bash su Windows: la chiave resterà nella sua cartella `~/.ssh`.
- Se Git viene eseguito nella sessione SSH Web4Student, usa quel terminale: la chiave resterà su Web4Student. Collegarsi dal PC della scuola non trasferisce automaticamente la chiave sul PC.

Nel nome della nuova chiave inserisci **il tuo nome e cognome** e il suffisso
**`_pass`**. Nell'esempio usiamo `mario_rossi`: sostituiscilo in tutti i comandi
e nella configurazione con il tuo nome, senza spazi.

```bash
ls -la ~/.ssh
ssh-keygen -t ed25519 -C "Mario Rossi - PC scuola - tua.email@example.com" -f ~/.ssh/pc_github_mario_rossi_ed25519_pass
```

Personalizza anche il commento dopo `-C`. Non aggiungere `-N ""`: in questa
procedura devi impostare una passphrase non vuota. Se il nome del file è già
occupato, scegli un nome diverso senza sovrascrivere una chiave esistente.

### 2. Inserire e confermare la passphrase

Il programma chiede:

```text
Enter passphrase (empty for no passphrase):
Enter same passphrase again:
```

Digita la passphrase alla prima richiesta e ripetila alla seconda. I caratteri
digitati non vengono mostrati nel terminale. Non scriverla nel comando, nel
README o nel repository.

Otterrai due file:

| File | Uso |
|---|---|
| `~/.ssh/pc_github_mario_rossi_ed25519_pass` | Chiave privata cifrata, da mantenere nel proprio ambiente |
| `~/.ssh/pc_github_mario_rossi_ed25519_pass.pub` | Chiave pubblica da registrare su GitHub |

Il suffisso `_pass` aiuta a riconoscere la variante: la protezione dipende dalla
passphrase impostata, non dal nome scelto.

### 3. Registrare la nuova chiave pubblica su GitHub

```bash
cat ~/.ssh/pc_github_mario_rossi_ed25519_pass.pub
```

Copia l'intera chiave pubblica e, nel browser GitHub:

1. Apri **Settings → SSH and GPG keys → New SSH key**.
2. Scegli il tipo **Authentication Key**.
3. Usa un titolo riconoscibile, per esempio `Mario Rossi - PC scuola - pass`.
4. Incolla la chiave pubblica nel campo **Key** e conferma con **Add SSH key**.

Se hai creato questa nuova coppia, devi registrare la sua chiave pubblica:
quella della variante senza passphrase appartiene a una coppia diversa.

### 4. Configurare SSH per usare la chiave protetta

Apri `~/.ssh/config` con un editor: puoi usare `nano ~/.ssh/config` nel terminale
oppure Blocco note su Windows, salvando il file come `config`, senza `.txt`.
Inserisci il blocco seguente oppure sostituisci il precedente blocco
`Host github.com`, così la connessione usa la chiave protetta:

```sshconfig
Host github.com
    HostName github.com
    User git
    IdentityFile ~/.ssh/pc_github_mario_rossi_ed25519_pass
    IdentitiesOnly yes
    IdentityAgent none
```

`IdentityFile` seleziona la chiave con il tuo nome e `_pass`. Per questa prova
su un PC condiviso, `IdentityAgent none` evita di usare chiavi già sbloccate
in un agente SSH: la passphrase verrà richiesta quando usi il file.
L'opzione è descritta nel [manuale OpenSSH](https://man.openbsd.org/ssh_config#IdentityAgent).

### 5. Verificare la connessione

Nel terminale dell'ambiente in cui hai configurato la chiave, esegui:

```bash
ssh -T git@github.com
```

Quando compare una richiesta simile alla seguente, digita la passphrase:

```text
Enter passphrase for key '.../pc_github_mario_rossi_ed25519_pass':
```

**Verifica:** dopo lo sblocco GitHub riconosce il tuo account. Anche `git push`
e `git pull` tramite SSH richiederanno lo sblocco della chiave con questa
configurazione. Ora puoi continuare dal Passo 5.

### Quale alternativa usare?

| Situazione | Procedura |
|---|---|
| PC con account personale protetto e file accessibili soltanto al proprietario | Passo 4: chiave senza passphrase, per il collegamento automatico |
| PC della scuola o ambiente in cui altri potrebbero accedere al file privato | Passo 4 bis: chiave con nome personale, suffisso `_pass` e passphrase |

La passphrase protegge il file salvato, non una sessione già accessibile ad
altre persone: al termine del lavoro chiudi le sessioni ed esci dal tuo account.

## Passo 5 — Documentare il progetto e scegliere i file da tracciare

Torna alla cartella `processi`, che sarà la radice del repository:

```bash
cd ~/processi
nano README.md
```

Scrivi almeno:

```markdown
# Programmazione concorrente

Esercitazioni di programmazione concorrente di Nome Cognome, classe ...

Questo repository contiene i programmi, gli esercizi e le risorse sui processi Linux.
Il programma dell'esercizio Git e GitHub è es01b-hello-world/hello.py.

Su Web4Student lavoro nella cartella personale ~/processi.
Per eseguire Hello World uso: python3 es01b-hello-world/hello.py
La sottocartella tmp contiene file temporanei e viene esclusa da Git.
```

Crea il file `.gitignore` nella stessa cartella:

```bash
nano .gitignore
```

Inserisci queste regole; se il file esiste già, integrale con quelle presenti:

```gitignore
/tmp/
__pycache__/
*.pyc
*.o
a.out
*.log
*.tmp
*.swp
*~
.env
.env.*
!.env.example
```

La regola `/tmp/` esclude la cartella temporanea nella radice del repository
e tutto il suo contenuto. Le altre regole escludono cache Python, file oggetto C,
l'eseguibile predefinito `a.out`, temporanei e file di ambiente con credenziali reali.
Gli altri sorgenti di `processi`, incluse eventuali sottocartelle e risorse,
devono entrare nel repository. `.gitignore` non rimuove file già tracciati.

Per verificare l'esclusione, crea un file di prova nella cartella `tmp`:

```bash
touch tmp/prova.txt
```

Lo controllerai dopo l'inizializzazione del repository.

## Passo 6 — Creare il repository locale e il primo commit

Esegui questi comandi **dentro `~/processi`**, non nella sottocartella del singolo
esercizio:

```bash
cd ~/processi
git init -b main                   # inizializza il repository con branch main
git rev-parse --show-toplevel      # mostra la radice del repository
git check-ignore -v tmp/prova.txt  # verifica l'esclusione del file temporaneo
git add .                         # aggiunge tutti i file non ignorati
git status                        # mostra i file preparati per il commit
git diff --cached --stat           # mostra le modifiche preparate
git diff --cached                  # mostra il contenuto delle modifiche preparate
```

**Verifica:** la radice termina con `/processi`; `git check-ignore` indica la regola
`/tmp/`. Nell'elenco dei file compaiono tutti i sorgenti e le risorse previste,
non soltanto Hello World, mentre `tmp/prova.txt` e le cache Python non compaiono
tra i file preparati per il commit. Il file `.gitignore` deve invece essere incluso.

Se l'elenco è corretto:

```bash
git commit -m "Importa l'intera cartella processi del corso di programmazione concorrente"
git log --oneline
git ls-files
```

L'ultimo comando elenca i file effettivamente tracciati. La cartella
`tmp` resta disponibile sul server ma, essendo ignorata, non viene trasferita
con un clone: potrai ricrearla con `mkdir -p tmp`.
Git registra file, non cartelle vuote. Vedi i manuali di
[git init](https://git-scm.com/docs/git-init) e [git add](https://git-scm.com/docs/git-add).

## Passo 7 — Creare il repository vuoto su GitHub

**Dove:** browser, nel tuo account GitHub.

1. Crea un nuovo repository: il nome suggerito è **Programmazione concorrente**, da scrivere come **`Programmazione-concorrente`** nel campo **Repository name**.
2. Scegli la visibilità secondo le indicazioni del corso.
3. Non aggiungere da GitHub README, `.gitignore` o licenza: il repository remoto deve partire vuoto.
4. Copia l'URL SSH, nella forma `git@github.com:TUO-ACCOUNT/Programmazione-concorrente.git`.

`TUO-ACCOUNT` è l'account GitHub, che può essere diverso dallo username Web4Student.
Se scegli un nome diverso, usa il corrispondente URL nei comandi successivi.
Questa procedura segue la [guida GitHub all'importazione di codice locale](https://docs.github.com/en/migrations/importing-source-code/using-the-command-line-to-import-source-code/adding-locally-hosted-code-to-github).

## Passo 8 — Collegare il remoto e fare push

Nel terminale Web4Student, dalla cartella `~/processi`:

```bash
cd ~/processi
git remote add origin git@github.com:TUO-ACCOUNT/Programmazione-concorrente.git
git remote -v
git push -u origin main
```

Aggiorna la pagina GitHub del repository. Devi vedere `README.md`, `.gitignore`
e **tutto il contenuto versionato di `processi`**, con `es01b-hello-world`
direttamente alla radice. Apri `es01b-hello-world/hello.py`
e confrontalo con il tuo file.
La cartella `tmp` e i suoi file non devono comparire.

`git push` invia i commit: non invia automaticamente modifiche non ancora
registrate. Il comando `python3 es01b-hello-world/hello.py` esegue invece il file
presente in `~/processi`, indipendentemente dal push.

CONGRATULAZIONI! Hai completato l'importazione degli esercizi su GitHub e puoi passare al passo successivo.

## Passo 9 — Registrare una modifica

Modifica direttamente il programma, sempre da `~/processi`:

```bash
nano es01b-hello-world/hello.py
```

Cambia il valore del messaggio in:

```python
messaggio = 'Hello World sotto controllo versione con Git.'
```

Poi controlla, registra e invia:

```bash
python3 -m py_compile es01b-hello-world/hello.py
git diff -- es01b-hello-world/hello.py
git add .
git commit -m "Aggiorna il messaggio di Hello World"
git push
git log --oneline -2
```

**Verifica:** se sei partito da un repository nuovo, GitHub mostra due commit
e il nuovo messaggio nel sorgente.

CONGRATULAZIONI! Hai completato la modifica e il push su GitHub. Puoi passare al passo successivo.

## Passo 10 — Verificare il programma e i file esclusi

Il file modificato si trova già nella cartella di lavoro. Non devi copiarlo
in un'altra directory. Esegui nuovamente:

```bash
cd ~/processi
python3 es01b-hello-world/hello.py
```

Ora deve comparire il nuovo messaggio.

Controlla anche lo stato del repository e l'esclusione del file temporaneo:

```bash
git status
git check-ignore -v tmp/prova.txt
git ls-files -- tmp
```

L'ultimo comando non deve elencare file. `.gitignore` esclude `tmp` dal controllo
di versione, ma non protegge i file sul server: non usarla per conservare
credenziali o dati riservati.

## Passo 11 — Preparare l'accesso a GitHub dai due PC

**Dove:** prima sul PC di casa, poi sul PC di scuola, nel tuo account personale.
Ripeti la preparazione su ciascuna postazione.

Apri un nuovo terminale **locale**. Se sei ancora nella sessione SSH di
Web4Student, usa `exit` per tornare al terminale del PC. I comandi di questo
passo devono operare sui file del computer che stai usando.

Verifica Git e configura la tua identità:

```bash
git --version
git config --global user.name "Nome Cognome"
git config --global user.email "tua.email@example.com"
```

Usa la stessa identità del Passo 3. Le impostazioni globali valgono per l'utente
del computer corrente: quelle configurate su Web4Student non vengono trasferite
automaticamente al PC.

Prepara la cartella delle chiavi, se non esiste:

```bash
mkdir -p ~/.ssh
ls -la ~/.ssh
```

Configura poi l'autenticazione seguendo le procedure già illustrate:

- **PC di casa:** segui il Passo 4 se usi un account personale protetto, oppure
  il Passo 4 bis per una chiave con passphrase.
- **PC di scuola:** segui il Passo 4 bis, usando una chiave con il tuo nome,
  il suffisso `_pass` e una passphrase personale.

Esegui quelle procedure nel terminale locale. Crea una coppia di chiavi per
ciascuna postazione e registra ogni chiave pubblica sullo **stesso account
GitHub**, con titoli riconoscibili come `Nome Cognome - PC casa` e
`Nome Cognome - PC scuola`. Non copiare la chiave privata da Web4Student o da
un PC all'altro. Se sul PC hai già una chiave personale configurata per GitHub,
puoi riutilizzarla.

Verifica l'accesso da ciascun PC:

```bash
ssh -T git@github.com
```

**Verifica:** il messaggio identifica il tuo account GitHub. L'accesso SSH a
Web4Student e l'autenticazione SSH verso GitHub sono due collegamenti distinti:
per clonare sul PC userai direttamente GitHub.

## Passo 12 — Clonare il repository sul PC di casa

**Dove:** terminale locale del PC di casa, dopo la preparazione del Passo 11.

Dal browser apri il repository creato al Passo 7, scegli **Code → SSH** e copia
il suo URL. Sostituisci `TUO-ACCOUNT` nei comandi con il tuo account GitHub e,
se hai scelto un nome diverso, adatta anche il nome del repository.

Spostati nella tua cartella personale e clona:

```bash
cd ~
git clone git@github.com:TUO-ACCOUNT/Programmazione-concorrente.git processi
cd ~/processi
git remote -v
git status
git log --oneline -2
```

Il comando `git clone` scarica i file versionati e la cronologia, crea la
cartella locale **`processi`** e configura automaticamente il remoto `origin`.
L'ultimo argomento sceglie il nome della cartella: senza `processi`, Git userebbe
il nome `Programmazione-concorrente`.

Non eseguire `git init` o `git remote add origin` dopo il clone: il repository
locale e il collegamento al remoto sono già pronti. Esegui il clone una sola
volta per postazione. Se `~/processi` esiste già ed è una cartella non vuota,
controllane il contenuto prima di procedere: se è già la copia di questo
repository, riutilizzala e aggiornala con il Passo 14; altrimenti scegli una
cartella di destinazione diversa, senza cancellare i file presenti.

Ricrea la cartella temporanea ed esegui l'esempio:

```bash
mkdir -p tmp
python3 es01b-hello-world/hello.py
```

Su Windows, se Python è disponibile tramite il launcher `py`, usa
`py -3 es01b-hello-world/hello.py` al posto del comando `python3`.
Gli esempi successivi che usano funzioni specifiche di Linux, come `os.fork()`,
andranno eseguiti in un ambiente Linux, per esempio Web4Student o WSL.

**Verifica:** `origin` punta al tuo repository GitHub, la cronologia contiene
i commit già inviati e Hello World mostra il messaggio aggiornato. `tmp` non
era presente nel clone perché è esclusa da Git.

## Passo 13 — Clonare il repository sul PC di scuola

**Dove:** terminale locale del PC di scuola, nel tuo account personale e dopo
aver configurato la chiave con passphrase al Passo 11.

Clona lo **stesso repository GitHub** usato a casa:

```bash
cd ~
git clone git@github.com:TUO-ACCOUNT/Programmazione-concorrente.git processi
cd ~/processi
git remote -v
git status
git log --oneline -2
mkdir -p tmp
```

Quando SSH richiede la passphrase della chiave locale, inseriscila nel terminale.
Anche qui, se `~/processi` contiene già una copia del repository, passa
all'aggiornamento del Passo 14 senza ripetere il clone.

Apri `es01b-hello-world/hello.py` e verifica che contenga il messaggio inviato
su GitHub. Puoi eseguirlo con lo stesso comando usato sul PC di casa.

**Verifica:** ora hai una copia su ciascuna postazione. Ogni copia possiede la
propria cartella `.git`, ma tutte usano lo stesso remoto GitHub. Le modifiche
salvate su un PC non compaiono automaticamente sugli altri.

## Passo 14 — Continuare il lavoro tra casa, scuola e Web4Student

**All'inizio di ogni sessione**, dalla copia su cui stai per lavorare:

```bash
cd ~/processi
git status
git pull --ff-only
```

Prima del pull verifica che non ci siano modifiche locali da completare e
registrare. `git pull --ff-only` scarica e applica gli aggiornamenti quando
la cronologia locale può avanzare senza creare un commit di merge. Se segnala
storie divergenti, confronta le modifiche prima di integrarle.

Per provare il passaggio tra postazioni, **dal PC di casa** apri `README.md`
con il tuo editor e aggiungi una riga, per esempio:

```text
Ho clonato il repository e verificato il programma sul PC di casa.
```

Controlla, registra e invia la modifica:

```bash
git diff -- README.md
git add README.md
git commit -m "Documenta la verifica dal PC di casa"
git push
```

**Dal PC di scuola**, entra nella sua copia e ricevi l'aggiornamento:

```bash
cd ~/processi
git status
git pull --ff-only
git log --oneline -3
```

Apri `README.md`: deve comparire la riga aggiunta a casa. Aggiungi una seconda
riga che documenti la verifica dal PC di scuola, poi esegui di nuovo
`git diff -- README.md`, `git add README.md`, un commit con un messaggio
appropriato e `git push`.

Alla successiva sessione sul PC di casa o su Web4Student, esegui il pull nella
rispettiva cartella `~/processi` e verifica la presenza di entrambe le righe.
Per aggiornare la copia Web4Student devi prima collegarti al server tramite SSH,
come nel Passo 1.

**Alla fine di ogni sessione**, controlla le modifiche, crea i commit del lavoro
completato e fai push prima di cambiare postazione. Il solo salvataggio o il
solo commit non trasferiscono il lavoro su GitHub. Al termine del lavoro a
scuola chiudi le sessioni ed esci dal tuo account.

## Comandi da usare durante il lavoro

| Azione | Comando da `~/processi` |
|---|---|
| Controllare lo stato | `git status` |
| Leggere modifiche non ancora preparate | `git diff` |
| Preparare modifiche all'intera raccolta di esercizi | `git add .` |
| Registrare una versione | `git commit -m "Descrizione della modifica"` |
| Inviare i commit | `git push` |
| Leggere la cronologia | `git log --oneline` |
| Ricevere aggiornamenti senza merge automatici | `git pull --ff-only` con la cartella di lavoro pulita, prima di iniziare nuove modifiche |

Se un push viene rifiutato perché il remoto è avanzato, controlla lo stato
e ricevi gli aggiornamenti. Se `--ff-only` segnala storie divergenti, confronta
le modifiche prima di integrare: non serve un push forzato per questo esercizio.

## Consegna e autovalutazione

Consegna URL del repository, output dell'esecuzione di `es01b-hello-world/hello.py`
e identificativi dei primi due commit e dei commit di verifica da casa e da
scuola. Verifica che il repository comprenda anche gli altri sorgenti e le
risorse presenti in `processi`. Documenta il clone sulle due postazioni e la
ricezione degli aggiornamenti mostrando l'output di `git remote -v` e
`git log --oneline -4` da ciascun PC, dopo la sincronizzazione.

Spiega:

1. Quale cartella costituisce la radice del repository e dove vengono modificati i programmi?
2. Che cosa cambia tra salvare un file, fare commit e fare push?
3. Perché il programma eseguito da `~/processi` mostra una modifica salvata anche prima del push?
4. Quale file della coppia SSH viene registrato su GitHub?
5. Quale regola esclude `tmp` e come puoi verificarne l'effetto?
6. Che cosa crea `git clone` e perché non devi eseguire nuovamente `git init`?
7. Perché ogni postazione deve avere una propria configurazione di accesso a GitHub?
8. Quali comandi usi per trasferire il lavoro dal PC di casa a quello di scuola?

---

[Torna ai concetti fondamentali sui processi](../01.%20Concetti%20fondamentali%20sui%20processi.md)
