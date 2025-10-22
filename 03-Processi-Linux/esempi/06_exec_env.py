#!/usr/bin/env python3
"""Esempio di exec() con environment personalizzato"""
import os

def esegui_con_env(comando, args, env_vars):
    """Esegue comando con environment custom"""
    pid = os.fork()
    
    if pid == 0:  # Child
        # Prepara environment
        new_env = {
            'PATH': '/bin:/usr/bin',
            'HOME': '/tmp',
        }
        new_env.update(env_vars)
        
        try:
            os.execve(comando, [comando] + args, new_env)
        except OSError as e:
            print(f"Errore: {e}")
            os._exit(1)
    else:  # Parent
        os.wait()

# Test
print("Eseguo script Python con MY_VAR=test")

# Crea script temporaneo
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
