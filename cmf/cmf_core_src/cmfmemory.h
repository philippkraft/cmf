#ifndef cmfmemory_h__
#define cmfmemory_h__

#if __cplusplus > 199711L || _MSC_VER > 1800 || SWIG
#pragma message("using std::shared_ptr")
#include <memory>
#elif __GNUG__ >= 4 && __GNUC_MINOR__ >=3
#pragma message("using std::tr1::shared_ptr")
#include <tr1/memory>
#include <tr1/cmath>
namespace std {
	using std::tr1::shared_ptr;
	using std::tr1::weak_ptr;
	using std::tr1::enable_shared_from_this;
	using std::tr1::dynamic_pointer_cast;
	using std::tr1::static_pointer_cast;
	using std::tr1::isfinite
}

#else
#pragma message("using boost::shared_ptr")
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
}
#endif

#endif