#ifndef groundwater_h__
#define groundwater_h__

#include "../water/WaterStorage.h"
#include "cell.h"
#include "cell_vector.h"
#include "conductable.h"
namespace cmf {
	namespace upslope {

		///@ingroup storages
		/// A class to represent large groundwater storages, 
		/// not bounded to the usual horizontal discretization scheme, the Cell.
		///
		/// Naturally aquifers are connected with aquifer_Darcy or kinematic_wave connections. 
		/// As a boundary condition for a cell based soil water system, aquifers are used 
		/// as a right hand side node of percolation connections.
		///
		/// Basic head (\f$\Psi\f$) / volume (\f$V\f$) relation:
		/// \f[ \Psi = z_{base} + \frac {V}{A \Phi} \f]
		/// - \f$\Psi\f$ water head in m
		/// - \f$z_{base}\f$ base height of the aquifer
		/// - \f$V\f$ volume of stored water in m3
		/// - \f$A\f$ Base area of the aquifer in m2
		/// - \f$\Phi\f$ Porosity, or more general, \f$\frac {dV_{bulk}}{dV_{H_2O}}\f$
		class aquifer : public cmf::water::WaterStorage, public cmf::upslope::conductable {

		protected:
			virtual real head_to_volume(real head) const;
			virtual real volume_to_head(real volume) const;

		public:
			typedef std::shared_ptr<aquifer> ptr;
			/// The horizontal area of the aquifer
			real area;
			/// The thickness of the aquifer in m
			real thickness;
			/// The porosity of the aquifer in m3/m3
			real porosity;
			/// The conductivity in m/day for the three spatial dimensions
			cmf::geometry::point K;
			/// Returns the conductivity in m/day for a specific direction.
			/// Takes account for anisotropy
			/// \f[ \|K\|(d) = \frac{d}{\|d\|} \bullet K\f]
			virtual real get_K(cmf::geometry::point direction) const;
			/// Returns the base height of the aquifer in m a.s.l.
			real get_base_height() const {
				return position.z - 0.5 * thickness;
			}
			/// Returns the top height of the aquifer in m a.s.l.
			real get_top_height() const {
				return position.z + 0.5 * thickness;
			
			}
			virtual real get_abs_errtol(real rel_errtol) {
				return rel_errtol * this->area * this->thickness * this->porosity;
			}

			virtual void set_potential(real new_potential);
			/// Creates an aquifer at a certain position
			/// @param p Project, this aquifer belongs to
			/// @param position Position of the aquifer center (x and y) and the aquifer top (z)
			/// @param area Area of the aquifer
			/// @param thickness Mean thickness of the aquifer in m
			/// @param porosity Mean porositiy of the aquifer
			/// @param K Conductivity of the aquifer in m/day. If the conductivity should be anisotropic, change the x,y and z values of the member K
			aquifer(cmf::project& p,cmf::geometry::point position, real area, real thickness, real porosity,real K=1e-4);
			
			
			/// Creates an aquifer below a collection of cells
			/// @param cells The cells above the aquifer. The area of the aquifer equals the area of the cells, 
			/// and the upper boundary equals the soildepth of the lowest cell
			/// @param thickness The thickness of the aquifer in m
			/// @param porosity Porosity of the aquifer
			/// @param K Conductivity of the aquifer in m/day. If the conductivity should be anisotropic, change the x,y and z values of the member K
			aquifer(cmf::upslope::cell_vector& cells,real thickness,real porosity, real K=1e-4);
			/// Casts a flux_node to an aquifer, if the flux_node is an aquifer
			static aquifer::ptr cast(cmf::water::flux_node::ptr for_cast) {
				return std::dynamic_pointer_cast<aquifer>(for_cast);
			}
		};

		///@ingroup latflux
		/// Lateral darcy flow between aquifer objects.
		///
		/// \f[v_{Darcy}=K_{1,2}\nabla\Psi \f]
		/// \f[q = v_{Darcy} w \Delta z\f]
		class aquifer_Darcy : public cmf::water::flux_connection {
		protected:
			aquifer::ptr gw1;
			aquifer::ptr gw2;
			/// Lateral darcy flow between aquifer objects.
			///
			/// \f[v_{Darcy}=K_{1,2}\nabla\Psi \f]
			/// \f[q = v_{Darcy} w \Delta z\f]
			virtual real calc_q(cmf::math::Time t);
			virtual void NewNodes() {
				gw1 = aquifer::cast(left_node());
				gw2 = aquifer::cast(right_node());
			}
		public:
			/// w, the width of the connection between the aquifers
			real flux_width;

			/// Creates a new Darcy flow connection between two aquifers, 
			/// or an aquifer and another node acting as Dirichlet boundary condition
			/// @param left One aquifer
			/// @param right Another aquifer, or a flux node that can be interpreted as an Dirichlet boundary
			/// @param width Width of the connection
			aquifer_Darcy(aquifer::ptr left, cmf::water::flux_node::ptr right, real width);

		};
		
	}
}
#endif // groundwater_h__

