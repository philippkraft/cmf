#ifndef cross_section_reach_h__
#define cross_section_reach_h__

#include "ReachType.h"

#include "../math/real.h"
#include <stdexcept>
#include <memory>
#include "../math/num_array.h"

namespace cmf {
	namespace river {

		/// @brief Structure for the description of reaches with a freely defined cross section
		///
		/// Although double triangular cross section reach are rarely met, a triangular reach does scale with its water load, and is therefore preferable in case where nothing about IChannel geometry is known
		class CrossSectionReach : public IChannel
		{
		private:
			double m_l;
			double m_nManning;
		public:
			/// The depth of the cross section in m below bank (depth[0]=0, usually)
			cmf::math::num_array depth;
			/// The x position in m for the depth value
			cmf::math::num_array x;

			virtual double get_nManning() const { return m_nManning; }
			virtual void set_nManning(double nManning) { m_nManning = nManning; }

			double get_length() const {
				return m_l;
			}
			char typecode() const { return 'C'; }

			virtual double get_channel_width(double depth) const;
			
			/// @brief Returns the wetted perimeter at a given depth
			virtual double get_wetted_perimeter(double depth) const;
			/// @brief Returns the depth at a given crossection area
			virtual double get_depth(double area) const;
			/// Returns the crossection area at a given depth
			virtual double get_flux_crossection(double depth) const;
			/// @brief Creates a new triangular reach type
			CrossSectionReach(double l, cmf::math::num_array x, cmf::math::num_array depth);
			CrossSectionReach(const CrossSectionReach& copy);
			CrossSectionReach* copy() const;
			virtual ~CrossSectionReach() {}
			void check_iterator(double h) const;

		};
	}
}

#endif
