/**
 * Esempio 4: Synchronized per risolvere race condition
 */
public class SynchronizedCounter {
    static class ContatoreSync {
        private int valore = 0;
        
        public synchronized void incrementa() {
            valore++;  // Atomico grazie a synchronized
        }
        
        public synchronized int getValore() {
            return valore;
        }
    }
    
    public static void main(String[] args) throws InterruptedException {
        System.out.println("=== Synchronized Counter ===\n");
        
        ContatoreSync counter = new ContatoreSync();
        
        Thread[] threads = new Thread[100];
        long start = System.currentTimeMillis();
        
        for (int i = 0; i < 100; i++) {
            threads[i] = new Thread(() -> {
                for (int j = 0; j < 1000; j++) {
                    counter.incrementa();
                }
            });
            threads[i].start();
        }
        
        for (Thread t : threads) {
            t.join();
        }
        
        long elapsed = System.currentTimeMillis() - start;
        
        System.out.println("Valore finale: " + counter.getValore());
        System.out.println("Tempo: " + elapsed + " ms");
        System.out.println("✓ CORRETTO (grazie a synchronized)");
    }
}
