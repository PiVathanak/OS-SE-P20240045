public class PagingTranslation {

    public static void main(String[] args) {

        int pageSize = 16;

        int[] pageTable = {5, 2, 1, -1, 7, -1, 0, 4};

        int[] addresses = {20, 100, 48, 16, 127, 54};

        for (int la : addresses) {

            int page = la / pageSize;
            int offset = la % pageSize;

            if (pageTable[page] == -1) {
                System.out.println(
                    "Logical " + la +
                    " -> page " + page +
                    " -> Page fault: page not in memory"
                );
            } else {

                int frame = pageTable[page];
                int physical = frame * pageSize + offset;

                System.out.println(
                    "Logical " + la +
                    " -> page " + page +
                    ", offset " + offset +
                    " -> frame " + frame +
                    " -> physical " + physical
                );
            }
        }
    }
}