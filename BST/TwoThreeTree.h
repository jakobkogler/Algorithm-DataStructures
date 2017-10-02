#include <array>


template <typename T>
class TwoThreeTreeNode
{
public:
    TwoThreeTreeNode(T elem) {
        elements[0] = elem;
        size = 1;
        children = { nullptr };
    }

    bool insert(T elem) {
        if (isLeaf()) {
            elements[size] = elem;
            for (int i = size - 1; i >= 0; i--) {
                if (elements[i] > elements[i+1])
                    std::swap(elements[i], elements[i+1]);
            }
            size++;
        } else {
            int idx = 0;
            for (int i = 0; i < size; i++) {
                if (elem >= elements[i]) {
                    idx++;
                }
            }
            bool three_node = children[idx]->insert(elem);
            if (!three_node) {
                auto child_left = children[idx];
                child_left->size = 1;
                auto child_right = new TwoThreeTreeNode<T>(child_left->elements[2]);
                child_right->children[0] = child_left->children[2];
                child_left->children[2] = nullptr;
                child_right->children[1] = child_left->children[3];
                child_left->children[3] = nullptr;
                T new_elem = child_left->elements[1];
                for (int i = size - 1; i >= idx; i--) {
                    elements[i + 1] = elements[i];
                    children[i + 2] = children[i + 1];
                }
                elements[idx] = new_elem;
                children[idx + 1] = child_right;
                size++;
            }
        }

        return size < 3;
    }

    bool contains(T elem) {
        if (isLeaf()) {
            for (int i = 0; i < size; i++) {
                if (elem == elements[i])
                    return true;
            }
            return false;
        } else {
            for (int i = size - 1; i >= 0; i--) {
                if (elem > elements[i])
                    return children[i + 1]->contains(elem);
                if (elem == elements[i])
                    return true;
            }
            return children[0]->contains(elem);
        }
    }


    bool isLeaf() {
        return children[0] == nullptr;
    }

    std::array<T, 3> elements;
    int size;
    std::array<TwoThreeTreeNode<T>*, 4> children; 
};


template <typename T>
class TwoThreeTree
{
public:
    void insert(T elem) {
        if (root) {
            bool three_node = root->insert(elem);
            if (!three_node) {
                auto child_left = root;
                child_left->size = 1;
                auto child_right = new TwoThreeTreeNode<T>(child_left->elements[2]);
                child_right->children[0] = child_left->children[2];
                child_left->children[2] = nullptr;
                child_right->children[1] = child_left->children[3];
                child_left->children[3] = nullptr;
                T new_elem = child_left->elements[1];
                root = new TwoThreeTreeNode<T>(new_elem);
                root->children[0] = child_left;
                root->children[1] = child_right;
                height++;
            }
        } else {
            root = new TwoThreeTreeNode<T>(elem);
            height = 1;
        }
    }

    bool contains(T elem) {
        if (root)
            return root->contains(elem);
        else
            return false;
    }

private:
    TwoThreeTreeNode<T>* root = nullptr;
    int height = 0;
};
