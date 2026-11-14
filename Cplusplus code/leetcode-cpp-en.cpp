// House Robber (LeetCode 198)
// You cannot rob two adjacent houses. Find the maximum total loot and
// which houses to rob.
//
// Build:           g++ -std=c++17 -O2 house_robber_en.cpp -o house_robber_en
// Run:             ./house_robber_en
// Your own street: ./house_robber_en "5,8,6,7"

#include <algorithm>
#include <cstddef>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

// Money in each house, separated by commas
const std::string STREET =
    "6,7,3,6,9,67,61,69,8,9,3,8,2,8,6,7,5,67,7,6,4,67,2,2,8,9,0,67,0,0,3,5,7,36,67,36";

// Split text at every separator. "6,7," gives {"6", "7", ""}: the empty last part is kept.
std::vector<std::string> split(const std::string& text, char separator) {
    std::vector<std::string> parts;
    std::size_t start = 0;
    while (true) {
        std::size_t end = text.find(separator, start);
        if (end == std::string::npos) {
            parts.push_back(text.substr(start));
            break;
        }
        parts.push_back(text.substr(start, end - start));
        start = end + 1;
    }
    return parts;
}

// Remove whitespace at both ends.
std::string trim(const std::string& text) {
    const char* whitespace = " \t\r\n";
    std::size_t first = text.find_first_not_of(whitespace);
    if (first == std::string::npos) return "";
    std::size_t last = text.find_last_not_of(whitespace);
    return text.substr(first, last - first + 1);
}

// Turn "9,4,4,..." into numbers. Return false if the input is invalid.
bool parse_houses(const std::string& text, std::vector<long long>& houses) {
    houses.clear();
    if (trim(text).empty()) return false;   // empty or only spaces
    for (const std::string& raw : split(text, ',')) {
        std::string part = trim(raw);
        if (part.empty()) return false;     // stray comma such as "6,7,"
        try {
            std::size_t used = 0;
            long long value = std::stoll(part, &used);
            if (used != part.size()) return false;   // e.g. "12abc"
            houses.push_back(value);
        } catch (const std::exception&) {   // not a number, or too large
            return false;
        }
    }
    return true;
}

// Maximum total loot only. O(n) time, O(1) extra memory.
long long max_loot(const std::vector<long long>& houses) {
    long long best_two_back = 0;   // best loot using the houses up to two steps behind
    long long best_one_back = 0;   // best loot using the houses up to the previous house
    for (long long money : houses) {
        // C++ has no tuple assignment like Python, so keep the new value in a variable first.
        long long best_now = std::max(best_one_back, best_two_back + money);
        best_two_back = best_one_back;
        best_one_back = best_now;
    }
    return best_one_back;
}

// Return the indexes (0-based) of the houses to rob. O(n) time, O(n) memory.
//
// best[idx + 2] = maximum loot using only the houses 0..idx
// (house idx itself may or may not be robbed).
// best[0] and best[1] are 0 ("no houses yet"), so the first two houses
// need no special case.
std::vector<int> choose_houses(const std::vector<long long>& houses) {
    const int n = static_cast<int>(houses.size());
    std::vector<long long> best(n + 2, 0);
    for (int idx = 0; idx < n; ++idx) {
        // skip house idx: best[idx + 1]
        // rob house idx:  money + best[idx]  (house idx - 1 is not allowed)
        best[idx + 2] = std::max(best[idx + 1], best[idx] + houses[idx]);
    }

    // Walk backwards from the last house to find which houses were robbed.
    std::vector<int> chosen;
    int pos = n - 1;
    while (pos >= 0) {
        if (best[pos + 2] == best[pos + 1]) {
            pos -= 1;                  // robbing house pos adds nothing: skip it
        } else {
            chosen.push_back(pos);     // house pos is robbed
            pos -= 2;                  // house pos - 1 is adjacent, so jump over it
        }
    }
    std::reverse(chosen.begin(), chosen.end());   // we collected them from the end
    return chosen;
}

// Print a label followed by a list such as [0, 2, 4].
template <typename T>
void print_list(const std::string& label, const std::vector<T>& values) {
    std::cout << label << "[";
    for (std::size_t k = 0; k < values.size(); ++k) {
        if (k > 0) std::cout << ", ";
        std::cout << values[k];
    }
    std::cout << "]\n";
}

int main(int argc, char* argv[]) {
    const std::string street = (argc > 1) ? std::string(argv[1]) : STREET;
    std::vector<long long> houses;
    if (!parse_houses(street, houses)) {
        std::cout << "Cannot rob this street: the input is empty or not a valid list of numbers.\n";
        return 0;
    }

    const long long total = max_loot(houses);
    const std::vector<int> chosen = choose_houses(houses);

    std::vector<long long> values;
    long long sum = 0;
    for (int idx : chosen) {
        values.push_back(houses[idx]);
        sum += houses[idx];
    }
    if (sum != total) {   // cross-check the two methods
        std::cerr << "Internal check failed: the two methods disagree.\n";
        return 1;
    }

    std::cout << "Number of houses: " << houses.size() << "\n";
    std::cout << "Maximum loot: " << total << "\n";
    print_list("Houses to rob (0-based index): ", chosen);
    print_list("Matching values: ", values);
    return 0;
}