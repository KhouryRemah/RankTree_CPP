#include <iostream>
#include "RankTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class CompareInt {
public:

    int operator()(const int& a, const int& b) {
        return a - b;
    }
};

class RankEditor {
public:
    int operator()(int* data) {
        return *data;
    }
};


void testRankTree() {

    std::cout << "Hello Remah Khoury!" << std::endl;
    RankTree<CompareInt, int, RankEditor>* tree = new RankTree<CompareInt, int, RankEditor>();

    tree->insert(5);
    printBT(tree->getRoot());
    std::cout << "max: " << *tree->getMaxData() << std::endl;

    tree->insert(10);
    printBT(tree->getRoot());
    std::cout << "max: " << *tree->getMaxData() << std::endl;

    tree->insert(40);
    printBT(tree->getRoot());
    std::cout << "max: " << *tree->getMaxData() << std::endl;

    tree->insert(15);
    printBT(tree->getRoot());
    std::cout << "max: " << *tree->getMaxData() << std::endl;

    tree->insert(7);
    printBT(tree->getRoot());
    std::cout << "max: " << *tree->getMaxData() << std::endl;

    tree->insert(22);
    printBT(tree->getRoot());
    std::cout << "max: " << *tree->getMaxData() << std::endl;

    tree->insert(3);
    printBT(tree->getRoot());
    std::cout << "max: " << *tree->getMaxData() << std::endl;

    tree->insert(7);
    printBT(tree->getRoot());
    std::cout << "max: " << *tree->getMaxData() << std::endl;

    tree->insert(16);
    printBT(tree->getRoot());
    std::cout << "max: " << *tree->getMaxData() << std::endl;

    int size = tree->getRoot()->getSizeOfSubTree();
    int** arr = new int* [size];
    tree->getInorderData(arr);
    for (int i = 0; i < size; i++)std::cout << " " << *arr[i];
    std::cout << std::endl;
    delete[] arr;

    std::cout << "select " << tree->select(2)->getData() << std::endl;
    std::cout << "select " << tree->select(5)->getData() << std::endl;
    std::cout << "select " << tree->select(1)->getData() << std::endl;
    std::cout << "select " << tree->select(8)->getData() << std::endl;
    std::cout << "select " << tree->select(7)->getData() << std::endl;


    RTNode<int>* node;
    node = tree->find(5);
    if (node != NULL) std::cout << "search " << node->getData() << std::endl;

    node = tree->find(6);
    if (node != NULL) std::cout << "search " << node->getData() << std::endl;

    node = tree->find(40);
    if (node != NULL) std::cout << "search " << node->getData() << std::endl;

    tree->remove(5);
    printBT(tree->getRoot());
    node = tree->find(5);
    if (node != NULL) std::cout << "search " << node->getData() << std::endl;

    node = tree->find(10);
    if (node != NULL) std::cout << "search " << node->getData() << std::endl;

    tree->remove(7);
    printBT(tree->getRoot());
    node = tree->find(7);
    if (node != NULL) std::cout << "search " << node->getData() << std::endl;

    tree->remove(15);
    printBT(tree->getRoot());
    node = tree->find(15);
    if (node != NULL) std::cout << "search " << node->getData() << std::endl;

    tree->remove(22);
    printBT(tree->getRoot());
    node = tree->find(22);
    if (node != NULL) std::cout << "search " << node->getData() << std::endl;

    size = tree->getRoot()->getSizeOfSubTree();
    arr = new int* [size];
    tree->getInorderData(arr);
    for (int i = 0; i < size; i++)std::cout << " " << *arr[i];
    std::cout << std::endl;
    delete[] arr;


    RankTree<CompareInt, int, RankEditor>* tree2 = new RankTree<CompareInt, int, RankEditor>();
    tree2->insert(5);
    tree2->insert(50);
    tree2->insert(4);
    tree2->insert(2);
    tree2->insert(23);
    printBT(tree2->getRoot());

    RankTree<CompareInt, int, RankEditor>* tree3 = tree2->mergeTrees(tree);
    printBT(tree3->getRoot());

    delete tree;
    delete tree2;
    delete tree3;
}

int main(int argc, const char** argv) {
    testRankTree();
    
    return 0;
}
