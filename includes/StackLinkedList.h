/* Created 02/23/2018
Author: Tianhe Wang
Email: wthvictor@gmail.com
*/

#pragma once

/* This is an explicit linked list implemented for supporting PoolAllocator's free list
   An explicit linked list is better than inplicit linked list here for better controlling
   over nodes' life time.

   To support free list, the linked list behaves like a stack, each time inserting/pushing
   a new node, the new node becomes head. Each time poping out a node from list, the head 
   is the node to pop out
*/

template<class T>
class StackLinkedList {
public:
	struct Node {
		// val can be ignore here if it is only designed for PoolAllocator
		T val;
		Node* next;
		Node(T value) : val(value), next(nullptr) {}
	};

	// Initialize with a new head node
	StackLinkedList(Node* headNode);

	// Add a node to head
	void push(Node* newNode);

	// Pop head as node
	Node* pop();

private:
	Node* head;
};

template<class T>
inline StackLinkedList<T>::StackLinkedList(Node * headNode)
{
	head = headNode;
}

template<class T>
inline void StackLinkedList<T>::push(Node * newNode)
{
	newNode->next = head;
	head = newNode;
}

template<class T>
inline Node * StackLinkedList<T>::pop()
{
	Node* node = head;
	head = head->next;
}
