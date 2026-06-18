import java.util.*;

public class PageReplacement {

    static int[] refs = {5, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3};

    public static void main(String[] args) {

        fifo();
        System.out.println();
        lru();
    }

    static void fifo() {

        System.out.println("=== FIFO ===");

        Queue<Integer> queue = new LinkedList<>();
        List<Integer> frames = new ArrayList<>();

        int faults = 0;

        for (int page : refs) {

            if (frames.contains(page)) {

                System.out.println(
                    "Ref " + page +
                    " | HIT | frames: " + frames
                );

            } else {

                faults++;

                if (frames.size() < 3) {

                    frames.add(page);
                    queue.add(page);

                } else {

                    int victim = queue.poll();

                    frames.remove(Integer.valueOf(victim));

                    frames.add(page);

                    queue.add(page);

                    System.out.println(
                        "Ref " + page +
                        " | FAULT | evicted " +
                        victim +
                        " | frames: " + frames
                    );

                    continue;
                }

                System.out.println(
                    "Ref " + page +
                    " | FAULT | frames: " + frames
                );
            }
        }

        System.out.println("Total page faults (FIFO): " + faults);
    }

    static void lru() {

        System.out.println("=== LRU ===");

        List<Integer> frames = new ArrayList<>();

        Map<Integer, Integer> recent = new HashMap<>();

        int faults = 0;

        for (int i = 0; i < refs.length; i++) {

            int page = refs[i];

            if (frames.contains(page)) {

                recent.put(page, i);

                System.out.println(
                    "Ref " + page +
                    " | HIT | frames: " + frames
                );

            } else {

                faults++;

                if (frames.size() < 3) {

                    frames.add(page);

                } else {

                    int victim = frames.get(0);

                    for (int p : frames) {

                        if (recent.get(p) < recent.get(victim)) {
                            victim = p;
                        }
                    }

                    frames.remove(Integer.valueOf(victim));
                    frames.add(page);

                    System.out.println(
                        "Ref " + page +
                        " | FAULT | evicted " +
                        victim +
                        " | frames: " + frames
                    );

                    recent.put(page, i);

                    continue;
                }

                recent.put(page, i);

                System.out.println(
                    "Ref " + page +
                    " | FAULT | frames: " + frames
                );
            }
        }

        System.out.println("Total page faults (LRU): " + faults);
    }
}