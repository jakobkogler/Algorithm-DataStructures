
template <typename T>
class RedBlackTreeNode
{
public:
    RedBlackTreeNode(T elem)
        : elem(elem), left(nullptr), right(nullptr), color(Color::red)
    {
    }

    static RedBlackTreeNode<T> *rotateLeft(RedBlackTreeNode<T> *h)
    {
        //    h                x
        //   / \\             / \
        //  a   x    =>      h   c
        //     / \          / \
        //    b   c        a   b
        RedBlackTreeNode<T> *x = h->right;
        h->right = x->left;
        x->left = h;
        x->color = h->color;
        h->color = Color::red;
        return x;
    }

    static RedBlackTreeNode<T> *rotateRight(RedBlackTreeNode<T> *h)
    {
        RedBlackTreeNode<T> *x = h->left;
        h->left = x->right;
        x->right = h;
        x->color = h->color;
        h->color = Color::red;
        return x;
    }

    static void flipColors(RedBlackTreeNode<T> *h)
    {
        h->color = Color::red;
        h->left->color = Color::black;
        h->right->color = Color::black;
    }

    static bool isRed(RedBlackTreeNode<T> *node)
    {
        if (node == nullptr)
            return false;
        return node->color == Color::red;
    }

    static RedBlackTreeNode<T> *insert(RedBlackTreeNode<T> *node, T elem)
    {
        if (node == nullptr) {
            return new RedBlackTreeNode<T>(elem);
        }
        if (elem < node->elem)
            node->left = insert(node->left, elem);
        else
            node->right = insert(node->right, elem);

        if (!isRed(node->left) && isRed(node->right))
            node = rotateLeft(node);

        if (isRed(node->left) && isRed(node->right))
            flipColors(node);

        return node;
    }

    static bool contains(RedBlackTreeNode *node, T elem)
    {
        if (node == nullptr)
            return false;

        if (elem < node->elem)
            return contains(node->left, elem);
        else if (elem == node->elem)
            return true;
        else
            return contains(node->right, elem);
    }

private:
    enum Color { black, red };

    T elem;
    RedBlackTreeNode *left, *right;
    Color color;
};

template <typename T>
class RedBlackTree
{
public:
    RedBlackTree() : root(nullptr) {}

    void insert(T elem)
    {
        if (root == nullptr) {
            root = new RedBlackTreeNode<T>(elem);
        } else {
            root = RedBlackTreeNode<T>::insert(root, elem);
        }
    }

    bool contains(T elem)
    {
        if (root == nullptr) {
            return false;
        } else {
            return RedBlackTreeNode<T>::contains(root, elem);
        }
    }

private:
    RedBlackTreeNode<T> *root;
};
