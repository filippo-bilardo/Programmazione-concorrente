import os

def main():
    print(f"\nAvvio del processo con pid={os.getpid()}")
    while True:
        pass  # Loop infinito

if __name__ == "__main__":
    main()