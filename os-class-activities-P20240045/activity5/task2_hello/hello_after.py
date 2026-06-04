import threading

a = threading.Semaphore(1)
b = threading.Semaphore(0)
c = threading.Semaphore(0)
d = threading.Semaphore(3)

def process1():
    while True:
        d.acquire()
        a.acquire()

        print("H", end="", flush=True)
        print("E", end="", flush=True)

        b.release()
        b.release()

def process2():
    while True:
        b.acquire()

        print("L", end="", flush=True)

        c.release()

def process3():
    while True:
        c.acquire()
        c.acquire()

        print("O", end="", flush=True)
        print()
        a.release()


t1 = threading.Thread(target=process1)
t2 = threading.Thread(target=process2)
t3 = threading.Thread(target=process3)

t1.start()
t2.start()
t3.start()

t1.join()
t2.join()
t3.join()

print()
