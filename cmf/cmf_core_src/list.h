#ifndef list_h__
#define list_h__

#include <vector>
#include <sstream>
#include <algorithm>
#include <stdexcept>

namespace cmf {

    template <typename T>
    class List: private std::vector<T> {
#ifndef SWIG
    public:
        // typedef typename std::vector<T>::const_iterator const_iterator;
        // typedef typename std::vector<T>::iterator iterator;
        List(std::initializer_list<T> l) : std::vector<T>(l) {}
        using std::vector<T>::begin;
        using std::vector<T>::end;

#endif
    public:
        void append(T r) {
            this->push_back(r);
        }
        void extend(const List<T>& rl) {
            this->insert(this->end(), rl.begin(), rl.end());
        }
        T operator[](ptrdiff_t index) const {
            if (index < 0) {
                index += size();
            }
            return std::vector<T>::at(index);
        }

        void remove(const T& what) {
            auto it = std::find(begin(), end(), what);
            if (it == end()) {
                throw std::out_of_range("Requested object is not a part of this cmf.List");
            } else {
                this->erase(it);
            }
        }
        void remove_at(long long index) {
            if (index < 0 ) index += size();
            auto it = begin() + index;
            if (it >= begin() && it < end()) {
                this->erase(it);
            } else {
                throw std::out_of_range("Requested position is not a part of this cmf.List");
            }
        }
        void clear() {std::vector<T>::clear();}
        size_t size() const {return std::vector<T>::size();};
        size_t index(const T& what) {
            auto it = std::find(begin(), end(), what);
            if (it == end()) {
                throw std::out_of_range("Requested object is not a part of this cmf.List");
            } else {
                return std::distance(begin(), it);
            }
        }
        List() = default;
        List(const List<T>& other) = default;
    };

}

#endif