#include "geos_wrapper.h"
#include "geos_c.h"
int cmf::geometry::geos::geos_geometry::instance_counter(0);
cmf::geometry::geos::geos_geometry::geos_geometry( GEOSGeometry* geom ) : m_geometry(geom)
{
	++instance_counter;
}

cmf::geometry::geos::geos_geometry::geos_geometry() : m_geometry(0)
{
	if (!instance_counter)
	{
		initGEOS(notice,log_and_exit);
	}
	++instance_counter;
}

cmf::geometry::geos::geos_geometry* cmf::geometry::geos::geos_geometry::Envelope() const
{
	if (!m_geometry) throw std::runtime_error("Can't calculate the envelope of no data");
	GEOSGeometry* res=GEOSEnvelope(m_geometry);
	return new geos_geometry(res);
}

cmf::geometry::geos::geos_geometry* cmf::geometry::geos::geos_geometry::Intersection( const geos_geometry& cmp ) const
{
	if (!m_geometry) throw std::runtime_error("Can't calculate the envelope of no data");
	GEOSGeometry* res=GEOSIntersection(m_geometry,cmp.m_geometry);
	return new geos_geometry(res);
}

cmf::geometry::geos::geos_geometry::~geos_geometry()
{
	--instance_counter;
	if (m_geometry)
		GEOSGeom_destroy(m_geometry);
	if (!instance_counter)
		finishGEOS();
}

bool cmf::geometry::geos::geos_geometry::disjoint( const geos_geometry& cmp ) const
{
	char res=GEOSDisjoint(this->m_geometry,cmp.m_geometry);
	if (res==2) throw std::runtime_error("Could not get GEOS topological operation failed");
	else return res!=0;
}

bool cmf::geometry::geos::geos_geometry::touches( const geos_geometry& cmp ) const
{
	char res=GEOSTouches(this->m_geometry,cmp.m_geometry);
	if (res==2) throw std::runtime_error("GEOS topological operation failed");
	else return res!=0;
}

bool cmf::geometry::geos::geos_geometry::intersects( const geos_geometry& cmp ) const
{
	char res=GEOSIntersects(this->m_geometry,cmp.m_geometry);
	if (res==2) throw std::runtime_error("GEOS topological operation failed");
	else return res!=0;
}

bool cmf::geometry::geos::geos_geometry::crosses( const geos_geometry& cmp ) const
{
	char res=GEOSCrosses(this->m_geometry,cmp.m_geometry);
	if (res==2) throw std::runtime_error("GEOS topological operation failed");
	else return res!=0;
}

bool cmf::geometry::geos::geos_geometry::within( const geos_geometry& cmp ) const
{
	char res=GEOSWithin(this->m_geometry,cmp.m_geometry);
	if (res==2) throw std::runtime_error("GEOS topological operation failed");
	else return res!=0;
}