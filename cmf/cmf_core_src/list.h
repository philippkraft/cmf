#ifndef list_h__
#define list_h__


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


        void clear() {std::vector<T>::clear();}
        size_t size() const {return std::vector<T>::size();};
        List() = default;
        List(const List<T>& other) = default;

    };

}

#endif