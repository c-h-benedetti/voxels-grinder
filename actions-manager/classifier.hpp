#ifndef CLASSIFIER_HPP_INCLUDED
#define CLASSIFIER_HPP_INCLUDED

#include <initializer_list> // To build the Path object which is based on a vector
#include <vector> // Base of Path
#include <map> // Base of Node


namespace classifier {


// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
// #   OBJECT ALLOWING TO SPECIFY A PATH TO FOLLOW IN A TREE, CONTAINING POTENTIALLY THE FINAL OBJECT                #
// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

template <typename T>
class Path : public std::vector<T> { // The vector represents a path through the categories of a tree

    T object{}; // This object represents the key of an object (leaf) stored in a tree.

public:

    inline void setObject(const T& t) { this->object = t; }
    inline const T& getObject() const { return this->object; }

    virtual bool hasObject() const = 0;

    Path() = default;
    // Path(const T& t): object(t){}
    Path(const std::initializer_list<T>& i): std::vector<T>(i){}
    Path(const std::initializer_list<T>& i, const T& t): std::vector<T>(i), object(t){}

    Path(std::vector<T>&& i): std::vector<T>(std::move(i)){}
    Path(std::vector<T>&& i, const T& t): std::vector<T>(std::move(i)), object(t){}
};



// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
// #   NODE REPRESENTING A LAYER OF THE TREE                                                                         #
// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

// A: Key used to designate a sub-category or an object in one of the maps representing tree's nodes.
// B: Type of stored object.

template <typename A, typename B>
class Node : protected std::map<A, Node<A, B>> {

    std::map<A, B> objects;

public:

    auto addSubCategory(const A& c) {
        return this->insert({c, Node<A, B>{}}).first;
    }

    auto addObject(const A& a, const B& b) {
        return this->objects.insert({a, b}).first;
    }

    const std::map<A, Node<A, B>>& getMappedCategories() const { return dynamic_cast<std::map<A, Node<A, B>>&>(*this); }

    const std::map<A, B>& getMappedObjects() const { return this->objects; }

    void objectsList(std::vector<A>& v) {
        v.clear();
        v.reserve(this->objects.size());
        for (const auto& item : this->objects) {
            v.push_back(item.first);
        }
    }

    void subCategoriesList(std::vector<A>& v) {
        v.clear();
        v.reserve(this->size());
        for (const auto& item : *this) {
            v.push_back(item.first);
        }
    }

    Node<A, B>* getSubCategory(const A& a) {
        auto it = this->find(a);
        if (it == this->end()) { return nullptr; }
        return &it->second;
    }

    B* getObject(const A& a) {
        auto it = this->objects.find(a);
        if (it == this->objects.end()) { return nullptr; }
        return &it->second;
    }

    template <typename U, typename K>
    friend class Tree;
};


template <typename A, typename B>
class Tree {

    Node<A, B> root{};

private:

    // Browses the tree and creates the missing nodes to create the specified path.
    Node<A, B>* createCategoriesPath(const Path<A>& p) {
        Node<A, B>* current = this->getRoot();
        for (const A& a : p) {
            auto it = current->find(a);
            if (it == current->end()) {
                it = current->addSubCategory(a);
            }
            current = &(it->second);
        }
        return current;
    }

public:

    inline Node<A, B>* getRoot() { return &(this->root); }

    // Returns the pointer to a category by following the Path p.
    Node<A, B>* getCategory(const Path<A>& p) {
        Node<A, B>* current = this->getRoot();
        for (const A& a : p) {
            auto it = current->find(a);
            if (it == current->end()) {
                current = nullptr;
                break;
            }
            current = &(it->second);
        }
        return current;
    }

    // Returns an object by following the Path p.
    B* getObject(const Path<A>& p) {
        Node<A, B>* node = this->getCategory(p);
        if (!node) { return nullptr; }
        return node->getObject(p.getObject());
    }

    // Add a new object after following the provided path.
    int addObject(const Path<A>& p, const B& b) {
        Node<A, B>* node = this->createCategoriesPath(p);
        if (!node) { return -1; }
        return node->addObject(p.getObject(), b) == node->getMappedObjects().end();
    }
};

};

#endif // CLASSIFIER_HPP_INCLUDED
