#include <iostream>
#include "RedBlackTree.h"

using namespace std;

int main() {
    RedBlackTree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(2);
    tree.insert(8);
    tree.insert(10);
    tree.insert(9);
    cout << tree.contains(1) << endl;
    cout << tree.contains(10) << endl;
    cout << tree.contains(9) << endl;
    cout << tree.contains(4) << endl;
}
