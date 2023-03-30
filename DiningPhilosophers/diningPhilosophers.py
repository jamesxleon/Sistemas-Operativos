import threading
import time
import random

# Number of philosophers
N = 5

# Semaphore for controlling access to chopsticks
chopsticks = [threading.Semaphore(1) for i in range(N)]

# Semaphore for limiting number of eating philosophers
max_eating = threading.Semaphore(N-1)

# Philosopher class that inherits from threading.Thread
class Philosopher(threading.Thread):
    def __init__(self, id):
        # Call parent constructor
        super().__init__()
        # Assign id
        self.id = id
    
    def run(self):
        # Repeat forever
        while True:
            # Think for some time
            self.think()
            # Try to eat
            self.eat()
    
    def think(self):
        # Print thinking message
        print(f"Philosopher {self.id} is thinking.")
        # Sleep for some random time between 0 and 3 seconds
        time.sleep(random.uniform(0, 3))
    
    def eat(self):
        # Acquire max_eating semaphore 
        max_eating.acquire()
        
        # Acquire left chopstick (use min and max to avoid deadlock)
        chopsticks[min(self.id, (self.id + 1) % N)].acquire()
        
        # Acquire right chopstick 
        chopsticks[max(self.id, (self.id + 1) % N)].acquire()
        
        # Print eating message 
        print(f"Philosopher {self.id} is eating.")
        
        # Sleep for some random time between 0 and 3 seconds 
        time.sleep(random.uniform(0, 3))
         
        # Release right chopstick 
        chopsticks[max(self.id, (self.id + 1) % N)].release()
         
        # Release left chopstick 
        chopsticks[min(self.id, (self.id + 1) % N)].release()
         
        # Release max_eating semaphore 
        max_eating.release()

# Create a list of philosophers 
philosophers = [Philosopher(i) for i in range(N)]

# Start all philosophers 
for p in philosophers:
    p.start()