

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
#include "Reach.h"
using namespace cmf::river;
using namespace cmf::water;

double cmf::river::make_river_gap( Reach::ptr reach )
{
	for (int i = 0; i < reach->upstream_count() ; ++i)
	{
		double gap_heigth = cmf::river::make_river_gap(reach->get_upstream(i));
		reach->position.z = std::min(gap_heigth,reach->position.z);
	}
	return reach->position.z;
}


void Reach::set_downstream(Reach::ptr new_downstream,bool use_meanchannel/*=false*/)
{
	// aquire reference to downstream
	Reach::ptr downstream=m_downstream.lock();
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
		std::unique_ptr<IChannel> channel;
		if (use_meanchannel) {
			Channel 
				this_ch=this->get_height_function(), 
				down_ch=new_downstream->get_height_function();
			channel.reset(new MeanChannel(this_ch,down_ch));
		} else {
			channel.reset(new Channel(this->get_height_function()));
		}
		// Connect this with Manning eq. to the down stream
		if (m_diffusive)
			new Manning_Diffusive(weak_this.lock(),new_downstream,*channel);		
		else
			new Manning_Kinematic(weak_this.lock(),new_downstream,*channel);		
	}

}
flux_node::ptr Reach::get_downstream() const
{
	if (!m_downstream.expired())
		return m_downstream.lock();
	else if (!m_outlet.expired())
		return m_outlet.lock();
	else 
		return flux_node::ptr();
		//throw std::runtime_error("No downstream is set. Use set_downstream or set_outlet to construct a downstream object");

}

bool Reach::add_upstream(Reach::ptr r )
{
	for(reach_vector::iterator it = m_upstream.begin(); it != m_upstream.end(); ++it)
	{
		Reach::ptr R = it->lock();
		if (R == r)
		{
			return false;
		}
	}
	m_upstream.push_back(r);
	return true;
}

bool Reach::remove_upstream(const Reach* r)
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

Reach::ptr Reach::get_root()
{
	Reach::ptr down=m_downstream.lock();
	if (down)
		return down->get_root();
	else
		return weak_this.lock();
}

void Reach::set_outlet(cmf::water::flux_node::ptr outlet )
{
	set_downstream(Reach::ptr());
	if (outlet) {
		m_outlet = outlet;
		Channel channel=get_height_function();
		if (m_diffusive)
			new Manning_Diffusive(weak_this.lock(),outlet, channel);
		else
			new Manning_Kinematic(weak_this.lock(),outlet, channel);
	}
}


Channel Reach::get_reachtype() const
{
	return cmf::river::OpenWaterStorage::get_height_function();
}

Reach::~Reach()
{
	if (!m_downstream.expired())
		m_downstream.lock()->remove_upstream(this);
}

Reach::ptr Reach::get_upstream( int index ) const
{
	return m_upstream.at(index<0 ? m_upstream.size()+index : index).lock();
}

void Reach::connect_to_surfacewater( cmf::upslope::Cell* cell, real width,bool diffusive )
{
	double distance = distance_to_cell(cell);
	if (diffusive)
		new Manning_Diffusive(weak_this.lock(),cell->get_surfacewater(),RectangularReach(distance,width));
	else
		new Manning_Kinematic(weak_this.lock(),cell->get_surfacewater(),RectangularReach(distance,width));
}

void Reach::set_diffusive( bool use_diffusive_wave )
{
	// Get the nodes connected with this reach
	cmf::water::connection_list cv = this->get_connections();
	// For each node connected to this reach:
	for(cmf::water::connection_list::iterator it = cv.begin(); it != cv.end(); ++it)
	{
		// Get the connection as a manning connection
		std::shared_ptr<cmf::river::Manning> mc = std::dynamic_pointer_cast<cmf::river::Manning>(*it);
		// Set the is_diffusive attribute to the given paramter
		if (mc)
			mc->is_diffusive_wave=use_diffusive_wave;
	}
	// Indicate the reach as solved as diffusive wave
	m_diffusive=use_diffusive_wave;
}

void Reach::set_dead_end()
{
	set_downstream(Reach::ptr());
	set_outlet(cmf::water::flux_node::ptr());
}

Reach::ptr Reach::create(cmf::project& project,const IChannel& shape, bool diffusive/*=false*/ )
{
	Reach* pR =	 new Reach(project,shape,diffusive);
	ptr R (pR);
	R->weak_this = R;
	return R;
}

real cmf::river::Reach::get_length() const
{
	return get_reachtype().get_length();
}

double cmf::river::Reach::distance_to_cell( cmf::upslope::Cell* cell ) const
{
	double 
		outer_distance = cell->get_position().distanceTo(position),
		cell_radius = sqrt(cell->get_area())/cmf::geometry::PI,
		distance = std::max(outer_distance,cell_radius/2);
	return distance;

}

void cmf::river::Reach::set_height_function( const IChannel& val )
{
	cmf::river::OpenWaterStorage::set_height_function(val);
}

real cmf::river::Reach::get_width() const
{
	return wet_area()/get_length();
}
Reach::Reach(cmf::project& _project,const IChannel& shape, bool diffusive/*=false*/ ) 
: OpenWaterStorage(_project,shape), m_diffusive(diffusive)
{

}

/************************************************************************/
/* Reach Iterator                                                       */
/************************************************************************/

Reach::ptr ReachIterator::next()
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

bool ReachIterator::valid() const
{
	return !reach_queue.empty();
}

ReachIterator::ReachIterator( Reach::ptr first )
{
	reach_queue.push(queue_value(first,0.0));
	current=reach_queue.front();
}

Reach::ptr ReachIterator::operator->() const
{
	return current.first;
}

void ReachIterator::operator++()
{
	next();
}

Reach::ptr ReachIterator::reach() const
{
	return current.first;
}

double ReachIterator::position() const
{
	return current.second;
}

