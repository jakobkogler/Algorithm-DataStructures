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

enum ValueType {NodeValues=0, EdgeValues=1};

/**
 * Heavy-Light decomposition
 *
 * Either every edge has a value, or every node has a value.
 * In the node version, the value of the node `v` is simply stored at `pos[v]`.
 * In the edge version, the value of the edge `u-v` is stored at `pos[v]`,
 * where `v` is the child of `u`. Be careful to not include the edge from the LCA to it's parent.
 *
 * `head[v]` will be the start of the current path (so the highest ancestor of `v`).
 */
template <ValueType VT = EdgeValues>
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

    template<ValueType V1 = VT, typename std::enable_if<V1 == NodeValues, int>::type = 0>
    void update_node(int v, int val) {
        segtree.update(pos[v], val);
    }

    template<ValueType V1 = VT, typename std::enable_if<V1 == EdgeValues, int>::type = 0>
    void update_edge(int u, int v, int val) {
        if (depth[u] > depth[v])
            std::swap(u, v);
        segtree.update(pos[v], val);
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
        int last_heavy_path = segtree.maximum(pos[a] + VT, pos[b] + 1);
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
