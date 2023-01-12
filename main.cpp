/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhjeon <junhjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 21:11:18 by junhjeon          #+#    #+#             */
/*   Updated: 2022/12/28 16:46:35 by junhjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
#include <iostream>
#include <string>
#include <deque>
#if 0 //CREATE A REAL STL EXAMPLE
	#include <map>
	#include <stack>
	#include <vector>
	namespace ft = std;
#else
	//#include <map.hpp>
	//#include <stack.hpp>
	#include <vector.hpp>
#endif

#include <stdlib.h>

#define MAX_RAM 4294967296
#define BUFFER_SIZE 4096
struct Buffer
{
	int idx;
	char buff[BUFFER_SIZE];
};


#define COUNT (MAX_RAM / (int)sizeof(Buffer))


// template<typename T>
// class MutantStack : public ft::stack<T>
// {
// public:
// 	MutantStack() {}
// 	MutantStack(const MutantStack<T>& src) { *this = src; }
// 	MutantStack<T>& operator=(const MutantStack<T>& rhs) 
// 	{
// 		this->c = rhs.c;
// 		return *this;
// 	}
// 	~MutantStack() {}

// 	typedef typename ft::stack<T>::container_type::iterator iterator;

// 	iterator begin() { return this->c.begin(); }
// 	iterator end() { return this->c.end(); }
// };


int main(int argc, char** argv) {
	if (argc != 2)
	{
		std::cerr << "Usage: ./test seed" << std::endl;
		std::cerr << "Provide a seed please" << std::endl;
		std::cerr << "Count value:" << COUNT << std::endl;
		return 1;
	}
	const int seed = atoi(argv[1]);
	srand(seed);

	ft::vector<std::string> vector_str;
	ft::vector<int> vector_int;
	//ft::stack<int> stack_int;
	ft::vector<Buffer> vector_buffer;
	//ft::stack<Buffer, std::deque<Buffer> > stack_deq_buffer;
	//ft::map<int, int> map_int;
    std::cout << "is working" << std::endl;
	for (int i = 0; i < COUNT; i++)
	{
		vector_buffer.push_back(Buffer());
	}

	for (int i = 0; i < COUNT; i++)
	{
		const int idx = rand() % COUNT;
		vector_buffer[idx].idx = 5;
	}
	ft::vector<Buffer>().swap(vector_buffer);

	try
	{
		for (int i = 0; i < COUNT; i++)
		{
			const int idx = rand() % COUNT;
            vector_buffer.at(idx);
			//std::cerr << "Error: THIS VECTOR SHOULD BE EMPTY!!" <<std::endl;
		}
	}
	catch(const std::exception& e)
	{
        std::cout << "error" << std::endl;
		//NORMAL ! :P
	}
	
    /*
    ///     map test
	for (int i = 0; i < COUNT; ++i)
	{
		map_int.insert(ft::make_pair(rand(), rand()));
	}

	int sum = 0;
	for (int i = 0; i < 10000; i++)
	{
		int access = rand();
		sum += map_int[access];
	}
	std::cout << "should be constant with the same seed: " << sum << std::endl;

	{
		ft::map<int, int> copy = map_int;
	}
	MutantStack<char> iterable_stack;
	for (char letter = 'a'; letter <= 'z'; letter++)
		iterable_stack.push(letter);
	for (MutantStack<char>::iterator it = iterable_stack.begin(); it != iterable_stack.end(); it++)
	{
		std::cout << *it;
	}
    
	std::cout << std::endl;
	return (0);
}
*/


#include "vector.hpp"
#include <iostream>
#include <vector>
#include <algorithm>


int main()
{
    ft::vector<int> temp;
    temp.push_back(1);
    temp.push_back(2);

    ft::vector<int> ft_vector;
    //ft_vector.insert(ft_vector.begin(), temp.begin(), temp.end());
    ft_vector.insert(ft_vector.begin(), 2, 2);
    ft_vector.erase(ft_vector.begin(), ft_vector.end());
	ft_vector.push_back(1);
	//2212 > 1 
    ////////// Capacity & element check
    std::cout << "size : " << ft_vector.size() << std::endl;
    std::cout << "max_size : " << ft_vector.max_size() << std::endl;
    std::cout << "empty : " <<  ft_vector.empty() << std::endl;
    ft_vector.reserve(100);
    std::cout << "capacity : " << ft_vector.capacity() << std::endl;
    std::cout << "at : "  << ft_vector.at(0) << std::endl;
    std::cout << "front : " << ft_vector.front() << std::endl;
    std::cout << "back : " << ft_vector.back() << std::endl;
    ft::vector<int>::iterator it = ft_vector.begin();
    for(it = ft_vector.begin(); it != ft_vector.end(); ++it)
        std::cout << "iterator : " << *it << std::endl;
    std::cout << *std::find(ft_vector.begin(), ft_vector.end(), 2) << std::endl;
    std::cout << "data : " << ft_vector.data() << std::endl;
    std::cout << &(ft_vector.front()) << std::endl;
    std::cout << ft_vector.at(0) << std::endl;
	std::cout << ft_vector.at(1) << std::endl;
    

    std::cout << "========== std vector ==========" << std::endl;
    std::vector<int> std_vector;

    std_vector.push_back(1);
    std_vector.push_back(2); 
    //std_vector.insert(std_vector.begin(), 2 ,2);
    std::cout << "size : " << std_vector.size() << std::endl;
    std::cout << "max_size : " << std_vector.max_size() << std::endl;
    std::cout << "empty : " <<  std_vector.empty() << std::endl;
    //std::cout << "reserve " << std_vector.reserve() << std::endl;
    std::cout << "capacity : " << std_vector.capacity() << std::endl;
    std::cout << "at : "  << std_vector.at(0) << std::endl;
    std::cout << "front : " << std_vector.front() << std::endl;
    std::cout << "back : " << std_vector.back() << std::endl;
    std::vector<int>::iterator it2 = std_vector.begin();
    for(it2 = std_vector.begin(); it2 != std_vector.end(); ++it2)
        std::cout << "iterator : " << *it2 << std::endl;
    std::cout << *std::find(std_vector.begin(), std_vector.end(), 2) << std::endl;
}
