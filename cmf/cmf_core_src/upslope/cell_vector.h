#ifndef cell_vector_h__
#define cell_vector_h__

#include <set>
#include "../math/statevariable.h"
namespace cmf {
	namespace upslope {
		class Cell;
		class cell_iterator;
		class cell_const_iterator;

		/// A cell vector holds a bunch of cells
		class cell_vector : public cmf::math::StateVariableOwner {
		private:
			typedef std::vector<cmf::upslope::Cell*> cellvec;
			cellvec m_cells;
			friend class cell_iterator;
			friend class cell_const_iterator;
		public:
			const cmf::upslope::Cell& operator[](ptrdiff_t index) const {
				return *m_cells.at(index >=0 ? index : m_cells.size() + index);
			}
			cell_vector() {}
			cell_vector(const cell_vector& copy)
				: m_cells(copy.m_cells) {}
			
			cmf::upslope::Cell& operator[](ptrdiff_t index) {
				return *m_cells.at(index >=0 ? index : m_cells.size() + index);
			}
						cmf::math::StateVariableList get_states();
			void append(cmf::upslope::Cell& cell) {
				m_cells.push_back(&cell);
			}
			void remove(ptrdiff_t index) {
				ptrdiff_t ndx = index<0 ? size()+index : index;
				m_cells.erase(m_cells.begin() + ndx);
			}
			void remove(const cmf::upslope::Cell& cell);
			/// Returns and removes the last cell
			Cell& pop();

			size_t size() const {return m_cells.size();}

			cell_vector get_slice(ptrdiff_t start,ptrdiff_t end,ptrdiff_t step=1) ;

			/// Returns the cell with the lowest height
			Cell& get_lowest() const;
			/// Returns the heighest cell
			Cell& get_highest() const;

			/// Returns sum of the area of the cells
			double get_area() const;
			bool contains(const cmf::upslope::Cell& cell) const;
			virtual ~cell_vector()
			{

			}



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

			cell_iterator& operator+=(ptrdiff_t pos) {
				current+=pos;
				return *this;
			}
			cell_iterator operator+(ptrdiff_t pos) {
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

			cell_iterator& operator-=(ptrdiff_t pos) {
				current-=pos;
				return *this;
			}

			cell_iterator operator-(ptrdiff_t pos) {
				cell_iterator res=*this;
				res-=pos;
				return res;
			}
			ptrdiff_t operator-(cell_iterator b) {
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

			cell_const_iterator& operator+=(ptrdiff_t pos) {
				current+=pos;
				return *this;
			}
			cell_const_iterator operator+(ptrdiff_t pos) {
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

			cell_const_iterator& operator-=(ptrdiff_t pos) {
				current-=pos;
				return *this;
			}

			cell_const_iterator operator-(ptrdiff_t pos) {
				cell_const_iterator res=*this;
				res-=pos;
				return res;
			}
			ptrdiff_t operator-(cell_const_iterator b) {
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
