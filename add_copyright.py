
import os

Notice_py ="""

# Copyright 2010 by Philipp Kraft
# This file is part of cmf.
#
#   cmf is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 2 of the License, or
#   (at your option) any later version.
#
#   cmf is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with cmf.  If not, see <http://www.gnu.org/licenses/>.
#   
"""
Notice_cpp = Notice_py.replace('#','//')
for root,dirs,files in os.walk('cmf'):
    for f in files:
        fname = os.path.join(root,f)
        # f.endswith('.cpp') or f.endswith('.h') or
        if f.endswith('.i'):
            lines = file(fname).readlines()
            if not "// Copyright 2010 by Philipp Kraft" in lines:
                print f
                f=file(fname,'w')
                f.write(Notice_cpp)
                f.writelines(lines)
                f.close()
                
            else: print f, " has copyright already"
                
##         elif f.endswith('.py'):
##             lines = file(fname).readlines()
##             if not "# Copyright 2010 by Philipp Kraft" in lines:
##                 print f
##                 f=file(fname,'w')
##                 i=0
##                 
##                 while i<len(lines) and lines[i].strip().startswith('#'):
##                     f.write(lines[i])
##                     i+=1
##                 f.write(Notice_py)
##                 f.writelines(lines[i:])
##             else: print f, " has copyright already"
                    

        

        

        

        

