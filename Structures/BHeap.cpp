#include <iostream>
#include "CircularDynamicArray.cpp"
#include <math.h>

using namespace std;

template <class keytype>
class BHeap {
    private:
        struct TreeNode;

        CircularDynamicArray <TreeNode*> h;
        TreeNode* min;
        keytype error;

    public:
        BHeap();

        BHeap(keytype* k,  int s);

        ~BHeap();

        BHeap(const BHeap& old);

        const BHeap& operator=(const BHeap& old);

        void insert(keytype k);

        void recursivePre(TreeNode* node);

        void printKey();

        keytype peekKey();

        void expandHeap(int correctDegree);

        void addTreeToHeap(TreeNode* root);

        void merge(BHeap <keytype> &H2);

        void setMin();

        keytype extractMin();

        int getSize();
};

template <class keytype>
struct BHeap<keytype>::TreeNode{
    TreeNode* leftChild;
    TreeNode* rightSibling;
    TreeNode* parent;
    keytype k;
    int degree;

    TreeNode(){
        leftChild = nullptr;
        rightSibling = nullptr;
        parent = nullptr;
        degree = 0;
    }

    TreeNode(keytype key){
        leftChild = nullptr;
        rightSibling = nullptr;
        parent = nullptr;
        degree = 0;
        k = key;
    }

    TreeNode(const TreeNode& old){
        leftChild = old.leftChild;
        rightSibling = old.rightSibling;
        parent = old.parent;
        k = old.k;
        degree = old.degree;
    }

    ~TreeNode(){
        leftChild = nullptr;
        rightSibling = nullptr;
        parent = nullptr;
    }

    TreeNode& operator=(const TreeNode& old){
        if(old == *this){
            return *this;
        }
        
        delete leftChild;
        delete rightSibling;
        delete parent;

        leftChild = old.leftChild;
        rightSibling = old.rightSibling;
        parent = old.parent;
        degree = old.degree;
        k = old.k;
        return *this;
    }
};

template <class keytype> 
BHeap<keytype>::BHeap(){
    h = CircularDynamicArray<TreeNode*>();
    min = nullptr;
}

template <class keytype>
BHeap<keytype>::BHeap(keytype* k,  int s){
    h = CircularDynamicArray<TreeNode*>();
    min = nullptr;
    for(int i = 0; i < s; i++){
        insert(k[i]);
    }
}

template <class keytype>
BHeap<keytype>::~BHeap(){
            
}

template <class keytype>
BHeap<keytype>::BHeap(const BHeap& old){
    for(int i = 0; i < old.h.length();i++){
        h.addEnd( old.h[i]);
    }
    min = old.min;
}

template <class keytype>
const BHeap<keytype>& BHeap<keytype>::operator=(const BHeap& old){
    for(int i = 0; i<h.length();i++){
        h[i] = old.h[i];
    }
    min = old.min;
    return *this;
}

template <class keytype>
void BHeap<keytype>::insert(keytype k){
    TreeNode* ins = new TreeNode(k);
    if(h.length() == 0){
        min = ins;
    }
    if(min->k > ins->k){
        min = ins;
    }
    addTreeToHeap(ins);
}

template <class keytype>
void BHeap<keytype>::recursivePre(TreeNode* node){
    if(node == nullptr) return;
    cout << node->k <<" ";
    recursivePre(node->leftChild);
    recursivePre(node->rightSibling);
}

template <class keytype>
void BHeap<keytype>::printKey(){
    for(int i = 0 ;i<h.length();i++){
        if(h[i] != nullptr){
            cout << "B" << i << endl;
            recursivePre(h[i]);
            cout << endl;
            cout << endl;
        }
    }
}

template <class keytype>
keytype BHeap<keytype>::peekKey(){
    if(min == nullptr){
        cout << "Error: Nothing in the heap" << endl;
        return error;
    }
    return min->k;
}

template <class keytype>
void BHeap<keytype>::expandHeap(int correctDegree){
    int i = 0;
    while(1){
        if(h.length()-1 == correctDegree){
            return;
        }
        else{
            h.addEnd(nullptr);
        }
    }
}

template <class keytype>
void BHeap<keytype>::addTreeToHeap(TreeNode* root){
    int i = root->degree;
    if(i > h.capacity()){
        expandHeap(i);
    }
    while(1){
        if(i == h.length()){ 
            h.addEnd(root);
            break;
        }
        if (h[i] == nullptr){
            h[i] = root;
            break;
        }
        else{
            if(root->k < h[i]->k){
                TreeNode* temp = root->leftChild;
                root->leftChild = h[i];
                h[i]->rightSibling = temp;
                h[i]->parent = root;
            }
            else{
                TreeNode* temp = h[i]->leftChild;
                h[i]->leftChild = root;
                root->rightSibling = temp;
                root->parent = h[i];
                root = h[i];
            }
            root->degree = (i+1);
            h[i] = nullptr;
            i++;
        }
    }
}

template <class keytype>
void BHeap<keytype>::merge(BHeap <keytype> &H2){
    for(int i = 0; i < H2.h.length();i++){
        if(H2.h[i] != nullptr) addTreeToHeap(H2.h[i]);
    }
    setMin();
    H2.h.clear();
}

template <class keytype>
void BHeap<keytype>::setMin(){
    int currMinInd = 0;
    while(1){
        if(h[currMinInd] != nullptr || currMinInd == h.length() - 1) break;
        currMinInd++;
    }
    for(int i = currMinInd + 1; i < h.length();i++){
        if(h[i] != nullptr){     
            if((h[i]->k < h[currMinInd]->k)){
                currMinInd = i;
        }
        }
    }
    min = h[currMinInd];
}

template <class keytype>
keytype BHeap<keytype>::extractMin(){
    keytype ret = min->k;
    if(h.length() == 1){
        h.clear();
        return ret;
    }
    CircularDynamicArray<TreeNode*> tempCDA(min->degree);
    TreeNode* curr = min->leftChild;
    h[min->degree] = nullptr;
    while(curr != nullptr){
        tempCDA[curr->degree] = curr;
        TreeNode* prev = curr;
        curr = curr->rightSibling;
        prev->rightSibling = nullptr;
        prev->parent = nullptr;
    }
    for(int i = 0; i < tempCDA.length();i++){
        if(tempCDA[i] != nullptr) addTreeToHeap(tempCDA[i]);
    }
    tempCDA.clear();
    setMin();
    return ret;
}

template <class keytype>
int BHeap<keytype>::getSize(){
    int total = 0;
    for(int i = 0;i<h.length();i++){
        // std::cout << h[i]->degree << std::endl;
        if(h[i] != nullptr){
            total+= pow(2,h[i]->degree);
        }
    }

    return total;
}