

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
#ifndef Solute_h__
#define Solute_h__
#include <string>
#include <map>
#include <vector>
#include "../math/timeseries.h"
#include "../math/num_array.h"

#include <string>
#include <sstream>
#include <stdexcept>
#include "../math/real.h"
namespace cmf

{
	/// @brief Contains generic classes for %solute and %water transport. 
	///
	/// You can use any unit for %solute amounts you like, like mol, g, kg, or none at all as long as you stay consistent. 
	/// But for the concentrations, the nominator of the unit must be \f$m^3\f$, e.g. \f$\frac{mol}{m^3},\frac{g}{m^3},\frac{1}{m^3}\f$. For simplicity, in the following the unit of concentrations
	/// are called \f$\frac{mol}{m^3}\f$.
	namespace water
	{
		class solute_vector;
		/// @brief A structure to identify a solute
		struct solute
		{
		private:
			solute(size_t id,const std::string& name,const std::string& unit="" ,double uptake=1)
				: Name(name),Uptake(uptake),Unit(unit),Id(id)
			{
			}
			friend class solute_vector;
		public:
			solute(const solute& copy) : Name(copy.Name),Uptake(copy.Uptake),Unit(copy.Unit),Id(copy.Id) {}
#ifndef SWIG
			solute& operator=(const solute& copy);
#endif
			bool operator==(const solute& cmp) const
			{
				return this->Id == cmp.Id;
			}
			bool operator<(const solute& cmp)	const
			{
				return Id<cmp.Id;
			}
			bool operator>(const solute& cmp)	const
			{
				return Id>cmp.Id;
			}							
			bool operator<=(const solute& cmp) const
			{
				return Id<cmp.Id;
			}
			bool operator>=(const solute& cmp) const
			{
				return Id>=cmp.Id;
			}						
			bool operator!=(const solute& cmp) const
			{
				return Id!=cmp.Id;
			}


			/// @brief Name of the solute
			std::string Name;
			/// @brief Used unit for amount of solute e.g. 'mol','mmol','g','mg' etc.
			std::string Unit;
			/// @brief Fraction of the available concentration, that is taken up by vegetation
			/// - 0 means no uptake
			/// - 0..1 mean uptake is partially inhibited
			/// - 1 solute is taken up without changes in concentration
			/// - >1 preferential uptake (not tested, may result in negative concentrations)
			double Uptake;
			/// @brief An automatically set identifier of the solute
			const size_t Id;
		};
		/// @brief Manages the solutes of the model. 
		class solute_vector
		{
			std::vector<cmf::water::solute>	m_Solutes;
		public:
			size_t size() const {return m_Solutes.size();}
			/// @brief Creates a solute vector from solute names, separated by whitespace. E.g. solutes=solute_vector("dO18 dH2")
			solute_vector(std::string str);
#ifndef SWIG
			const solute& operator[](int i) const;
			typedef std::vector<solute>::const_iterator const_iterator;
			const_iterator begin() const {return m_Solutes.begin();}
			const_iterator end()   const {return m_Solutes.end();  }
#endif
			cmf::water::solute* get_solute(int position)
			{
				return &m_Solutes.at(position<0 ? position + size() : position);
			}
		};

		/// @brief A map of concentration time series for solutes
		class SoluteTimeseries
		{
		private:
			typedef std::vector<cmf::math::timeseries> timeseriesVector;
			timeseriesVector conc_ts;
		public:
#ifndef SWIG
			/// @brief Returns a reference to the time series of the solute
			cmf::math::timeseries& operator[](const cmf::water::solute& _Solute)
			{
				if (conc_ts.size()<=_Solute.Id)
					conc_ts.resize(_Solute.Id+1);
				return conc_ts[_Solute.Id];
			}
			cmf::math::timeseries operator[](const cmf::water::solute& _Solute) const
			{
				if (conc_ts.size()<=_Solute.Id)
					return cmf::math::timeseries();
				else
					return conc_ts[_Solute.Id];
			}
			// Fakes the std::map interface
			typedef timeseriesVector::iterator iterator;
			// Fakes the std::map interface
			iterator begin() {return conc_ts.begin();}
			// Fakes the std::map interface
			iterator end()   {return conc_ts.end();  } 
			// Fakes the std::map interface
			typedef timeseriesVector::const_iterator const_iterator;
			// Fakes the std::map interface
			const_iterator begin() const {return const_iterator(conc_ts.begin());}
			// Fakes the std::map interface
			const_iterator end()   const {return const_iterator(conc_ts.end());  } 
#endif
			real conc(cmf::math::Time t,const cmf::water::solute& _Solute) const;
			/// @brief Returns the number of solutes in the solution
			size_t size() const
			{
				return conc_ts.size();
			}
			SoluteTimeseries() 
				: conc_ts() 
			{}
			SoluteTimeseries(const solute_vector& solutes,cmf::math::Time begin,cmf::math::Time step) : conc_ts() 
			{
				for (solute_vector::const_iterator it = solutes.begin();it!=solutes.end();++it)
				{
					conc_ts.push_back(cmf::math::timeseries(begin,step));
				}

			}

			SoluteTimeseries(const cmf::water::SoluteTimeseries& sts) : conc_ts(sts.begin(),sts.end()) {}
		};
	}
}
#endif // Solute_h__
