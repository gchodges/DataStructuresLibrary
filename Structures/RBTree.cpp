#define Red true
#define Black false
#include <iostream>
#include <cmath>

using namespace std;

template <class keytype,class valtype>
class RBTree{
    private:
        
        struct TreeNode;

        TreeNode* root;     //The root of the Red-Black Binary search tree. It is a tree node
        TreeNode* nil;      //I used a user defined nil node as used in the book

        //Private Methods
        void leftRotate(TreeNode* x);

        void rightRotate(TreeNode* y);

        //This function prints the inorder traversal of the tree in O(n) time. Follows a standard inorder Traversal.
        void inorderRecursive(TreeNode* node);

        bool isSorted(keytype* k, int s);

        //This function recursively generates a red-black binary search tree based upon a sorted keytype array in
        //O(nlogn) time.
        TreeNode* recursiveCons(keytype* k, valtype* v, int front, int end, TreeNode* p, int currLevel, int redLevel, bool perf);

        int logRoundDown(int s);

        bool isPerfect(int s);

        void updateNodeSize(TreeNode* node, keytype k);

        void transplant(TreeNode* u, TreeNode* v);

        void delete_fixup(TreeNode* x);

    public:

    //A function I found and messed with to make a visually appealing tree
    // void printBT(const string& prefix, const TreeNode *node, bool isLeft);

    // void printBT();

    //Default RBTree Constructor
    RBTree();

    RBTree(keytype* k, valtype* v, int s);

    void destructorHelper(TreeNode* node);

    ~RBTree();

    //Copy Constructor
    RBTree(const RBTree& old);

    //Copy constructor helper function
    TreeNode* copyConsHelpe(TreeNode* old, TreeNode* n, TreeNode* p);

    RBTree& operator=(const RBTree& old);


    int recursiveRank(TreeNode* node, keytype k);
    
    //My rank method
    int rank(keytype k);

    //Select is reverse rank. I found recursion harder, so I used an iterative method
    keytype select(int pos);

    //To see why rank/select are not working after delete

    void helpWhatIsWrongHelper(TreeNode* root);

    void helpWhatIsWrong();

    //Insertion method from book
    void insert(keytype k, valtype v);

    //This is a simple method to return a boolean indicating whether or not a node with key k exists. It is used in other methods
    bool isFound(keytype z);

    //This method works exactly as isFound, but returns the node we are searching for
    TreeNode* searchForNode(keytype k);

    //This is the remove algorithm from the book
    int remove(keytype k);

    //This is the successor method as requested in the lab file
    keytype* successor(keytype k);

    //This works the exact same way as successor, but it returns a node
    TreeNode* successorNode(TreeNode* node);


    //This is a recursive helper function for my preorder() method.
    //It is a standard preorder traversal printing before recursing
    void preorderRecursive(TreeNode* node);

    //Preorder calls the recursive helper and prints a newline
    void preorder();

    //This is my postorder method that recursively prints the key after recursing
    void postorderRecursive(TreeNode* node);

    //This method calls the recursive postorder function and prints an endline
    void postorder();

    //This is my predecessor method
    keytype* predecessor(keytype k);

    //Same as predecessor, but returns a node
    TreeNode* predecessorNode(TreeNode* node);

    // int remove(){

    // }

    
    void insert_fixup(TreeNode* z);

    valtype* search(keytype k);


    void inorder();
    

    //Getters Below
    keytype getKey(TreeNode node);

    valtype getVal(TreeNode node);

    int size();

};

template <class keytype,class valtype>
void RBTree<keytype,valtype>::leftRotate(TreeNode* x){       //This is the Left Rotate method defined in the book
    TreeNode* y = x->right;         //Create a new node pointer that points to x's right child          //Turn y's left subtree into x's right subtree
    x->right = y->left;             //Change x's right child to y's left child
    if(y->left != nil){             //If y->left is NOT Nil, x becomes the parent of y's right child
        y->left->parent = x;
    }
    y->parent = x->parent;          //x's parent becomes y's parent
    if(x->parent == nil){        //If x is root, root = y
        root = y;
    }
    else if(x == x->parent->left){      //if x is a left child, make x's parents left child y
        x->parent->left = y;
    }
    else{
        x->parent->right = y;           //otherwise, make x's parent's right child y
    }
    y->left = x;                //y's left child becomes x
    x->parent = y;
    y->nodeSize = x->nodeSize;      //y's size becomes x's size
    x->nodeSize = x->right->nodeSize + x->left->nodeSize + 1;       //x's size becomes the sum if its children's size plus one (itself)
    //NEED TO CHANGE SIZE;
}

template <class keytype,class valtype>
void RBTree<keytype,valtype>::rightRotate(TreeNode* y){      //This is the Right Rotate method defined in the book, same as above but flipped
    TreeNode* x = y->left;
    y->left = x->right;
    if(x->right != nil){
        x->right->parent = y;
    }
    x->parent = y->parent;
    if(y->parent == nil){
        root = x;
    }
    else if(y == y->parent->left){
        y->parent->left = x;
    }
    else{
        y->parent->right = x;
    }
    x->right = y;
    x->nodeSize = y->nodeSize;
    y->parent = x;
    y->nodeSize = y->left->nodeSize + y->right->nodeSize + 1;
    
    //NEED TO CHANGE SIZE
}

//This function prints the inorder traversal of the tree in O(n) time. Follows a standard inorder Traversal.
template <class keytype,class valtype>
void RBTree<keytype,valtype>::inorderRecursive(TreeNode* node){
    if (node == nil){           //Base case: The node is nil, so return
        return;
    }
    inorderRecursive(node->left);       //Recurse over the left subtree first

    cout << node->key << " ";           //Print the node value

    inorderRecursive(node->right);      //Recurse into the right subtree
}

template <class keytype,class valtype>
bool RBTree<keytype,valtype>::isSorted(keytype* k, int s){           //Checks if the array is sorted. A helper function for the array based constructor
    for(int i = 0; i< s - 1;i++){
        if(k[i] > k[i+1])   return false;
    }
    return true;
}

//This function recursively generates a red-black binary search tree based upon a sorted keytype array in
//O(nlogn) time.
template <class keytype,class valtype>
typename RBTree<keytype,valtype>::TreeNode* RBTree<keytype,valtype>::recursiveCons(keytype* k, valtype* v, int front, int end, TreeNode* p, int currLevel, int redLevel, bool perf){
    if(end <front){     //Base case. If the front pointer has passed the end index pointer, it is a nil node (End of tree)
        
        return nil;
    }
    int med = (end + front) /2;     //The median of the current subarray is calculated
    TreeNode* node = new TreeNode();        //A new tree node is created to add to the tree
    node->val = v[med];         //The key and value array values at median are stored in the new node as they are to be added next
    node->key = k[med];
    node->color = Black;        //Color is set to black UNLESS NODE IS AT RED LEVEL
    node->parent = p;           //The parent of the node is set to the previous node
    node->nodeSize = (end - front + 1);         //The size of the node includes all nodes beneath it plus itself
    if((currLevel == redLevel) && (!perf)) node->color = Red;       //If it is at the red  level & the tree is not perfect, the color remains black
    currLevel++;                            //Increase current track iterator
    node->left = recursiveCons(k,v,front, med - 1, node, currLevel, redLevel, perf);        //Recurse on the left subarray to find the left child
    node->right = recursiveCons(k,v,med+1,end, node, currLevel, redLevel, perf);            //Recurse on the right subarray for right child
    return node;
}

template <class keytype,class valtype>
int RBTree<keytype,valtype>::logRoundDown(int s){        //This is a helper function to help find red level. Essentially takes the floor of log2(s)
    int count = 0;
    while(1){
        s /= 2;
        if(s == 0)  break;
        count++;
    }
    return count;
}

template <class keytype,class valtype>
bool RBTree<keytype,valtype>::isPerfect(int s){          //Checks if the tree is of size +1 is a perfect log base 2 (ie ceiling and floor are the same)
    return(ceil(log2(s))==floor(log2(s)));
}

template <class keytype,class valtype>
void RBTree<keytype,valtype>::updateNodeSize(TreeNode* node, keytype k){     //This updates node size upon deletion of a node by traversing through the tree and decreasing size
                                                    //until node is found
    TreeNode* curr = root;
    while(1){
        if(curr == node) break;
        curr->nodeSize--;
        if(curr->key < k)   curr = curr->right;
        else    curr = curr->left;
    }
}

template <class keytype,class valtype>
void RBTree<keytype,valtype>::transplant(TreeNode* u, TreeNode* v){      //Transplant function from the book. It calls updateNodeSize
    //updateNodeSize(u, u->key);
    if(u->parent == nil){
        root = v;
    }
    else if(u == u->parent->left){
        u->parent->left = v;
    }
    else{
        u->parent->right = v;
    }
    v->parent = u->parent;
}

template <class keytype,class valtype>
void RBTree<keytype,valtype>::delete_fixup(TreeNode* x){                 //Delete fixup from the book
    while((x != root) && (x->color == Black)){
        if(x == x->parent->left){
            TreeNode* w = x->parent->right;
            if(w->color == Red){
                w->color = Black;                       //Case 1
                x->parent->color = Red;
                leftRotate(x->parent);
                w = x->parent->right;
            }
            if((w->left->color == Black) && (w->right->color == Black)){        //Case 2
                w->color = Red;
                x = x->parent;
            }                   
            else{
                if(w->right->color == Black){                   //Case 3
                    w->left->color = Black;
                    w->color = Red;
                    rightRotate(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;//Case  4
                x->parent->color = Black;
                w->right->color = Black;
                leftRotate(x->parent);
                x = root;
            }
        }
        else{
            TreeNode* w = x->parent->left;
            if(w->color == Red){        //Case 1
                w->color = Black;
                x->parent->color = Red;
                rightRotate(x->parent);
                w = x->parent->left;
            }
            if((w->right->color == Black) && (w->left->color == Black)){        //Case 2
                w->color = Red;
                x = x->parent;
            }
            else{
                if(w->left->color == Black){        //Case 3
                    w->right->color = Black;
                    w->color = Red;
                    leftRotate(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;//Case 4
                x->parent->color = Black;
                w->left->color = Black;
                rightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = Black;
}

template <class keytype,class valtype>
struct RBTree<keytype, valtype>::TreeNode{        //This is a user defined  struct to store a full Tree Node
    keytype key;        
    valtype val;
    TreeNode* left;
    TreeNode* right;
    TreeNode* parent;
    bool color;
    int nodeSize;

    TreeNode(){         //The node constructor sets all pointers to nullptr and sets the size to 1
        left = nullptr;
        right = nullptr;
        color = Red;
        parent = nullptr;
        nodeSize = 1;
    }
    ~TreeNode(){        //The destructor sets all pointers to nullptr to manage memory
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }

    TreeNode(const TreeNode& old){      //Deep copy constructor deep copies a node to a new node and sets all of its pointers to nullptr
        key = old.key;
        val = old.val;
        color = old.color;
        left = nullptr;
        right  = nullptr;
        parent  = nullptr;
    }

    TreeNode& operator=(const TreeNode& old){       //Copy assignment deep copies a node's characteristics to an existing node;
        if(this == old) return *old;
        key = old.key;
        val = old.val;
        color = old.color;
        left = nullptr;
        right  = nullptr;
        parent  = nullptr;
        return *this;
    }

};

template <class keytype,class valtype>
//Default RBTree Constructor
RBTree<keytype,valtype>::RBTree(){
    nil = new TreeNode();
    nil->nodeSize = 0;
    nil->color = Black;
    nil->left = nil;
    nil->right = nil;
    nil->parent = nil;
    root = nil;
}

template <class keytype,class valtype>
RBTree<keytype,valtype>::RBTree(keytype* k, valtype* v, int s){      //RBTree Array Constructor
    nil = new TreeNode();       //Create nil node
    nil->nodeSize = 0;
    nil->color = Black;
    nil->right = nil;
    nil->left = nil;
    nil->parent = nil;
    root = nil;
    root->parent = nil;
    bool sorted = isSorted(k, s);       
    

    if(sorted){         //If the array is sorted, 
        int front = 0;
        int end = s - 1;
        int med = (end + front) / 2;

        int currLayer = 0;

        int maxLayer = logRoundDown(s);     //Find max level
        bool perfect = isPerfect(s + 1);        //Find out if perfect
        root = recursiveCons(k,v,front,end, nil,currLayer,maxLayer, perfect);       //And recurse

        root->parent = nil;

    }
    else{           //If it is not sorted, use repeated insertion
        for(int i = 0;i < s; i++){
            insert(k[i],v[i]);
        }
    }
}

template <class keytype,class valtype>
void RBTree<keytype,valtype>::destructorHelper(TreeNode* node){      //The destructor Helper function. Recurses through tree and deletes nodes bottom-up
    if(node == nil){
        return;
    }
    destructorHelper(node->left);
    destructorHelper(node->right);
    delete node;
}

template <class keytype,class valtype>
RBTree<keytype,valtype>::~RBTree(){
    destructorHelper(root);     //Destructor just calls helper
}

//Copy Constructor
template <class keytype,class valtype>
RBTree<keytype,valtype>::RBTree(const RBTree& old){
    nil = new TreeNode();
    nil->parent = nil;
    nil->left = nil;
    nil->right = nil;       //Deep copies by calling helper
    nil->nodeSize = 0;
    nil->color = Black;
    root = nil;
    root->parent = nil;
    TreeNode* curr = old.root;
    root = copyConsHelpe(curr,old.nil,nil);
}

//Copy constructor helper function
template <class keytype,class valtype>
typename RBTree<keytype, valtype>::TreeNode* RBTree<keytype, valtype>::copyConsHelpe(TreeNode* old, TreeNode* n, TreeNode* p){
    if(old == n) return nil;        //If it source is nil return nil
    TreeNode* to_copy_to = new TreeNode(*old);
    to_copy_to->nodeSize = old->nodeSize;
    to_copy_to->parent = p;
    to_copy_to->left = copyConsHelpe(old->left,n,to_copy_to);       //deep copy and recurse to set children
    to_copy_to->right = copyConsHelpe(old->right,n,to_copy_to);
    // if(to_copy_to->parent == nil)root = to_copy_to;
    return to_copy_to;
}

template <class keytype,class valtype>
RBTree<keytype,valtype>& RBTree<keytype,valtype>::operator=(const RBTree& old){       //Destroys old tree and deep copies
    if(this != &old){
        destructorHelper(root);
        TreeNode* curr = old.root;
        root = copyConsHelpe(curr, old.nil, nil);
    }
    else{
        return *this;
    }
}

template <class keytype,class valtype>
int RBTree<keytype,valtype>::recursiveRank(TreeNode* node, keytype k){       //The recursive rank helper function.
    if(node == nil) return 1;       //If the node is nil, return 1 for the lowest possible rank
    if(node->key < k){      //if the node you are looking for is higher return the left childs size plus the recursion on the right
        return 1 + node->left->nodeSize + recursiveRank(node->right,k);
    }
    else{
        return recursiveRank(node->left,k);     //otherwise move left
    }
}

//My rank method
template <class keytype,class valtype>
int RBTree<keytype,valtype>::rank(keytype k){        
    if(!isFound(k)) return 0;       //If there is no node with key k, return 0

    TreeNode* curr = root;          //A temporary TreeNode* pointing to root

    return recursiveRank(curr,k);       //Recursively search for rank
}

//Select is reverse rank. I found recursion harder, so I used an iterative method
template <class keytype,class valtype>
keytype RBTree<keytype,valtype>::select(int pos){
    TreeNode* curr = root;
    while(1){
        if((curr->left->nodeSize + 1) == pos) return curr->key;     //If you find the kth smallest element, return
        if(pos > (curr->left->nodeSize + 1)){                       //If the position you are looking for is greater than the current position
            pos = pos - (curr->left->nodeSize + 1);     //Subtract the current position from the position you are looking for and move right
            curr = curr->right;
        }
        else{           //Otherwise, the position is in the left subtree
            curr = curr->left;
        }
    }
}

//Insertion method from book
template <class keytype,class valtype>
void RBTree<keytype,valtype>::insert(keytype k, valtype v){
    TreeNode* to_insert = new TreeNode();       //Create a new node and assign all values as default values
    to_insert->key = k;
    to_insert->val = v;
    to_insert->nodeSize = 1;
    to_insert->color = Red;                 //RBTrees insert red nodes and recolor if necessary
    to_insert->left = nil;
    to_insert->right = nil;
    to_insert->parent = nil;
    TreeNode* currX = root;         //Create a pointer to the root
    TreeNode* currY = nil;          //this pointer will stay one behind X to be assigned as the parent later
    while(currX != nil){
        currY = currX;
        currX->nodeSize++;          //Increment size of the nodes on the way down
        if(k < currX->key){             //Move left if the key is less than current node key
            currX = currX->left;
        }
        else{
            currX = currX->right;           //move right otherwise
        }
    }
    to_insert->parent = currY;          //The parent equals the currY which is one node behind
    if(currY == nil){           //If Y is nil, that means that the new node is the root
        root = to_insert;
    }

    else if(to_insert->key < currY->key){           //If the key of the insertion node is less than its parent, it is a left child
        currY->left = to_insert;
    }
    
    else{                       //Otherwise it is a right child
        currY->right = to_insert;
    }
    if(to_insert->parent == nil){           //If it is the root, it is assigned black (First insertion)
        to_insert->color = Black;
        return;
    }
    if(to_insert->parent->parent == nil){       //If it is the second insertion we do not need to fixup
        return;
    }
    insert_fixup(to_insert);            //Otherwisem fixup.
    //Algorithm for Red-Black insertion from book
}

//This is a simple method to return a boolean indicating whether or not a node with key k exists. It is used in other methods
template <class keytype,class valtype>
bool RBTree<keytype,valtype>::isFound(keytype z){            
    TreeNode* curr = root;
    while(1){           //Search until you find the node or nil. If nil, it does not exist. If found, it does
        if(curr == nil){
            return false;
        }
        if(curr->key == z) return true;
        if(curr->key < z){
            curr = curr->right;
        }
        else{
            curr = curr->left;
        }
    }
    return true;
}

//This method works exactly as isFound, but returns the node we are searching for
template <class keytype,class valtype>
typename RBTree<keytype,valtype>::TreeNode* RBTree<keytype,valtype>::searchForNode(keytype k){
    TreeNode* curr = root;
    while(1){
        if(curr == nil){
            return nil;
        }
        if(curr->key == k){
            return curr;
        }
        if(curr->key > k){
            curr = curr->left;
        }
        else if(curr->key < k){
            curr = curr->right;
        }
    }
}

//This is the remove algorithm from the book
template <class keytype,class valtype>
int RBTree<keytype,valtype>::remove(keytype k){
    bool found = isFound(k);        //If the node does not exist, return 0
    if(found == false)  return 0;
    TreeNode* z = searchForNode(k);         //Create the nodes used for remove in  the book
    TreeNode* y = z;
    TreeNode* x;

    bool y_original_color = y->color;
    
    if(z->left == nil){             //If z has no left child
        x = z->right;          
        transplant(z,z->right);         //Replace Z with its right child
        if(z->right != nil) updateNodeSize(x, x->key);          //If z->left is not nil, update node size of all nodes greater than x
        else{                               //If z->right is nil, updateNodeSize does not find nil and update correctly as it has no key
            updateNodeSize(x->parent,x->parent->key);               //Instead, update node size up to x->parent and then decrement x->size
            x->parent->nodeSize--;
        }
    }
    
    else if(z->right == nil){           //Same as previous, but with z->right as nil
        x = z->left;
        transplant(z,z->left);
        if(z->left != nil) updateNodeSize(x, x->key);
        else{
            updateNodeSize(x->parent,x->parent->key);
            x->parent->nodeSize--;
        }
    }
    else{           //Otherwise, z has two children and predecessor delete is necessary
        y = predecessorNode(z);             //Set y to be the predecessor of z
        y_original_color = y->color;        //Set the y_original_color variable for later
        x = y->left;            //Set x to be the child of y
        y->nodeSize = z->nodeSize - 1;          //Set the size of y to be the size of z minus 1 (itself);
        updateNodeSize(y,y->key);       //Update the size of the nodes up to y
        // y->parent->nodeSize--;
        if(y->parent == z){             //If y is the child of z, y becomes the parent of x
            x->parent = y;
        }
        else{           //otherwise, replace y with its left child and assign child pointers
            transplant(y,y->left);
            y->left = z->left;
            y->left->parent = y;
        }
        transplant(z,y);            //Replace y with z and assign pointers/color
        y->right = z->right;
        y->right->parent = y;
        y->color = z->color;
    }
    delete z;           //Deallocate the memory for z. Should call node destructor
    if(y_original_color == Black){              //If the original color was black, call delete_fixup
        delete_fixup(x);
    }
    return 1;           //Return 1 for a successful delete
}

//This is the successor method as requested in the lab file
template <class keytype,class valtype>
keytype* RBTree<keytype,valtype>::successor(keytype k){
    if(isFound(k) == false) return NULL;            //If there is no node with key k, return NULL 
    TreeNode* curr = searchForNode(k);
    if(curr->right != nil){             //If the current->right pointer is not nil
        curr = curr->right;             //Step right, then step as far as possible left
        while(1){
            if(curr->left == nil){
                keytype* ret = &curr->key;          //Return the successor key
                return ret;
            }
            curr = curr->left;
        }
    }
    else{           //If the current->right is nil, 
        while(1){                          
            if(curr->parent == nil) return NULL;            //If you reach the root, return NULL (Successor does not exist)
            if(curr == curr->parent->left){             //If the current node is a left child
                keytype* ret = &curr->parent->key;          //return its key
                return ret;
            }
            curr = curr->parent;            //Otherwise, keep moving up the tree
        }
        
    }
}

//This works the exact same way as successor, but it returns a node
template <class keytype,class valtype>
typename RBTree<keytype,valtype>::TreeNode* RBTree<keytype,valtype>::successorNode(TreeNode* node){
    TreeNode* curr = node;
    if(curr->right != nil){
        curr = curr->right;
        while(1){
            if(curr->left == nil) return curr;
            curr = curr->left;
        }
    }
    else{
        while(1){    
            if(curr->parent == nil) return NULL;
            if(curr == curr->parent->left) return curr->parent;
            curr = curr->parent;
        }
        
    }
}


//This is a recursive helper function for my preorder() method.
//It is a standard preorder traversal printing before recursing
template <class keytype,class valtype>
void RBTree<keytype,valtype>::preorderRecursive(TreeNode* node){
    if(node == nil){
        return;
    }
    cout << node->key << " ";
    preorderRecursive(node->left);
    preorderRecursive(node->right);
}

//Preorder calls the recursive helper and prints a newline
template <class keytype,class valtype>
void RBTree<keytype,valtype>::preorder(){
    preorderRecursive(root);
    cout << endl;
}

//This is my postorder method that recursively prints the key after recursing
template <class keytype,class valtype>
void RBTree<keytype,valtype>::postorderRecursive(TreeNode* node){
    if(node == nil){
        return;
    }
    postorderRecursive(node->left);
    postorderRecursive(node->right);
    cout << node->key << " ";
}

//This method calls the recursive postorder function and prints an endline
template <class keytype,class valtype>
void RBTree<keytype,valtype>::postorder(){
    postorderRecursive(root);
    cout << endl;
}

//This is my predecessor method
template <class keytype,class valtype>
keytype* RBTree<keytype,valtype>::predecessor(keytype k){
    if(isFound(k) == false) return NULL;            //If no node with key k exists, return NULL
    TreeNode* curr = searchForNode(k);
    if(curr->left != nil){          //If the node with key k has a left child
        curr = curr->left;          //Step left once, then step right until you can't anymore
        while(1){
            if(curr->right == nil){
                keytype* ret = &curr->key;
                return ret;
            }
            curr = curr->right;
        }
    }
    else{                           //If the node has no left child
        while(1){    
            if(curr->parent == nil) return NULL;    //If the current node has no parent, it does not have a predecessor
            if(curr == curr->parent->right){            //Otherwise, if it is a right child it is the predecessor
                keytype* ret = &curr->parent->key;
                return ret;     //Return it
            }
            curr = curr->parent;                //Move to the parent and continue
        }
        
    }
}

//Same as predecessor, but returns a node
template <class keytype,class valtype>
typename RBTree<keytype,valtype>::TreeNode* RBTree<keytype,valtype>::predecessorNode(TreeNode* node){
    TreeNode* curr = node;
    if(curr->left != nil){
        curr = curr->left;
        while(1){
            if(curr->right == nil) return curr;
            curr = curr->right;
        }
    }
    else{
        while(1){    
            if(curr->parent == nil) return nil;
            if(curr == curr->parent->right) return curr->parent;
            curr = curr->parent;
        }
        
    }
}

template <class keytype,class valtype>
void RBTree<keytype,valtype>::insert_fixup(TreeNode* z){
    while(z->parent->color == Red){
        if(z->parent == z->parent->parent->left){
            TreeNode* y = z->parent->parent->right;
            if(y->color == Red){
                z->parent->color = Black;
                y->color = Black;
                z->parent->parent->color = Red;
                z = z->parent->parent;
            }
            else{
                if(z == z->parent->right){
                    z = z->parent;
                    leftRotate(z);
                }
                z->parent->color = Black;
                z->parent->parent->color = Red;
                rightRotate(z->parent->parent);
            }
        }
        else{
                TreeNode* y = z->parent->parent->left;
                if(y->color == Red){
                    z->parent->color = Black;
                    y->color = Black;
                    z->parent->parent->color = Red;
                    z = z->parent->parent;
                }
                else{ 
                    if(z == z->parent->left){
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = Black;
                    z->parent->parent->color = Red;
                    leftRotate(z->parent->parent);
                }
        }
        if(z == root){
            z->color = Black;
            break;
        }
    }
    root->color = Black;
}

template <class keytype,class valtype>
valtype* RBTree<keytype,valtype>::search(keytype k){
    TreeNode* curr = root;
    while(1){
        if(curr == nil){
            valtype* ret = NULL;
            return ret;
        }
        if(curr->key == k){
            valtype* ret = &curr->val;
            return ret;
        }
        if(curr->key > k){
            curr = curr->left;
        }
        else if(curr->key < k){
            curr = curr->right;
        }
    }
}

template <class keytype,class valtype>
void RBTree<keytype,valtype>::inorder(){
    inorderRecursive(root);
    cout << endl;
}


//Getters Below
template <class keytype,class valtype>
keytype RBTree<keytype,valtype>::getKey(TreeNode node){
    return node.key;
}

template <class keytype,class valtype>
valtype RBTree<keytype,valtype>::getVal(TreeNode node){
    return node.val;
}

template <class keytype,class valtype>
int RBTree<keytype,valtype>::size(){
    return root->nodeSize;
}
