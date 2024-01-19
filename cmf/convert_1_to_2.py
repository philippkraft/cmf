"""
Converts a Python program using cmf from cmf version 1.x to 2.0.
"""
import sys
import re
import io


def regex_escape(seq)->list:
    return [
        re.escape(k)
        for k in seq
    ]
rename = {
    'TechnicalFlux': 'ConstantFlux',
    'generic_gradient_connection': 'LinearGradientFlux',
    'statecontrol_connection': 'ConstantStateFlux',
    'waterbalance_connection': 'WaterbalanceFlux',
    'SimpleInfiltration': 'ConceptualInfiltration',
    'external_control_connection': 'ExternallyControlledFlux',
    'SimpleTindexSnowMelt': 'TempIndexSnowMelt',
    'CVodeIntegrator': 'CVodeKrylov',
    'SimplRichards': 'FreeDrainagePercolation',
    '.AsPython': '.as_datetime',
    'AsCMFTime': 'datetime_to_cmf'
}

warning = {
    'bidirectional_kinematic_exchange':
        'no replacement available. Inform the authors if needed',
    'constraint_kinematic_wave':
        'consider to use cmf.ConstraintLinearStorageFlux',
    'kinematic_wave':
        'consider to use cmf.LinearStorageConnection and cmf.PowerLawConnection',
}


def get_pattern():
    keys = regex_escape(rename.keys()) + regex_escape(warning.keys())
    return re.compile("|".join(keys))


def repl_or_warn(text: str, pattern: re.Pattern):
    m = pattern.search(text)
    if not m:
        return '', '', ''
    g = m.group(0)
    if g in rename:
        return g, rename[g], ''
    elif g in warning:
        return g, g, warning[g].split('\n')[0]
    else:
        raise KeyError(g + ' not in warnings or rename dictionary')


def convert_1_to_2(stream_in=sys.stdin, stream_out=sys.stdout, fn=''):
    pattern = get_pattern()
    for i, line in enumerate(stream_in):
        match, repl, warn = repl_or_warn(line, pattern)
        if repl:
            new_line = pattern.sub(lambda m: repl, line)
        else:
            new_line = line
        if warn:
            new_line = new_line.rstrip('\n') + '  # TODO: replace {}, {}\n'.format(match, warn)
        if match:
            warn_text = 'WARNING: ' if warn else ''
            sys.stderr.write(fn + ' ' + warn_text + 'l:{:4d} {} -> {}\n'.format(i+1, match, warn or repl))
        stream_out.write(new_line)


if __name__ == '__main__':

    if len(sys.argv) > 1:
        if sys.argv[1] == '-i' and len(sys.argv) > 2:
            stream_in = io.StringIO(open(sys.argv[2]).read())
            fn = sys.argv[2]
        else:
            stream_in = open(sys.argv[1])
            fn = sys.argv[1]
    else:
        stream_in = sys.stdin
    if len(sys.argv) > 2:
        if sys.argv[-1] == 'test':
            stream_out = io.StringIO()
        else:
            stream_out = open(sys.argv[-1], 'w')
    else:
        stream_out = sys.stdout
    convert_1_to_2(stream_in, stream_out, fn)
    stream_out.close()
    stream_in.close()

else:
    from textwrap import dedent
    from . import *
    from logging import warning
    template = dedent('''
    def DeprecatedName(*args, **kwargs):
        """
        DeprecatedName is deprecated, use NewName instead
        """
        warning('DeprecatedName is deprecated use NewName instead. Creating NewName. \n' \
                'Consider to use convert_1_to_2.py to convert your script automatically')
        return NewName(*args, **kwargs)
    ''')
    for k, v in rename.items():
        if not k.startswith('.'):
            eval(template.replace(k, v))

    





