#ifndef __MML_HASH
#define __MML_HASH
#include <cstddef> // std::size_t
#include <functional> // std::hash

namespace mml::hash {

using hash_t = std::size_t;

class UniqueObject {
private:
    hash_t _unique_id;
public:
    UniqueObject() : _unique_id(0) {}
    UniqueObject(hash_t given_id) : _unique_id(given_id) {}
    inline hash_t id() const { return this->_unique_id; }
};

inline hash_t combine(hash_t a, hash_t b) {
    // Assume hash_t is 64 bits then mask is 00..<32>...01...<32>...1
    hash_t mask = (hash_t(1) << 8*sizeof(hash_t)/2) - 1;
    // Second half (from left) of the hash of a
    // Second half (from left) of the hash of b
    return ((a << 8*sizeof(hash_t)/2) & mask) & (b & mask);
}

template <class T = UniqueObject>
inline hash_t combine_hash(const T& a, const T& b) {
    std::hash<T> hasher;
    return combine(hasher(a), hasher(b));
}

class CombinedHashGenerator {
private:
    hash_t _base_hash;
    hash_t _hash_cpt;
public:
    CombinedHashGenerator() = delete;
    CombinedHashGenerator(hash_t base) : _base_hash(base), _hash_cpt(0) {} 
    inline hash_t
    next_hash() {
        return combine_hash(_base_hash, ++this->_hash_cpt);
    }
};

template<class Parent = UniqueObject>
class UniqueObjectFactory : public CombinedHashGenerator {
public:
    UniqueObjectFactory(const Parent& p) : CombinedHashGenerator(p.id()) {}    

    template<class Children = UniqueObject>
    inline Children
    make() {
        return Children(this->next_hash());
    }

    template<class Children = UniqueObject>
    inline Children
    make(hash_t fixed_hash) {
        return Children(fixed_hash);
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
    struct hash<mml::hash::UniqueObject>
    {
        mml::hash::hash_t operator()(const mml::hash::UniqueObject& o) const noexcept {
            return o.id();
        }
    };
} /* std */

#endif