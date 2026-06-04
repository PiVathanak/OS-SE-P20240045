"""
Before semaphore version - intentionally unsafe/incomplete synchronization.

This file intentionally demonstrates an unsafe/incomplete implementation
of the Producer/Consumer simulation: semaphores are declared but the
consumer/producer do not use them correctly (possible race conditions).
Use this file to observe incorrect behavior and compare with
the corrected `particles_after.py`.
"""

import threading
import time
import random
from threading import Semaphore

BUFFER_CAPACITY = 50  # max number of pairs

# Shared buffer (unsafe access)
buffer = []

# Semaphores are defined but NOT used correctly here (intentional)
space = Semaphore(BUFFER_CAPACITY)  # starts full of free slots
s     = Semaphore(0)                # starts with 0 pairs available
lock  = Semaphore(1)                # binary mutex

# ── Producer (unsafe) ────────────────────────────────────────────────────────
def Producer(id):
    while True:
        P1 = random.randint(1, 100)
        P2 = random.randint(1, 100)

        # Intentionally only partially using semaphores to show the bug:
        # we acquire `space` but never release `s` or use `lock`.
        acquired = space.acquire(timeout=50)
        if not acquired:
            print(f"[Producer {id}] ERROR: Buffer is FULL ({BUFFER_CAPACITY} pairs). Stopping.")
            break

        # --- Critical section (UNSAFE - no mutex) ---
        buffer.append((P1, P2))
        buf_size = len(buffer)
        print(f"[Producer {id}] Produced pair: ({P1}, {P2})  | buffer={buf_size}/{BUFFER_CAPACITY}")

        time.sleep(random.uniform(0.3, 0.8))


# ── Consumer (unsafe) ────────────────────────────────────────────────────────
def Consumer():
    while True:

        # --- Critical section (UNSAFE - no semaphore or mutex) ---
        if not buffer:
            # busy-wait to simulate an unsafe consumer
            time.sleep(0.1)
            continue

        P1, P2 = buffer.pop(0)
        buf_size = len(buffer)
        print(f"[Consumer]    Consumed pair:  ({P1}, {P2})  | buffer={buf_size}/{BUFFER_CAPACITY}")

        time.sleep(random.uniform(0.5, 1.2))


# ── Main (unsafe) ───────────────────────────────────────────────────────────
if __name__ == '__main__':
    p1 = threading.Thread(target=Producer, args=(1,))
    p2 = threading.Thread(target=Producer, args=(2,))
    c1 = threading.Thread(target=Consumer)

    p1.start()
    p2.start()
    c1.start()

    p1.join()
    p2.join()
    # consumer is unsafe and may never exit in the unsafe version; we don't join it

    print("\n=== Producers finished (consumer may still be running) ===")
    print(f"Remaining items in buffer: {len(buffer)}")
