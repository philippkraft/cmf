#ifndef geos_wrapper_h__
#define geos_wrapper_h__
#include <iostream>
#ifndef SWIG
#ifndef GEOSGeometry
typedef struct GEOSGeom_t GEOSGeometry;
typedef struct GEOSPrepGeom_t GEOSPreparedGeometry;
typedef struct GEOSCoordSeq_t GEOSCoordSequence;
#endif
#endif


namespace cmf { 
	namespace geometry {
		namespace geos {
			class geos_geometry
			{
			private:
				static int instance_counter;
				static void notice(const char *fmt, ...) {
					std::cout << "GEOS NOTICE: " << fmt << std::endl;

				}
				static void log_and_exit(const char *fmt, ...) {
					std::cout << "GEOS ERROR: " << fmt << std::endl;
				}
			protected:
				GEOSGeometry* m_geometry;
				geos_geometry(GEOSGeometry* geom);
				geos_geometry();
			public:
				geos_geometry* Envelope() const;
				geos_geometry* Intersection(const geos_geometry& cmp) const;
				
				bool disjoint(const geos_geometry& cmp) const;
				bool touches(const geos_geometry& cmp) const;
				bool intersects(const geos_geometry& cmp) const;
				bool crosses(const geos_geometry& cmp) const;
				bool within(const geos_geometry& cmp) const;
				bool contains(const geos_geometry& cmp) const;
				bool equals(const geos_geometry& cmp) const;
				bool operator ==(const geos_geometry& cmp) const {return equals(cmp);}
				virtual ~geos_geometry();
			};

		}

	}

}
#endif // geos_wrapper_h__

