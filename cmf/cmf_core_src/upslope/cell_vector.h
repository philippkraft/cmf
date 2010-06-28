#ifndef cell_vector_h__
#define cell_vector_h__

#include <set>
#include "../math/statevariable.h"
namespace cmf {
	namespace upslope {
		class Cell;
		class cell_iterator;
		class cell_const_iterator;
		class cell_vector : public cmf::math::StateVariableOwner {
		private:
			typedef std::vector<cmf::upslope::Cell*> cellvec;
			cellvec m_cells;
			friend class cell_iterator;
			friend class cell_const_iterator;
		public:
			const cmf::upslope::Cell& operator[](int index) const {
				return *m_cells.at(index >=0 ? index : m_cells.size() + index);
			}
			cell_vector() {}
			cell_vector(const cell_vector& copy)
				: m_cells(copy.m_cells) {}
			cmf::upslope::Cell& operator[](int index) {
				return *m_cells.at(index >=0 ? index : m_cells.size() + index);
			}
						cmf::math::state_queue get_states();
			void append(cmf::upslope::Cell& cell) {
				m_cells.push_back(&cell);
			}
			size_t size() const {return m_cells.size();}
			cell_vector get_slice(int start,int end,int step) ;

#ifndef SWIG
			typedef cell_iterator iterator;
			typedef cell_const_iterator const_iterator;
			cell_vector(cell_const_iterator first,cell_const_iterator last);
			cell_iterator begin();
			cell_iterator end();
			cellvec::iterator ptr_begin() {return m_cells.begin();}
			cellvec::iterator ptr_end() {return m_cells.end();}
			cell_const_iterator begin() const;
			cell_const_iterator end() const ;
			std::set<Cell*> as_set() {
				return std::set<Cell*>(m_cells.begin(),m_cells.end());
			}
#endif
		};
#ifndef SWIG

		class cell_iterator {
		private:
			friend class cell_vector;
			friend class cell_const_iterator;
			typedef cell_vector::cellvec::iterator iterator;
			iterator current;
			cell_iterator(const cell_vector::cellvec::iterator& iter) 
				: current(iter) {}
		public:
			cell_iterator()
				: current() {}
			cell_iterator(const cell_iterator& copy) 
				: current(copy.current)	{}
			cell_iterator& operator=(const cell_iterator& copy) {
				current = copy.current;
				return *this;
			}
			iterator as_iterator() { return current;}
			cmf::upslope::Cell& operator*() const {
				return **current;
			}
			cmf::upslope::Cell* operator->() const {
				return *current;
			}
			operator Cell*() const { return *current;}

			cmf::upslope::Cell* ptr() const {return *current;}
			void operator++() {
				++current;
			}
			cell_iterator operator++(int) {
				cell_iterator res=*this;
				operator++();
				return res;
			}

			cell_iterator& operator+=(int pos) {
				current+=pos;
				return *this;
			}
			cell_iterator operator+(int pos) {
				cell_iterator res=*this;
				res+=pos;
				return res;
			}
			void operator--() {
				--current;
			}
			cell_iterator operator--(int) {
				cell_iterator res=*this;
				operator--();
				return res;
			}

			cell_iterator& operator-=(int pos) {
				current-=pos;
				return *this;
			}

			cell_iterator operator-(int pos) {
				cell_iterator res=*this;
				res-=pos;
				return res;
			}
			int operator-(cell_iterator b) {
				return current - b.current;
			}

			bool operator==(const cell_iterator& cmp) {return current == cmp.current;}
			bool operator!=(const cell_iterator& cmp) {return current != cmp.current;}
			bool operator<(const cell_iterator& cmp)  {return current < cmp.current;}
			bool operator<=(const cell_iterator& cmp) {return current <= cmp.current;}
			bool operator>(const cell_iterator& cmp)  {return current > cmp.current;}
			bool operator>=(const cell_iterator& cmp) {return current >= cmp.current;}
		};

		class cell_const_iterator {
		private:
			friend class cell_vector;
			cell_const_iterator(const cell_vector::cellvec::const_iterator& iter) 
				: current(iter) {}
			typedef cell_vector::cellvec::const_iterator const_iterator;
			const_iterator current;
		public:
			cell_const_iterator(const cell_const_iterator& copy) 
				: current(copy.current)	{}
			cell_const_iterator(const cell_iterator& copy)
				: current(copy.current) {}
			cell_const_iterator& operator=(const cell_const_iterator& copy) {
				current = copy.current;
				return *this;
			}
			const_iterator as_iterator() { return current;}

			cmf::upslope::Cell& operator*() const {
				return **current;
			}
			cmf::upslope::Cell* operator->() const {
				return *current;
			}
			operator const Cell*() const { return *current;}
			const cmf::upslope::Cell* ptr() const {return *current;}
			void operator++() {
				++current;
			}
			cell_const_iterator operator++(int) {
				cell_const_iterator res=*this;
				operator++();
				return res;
			}

			cell_const_iterator& operator+=(int pos) {
				current+=pos;
				return *this;
			}
			cell_const_iterator operator+(int pos) {
				cell_const_iterator res=*this;
				res+=pos;
				return res;
			}
			void operator--() {
				--current;
			}
			cell_const_iterator operator--(int) {
				cell_const_iterator res=*this;
				operator--();
				return res;
			}

			cell_const_iterator& operator-=(int pos) {
				current-=pos;
				return *this;
			}

			cell_const_iterator operator-(int pos) {
				cell_const_iterator res=*this;
				res-=pos;
				return res;
			}
			int operator-(cell_const_iterator b) {
				return current - b.current;
			}

			bool operator==(const cell_const_iterator& cmp) {return current == cmp.current;}
			bool operator!=(const cell_const_iterator& cmp) {return current != cmp.current;}
			bool operator<(const cell_const_iterator& cmp) {return current < cmp.current;}
			bool operator<=(const cell_const_iterator& cmp) {return current <= cmp.current;}
			bool operator>(const cell_const_iterator& cmp) {return current > cmp.current;}
			bool operator>=(const cell_const_iterator& cmp) {return current >= cmp.current;}
		};
#endif

		
	}
}

#endif // cell_vector_h__
