#ifndef GENERAL_UTILS_HPP_INCLUDED
#define GENERAL_UTILS_HPP_INCLUDED

#ifdef DEBUG_MODE
    #define ACCESS_MODIFIER public
    #include <iostream>
#else
    #define ACCESS_MODIFIER private
#endif

inline bool b_equality(auto t1, auto t2) { return !(t1 ^ t2); }

/**
 * Function allowing to print something only if we are in debug mode.
 * It has the advantage not to require a condition check.
 */

#ifdef DEBUG_MODE

#define DEBUG_PRINT(...) \
do { std::cerr << #__VA_ARGS__ << " = "; debugPrint(__VA_ARGS__); } while(0)

// Une fonction récursive qui imprime les arguments
template<typename First, typename... Rest>
void debugPrint(const First& first, const Rest&... rest) {
    std::cout << first;
    if constexpr (sizeof...(rest) != 0) {
        std::cout << ", ";
        debugPrint(rest...); // Appel récursif avec le reste des arguments
    } else {
        std::cout << std::endl;
    }
}

#else

// Si DEBUG n'est pas défini, la macro ne fait rien
#define DEBUG_PRINT(...) 

#endif

#define N_THREADS 8

#endif // GENERAL_UTILS_HPP_INCLUDED