#include <map>
#include "common.h"

#ifndef ST_INCLUDED
#define ST_INCLUDED

using namespace std;

template<class T>
class SymbolTable {
	map<string, T> table;

public:
	void add(T s);
	void add(string key, T s);
	T get(string name);
	void assign(Variable *var, T value);
	void assign(string var, T value);
};

template<class T>
void SymbolTable<T>::add(string key, T s) {
	table.insert(std::make_pair(key, s));
}

template<class T>
void SymbolTable<T>::add(T s) {
	table.insert(std::make_pair(s->symbol_name, s));
}

template<class T>
T SymbolTable<T>::get(string name) {
	typename map<string, T>::iterator it;
	it = table.find(name);
	return it->second;
}
template<class T>
void SymbolTable<T>::assign(Variable *var, T value) {
	string name = var->get_name();
	typename map<string, T>::iterator it;
	it = table.find(name);
	it->second = value;

}
template<class T>
void SymbolTable<T>::assign(string var, T value) {
	typename map<string, T>::iterator it;
	it = table.find(var);
	it->second = value;
}

#endif

