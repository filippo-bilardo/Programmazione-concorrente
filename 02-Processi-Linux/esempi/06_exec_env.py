#!/usr/bin/env python3
# =============================================================================
# File: 06_exec_env.py
# Argomento: Avvio di un programma con ambiente personalizzato
# Scopo: Passa a execve() un insieme esplicito di variabili di ambiente.
# Esecuzione: python3 -u 06_exec_env.py
# Nota: Richiede Linux/Unix; crea o sovrascrive /tmp/test_env.py e lo esegue.
# =============================================================================
"""Esempio di exec() con environment personalizzato"""
import os

def esegui_con_env(comando, args, env_vars):
    """Esegue comando con environment custom"""
    pid = os.fork()
    
    if pid == 0:  # Child
        # Questo dizionario sostituisce l’ambiente ereditato: non copia os.environ.
        new_env = {
            'PATH': '/bin:/usr/bin',
            'HOME': '/tmp',
        }
        # Le variabili fornite dal chiamante aggiungono o sovrascrivono le chiavi.
        new_env.update(env_vars)
        
        try:
            # execve usa il percorso indicato; non cerca il programma nel PATH.
            os.execve(comando, [comando] + args, new_env)
        except OSError as e:
            print(f"Errore: {e}")
            os._exit(1)
    else:  # Parent
        os.wait()

# Test
print("Eseguo script Python con MY_VAR=test")

# Il piccolo script verifica le variabili viste dal nuovo interprete.
# La modalità w crea il file o ne sostituisce il contenuto esistente.
with open('/tmp/test_env.py', 'w') as f:
    f.write("""
import os
print(f"MY_VAR = {os.environ.get('MY_VAR', 'NON DEFINITA')}")
print(f"PATH = {os.environ.get('PATH')}")
""")

esegui_con_env(
    '/usr/bin/python3',
    ['/tmp/test_env.py'],
    {'MY_VAR': 'test_value'}
)
