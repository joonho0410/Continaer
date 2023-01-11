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

#include "Vector.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
    //ft::vector<int> a(2, 2);
    //std::vector<int> b(2, 2);

    ft::vector<int> a;

    std::cout << a.capacity() << std::endl;
    a.push_back(1);
     std::cout << a.capacity() << std::endl;
    a.push_back(1);
     std::cout << a.capacity() << std::endl;
    a.push_back(1);
     std::cout << a.capacity() << std::endl;
    
    a.insert(a.begin(), 5);
    ft::vector<int>::iterator it = a.begin();
    
    for(;it != a.end(); ++it)
        std::cout << *it << std::endl;
    






    /*
    ////////// Capacity & element check
    std::cout << "size : " << a.size() << std::endl;
    std::cout << "max_size : " << a.max_size() << std::endl;
    std::cout << "empty : " <<  a.empty() << std::endl;
    //std::cout << "reserve " << a.reserve() << std::endl;
    std::cout << "capacity : " << a.capacity() << std::endl;
    //std::cout << "at : "  << a.at(1) << std::endl;
    std::cout << "front : " << a.front() << std::endl;
    std::cout << "back : " << a.back() << std::endl;
    ft::vector<int>::iterator it = a.begin();
    for(it = a.begin(); it != a.end(); ++it)
        std::cout << "iterator : " << *it << std::endl;
    std::cout << *std::find(a.begin(), a.end(), 2) << std::endl;
    std::cout << "data : " << a.data() << std::endl;
    std::cout << &(a.front()) << std::endl;
    std::cout << a[0] << a[1] << std::endl;
    
    std::cout << "========== std vector ==========" << std::endl;
    std::cout << "size : " << b.size() << std::endl;
    std::cout << "max_size : " << b.max_size() << std::endl;
    std::cout << "empty : " <<  b.empty() << std::endl;
    //std::cout << "reserve " << a.reserve() << std::endl;
    std::cout << "capacity : " << b.capacity() << std::endl;
    std::cout << "at : "  << b.at(0) << std::endl;
    std::cout << "front : " << b.front() << std::endl;
    std::cout << "back : " << b.back() << std::endl;
    std::vector<int>::iterator it2 = b.begin();
    for(it2 = b.begin(); it2 != b.end(); ++it2)
        std::cout << "iterator : " << *it2 << std::endl;
    std::cout << *std::find(b.begin(), b.end(), 2) << std::endl;
    */   
}