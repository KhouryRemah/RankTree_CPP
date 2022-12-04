
#ifndef RANKTREE_H
#define RANKTREE_H

#include "RTNode.h"
#include <cstdlib>
#include <string>
#include <iostream>

template<class Compare, class Data, class REdit>
class RankTree {
    RTNode<Data>* root;

    void clearTree(RTNode<Data>* root);
    RTNode<Data>* aux_insert(Data data, RTNode<Data>* root);
    void updateheight(RTNode<Data>* node);
    int getSubMaxHeight(RTNode<Data>* node);
    void updateSubSize(RTNode<Data>* node);
    void updateNodesSum(RTNode<Data>* node);
    void makeRolls(RTNode<Data>* node);
    int getBF(const RTNode<Data>* node);
    void do_LL(RTNode<Data>* node);
    void do_RR(RTNode<Data>* node);
    RTNode<Data>* aux_find(RTNode<Data>* node, Data data);
    void aux_getInorderData(RTNode<Data>* node, int* index, Data** arr)const;
    RTNode<Data>* aux_remove(Data data, RTNode<Data>* root);
    static void mergeArrays(Data** arr1, int size1, Data** arr2, int size2, Data** mergedArray);
    RankTree<Compare, Data, REdit>* createEmptyTree(int size);
    void aux_createEmptyTree(int size, int index, RTNode<Data>* node);
    void fillTree(Data** array);
    void aux_fillTree(Data** array, int* index, RTNode<Data>* node);
    RTNode<Data>* aux_select(int k, RTNode<Data>* node);

public:
    RankTree();
    RankTree(Data data);
    ~RankTree();

    RTNode<Data>* insert(Data data);
    RTNode<Data>* find(Data data);
    void getInorderData(Data** arr)const;
    RTNode<Data>* remove(Data data);
    RankTree<Compare, Data, REdit>* mergeTrees(RankTree<Compare, Data, REdit>* tree1);

    RTNode<Data>* select(int k);
    Data* getMaxData();
    RTNode<Data>* getRoot() { return root; }


};



template<class Compare, class Data, class REdit>
RankTree<Compare, Data, REdit>::RankTree() :root(NULL) {
}

template<class Compare, class Data, class REdit>
RankTree<Compare, Data, REdit>::RankTree(Data data) : root(new RTNode<Data>(data)) {}

template<class Compare, class Data, class REdit>
RankTree<Compare, Data, REdit>::~RankTree() {
    clearTree(this->root);
}

template<class Compare, class Data, class REdit>
void RankTree<Compare, Data, REdit>::clearTree(RTNode<Data>* root) {
    if (root == NULL) {
        return;
    }
    clearTree(root->getRightSon());
    clearTree(root->getLeftSon());
    delete root;
}

template<class Compare, class Data, class REdit>
RTNode<Data>* RankTree<Compare, Data, REdit>::insert(Data data) {
    if (root == NULL) {
        root = new RTNode<Data>(data);
        root->setSum(REdit()(&data));
        return root;
    }
    RTNode<Data>* newNode = aux_insert(data, root);
    if (newNode == NULL) {
        return NULL;
    }
    makeRolls(newNode);
    return newNode;
}

template<class Compare, class Data, class REdit>
RTNode<Data>* RankTree<Compare, Data, REdit>::aux_insert(Data data, RTNode<Data>* root) {
    if (Compare()(data, root->getData()) < 0) {
        if (root->getLeftSon() == NULL) {
            RTNode<Data>* newNode = new RTNode<Data>(data);
            newNode->setParent(root);
            root->setLeftSon(newNode);
            return newNode;
        }
        return aux_insert(data, root->getLeftSon());
    }
    else if (Compare()(data, root->getData()) > 0) {
        if (root->getRightSon() == NULL) {
            RTNode<Data>* newNode = new RTNode<Data>(data);
            newNode->setParent(root);
            root->setRightSon(newNode);
            return newNode;
        }
        return aux_insert(data, root->getRightSon());
    }
    else {
        return NULL;
    }
}

template<class Compare, class Data, class REdit>
void RankTree<Compare, Data, REdit>::updateheight(RTNode<Data>* node) {
    node->setHeight(1 + getSubMaxHeight(node));
}

template<class Compare, class Data, class REdit>
int RankTree<Compare, Data, REdit>::getSubMaxHeight(RTNode<Data>* node) {
    RTNode<Data>* left = node->getLeftSon();
    RTNode<Data>* right = node->getRightSon();
    if (left == NULL && right == NULL)return 0;
    if (left == NULL)return right->getHeight();
    if (right == NULL)return left->getHeight();
    int h1 = right->getHeight();
    int h2 = left->getHeight();
    return (h1 > h2 ? h1 : h2);
}

template<class Compare, class Data, class REdit>
void RankTree<Compare, Data, REdit>::updateSubSize(RTNode<Data>* node) {
    int lsize = (node->getLeftSon() == NULL ? 0 : node->getLeftSon()->getSizeOfSubTree());
    int rsize = (node->getRightSon() == NULL ? 0 : node->getRightSon()->getSizeOfSubTree());
    node->setSizeOfSubTree(lsize + rsize + 1);
}

template<class Compare, class Data, class REdit>
void RankTree<Compare, Data, REdit>::updateNodesSum(RTNode<Data>* node) {
    int lsum = (node->getLeftSon() == NULL ? 0 : node->getLeftSon()->getSum());
    int rsum = (node->getRightSon() == NULL ? 0 : node->getRightSon()->getSum());
    node->setSum(lsum + rsum + REdit()(node->getPointerToData()));
}

template<class Compare, class Data, class REdit>
void RankTree<Compare, Data, REdit>::makeRolls(RTNode<Data>* node) {
    if (node == NULL)return;
    updateheight(node);
    updateSubSize(node);
    updateNodesSum(node);
    if (getBF(node) == 2 && getBF(node->getLeftSon()) >= 0) {
        do_LL(node);
    }
    else if (getBF(node) == 2 && getBF(node->getLeftSon()) == -1) {
        do_RR(node->getLeftSon());
        do_LL(node);
    }
    else if ((getBF(node) == -2 && getBF(node->getRightSon()) == 1)) {
        do_LL(node->getRightSon());
        do_RR(node);
    }
    else if (getBF(node) == -2 && getBF(node->getRightSon()) <= 0) {
        do_RR(node);
    }
    makeRolls(node->getParent());
}

template<class Compare, class Data, class REdit>
int RankTree<Compare, Data, REdit>::getBF(const RTNode<Data>* node) {
    int lHeight = (node->getLeftSon() == NULL ? 0 : node->getLeftSon()->getHeight());
    int rHeight = (node->getRightSon() == NULL ? 0 : node->getRightSon()->getHeight());
    return lHeight - rHeight;
}

template<class Compare, class Data, class REdit>
void RankTree<Compare, Data, REdit>::do_LL(RTNode<Data>* node) {
    RTNode<Data>* B = node;
    RTNode<Data>* A = node->getLeftSon();
    RTNode<Data>* C = node->getParent();
    RTNode<Data>* Ar = A->getRightSon();

    A->setParent(C);
    if (C != NULL && C->getLeftSon() == B) {
        C->setLeftSon(A);
    }
    else if (C != NULL && C->getRightSon() == B) {
        C->setRightSon(A);
    }

    B->setParent(A);
    A->setRightSon(B);

    if (Ar != NULL) {
        Ar->setParent(B);
    }
    B->setLeftSon(Ar);

    updateheight(B);
    updateNodesSum(B);
    updateSubSize(B);
    updateheight(A);
    updateNodesSum(A);
    updateSubSize(A);

    if (C == NULL) {
        this->root = A;
    }
}

template<class Compare, class Data, class REdit>
void RankTree<Compare, Data, REdit>::do_RR(RTNode<Data>* node) {
    RTNode<Data>* B = node;
    RTNode<Data>* A = node->getRightSon();
    RTNode<Data>* C = node->getParent();
    RTNode<Data>* Al = A->getLeftSon();

    A->setParent(C);
    if (C != NULL && C->getLeftSon() == B) {
        C->setLeftSon(A);
    }
    else if (C != NULL && C->getRightSon() == B) {
        C->setRightSon(A);
    }

    B->setParent(A);
    A->setLeftSon(B);

    if (Al != NULL) {
        Al->setParent(B);
    }
    B->setRightSon(Al);

    updateheight(B);
    updateNodesSum(B);
    updateSubSize(B);
    updateheight(A);
    updateNodesSum(A);
    updateSubSize(A);

    if (C == NULL) {
        this->root = A;
    }
}

template<class Compare, class Data, class REdit>
RTNode<Data>* RankTree<Compare, Data, REdit>::find(Data data) {
    return aux_find(root, data);
}

template<class Compare, class Data, class REdit>
RTNode<Data>* RankTree<Compare, Data, REdit>::aux_find(RTNode<Data>* node, Data data) {
    if (node == NULL) return NULL;
    if (Compare()(node->getData(), data) > 0) return aux_find(node->getLeftSon(), data);
    if (Compare()(node->getData(), data) < 0) return aux_find(node->getRightSon(), data);
    return node;
}

template<class Compare, class Data, class REdit>
void RankTree<Compare, Data, REdit>::getInorderData(Data** arr)const {
    if (arr == NULL)return;
    int index = 0;
    aux_getInorderData(root, &index, arr);
}

template<class Compare, class Data, class REdit>
void RankTree<Compare, Data, REdit>::aux_getInorderData(RTNode<Data>* node, int* index, Data** arr)const {
    if (arr == NULL || node == NULL)return;
    aux_getInorderData(node->getLeftSon(), index, arr);
    arr[(*index)] = node->getPointerToData();
    *index += 1;
    aux_getInorderData(node->getRightSon(), index, arr);
}

template<class Compare, class Data, class REdit>
RTNode<Data>* RankTree<Compare, Data, REdit>::remove(Data data) {
    RTNode<Data>* parent = aux_remove(data, root);
    makeRolls(parent);
    return parent;
}

template<class Compare, class Data, class REdit>
RTNode<Data>* RankTree<Compare, Data, REdit>::aux_remove(Data data, RTNode<Data>* node) {
    if (node == NULL) return NULL;
    if (Compare()(data, node->getData()) < 0) {
        return aux_remove(data, node->getLeftSon());
    }
    else if (Compare()(data, node->getData()) > 0) {
        return aux_remove(data, node->getRightSon());
    }
    else {
        RTNode<Data>* parent = node->getParent();
        if (node->getHeight() == 1) {//no sons
            if (parent == NULL)root = NULL;//node is a root without son:
            else if (parent->getLeftSon() == node)parent->setLeftSon(NULL);
            else parent->setRightSon(NULL);
        }
        else {//node has a son!
            if (node->getLeftSon() != NULL && node->getRightSon() != NULL) {
                //node has a right son and a left son
                RTNode<Data>* toReplace = node->getRightSon();
                while (toReplace->getLeftSon() != NULL)
                    toReplace = toReplace->getLeftSon();
                Data temp = node->getData();
                node->setData(toReplace->getData());
                toReplace->setData(temp);
                return aux_remove(data, toReplace);
            }
            else {
                RTNode<Data>* son = NULL;
                if (node->getLeftSon() == NULL)
                    son = node->getRightSon();
                else if (node->getRightSon() == NULL)
                    son = node->getLeftSon();

                if (parent == NULL)root = son;
                else if (parent->getLeftSon() == node)parent->setLeftSon(son);
                else parent->setRightSon(son);
                son->setParent(parent);
            }
        }
        delete node;
        return parent;
    }
}

template<class Compare, class Data, class REdit>
RankTree<Compare, Data, REdit>* RankTree<Compare, Data, REdit>::createEmptyTree(int size) {
    if (size <= 0) return NULL;
    RankTree<Compare, Data, REdit>* newTree = new RankTree<Compare, Data, REdit>();
    newTree->root = new RTNode<Data>();
    aux_createEmptyTree(size, 1, newTree->root);
    return newTree;
}

template<class Compare, class Data, class REdit>
void RankTree<Compare, Data, REdit>::aux_createEmptyTree(int size, int index, RTNode<Data>* node) {
    if (index * 2 <= size) {
        node->setLeftSon(new RTNode<Data>());
        node->getLeftSon()->setParent(node);
        aux_createEmptyTree(size, index * 2, node->getLeftSon());
    }
    if (index * 2 + 1 <= size) {
        node->setRightSon(new RTNode<Data>());
        node->getRightSon()->setParent(node);
        aux_createEmptyTree(size, index * 2 + 1, node->getRightSon());
    }
    updateheight(node);
    updateSubSize(node);
    //updateNodesSum(node);
}



template<class Compare, class Data, class REdit>
void RankTree<Compare, Data, REdit>::fillTree(Data** array) {
    if (array == NULL) return;
    int index = 0;
    aux_fillTree(array, &index, root);
}

template<class Compare, class Data, class REdit>
void RankTree<Compare, Data, REdit>::aux_fillTree(Data** array, int* index, RTNode<Data>* node) {
    if (array == NULL || node == NULL) return;
    aux_fillTree(array, index, node->getLeftSon());
    node->setData(*array[(*index)]);
    *index += 1;
    aux_fillTree(array, index, node->getRightSon());

}

template<class Compare, class Data, class REdit>
void RankTree<Compare, Data, REdit>::mergeArrays(Data** arr1, int size1, Data** arr2, int size2, Data** mergedArray) {
    int a = 0, b = 0, c = 0;
    for (; a < size1 && b < size2 && c < size1 + size2; c++) {
        if (Compare()(*arr1[a], *arr2[b]) < 0) {
            mergedArray[c] = arr1[a++];
        }
        else {
            mergedArray[c] = arr2[b++];
        }
    }
    while (a < size1) {
        mergedArray[c++] = arr1[a++];
    }
    while (b < size2) {
        mergedArray[c++] = arr2[b++];
    }
}

template<class Compare, class Data, class REdit>
RankTree<Compare, Data, REdit>* RankTree<Compare, Data, REdit>::mergeTrees(RankTree<Compare, Data, REdit>* tree1) {
    if ((root == NULL && tree1 == NULL) || (root == NULL && tree1 != NULL && tree1->root == NULL)) {
        RankTree<Compare, Data, REdit>* newTree = new RankTree<Compare, Data, REdit>();
        return newTree;
    }
    else if ((root != NULL && tree1 == NULL) || (root != NULL && tree1 != NULL && tree1->root == NULL)) {
        Data** arr = new Data * [root->getSizeOfSubTree()];
        getInorderData(arr);
        RankTree<Compare, Data, REdit>* newTree = createEmptyTree(root->getSizeOfSubTree());
        newTree->fillTree(arr);
        delete[]arr;
        return newTree;
    }
    else if (root == NULL && tree1 != NULL) {//tree->root not null!!
        Data** arr = new Data * [tree1->root->getSizeOfSubTree()];
        tree1->getInorderData(arr);
        RankTree<Compare, Data, REdit>* newTree = createEmptyTree(tree1->root->getSizeOfSubTree());
        newTree->fillTree(arr);
        delete[]arr;
        return newTree;
    }
    else {
        int size1 = root->getSizeOfSubTree();
        int size2 = tree1->root->getSizeOfSubTree();
        Data** arr1 = new Data * [size1];
        this->getInorderData(arr1);
        Data** arr2 = new Data * [size2];
        tree1->getInorderData(arr2);
        Data** arr3 = new Data * [size1 + size2];
        mergeArrays(arr1, size1, arr2, size2, arr3);
        RankTree<Compare, Data, REdit>* newTree = createEmptyTree(size1 + size2);
        newTree->fillTree(arr3);
        delete[]arr1;
        delete[]arr2;
        delete[]arr3;
        return newTree;
    }
}

template<class Compare, class Data, class REdit>
RTNode<Data>* RankTree<Compare, Data, REdit>::select(int k) {
    return aux_select(k, root);
}

template<class Compare, class Data, class REdit>
RTNode<Data>* RankTree<Compare, Data, REdit>::aux_select(int k, RTNode<Data>* node) {
    if (node == NULL) return NULL;
    int w = 0;
    if (node->getLeftSon() != NULL) {
        w = node->getLeftSon()->getSizeOfSubTree();
    }
    if (w == k - 1)return node;
    if (w > k - 1) return aux_select(k, node->getLeftSon());
    else return aux_select(k - w - 1, node->getRightSon());
}

template<class Compare, class Data, class REdit>
Data* RankTree<Compare, Data, REdit>::getMaxData() {
    if (root == NULL) return NULL;
    RTNode<Data>* node = root;
    while (node->getRightSon() != NULL)node = node->getRightSon();
    return node->getPointerToData();
}

template< class Data>
void aux_printBT(const std::string& prefix, RTNode<Data>* node, bool isLeft) {
    if (node != nullptr) {
        std::cout << prefix;
        std::cout << (isLeft ? "|--" : "L--");

        // print the value of the node
        std::cout << node->getData() << std::endl;

        // enter the next tree level - left and right branch
        aux_printBT(prefix + (isLeft ? "|   " : "    "), node->getLeftSon(), true);
        aux_printBT(prefix + (isLeft ? "|   " : "    "), node->getRightSon(), false);
    }
}

template< class Data>
void printBT(RTNode<Data>* node) {
    std::cout << std::endl;
    aux_printBT("", node, false);
}



#endif //RANKTREE_H