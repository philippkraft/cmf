#include <iostream>
#include "num_array.h"
namespace cmf {
	namespace math {
		/// Interpolates points with a cubic spline interpolation
		///
		/// Code is modified after: http://ganeshtiwaridotcomdotnp.blogspot.de/2009/12/c-c-code-cubic-spline-interpolation.html
		class cubicspline {
		private:
			num_array h,s,x,f,F;
			bool _valid;
		public:
			cubicspline(const cmf::math::num_array& x,const cmf::math::num_array& y);
			void generate();
			size_t size() const {
				return x.size();
			}
			real operator()(real x);
		};
		
	}
	
}