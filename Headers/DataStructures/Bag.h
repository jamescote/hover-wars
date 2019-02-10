#pragma once
#include <unordered_map>
#include <iterator>

using namespace std;

/*
Bags contain quantities of objects, which can be added or removed.
*/
template <class K>
class Bag
{
public:
	Bag();
	~Bag();

	bool contains(K key);
	int add(K key, int value);
	int erase(K key);
	int remove(K key, int value);
	int count(K key);
	int size();
	void clear();

private:

	unordered_map<K, int> inventory;

};



