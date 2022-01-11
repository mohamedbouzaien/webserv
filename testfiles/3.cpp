#include <iostream>
#include <string>
#include <deque>
#if 1//CREATE A REAL STL EXAMPLE
	#include <map>
	#include <stack>
	#include <map>
	namespace ft = std;
#else
	#include "../map.hpp"
	#include "../stack.hpp"
	#include "../map.hpp"
#endif

#include <stdlib.h>

template<class T>
void	print_map(T& m) {
	typename T::const_iterator it = m.begin();
	typename T::const_iterator ite = m.end();
	std::cout << "MAP CONTENT :" << std::endl;
	for(; it != ite; it++)
		std::cout << it->first  << ", " << it->second << std::endl;
	std::cout << "----------" << std::endl;
	std::cout << "MAP SIZE :" << std::endl;
	std::cout << m.size() << std::endl;
	std::cout << "MAP MAX SIZE :" << std::endl;
	std::cout << m.max_size() << std::endl;
	std::cout << "IS EMPTY ? ";
	if (m.empty())
		std::cout << "YES" << std::endl;
	else
		std::cout << "NO" << std::endl;
	std::cout << "----------" << std::endl;
}

void	map_constructor() {
	std::cout << "\x1B[32mCONSTRUCTORS TEST \033[0m" << std::endl;
	std::cout << "EMPTY CONSTRUCTOR :" << std::endl;
	ft::map<int, int> m1;
	print_map(m1);
	std::cout << "----------" << std::endl;
	std::cout << "RANGE CONSTRUCTOR :" << std::endl;
	for (int i = 0; i < 10; i++)
		m1[i] = i + 10;
	ft::map<int, int> m2(m1.begin(), m1.end());
	print_map(m2);
	std::cout << "----------" << std::endl;
	std::cout << "COPY CONSTRUCTOR :" << std::endl;
	ft::map<int, int> m3(m2);
	print_map(m3);
	std::cout << "----------" << std::endl;
}

void	map_iterator() {

	std::cout << "\x1B[32mITERATORS TEST \033[0m" << std::endl;
	ft::map<int, int> m;
	for (int i = 0; i < 10; i++)
		m[i] = i + 10;
	std::cout << "ITERATOR TEST :" << std::endl;
	ft::map<int, int>::const_iterator it = m.begin();
	ft::map<int, int>::const_iterator ite = m.end();
	for(; it != ite; it++)
		std::cout << it->first << ", " << it->second << std::endl;
	std::cout << "REVERSE ITERATOR TEST :" << std::endl;
	ft::map<int, int>::const_reverse_iterator rit = m.rbegin();
	ft::map<int, int>::const_reverse_iterator rite = m.rend();
	for(; rit != rite; rit++)
		std::cout << (*rit).first << ", " << (*rit).second << std::endl;
	std::cout << "CHECK MAP CONTENT:" << std::endl;
	print_map(m);
}

void	map_capacity() {
	ft::map<int, int> m;
	ft::map<int, int> m1;
	for (int i = 0; i < 10; i++)
		m1[i] = i + 10;
	std::cout << "\x1B[32mCAPACITY TEST \033[0m" << std::endl;
	std::cout << "INITIALY EMPTY MAP : " << std::endl;
	print_map(m);
	std::cout << "INITIALY NO EMPTY MAP : " << std::endl;
	print_map(m1);
	print_map(m1);
}

void	map_access() {
	ft::map<int, int> m;
	for (int i = 0; i < 10; i++)
		m[i] = i + 10;
	std::cout << "\x1B[32mACCESS TEST \033[0m" << std::endl;
	std::cout << "ACCESS OPERATOR [] :" << std::endl;
	for(unsigned long i = 0; i < m.size(); i++)
		std::cout << m[i] << std::endl;
	m[0] = 42;
	std::cout << "AFTER MODIFICATION W/ []" << std::endl;
	for(unsigned long i = 0; i < m.size(); i++)
		std::cout << m[i] << std::endl;
}

void	map_modifiers() {
	ft::map<int, int> m;
	for (int i = 0; i < 10; i++)
		m[i] = i + 10;
	ft::map<int, int>m1;
	std::cout << "\x1B[32mMODIFIERS TEST \033[0m" << std::endl;
	std::cout << "INSERT TEST : " << std::endl;
	std::cout << "TEST 1 : WITH HINT" << std::endl;
	m.insert(m.begin(), ft::make_pair(42, 21));
	print_map(m1);
	std::cout << "TEST 2 : SINGLE ELEMENT" << std::endl;
	m.insert(ft::make_pair(21, 42));
	print_map(m1);
	std::cout << "TEST 3 : RANGE" << std::endl;
	m1.insert(++m.begin(), --m.end());
	print_map(m1);
	std::cout << "ERASE TEST" << std::endl;
	std::cout << "TEST 1 : SINGLE ELEMENT BY KEY" << std::endl;
	m1.erase(3);
	std::cout << "TEST 1 : SINGLE ELEMENT BY IT" << std::endl;
	m1.erase(m1.begin());
	print_map(m1);
	std::cout << "TEST 2 : RANGE" << std::endl;
	m1.erase(m1.begin(), m1.end());
	print_map(m1);
	std::cout << "SWAP TEST : " << std::endl;
	std::cout << "V BEFORE SWAP : " << std::endl;
	print_map(m);
	std::cout << "M1 BEFORE SWAP : " << std::endl;
	print_map(m1);
	m1.swap(m);
	std::cout << "V AFTER SWAP : " << std::endl;
	print_map(m);
	std::cout << "M1 AFTER SWAP : " << std::endl;
	print_map(m1);
	std::cout << "CLEAR TEST : " << std::endl;
	m1.clear();
	print_map(m1);
}

void	map_observer() {
	ft::map<int, int> m;
	ft::map<int, int> m1(m.key_comp());

	std::cout << "\x1B[32mOBSERVER TEST \033[0m" << std::endl;
	if (typeid(m.key_comp()) == typeid(m1.key_comp()))
		std::cout << "KEY COMPARATORS ARE SAME" << std::endl;
	else
		std::cout << "FAILURE" << std::endl;
	if (typeid(m.value_comp()) == typeid(m1.value_comp()))
		std::cout << "VALUE COMPARATORS ARE SAME" << std::endl;
	else
		std::cout << "FAILURE" << std::endl;
}

void	map_operations() {
	ft::map<int, int> m;
	for (int i = 0; i < 10; i++)
		m[i] = i + 10;
	std::cout << "\x1B[32mOPERATIONS TEST \033[0m" << std::endl;
	std::cout << "FIND TEST : " << std::endl;
	std::cout << "Try to find 5 ...";
	ft::map<int, int>::const_iterator it = m.find(5);
	ft::map<int, int>::const_iterator ite = m.end();
	if (it != ite)
	{
		std::cout << "Find ! That is the next : " << std::endl; 
		for(; it != ite; it++)
			std::cout << it->first << ", " << it->second << std::endl;
	}
	else
		std::cout << "NOT FOUND" << std::endl;
	it = m.find(42);
	if (it != ite)
	{
		std::cout << "Find ! That is the next : " << std::endl; 
		for(; it != ite; it++)
			std::cout << it->first << ", " << it->second << std::endl;
	}
	else
		std::cout << "NOT FOUND" << std::endl;
	std::cout << "COUNT TEST :" << std::endl;
	std::cout << "In M, 5 appears " << m.count(5) << ", and 42 apperas " << m.count(42) << std::endl;
	std::cout << "LOWER BOUND TEST :" << std::endl;

	std::cout << "TRY WITH 5 :" << std::endl;
	it = m.lower_bound(5);
	for (; it != ite; it++)
		std::cout << it->first << ", " << it->second << std::endl;
	std::cout << "UPPER BOUND TEST :" << std::endl;

	std::cout << "TRY WITH 8 :" << std::endl;
	it = m.lower_bound(8);
	for (; it != ite; it++)
		std::cout << it->first << ", " << it->second << std::endl;
	std::cout << "UPPER BOUND TEST :" << std::endl;
	std::cout << "TRY WITH 42 :" << std::endl;
	ft::pair<ft::map<int, int>::iterator, ft::map<int, int>::iterator > range = m.equal_range(42);
	for (; range.first != range.second; range.first++)
		std::cout << range.first->first << ", " << range.second->second << std::endl;
}


void	map_allocator() {
	ft::map<int, int> m;
	ft::map<int, int> m1(m.key_comp(), m.get_allocator());

	std::cout << "\x1B[32mALLOCATOR TEST \033[0m" << std::endl;
	if (typeid(m.get_allocator()) == typeid(m1.get_allocator()))
		std::cout << "ALLOCATORS ARE SAME" << std::endl;
	else
		std::cout << "FAILURE" << std::endl;
}

void	map_operators() {
	ft::map<int, int> m;
	ft::map<int, int> m1(m);
	ft::map<int, int> m2;
	for (int i = 0; i < 10; i++)
		m2[i] = i + 10;

	std::cout << "\x1B[32mOPERATOR TEST \033[0m" << std::endl;
	if (m == m1)
		std::cout << "V && M1 ARE SAME" << std::endl;
	if (m != m2)
		std::cout << "V && M2 ARE DIFFERENTS" << std::endl;
	if (m < m2)
		std::cout << "V IS LESS THAN M2" << std::endl;
	if (m2 > m1)
		std::cout << "M2 IS MORE THAN M1" << std::endl;
	if (m1 >= m1)
		std::cout << "V IS MORE OR EQUAL TO M1" << std::endl;
}

void	map_non_member_swap() {
	ft::map<int, int> m;
	ft::map<int, int> m1;
	for (int i = 0; i < 10; i++)
		m1[i] = i + 10;
	std::cout << "\x1B[32mNON MEMBER SWAP TEST \033[0m" << std::endl;
	std::cout << "V BEFORE SWAP : " << std::endl;
	print_map(m);
	std::cout << "M1 BEFORE SWAP : " << std::endl;
	print_map(m1);
	ft::swap(m, m1);
	std::cout << "V AFTER SWAP : " << std::endl;
	print_map(m);
	std::cout << "M1 AFTER SWAP : " << std::endl;
	print_map(m1);
}

int		main() {

	map_constructor();
	map_iterator();
	map_capacity();
	map_access();
	map_modifiers();
	map_allocator();
	map_observer();
	map_operations();
	map_operators();
	map_non_member_swap();
}
