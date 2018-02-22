// CustomAllocators.cpp : �������̨Ӧ�ó������ڵ㡣
//

//#include "stdafx.h"
#include "StackAllocator.h"
#include <iostream>
using namespace std;

int main()
{
	StackAllocator* sa = new StackAllocator(256);

	cout << "initial address: " << sa->startPointer() << endl;

	auto mem1 = sa->alloc(7);

	cout << "marker: " << sa->getMarker() << endl;
    return 0;
}

