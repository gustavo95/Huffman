#ifndef HUFFNODE_H
#define HUFFNODE_H

#include <QString>
#include <QBitArray>

typedef unsigned char BYTE;
class HuffNode{
protected:
    BYTE it;
    double wgt;
    bool leaf;
    QBitArray ba;
    HuffNode *left;
    HuffNode *right;

public:

    HuffNode(){
        this->it = 0;
        this->wgt = 0;
        this->left = 0;
        this->right = 0;
        this->leaf = false;
    }

    //construtor de folha
    HuffNode(const BYTE &val, int freq){
        this->it = val;
        this->wgt = freq;
        this->left = 0;
        this->right = 0;
        this->leaf = true;
    }

    //construtor de no interno
    HuffNode(HuffNode *l, HuffNode *r){
        this->it = 0;
        this->wgt = l->weight() + r->weight();
        this->left = l;
        this->right = r;
        this->leaf = false;
    }

    virtual ~HuffNode() {}

    //returna o conteudo do no
    BYTE contet(){
        return this->it;
    }

    //retorna a frequencia
    int weight(){
        return this->wgt;
    }

    //aumenta a frequencia
    void freqPlus(){
        this->wgt++;
    }

    bool isLeaf(){
        return this->leaf;
    }

    void setBitArray(QBitArray b){
        this->ba = b;
    }

    QBitArray getBitArray(){
        return ba;
    }

    void setIt(BYTE c){
        it = c;
        leaf = true;
    }

    void setLeft(HuffNode *l){
        left = l;
    }

    void setRight(HuffNode *r){
        right = r;
    }

    HuffNode* getLeft(){
        return left;
    }

    HuffNode* getRight(){
        return right;
    }

};

#endif // HUFFNODE_H

