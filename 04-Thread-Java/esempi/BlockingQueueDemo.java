import java.util.concurrent.*;

/**
 * Esempio 11: BlockingQueue (producer-consumer avanzato)
 */
public class BlockingQueueDemo {
    public static void main(String[] args) throws InterruptedException {
        System.out.println("=== BlockingQueue Demo ===\n");
        
        BlockingQueue<Integer> queue = new ArrayBlockingQueue<>(5);
        
        // Producer
        Thread producer = new Thread(() -> {
            try {
                for (int i = 1; i <= 10; i++) {
                    queue.put(i);  // Blocca se piena
                    System.out.println("Prodotto: " + i + " (size: " + queue.size() + ")");
                    Thread.sleep(100);
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        });
        
        // Consumer
        Thread consumer = new Thread(() -> {
            try {
                for (int i = 1; i <= 10; i++) {
                    Integer item = queue.take();  // Blocca se vuota
                    System.out.println("Consumato: " + item + " (size: " + queue.size() + ")");
                    Thread.sleep(300);  // Consumer più lento
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        });
        
        producer.start();
        consumer.start();
        
        producer.join();
        consumer.join();
        
        System.out.println("\nCompletato!");
    }
}
