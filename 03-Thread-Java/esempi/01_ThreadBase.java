/**
 * Esempio 1: Creazione base thread (estendendo Thread)
 */
public class 01_ThreadBase {
    static class MioThread extends Thread {
        private String nome;
        
        public MioThread(String nome) {
            this.nome = nome;
        }
        
        @Override
        public void run() {
            for (int i = 1; i <= 5; i++) {
                System.out.println(nome + ": conteggio " + i);
                try {
                    Thread.sleep(500);
                } catch (InterruptedException e) {
                    System.out.println(nome + " interrotto");
                    return;
                }
            }
            System.out.println(nome + " completato");
        }
    }
    
    public static void main(String[] args) {
        System.out.println("=== Esempio: Thread Base ===\n");
        
        MioThread t1 = new MioThread("Thread-A");
        MioThread t2 = new MioThread("Thread-B");
        
        t1.start();
        t2.start();
        
        try {
            t1.join();
            t2.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        
        System.out.println("\nTutti i thread completati!");
    }
}
