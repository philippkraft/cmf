#ifndef Solute_h__
#define Solute_h__
#include <string>
#include <map>
#include <vector>
#include "../math/timeseries.h"
#include <string>
#include <sstream>
namespace cmf
{
	/// Contains generic classes for %matter transport. You can use any unit for %matter amounts you like, like mol, g, kg, or none at all as long as you stay consistent. 
	/// But for the concentrations, the nominator of the unit must be \f$m^3\f$, e.g. \f$\frac{mol}{m^3},\frac{g}{m^3},\frac{1}{m^3}\f$. For simplicity, in the following the unit of concentrations
	/// are called \f$\frac{mol}{m^3}\f$.
	namespace water
	{
		class Solutes;
		/// A structure to identify a solute
		struct Solute
		{
		private:
			Solute(size_t id) :Id(id), Name(""),Uptake(1),Unit("") {}
			Solute(size_t id,const std::string& name="",const std::string& unit="" ,double uptake=1)
				: Name(name),Uptake(uptake),Unit(unit),m_Id(id),Id(m_Id)
			{
			}
			size_t m_Id;
		public:
			Solute(const Solute& copy) : Name(copy.Name),Uptake(copy.Uptake),Unit(copy.Unit),m_Id(copy.Id),Id(m_Id) {}
#ifndef SWIG
			Solute& operator=(const Solute& copy);
#endif
			friend class Solutes;
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
			const size_t & Id;
			static const Solute& create(const std::string& name,const std::string& unit="",double uptake=1);
		};
		/// Manages the solutes of the whole model. The static member All is the only instance. Before you  are creating any cmf object 
		/// (like UpslopeCell, Reach or IVegetation or Meteorology), make sure you have added all solutes you are interest in to Solutes::All
		/// Generally you are free to add any kind of Solute to the model, but each additional will slow down calculations remarkably, and if you 
		/// intend to use one or more ExternalFunctionReaction special requirements apply.
		class Solutes
		{
			std::vector<Solute>	m_Solutes;
			bool InUse;
			static Solutes m_All;
			Solutes() : InUse(false) {}

		public:
			/// Creates a solute in Solute::All
			/// @param name the name of the solute, e.g. 'Tracer', 'NO3'
			/// @param unit Used unit for amount of solute e.g. 'mol','mmol','g','mg' etc.
			/// @param uptake Fraction of the available concentration, that is taken up by vegetation
			const Solute& add(const std::string& name,const std::string& unit="",double uptake=1);
			size_t size() {return m_Solutes.size();}
			/// If the Solutes of the model are marked as "InUse", no more solutes may be added. They get marked as "InUse" as soon as an object depending on the
			/// number of solutes is created (e.g. an UpslopeCell, Reach, IVegetation or Meteorology)
			void SetInUse();
#ifndef SWIG
			const Solute& operator[](int i) const
			{
				size_t ndx;
				if (i<0) 
					ndx=size_t(m_Solutes.size()+i);
				else
					ndx=size_t(i);
				return m_Solutes[ndx];
			}
			Solute& operator[](int i)
			{
				size_t ndx=i;
				if (i<0) ndx=size_t(m_Solutes.size()+i);
				return m_Solutes[ndx];
			}
			typedef std::vector<Solute>::iterator iterator;
			iterator begin() {return m_Solutes.begin();}
			iterator end()   {return m_Solutes.end();  }
			typedef std::vector<Solute>::const_iterator const_iterator;
			const_iterator begin() const {return m_Solutes.begin();}
			const_iterator end()   const {return m_Solutes.end();  }
#endif
			static Solutes& all() {return Solutes::m_All;}
		};
	}
}

namespace cmf {
	namespace water {	
		/// Map of concentrations for different solutes
		class WaterQuality 
		{
		private:
			typedef std::vector<double> Concentrations;
			Concentrations	conc;
		public:
#ifndef SWIG
			/// Returns a reference to the concentration of the given Solute
			double& operator[](const cmf::water::Solute& solute)
			{
				return conc[solute.Id];
			}
			double operator[](const cmf::water::Solute& solute) const
			{
				return conc[solute.Id];
			}
			/// Returns a reference to the concentration of the given Solute
			double& operator[](size_t i)
			{
				return conc[i];
			}
			double operator[](size_t i) const
			{
				return conc[i];
			}

			WaterQuality& operator=(const cmf::water::WaterQuality& right)
			{
				//map.clear();
				conc=Concentrations(right.begin(),right.end());
				return *this;
			}
			typedef Concentrations::iterator iterator;
			iterator begin() {return conc.begin();}
			iterator end()   {return conc.end();  } 
			typedef Concentrations::const_iterator const_iterator;
			const_iterator begin() const {return const_iterator(conc.begin());}
			const_iterator end()   const {return const_iterator(conc.end());  } 

#endif
			/// Returns the number of solutes
			size_t size() const
			{
				return conc.size();
			}
			std::string tostring() const
			{
				std::stringstream out;
				out.precision(4);
				for (int i = 0; i < int(size()) ; ++i)
				{				 
					out << Solutes::all()[i].Name << ": " << conc[i] << " " << Solutes::all()[i].Unit << " ";
				}
				return out.str();
			}

			/// Clears the water
			void clear();

		  /// Standard constructor
			WaterQuality() : conc(cmf::water::Solutes::all().size(),0) {/*cmf::water::Solutes::all().SetInUse();*/}
			WaterQuality(const cmf::water::WaterQuality& wq) : conc(wq.conc) {}
		};
		/// A map of concentration time series for solutes
		class SoluteTimeseries
		{
		private:
			typedef std::vector<cmf::math::timeseries> timeseriesVector;
			timeseriesVector conc_ts;
		public:
#ifndef SWIG
			/// Returns a reference to the time series of the solute
			cmf::math::timeseries& operator[](const cmf::water::Solute& solute)
			{
				return conc_ts[solute.Id];
			}
			const cmf::math::timeseries& operator[](const cmf::water::Solute& solute) const
			{
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
			WaterQuality conc(cmf::math::Time t) const
			{
				WaterQuality res;
				for(Solutes::const_iterator it = Solutes::all().begin(); it != Solutes::all().end(); ++it)
				{
					//cmf::math::timeseries& ts=(*this)[*it];
					res[*it]=(*this)[*it][t];
				}
				return res;
			}
			/// Returns the number of solutes in the solution
			size_t size() const
			{
				return conc_ts.size();
			}
			SoluteTimeseries() : conc_ts(cmf::water::Solutes::all().size()) {cmf::water::Solutes::all().SetInUse();}
			SoluteTimeseries(cmf::math::Time begin,cmf::math::Time step) : conc_ts() 
			{
				cmf::water::Solutes::all().SetInUse();
				for (Solutes::const_iterator it = Solutes::all().begin();it!=Solutes::all().end();++it)
				{
					conc_ts.push_back(cmf::math::timeseries(begin,step));
				}

			}

			SoluteTimeseries(const cmf::water::SoluteTimeseries& sts) : conc_ts(sts.begin(),sts.end()) {}
		};
	}
}
#ifdef SWIG
%extend cmf::water::Solutes
{
	Solute& __getitem__(int i)
	{
		return (*$self)[i];
	}
}
%extend cmf::water::WaterQuality
{
	double __getitem__(const cmf::water::Solute& solute)
	{
		return (*$self)[solute];
	}
	void __setitem__(const cmf::water::Solute& solute,double concentration)
	{
		(*$self)[solute]=concentration;
	}
	size_t __len__() const
	{
		return $self->size();
	}
	std::string __str__() const
	{
		return $self->tostring();
	}
}
%extend cmf::water::SoluteTimeseries
{
	cmf::math::timeseries& __getitem__(const cmf::water::Solute& solute)
	{
		return (*$self)[solute];
	}
	void __setitem__(const cmf::water::Solute& solute,cmf::math::timeseries concentration)
	{
		(*$self)[solute]=concentration;
	}
	size_t __len__() const
	{
		return $self->size();
	}
}
#endif
#endif // Solute_h__
