import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

/**
 * Esempio 12: ReentrantLock (lock esplicito)
 */
public class ReentrantLockDemo {
    static class ContatoreConLock {
        private int valore = 0;
        private Lock lock = new ReentrantLock();
        
        public void incrementa() {
            lock.lock();
            try {
                valore++;
            } finally {
                lock.unlock();
            }
        }
        
        public boolean tryIncrementa() {
            if (lock.tryLock()) {
                try {
                    valore++;
                    return true;
                } finally {
                    lock.unlock();
                }
            }
            return false;
        }
        
        public int getValore() {
            lock.lock();
            try {
                return valore;
            } finally {
                lock.unlock();
            }
        }
    }
    
    public static void main(String[] args) throws InterruptedException {
        System.out.println("=== ReentrantLock Demo ===\n");
        
        ContatoreConLock counter = new ContatoreConLock();
        
        Thread[] threads = new Thread[50];
        for (int i = 0; i < 50; i++) {
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
        
        System.out.println("Valore finale: " + counter.getValore());
        System.out.println("✓ Thread-safe con ReentrantLock");
    }
}
