

// Copyright 2010 by Philipp Kraft
// This file is part of cmf.
//
//   cmf is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
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
#ifndef flux_connection_h__
#define flux_connection_h__
#include "../math/time.h"
#include "../math/real.h"
#include "../math/geometry.h"
#include "Solute.h"
#include <map>
#include <vector>
#include <string>
#include "flux_node.h"
#include <stdexcept>
#include <memory>
namespace cmf {
	class project;
	namespace water {

		/// @defgroup connections Flux connections
		///
		/// @brief The connections between the nodes (boundary conditions, storages) of the water network
        ///
		/// The connections in cmf hold the processes for the calculation of fluxes between water storages and model boundaries
		///
		/// @ingroup connections
		/// Represents a connection between flux_nodes, where water fluxes occur. 
		class flux_connection 
		{
		protected:
			typedef std::weak_ptr<flux_node> weak_flux_node_ptr;
		private:
			friend class flux_node;
			static int nextconnectionid;
			flux_connection(const flux_connection& copy):connection_id(nextconnectionid) {
				throw std::runtime_error("Never copy construct a flux_connection");
			}
			std::weak_ptr<flux_connection> weak_this;
			weak_flux_node_ptr m_left;
			weak_flux_node_ptr m_right;
			real m_tracer_filter;
			std::map < cmf::water::solute, real > m_tracer_filter_map;
		protected:
			virtual void NewNodes() {};
			bool RecalcAlways;
			virtual real calc_q(cmf::math::Time t) = 0;
			real m_q; // Positive q means flux left to right
			real q(cmf::math::Time t);
			double prevent_negative_volume(double flow) {
				real empty = 1.0;
				if (flow>0)
					empty=left_node()->is_empty();
				else
					empty=right_node()->is_empty();
				return flow * (1 - empty);
			}


		public:
			typedef std::shared_ptr<flux_connection> ptr;
#ifndef SWIG
			ptr get_ptr() const {
				return weak_this.lock();
			}
			
#endif
			/// @brief Returns the left node of this connection
			flux_node::ptr left_node() const {return m_left.expired() ? flux_node::ptr() : flux_node::ptr(m_left);}
			/// @brief returns the right node of this connection
			flux_node::ptr right_node() const {return m_right.expired() ? flux_node::ptr() : flux_node::ptr(m_right);}
			
			/// @brief Deregisters this connection from its nodes. Returns true if only one reference is left.
			bool kill_me();
			/// @brief Performes a new calculation of the flux
			void refresh(cmf::math::Time t) {
				m_q = calc_q(t);
			}


			const int connection_id;
			
			bool operator==(const cmf::water::flux_connection& other) {return connection_id==other.connection_id;}
			
			/// @brief Returns the other end of a connection than the asking end
			flux_node::ptr get_target(const flux_node& inquirer);
			/// @brief With index 0, the left node is returned, with index 1 the right node of the connection
			flux_node::ptr get_target(int index) const;
			
			void exchange_target(flux_node::ptr oldtarget,flux_node::ptr newTarget);
			
			
			/// Returns the current flux through a connection. Negative signs mean out of the inquirer, positive are inflows to the inquirer
			real q(const flux_node& inquirer,cmf::math::Time t) {
				return inquirer==*left_node() ? -q(t) 
					: inquirer==*right_node() ? q(t) : 0;  
			}
			
			/// @brief Returns the concentration of the flux. 
            ///
            /// If not overridden, it returns the concentration of the source of the flux (direction depending)
			real conc(cmf::math::Time t, const cmf::water::solute& _Solute);
			/// @brief A string describing the type of the connection
			const std::string type;
			/// A value ranging from 0 to 1 to filter tracers out of the water flux. 
			///
			/// 1.0 is no filter and 0.0 means no solute is crossing this connection
			real get_tracer_filter() { return m_tracer_filter;}
			
			/// A value ranging from 0 to 1 to filter tracers out of the water flux. 
			///
			real get_tracer_filter(solute S);
			/// A value ranging from 0 to 1 to filter tracers out of the water flux
			///
			/// 1.0 is no filter and 0.0 means no solute is crossing this connection
			void set_tracer_filter(real value) { 
				if (value<0 || value>1) throw std::runtime_error("Tracer filter must be between 0 and 1");
				m_tracer_filter = value;
			}
			void set_tracer_filter(solute S, real value) {
				if (value<0 || value>1) throw std::runtime_error("Tracer filter must be between 0 and 1");
				m_tracer_filter_map[S] = value;
			}
			virtual std::string to_string() const;
			virtual std::string short_string() const;
			/// @brief Creates a flux connection between the flux_node left and right
			/// @param left flux_node on the one side of the connection
			/// @param right flux_node on the other side of the connection
			/// @param _type Type of the flux connection
			flux_connection(flux_node::ptr left,flux_node::ptr right,std::string _type);

			virtual ~flux_connection();
		};

		int replace_node(cmf::water::flux_node::ptr oldnode,cmf::water::flux_node::ptr newnode);

		/// A self sorting list of connections
		class connection_list  
		{
		private:
			typedef std::vector<flux_connection::ptr> _list;
			_list m_con;
		public:
			typedef _list::iterator iterator;
			typedef _list::const_iterator const_iterator;
			bool append(cmf::water::flux_connection::ptr connection);
			void extend(const connection_list& connections);
			bool contains(cmf::water::flux_connection::ptr connection) const;
			bool remove(cmf::water::flux_connection::ptr connection);
			void do_action(cmf::math::Time t, bool use_OpenMP = true);
			size_t size() const { return m_con.size();}

#ifndef SWIG
			cmf::water::flux_connection::ptr operator[](size_t pos) const {
				return m_con[pos];
			}
#endif
			cmf::water::flux_connection::ptr at(size_t pos) const { return m_con.at(pos); }
			iterator begin() {
				return m_con.begin();
			}
			iterator end() {
				return m_con.end();
			}
			const_iterator begin() const {
				return m_con.begin();
			}
			const_iterator end() const {
				return m_con.end();
			}
			virtual ~connection_list()
			{

			}
		};



	}
}

#endif // flux_connection_h__
