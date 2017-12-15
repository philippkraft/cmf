

// Copyright 2010 by Philipp Kraft
// This file is part of cmf.
//
//   cmf is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 2 of the License, or
//   (at your option) any later version.
//
//   cmf is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with cmf.  If not, see <http://www.gnu.org/licenses/>.
//   
#ifndef collections_h__
#define collections_h__
#include "flux_node.h"
#include "boundary_condition.h"
#include "../geometry/geometry.h"
#include "../math/num_array.h"
namespace cmf {
	namespace water {

		/// @brief A collection of nodes for fast access of the waterbalance
		///
		/// In setups with many storages and rather fast computations, the speed of data access for output generation can take a high portion of the total run time.
		/// To accelerate data access, one can use the node_list object
		///
		/// @todo Add a get_volume / set_volume function pair, to complement get_potential / set_potential
		class node_list	: public cmf::math::StateVariableOwner
		{
		private:

			typedef std::vector<flux_node::ptr> node_vector;
			node_vector m_nodes;
		public:
			/// @brief The number of nodes
			size_t size() const {return m_nodes.size();}

#ifndef SWIG
			flux_node::ptr operator[](ptrdiff_t index) const
			{
				return m_nodes.at(index<0 ? size()+index : index);
			}
			node_vector::iterator begin() {return m_nodes.begin();}
			node_vector::iterator end() {return m_nodes.end();}
			node_vector::const_iterator begin() const {return m_nodes.begin();}
			node_vector::const_iterator end() const {return m_nodes.end();}
			typedef node_vector::iterator iterator;
			typedef node_vector::const_iterator const_iterator;

#endif
			/// @brief Creates an empty node_lust			
			node_list() {}
			/// @brief Copy the node_list
			node_list(const cmf::water::node_list& forcopy)
				: m_nodes(forcopy.m_nodes) {}

			/// @brief Extends the node_list with another
			node_list& operator+=(const cmf::water::node_list& right);
			
			/// @brief Returns the union of two node_lists
			cmf::water::node_list operator+(const cmf::water::node_list & right) const;
			
			/// @brief Returns a node in the node_list
			flux_node::ptr get(ptrdiff_t index) const;
			
			/// @brief Returns a slice of the node_list
			node_list getslice(ptrdiff_t begin,ptrdiff_t end,ptrdiff_t step=1) const;

			/// @brief Implements StateVariableOwner
			cmf::math::StateVariableList get_states();
			/// @brief Adds a flux node to the list
			void append(flux_node::ptr node);
			/// @brief Removes a flux node from the list, returns true if successful
			bool remove(flux_node::ptr node);

			/// @brief Sets the potentials of the node_list
			///
			/// If nodes do not have changeable potentials, they are skipped silently
			/// @returns The number of nodes with changed potential
			ptrdiff_t set_potentials(const cmf::math::num_array& potentials);
			
			/// @brief Returns the potential of the nodes
			cmf::math::num_array get_potentials();
            

			/// @brief Returns the sum of the water balances of the nodes
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
						
			/// @brief Returns the water balance of each vector as a vector
			///
			/// \f[ \sigma_i = \sum_{j=0}^{C_i}{q_{ij}(t)} \f]
			///
			/// Replaces slow Python code like:
			/// @code
			/// balances=[n.water_balance(t) for n in nodes]
			/// @endcode
			cmf::math::num_array water_balance(cmf::math::Time t) const;

			/// @brief Returns an array holding the concentration of all the flux nodes for the given solute
			cmf::math::num_array conc(cmf::math::Time t, const cmf::water::solute& _Solute) const;
			
			/// @brief Sets the source flux of a solute storage associated  with a node (node has to be a water storage)
			ptrdiff_t set_solute_source(const cmf::water::solute& _Solute, cmf::math::num_array source_fluxes);

			/// @brief A fast method to perform flux queries as a batch. The node lists left and right should have the same length.
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
			
			/// @brief Returns the flux vectors to the nodes of a given target node_list
			cmf::geometry::point_vector get_fluxes3d_to( const cmf::water::node_list& targets ,cmf::math::Time t) const;
			
			/// @brief Returns the current flow vector for each node
			cmf::geometry::point_vector get_fluxes3d(cmf::math::Time t) const;
			
			/// @brief Returns the positions of the nodes
			cmf::geometry::point_vector get_positions() const;
			virtual ~node_list()
			{

			}
		};


		/// @brief Provides fast access to Neumann boundaries for flux update
		///
		/// If many Neumann boundary conditions are present in a project, a fast data exchange to update the fluxes might be needed.
		///
		/// With this specialized list a num_array can be passed to the boundary conditions for a fast flux update
		/// If a multiple system layout for the cmf setup is chosen, we might have a 
		/// node_list Dirichlet boundary conditions (dbc), a corresponding NeumannBoundary_list (nbc) of Neumann boundaries and a node_list
		/// containing the storages connected with the NeumannBoundary_list (storages). The fast data exchange is written in Python as:
		/// @code
		/// # Update the fluxes at the Neumann boundary conditions with net fluxes at the Dirichlet boundary conditions
		/// nbc.fluxes = dbc.water_balance(t)    
		/// # Update the potentials at the Dirichlet boundary conditions
		/// dbc.potentials = storages.potentials 
		/// @endcode
		class NeumannBoundary_list
		{
		private:
			typedef std::vector<std::shared_ptr<NeumannBoundary> > vector;
			vector m_boundaries;
		public:
#ifndef SWIG
			NeumannBoundary& operator[](ptrdiff_t index) const
			{ return *m_boundaries.at(index<0 ? m_boundaries.size()+index : index );	}
#endif
			/// @brief Returns the Neumann boundary condition at position index
			///
			/// From Python you can use []
			NeumannBoundary::ptr get(ptrdiff_t index) const
			{ return m_boundaries.at(index<0 ? m_boundaries.size()+index : index );	}

			/// @brief Returns the fluxes of the items as an array
			///
			/// get_fluxes and set_fluxes are wrapped with the Python property fluxes
			cmf::math::num_array get_fluxes(cmf::math::Time t=cmf::math::Time()) const;
			/// @brief Sets the fluxes of the items from an array. 
			///
			/// get_fluxes and set_fluxes are wrapped with the Python property fluxes
			void set_fluxes(cmf::math::num_array values);
			///@brief Appends a neumann boundary to this list
			void append(NeumannBoundary::ptr nbc)
			{
				m_boundaries.push_back(nbc);
			}
			///@brief returns the number of stored boundary conditions
			size_t size() const
			{
				return m_boundaries.size();
			}
			NeumannBoundary_list() {};
			NeumannBoundary_list(const cmf::water::node_list& copy);
			NeumannBoundary_list(const NeumannBoundary_list& copy);

			/// @brief Creates a node_list from this NeumannBoundary_list
			cmf::water::node_list to_node_list() const;

			/// @brief Returns the sum of the water balances of the nodes
			/// \f[\sigma_{global} = \sum_{i=0}^N{\sum_{j=0}^{C_i}{q_{ij}(t)}} \f]
			///
			/// Replaces slow Python code like:
			/// @code
			/// sum=0
			/// for n in nodes:
			///     sum+=n.water_balance(t)
			/// @endcode
			real global_water_balance(cmf::math::Time t) const;
			/// @brief Returns the water balance of each vector as a vector
			/// \f[ \sigma_i = \sum_{j=0}^{C_i}{q_{ij}(t)} \f]
			///
			/// @brief Replaces slow Python code like:
			/// @code
			/// balances=[n.water_balance(t) for n in nodes]
			/// @endcode
			cmf::math::num_array water_balance(cmf::math::Time t) const;


		};
		
	}

	
}
#endif // collections_h__
