int dfs(int v, int p=-1) {
    int size = 1;
    for (auto u : adj[v]) {
        if (u == p)
            continue;
        size += dfs(u, v);
    }
    return size;
}
