import java.util.concurrent.ConcurrentHashMap;

/**
 * Esempio 10: ConcurrentHashMap (thread-safe map)
 */
public class ConcurrentHashMapDemo {
    static class ContatoreConcorrente {
        private ConcurrentHashMap<String, Integer> contatori = new ConcurrentHashMap<>();
        
        public void incrementa(String chiave) {
            contatori.merge(chiave, 1, Integer::sum);
        }
        
        public int get(String chiave) {
            return contatori.getOrDefault(chiave, 0);
        }
    }
    
    public static void main(String[] args) throws InterruptedException {
        System.out.println("=== ConcurrentHashMap Demo ===\n");
        
        ContatoreConcorrente cc = new ContatoreConcorrente();
        
        Thread[] threads = new Thread[50];
        for (int i = 0; i < 50; i++) {
            threads[i] = new Thread(() -> {
                for (int j = 0; j < 100; j++) {
                    cc.incrementa("A");
                    cc.incrementa("B");
                    cc.incrementa("C");
                }
            });
            threads[i].start();
        }
        
        for (Thread t : threads) {
            t.join();
        }
        
        System.out.println("Contatore A: " + cc.get("A"));
        System.out.println("Contatore B: " + cc.get("B"));
        System.out.println("Contatore C: " + cc.get("C"));
        System.out.println("\n✓ Thread-safe senza lock espliciti!");
    }
}
