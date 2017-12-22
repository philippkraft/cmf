#ifndef cmfmemory_h__
#define cmfmemory_h__

#if __cplusplus > 199711L || _MSC_VER > 1800 || SWIG

#include <memory>

#else

#include <boost/shared_ptr.hpp>
#include <boost/pointer_cast.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace std
{
	using boost::shared_ptr;
	using boost::weak_ptr;
	using boost::enable_shared_from_this;
	using boost::dynamic_pointer_cast;
	using boost::static_pointer_cast;
	inline bool isfinite(double v) {
		return v == v;
	}
#define unique_ptr auto_ptr
}
#endif

#endif