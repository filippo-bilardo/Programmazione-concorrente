import java.util.concurrent.atomic.AtomicInteger;

/**
 * Esempio 9: AtomicInteger (lock-free counter)
 */
public class AtomicIntegerDemo {
    static class ContatoreAtomic {
        private AtomicInteger valore = new AtomicInteger(0);
        
        public void incrementa() {
            valore.incrementAndGet();  // Atomico, lock-free
        }
        
        public int get() {
            return valore.get();
        }
    }
    
    public static void main(String[] args) throws InterruptedException {
        System.out.println("=== AtomicInteger Demo ===\n");
        
        ContatoreAtomic counter = new ContatoreAtomic();
        
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
        
        System.out.println("Valore finale: " + counter.get());
        System.out.println("Tempo: " + elapsed + " ms");
        System.out.println("✓ CORRETTO (atomic, lock-free)");
    }
}
