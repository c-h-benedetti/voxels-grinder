#ifndef FILTER_HPP_INCLUDED
#define FILTER_HPP_INCLUDED

// Struct helping accepting only certain types for processing
struct FilterAcceptedTypes {
    template <typename T>
    static bool isHandled() { return false; }
};

// We only want to handle ints and floats.

template <>
bool FilterAcceptedTypes::isHandled<int>() { return true; }

template <>
bool FilterAcceptedTypes::isHandled<float>() { return true; }

#endif // FILTER_HPP_INCLUDED