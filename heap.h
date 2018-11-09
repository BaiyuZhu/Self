#ifndef HEAP_H
#define HEAP_H
#include <iostream>
#include <functional>
#include <stdexcept>
#include <vector>

template <typename T, typename PComparator = std::less<T> >
class Heap
{
 public:
  /// Constructs an m-ary heap for any m >= 2
  Heap(int m, PComparator c = PComparator());

  /// Destructor as needed
  ~Heap();

  /// Adds an item
  void push(const T& item);

  /// returns the element at the top of the heap 
  ///  max (if max-heap) or min (if min-heap)
  T const & top() const;

  /// Removes the top element
  void pop();

  /// returns true if the heap is empty
  bool empty() const;

  void mini(int node);

  void print();

 private:
  /// Add whatever helper functions and data members you need below
 	std::vector<T> heapvec;
 	int m;
 	PComparator comp;




};

// Add implementation of member functions here
template <typename T, typename PComparator>
Heap<T, PComparator>::Heap(int m, PComparator c)
{
	this->m = m;
	comp = c;
}

template <typename T, typename PComparator>
Heap<T, PComparator>::~Heap()
{}

//A print function for testing.
template <typename T, typename PComparator>
void Heap<T,PComparator>::print()
{
	for(unsigned i = 0; i < heapvec.size(); i++){
		std::cout << " " << heapvec[i];
	}

}

//push a value to the end then move it to the correct position
template <typename T, typename PComparator>
void Heap<T,PComparator>::push(const T& item)
{
	if(empty()){
		heapvec.push_back(item);
		return;
	}else{
		heapvec.push_back(item);
		int curr = heapvec.size()-1;
		while(comp(item, heapvec[(curr-1)/m])){
			T temp = item;
			heapvec[curr] = heapvec[(curr-1)/m];
			heapvec[(curr-1)/m] = temp;
			curr = (curr-1)/m;
		}
	}
	

}

template <typename T, typename PComparator>
bool Heap<T,PComparator>::empty() const
{
	if(heapvec.empty()){
		return true;
	}else{
		return false;
	}
}

// We will start top() for you to handle the case of 
// calling top on an empty heap
template <typename T, typename PComparator>
T const & Heap<T,PComparator>::top() const
{
  // Here we use exceptions to handle the case of trying
  // to access the top element of an empty heap
  if(empty()){
    throw std::logic_error("can't top an empty heap");
  }
  // If we get here we know the heap has at least 1 item
  // Add code to return the top element
  return heapvec[0];
}


// We will start pop() for you to handle the case of 
// calling top on an empty heap
template <typename T, typename PComparator>
void Heap<T,PComparator>::pop()
{
  if(empty()){
    throw std::logic_error("can't pop an empty heap");
  }
  heapvec[0] = heapvec[heapvec.size()-1];
  heapvec.pop_back();
  mini(0);
}

template <typename T, typename PComparator>
void Heap<T,PComparator>::mini(int node)
{
	//recursively running to put the value into correct position.
	int size = heapvec.size();
	if(node*m + 1 >= size){
		return;
	}else{
		T min = heapvec[node*m + 1];
		int minidx = node*m + 1;
		int idx = node*m + 1;
		
		while(idx <= node*m + m && idx < size){
			if(comp(heapvec[idx],min)){
				min = heapvec[idx];
				minidx = idx;
			}
			idx ++;
		}
		if(comp(min, heapvec[node])){
			T temp = heapvec[node];
			heapvec[node] = min;
			heapvec[minidx] = temp;
			mini(minidx);
		}else{
			return;
		}
		
	}
}


#endif

