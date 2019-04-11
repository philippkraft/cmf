#!/usr/bin/env python3

import sys
from pathlib import Path
if len(sys.argv) < 2:
    sys.stderr('Usage: abspath.py <file1> <file2> ...')
for sp in sys.argv[1:]:
    p = Path(sp)
    print(p.absolute().resolve())

