#ifndef layer_list_h__
#define layer_list_h__
#include "../water/collections.h"
#include "SoilLayer.h"
#include "../math/num_array.h"
#include <vector>
namespace cmf {
	namespace upslope {
		/// A vector of layers, with array access to the properties of the layers, for fast data exchange
		class layer_list {
		private:
			typedef std::vector<SoilLayer::ptr> layer_vector;
			layer_vector m_layers;
		public:
			/// Copy constructor
			layer_list(const layer_list& for_copy) 
				: m_layers(for_copy.m_layers) {}
			/// Creates an empty layer list
			layer_list() {}
			/// Creates a list of all soil layers from a node_list
			layer_list(const cmf::water::node_list& for_copy);

#ifndef SWIG
			typedef layer_vector::iterator iterator;
			typedef layer_vector::const_iterator const_iterator;
			iterator begin() {return m_layers.begin();}
			const_iterator begin() const {return m_layers.begin();}
			iterator end() {return m_layers.end();}
			const_iterator end() const {return m_layers.end();}
			/// Returns a layer from the list
			SoilLayer::ptr operator[](ptrdiff_t index) const {
				return m_layers.at(index>=0 ? index : size() + index);
			}
#endif
			/// Deletes the last layer from the list and returns it
			SoilLayer::ptr pop() {
				SoilLayer::ptr l = m_layers.back();
				m_layers.pop_back();
				return l;
			}
			/// Appends a soil layer to the list
			layer_list& append(SoilLayer::ptr l) { m_layers.push_back(l); return *this;}
			/// Appends all soil layers of layer_list to this
			layer_list& extend(const layer_list& ll) { std::copy(ll.begin(),ll.end(),this->end()); return *this;}
			/// Appends all soil layers from a node_list to this
			layer_list& extend(const cmf::water::node_list& nl); 

			layer_list get_slice(size_t first=0, size_t last=1000000, size_t step = 1);
			/// Clears the list
			void clear() { m_layers.clear();}
			/// Number of layers in the list
			size_t size() const {return m_layers.size();}
			/// Sets the wetness in m3/m3 of layers [offset : arraysize]
			void set_wetness(const cmf::math::num_array& Value,size_t offset = 0);
			/// Returns an array containing the potentials of all layers in the list
			cmf::math::num_array get_potential() const;
			/// Sets the potential (head) in m of layers [offset : arraysize]
			void set_potential(const cmf::math::num_array& Value,size_t offset = 0);
			/// Returns an array containing the wetness in m3 H2O/m3 pore space of all layers in the list
			cmf::math::num_array get_wetness() const;
			/// Sets the Volume in m3 of layers [offset : arraysize]
			void set_volume(const cmf::math::num_array& Value,size_t offset = 0);
			///  Returns an array containing the matrix potential in m of all layers in the list
			cmf::math::num_array get_matrix_potential() const;
			///  Returns an array containing the porosity in m3/m3 of all layers in the list
			cmf::math::num_array get_porosity() const;
			///  Returns an array containing the volume in m3 of stored water of all layers in the list
			cmf::math::num_array get_volume() const;
			///  Returns an array containing the thickness in m of all layers in the list
			cmf::math::num_array get_thickness() const;
			///  Returns an array containing the gravitational potential in m of all layers in the list
			cmf::math::num_array get_gravitational_potential() const;
			///  Returns an array containing the conductivity in m/day of all layers in the list
			cmf::math::num_array get_K() const;
			///  Returns an array containing the saturated conductivity in m/day of all layers in the list
			cmf::math::num_array get_Ksat() const;
			///  Returns an array containing the lower boundary in m below surface of all layers in the list
			cmf::math::num_array get_lower_boundary() const;
			///  Returns an array containing the upper boundary in m below surface of all layers in the list
			cmf::math::num_array get_upper_boundary() const;
			/// Returns an array containing the rootfraction of each layer
			cmf::math::num_array get_rootfraction() const;

			cmf::math::num_array get_ice_fraction() const;
			cmf::math::num_array get_theta() const;

			/// Returns the flux to each layer from the upper layer, or, in case of the first layer from the surface water 
			cmf::math::num_array get_percolation(cmf::math::Time t) const;


			/// Sets the volumetric water content of the soil
			void set_theta(const cmf::math::num_array& Value, size_t offset=0);
			/// Sets the fraction of the ice content of the soil water
			void set_ice_fraction(const cmf::math::num_array& Value, size_t offset=0);
			/// Sets the fraction of roots in each layer
			void set_rootfraction(const cmf::math::num_array& Value, size_t offset=0);

		};
		
	}
}
#endif // layer_list_h__
