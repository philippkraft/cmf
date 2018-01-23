'''
Converts the math in the docstrings created with doxy2swig in reSt math

philipp kraft
'''
from __future__ import division, print_function
import re
import sys
from traceback import format_exc as traceback

if len(sys.argv)<2:
    sys.stderr.write("Usage: python docstrings2rest.py docstrings.i >docstrings.i")
    exit(1)

# Regular expression to find inline math
patterninline =re.compile( r'\$(.*?)\$',flags=re.DOTALL)
# Regular expression to find block math
patternblock = re.compile(r'\\\\\\\\\[(.*?)\\\\\\\\\]\s*',flags=re.DOTALL)
# Regular expression to find eqnarray math
patternblock2 = re.compile(r'\\\\\\\\begin\{eqnarray\*\}(.*?)\\\\\\\\end\{eqnarray\*\}',flags=re.DOTALL)

# function to replace inline math with reSt syntax
def replinline(matchobj):
    # Convert all white space in inline to single space, remove outer white space
    # and remove whitespace before {. Otherwise reSt cannot handle the math.    
    res = (' '.join(matchobj.group(1).split())).strip().replace(' {','{')
    return ':math:`' + res + '`'
    
# function to replace block math with reSt syntax
def replblock(matchobj):                                                             
    return '\n\n.. math::\n\n    ' + '\n    '.join(matchobj.group(1).split('\n')) + '\n\n'       

# Read text from file
text = open(sys.argv[1]).read()

text = re.sub(patternblock2, replblock, text)
text = re.sub(patternblock, replblock, text)
text = re.sub(patterninline, replinline, text)

print(text)
