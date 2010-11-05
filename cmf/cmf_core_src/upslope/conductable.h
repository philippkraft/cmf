#ifndef conductable_h__
#define conductable_h__

#include <tr1/memory>
#include "../geometry/geometry.h"

namespace cmf {
	namespace upslope {
		/// An abstract interface for all classes providing a conductivity (e.g. soil layer or aquifer)
		/// Each water storage were the concept of conductivity is applicable should implement (by deriving) the interface
		///
		/// This class is not part of the Python interface, since a dynamic typed language can use duck typing instead of this interface
		class conductable {
		public:
			virtual real get_K(cmf::geometry::point direction) const=0;
			typedef std::tr1::shared_ptr<conductable> ptr;
		};
	}

}


#endif // conductable_h__
