/**
 * Esempio 2: Creazione thread con Runnable (approccio consigliato)
 */
public class RunnableDemo {
    static class ContatoreRunnable implements Runnable {
        private String nome;
        
        public ContatoreRunnable(String nome) {
            this.nome = nome;
        }
        
        @Override
        public void run() {
            for (int i = 1; i <= 5; i++) {
                System.out.println(nome + ": " + i);
                try {
                    Thread.sleep(300);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
        }
    }
    
    public static void main(String[] args) {
        System.out.println("=== Thread con Runnable ===\n");
        
        Thread t1 = new Thread(new ContatoreRunnable("Runnable-A"));
        Thread t2 = new Thread(new ContatoreRunnable("Runnable-B"));
        
        // Lambda (Java 8+)
        Thread t3 = new Thread(() -> {
            for (int i = 1; i <= 5; i++) {
                System.out.println("Lambda-C: " + i);
                try {
                    Thread.sleep(300);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
        });
        
        t1.start();
        t2.start();
        t3.start();
        
        try {
            t1.join();
            t2.join();
            t3.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        
        System.out.println("\nCompletato!");
    }
}
