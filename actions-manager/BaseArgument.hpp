#ifndef VFS_ARGUMENTS_BASE_HPP_INCLUDED
#define VFS_ARGUMENTS_BASE_HPP_INCLUDED

#include <string>
#include <functional>
#include "stack-segments/ID.hpp"
#include "Receiver.hpp"

#define SIZE_OF_DESCRIPTION 256
#define SIZE_OF_CATEGORY 16

using Name        = std::string;
using Description = std::string;

//####################################################################################################

/**
 * Textual representation of the type of this class to be identified by a client app.
 * Must be unique for each type of argument.
 */
class Category : public StackSegment<SIZE_OF_CATEGORY>{
public:
	Category(const char* c): StackSegment<SIZE_OF_CATEGORY>(0) { this->override(c, strlen(c)); }
};

// # - # - # - # - # - # - # - # - # - # - # - # - # - # - # - # - # - # - # - # - # - # - # - # - # - # - # - # - # - #

class Hint {

	const Name        displayed_name;
	const ID          id;
	const Category    category;
	const bool        mandatory;
	const Description description;
	const std::string validity_regex;

	const std::string assembled;

private:

	std::string assemble() { 
		std::string base = "{";
		base += "\"displayedName\": \"" + this->displayed_name + "\", ";
		base += "\"id\": \"" + std::string(this->id.c_str(), SIZE_OF_ID) + "\", ";
		base += "\"category\": \"" + std::string(this->category.c_str(), SIZE_OF_CATEGORY) + "\", ";
		base += "\"mandatory\": " + std::string(this->mandatory ? "True" : "False") + ", ";
		base += "\"description\": \"" + this->description + "\"";
		base += "}";
		
		return base;
	}

public:

	inline const std::string& getAssembled() const { return this->assembled; }

	inline const ID& readID() const { return this->id; }

	Hint(const Name& n, const ID& i, const Category& c, const bool& m, const Description& d, const std::string& re): displayed_name(n), id(i), category(c), mandatory(m), description(d), validity_regex(re), assembled(this->assemble()) {}
};


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


template <typename T>
class BaseArgument {

	T data;
	const std::function<bool(const T&)> validity;
	const Hint hint;

public:

	BaseArgument(Hint&& h) : hint(h), validity([](const T& t){ return true; }) {}
	BaseArgument(Hint&& h, std::function<bool(const T&)> va) : hint(h), validity(va) {}
	BaseArgument(Hint&& h, const T& t, std::function<bool(const T&)> va) : hint(h), data(t), validity(va) {}

	inline const T& get() const { return this->data; }

	inline bool isValid() const { return this->validity(this->data); }

	inline void set(const T& t){
		this->data = t;
	}

	BaseArgument<T>& operator=(const T& t){
		this->set(t);
		return *this;
	}

	size_t assign(const rcv::receivedMap& map) {
		auto it = map.find(this->hint.readID());
		if (it == map.end()) { return 2; }

		try {
			const T& data_ref = std::get<T>(it->second);
			if (this->validity(data_ref)) {
				this->data = data_ref;
				return 0;
			}
			return 3;
		}
		catch (const std::exception& e) {
			return 4;
		}
	}
};


#endif // VFS_ARGUMENTS_BASE_HPP_INCLUDED
