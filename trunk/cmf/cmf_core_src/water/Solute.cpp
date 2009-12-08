#include "Solute.h"
#include <iostream>



cmf::water::solute& cmf::water::solute::operator=( const solute& copy )
{
	throw std::runtime_error("Never assign a solute");
}


cmf::water::solute_vector::solute_vector( std::string str )
{
	using namespace std;
	string buf;
	int i=0;
	stringstream ss(str); // Insert the string into a stream
	while (ss >> buf)
	{
		m_Solutes.push_back(solute(i++,buf));
	}
}

const cmf::water::solute& cmf::water::solute_vector::operator[]( int position ) const
{
	return m_Solutes.at(position<0 ? position + size() : position);
}

real cmf::water::SoluteTimeseries::conc( cmf::math::Time t,const cmf::water::solute& solute ) const
{
	if (conc_ts.size()<=solute.Id || conc_ts[solute.Id].is_empty())
		return 0.0;
	else
		return conc_ts[solute.Id][t];
}