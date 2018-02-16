#include <iostream>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
  BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize];
}  // LeafNode()



int LeafNode::getMinimum()const
{
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;

} // LeafNode::getMinimum()


LeafNode* LeafNode::insert(int value)
{
  // students must write this
//cout << " count is " << count << endl;
  if(count < leafSize) // If LeafNode has space
  {
//cout << " I am in fisrt condition" << endl;
    values[count] = value;
    count ++;
    sort(); // sorts the LeafNode
    if(value == values[0] && parent)
    {
		//cout << "Bumshakalaka " << endl;
		parent->resetMinimum(this);
	}	
//  cout << " count after addition " << count << endl;
    return NULL;
  }
  // Checks if the Left sibiling is not empty 
  
  
  if(leftSibling && leftSibling->getCount() < leafSize)
  {
//cout << "I am in Leafnode left sib" << endl;
	  leftSibling->insert(getMinimum()); // sets the min from this to leftSib
	  int i = 0;
	  for (; i< count - 1; i++) // frees space so the value can be added on proper spot
	  {
		  values[i] = values[i+1];
	  }
	  count --;
//cout << "Value To be added " << value;
	  insert(value); // inserts the value to the proper space
	  if (parent)
	  {
		  parent->resetMinimum(this);
//cout << " The minimum is (leafnode left sib)" << this->getMinimum() << endl;
	  }

	  return NULL;
  }
  // Checks if the Right sibiling is not empty
  else
  {
	  if (rightSibling && rightSibling->getCount() < leafSize)
	  {
		  addToRight(value);
		  return NULL;
	  }
	  else
	  {
//cout << "split " << endl;
		  return split(value);
	  }
  }
}  // LeafNode::insert()

void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()

//*****Helper Functions*****





//fix function
LeafNode* LeafNode::split(int value)
{
	LeafNode* ptr = new LeafNode(leafSize, parent, this, rightSibling);

	if (rightSibling)
	{
		rightSibling -> setLeftSibling(ptr);
	}
	this -> setRightSibling(ptr);

	int tempCount = leafSize;
	int midPoint = ((leafSize + 1)/2);
	for(;tempCount >= midPoint; tempCount--)
	
	{
//cout << " lol " << value  << " "  <<values[tempCount - 1] << " " << tempCount<< endl;
		
		if(value < values[count - 1] && tempCount == midPoint)
		{
//cout << " lol4" << endl;
			this -> insert(value);
		}

		if(value > values[count - 1] && tempCount == count)// possible error
		{
//cout << " lol2" << endl;
			ptr -> insert(value);
		}
		else
		{
//cout << " lol3 " << count -1 << endl;
//cout << values[count - 1] << endl;
			ptr -> insert(values[count - 1]);
			count--;
		}
	} 
	return ptr;
}//Split



void LeafNode::addToRight(int value)
{

//cout << "Made it in right sib " << " val[] is: " << values[leafSize-1] << endl;
		  //*******FIX THAT FOR 5 MORE POINTS***********************
		  if (values[leafSize - 1] > value)
		  {
			  this->rightSibling->insert(values[leafSize - 1]);
			  count--;
			  this->insert(value);
		  }
		  else 
		  {
			  rightSibling->insert(value);
		  }
		  //******************************************************
		  if (value == values[0] && parent)
			  parent->resetMinimum(this);
}

void LeafNode::sort()
{
	int temp;
	for(int i = 0; i < this->count; i++)
	{
		for(int j = this->count - 1; j > i; j--)
		{
			if(values[j] < values [j-1])
			{
				temp = values[j-1];
				values[j-1] = values[j];
				values[j] = temp;
			}
		}
	}
}// Bubble Sort --> sorts the LeafNodes in proper order

