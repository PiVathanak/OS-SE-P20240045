import java.util.concurrent.Semaphore;

class Account {
    String name;
    int balance;

    Account(String name, int balance) {
        this.name = name;
        this.balance = balance;
    }
}

class Transfer {

    // One shared mutex for all transfers
    static Semaphore mutex = new Semaphore(1);

    static void transfer(Account from, Account to, int amount) {
        try {
            System.out.println(Thread.currentThread().getName()
                    + " waiting for mutex");

            mutex.acquire();

            try {
                System.out.println(Thread.currentThread().getName()
                        + " entered critical section");

                Thread.sleep(1000);

                from.balance -= amount;
                to.balance += amount;

                System.out.println(Thread.currentThread().getName()
                        + " transferred $" + amount
                        + " from " + from.name
                        + " to " + to.name);

            } finally {
                System.out.println(Thread.currentThread().getName()
                        + " leaving critical section");

                mutex.release();
            }

        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}

public class DeadlockFixed {

    public static void main(String[] args) throws InterruptedException {

        Account accountA = new Account("Account-A", 1000);
        Account accountB = new Account("Account-B", 1000);

        int startingTotal = accountA.balance + accountB.balance;

        System.out.println("Starting Balances");
        System.out.println(accountA.name + ": " + accountA.balance);
        System.out.println(accountB.name + ": " + accountB.balance);
        System.out.println("Starting total: " + startingTotal);
        System.out.println();

        Thread t1 = new Thread(
                () -> Transfer.transfer(accountA, accountB, 100),
                "Worker-1"
        );

        Thread t2 = new Thread(
                () -> Transfer.transfer(accountB, accountA, 200),
                "Worker-2"
        );

        t1.start();
        t2.start();

        t1.join();
        t2.join();

        int finalTotal = accountA.balance + accountB.balance;

        System.out.println("\nFinal Balances");
        System.out.println(accountA.name + ": " + accountA.balance);
        System.out.println(accountB.name + ": " + accountB.balance);

        System.out.println("\nFinal total: " + finalTotal);
        System.out.println("No deadlock occurred");
    }
}