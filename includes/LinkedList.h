/* Created 02/26/2018
Author: Tianhe Wang
Email: wthvictor@gmail.com
*/

#pragma once

#include <assert.h>

/* This is an explicit linked list implemented for supporting FreeListAllocator's free list
An explicit linked list is better than inplicit linked list here for better controlling
over nodes' life time.
*/

template<class T>
class LinkedList {
public:
	struct Node{
		T val;
		Node* next;
		Node(T value) : val(value), next(nullptr) {}
	};

	LinkedList();

	LinkedList(Node* headNode);

	// No need to have destructor, because it is FreeListAllocator that handles its lifetimes

	void insert(Node* prevNode, Node* newNode);

	void remove(Node* prevNode, Node* deleteNode);

	Node* head;
};

template<class T>
inline LinkedList<T>::LinkedList()
{
	head = nullptr;
}

template<class T>
inline LinkedList<T>::LinkedList(Node * headNode)
{
	head = headNode;
}

template<class T>
void LinkedList<T>::insert(Node * prevNode, Node * newNode)
{
	// newNode is the head of linkedlist
	if (!prevNode) {
		newNode->next = head;
		head = newNode;
	}
	else {
		// this is the last node of linkedlist
		if (prevNode->next == nullptr) {
			prevNode->next = newNode;
			newNode->next = nullptr;
		}
		else {
			newNode->next = prevNode->next;
			prevNode->next = newNode;
		}
	}
}

template<class T>
void LinkedList<T>::remove(Node * prevNode, Node * deleteNode)
{
	// delete node shouldn't be nullptr
	assert(deleteNode != nullptr);

	if (!prevNode) {
		head = deleteNode->next;
	}
	else {
		prevNode->next = deleteNode->next;
	}
}
