#include <map>

template <typename T, typename S>
class MonotoneStorage {
public:
    void insert(T x, S y) {
        auto it = storage.upper_bound(x);
        if (it != storage.begin()) {
            auto prev = it;
            --prev;
            if (prev->second >= y)
                return;
            if (it->first == x)
                it = prev;
        }

        auto ti = it;
        while (ti != storage.end() && y >= ti->second)
            ++ti;
        
        storage.erase(it, ti);
        storage[x] = y;
    }

    S max_upto(T x) {
        auto it = storage.upper_bound(x);
        if (it == storage.begin())
            return 0;
        --it;
        return it->second;
    }

private:
    std::map<T, S> storage;
};
