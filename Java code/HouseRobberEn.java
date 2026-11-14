// House Robber (LeetCode 198)
// You cannot rob two adjacent houses. Find the maximum total loot and
// which houses to rob.
//
// Build:           javac HouseRobberEn.java
// Run:             java HouseRobberEn
// Your own street: java HouseRobberEn "5,8,2,9"

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class HouseRobberEn {

    // Money in each house, separated by commas
    static final String STREET =
        "6,7,3,6,9,0,0,4,8,9,3,8,2,8,6,7,5,36,7,6,4,3,2,2,8,9,0,36,0,0,3,5,7,36,67,36";

    // Turn "9,4,4,..." into a list of numbers. Return null if the input is invalid.
    static List<Long> parseHouses(String text) {
        if (text.trim().isEmpty()) {   // empty or only spaces
            return null;
        }
        List<Long> houses = new ArrayList<>();
        // The -1 keeps an empty last part, so "6,7," is rejected instead of silently ignored.
        for (String part : text.split(",", -1)) {
            try {
                houses.add(Long.parseLong(part.trim()));
            } catch (NumberFormatException e) {   // not a number, empty part, or too large
                return null;
            }
        }
        return houses;
    }

    // Maximum total loot only. O(n) time, O(1) extra memory.
    static long maxLoot(List<Long> houses) {
        long bestTwoBack = 0;   // best loot using the houses up to two steps behind
        long bestOneBack = 0;   // best loot using the houses up to the previous house
        for (long money : houses) {
            // Java has no tuple assignment like Python, so keep the new value in a variable first.
            long bestNow = Math.max(bestOneBack, bestTwoBack + money);
            bestTwoBack = bestOneBack;
            bestOneBack = bestNow;
        }
        return bestOneBack;
    }

    // Return the indexes (0-based) of the houses to rob. O(n) time, O(n) memory.
    //
    // best[idx + 2] = maximum loot using only the houses 0..idx
    // (house idx itself may or may not be robbed).
    // best[0] and best[1] are 0 ("no houses yet"), so the first two houses
    // need no special case.
    static List<Integer> chooseHouses(List<Long> houses) {
        int n = houses.size();
        long[] best = new long[n + 2];   // starts as all zeros
        for (int idx = 0; idx < n; idx++) {
            // skip house idx: best[idx + 1]
            // rob house idx:  money + best[idx]  (house idx - 1 is not allowed)
            best[idx + 2] = Math.max(best[idx + 1], best[idx] + houses.get(idx));
        }

        // Walk backwards from the last house to find which houses were robbed.
        List<Integer> chosen = new ArrayList<>();
        int pos = n - 1;
        while (pos >= 0) {
            if (best[pos + 2] == best[pos + 1]) {
                pos -= 1;                // robbing house pos adds nothing: skip it
            } else {
                chosen.add(pos);         // house pos is robbed
                pos -= 2;                // house pos - 1 is adjacent, so jump over it
            }
        }
        Collections.reverse(chosen);     // we collected them from the end
        return chosen;
    }

    public static void main(String[] args) {
        String street = (args.length > 0) ? args[0] : STREET;
        List<Long> houses = parseHouses(street);
        if (houses == null) {
            System.out.println("Cannot rob this street: the input is empty or not a valid list of numbers.");
            return;
        }

        long total = maxLoot(houses);
        List<Integer> chosen = chooseHouses(houses);

        List<Long> values = new ArrayList<>();
        long sum = 0;
        for (int idx : chosen) {
            values.add(houses.get(idx));
            sum += houses.get(idx);
        }
        // Cross-check the two methods. (A plain "assert" is off by default in Java.)
        if (sum != total) {
            throw new IllegalStateException("Internal check failed: the two methods disagree.");
        }

        System.out.println("Number of houses: " + houses.size());
        System.out.println("Maximum loot: " + total);
        System.out.println("Houses to rob (0-based index): " + chosen);
        System.out.println("Matching values: " + values);
    }
}