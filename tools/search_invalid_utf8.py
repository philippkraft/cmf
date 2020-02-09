import sys
import glob
if __name__ == '__main__':

    for fn in glob.glob(sys.argv[1]):
        btext = open(fn, 'rb').read()
        blist=btext.split(b'\n')
        for i, l in enumerate(blist):
            try:
                lt = l.decode('utf-8')
            except UnicodeDecodeError:
                sys.stderr.write(f'{fn}:{i:4} {l[:30]!r}\n')
