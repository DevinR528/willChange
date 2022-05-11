
// vim: set tabstop=4 smarttab

#pragma once

#include <memory>

namespace zade {

template<typename Data> struct node {
	Data data;
	std::unique_ptr<node> next;
};

template<typename Data> struct pqueue {
	std::unique_ptr<node<Data>> head;

	pqueue() : head(std::unique_ptr<Data>{}) {}
	~pqueue() { head.~unique_ptr(); }

	void append(Data data);
	Data pop(Data data);
};

bool new_pqueue(pqueue<int>*& queue);

}  // namespace zade
