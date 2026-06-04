import threading
import time
import random
from threading import Semaphore

BUFFER_CAPACITY = 50  # max number of pairs

# Shared buffer
buffer = []

# Semaphores
space = Semaphore(BUFFER_CAPACITY)  # starts full of free slots
s     = Semaphore(0)                # starts with 0 pairs available
lock  = Semaphore(1)                # binary mutex

# ── Producer ──────────────────────────────────────────────────────────────────
def Producer(id):
    while True:
        P1 = random.randint(1, 100)
        P2 = random.randint(1, 100)

        # wait(space) — non-blocking: error + break if buffer is full
        acquired = space.acquire(timeout=50)
        if not acquired:
            print(f"[Producer {id}] ERROR: Buffer is FULL ({BUFFER_CAPACITY} pairs). Stopping.")
            break
        

        # wait(lock)
        lock.acquire()

        # --- Critical section ---
        buffer.append((P1, P2))
        buf_size = len(buffer)
        print(f"[Producer {id}] Produced pair: ({P1}, {P2})  | buffer={buf_size}/{BUFFER_CAPACITY}")

        # signal(lock)
        lock.release()

        # signal(s) — announce a new pair is ready
        s.release()

        time.sleep(random.uniform(0.3, 0.8))

# ── Consumer ──────────────────────────────────────────────────────────────────
def Consumer():
    while True:

        # wait(s) — non-blocking: break if buffer is empty
        acquired = s.acquire(timeout=50)
        if not acquired:
            print("[Consumer] Buffer is EMPTY — nothing to consume. Stopping.")
            break

        # wait(lock)
        lock.acquire()

        # --- Critical section ---
        P1, P2 = buffer.pop(0)
        buf_size = len(buffer)
        print(f"[Consumer]    Consumed pair:  ({P1}, {P2})  | buffer={buf_size}/{BUFFER_CAPACITY}")

        # signal(lock)
        lock.release()

        # signal(space) — free up one slot
        space.release()

        time.sleep(random.uniform(0.5, 1.2))

# ── Main ──────────────────────────────────────────────────────────────────────
p1 = threading.Thread(target=Producer, args=(1,))
p2 = threading.Thread(target=Producer, args=(2,))
c1 = threading.Thread(target=Consumer)

p1.start()
p2.start()
c1.start()

p1.join()
p2.join()
c1.join()

print("\n=== All threads finished ===")
print(f"Remaining items in buffer: {len(buffer)}")
