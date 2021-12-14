#ifndef __MML_HASH
#define __MML_HASH
#include <cstddef> // std::size_t
#include <functionnal> // std::hash

namespace mml::hash {

using hash_t = std::size_t;

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

class UniqueObject {
private:
    hash_t _unique_id;
public:
    UniqueObject::UniqueObject() : _unique_id(0) {}
    UniqueObject::UniqueObject(hash_t given_id) : _unique_id(given_id) {}
    inline hash_t id() const { return this->_unique_id; }
};

// Custom specialization of std::hash can be injected in namespace std
// Compatible with concept Hashable
template <>
struct std::hash<UniqueObject>
{
    hash_t operator()(const UniqueObject& o) {
        return o.id();
    }
};

inline hash_t combine(hash_t a, hash_t b) {
    // Assume hash_t is 64 bits then mask is 00..<32>...01...<32>...1
    hash_t mask = (hash_t(1) << 8*sizeof(hash_t)/2) - 1;
    // Second half (from left) of the hash of a
    // Second half (from left) of the hash of b
    return (a << 8*sizeof(hash_t)/2) & mask) & (b & mask);
}

template <class T = UniqueObject>
inline hash_t combine_hash(const T& a, const T& b) {
    std::hash<T> hasher;
    return mml::combine(hasher(a), hasher(b));
}

std::pair<hash_t, hash_t> extract(hash_t);

class CombinedHashGenerator {
private:
    hash_t _base_hash;
    hash_t _hash_cpt;
public:
    CombinedHashGenerator(hast_t base) : _base_hash(base), _hash_cpt(0) {} 

    /* TODO delete copy constructor ? */
    inline hash_t
    next_hash() {
        return combine_hash(_base_hash, ++this->_hash_cpt);
    }
};


using CombinedHash = std::pair<UniqueObject, UniqueObject>;
template <>
struct std::hash<CombinedHash>
{
    hash_t operator()(const CombinedHash& o) {
        return combine_hash(o.first, o.second);
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

} /* mml */
#endif