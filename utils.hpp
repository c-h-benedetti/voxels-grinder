#ifndef UTILS_HPP_INCLUDED
#define UTILS_HPP_INCLUDED

//#include <vector>

template< typename T >
typename std::vector<T>::iterator 
   insert_sorted( std::vector<T> & vec, T const& item )
{
    return vec.insert
        ( 
            std::upper_bound( vec.begin(), vec.end(), item ),
            item 
        );
}

#endif // UTILS_HPP_INCLUDED