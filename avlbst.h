#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
		void leftRotate(AVLNode<Key,Value>* n);
		void rightRotate(AVLNode<Key,Value>* n);
		void remove(AVLNode<Key, Value>* temp);
};

template<class Key, class Value>
void AVLTree<Key, Value>::leftRotate(AVLNode<Key,Value>* n) {
	AVLNode<Key, Value>* R = n->getRight();
	//AVLNode<Key, Value>* L = n->getLeft();
	AVLNode<Key, Value>* RL = R->getLeft();
	//AVLNode<Key, Value>* RR = R->getRight();

	if (n->getParent()==NULL) {
		this->root_ = R;
		R->setParent(NULL);
		R->setLeft(n);
		n->setParent(R);
		n->setRight(RL);
		if (RL!=NULL) RL->setParent(n);
	}
	else {
		if (((n->getParent())->getRight())==n) {
			(n->getParent())->setRight(R);
		}
		else if (((n->getParent())->getLeft())==n) {
			(n->getParent())->setLeft(R);
		}
		R->setParent(n->getParent());
		R->setLeft(n);
    n->setParent(R);
    //n->setLeft(L);
    //if(L != NULL) L->setParent(n);
    n->setRight(RL);
    if(RL != NULL) RL->setParent(n);
    //R->setRight(RR);
    //if(RR != NULL) RR->setParent(R);
	}
	//update balance
	int nb = -1 * std::max(0,(int)R->getBalance()) - 1 + n->getBalance();
	int Rb = R->getBalance();
	n->setBalance(nb);
	Rb = std::min(0, (int)n->getBalance()) + Rb -1;
  R->setBalance(Rb);

	return;
}

template<class Key, class Value>
void AVLTree<Key, Value>::rightRotate(AVLNode<Key,Value>* n) {
	//AVLNode<Key, Value>* R = n->getRight();
	AVLNode<Key, Value>* L = n->getLeft();
	//AVLNode<Key, Value>* LL = L->getLeft();
	AVLNode<Key, Value>* LR = L->getRight();
	if (n->getParent()==NULL) {
		this->root_ = L;
		L->setParent(NULL);
		L->setRight(n);
		n->setParent(L);
		n->setLeft(LR);
		if(LR!=NULL) LR->setParent(n);
		//L->setLeft(LL);
		//if (LL!=NULL) LL->setParent(L);
		//n->setRight(R);
    //if(R != NULL) R->setParent(n);
	}
	else {
		if (((n->getParent())->getRight())==n) {
			(n->getParent())->setRight(L);
		}
		else if (((n->getParent())->getLeft())==n) {
			(n->getParent())->setLeft(L);
		}
		L->setParent(n->getParent());
		L->setRight(n);
		n->setParent(L);
		n->setLeft(LR);
		if(LR!=NULL) LR->setParent(n);
		//L->setLeft(LL);
		//if (LL!=NULL) LL->setParent(L);
		//n->setRight(R);
    //if(R != NULL) R->setParent(n);
	}
	//update balances
	int Lb = L->getBalance();
  int nb = n->getBalance() + std::max(0, Lb) - Lb + 1;
	n->setBalance(nb);
	Lb = Lb + 1 + std::max(0, (int)n->getBalance());
  L->setBalance(Lb);

	return;
}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
  //AVLNode<Key, Value>* temp = (AVLNode<Key, Value>*)this->internalFind(new_item.first);
  AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*> (this -> internalFind(new_item.first));
  if (temp != NULL) {//key already exist
    temp ->setValue(new_item.second);
    //no need for balancing
  }
  else if (this->root_ == NULL) {//no existing key & empty tree
    this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
    //no need for balancing
  }
  else {//not empty tree, key not in the tree
    temp = (AVLNode<Key, Value>*)this->root_;
    AVLNode<Key, Value>* temp_parent = temp;
    while (temp != NULL) {
      temp_parent = temp;
      if (new_item.first > (temp->getKey())) {
        temp = temp->getRight();
      }
      else if (new_item.first < (temp->getKey())) {
        temp = temp->getLeft();
      }
    }
    AVLNode<Key, Value>* new_node = new AVLNode<Key, Value>(new_item.first, new_item.second, temp_parent);
    if (new_item.first > (temp_parent->getKey())) {
        temp_parent->setRight(new_node);
    }
    else if (new_item.first < (temp_parent->getKey())) {
        temp_parent->setLeft(new_node);
    }
    //avl balancing
    while (temp_parent!=NULL) {
      if (new_node == temp_parent->getRight()) {
        temp_parent->updateBalance(1);
      }
      else if (new_node == temp_parent->getLeft()) {
        temp_parent->updateBalance(-1);
      }
      if (temp_parent->getBalance()==0) {
        break;
      }
      else if (temp_parent->getBalance()==2) {
        if (new_node->getBalance()==-1) rightRotate(new_node);
        leftRotate(temp_parent);
        break;
      }
      else if (temp_parent->getBalance()==-2) {
        if (new_node->getBalance()==1) leftRotate(new_node);
        rightRotate(temp_parent);
        break;
      }
      new_node = temp_parent;
      temp_parent = temp_parent->getParent();
    }
  }
  return;
}


//helper function
template<typename Key, typename Value>
void AVLTree<Key, Value>::remove(AVLNode<Key, Value>* temp)
{
    // TODO
    if (temp == NULL) return;
    if (temp->getLeft()==NULL && temp->getRight()==NULL) {//leaf node
      AVLNode<Key, Value>* temp_parent = temp->getParent();
      if (temp_parent!=NULL) {//has parent node
        if (temp_parent->getLeft() == temp) {//left child
          temp_parent->setLeft(NULL);
        }
        else if (temp_parent->getRight() == temp){//right child
          temp_parent->setRight(NULL);
        }
      }
      else {//empty tree after removal
        this->root_ = NULL;
      }
      delete temp;
    }
    else if (temp->getLeft()==NULL) {//single child
      AVLNode<Key, Value>* temp_child = temp->getRight();
      AVLNode<Key, Value>* temp_parent = temp->getParent();
      if (temp_parent == NULL) {//no parent node
        temp_child->setParent(NULL);
        this->root_ = temp_child;
      }
      else {//has parent node
        temp_child->setParent(temp_parent);
        if (temp_parent->getLeft()==temp) {//left child
          temp_parent->setLeft(temp_child);
        }
        else if (temp_parent->getRight()==temp){//right child
          temp_parent->setRight(temp_child);
        }
      }
      delete temp;
    }
    else if (temp->getRight()==NULL) {//single child
      AVLNode<Key, Value>* temp_child = temp->getLeft();
      AVLNode<Key, Value>* temp_parent = temp->getParent();
      if (temp_parent==NULL) {
        temp_child->setParent(NULL);
        this->root_ = temp_child;
      }
      else {//has parent node
        temp_child->setParent(temp_parent);
        if (temp_parent->getLeft()==temp) {//left child
          temp_parent->setLeft(temp_child);
          temp_parent->updateBalance(1);
        }
        else if (temp_parent->getRight()==temp){//right child
          temp_parent->setRight(temp_child);
          temp_parent->updateBalance(-1);
        }
      }
      delete temp;
    }
    else {//two children
      AVLNode<Key, Value>* pred = (AVLNode<Key, Value>*)this->predecessor(temp);
      nodeSwap(temp, pred);
      remove(temp);
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    if (this->root_ == NULL) return;
    AVLNode<Key, Value>* temp = (AVLNode<Key, Value>*)this->internalFind(key);
    //AVLNode<Key, Value>* temp_parent = temp->getParent();
    if (temp == NULL) return;
    else remove(temp);
    //balancing
    // while (temp_parent!=NULL) {
    //   if (abs(temp_parent->getBalance()) == 2) {
    //     if (temp_parent->getBalance() == -2) {
    //       if(temp_parent->getLeft()->getBalance() == 1) {
    //         leftRotate(temp_parent->getLeft());
    //       }
    //       rightRotate(temp_parent);
    //     }
    //     else if (temp_parent->getBalance() == 2) {
    //       if(temp_parent->getLeft()->getBalance() == -1) {
    //         rightRotate(temp_parent->getRight());
    //       }
    //       leftRotate(temp_parent);
    //     }
    //     temp_parent = temp_parent->getParent();
    //   }
    //   if (temp_parent->getBalance() == 1) break;
    //   else if (temp_parent->getBalance() == -1) break;
    //   temp = temp_parent;//update
    //   temp_parent = temp_parent->getParent();
    //   if (temp_parent!=NULL && temp==temp_parent->getLeft()) {
    //     temp_parent->updateBalance(+1);
    //   }
    //   else if (temp_parent!=NULL && temp==temp_parent->getRight()) {
    //     temp_parent->updateBalance(-1);
    //   }
    // }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
