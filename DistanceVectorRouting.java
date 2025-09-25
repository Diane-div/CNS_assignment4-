import java.util.Scanner;

public class DistanceVectorRouting {

    static final int INF = 9999;

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        System.out.print("Enter number of routers: ");
        int n = sc.nextInt();

        int[][] cost = new int[n][n];
        int[][][] table = new int[n][n][2]; // [router][destination][0=cost,1=nextHop]

        System.out.println("Enter cost matrix (use 9999 for infinity):");
        for (int i = 0; i < n; i++) {
            System.out.print("Row " + i + ": ");
            for (int j = 0; j < n; j++) {
                cost[i][j] = sc.nextInt();
                table[i][j][0] = cost[i][j];
                if (cost[i][j] != INF && i != j)
                    table[i][j][1] = j;
                else
                    table[i][j][1] = -1;
            }
        }

        boolean changed;
        int iteration = 0;

        do {
            changed = false;
            System.out.println("\n=== Iteration " + (++iteration) + " ===");

            for (int i = 0; i < n; i++) { // for each router i
                for (int j = 0; j < n; j++) { // for each destination j
                    for (int k = 0; k < n; k++) { // for each neighbor k
                        if (cost[i][k] != INF && table[k][j][0] != INF) {
                            int newCost = cost[i][k] + table[k][j][0];
                            if (newCost < table[i][j][0]) {
                                table[i][j][0] = newCost;
                                table[i][j][1] = k;
                                changed = true;
                            }
                        }
                    }
                }
            }

        } while (changed);

        System.out.println("\n=== Final Routing Tables ===");
        for (int i = 0; i < n; i++) {
            System.out.println("\nRouter " + i + " routing table:");
            System.out.println("Destination\tCost\tNext Hop");
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    String nextHop = (table[i][j][1] == -1) ? "-" : String.valueOf(table[i][j][1]);
                    String costVal = (table[i][j][0] == INF) ? "INF" : String.valueOf(table[i][j][0]);
                    System.out.println(j + "\t\t" + costVal + "\t" + nextHop);
                }
            }
        }

        sc.close();
    }
}
