#!/usr/bin/env python3
"""Elaborazione parallela di file"""
from multiprocessing import Pool
import os
import time

def conta_parole(filepath):
    """Conta le parole in un file"""
    try:
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
            words = len(content.split())
            return (filepath, words)
    except:
        return (filepath, 0)

def trova_file_txt(directory):
    """Trova tutti i file .txt in una directory"""
    txt_files = []
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith('.txt') or file.endswith('.md') or file.endswith('.py'):
                txt_files.append(os.path.join(root, file))
    return txt_files

if __name__ == '__main__':
    # Directory da analizzare
    directory = '/home/git-projects/TPSIT_2_MY/A-Processi_sequenziali/Corso Programmazione Concorrente'
    
    print("=== Elaborazione Parallela File ===\n")
    print(f"Directory: {directory}\n")
    
    # Trova file
    print("Ricerca file...")
    files = trova_file_txt(directory)
    print(f"Trovati {len(files)} file\n")
    
    if not files:
        print("Nessun file trovato")
        exit()
    
    # Elabora con Pool
    print("Elaborazione in corso...\n")
    start = time.time()
    
    with Pool(processes=4) as pool:
        results = pool.map(conta_parole, files)
    
    elapsed = time.time() - start
    
    # Risultati
    total_words = sum(words for _, words in results)
    
    print("Top 10 file per numero parole:")
    results_sorted = sorted(results, key=lambda x: x[1], reverse=True)
    for path, words in results_sorted[:10]:
        filename = os.path.basename(path)
        print(f"  {words:>6} parole - {filename}")
    
    print(f"\nTotale: {total_words} parole in {len(files)} file")
    print(f"Tempo: {elapsed:.2f}s")
