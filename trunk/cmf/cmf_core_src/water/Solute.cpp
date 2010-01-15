

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