#ifndef ALIST_H
#define ALIST_H

#include "iostream"
#include "HuffNode.h"

using namespace std;
typedef unsigned char BYTE;
class AList{
private:

    int maxSize; // Maximum size of list
    int listSize; // Number of list items now
    int curr; // Position of current element
    HuffNode* listArray; // Array holding list elements

public:

    AList(int size) { // Constructor
        maxSize = size;
        listSize = curr = 0;
        listArray = new HuffNode[maxSize];
    }

    ~AList() { delete [] listArray; } // Destructor

    void clear() { // Reinitialize the list
        delete [] listArray; // Remove the array
        listSize = curr = 0; // Reset the size
        listArray = new HuffNode[maxSize]; // Recreate array
    }

    // Insert "it" at current position
    void insert(const HuffNode& it) {

        if(listSize < maxSize){
            for(int i=listSize; i>curr; i--) // Shift elements up
                listArray[i] = listArray[i-1]; // to make room
            listArray[curr] = it;
            listSize++; // Increment list size
        }else{

            cout << "ArrayList capacity exceeded" << endl;
        }
    }

    void append(const HuffNode& it) { // Append "it"
        if(listSize < maxSize){
            listArray[listSize++] = it;
        }else{
            cout << "List capacity exceeded" << endl;
        }
    }

    // Remove and return the current element.
    HuffNode remove() {
        if((curr>=0) && (curr < listSize)){
            HuffNode it = listArray[curr]; // Copy the element
            for(int i=curr; i<listSize-1; i++) // Shift them down
                listArray[i] = listArray[i+1];
            listSize--; // Decrement size
            return it;
        }else{
            cout << "No element" << endl;
        }
    }

    void moveToStart() { curr = 0; } // Reset position

    void moveToEnd() { curr = listSize; } // Set at end

    void prev() { if (curr != 0) curr--; } // Back up

    void next() { if (curr < listSize) curr++; } // Next

    // Return list size
    int length() const { return listSize; }

    // Return current position
    int currPos() const { return curr; }

    // Set current list position to "pos"
    void moveToPos(int pos) {
        if((pos>=0)&&(pos<=listSize)){
            curr = pos;
        }else{
            cout << "Pos out of range" << endl;
        }
    }

    const HuffNode& getValue() const { // Return current element
        if((curr>=0)&&(curr<listSize)){
            return listArray[curr];
        }else{
            cout << "No current element" << endl;
        }

    }

    void bubble(){
        HuffNode aux;
        for(int i = listSize-1; i >= 1; i--) {
            for(int j = 0; j < i ; j++) {
                if(listArray[j].weight() > listArray[j+1].weight()) {
                    aux = listArray[j];
                    listArray[j] = listArray[j+1];
                    listArray[j+1] = aux;
                }
                else if(listArray[j].weight() == listArray[j+1].weight() && listArray[j].contet() > listArray[j+1].contet()){
                    aux = listArray[j];
                    listArray[j] = listArray[j+1];
                    listArray[j+1] = aux;
                }
            }
        }
    }
};


#endif // ALIST_H
