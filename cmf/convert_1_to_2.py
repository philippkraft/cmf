"""
Converts a Python program using cmf from cmf version 1.x to 2.0.
"""
import cmf
import sys
import re
import io


def regex_escape(seq)->list:
    return [
        re.escape(k)
        for k in seq
    ]
rename = {
    'cmf.TechnicalFlux': 'cmf.ConstantFlux',
    'cmf.generic_gradient_connection': 'LinearGradientFlux',
    'cmf.statecontrol_connection': 'cmf.ConstantStateFlux',
    'cmf.waterbalance_connection': 'cmf.WaterBalanceFlux',
    'cmf.SimpleInfiltration': 'cmf.ConceptualInfiltration',
    'cmf.external_control_connection': 'cmf.ExternallyControlledFlux',
    'cmf.SimpleTindexSnowMelt': 'cmf.TempIndexSnowMelt',
    'cmf.CVodeIntegrator': 'cmf.CVodeKrylov',
    'cmf.SimplRichards': 'cmf.FreeDrainagePercolation',
}

warning = {
    'cmf.bidirectional_kinematic_exchange':
        'no replacement available. Inform the authors if needed',
    'cmf.constraint_kinematic_wave':
        'consider to use cmf.ConstraintLinearStorageFlux',
    'cmf.kinematic_wave':
        'consider to use cmf.cmf.LinearStorageConnection and cmf.PowerLawConnection',
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


def convert_1_to_2(stream_in=sys.stdin, stream_out=sys.stdout):
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
            sys.stderr.write(warn_text + 'l:{:4d} {} -> {}\n'.format(i+1, match, warn or repl))
        stream_out.write(new_line)

if __name__ == '__main__':

    if len(sys.argv) > 1:
        if sys.argv[1] == '-i' and len(sys.argv) > 2:
            stream_in = io.StringIO(open(sys.argv[2]).read())
        else:
            stream_in = open(sys.argv[1])
    else:
        stream_in = sys.stdin
    if len(sys.argv) > 2:
        if sys.argv[-1] == 'test':
            stream_out = io.StringIO()
        else:
            stream_out = open(sys.argv[-1], 'w')
    else:
        stream_out = sys.stdout
    convert_1_to_2(stream_in, stream_out)
    stream_out.close()
    stream_in.close()



