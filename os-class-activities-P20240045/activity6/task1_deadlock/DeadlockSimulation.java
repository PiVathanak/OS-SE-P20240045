import java.util.concurrent.Semaphore;

class Account {
    String name;
    int balance;
    Semaphore lock = new Semaphore(1);

    Account(String name, int balance) {
        this.name = name;
        this.balance = balance;
    }
}

class Transfer {

    static volatile boolean completed = false;

    static void transfer(Account from, Account to, int amount) {
        try {
            System.out.println(Thread.currentThread().getName()
                    + " trying to lock FROM " + from.name);

            from.lock.acquire();

            System.out.println(Thread.currentThread().getName()
                    + " locked FROM " + from.name);

            Thread.sleep(1000);

            System.out.println(Thread.currentThread().getName()
                    + " waiting for TO " + to.name);

            to.lock.acquire();

            System.out.println(Thread.currentThread().getName()
                    + " locked TO " + to.name);

            from.balance -= amount;
            to.balance += amount;

            completed = true;

            System.out.println(Thread.currentThread().getName()
                    + " transfer completed");

            System.out.println(from.name + " Balance: " + from.balance);
            System.out.println(to.name + " Balance: " + to.balance);

            to.lock.release();
            from.lock.release();

        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}

public class DeadlockSimulation {

    public static void main(String[] args) throws InterruptedException {

        Account account1 = new Account("Account-A", 1000);
        Account account2 = new Account("Account-B", 1000);

        System.out.println("Starting Balances");
        System.out.println(account1.name + ": " + account1.balance);
        System.out.println(account2.name + ": " + account2.balance);
        System.out.println();

        Thread t1 = new Thread(
                () -> Transfer.transfer(account1, account2, 100),
                "Worker-1");

        Thread t2 = new Thread(
                () -> Transfer.transfer(account2, account1, 200),
                "Worker-2");

        t1.start();
        t2.start();

        // Watchdog
        Thread.sleep(3000);

        if (!Transfer.completed) {
            System.out.println();
            System.out.println("Deadlock detected: transactions are stuck");
            System.out.println("Worker-1 is waiting for Account-B");
            System.out.println("Worker-2 is waiting for Account-A");
        }
    }
}