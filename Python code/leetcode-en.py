# House Robber (LeetCode 198)
#You cannot rob two adjacent houses. Find the maximum total loot and
# which houses to rob.
#11-11-2026
# Run:  python house_robber_en.py
# Your own street: python house_robber_en.py "5,8,6,7"

import sys

# Money in each house, separated by commas
STREET = "6,7,3,6,9,67,61,69,8,9,3,8,2,8,6,7,5,67,7,6,4,67,2,2,8,9,0,67,0,0,3,5,7,36,67,36"


def parse_houses(text):
    """Turn "9,4,4,..." into a list of integers. Return None if the input is invalid."""
    if text.strip() == "":   # empty or only spaces
        return None
    try:
        return [int(part) for part in text.split(",")]
    except ValueError:       # a non-number, or a stray comma such as "6,7,"
        return None


def max_loot(houses):
    """Maximum total loot only. O(n) time, O(1) extra memory."""
    best_two_back = 0   # best loot using the houses up to two steps behind
    best_one_back = 0   # best loot using the houses up to the previous house
    for money in houses:
        best_two_back, best_one_back = best_one_back, max(best_one_back, best_two_back + money)
    return best_one_back


def choose_houses(houses):
    """Return the indexes (0-based) of the houses to rob. O(n) time, O(n) memory.

    best[idx + 2] = maximum loot using only the houses 0..idx
    (house idx itself may or may not be robbed).
    best[0] and best[1] are 0 ("no houses yet"), so the first two houses
    need no special case.
    """
    n = len(houses)
    best = [0] * (n + 2)
    for idx, money in enumerate(houses):
        # skip house idx: best[idx + 1]
        # rob house idx:  money + best[idx]  (house idx - 1 is not allowed)
        best[idx + 2] = max(best[idx + 1], best[idx] + money)

    # Walk backwards from the last house to find which houses were robbed.
    chosen = []
    pos = n - 1
    while pos >= 0:
        if best[pos + 2] == best[pos + 1]:
            pos -= 1             # robbing house pos adds nothing: skip it
        else:
            chosen.append(pos)   # house pos is robbed
            pos -= 2             # house pos - 1 is adjacent, so jump over it
    chosen.reverse()             # we collected them from the end
    return chosen


def main():
    street = sys.argv[1] if len(sys.argv) > 1 else STREET
    houses = parse_houses(street)
    if houses is None:
        print("Cannot rob this street: the input is empty or not a valid list of numbers.")
        return
    total = max_loot(houses)
    chosen = choose_houses(houses)
    assert sum(houses[idx] for idx in chosen) == total   # cross-check the two methods
    print("Number of houses:", len(houses))
    print("Maximum loot:", total)
    print("Houses to rob (0-based index):", chosen)
    print("Matching values:", [houses[idx] for idx in chosen])
