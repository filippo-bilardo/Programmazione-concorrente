## H - Altri argomenti
### Esercitazioni
### Teoria
- [1.1 - Multitasking nella vita quotidiana](<1.1 - Multitasking nella Vita Quotidiana.md>)
- [1.2 - Multitasking con i microcontrollori](<1.2 - Multitasking con i microcontrollori.md>)
- [1.3 - Multi Tasking in Arduino](<1.3 - Multi Tasking in Arduino.md>)
- [1.4 - Sistema di Irrigazione intelligente](<1.4 - Sistema di Irrigazione intelligente.md>)
- [1.5 - Responsività](<1.5 - Responsività.md>)

--- 
[INDICE](../README.md) 

---
https://wokwi.com/projects/423168989470781441 - Arduino TASK
https://wokwi.com/projects/357812590739230721 - Led task mini
https://wokwi.com/projects/422171332361372673 - FSM_Pulsante_OOP_v10
https://wokwi.com/projects/357819864002560001 - OOP-Led task

https://wokwi.com/projects/423301259081486337 GestioneParcheggio_Task_OOP Copy
https://wokwi.com/projects/422167806095033345 My OOP-Leds_Pulsanti3.ino
https://wokwi.com/projects/387463839792162817 My_Led_Pulsanti_task

---
2. **Panoramica delle tecniche di gestione degli eventi**  
   - Differenze generali tra polling e interrupt  
   - Quando e perché scegliere una tecnica rispetto all'altra

3. **Polling**  
   - Definizione e funzionamento  
   - Vantaggi: semplicità, controllo diretto sul flusso  
   - Svantaggi: inefficienza in termini di consumo CPU e latenze  
   - Esempi pratici: cicli di controllo in microcontrollori, lettura di sensori

4. **Interrupt**  
   - Meccanismo di funzionamento: segnalazione asincrona e ISR (Interrupt Service Routine)  
   - Vantaggi: risposta rapida agli eventi, migliore utilizzo delle risorse  
   - Svantaggi: complessità nella gestione, possibilità di conflitti e problemi di concorrenza  
   - Esempi pratici: gestione di input esterni, comunicazioni, eventi critici in tempo reale

5. **Confronto tra polling e interrupt**  
   - Analisi in termini di latenza, efficienza energetica e complessità implementativa  
   - Trade-off: contesto d'uso ideale per ciascuna tecnica

6. **Analoghe nella vita reale**  
   - **Polling vs. Interrupt**:  
     - **Polling**: come controllare continuamente la posta elettronica o monitorare costantemente un indicatore (es. un vigilante che ispeziona ripetutamente una porta)  
     - **Interrupt**: come rispondere a un campanello o a un messaggio push che ti avverte solo quando c'è qualcosa di importante  
   - Esempi pratici:  
     - In cucina: un cuoco che controlla continuamente la cottura (polling) vs. uno che si affida a un timer che suona al momento giusto (interrupt)  
     - Gestione delle attività quotidiane: monitorare continuamente una situazione rispetto a reagire solo quando si riceve una notifica urgente

7. **Implementazione e gestione del multitasking**  
   - Strategie di scheduling: round-robin, priorità, preemption  
   - Gestione della concorrenza e delle risorse condivise  
   - Strumenti di debugging e profilazione in ambienti embedded

8. **Casi di studio ed esempi pratici**  
   - Analisi di sistemi reali in cui vengono utilizzati polling e interrupt  
   - Dimostrazioni pratiche o simulazioni per evidenziare le differenze comportamentali
