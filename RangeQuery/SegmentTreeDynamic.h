#include <vector>

struct Node
{
    int data = 0;
    Node* L = nullptr;
    Node* R = nullptr;
    int todo = 0;
};

class SegmentTreeRec
{
public:
    SegmentTreeRec(int n) : n(n) {
        nodes.reserve(15000010);
        auto start = createNode();
        start->data = n;
        start->todo = 2;
    }

    Node* createNode() {
        nodes.emplace_back();
        return &nodes[nodeCnt++];
    }

    void update(int l, int r, int val, Node* cur=nullptr, int seg_l=0, int seg_r=-1) {
        if (seg_r == -1)
            seg_r = n;

        if (seg_r <= l || seg_l >= r)
            return;

        if (l <= seg_l && r >= seg_r) {
            cur->data = val;
            cur->todo = val;
            return;
        }

        if (cur->L == nullptr) 
            cur->L = createNode();
        if (cur->R == nullptr) 
            cur->R = createNode();
        
        int seg_m = seg_l + (seg_r - seg_l) / 2;
        if (cur->todo == 1) {
            cur->L->data = 0;
            cur->L->todo = 1;
            cur->R->data = 0;
            cur->R->todo = 1;
            cur->todo = 0;
        }
        if (cur->todo == 2) {
            cur->L->data = seg_m - seg_l;
            cur->L->todo = 2;
            cur->R->data = seg_r - seg_m;
            cur->R->todo = 2;
            cur->todo = 0;
        }

        update(l, r, val, cur->L, seg_l, seg_m);
        update(l, r, val, cur->R, seg_m, seg_r);
        cur->data = cur->L->data + cur->R->data;
    }

    int n;
    std::vector<Node> nodes;
    Node* start;
    int nodeCnt = 0;
};
