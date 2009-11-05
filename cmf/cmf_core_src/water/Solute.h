#ifndef Solute_h__
#define Solute_h__
#include <string>
#include <map>
#include <vector>
#include "../math/timeseries.h"
#include <string>
#include <sstream>
#include <stdexcept>
#include "../math/real.h"
namespace cmf

{
	/// Contains generic classes for %matter transport. You can use any unit for %matter amounts you like, like mol, g, kg, or none at all as long as you stay consistent. 
	/// But for the concentrations, the nominator of the unit must be \f$m^3\f$, e.g. \f$\frac{mol}{m^3},\frac{g}{m^3},\frac{1}{m^3}\f$. For simplicity, in the following the unit of concentrations
	/// are called \f$\frac{mol}{m^3}\f$.
	namespace water
	{
		class solute_vector;
		/// A structure to identify a solute
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
			bool operator==(const solute& cmp)
			{
				return this->Id == cmp.Id;
			}
			bool operator<(const solute& cmp)
			{
				return Id<cmp.Id;
			}
			bool operator>(const solute& cmp)
			{
				return Id>cmp.Id;
			}							
			bool operator<=(const solute& cmp)
			{
				return Id<cmp.Id;
			}
			bool operator>=(const solute& cmp)
			{
				return Id>=cmp.Id;
			}						
			bool operator!=(const solute& cmp)
			{
				return Id!=cmp.Id;
			}


			/// Name of the solute
			std::string Name;
			/// Used unit for amount of solute e.g. 'mol','mmol','g','mg' etc.
			std::string Unit;
			/// Fraction of the available concentration, that is taken up by vegetation
			/// - 0 means no uptake
			/// - 0..1 mean uptake is partially inhibited
			/// - 1 solute is taken up without changes in concentration
			/// - >1 preferential uptake (not tested, may result in negative concentrations)
			double Uptake;
			/// An automatically set identifier of the solute
			const size_t Id;
		};
		/// Manages the solutes of the model. 
		class solute_vector
		{
			std::vector<cmf::water::solute>	m_Solutes;
		public:
			size_t size() const {return m_Solutes.size();}
			/// Creates a solute vector from solute names, separated by whitespace. E.g. solutes=solute_vector("dO18 dH2")
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
	}
}
namespace cmf {
	namespace water {
		/// A map of concentration time series for solutes
		class SoluteTimeseries
		{
		private:
			typedef std::vector<cmf::math::timeseries> timeseriesVector;
			timeseriesVector conc_ts;
		public:
#ifndef SWIG
			/// Returns a reference to the time series of the solute
			cmf::math::timeseries& operator[](const cmf::water::solute& solute)
			{
				if (conc_ts.size()<=solute.Id)
					conc_ts.resize(solute.Id+1);
				return conc_ts[solute.Id];
			}
			cmf::math::timeseries operator[](const cmf::water::solute& solute) const
			{
				if (conc_ts.size()<=solute.Id)
					return cmf::math::timeseries();
				else
					return conc_ts[solute.Id];
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
			real conc(cmf::math::Time t,const cmf::water::solute& solute) const;
			/// Returns the number of solutes in the solution
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
