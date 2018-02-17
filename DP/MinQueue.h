#include <deque>

class MinQueue {
public:
    MinQueue(int capacity = 1'000'000'000) : capacity(capacity) {}

    int min() {
        return dq.front().first;
    }

    void add(int elem) {
        while (!dq.empty() && dq.back().first > elem) {
            dq.pop_back();
        }
        dq.push_back({elem, added});
        added++;
        if (added - removed > capacity)
            remove();
    }

    void remove() {
        if (!dq.empty() && dq.front().second == removed)
            dq.pop_front();
        removed++;
    }

private:
    std::deque<std::pair<int, int>> dq;
    int added = 0;
    int removed = 0;
    int capacity;
};
