import java.util.concurrent.*;

/**
 * Esempio 8: Callable e Future (task con ritorno valore)
 */
public class CallableFutureDemo {
    public static void main(String[] args) {
        System.out.println("=== Callable e Future Demo ===\n");
        
        ExecutorService executor = Executors.newFixedThreadPool(2);
        
        Callable<Integer> task1 = () -> {
            System.out.println("Task 1: calcolo somma...");
            Thread.sleep(2000);
            return 100 + 200;
        };
        
        Callable<Integer> task2 = () -> {
            System.out.println("Task 2: calcolo prodotto...");
            Thread.sleep(1500);
            return 10 * 20;
        };
        
        try {
            Future<Integer> future1 = executor.submit(task1);
            Future<Integer> future2 = executor.submit(task2);
            
            System.out.println("Main thread: lavoro in corso...\n");
            
            Integer risultato1 = future1.get();
            Integer risultato2 = future2.get();
            
            System.out.println("\nSomma: " + risultato1);
            System.out.println("Prodotto: " + risultato2);
            
        } catch (InterruptedException | ExecutionException e) {
            e.printStackTrace();
        } finally {
            executor.shutdown();
        }
    }
}
