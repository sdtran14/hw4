#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"
//#include <stack>
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
* An explicit constructor to initialize the elements by calling the base class constructor
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
		void fixInsert(AVLNode<Key,Value>* r);
		void fixRemove(AVLNode<Key,Value>* r);
		int useRot(AVLNode<Key,Value>* temp);


};

template<class Key, class Value>
void AVLTree<Key, Value>::leftRotate(AVLNode<Key,Value>* n)
{
	AVLNode<Key,Value>* p = n->getParent();
	AVLNode<Key,Value>* y = n->getRight();
	AVLNode<Key,Value>* b = y->getLeft();


	if(b!=NULL) b->setParent(n);
	n->setRight(b);

	y->setParent(p);
	if(p == NULL) BinarySearchTree<Key, Value>::root_ = y;
	else if(n->getKey() < p->getKey()) p->setLeft(y);
	else p->setRight(y);

	n->setParent(y);
	y->setLeft(n);
	
}
template<class Key, class Value>
void AVLTree<Key, Value>::rightRotate(AVLNode<Key,Value>* n)
{
	AVLNode<Key,Value>* p = n->getParent();
	AVLNode<Key,Value>* y = n->getLeft();
	AVLNode<Key,Value>* c = y->getRight();

	if(c!=NULL) c->setParent(n);
	n->setLeft(c);

	y->setParent(p);
	if(p == NULL) BinarySearchTree<Key, Value>::root_ = y;
	else if(n->getKey() < p->getKey()) p->setLeft(y);
	else p->setRight(y);
	
	n->setParent(y);
	y->setRight(n);
	
}

template<class Key, class Value>
void AVLTree<Key, Value>::fixInsert(AVLNode<Key,Value>* r)
{
			AVLNode<Key,Value>* temp = r;
			if(temp==NULL) return;
			while(temp->getParent()!=NULL) 
			{
				if(temp->getKey() < temp->getParent()->getKey())
				{
					temp->getParent()->updateBalance(1);
				} else 
				{
					temp->getParent()->updateBalance(-1);
				}
				int parentBal = temp->getParent()->getBalance();
				if(parentBal==0) return;
				//std::cout<<"s"<<std::endl;
				if(parentBal>1 || parentBal<-1) 
				{
					useRot(temp);
					//std::cout<<"e"<<std::endl;
					return;
				}
				temp = temp->getParent();
			}
			//std::cout<<"e"<<std::endl;
}
template<class Key, class Value>
void AVLTree<Key, Value>::fixRemove(AVLNode<Key,Value>* r)
{
			AVLNode<Key,Value>* temp = r;
			if(temp==NULL) return;
			while(temp->getParent()!=NULL) 
			{
				int parentBal; 
				if(temp == temp->getParent()->getLeft())
				{
					temp->getParent()->updateBalance(-1);
					parentBal = temp->getParent()->getBalance();
					if(parentBal==-1) return;
				} else 
				{
					temp->getParent()->updateBalance(1);
					parentBal = temp->getParent()->getBalance();
					if(parentBal==1) return;
				}
				
				if(parentBal>1) 
				{
					if(useRot(temp->getParent()->getLeft()) == 0) return;
					temp = temp->getParent();
				} else if(parentBal<-1) 
				{
					if(useRot(temp->getParent()->getRight()) == 0) return;
					temp = temp->getParent();
				}
				temp = temp->getParent();
			}
}
template<class Key, class Value>
int AVLTree<Key, Value>::useRot(AVLNode<Key,Value>* temp)
{
				int tempBal = temp->getBalance();
				if(temp->getParent()->getBalance() > 1) 
				{
					if(tempBal<0) 
					{
						int childBal = temp->getRight()->getBalance(); 
						leftRotate(temp);
						temp = temp->getParent();
						rightRotate(temp->getParent());

						temp->setBalance(0);
						if(childBal == 1) temp->getRight()->setBalance(-1);
						else temp->getRight()->setBalance(0);
						if(childBal == -1) temp->getLeft()->setBalance(1);
						else temp->getLeft()->setBalance(0);
					}
					else 
					{
						rightRotate(temp->getParent());
						temp->getRight()->setBalance(1-tempBal);
						temp->setBalance(tempBal-1);
					}
					
				} else if(temp->getParent()->getBalance() < -1) 
				{
					if(tempBal>0) 
					{
						int childBal = temp->getLeft()->getBalance();
						rightRotate(temp);
						temp = temp->getParent();
						leftRotate(temp->getParent());

						temp->setBalance(0);
						if(childBal == 1) temp->getRight()->setBalance(-1);
						else temp->getRight()->setBalance(0);
						if(childBal == -1) temp->getLeft()->setBalance(1);
						else temp->getLeft()->setBalance(0);
						
					} else 
					{
						
						leftRotate(temp->getParent());
						temp->getLeft()->setBalance(-1-tempBal);
						temp->setBalance(tempBal+1);
					}
					
				}
				return tempBal;

				
			
}
/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
 
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
		
    Key key = new_item.first;
		if(this->root_ == NULL) 
		{
			this->root_ = new AVLNode<Key, Value>(key, new_item.second, NULL);
			return;
		}
		AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->root_);
    while(temp!=NULL) 
    {
        Key tkey = temp->getKey();
        if(tkey == key)
        {
            temp->setValue(new_item.second);
						return;
        }
        if(key<tkey) 
        {
            if(temp->getLeft() != NULL) 
						{
							temp = temp->getLeft();
						} 
            else 
            {
                temp->setLeft(new AVLNode<Key, Value>(key, new_item.second, temp));
								temp = temp->getLeft();
                break;
            }
        } 
        else 
        {
            if(temp->getRight() != NULL) 
						{
							temp = temp->getRight();
						}
            else 
            {
                temp->setRight(new AVLNode<Key, Value>(key, new_item.second, temp));
								temp = temp->getRight();
                break;
            }
        }
    }
		
		fixInsert(temp);
		

    // TODO
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{

   AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
		while(temp!=NULL) 
		{
			AVLNode<Key, Value>* r = temp->getRight();
			AVLNode<Key, Value>* l = temp->getLeft();
			AVLNode<Key, Value>* setPar = NULL;
			if(r != NULL && l != NULL) 
			{
				AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(temp));
                
				AVLTree<Key, Value>::nodeSwap(pred, temp);
				continue;
			}
			else if(r != NULL) 
			{
					r->setParent(temp->getParent());
					setPar = r;
					
			} else if(l != NULL) 
			{
					l->setParent(temp->getParent());
					setPar = l;
			}
			if(temp->getParent() !=NULL) 
			{
				
				fixRemove(temp);
				if(temp->getParent()->getLeft() == temp)temp->getParent()->setLeft(setPar);
				else temp->getParent()->setRight(setPar);
				
				
			} else this->root_ = setPar;

			//std::cout<<temp->getParent()->getLeft()<<std::endl;

            delete temp;
            break;
		}

		  // TODO
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
