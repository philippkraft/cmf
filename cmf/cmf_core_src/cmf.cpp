

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


#ifdef _CONSOLE
	
#include "apps/reach_network.h"
#include <stdexcept>
#include <iostream>

int main(int argc, char* argv[])
{
	for (int i=1; i<=10; ++i)
		cmf::apps::run_reaches(i, cmf::apps::dense, 0);
	std::cin.ignore();

}
#endif //Not def _DLL

