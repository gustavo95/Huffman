#ifndef HUFFTREE_H
#define HUFFTREE_H

#include "HuffNode.h"
#include "AList.h"
#include "AStack.h"

#include <iostream>
#include <QString>

typedef unsigned char BYTE;
using namespace std;
class HuffTree{

private:

    HuffTree(const HuffTree&){

    }

    HuffNode *root;

    AList *compDictionary;

    string tree;

public:

    HuffTree(){

    }

    ~HuffTree(){

    }

    //constroi a arvore
    void buildHuffTree(AList *list){
        compDictionary = new AList(256);

        if(list->length() == 1){
            list->moveToStart();
            HuffNode *aux1 = new HuffNode;
            *aux1 = list->getValue();
            list->remove();
            root = aux1;

             QBitArray ba;
             ba.resize(1);
             ba.setBit(0,true);

             root->setBitArray(ba);
             compDictionary->append(*root);

        }
        else{
            list->bubble();

            while(list->length() > 1){
                list->moveToStart();
                HuffNode *aux1 = new HuffNode;
                *aux1 = list->getValue();
                list->remove();
                HuffNode *aux2 = new HuffNode;
                *aux2 = list->getValue();
                list->remove();

                HuffNode *newNode = new HuffNode(aux1,aux2);
                root = newNode;
                list->append(*newNode);
                list->bubble();
            }

            QBitArray ba;

            compact(root, ba);
        }
    }

    //constroi o dicionario com a codificacao dos BYTEs
    void compact(HuffNode *node, QBitArray ba){
        if(node->isLeaf()){
            node->setBitArray(ba);
            compDictionary->append(*node);
        }
        else{
            int size = ba.size();
            ba.resize(size + 1);

            ba.setBit(size, false);
            compact(node->getLeft(), ba);

            ba.setBit(size, true);
            compact(node->getRight(), ba);
        }
    }

    AList* getDictionary(){
        return compDictionary;
    }

    HuffNode* getRoot(){
        return root;
    }

    //constroi a representacao da arvore
    void treeRepresentation(HuffNode *node){
        if(root->isLeaf()){
            tree += 0x28; //(
            addBYTEonTree(root->contet());
        }
        else{
            if(node->isLeaf()){
                addBYTEonTree(node->contet());
            }
            else{
                tree += 0x28; //(
                treeRepresentation(node->getLeft());
                treeRepresentation(node->getRight());
            }
        }
    }

    //coloca um BYTE na representacao da arvore
    void addBYTEonTree(BYTE c){
        //      @            (
        if(c == 0x40 || c == 0x28){
            tree += 0x40; //@
            tree += c;
        }
        else{
           tree += c;
        }
    }

    string getTree(){
        return tree;
    }

    //reconstroi a arvore
    void rebuildTree(string treeRepr){
       if(treeRepr.length() == 1){
           HuffNode *aux = new HuffNode(treeRepr.at(0), 0);
           root = aux;
       }
       else if(treeRepr.length() == 2 && treeRepr.at(0) == 0x40){
           HuffNode *aux = new HuffNode(treeRepr.at(1), 0);
           root = aux;
       }
       else{
           AStack *stack = new AStack(512);
           root = new HuffNode();

           HuffNode *aux;
           HuffNode *auxL = new HuffNode();
           HuffNode *auxR = new HuffNode();

           root->setLeft(auxL);
           root->setRight(auxR);

           stack->push(auxR);
           stack->push(auxL);

           for(int i = 0; i < treeRepr.size(); i++){
               if(treeRepr[i] == 0x28){
                   aux = stack->pop();

                   auxL = new HuffNode();
                   auxR = new HuffNode();

                   aux->setLeft(auxL);
                   aux->setRight(auxR);

                   stack->push(auxR);
                   stack->push(auxL);
               }
               else{
                   if(treeRepr[i] == 0x40){
                       i++;
                   }
                   aux = stack->pop();
                   aux->setIt(treeRepr[i]);
               }
           }
       }
    }

};

#endif // HUFFTREE_H

