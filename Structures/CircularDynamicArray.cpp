#include <iostream>

using namespace std;

template <class elmtype>
class CircularDynamicArray{
    private:
        int cap;
        int size;
        int front;
        int back;
        bool isReversed;
        elmtype* CDArray;
        elmtype error;

        /*
        This method creates a new array of double the cap of the current CDArray and 
        copies the contents of the old array to indices 0-(size - 1);
        */
        void resizeArray();

        /*
        This method creates a new array of half the cap of the current CDArray and 
        copies the contents of the old array to indices 0-(size - 1);
        */

        void downsizeArray();

        /*
        This is the standard mergesort algorithm. It breaks down the array into partitions until the are Size one arrays.
        It then calls the merge function, which iterates through each subarray, combining them in order into a larger subarray, 
        which recurses until the beginning pointer crosses the end.
        */
        
        void mergeSort(int const begin, int const end);

        void merge(int const begin, int const mid, int const end);

        /*
        This uses the quick select algorithm discussed in the class slides.
        */

        elmtype quickSelectAlgo(CircularDynamicArray <elmtype> array, int k);

    public:

        /*
        Default constructor
        */
        
        CircularDynamicArray();

        /*
        Constructor that more or less acts as a standard array with dynamic array abilities. The user can instantly
        access all indices in the capacity of the array, but the size can increase.
        */

        CircularDynamicArray(int s);

        /*
        Copy Constructor
        */

        CircularDynamicArray(const CircularDynamicArray &old);

        /*
        Copy assignment operator
        */

        CircularDynamicArray& operator=(const CircularDynamicArray& old);

        /*
        Destructor
        */

        ~CircularDynamicArray();

        /*
        Bracket operator. The user can access any index in the array from the user perspective. For example, if the array currently had a back index of 
        7  and a front index of 10 with capacity 16, CDA[2] = memberArray[12]. I handle the reversal of the array mostly through the use of this operator.
        This allows me to write less code for most other methods in the class as they can be used both reversed and not reversed.
        */

        elmtype& operator[](int i);


        /*
        This method adds a new element to the front of the circular dynamic array, resizing if necessary.
        */

        void addFront(elmtype v);

        /*
        This method adds a new element to the end of the circular dynamic array, resizing if necessary.
        */

        void addEnd(elmtype v);

        /*
        This method 'deletes' the last element in the array by decreasing array size and decreasing the back index by one.
        */

        void delEnd();

        /*
        This method 'deletes' the first element in the array by decreasing array size and increasing the front index by one.
        */

        void delFront();

        /*
        This method deallocates any memory currently allocated for the CDA and creates a new array of size 0 and cap 2.
        */

        void clear();

    /*
    This method performs a linear search of the circular dynamic array and returns the index of the first appearance of elmtype e or -1 if it is not found.
    */

        int linearSearch(elmtype e);

        /*
        This method performs a binary search of the circular dynamic array and returns the index of elmtype e in the array if found, otherwise
        it returns -1. Assumes the array is already sorted in ascending order.
        */

        int binSearch(elmtype e);

        /*
        This method flips the isReversed boolean, thus changing how other functions 'see' the array. This is seen by my bracket operator, which handles most of the trouble.
        */

        void reverse();

    /*
    This method uses the merge sort algorithm to sort the circular dynamic array. Merge sort was chosen as it is both a stable sorting algorithm and
    it has a time complexity of O(nlgn).
    */
        void stableSort();

        /*
        This method returns the kth smallest element in the array using the quickselect algorithm in O(size) time complexity
        */

        elmtype QuickSelect(int k);

        //Getters below

        int capacity();

        int getFront();

        int getEnd();

        int length();
};


/*
    This method creates a new array of double the cap of the current CDArray and 
    copies the contents of the old array to indices 0-(size - 1);
    */
template <class elmtype>   
void CircularDynamicArray<elmtype>::resizeArray(){
    elmtype* newCDA = new elmtype[cap * 2];
    for(int i = 0; i< size;i++){
        // newCDA[i] = CDArray[(front + i + cap) % cap];
        newCDA[i] = operator[](i);
    }
    delete []CDArray;
    CDArray = newCDA;
    front = 0;
    back = size - 1;
    cap *= 2;
    isReversed = false;
}

/*
This method creates a new array of half the cap of the current CDArray and 
copies the contents of the old array to indices 0-(size - 1);
*/

template <class elmtype>
void CircularDynamicArray<elmtype>::downsizeArray(){
    elmtype* newCDA = new elmtype[cap / 2];
    for(int i = 0; i< size;i++){
        // newCDA[i] = CDArray[(front + i + cap) % cap];
        newCDA[i] = operator[](i);
    }
    delete []CDArray;
    CDArray = newCDA;
    front = 0;
    back = size - 1;
    cap /= 2;
    isReversed = false;
}

/*
This is the standard mergesort algorithm. It breaks down the array into partitions until the are Size one arrays.
It then calls the merge function, which iterates through each subarray, combining them in order into a larger subarray, 
which recurses until the beginning pointer crosses the end.
*/
template <class elmtype>
void CircularDynamicArray<elmtype>::mergeSort(int const begin, int const end){
    //if pointers cross the array is sorted
    if (begin >= end){
        return;
    }

    int mid = begin + (end - begin) / 2;
    mergeSort(begin,mid);
    mergeSort(mid + 1,end);
    merge(begin, mid, end);
}

template <class elmtype>
void CircularDynamicArray<elmtype>::merge(int const begin, int const mid, int const end){
    int const leftSubSize = (mid - begin + 1);
    int const rightSubSize = end - mid;

    elmtype* leftSub = new elmtype[leftSubSize];
    elmtype* rightSub = new elmtype[rightSubSize];

    //put all elements into their respective subarrays
    for(int i = 0; i<leftSubSize;i++){
        leftSub[i] = operator[](begin + i);
    }
    for(int j = 0; j<rightSubSize;j++){
        rightSub[j] = operator[](mid + 1 + j);
    }

    int currLeftInd = 0;
    int currRightInd = 0;
    int currMergedInd = begin;

    //Merge the subarrays, preserving stability and sorting in the process
    while(currLeftInd < leftSubSize && currRightInd < rightSubSize){
        if(leftSub[currLeftInd] < rightSub[currRightInd]){
            operator[](currMergedInd) = leftSub[currLeftInd];
            currLeftInd++;
        }
        else{
            operator[](currMergedInd) = rightSub[currRightInd];
            currRightInd++;
        }
        currMergedInd++;
    }
    //If either subarray is empty, fill the rest of the merged array with the other
    while(currLeftInd < leftSubSize){
        operator[](currMergedInd) = leftSub[currLeftInd];
        currLeftInd++;
        currMergedInd++;
    }
    while(currRightInd < rightSubSize){
        operator[](currMergedInd) = rightSub[currRightInd];
        currRightInd++;
        currMergedInd++;
    }
    delete[] leftSub;
    delete[] rightSub;
}

/*
This uses the quick select algorithm discussed in the class slides.
*/
template <class elmtype>
elmtype CircularDynamicArray<elmtype>::quickSelectAlgo(CircularDynamicArray <elmtype> array, int k){
    
    //Seeds the random number generator to ensure a more random number
    srand((unsigned)time(NULL));
    //Generates a random number
    int random = rand();
    //Turns the random number into an array index and generates the pivot value from the random pivot index
    int pivotIndex = (random % (array.length()));
    elmtype pivot = array[pivotIndex];
    //Create three lists to store the values less than, equal to, and greater than the pivot
    CircularDynamicArray <elmtype> less, equal, greater;
    //Populate the elements of the partition to their proper arrays
    for(int i = 0;i < array.length();i++){
        if(array[i] < pivot)
            less.addEnd(array[i]);
        else if(pivot == array[i])
            equal.addEnd(array[i]);
        else 
            greater.addEnd(array[i]);
    }
    //Determine whether the kth value is this value, is in the lesser partition, or is in the greater than partition
    if(k <= less.length())
        //if in the lesser partition, run quick select on the left partition, searching for the kth element 
        return quickSelectAlgo(less, k);
    else if(k <= equal.length() + less.length())
        return pivot;
    else
        //if in the greater partition, run quick select on the greater partition searching for the (k - number of lesser values - number of equal values)th
        //value
        return quickSelectAlgo(greater, k - less.length() - equal.length());
}

/*
Default constructor
*/
template <class elmtype>
CircularDynamicArray<elmtype>::CircularDynamicArray(){
    CDArray = new elmtype[2];
    size = 0;
    cap = 2;
    front = -1;
    back = -1;
    isReversed = false;
};

/*
Constructor that more or less acts as a standard array with dynamic array abilities. The user can instantly
access all indices in the capacity of the array, but the size can increase.
*/
template <class elmtype>
CircularDynamicArray<elmtype>::CircularDynamicArray(int s){
    CDArray = new elmtype[s];
    size = s;
    cap = s;
    front = 0;
    back = s - 1;
    isReversed = false;
};

/*
Copy Constructor
*/
template <class elmtype>
CircularDynamicArray<elmtype>::CircularDynamicArray(const CircularDynamicArray &old){
    front = old.front;
    back = old.back;
    size = old.size;
    isReversed = old.isReversed;
    cap = old.cap;
    CDArray = new elmtype[cap];
    for(int i = 0; i< old.size;i++){
            CDArray[(front + i + cap) % cap] = old.CDArray[(front + i + cap) % cap];
        }
}

/*
Copy assignment operator
*/
template <class elmtype>
CircularDynamicArray<elmtype>& CircularDynamicArray<elmtype>::operator=(const CircularDynamicArray& old){
    if(this != &old){
        delete[] CDArray;
        front = old.front;
        back = old.back;
        size = old.size;
        isReversed = old.isReversed;
        cap = old.cap;
        CDArray = new elmtype[cap];
        for(int i = 0; i< size;i++){
            CDArray[(front + i + cap) % cap]= old.CDArray[(front + i + cap) % cap];
        }
    }
    return *this;
}

/*
Destructor
*/
template <class elmtype>
CircularDynamicArray<elmtype>::~CircularDynamicArray(){
    delete []CDArray;
};

/*
Bracket operator. The user can access any index in the array from the user perspective. For example, if the array currently had a back index of 
7  and a front index of 10 with capacity 16, CDA[2] = memberArray[12]. I handle the reversal of the array mostly through the use of this operator.
This allows me to write less code for most other methods in the class as they can be used both reversed and not reversed.
*/
template <class elmtype>
elmtype& CircularDynamicArray<elmtype>::operator[](int i){
    if ((i >= (size)) || (i < 0)){
        cout << "Index out of bounds" << endl;
        return error;
    }
    else{
        //cout << "inserting at index: " << (front + i + cap) % cap << endl;
        if(isReversed == true) return *(CDArray + ((back - i + cap) % cap));
        else return CDArray[((front + i + cap) % cap)];
    }
}


/*
This method adds a new element to the front of the circular dynamic array, resizing if necessary.
*/
template <class elmtype>
void CircularDynamicArray<elmtype>::addFront(elmtype v){
    if(size == cap){
        //calls my method to double the size of the array
        resizeArray();
    
    }
    // CDArray[(front - 1 + cap) % cap] = v;       // I prefer the cleaner use of the operator[](int i)
    if(!isReversed){
        //I declare my arrays with front and back at -1 to help with adding the first element and setting front and back to the correct values.
        if(front == -1){
            CDArray[0] = v;
            front = 0;
            back = 0;
            size++;
        }
        else{
            // operator[](cap - 1) = v;                 // Not sure why this didn't work? Should have worked the same way as below
            CDArray[(front + cap - 1 ) % cap] = v;
            size++;
            front = (front - 1 + cap) % cap;
        }
    }
    else{
        if(front == -1){
            CDArray[0] = v;
            front = 0;
            back = 0;
            size++;
        }
        else{
            // operator[](cap - 1) = v;                 // Not sure why this didn't work? Should have worked the same way as below
            CDArray[(back + 1 + cap) % cap] = v;
            size++;
            back = (back + 1 + cap) % cap;
        }
    }
}

/*
This method adds a new element to the end of the circular dynamic array, resizing if necessary.
*/
template <class elmtype>
void CircularDynamicArray<elmtype>::addEnd(elmtype v){
    if(size == cap){
        resizeArray();
    
    }
    // CDArray[(back + 1 + cap) % cap] = v;       // I prefer the cleaner use of the operator[](int i)
    //cout << "back + 1 = " << back + 1 << endl;
    if(!isReversed){
        if(front==-1){
            CDArray[0] = v;
            front = 0;
            back = 0;
            size++;
        }
        else{
            // operator[](back+1) = v;
            CDArray[(back + 1 + cap) % cap] = v;
            back = (back + 1 + cap) % cap;
            size++;
        }
    }
    else{
        if(front == -1){
            CDArray[0] = v;
            front = 0;
            back = 0;
            size++;
        }
        else{
            // operator[](cap - 1) = v;                 // Not sure why this didn't work? Should have worked the same way as below
            CDArray[(front + cap - 1) % cap] = v;
            size++;
            front = (front - 1 + cap) % cap;
        }
    }
}

/*
This method 'deletes' the last element in the array by decreasing array size and decreasing the back index by one.
*/
template <class elmtype>
void CircularDynamicArray<elmtype>::delEnd(){
    if(!isReversed){
        back = (back - 1 + cap) % cap;
        size--;
    }
    else{
        front = (front + 1 + cap) % cap;
        size--;
    }
    if(((float)size)/((float)cap) == .25){
        //downsizeArray halves the array at a quarter full
        downsizeArray();
    }
}

/*
This method 'deletes' the first element in the array by decreasing array size and increasing the front index by one.
*/
template <class elmtype>
void CircularDynamicArray<elmtype>::delFront(){
    if(!isReversed){
        front = (front + 1 + cap) % cap;
        size--;
    }
    else{
        back = (back - 1 + cap) % cap;
        size--;
    }
    if(((float)size)/((float)cap) == .25){
        downsizeArray();
    }
}

/*
This method deallocates any memory currently allocated for the CDA and creates a new array of size 0 and cap 2.
*/
template <class elmtype>
void CircularDynamicArray<elmtype>::clear(){
    size = 0;
    cap = 2;
    isReversed = false;
    front = back = -1;
    delete[] CDArray;
    CDArray = new elmtype[2];
}

/*
This method performs a linear search of the circular dynamic array and returns the index of the first appearance of elmtype e or -1 if it is not found.
*/
template <class elmtype>
int CircularDynamicArray<elmtype>::linearSearch(elmtype e){
    for (int i =0; i < size;i++){
        if (operator[](i) == e){
            return i;
        }
    }
    return -1;
}

/*
This method performs a binary search of the circular dynamic array and returns the index of elmtype e in the array if found, otherwise
it returns -1. Assumes the array is already sorted in ascending order.
*/
template <class elmtype>
int CircularDynamicArray<elmtype>::binSearch(elmtype e){
    int l = 0;
    int r = size - 1;
    int mid;

    while (l<=r){
        mid = (l + r) / 2;
        if(operator[](mid) == e) return mid;
        if(operator[](mid) < e) l = mid + 1;
        if(operator[](mid) > e) r = mid - 1;
    }

    return -1;
}

/*
This method flips the isReversed boolean, thus changing how other functions 'see' the array. This is seen by my bracket operator, which handles most of the trouble.
*/
template <class elmtype>
void CircularDynamicArray<elmtype>::reverse(){
if (isReversed == true){
    isReversed = false;
}
else{
    isReversed = true;
}
}

/*
This method uses the merge sort algorithm to sort the circular dynamic array. Merge sort was chosen as it is both a stable sorting algorithm and
it has a time complexity of O(nlgn).
*/
template <class elmtype>
void CircularDynamicArray<elmtype>::stableSort(){
    mergeSort(0, size - 1);
}

/*
This method returns the kth smallest element in the array using the quickselect algorithm in O(size) time complexity
*/
template <class elmtype>
elmtype CircularDynamicArray<elmtype>::QuickSelect(int k){
    CircularDynamicArray <elmtype> arr;
    for(int i = 0;i<size;i++){
        arr.addEnd(operator[](i));
    }
    return quickSelectAlgo(arr, k);
}

//Getters below
template <class elmtype>
int CircularDynamicArray<elmtype>::capacity(){
    return cap;
}

template <class elmtype>
int CircularDynamicArray<elmtype>::getFront(){
    return front;
}

template <class elmtype>
int CircularDynamicArray<elmtype>::getEnd(){

    return back;
}

template <class elmtype>
int CircularDynamicArray<elmtype>::length(){
    return size;
}