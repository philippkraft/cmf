#include "cell_vector.h"
#include "cell.h"
#include <algorithm>
using namespace cmf::upslope;

cell_iterator cell_vector::begin()
{
	return m_cells.begin();
}

cell_iterator cell_vector::end()
{
	return m_cells.end();
}

cmf::math::StateVariableList cell_vector::get_states()
{
	cmf::math::StateVariableList q;
	for(cell_vector::iterator it = begin(); it != end(); ++it)
		q.extend(*it);
	return q;
}

cmf::upslope::cell_const_iterator cmf::upslope::cell_vector::begin() const
{
	return m_cells.begin();
}

cmf::upslope::cell_const_iterator cmf::upslope::cell_vector::end() const
{
	return m_cells.end();

}

cmf::upslope::cell_vector::cell_vector( cell_const_iterator first,cell_const_iterator last )
: m_cells(first.current,last.current)
{	
}

cmf::upslope::cell_vector cmf::upslope::cell_vector::get_slice( ptrdiff_t start,ptrdiff_t end,ptrdiff_t step ) 
{
	cell_vector res;
	for (ptrdiff_t i = start; i < end && i<ptrdiff_t(size())  ; i+=step)
		res.append(operator[](i));
	return res;
}

Cell& cmf::upslope::cell_vector::get_lowest() const
{
	if (size()) {
		cell_const_iterator res = begin();
		for(cell_vector::const_iterator it = begin(); it != end(); ++it)
			if (it->z < res->z) res = it;
		return *res;
	} else
		throw std::out_of_range("Cell list is empty");
}

Cell& cmf::upslope::cell_vector::get_highest() const
{
	if (size()) {
		cell_const_iterator res = begin();
		for(cell_vector::const_iterator it = begin(); it != end(); ++it)
			if (it->z > res->z) res = it;
		return *res;
	} else
		throw std::out_of_range("Cell list is empty");
}

double cmf::upslope::cell_vector::get_area() const
{
	double sum=0;
	for(cell_vector::const_iterator it = begin(); it != end(); ++it)
		sum+=it->get_area();				    
	return sum;
}

Cell& cmf::upslope::cell_vector::pop()
{
	Cell* res = m_cells.back();
	m_cells.pop_back();
	return *res;
}

void cmf::upslope::cell_vector::remove( const cmf::upslope::Cell& cell )
{
	cellvec::iterator find = std::find(m_cells.begin(),m_cells.end(),&cell);
	if (find!=m_cells.end())
		m_cells.erase(find);
	else
		throw std::out_of_range(cell.to_string() + " not found in cell vector");
}

bool cmf::upslope::cell_vector::contains( const cmf::upslope::Cell& cell ) const
{
	cellvec::const_iterator find = std::find(m_cells.begin(),m_cells.end(),&cell);
	return find!=m_cells.end();
	
}