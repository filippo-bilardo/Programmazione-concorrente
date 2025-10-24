/**
 * Esempio 6: Deadlock (e come evitarlo)
 */
public class DeadlockDemo {
    private final Object lock1 = new Object();
    private final Object lock2 = new Object();
    
    public void metodo1() {
        synchronized(lock1) {
            System.out.println(Thread.currentThread().getName() + ": acquisito lock1");
            try { Thread.sleep(50); } catch (InterruptedException e) {}
            
            System.out.println(Thread.currentThread().getName() + ": attendo lock2...");
            synchronized(lock2) {
                System.out.println(Thread.currentThread().getName() + ": acquisito lock2");
            }
        }
    }
    
    public void metodo2() {
        synchronized(lock2) {
            System.out.println(Thread.currentThread().getName() + ": acquisito lock2");
            try { Thread.sleep(50); } catch (InterruptedException e) {}
            
            System.out.println(Thread.currentThread().getName() + ": attendo lock1...");
            synchronized(lock1) {
                System.out.println(Thread.currentThread().getName() + ": acquisito lock1");
            }
        }
    }
    
    public static void main(String[] args) throws InterruptedException {
        System.out.println("=== Deadlock Demo ===\n");
        System.out.println("Questo esempio causa un DEADLOCK!");
        System.out.println("Premi Ctrl+C per terminare\n");
        
        DeadlockDemo demo = new DeadlockDemo();
        
        Thread t1 = new Thread(() -> demo.metodo1(), "Thread-1");
        Thread t2 = new Thread(() -> demo.metodo2(), "Thread-2");
        
        t1.start();
        t2.start();
        
        Thread.sleep(5000);
        
        System.out.println("\nDEADLOCK rilevato!");
        System.out.println("Thread-1 ha lock1, attende lock2");
        System.out.println("Thread-2 ha lock2, attende lock1");
        
        System.exit(0);
    }
}
