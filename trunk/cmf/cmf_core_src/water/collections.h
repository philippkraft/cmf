#ifndef collections_h__
#define collections_h__
#include "flux_node.h"
#include "boundary_condition.h"
#include "../Geometry/geometry.h"
#include "../math/num_array.h"
namespace cmf {
	namespace water {

		/// A collection of nodes for fast access of the waterbalance
		///
		/// In setups with many storages and rather fast computations, the speed of data access for output generation can take a high portion of the total run time.
		/// To accelerate data access, one can use the node_list object
		class node_list	: public cmf::math::StateVariableOwner
		{
		private:

			typedef std::vector<flux_node::ptr> node_vector;
			node_vector m_nodes;
		public:
			/// The number of nodes
			int size() const {return int(m_nodes.size());}

#ifndef SWIG
			flux_node::ptr operator[](int index) const
			{
				return m_nodes.at(index<0 ? size()+index : index);
			}
			node_vector::iterator begin() {return m_nodes.begin();}
			node_vector::iterator end() {return m_nodes.end();}

#endif
			/// Creates an empty node_lust			
			node_list() {}
			/// Copy the node_list
			node_list(const node_list& forcopy)
				: m_nodes(forcopy.m_nodes) {}

			/// Extends the node_list with another
			node_list& operator+=(const node_list& right)
			{
				for (int i = 0; i < right.size() ; ++i)
				{
					append(right[i]);
				}
				return *this;
			}
			/// Returns the union of two node_lists
			cmf::water::node_list operator+(const cmf::water::node_list & right) const
			{
				cmf::water::node_list res(*this);
				res+=right;
				return res;
			}
			/// Returns a node in the node_list
			flux_node::ptr get(int index) const
			{
				return m_nodes.at(index<0 ? size()+index : index);
			}
			/// Returns a slice of the node_list
			node_list get(int begin,int end,int step=1) const
			{
				node_list res;
				for (int i = begin; i <end  ; i+=step)
				{
					res.append(get(i));
				}
				return res;
			}

			/// Implements StateVariableOwner
			void AddStateVariables(cmf::math::StateVariableVector& vector);
			/// Adds a flux node to the list
			void append(flux_node::ptr node);

			/// Sets the potentials of the node_lists
			///
			/// If node do not have changeabe potentials, they are skipped
			/// @returns The number of nodes with changed potential
			int set_potentials(const cmf::math::num_array& potentials);
			cmf::math::num_array get_potentials();
			/// Returns the sum of the water balances of the nodes
			///
			/// \f[\sigma_{global} = \sum_{i=0}^N{\sum_{j=0}^{C_i}{q_{ij}(t)}} \f]
			///
			/// Replaces slow Python code like:
			/// @code
			/// sum=0
			/// for n in nodes:
			///     sum+=n.water_balance(t)
			/// @endcode
			real global_water_balance(cmf::math::Time t) const;
			/// Returns the water balance of each vector as a vector
			///
			/// \f[ \sigma_i = \sum_{j=0}^{C_i}{q_{ij}(t)} \f]
			///
			/// Replaces slow Python code like:
			/// @code
			/// balances=[n.water_balance(t) for n in nodes]
			/// @endcode
			cmf::math::num_array water_balance(cmf::math::Time t) const;

			/// Returns an array holding the concentration of all the flux nodes for the given solute
			cmf::math::num_array conc(cmf::math::Time t, const cmf::water::solute& _Solute) const;
			/// Sets the source flux of a solute storage associated  with a node (node has to be a water storage)
			int set_solute_source(const cmf::water::solute& _Solute, cmf::math::num_array source_fluxes);

			/// A fast method to perform flux queries as a batch. The node lists left and right should have the same length.
			///
			/// @returns The vector containing the flux from left to right at the same position
			/// @param targets A node_list containing the source nodes
			/// @param t The time for the fluxes
			///
			/// Replaces slow Python code like:
			/// @code
			/// fluxes=[left_node[i].flux_to(right_node[i],t) for i in range(len(left_nodes))]
			/// @endcode
			cmf::math::num_array get_fluxes_to( const cmf::water::node_list& targets ,cmf::math::Time t) const;
			/// Returns the flux vectors to the nodes of a given target node_list
			cmf::geometry::point_vector get_fluxes3d_to( const cmf::water::node_list& targets ,cmf::math::Time t) const;
			/// Returns the current flow vector for each node
			cmf::geometry::point_vector get_fluxes3d(cmf::math::Time t) const;
			/// Returns the positions of the nodes
			cmf::geometry::point_vector get_positions() const;

		};

		typedef std::tr1::shared_ptr<NeumannBoundary> NeumannBoundary_ptr;

		/// If many Neumann boundary conditions are present in a project, a fast data exchange to update the fluxes might be needed.
		///
		/// With this specialized list a num_array can be passed to the boundary conditions for a fast flux update
		/// If a multiple system layout for the cmf setup is chosen, we might have a 
		/// node_list Drichelet boundary conditions (dbc), a corresponding NeumannBoundary_list (nbc) of Neumann boundaries and a node_list
		/// containing the storages connected with the NeumannBoundary_list (storages). The fast data exchange is written in Python as:
		/// @code
		/// # Update the fluxes at the Neumann boundary conditions with net fluxes at the Drichelet boundary conditions
		/// nbc.fluxes = dbc.water_balance(t)    
		/// # Update the potentials at the Drichelet boundary conditions
		/// dbc.potentials = storages.potentials 
		/// @endcode
		class NeumannBoundary_list
		{
		private:
			typedef std::vector<std::tr1::shared_ptr<NeumannBoundary> > vector;
			vector m_boundaries;
		public:
#ifndef SWIG
			NeumannBoundary& operator[](int index) const
			{ return *m_boundaries.at(index<0 ? m_boundaries.size()+index : index );	}
#endif
			/// Returns the Neumann boundary condition at position index
			NeumannBoundary_ptr get(int index) const
			{ return m_boundaries.at(index<0 ? m_boundaries.size()+index : index );	}

			/// Returns the fluxes of the items as an array
			cmf::math::num_array get_fluxes(cmf::math::Time t=cmf::math::Time()) const;
			void set_fluxes(cmf::math::num_array values);
			void append(NeumannBoundary_ptr nbc)
			{
				m_boundaries.push_back(nbc);
			}
			size_t size() const
			{
				return m_boundaries.size();
			}
			NeumannBoundary_list() {};
			NeumannBoundary_list(const cmf::water::node_list& copy);
			NeumannBoundary_list(const NeumannBoundary_list& copy);

			/// Creates a node_list from this NeumannBoundary_list
			cmf::water::node_list to_node_list() const;

			/// Returns the sum of the water balances of the nodes
			/// \f[\sigma_{global} = \sum_{i=0}^N{\sum_{j=0}^{C_i}{q_{ij}(t)}} \f]
			///
			/// Replaces slow Python code like:
			/// @code
			/// sum=0
			/// for n in nodes:
			///     sum+=n.water_balance(t)
			/// @endcode
			real global_water_balance(cmf::math::Time t) const;
			/// Returns the water balance of each vector as a vector
			/// \f[ \sigma_i = \sum_{j=0}^{C_i}{q_{ij}(t)} \f]
			///
			/// Replaces slow Python code like:
			/// @code
			/// balances=[n.water_balance(t) for n in nodes]
			/// @endcode
			cmf::math::num_array water_balance(cmf::math::Time t) const;


		};
	}
	
}
#endif // collections_h__
