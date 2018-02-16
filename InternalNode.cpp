



#include <iostream>
#include "InternalNode.h"

using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
  InternalNode *p, BTreeNode *left, BTreeNode *right) :
  BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode* [ISize];
} // InternalNode::InternalNode()


int InternalNode::getMinimum()const
{
  if(count > 0)   // should always be the case
    return children[0]->getMinimum();
  else
    return 0;
} // InternalNode::getMinimum()


InternalNode* InternalNode::insert(int value)
{
	int position; // correct key position
	for(position = count - 1; position > 0 && keys[position] > value; position--);
	BTreeNode *ptr = children[position] -> insert(value);
	if(!ptr) //equal to null no split needed and empty 
	{
		return NULL;
	}

	if (count < internalSize) // if space for a new keys
	{
		if (count == position + 1) // if the position should be at the end ex 1|4|here
		{
			children[position + 1] = ptr;
			keys[position + 1] = ptr->getMinimum();
			ptr->setParent(this);
			count++;
			if (parent)
			{
				parent->resetMinimum(this);
			}
			return NULL;
		}
		
		else // if the position should be inside ex 1|3|here|8
		{
			for (int i = count - 1; i >= position; i--) //clears space for the entry
			{
				children[i + 1] = children[i];
				keys[i + 1] = keys[i];
			}
		}

		children[position + 1] = ptr;
		keys[position + 1] = ptr -> getMinimum();
		ptr->setParent(this);
		count++;
		if (parent)
		{
			parent->resetMinimum(this);
		}
		return NULL;
	}
	
	if(leftSibling && leftSibling->getCount() < internalSize)
	{
	((InternalNode*)leftSibling) -> insert(children[0]);
	for (int i = 0; i < count; i++) // moves everything to the left one
	{
		children[i] = children[i + 1];
		keys[i] = keys[i + 1];
	}
	count--;

	this->insert(ptr);
	if (parent)
	{
		parent->resetMinimum(this);
	}
	return NULL;
	}	
	else
	{   
		if (rightSibling && rightSibling->getCount() < internalSize)
		{
			if (ptr->getMinimum() < children[internalSize - 1]->getMinimum())
			{
				((InternalNode*)rightSibling)->insert(children[internalSize - 1]);
				count--;
				this->insert(ptr);
			}
			else
			{
				((InternalNode*)rightSibling)->insert(ptr);
			}
			if (parent)
			{
				parent->resetMinimum(this);
			}
			return NULL;
		}
		else
		{
		return split(ptr);
		}	
	}
} // InternalNode::insert()


//change function
void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{ // Node must be the root, and node1
  // students must write this

  children[0] = oldRoot;
  children[1] = node2;
  keys[0] = oldRoot->getMinimum();
  keys[1] = node2->getMinimum();
  count = 2;
  children[0]->setLeftSibling(NULL);
  children[0]->setRightSibling(children[1]);
  children[1]->setLeftSibling(children[0]);
  children[1]->setRightSibling(NULL);
  oldRoot->setParent(this);
  node2->setParent(this);
} // InternalNode::insert()


//Fixed BTree node
void InternalNode::insert(BTreeNode *newNode) // from a sibling
{

	int position; // correct key position
	for (position = count - 1; position > 0 && keys[position - 1] > newNode->getMinimum(); position--);
	
	if (count == position + 1) // if the position should be at the end ex 1|4|here
	{
		children[position + 1] = newNode;
		keys[position + 1] = newNode->getMinimum();
		newNode->setParent(this);
		count++;
		if (parent)
		{
			parent->resetMinimum(this);
		}	
	}

	else // if the position should be inside ex 1|3|here|8
	{
		for (int i = count - 1; i >= position; i--) //clears space for the entry
		{
			children[i + 1] = children[i];
			keys[i + 1] = keys[i];
		}
		children[position] = newNode;
		keys[position] = newNode->getMinimum();
		newNode->setParent(this);
		count++;
		if (parent)
		{
			parent->resetMinimum(this);
		}
	}

} // InternalNode::insert()



void InternalNode::print(Queue <BTreeNode*> &queue)
{
  int i;

  cout << "Internal: ";
  for (i = 0; i < count; i++)
    cout << keys[i] << ' ';
  cout << endl;

  for(i = 0; i < count; i++)
    queue.enqueue(children[i]);

} // InternalNode::print()

//*****Helper Functions*****

//Reset minValue function
//change function
void InternalNode::resetMinimum(const BTreeNode* child)
{
	for(int i = 0; i < count; i++)
	{
		if(children[i] == child)
		{
			keys[i] = children[i] -> getMinimum();
			if(/*i == 0 && */parent)
			{
				parent -> resetMinimum(this);
			}
		}
	}// the problem is that it doesn't reset the internal node on the right
}//InternalNode::resetMinimum()




InternalNode* InternalNode::split(BTreeNode *ptr)
{
	BTreeNode* last = sort(ptr);
	cout << "The min in last is: " << last->getMinimum() << endl;
	InternalNode* tempPtr = new InternalNode(internalSize, leafSize, parent, this, rightSibling);
	//Fixed that  tempPtr instead of ptr
	if (rightSibling)
	{
		rightSibling->setLeftSibling(tempPtr);
	}
	rightSibling = tempPtr;

	//*******************************************
	int	midPoint = ((internalSize + 1) / 2);

	int tempCount = midPoint;
	for (; internalSize > tempCount; tempCount++)
	{
		//cout << " YOLO2 " << "tempCount is : " << tempCount <<  "The key at tempCOunt :  " << keys[secondCount] <<endl;
		// ***********************FIX THAT FOR FIVE POOINTS**************************
		tempPtr->keys[tempPtr->count] = keys[tempCount];
		tempPtr->children[tempPtr->count++] = children[tempCount];
		//I just fixed that
		children[tempCount]->setParent(tempPtr);
		count--;
		//****************************************************** + creat secondCounter
	}
	tempPtr->keys[tempPtr->count] = last->getMinimum();
	tempPtr->children[tempPtr->count++] = last;
	//I just fixed that
	last->setParent(tempPtr);
	return tempPtr;
}//Split





BTreeNode* InternalNode::sort(BTreeNode *ptr)
{
	int pos;
	for (pos = count - 1; pos > 0 && keys[pos -1] > ptr->getMinimum(); pos--);
	if (pos == internalSize - 1 && keys[count - 1] < ptr->getMinimum())
		return ptr;

	BTreeNode *last = children[count - 1];

	for (int i = count - 2; i >= pos; i--)
	{
		children[i + 1] = children[i];
		keys[i + 1] = keys[i];
	} // shift things to right to make room for ptr, i can be -1!

	children[pos] = ptr;  // i will end up being the position that it is inserted
	keys[pos] = ptr->getMinimum();
	ptr->setParent(this);
	return last;

}//InternalNode::sort()
