/*****************************************
* Shahrood User ID:  9814393
* Submitted for Data Structure Course
* Department of Computer Engineering and IT
* Shahrood University of Technology
*
* By submitting this file, I affirm that
* I am the author of all modifications to
* the provided code.
*****************************************/

#ifndef ABSTRACT_TREE_H
#define ABSTRACT_TREE_H

#include <iostream>
#include "exception.h"
#include "my_stack.h"
#include "my_queue.h"

using namespace std;

class AbstractTree {
public:
    class TreeNode {
    public:

        /*
         * Constructor for tree node
         * Takes the value and the parent of the node
         * If the node has no parent it will be NULL
         */
        explicit TreeNode(int const &, TreeNode * = nullptr);

        /*
         * Returns the value of the node
         */
        int value() const;

        /*
         * Returns numbers of the children
         */
        int degree() const;

        /*
         * Returns the parent of the node
         */
        TreeNode *getParent() const;

        /*
         * Gets the number of the child as input and
         * returns the child
         */
        TreeNode *child(int const &) const;

    private:

        // Stores the value of the node
        int nodeValue;

        // Stores the address of the parent
        TreeNode *parent;

        // Stores the addresses of the children
        DoubleLinkedList<TreeNode *> children;

        // Functions

        /*
         * Calculates the number of the nodes of a subtree that root is this node
         */
        int subtreeSize() const;

        /*
         * Calculates the height of a subtree that root is this node
         */
        int subtreeHeight() const;


        /*
         * Searches the subtree of this node the given value
         * returns true if the value is found
         */
        bool findInChildren(const int &) const;

        /*
         * Searches the subtree of this node the given value
         * returns the numbers of the given value
         */
        int count(const int &) const;

        /*
         * Creates a new TreeNode and adds it to children
         */
        void push(const int &);

        /*
         * Check if the children size is equal or less than the maximum degree
         */
        bool isFull(const int &) const;

        /*
         * Cursively deletes all of nodes in tree
         */
        void clear();

        /*
         * Gets the copy tree and the corresponding node in the given
         * tree and adds the corresponding node's children to this node
         */
        void copy(const AbstractTree &, TreeNode *);

        friend AbstractTree;

    };

    /*
     * Constructor for the SearchTree
     * Gets maximum degree
     */
    explicit AbstractTree(int const &);

    /*
     * Constructor for the SearchTree
     * Gets a tree to copy it to itself
     */
    AbstractTree(AbstractTree const &);

    /*
     * Destructor
     * Calls the clear function for the root node
     */
    ~AbstractTree();

    // Accessors

    /*
     * Gets the size of root node
     */
    int size() const;

    /*
     * Gets the height of root node
     */
    int height() const;

    /*
     * Check if the root node is NULL or not
     */
    bool empty() const;

    /*
     * Calls the root find function
     */
    bool find(int const &);

    /*
     * Calls the root count function
     */
    int count(int const &);

    // Mutators

    /*
     * Pushes the given value to the last node of the tree
     */
    void push(int const &);

    /*
     * Calls the push function for each given number
     */
    void push(int *&, int const &);

    /*
     * Deletes the last node in tree and returns the value of it
     */
    int pop();

    // Printers

    /*
     * Uses a queue for navigation the tree and prints it
     */
    void printBFS(std::ostream &);

    /*
     * Uses a stack for navigation the tree and prints it
     */
    void printDFS(std::ostream &);

private:

    // Stores the root of tree
    TreeNode *root;

    // Stores permissible maximum
    int maxDegree;

};

/////////////////////////////////////////////////////////////////////////
//                      Public member functions                        //
/////////////////////////////////////////////////////////////////////////

AbstractTree::AbstractTree(int const &maxDegree) :

        maxDegree(maxDegree),
        root(nullptr) {

}

AbstractTree::AbstractTree(AbstractTree const &tree) :
        maxDegree(tree.maxDegree),
        root(nullptr) {

    if (!tree.empty()) {
        root = new TreeNode(tree.root->nodeValue);

        root->copy(tree, tree.root);
    }
}

AbstractTree::~AbstractTree() {

    if (!empty())
        root->clear();
}

int AbstractTree::size() const {

    if (empty()) {
        return 0;
    }
    return root->subtreeSize();
}

int AbstractTree::height() const {
    if (empty()) return -1;
    return root->subtreeHeight();
}

bool AbstractTree::empty() const {
    return root == nullptr;
}

bool AbstractTree::find(const int &val) {

    if (empty()) return false;

    if (val == root->nodeValue) {
        return true;
    }

    return root->findInChildren(val);
}

int AbstractTree::count(const int &val) {

    int number = 0;

    if (!empty()) {
        if (root->value() == val) {
            number++;
        }

        number += root->count(val);
    }

    return number;
}

void AbstractTree::push(const int &val) {

    if (empty()) {
        root = new TreeNode(val);
    } else {

        Queue<TreeNode *> queue;

        queue.push(root);

        while (!queue.empty()) {
            TreeNode *currentNode = queue.pop();

            if (!currentNode->isFull(maxDegree)) {
                currentNode->push(val);
                return;
            }

            for (int i = 0; i < currentNode->degree(); ++i) {
                queue.push(currentNode->child(i));
            }
        }
    }
}

void AbstractTree::push(int *&ints, const int &number) {

    for (int i = 0; i < number; ++i) {
        push(ints[i]);
    }

}

int AbstractTree::pop() {

    if (!empty()) {

        Queue<TreeNode *> queue;

        TreeNode *lastNode = nullptr;

        queue.push(root);

        while (!queue.empty()) {
            TreeNode *node = queue.pop();

            lastNode = node;
            for (int i = 0; i < node->degree(); ++i) {
                queue.push(node->child(i));
            }
        }

        int lastNodeValue = lastNode->nodeValue;
        if (lastNode->getParent() != nullptr)
            lastNode->getParent()->children.erase(lastNode);
        else
            root = nullptr;
        delete lastNode;
        return lastNodeValue;

    } else throw Underflow();
}

void AbstractTree::printBFS(ostream &out) {

    out << "START->";

    Queue<TreeNode *> queue;

    if (!empty()) {

        queue.push(root);

        while (!queue.empty()) {
            TreeNode *currentNode = queue.pop();

            out << currentNode->nodeValue << "->";

            for (int i = 0; i < currentNode->degree(); ++i) {
                queue.push(currentNode->child(i));
            }
        }

    }

    out << "END";
    out << endl;
}

void AbstractTree::printDFS(ostream &out) {

    out << "START->";

    Stack<TreeNode *> stack;

    if (!empty()) {
        stack.push(root);

        while (!stack.empty()) {
            TreeNode *node = stack.pop();

            out << node->nodeValue << "->";

            for (int i = node->degree() - 1; i >= 0; --i) {
                stack.push(node->child(i));
            }
        }
    }

    out << "END";
    out << endl;
}

// Enter your implementation here

//////////////////////////////////////////////////////////////////////
//                   treeNode Functions                             //
//////////////////////////////////////////////////////////////////////


AbstractTree::TreeNode::TreeNode(
        int const &nv,
        TreeNode *parent) :
        nodeValue(nv),
        parent(parent) {
// Updated the initialization list here
// Enter your implementation here

}

int AbstractTree::TreeNode::value() const {
    return nodeValue;
}

int AbstractTree::TreeNode::degree() const {
    return children.size();
}

AbstractTree::TreeNode *AbstractTree::TreeNode::child(const int &n) const {
    if (n < 0 || n >= degree()) {
        return nullptr;
    }

    auto child = children.begin();

    for (int i = 0; i < n; ++i) {
        child = child->next();
    }
    return child->value();
}

int AbstractTree::TreeNode::subtreeSize() const {
    int subtreeSize = 1;

    for (int i = 0; i < degree(); ++i) {

        subtreeSize += child(i)->subtreeSize();
    }

    return subtreeSize;
}

int AbstractTree::TreeNode::subtreeHeight() const {
    int subtreeHeight = 0;

    for (int i = 0; i < degree(); ++i) {
        subtreeHeight = max(subtreeHeight, child(i)->subtreeHeight() + 1);
    }

    return subtreeHeight;
}

bool AbstractTree::TreeNode::findInChildren(const int &val) const {

    for (int i = 0; i < degree(); ++i) {
        if (val == child(i)->nodeValue) {
            return true;
        }

        if (child(i)->findInChildren(val)) {
            return true;
        }
    }

    return false;
}

int AbstractTree::TreeNode::count(const int &val) const {

    int n = 0;

    for (int i = 0; i < degree(); ++i) {
        if (val == child(i)->nodeValue) {
            n++;
        }

        n += child(i)->count(val);
    }

    return n;
}

AbstractTree::TreeNode *AbstractTree::TreeNode::getParent() const {
    return parent;
}

void AbstractTree::TreeNode::push(const int &val) {

    children.pushBack(new AbstractTree::TreeNode(val, this));
}

bool AbstractTree::TreeNode::isFull(const int &max) const {
    return max == degree();
}

void AbstractTree::TreeNode::clear() {

    for (int i = 0; i < degree(); ++i) {
        child(i)->clear();
    }

    delete this;

}

void AbstractTree::TreeNode::copy(const AbstractTree &tree, TreeNode *copyNode) {

    for (int i = 0; i < copyNode->degree(); ++i) {

        children.pushBack(new TreeNode(copyNode->child(i)->nodeValue, this));
        child(i)->copy(tree, copyNode->child(i));

    }

}

#endif