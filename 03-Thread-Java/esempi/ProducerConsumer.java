import java.util.LinkedList;
import java.util.Queue;

/**
 * Esempio 5: Producer-Consumer con wait/notify
 */
public class ProducerConsumer {
    static class BufferCondiviso {
        private Queue<Integer> buffer = new LinkedList<>();
        private final int MAX_SIZE = 5;
        
        public synchronized void produce(int valore) throws InterruptedException {
            while (buffer.size() == MAX_SIZE) {
                System.out.println("Buffer pieno, producer attende...");
                wait();
            }
            
            buffer.add(valore);
            System.out.println("Prodotto: " + valore + " (size: " + buffer.size() + ")");
            notifyAll();
        }
        
        public synchronized int consume() throws InterruptedException {
            while (buffer.isEmpty()) {
                System.out.println("Buffer vuoto, consumer attende...");
                wait();
            }
            
            int valore = buffer.remove();
            System.out.println("Consumato: " + valore + " (size: " + buffer.size() + ")");
            notifyAll();
            return valore;
        }
    }
    
    public static void main(String[] args) {
        System.out.println("=== Producer-Consumer Pattern ===\n");
        
        BufferCondiviso buffer = new BufferCondiviso();
        
        // Producer
        Thread producer = new Thread(() -> {
            try {
                for (int i = 1; i <= 10; i++) {
                    buffer.produce(i);
                    Thread.sleep(200);
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        });
        
        // Consumer
        Thread consumer = new Thread(() -> {
            try {
                for (int i = 1; i <= 10; i++) {
                    buffer.consume();
                    Thread.sleep(500);  // Consumer più lento
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        });
        
        producer.start();
        consumer.start();
        
        try {
            producer.join();
            consumer.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        
        System.out.println("\nCompletato!");
    }
}
