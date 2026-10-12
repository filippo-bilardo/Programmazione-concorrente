import os


print(f"\nAvvio del processo con pid={os.getpid()}")
while True:
    pass  # Loop infinito

print(f"\nQuesto codice non verrà mai eseguito, perché il processo è in loop infinito")
 