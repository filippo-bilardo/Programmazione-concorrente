import java.util.concurrent.*;

/**
 * Esempio 7: ExecutorService con FixedThreadPool
 */
public class ExecutorServiceDemo {
    public static void main(String[] args) {
        System.out.println("=== ExecutorService Demo ===\n");
        
        ExecutorService executor = Executors.newFixedThreadPool(3);
        
        for (int i = 1; i <= 10; i++) {
            final int taskId = i;
            executor.submit(() -> {
                System.out.println("Task " + taskId + " eseguito da " + 
                                 Thread.currentThread().getName());
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            });
        }
        
        executor.shutdown();
        
        try {
            if (!executor.awaitTermination(60, TimeUnit.SECONDS)) {
                executor.shutdownNow();
            }
        } catch (InterruptedException e) {
            executor.shutdownNow();
        }
        
        System.out.println("\nTutti i task completati!");
    }
}
