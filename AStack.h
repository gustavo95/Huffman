#ifndef ASTACK_H
#define ASTACK_H

#include "HuffNode.h"

// Array-based stack implementation
class AStack{

private:
    int maxSize; // Maximum size of stack
    int top; // Index for top element
    HuffNode **listArray; // Array holding stack elements

public:
    AStack(int size) // Constructor
    { maxSize = size; top = 0; listArray = new HuffNode*[size]; }

    ~AStack() { delete [] listArray; } // Destructor

    void clear() { top = 0; } // Reinitialize

    void push(HuffNode *it) { // Put "it" on stack
        if(top != maxSize){
            listArray[top++] = it;
        }else{
            cout << "Stack is full" << endl;
        }
    }

    HuffNode* pop() { // Pop top element
        if(top != 0){
            return listArray[--top];
        }else{
            cout << "Stack is empty" << endl;
        }
    }

    const HuffNode* topValue() const { // Return top element
        if(top != 0){
            return listArray[top-1];
        }else{
            cout << "Stack is empty" << endl;
        }
    }

    int length() const { return top; } // Return length
};


#endif // ASTACK_H
