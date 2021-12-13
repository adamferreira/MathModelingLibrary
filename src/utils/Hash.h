#ifndef __MML_HASH
#define __MML_HASH
#include <cstddef> // std::size_t
#include <functionnal> // std::hash

namespace mml {

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
    UniqueObject::UniqueObject(hash_t given_id) : _unique_id(given_id) {}
    inline hash_t id() const { return _unique_id; }
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

template <class T = UniqueObject>
inline hash_t combine_hash(const T& a, const T& b) {
    std::hash<T> hasher;
    // Assume hash_t is 64 bits then mask is 00..<32>...01...<32>...1
    hash_t mask = (hash_t(1) << 8*sizeof(hash_t)/2) - 1;
    // First half (from left) of the hash of a
    // Second hasf (from left) of the hash of b
    return (hasher(a) & ~mask) & (hasher(b) & mask);
}

using CombinedHash = std::pair<UniqueObject, UniqueObject>;

struct std::hash<CombinedHash>
{
    hash_t operator()(const CombinedHash& o) {
        return combine_hash(o.first, o.second);
    }
};

} /* mml */
#endif