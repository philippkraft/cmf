

// Copyright 2010 by Philipp Kraft
// This file is part of cmf.
//
//   cmf is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 2 of the License, or
//   (at your option) any later version.
//
//   cmf is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with cmf.  If not, see <http://www.gnu.org/licenses/>.
//   
#include "reach.h"
double cmf::river::make_river_gap( Reach_ptr reach )
{
	for (int i = 0; i < reach->upstream_count() ; ++i)
	{
		reach->Location.z=minimum(make_river_gap(reach->get_upstream(i)),reach->Location.z);
	}
	return reach->Location.z;
}


void cmf::river::Reach::set_downstream(Reach_ptr new_downstream)
{
	// aquire reference to downstream
	Reach_ptr downstream=m_downstream.lock();
	// If downstream exists already
	if (downstream) 
	{
		// Remove this from upstream list of downstream
		downstream->remove_upstream(this);
		this->remove_connection(downstream);
	}
	// Set downstream variable
	m_downstream=weak_reach_ptr(new_downstream);
	// If it is really a pointer to a downstream
	if (new_downstream)
	{
		// add this to the list of upstream reaches of the downstream
		new_downstream->add_upstream(weak_this.lock());	
		Channel 
			this_ch=this->get_height_function(), 
			down_ch=new_downstream->get_height_function();
		MeanChannel channel_between(this_ch,down_ch);
		// Connect this with Manning eq. to the down stream
		if (m_diffusive)
			new cmf::river::Manning_Diffusive(weak_this.lock(),new_downstream,channel_between);		
		else
			new cmf::river::Manning_Kinematic(weak_this.lock(),new_downstream,channel_between);		
	}

}
cmf::river::Reach_ptr cmf::river::Reach::get_downstream() const
{
	return Reach_ptr(m_downstream);
}

bool cmf::river::Reach::add_upstream(Reach_ptr r )
{
	for(reach_vector::iterator it = m_upstream.begin(); it != m_upstream.end(); ++it)
	{
		Reach_ptr R = it->lock();
		if (R == r)
		{
			return false;
		}
	}
	m_upstream.push_back(r);
	return true;
}

bool cmf::river::Reach::remove_upstream(const cmf::river::Reach* r)
{
	for(reach_vector::iterator it = m_upstream.begin(); it != m_upstream.end(); ++it)
	{
		if (it->lock().get() == r)
		{
			m_upstream.erase(it);
			return true;
		}
	}
	return false;
}

cmf::river::Reach_ptr cmf::river::Reach::get_root()
{
	Reach_ptr down=get_downstream();
	if (down)
		return down->get_root();
	else
		return Reach_ptr(this);
}

void cmf::river::Reach::set_outlet(cmf::water::flux_node::ptr outlet )
{
	set_downstream(Reach_ptr());
	if (outlet)
		if (m_diffusive)
			new cmf::river::Manning_Diffusive(weak_this.lock(),outlet, get_height_function());
		else
			new cmf::river::Manning_Kinematic(weak_this.lock(),outlet, get_height_function());
}

cmf::upslope::cell_vector cmf::river::Reach::get_cells() const
{
	cmf::upslope::cell_vector res;
	for(cell_map::const_iterator it = m_cells.begin(); it != m_cells.end(); ++it)
		res.push_back(it->first);
	return res;
}

const cmf::river::IChannel& cmf::river::Reach::get_height_function() const
{
	return m_shape;
}

cmf::river::Reach::~Reach()
{
	if (!m_downstream.expired())
		m_downstream.lock()->remove_upstream(this);
}

cmf::river::Reach_ptr cmf::river::Reach::get_upstream( int index ) const
{
	return m_upstream.at(index<0 ? m_upstream.size()+index : index).lock();
}

void cmf::river::Reach::add_cell( cmf::upslope::Cell* cell, bool soil_cut, real distance,real width )
{
	m_cells[cell] = cell_relation(soil_cut,distance,width);
	if (m_diffusive)
		new Manning_Diffusive(weak_this.lock(),cell->get_surfacewater(),RectangularReach(distance,width));
	else
		new Manning_Kinematic(weak_this.lock(),cell->get_surfacewater(),RectangularReach(distance,width));
}

void cmf::river::Reach::set_diffusive( bool use_diffusive_wave )
{
	// Get the nodes connected with this reach
	cmf::water::connection_vector cv = this->get_connections();
	// For each node connected to this reach:
	for(cmf::water::connection_vector::iterator it = cv.begin(); it != cv.end(); ++it)
	{
		// Get the connection as a manning connection
		Manning* mc = dynamic_cast<Manning*>(*it);
		// Set the is_diffusive attribute to the given paramter
		if (mc) mc->is_diffusive_wave=use_diffusive_wave;
	}
	// Indicate the reach as solved as diffusive wave
	m_diffusive=use_diffusive_wave;
}

void cmf::river::Reach::set_dead_end()
{
	set_downstream(Reach_ptr());
	set_outlet(cmf::water::flux_node::ptr());
}

cmf::river::Reach::ptr cmf::river::Reach::create( const cmf::project& project,cmf::river::Channel shape, bool diffusive/*=false*/ )
{
	Reach* pR =	 new Reach(project,shape,diffusive);
	ptr R (pR);
	R->weak_this = R;
	return R;
}
/************************************************************************/
/* Reach Iterator                                                       */
/************************************************************************/

cmf::river::Reach_ptr cmf::river::ReachIterator::next()
{
	if (reach_queue.empty()) throw std::out_of_range("No reaches in queue");
	for (int i = 0; i < reach_queue.front().first->upstream_count() ; ++i)
	{
		double nextpos=reach_queue.front().second+reach_queue.front().first->get_length();
		queue_value next_item=queue_value(reach_queue.front().first->get_upstream(i),nextpos);
		reach_queue.push(next_item);
	}
	current=reach_queue.front();
	reach_queue.pop();
	return current.first;
}

bool cmf::river::ReachIterator::valid() const
{
	return !reach_queue.empty();
}

cmf::river::ReachIterator::ReachIterator( Reach_ptr first )
{
	reach_queue.push(queue_value(first,0.0));
	current=reach_queue.front();
}

cmf::river::Reach_ptr cmf::river::ReachIterator::operator->() const
{
	return current.first;
}

void cmf::river::ReachIterator::operator++()
{
	next();
}

cmf::river::Reach_ptr cmf::river::ReachIterator::reach() const
{
	return current.first;
}

double cmf::river::ReachIterator::position() const
{
	return current.second;
}
