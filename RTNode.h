
#ifndef RTNode_H
#define RTNode_H

#include <cstdlib>
#include <iostream>

template<class Data>
class RTNode {
    RTNode* parent;
    RTNode* leftSon;
    RTNode* rightSon;
    int height;
    int subSize;
    int nodesSum;
    Data data;

public:
    RTNode() :
        parent(NULL), leftSon(NULL), rightSon(NULL), height(1), subSize(1), nodesSum(0) {}
    RTNode(Data data) :
        parent(NULL), leftSon(NULL), rightSon(NULL), height(1), subSize(1), nodesSum(0), data(data) {}

    void setParent(RTNode* parent) {
        this->parent = parent;
    }

    RTNode* getParent() const {
        return parent;
    }

    void setLeftSon(RTNode* leftSon) {
        this->leftSon = leftSon;
    }

    RTNode* getLeftSon() const {
        return leftSon;
    }

    void setRightSon(RTNode* rightSon) {
        this->rightSon = rightSon;
    }

    RTNode* getRightSon() const {
        return rightSon;
    }

    void setHeight(int height) {
        this->height = height;
    }

    int getHeight() const {
        return height;
    }

    void setSizeOfSubTree(int subSize) {
        this->subSize = subSize;
    }

    int getSizeOfSubTree() {
        return subSize;
    }

    void setSum(int nodesSum) {
        this->nodesSum = nodesSum;
    }

    int getSum() const {
        return nodesSum;
    }

    void setData(Data data) {
        this->data = data;
    }

    Data getData() {
        return data;
    }

    Data* getPointerToData() {
        return &data;
    }
};



#endif // RTNode_H
