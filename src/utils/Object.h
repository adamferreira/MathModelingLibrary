#ifndef __MML_OBJECT
#define __MML_OBJECT

#include <memory> // std::weak_ptr

#include "Object.h"
#include "Types.h"

using mml::types::hash_t;

namespace mml {

// An UniqueObject is an object with a unique id (hash) within each instance of a programm
class UniqueObject {
protected:
    hash_t _unique_id;
public:
    UniqueObject() : _unique_id(0) {}
    UniqueObject(hash_t given_id) : _unique_id(given_id) {}
    UniqueObject(const UniqueObject&) = default;
    inline hash_t id() const { return this->_unique_id; }
};

// View is basically a weak_ptr that cannot recreates shared_ptr to its control block
// A View on a object does not impact its lifetime
// TODO : make all access atomic
template<class Observed>
class View {
protected:
    std::weak_ptr<Observed> _hidden_object;
public:
    auto inline expired() const noexcept { return _hidden_object.expired(); }
    View(const View&) = default;
    View() = delete;
    ~View() = default;
    View(std::shared_ptr<Observed> o) : _hidden_object(o) {}
};

// A HiddenObject cannot be directly instantied
// Private inheritance makes all constructor privates
// An HiddenObject should not be able to alter its unique id (hash) by itself
template<class H>
class HiddenObject : private UniqueObject { friend class View<H>; };

} /* mml */

namespace mml {
    auto inline unique_id(const UniqueObject& o) { return o.id(); }
    int mystere();
} /* mml */

#endif