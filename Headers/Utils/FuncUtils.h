#pragma once

/* INCLUDES */
#include <map>

/* CLASS */
/*
Provides non-class specific functions for data manipulation.

@author Evan Quan
*/
template <class K, class V>
class FuncUtils
{
public:
    /*
    Retrieve the value from the specified key in a given map. If the key/value
    pair does not exist, return the default value instead.

    @param map<K, V>    m to check if key exists
    @param K            key to check in map
    @param V            defaultValue to return if key not present in m
    @return value to key if present, or default if not present
    */
    static V getValueIfNotDefault(map<K, V> m, K key, V defaultValue)
    {
        return contains(m, key) ? m[key] : defaultValue;
    }

    /*
    Check if a map contains a specified key.

    This is a method of map in C++20. 

    @return true if the specified map contains the specified key.
    */
    static bool contains(map<K, V> m, K key)
    {
        return m.find(key) != m.end();
    }

    static float magnitude(float x, float y)
    {
        return sqrt((x * x) + (y * y));
    }

private:
    /*
    Not instantiable.
    */
    FuncUtils() {};
    ~FuncUtils() {};
};

