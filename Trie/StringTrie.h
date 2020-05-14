#include <vector>
#include <array>

struct Vertex {
    std::array<int, 26> next{};
    bool terminal = false;
};

class Trie {
public:
    Trie() : t(1) {}

    void add_string(std::string const& s) {
        int v = 0;
        for (char ch : s) {
            int c = ch - 'a';
            if (!t[v].next[c]) {
                t[v].next[c] = t.size();
                t.emplace_back();
            }
            v = t[v].next[c];
        }
        t[v].terminal = true;
    }

private:
    std::vector<Vertex> t;
};
