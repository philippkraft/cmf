%include "stl.i"

// enable exception support
%include "exception.i"
%exception {
    try {
        $action
    } 
    SWIG_CATCH_STDEXCEPT
}

%module cmf_math
%include "math/math.i"
