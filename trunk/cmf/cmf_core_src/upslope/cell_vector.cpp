#include "cell_vector.h"
#include "cell.h"
using namespace cmf::upslope;

cell_iterator cell_vector::begin()
{
	return m_cells.begin();
}

cell_iterator cell_vector::end()
{
	return m_cells.end();
}

cmf::math::state_queue cell_vector::get_states()
{
	cmf::math::state_queue q;
	for(cell_vector::iterator it = begin(); it != end(); ++it)
		q.push(*it);
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

cmf::upslope::cell_vector cmf::upslope::cell_vector::get_slice( int start,int end,int step ) 
{
	cell_vector res;
	for (int i = start; i < end && i<int(size())  ; i+=step)
		res.append(operator[](i));
	return res;
}