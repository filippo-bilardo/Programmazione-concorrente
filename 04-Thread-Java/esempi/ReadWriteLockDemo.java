import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

/**
 * Esempio 13: ReadWriteLock (lock separati read/write)
 */
public class ReadWriteLockDemo {
    static class Cache {
        private Map<String, String> map = new HashMap<>();
        private ReadWriteLock rwLock = new ReentrantReadWriteLock();
        
        public String get(String key) {
            rwLock.readLock().lock();
            try {
                Thread.sleep(10);  // Simula lettura
                return map.get(key);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                return null;
            } finally {
                rwLock.readLock().unlock();
            }
        }
        
        public void put(String key, String value) {
            rwLock.writeLock().lock();
            try {
                Thread.sleep(50);  // Simula scrittura
                map.put(key, value);
                System.out.println("Scritto: " + key + " = " + value);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            } finally {
                rwLock.writeLock().unlock();
            }
        }
    }
    
    public static void main(String[] args) throws InterruptedException {
        System.out.println("=== ReadWriteLock Demo ===\n");
        
        Cache cache = new Cache();
        
        // 1 Writer
        Thread writer = new Thread(() -> {
            for (int i = 0; i < 5; i++) {
                cache.put("key" + i, "value" + i);
            }
        });
        
        // 10 Readers
        Thread[] readers = new Thread[10];
        for (int i = 0; i < 10; i++) {
            final int readerId = i;
            readers[i] = new Thread(() -> {
                for (int j = 0; j < 5; j++) {
                    String value = cache.get("key0");
                    if (value != null) {
                        System.out.println("Reader-" + readerId + " legge: " + value);
                    }
                }
            });
        }
        
        writer.start();
        for (Thread reader : readers) {
            reader.start();
        }
        
        writer.join();
        for (Thread reader : readers) {
            reader.join();
        }
        
        System.out.println("\n✓ Multipli reader concorrenti, writer esclusivo");
    }
}
