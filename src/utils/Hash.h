#ifndef __MML_HASH
#define __MML_HASH
#include <functional> // std::hash
#include <utility> // std::make_pair

#include "Types.h"
#include "Object.h"

using mml::types::hash_t;

namespace mml::hash {

inline 
hash_t combine(hash_t a, hash_t b) {
    // Assume hash_t is 64 bits then mask is 00..<32>...01...<32>...1
    hash_t mask = (hash_t(1) << 8*sizeof(hash_t)/2) - 1;
    // Second half (from left) of the hash of a
    // Second half (from left) of the hash of b
    return ((a << 8*sizeof(hash_t)/2) & mask) & (b & mask);
}

inline
auto extract(hash_t h) {
    // Assume hash_t is 64 bits then mask is 00..<32>...01...<32>...1
    hash_t mask = (hash_t(1) << 8*sizeof(hash_t)/2) - 1;
    return std::make_pair(((h << 8*sizeof(hash_t)/2) & mask), h & mask);
}

template <class T = UniqueObject>
inline 
auto combine(const T& a, const T& b) {
    std::hash<T> hasher;
    return combine(hasher(a), hasher(b));
}

template <class T = UniqueObject>
inline 
auto extract(const T& a) {
    return extract(std::hash<T>{}(a));
}

// TODO : make it a generator ? use yield ?
class CombinedHashGenerator {
private:
    hash_t _base_hash;
    hash_t _hash_cpt;
public:
    CombinedHashGenerator() = delete;
    CombinedHashGenerator(hash_t base) : _base_hash(base), _hash_cpt(0) {} 
    inline hash_t
    next() {
        return combine(this->_base_hash, ++this->_hash_cpt);
    }
};

// An OwnedObject cannot be directly instantied
// Private inheritance makes all constructor privates
// An OwnedObject should not be able to alter its unique id (hash) by itself
class OwnedObject : private UniqueObject { friend class UniqueObjectFactory; };

template<class Parent = UniqueObject>
class OwnedObjectFactory : public CombinedHashGenerator {
public:
    OwnedObjectFactory(const Parent& p) : CombinedHashGenerator(p.id()) {}    

    template<class Children = OwnedObject>
    inline 
    Children make() {
        return Children(this->next());
    }

private:
    template<class Children = OwnedObject>
    inline 
    Children make(hash_t fixed_hash) const {
        return Children(fixed_hash);
    }

    template<class Children = OwnedObject>
    inline 
    Children make(const Parent& p) const {
        return this->make(std::hash<Parent>{}(p));
    }
};

} /* mml::hash */

#if 0
// https://en.cppreference.com/w/cpp/language/constraints
// Declaration of the concept "Hashable", which is satisfied by any type 'T'
// such that for values 'a' of type 'T', the expression std::hash<T>{}(a)
// compiles and its result is convertible to std::size_t
template<typename T>
concept Hashable = requires(T a) {
    { std::hash<T>{}(a) } -> std::convertible_to<hash_t>;
};
#endif

// Custom specialization of std::hash can be injected in namespace std
// Compatible with concept Hashable
namespace std {
    template <> 
    struct hash<mml::UniqueObject>
    {
        mml::types::hash_t operator()(const mml::UniqueObject& o) const noexcept {
            return o.id();
        }
    };
} /* std */

#endif