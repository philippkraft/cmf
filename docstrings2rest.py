'''
Converts the math in the docstrings created with doxy2swig in reSt math

philipp kraft
'''

import re
import sys
from traceback import format_exc as traceback

if len(sys.argv)<2:
    sys.stderr.write("Usage: python docstrings2rest.py docstrings.i >docstrings.i")
    exit(1)

# Regular expression to find inline math
patterninline =re.compile( r'\$(.*?)\$')
# Regular expression to find block math
patternblock = re.compile(r'\\\\\\\\\[(.*?)\\\\\\\\\]',flags=re.DOTALL)
# Regular expression to find eqnarray math
patternblock2 = re.compile(r'\\\\\\\\begin\{eqnarray\*\}(.*?)\\\\\\\\end\{eqnarray\*\}',flags=re.DOTALL)

# function to replace inline math with reSt syntax
def replinline(matchobj):                                                            
    return ':math:`' + matchobj.group(1) + '`'
    
# function to replace block math with reSt syntax
def replblock(matchobj):                                                             
    return '\n\n.. math::\n\n  ' + '\n  '.join(matchobj.group(1).split('\n')) + '\n.\n'       

# Read text from file
text = file(sys.argv[1]).read()

text = re.sub(patternblock2, replblock, text)
text = re.sub(patternblock, replblock, text)
text = re.sub(patterninline, replinline, text)

print text
