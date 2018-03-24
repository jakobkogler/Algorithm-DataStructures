#include <vector>
#include <string>

class PrefixAutomaton {
public:
    PrefixAutomaton(std::string str) : s(str) {
        s += '#';
        int n = s.size();
        pi = prefix_function(s);
        aut.assign(n, std::vector<int>(26));
        for (int i = 0; i < n; i++) {
            for (int c = 0; c < 26; c++) {
                if (i > 0 && 'a' + c != s[i])
                    aut[i][c] = aut[pi[i-1]][c];
                else
                    aut[i][c] = i + ('a' + c == s[i]);
            }
        }
    }

    int transition(int state, char c) {
        return aut[state][c - 'a'];
    }

    static std::vector<int> prefix_function(std::string s) {
        int n = (int)s.length();
        std::vector<int> pi(n);
        for (int i = 1; i < n; i++) {
            int j = pi[i-1];
            while (j > 0 && s[i] != s[j])
                j = pi[j-1];
            if (s[i] == s[j])
                j++;
            pi[i] = j;
        }
        return pi;
    }

private:
    std::string s;
    std::vector<int> pi;
    std::vector<std::vector<int>> aut;
};
