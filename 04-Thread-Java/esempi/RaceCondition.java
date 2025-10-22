/**
 * Esempio 3: Race Condition (problema di sincronizzazione)
 */
public class RaceCondition {
    static class Contatore {
        private int valore = 0;
        
        public void incrementa() {
            valore++;  // NON atomico!
        }
        
        public int getValore() {
            return valore;
        }
    }
    
    public static void main(String[] args) throws InterruptedException {
        System.out.println("=== Race Condition Demo ===\n");
        
        Contatore counter = new Contatore();
        
        // 100 thread incrementano 1000 volte ciascuno
        Thread[] threads = new Thread[100];
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
        
        System.out.println("Valore atteso: 100,000");
        System.out.println("Valore effettivo: " + counter.getValore());
        System.out.println("Differenza: " + (100000 - counter.getValore()));
    }
}
