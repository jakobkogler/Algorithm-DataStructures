#include <iostream>
#include "TwoThreeTree.h"


int main() {
    TwoThreeTree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(2);
    tree.insert(4);
    tree.insert(6);
    std::cout << tree.contains(6) << std::endl;
    std::cout << tree.contains(1) << std::endl;
}
