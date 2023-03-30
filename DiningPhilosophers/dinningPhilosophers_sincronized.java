

public class dinningPhilosophers_sincronized {

   // Main method
   public static void main(String[] args) throws InterruptedException {
       int N = 5; // Number of philosophers
       Philosopher[] philosophers = new Philosopher[N]; // Creating an array of philosopher objects
       Object[] chopsticks = new Object[N]; // Creating an array of object for chopsticks
   
       // Initializing the chopsticks array with objects
       for (int i = 0; i < N; i++) {
           chopsticks[i] = new Object();
       }
   
       // Initializing the philosopher objects and starting their threads
       for (int i = 0; i < N; i++) {
           philosophers[i] = new Philosopher(i, chopsticks[i], chopsticks[(i + 1) % N]);
           philosophers[i].start();
       }
   }
   
   // Philosopher class that extends Thread
   private static class Philosopher extends Thread {
       private final int id; // The id of the philosopher.
       private final Object leftChopstick; // The object representing the left chopstick.
       private final Object rightChopstick; // The object representing the right chopstick.
   
       // Constructor to initialize the philosopher object
       public Philosopher(int id, Object leftChopstick, Object rightChopstick) {
           this.id = id;
           this.leftChopstick = leftChopstick;
           this.rightChopstick = rightChopstick;
       }
   
       // Method to simulate the philosopher thinking
       private void think() throws InterruptedException {
           int time1 = (int) (Math.random() * (4000)) + 1000; // Random sleep time between 1-5 seconds
           sleep(time1);
           System.out.println("Philosopher " + id + " is thinking for " + time1 + " milliseconds");
   
           int time2= (int) (Math.random() * (4000)) + 1000; // Random sleep time between 1-5 seconds
           sleep(time2);
           System.out.println("Philosopher " + id + " is hungry");
       }
   
       // Method to simulate the philosopher eating.
       private void eat() throws InterruptedException {
           synchronized(leftChopstick) { // Acquiring lock on left chopstick object
               while (true) {
                   synchronized(rightChopstick) { // Acquiring lock on right chopstick object
                       int time3 = (int) (Math.random() * (4000)) + 1000; // Random sleep time between 1-5 seconds
                       sleep(time3);
                       System.out.println("Philosopher " + id + " is eating for " + time3 + " milliseconds");
                       //Release the lock on the right chopstick object and prevent a deadlock situation
                       break;
                   }
               }
           }
       }
   
       // Overriding run method of Thread class
       public void run() {
           try {
               while (true) {
                   think();
                   eat();
               }
           } catch (InterruptedException e) {
               Thread.currentThread().interrupt();
               return;
           }
       }
   }
   
}

