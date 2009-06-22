#include "reach.h"




cmf::river::Reach::Reach(cmf::upslope::Cell& Cell, real Length,cmf::river::ReachType* rt,cmf::river::OpenWaterStorage* waterstorage,real depth/*=0.25*/,real width/*=1.0*/ ) 
: cell(&Cell),length(Length),channel_shape(rt->copy()),water(waterstorage),downstream(0)
{
	// Connect the surface water of the cell with the reach
	new cmf::river::Manning(*water,Cell.get_surfacewater(),*rt,sqrt(Cell.get_area()/Pi));

}

bool cmf::river::Reach::operator<( const cmf::river::Reach& cmp ) const
{
	if (water->Location.z==cmp.water->Location.z)
	{
		return water->node_id<water->node_id;
	}
	else
		return water->Location.z<cmp.water->Location.z;
}

bool cmf::river::Reach::operator==( const cmf::river::Reach& cmp ) const
{
	return *water==*cmp.water;
}

bool cmf::river::Reach::operator!=( const cmf::river::Reach& cmp ) const
{
	return !(*water==*cmp.water);
}


bool cmf::river::Reach::add_upstream(Reach * r )
{
	std::vector<Reach*>::iterator it=std::lower_bound(upstream.begin(),upstream.end(),r);
	if (it!=upstream.end() && **it==*r) 
		return false;
	else
	{
		upstream.insert(it,r);
		return true;
	}
}
bool cmf::river::Reach::remove_upstream(Reach* r)
{
	std::vector<Reach*>::iterator it=std::lower_bound(upstream.begin(),upstream.end(),r);
	if (it!=upstream.end())
	{
		upstream.erase(it);
		return true;
	}
	else
		return false;

}

void cmf::river::Reach::set_downstream( Reach* downstream_reach )
{
	if (downstream_reach==downstream) return;
	if (downstream) 
	{
		downstream->remove_upstream(this);
		get_water().remove_connection(downstream->get_water());
	}
	downstream=downstream_reach;
	if (downstream_reach)
	{
		downstream->add_upstream(this);	
		// Connect this with Manning eq. to the down stream
		new cmf::river::Manning(get_water(),downstream->get_water(),*channel_shape,length);		
	}
}

void cmf::river::Reach::set_outlet( cmf::water::FluxNode& outlet )
{
	set_downstream(0);
	new cmf::river::Manning(get_water(),outlet,*channel_shape,length);
}

void cmf::river::Reach::set_depth( real newdepth )
{
	if (newdepth<=0.0) throw std::runtime_error("The depth of a reach needs to be greater then 0");
	water->Location.z=cell->z-newdepth;
}

real cmf::river::Reach::get_depth() const
{
	return cell->z-water->Location.z;
}
double cmf::river::make_river_gap( Reach * reach )
{
	for (int i = 0; i < reach->upstream_count() ; ++i)
	{
		reach->get_water().Location.z=minimum(make_river_gap(&reach->get_upstream(i)),reach->get_water().Location.z);
	}
	return reach->get_water().Location.z;
}
