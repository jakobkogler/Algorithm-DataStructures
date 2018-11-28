#include <vector>

class SegmentTree {
public:
    SegmentTree(int count) {
        n = count;
        data.assign(2 * n, 0);
    }

    int maximum(int left, int right) {  // interval [left, right)
        int ret = 0;
        left += n;
        right += n;

        while (left < right) {
            if (left & 1)
                ret = std::max(ret, data[left++]);
            if (right & 1)
                ret = std::max(ret, data[--right]);
            left >>= 1;
            right >>= 1;
        }
        return ret;
    }

    void update(int idx, int val) {
        idx += n;
        data[idx] = val;

        while (idx > 1) {
            idx /= 2;
            data[idx] = std::max(data[idx * 2], data[idx * 2 + 1]);
        }
    }

private:
    int n;
    std::vector<int> data;
};

class HLD {
public:
    HLD(std::vector<std::vector<int>> const& adj, int root = 0)
        : adj(adj), n(adj.size()), segtree(n) {
        parent = std::vector<int>(n);
        depth = std::vector<int>(n);
        heavy = std::vector<int>(n, -1);
        head = std::vector<int>(n);
        pos = std::vector<int>(n);
        cur_pos = 0;

        dfs(root);
        decompose(root, root);

        segtree = SegmentTree(n);
    }

    int dfs(int v, int p = -1, int d = 0) {
        parent[v] = p;
        depth[v] = d;
        int size = 1;
        int max_child_size = 0;
        for (int u : adj[v]) {
            if (u != parent[v]) {
                int u_size = dfs(u, v, d + 1);
                size += u_size;
                if (u_size > max_child_size) {
                    max_child_size = u_size;
                    heavy[v] = u;
                }
            }
        }
        return size;
    }

    void decompose(int v, int h) {
        head[v] = h;
        pos[v] = cur_pos++;
        if (heavy[v] != -1)
            decompose(heavy[v], h);
        for (int u : adj[v]) {
            if (u != parent[v] && u != heavy[v])
                decompose(u, u);
        }
    }

    int query_max(int a, int b) {
        int res = 0;
        for (; head[a] != head[b]; b = parent[head[b]]) {
            if (depth[head[a]] > depth[head[b]])
                std::swap(a, b);
            int cur_heavy_path = segtree.maximum(pos[head[b]], pos[b] + 1);
            res = std::max(res, cur_heavy_path);
        }

        if (depth[a] > depth[b])
            std::swap(a, b);
        int last_heavy_path = segtree.maximum(pos[a] + 1, pos[b] + 1);
        res = std::max(res, last_heavy_path);
        return res;
    }

private:
    std::vector<std::vector<int>> const& adj;
    int n;

public:
    SegmentTree segtree;
    int cur_pos;
    std::vector<int> parent, depth, heavy, head, pos;
};
