
// File: index.xml

// File: classcmf_1_1water_1_1_adsorption.xml
%feature("docstring") cmf::water::Adsorption "

Abstract class to use adsorption process for tracers on surfaces.

Use the derived classes to use a certain isotherm

C++ includes: adsorption.h ";

%feature("docstring")  cmf::water::Adsorption::~Adsorption "virtual
~Adsorption() ";

%feature("docstring")  cmf::water::Adsorption::copy "virtual
Adsorption* copy(real m=-1) const =0

returns a copy of the Adsorption object.

If the adsorption is depending on the sorbent mass, you can give a
positive value for the sorbent mass m. If the value is not given or
negative, m is used from the original object. ";

%feature("docstring")  cmf::water::Adsorption::freesolute "virtual
real freesolute(real xt, real V) const =0

Returns the mass of dissolved tracer as a function of the total tracer
mass in the solute storage and the water volume.

Parameters:
-----------

xt:   :math:`x_t` the total tracer mass in the storage

V:   :math:`V m^3` the water volume in the storage

:math:`x_f` the dissolved mass of the tracer ";

%feature("docstring")  cmf::water::Adsorption::totalsolute "virtual
real totalsolute(real xf, real V) const =0

Returns the total mass of the tracer from the dissolved concetration
in tracer unit/m3.

Parameters:
-----------

xf:   :math:`x_f` the dissolved tracer mass in the storage

V:   :math:`V m^3` the water volume in the storage

:math:`x_t` the total mass of the tracer ";


// File: classcmf_1_1atmosphere_1_1aerodynamic__resistance.xml
%feature("docstring") cmf::atmosphere::aerodynamic_resistance "

Abstract class. Child classes can be used to calculate aerodynamic
resistances against turbulent heat fluxes.

C++ includes: meteorology.h ";

%feature("docstring")
cmf::atmosphere::aerodynamic_resistance::~aerodynamic_resistance "virtual ~aerodynamic_resistance() ";

%feature("docstring")
cmf::atmosphere::aerodynamic_resistance::get_aerodynamic_resistance "virtual void get_aerodynamic_resistance(double &r_ag, double &r_ac,
cmf::math::Time t) const =0

aerodynamic resistance from ground to atmosphere (r_ag) and from
canopy to atmosphere (r_ac) ";


// File: classcmf_1_1draw_1_1cellmap_1_1_animator.xml
%feature("docstring") cmf::draw::cellmap::Animator "";

%feature("docstring")  cmf::draw::cellmap::Animator::__init__ "def
__init__(self, cells, solver, start, end, step)

:param cells: A sequence of cmf Cells, can be a project :param solver:
A cmf solver Eg. cmf.CVodeDense() :param start: Start time :param end:
End time :param step: Time step ";

%feature("docstring")  cmf::draw::cellmap::Animator::__call__ "def
__call__(self, args, kwargs) ";

%feature("docstring")  cmf::draw::cellmap::Animator::draw "def
draw(self, frame=None) ";


// File: classcmf_1_1upslope_1_1aquifer.xml
%feature("docstring") cmf::upslope::aquifer "

A class to represent large groundwater storages, not bounded to the
usual horizontal discretization scheme, the Cell.

Naturally aquifers are connected with aquifer_Darcy or kinematic_wave
connections. As a boundary condition for a cell based soil water
system, aquifers are used as a right hand side node of percolation
connections.

Basic head ( :math:`\\\\Psi`) / volume ( :math:`V`) relation: 

.. math::

     \\\\Psi =
    z_{base} + \\\\frac {V}{A \\\\Phi} 

:math:`\\\\Psi` water head in m

:math:`z_{base}` base height of the aquifer

:math:`V` volume of stored water in m3

:math:`A` Base area of the aquifer in m2

:math:`\\\\Phi` Porosity, or more general, :math:`\\\\frac{dV_{bulk}}{dV_{H_2O}}`

C++ includes: groundwater.h ";

/*  Overrides of flux_node  */

%feature("docstring")  cmf::upslope::aquifer::RecalcFluxes "virtual
bool RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node.

WaterStorage overrides this, since state changes require an update of
the fluxes ";

%feature("docstring")  cmf::upslope::aquifer::is_empty "virtual
double is_empty() const

Returns true if the node has no water. ";

%feature("docstring")  cmf::upslope::aquifer::to_string "virtual
std::string to_string() const ";

%feature("docstring")  cmf::upslope::aquifer::aquifer "aquifer(cmf::project &p, cmf::geometry::point position, real area,
real thickness, real porosity, real K=1e-4)

Creates an aquifer at a certain position.

Parameters:
-----------

p:  Project, this aquifer belongs to

position:  Position of the aquifer center (x and y) and the aquifer
top (z)

area:  Area of the aquifer

thickness:  Mean thickness of the aquifer in m

porosity:  Mean porositiy of the aquifer

K:  Conductivity of the aquifer in m/day. If the conductivity should
be anisotropic, change the x,y and z values of the member K ";

%feature("docstring")  cmf::upslope::aquifer::aquifer "aquifer(cmf::upslope::cell_vector &cells, real thickness, real
porosity, real K=1e-4)

Creates an aquifer below a collection of cells.

Parameters:
-----------

cells:  The cells above the aquifer. The area of the aquifer equals
the area of the cells, and the upper boundary equals the soildepth of
the lowest cell

thickness:  The thickness of the aquifer in m

porosity:  Porosity of the aquifer

K:  Conductivity of the aquifer in m/day. If the conductivity should
be anisotropic, change the x,y and z values of the member K ";

%feature("docstring")  cmf::upslope::aquifer::add_connected_states "virtual void add_connected_states(cmf::math::StateVariable::list
&states) ";

%feature("docstring")  cmf::upslope::aquifer::conc "real conc(const
cmf::water::solute &_Solute) const

Returns the concentration of the given solute. ";

%feature("docstring")  cmf::upslope::aquifer::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute) const
override

Returns the current WaterQuality (concentration of all solutes) ";

%feature("docstring")  cmf::upslope::aquifer::conc "void conc(const
cmf::water::solute &_Solute, real NewConcetration)

Sets a new concentration. ";

%feature("docstring")  cmf::upslope::aquifer::connection_to "cmf::water::flux_connection* connection_to(const cmf::water::flux_node
&target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::upslope::aquifer::dxdt "virtual real
dxdt(const cmf::math::Time &time)

Returns the derivate of the state variable at time time. ";

%feature("docstring")  cmf::upslope::aquifer::flux3d_to "cmf::geometry::point flux3d_to(const cmf::water::flux_node &target,
cmf::math::Time t) ";

%feature("docstring")  cmf::upslope::aquifer::flux_to "real
flux_to(const cmf::water::flux_node &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from this into target\") ";

%feature("docstring")  cmf::upslope::aquifer::get_3d_flux "cmf::geometry::point get_3d_flux(cmf::math::Time t)

Returns the sum of all flux vectors. ";

%feature("docstring")  cmf::upslope::aquifer::get_abs_errtol "virtual
real get_abs_errtol(real rel_errtol) ";

%feature("docstring")  cmf::upslope::aquifer::get_abs_errtol "virtual
real get_abs_errtol(real rel_errtol) const ";

%feature("docstring")  cmf::upslope::aquifer::get_base_height "real
get_base_height() const

Returns the base height of the aquifer in m a.s.l. ";

%feature("docstring")  cmf::upslope::aquifer::get_connections "cmf::water::connection_list get_connections() const ";

%feature("docstring")  cmf::upslope::aquifer::get_K "virtual real
get_K(cmf::geometry::point direction) const

Returns the conductivity in m/day for a specific direction.

Takes account for anisotropy 

.. math::

     \\\\|K\\\\|(d) =
    \\\\frac{d}{\\\\|d\\\\|} \\\\bullet K

";

%feature("docstring")  cmf::upslope::aquifer::get_potential "real
get_potential(cmf::math::Time t=cmf::math::never) const override

Returns the water potential of the node in m waterhead.

The base class water storage always returns the height of the location
";

%feature("docstring")  cmf::upslope::aquifer::get_project "cmf::project& get_project() const

Returns the project, this node is part of. ";

%feature("docstring")  cmf::upslope::aquifer::get_state "real
get_state() const

Returns the current state of the variable. ";

%feature("docstring")
cmf::upslope::aquifer::get_state_variable_content "char
get_state_variable_content() const

A character indicating the integrated variable (either 'V' for Volume
or 'h' for head) ";

%feature("docstring")  cmf::upslope::aquifer::get_top_height "real
get_top_height() const

Returns the top height of the aquifer in m a.s.l. ";

%feature("docstring")  cmf::upslope::aquifer::get_volume "virtual
real get_volume() const

Returns the volume of water in this storage in m3 ";

%feature("docstring")  cmf::upslope::aquifer::is_connected "virtual
bool is_connected(const cmf::math::StateVariable &other) const

Returns True if this waterstorage is effected by another state. ";

%feature("docstring")  cmf::upslope::aquifer::is_storage "virtual
bool is_storage() const

Returns true, since this is a storage. ";

%feature("docstring")  cmf::upslope::aquifer::remove_connection "bool
remove_connection(cmf::water::flux_node::ptr To)

Remove the connection. ";

%feature("docstring")  cmf::upslope::aquifer::set_potential "virtual
void set_potential(real new_potential)

Sets the potential of this flux node. ";

%feature("docstring")  cmf::upslope::aquifer::set_state "void
set_state(real newState)

Gives access to the state variable. ";

%feature("docstring")
cmf::upslope::aquifer::set_state_variable_content "void
set_state_variable_content(char content)

A character indicating the integrated variable (either 'V' for Volume
or 'h' for head) ";

%feature("docstring")  cmf::upslope::aquifer::set_volume "virtual
void set_volume(real newwatercontent)

Sets the volume of water in this storage in m3 ";

%feature("docstring")  cmf::upslope::aquifer::Solute "SoluteStorage&
Solute(const cmf::water::solute _Solute)

Returns the water quality of the water storage. ";

%feature("docstring")  cmf::upslope::aquifer::Solute "const
SoluteStorage& Solute(const cmf::water::solute _Solute) const ";

%feature("docstring")  cmf::upslope::aquifer::waterbalance "real
waterbalance(cmf::math::Time t, const flux_connection *Without=0)
const

Returns the sum of all fluxes (positive and negative) at time t.

Single fluxes can be excluded from the calculation

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the waterbalance
(e.g. to prevent closed circuits) ";


// File: classcmf_1_1upslope_1_1aquifer___darcy.xml
%feature("docstring") cmf::upslope::aquifer_Darcy "

Lateral darcy flow between aquifer objects.



.. math::

    v_{Darcy}=K_{1,2}\\\\nabla\\\\Psi 



.. math::

    q = v_{Darcy} w
    \\\\Delta z

C++ includes: groundwater.h ";

%feature("docstring")  cmf::upslope::aquifer_Darcy::aquifer_Darcy "aquifer_Darcy(aquifer::ptr left, cmf::water::flux_node::ptr right,
real width)

Creates a new Darcy flow connection between two aquifers, or an
aquifer and another node acting as Dirichlet boundary condition.

Parameters:
-----------

left:  One aquifer

right:  Another aquifer, or a flux node that can be interpreted as an
Dirichlet boundary

width:  Width of the connection ";

%feature("docstring")  cmf::upslope::aquifer_Darcy::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")  cmf::upslope::aquifer_Darcy::exchange_target "void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")  cmf::upslope::aquifer_Darcy::get_ptr "ptr
get_ptr() const ";

%feature("docstring")  cmf::upslope::aquifer_Darcy::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::aquifer_Darcy::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")  cmf::upslope::aquifer_Darcy::get_tracer_filter
"real get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")  cmf::upslope::aquifer_Darcy::get_tracer_filter
"real get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::upslope::aquifer_Darcy::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::upslope::aquifer_Darcy::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::aquifer_Darcy::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::aquifer_Darcy::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")  cmf::upslope::aquifer_Darcy::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::upslope::aquifer_Darcy::set_tracer_filter
"void set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")  cmf::upslope::aquifer_Darcy::set_tracer_filter
"void set_tracer_filter(solute S, real value) ";

%feature("docstring")  cmf::upslope::aquifer_Darcy::short_string "virtual std::string short_string() const ";

%feature("docstring")  cmf::upslope::aquifer_Darcy::to_string "virtual std::string to_string() const ";


// File: classcmf_1_1upslope_1_1connections_1_1_base_macro_flow.xml
%feature("docstring") cmf::upslope::connections::BaseMacroFlow "";

%feature("docstring")  cmf::upslope::connections::BaseMacroFlow::conc
"real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::BaseMacroFlow::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")
cmf::upslope::connections::BaseMacroFlow::get_ptr "ptr get_ptr()
const ";

%feature("docstring")
cmf::upslope::connections::BaseMacroFlow::get_target "flux_node::ptr
get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::BaseMacroFlow::get_target "flux_node::ptr
get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::BaseMacroFlow::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::BaseMacroFlow::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")
cmf::upslope::connections::BaseMacroFlow::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::BaseMacroFlow::left_node "flux_node::ptr
left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::connections::BaseMacroFlow::q "real q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::BaseMacroFlow::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::upslope::connections::BaseMacroFlow::right_node "flux_node::ptr
right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::BaseMacroFlow::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::BaseMacroFlow::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::connections::BaseMacroFlow::short_string "virtual
std::string short_string() const ";

%feature("docstring")
cmf::upslope::connections::BaseMacroFlow::to_string "virtual
std::string to_string() const ";


// File: classcmf_1_1math_1_1_b_d_f2.xml
%feature("docstring") cmf::math::BDF2 "

An order 2 BDF-Method with fixed-point iteration and variable step
size.

Derived from Roussel C. and Roussel M. (2003) \"Generic Object-
Oriented Differential Equation Integrators\", C/C++ User Journal, Nov.
2003,http://www.ddj.com/cpp/184401724?pgno=8 and

Eckert S., Baaser H., Gross D. and Scherf O. (2004) \"A BDF2
integration method with step size control for elasto-plasticity\",
Computational Mechanics 34, 377 - 386, DOI: 10.1007/s00466-004-0581-1

Most important function: Integrate

C++ includes: bdf2.h ";

/*  Internal data storages  */

/*  Gear functions: The following functions evaluate the Gear formulas
of specified order, loading the results into compareStates.  */

/*  Accuracy parameters  */

/*  model time  */

%feature("docstring")  cmf::math::BDF2::get_t "cmf::math::Time
get_t() const

Returns the current model time. ";

%feature("docstring")  cmf::math::BDF2::set_t "void
set_t(cmf::math::Time val)

Sets the current model time. ";

%feature("docstring")  cmf::math::BDF2::get_dt "cmf::math::Time
get_dt() const

Returns the last time step. ";

/*  Integrate  */

%feature("docstring")  cmf::math::BDF2::integrate_until "void
integrate_until(cmf::math::Time t_max, cmf::math::Time dt=Time(), bool
reset=false)

Integrates the vector of state variables until t_max.

Parameters:
-----------

t_max:   Time, the solver should run to

dt:   Time step (may be omitted)

reset:  If true, solver is reseted before integration starts ";

%feature("docstring")  cmf::math::BDF2::BDF2 "BDF2(real epsilon=1e-9,
cmf::math::Time tStepMin=cmf::math::timespan(10))

Constructs a new BDF2 integrator.

Parameters:
-----------

epsilon:  relative error tolerance per time step (default=1e-9)

tStepMin:  minimum time step (default=10s) ";

%feature("docstring")  cmf::math::BDF2::BDF2 "BDF2(const
cmf::math::state_list &states, real epsilon=1e-9, cmf::math::Time
tStepMin=cmf::math::timespan(10))

Constructs a new Gears_var_Step.

Parameters:
-----------

states:  States to be added to the solver

epsilon:  relative error tolerance per time step (default=1e-9)

tStepMin:  minimum time step (default=10s) ";

%feature("docstring")  cmf::math::BDF2::BDF2 "BDF2(const Integrator
&templ)

Constructs a new BDF2 integrator.

Parameters:
-----------

templ:  Template to be used to construct a BDF2 method ";

%feature("docstring")  cmf::math::BDF2::copy "Integrator* copy()
const override

Polymorphic copy constructor. ";

%feature("docstring")  cmf::math::BDF2::get_error_position "ptrdiff_t
get_error_position() const

Returns the position of the biggest error. ";

%feature("docstring")  cmf::math::BDF2::integrate "int
integrate(cmf::math::Time MaxTime, cmf::math::Time TimeStep) override

Integrates the vector of state variables.

if :math:`t_{max}-t_{now}\\\\leq 2h_n` then :math:`h_{n+1}= t_{max}-t_{now}`
else :math:`h_{n+1}= 2h_n`

Uses pastStatesArray to store :math:`y_n,y_{n-1}`

if more than one step is taken start iterating:  :math:`y_{n+1}^i=\\\\frac{1}{1+2\\\\rho }\\\\left( (\\\\rho +1)^{2}y_{n}-\\\\rho ^{2}y_{n-1}+(\\\\rho +1)h\\\\frac{dy}{dt}(y_{n+1}^{i-1},t_{n+1} \\\\right), y_{n+1}^0=y_n`
with :math:`\\\\rho = \\\\frac{h_{n+1}}{h_n}`

until :math:`y_{n+1}^{i} - y_{n+1}^{i-1} \\\\leq \\\\ \\\\|\\\\epsilon + \\\\epsilon y_{n+1}\\\\|_{\\\\infty}`

If too many iterations are needed, or the error is rising, repeat
iteration with :math:`h_{n+1} = \\\\frac{h_{n+1}}{2}`

Parameters:
-----------

MaxTime:  To stop the model (if running in a model framework) at time
steps of value exchange e.g. full hours, the next value exchange time
can be given

TimeStep:  Takes the proposed timestep, ignored by this solver ";

%feature("docstring")  cmf::math::BDF2::reset "void reset() override

Resets any saved history (for multistep methods) ";

%feature("docstring")  cmf::math::BDF2::set_system "void
set_system(const state_list &states) ";

%feature("docstring")  cmf::math::BDF2::size "size_t size() const ";

%feature("docstring")  cmf::math::BDF2::to_string "std::string
to_string() const override

Desrcibes the iterator. ";


// File: classcmf_1_1math_1_1root__finding_1_1_bisect.xml
%feature("docstring") cmf::math::root_finding::Bisect "";

%feature("docstring")  cmf::math::root_finding::Bisect::Bisect "Bisect(double tolerance=1e-12, unsigned int max_iterations=1000) ";

%feature("docstring")  cmf::math::root_finding::Bisect::f "virtual
double f(double) const =0 ";


// File: classcmf_1_1math_1_1root__finding_1_1_brents_method.xml
%feature("docstring") cmf::math::root_finding::BrentsMethod "";

%feature("docstring")
cmf::math::root_finding::BrentsMethod::BrentsMethod "BrentsMethod(double tolerance=1e-12, unsigned int max_iterations=1000)
";

%feature("docstring")  cmf::math::root_finding::BrentsMethod::f "virtual double f(double) const =0 ";


// File: classcmf_1_1upslope_1_1_brooks_corey_retention_curve.xml
%feature("docstring") cmf::upslope::BrooksCoreyRetentionCurve "

Provides the use of the Brooks-Corey retention curve.



.. math::

     W(\\\\theta) &=& \\\\frac{\\\\theta -
    \\\\theta_r}{\\\\theta_s - \\\\theta_r} \\\\\\\\ K(W) &=& K_{sat}
    W^{2b+3} \\\\\\\\ \\\\Psi(W) &=& \\\\Psi_X
    \\\\left(\\\\frac{W}{W_X}\\\\right)^{-b} \\\\\\\\ W(\\\\Psi) &=&
    {\\\\left( \\\\frac{\\\\Psi_X}{\\\\Psi}\\\\right)
    }^{\\\\frac{1}{b}}\\\\ W_X 

 where:  :math:`K` is the
conductivity in :math:`\\\\frac m{day}`

:math:`W` is the wetness (Volume of soil water per volume of pores)

:math:`b` is the shape of the retention curve (usually between 4 (sand) and
14 (clay))

:math:`\\\\Psi(W)` is the matric potential in :math:`m H_2O` at wetness W

:math:`\\\\Psi_X` is a matric potential at a known wetness in :math:`m H_2O`

:math:`W_X` is the wetness with a known matric potential for dynamic changes
with depth, exponential decays of porosity and saturated conductivity
are used The decay function is: :math:`v(d)=v(0) (1+a)^{-d}`, where v is
the value ( :math:`K_{sat},\\\\Phi`), d is the depth in m and a is the
fractional decay per m. E.g. 0.1 means the value has in 1 m depth 90%
of the value at the surface

C++ includes: RetentionCurve.h ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::BrooksCoreyRetentionCurve "BrooksCoreyRetentionCurve(real ksat=15, real porosity=0.5, real _b=5,
real theta_x=0.2, real psi_x=pF_to_waterhead(2.5), real
porosity_decay=0)

Creates a brooks corey retention curve.

Parameters:
-----------

ksat:  Saturated conductivity :math:`\\\\frac{m}{day}`

porosity:   :math:`\\\\frac{m^3 Pores}{m^3 Soil}`

_b:  Shape of the retention curve (if you do not know how to
parameterize this, take a look at the other constructor)

theta_x:   :math:`\\\\theta_X` Water content at a specific suction pressure

psi_x:  Suction pressure for :math:`\\\\theta_X` in m water column, use the
conversion functions pF_to_waterhead, pressure_to_waterhead to convert
pressure in to waterhead height (default pF=2.5)

porosity_decay:  Relative decay of porosity with depth, e.g. 0.1 means
conductivity gets 10% smaller per meter ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::~BrooksCoreyRetentionCurve "virtual ~BrooksCoreyRetentionCurve() ";

%feature("docstring")  cmf::upslope::BrooksCoreyRetentionCurve::copy "virtual BrooksCoreyRetentionCurve* copy() const ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::Diffusivity "virtual real
Diffusivity(real wetness) const

Returns the Diffusivity of the soil.

Not implemented for all retention curves. Diffusivity is used by
MACROlikeMacroMicroExchange ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::Diffusivity "cmf::math::num_array Diffusivity(cmf::math::num_array &wetness) ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::dPsiM_dW "virtual real
dPsiM_dW(real wetness) const ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::dPsiM_dW "cmf::math::num_array dPsiM_dW(const cmf::math::num_array &wetness)
const ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::FillHeight "real
FillHeight(real lowerDepth, real Area, real Volume) const

Returns the thickness of a soil column with a certain pore volume. ";

%feature("docstring")  cmf::upslope::BrooksCoreyRetentionCurve::get_b
"real get_b() const

Retention curve shape parameter. ";

%feature("docstring")  cmf::upslope::BrooksCoreyRetentionCurve::K "cmf::math::num_array K(const cmf::math::num_array &wetness) const ";

%feature("docstring")  cmf::upslope::BrooksCoreyRetentionCurve::K "virtual real K(real wetness) const

Returns the conductivity in m/day at a certain depth and water
content. ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::MatricPotential "cmf::math::num_array MatricPotential(const cmf::math::num_array
&wetness) const ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::MatricPotential "real
MatricPotential(real wetness) const

Returns the suction pressure in m, use conversion functions
waterhead_to_pressure and waterhead_to_pF fro conversions. ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::Porosity "real Porosity(real
depth) const

real (Porosity) ";

%feature("docstring")  cmf::upslope::BrooksCoreyRetentionCurve::set_b
"void set_b(real new_b) ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::SetPorosity "void
SetPorosity(real porosity, real porosity_decay=0)

Sets the porosity (Volume of pores per volume of soil) and the
exponential porosity decline with depth. ";

%feature("docstring")  cmf::upslope::BrooksCoreyRetentionCurve::theta
"virtual real theta(real wetness) const

returns the water content :math:`theta` for a given wetness ";

%feature("docstring")  cmf::upslope::BrooksCoreyRetentionCurve::theta
"cmf::math::num_array theta(const cmf::math::num_array &wetness)
const ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::Transmissivity "real
Transmissivity(real upperDepth, real lowerDepth, real theta) const ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::VoidVolume "real
VoidVolume(real upperDepth, real lowerDepth, real Area) const

Returns the pore volume in a region of a soil column.

If there is a porosity decay, the void volume is the integral of
porosity over depth times area ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::Wetness "cmf::math::num_array Wetness(const cmf::math::num_array &suction)
const ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::Wetness "real Wetness(real
suction) const

returns the wetness (volumetric water content per pore space) at a
given suction pressure ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::Wetness_eff "virtual real
Wetness_eff(real wetness, real pF_r=4.2) const

Returns the effective wetness, using a residual pF value 

.. math::

    w_{eff}
    =
    \\\\frac{w_{act}-w\\\\left(pF_r\\\\right)}{1-w\\\\left(pF_r\\\\right)}

.
";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::Wetness_pF "real
Wetness_pF(real pF) const

returns the volumetric water content at a given pF value ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::Wetness_pF "cmf::math::num_array Wetness_pF(const cmf::math::num_array &pF) const
";


// File: classcmf_1_1upslope_1_1connections_1_1_canopy_overflow.xml
%feature("docstring") cmf::upslope::connections::CanopyOverflow "

Calculates the overflow of a canopy storage using a kinematic wave
approach.

This model routes only water that exceeds the canopy capacity to the
ground with an ad hoc estimated function: 

.. math::

    q_{CO} =
    \\\\left(\\\\frac{V_{act}-V_{max}}{V_{max}}\\\\right) ^2 \\\\cdot 2400
    \\\\frac{A_{cell}}{1000}

With:  :math:`q_{CO}(t)[\\\\frac{m^3}{day}]`:
The flux from canopy to the ground

:math:`V_{act}[mm]=1000 [mm/m] \\\\frac{V_{canopy}[m^3]}{A_{cell} [m^2]}`
The stored water of the canopy in mm

:math:`V_{max}[mm]=c_{LAI}[mm] LAI` The capacity of the canopy in mm,
defined by the factor CanopyCapacityPerLAI [mm/LAI], and the leaf area
index LAI. (see: cmf::upslope::Vegetation)

C++ includes: surfacefluxes.h ";

%feature("docstring")
cmf::upslope::connections::CanopyOverflow::CanopyOverflow "CanopyOverflow(cmf::water::WaterStorage::ptr Canopy,
cmf::water::flux_node::ptr target, cmf::upslope::Cell &cell) ";

%feature("docstring")  cmf::upslope::connections::CanopyOverflow::conc
"real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::CanopyOverflow::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")
cmf::upslope::connections::CanopyOverflow::get_ptr "ptr get_ptr()
const ";

%feature("docstring")
cmf::upslope::connections::CanopyOverflow::get_target "flux_node::ptr
get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::CanopyOverflow::get_target "flux_node::ptr
get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::CanopyOverflow::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::CanopyOverflow::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")
cmf::upslope::connections::CanopyOverflow::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::CanopyOverflow::left_node "flux_node::ptr
left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::connections::CanopyOverflow::q "real q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::CanopyOverflow::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::upslope::connections::CanopyOverflow::right_node "flux_node::ptr
right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::CanopyOverflow::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::CanopyOverflow::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::connections::CanopyOverflow::short_string "virtual
std::string short_string() const ";

%feature("docstring")
cmf::upslope::connections::CanopyOverflow::to_string "virtual
std::string to_string() const ";


// File: classcmf_1_1upslope_1_1_e_t_1_1_canopy_storage_evaporation.xml
%feature("docstring") cmf::upslope::ET::CanopyStorageEvaporation "

Calculates the evaporation from a canopy storage.

C++ includes: ET.h ";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::CanopyStorageEvaporation "CanopyStorageEvaporation(cmf::water::WaterStorage::ptr CanopyStorage,
cmf::water::flux_node::ptr ET_target, cmf::upslope::Cell &cell) ";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::get_ptr "ptr get_ptr()
const ";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::left_node "flux_node::ptr
left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::ET::CanopyStorageEvaporation::q "real q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::short_string "virtual
std::string short_string() const ";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::to_string "virtual
std::string to_string() const ";


// File: classcmf_1_1upslope_1_1_cell.xml
%feature("docstring") cmf::upslope::Cell "

This class is the basic landscape object.

It is the owner of water storages, and the upper and lower boundary
conditions of the system (rainfall, atmospheric vapor, deep
groundwater)

C++ includes: cell.h ";

/*  Location  */

%feature("docstring")  cmf::upslope::Cell::get_topology "cmf::upslope::Topology& get_topology() ";

%feature("docstring")  cmf::upslope::Cell::get_position "cmf::geometry::point get_position() const

Returns the location of the cell. ";

%feature("docstring")  cmf::upslope::Cell::get_area "double
get_area() const

Returns the area of the cell. ";

%feature("docstring")  cmf::upslope::Cell::m3_to_mm "double
m3_to_mm(double volume) const

Converts a volume in m3 in mm for the cell area. ";

%feature("docstring")  cmf::upslope::Cell::mm_to_m3 "double
mm_to_m3(double depth) const ";

/*  Saturation  */

%feature("docstring")  cmf::upslope::Cell::InvalidateSatDepth "void
InvalidateSatDepth() const

Marks the saturated depth as unvalid. This is done automatically, when
the state of a layer changes. ";

%feature("docstring")  cmf::upslope::Cell::get_saturated_depth "real
get_saturated_depth() const

Returns the potential :math:`\\\\Psi_{total}` of the deepest unsaturated
layer as distance from the surface.

This function is wrapped as the property saturated_depth in Python ";

%feature("docstring")  cmf::upslope::Cell::set_saturated_depth "void
set_saturated_depth(real depth)

Sets the potential :math:`\\\\Psi_{total}` of each layer as distance from
the surface.

This function is wrapped as the property saturated_depth in Python ";

/*  Flux nodes of the cell  */

%feature("docstring")  cmf::upslope::Cell::get_meteorology "cmf::atmosphere::Meteorology& get_meteorology() const

Returns the meteorological data source. ";

%feature("docstring")  cmf::upslope::Cell::set_aerodynamic_resistance
"void
set_aerodynamic_resistance(cmf::atmosphere::aerodynamic_resistance::ptr
Ra)

Sets the method to calculate aerodynamic resistance against turbulent
sensible heat fluxes. ";

%feature("docstring")  cmf::upslope::Cell::set_meteorology "void
set_meteorology(const cmf::atmosphere::Meteorology &new_meteo)

Sets a meteorological data source. ";

%feature("docstring")  cmf::upslope::Cell::set_weather "void
set_weather(const cmf::atmosphere::Weather &weather)

Sets the weather for this cell. Connectivity to a meteorological
station is lost. ";

%feature("docstring")  cmf::upslope::Cell::set_rainfall "void
set_rainfall(double rainfall)

Exchanges a timeseries of rainfall with a constant flux. ";

%feature("docstring")  cmf::upslope::Cell::get_rainfall "double
get_rainfall(cmf::math::Time t) const

Returns the current rainfall flux in m3/day. ";

%feature("docstring")  cmf::upslope::Cell::set_rain_source "void
set_rain_source(cmf::atmosphere::RainSource::ptr new_source)

Changes the current source of rainfall. ";

%feature("docstring")  cmf::upslope::Cell::get_rain_source "cmf::atmosphere::RainSource::ptr get_rain_source()

Returns the current source for rainfall. ";

%feature("docstring")  cmf::upslope::Cell::set_uptakestress "void
set_uptakestress(const cmf::upslope::ET::RootUptakeStressFunction
&stressfunction)

Uses the given WaterStressFunction for all stressedET like connections
to the transpiration target. ";

%feature("docstring")  cmf::upslope::Cell::get_evaporation "cmf::water::flux_node::ptr get_evaporation()

Returns the end point of all evaporation of this cell (a
cmf::water::flux_node) ";

%feature("docstring")  cmf::upslope::Cell::get_transpiration "cmf::water::flux_node::ptr get_transpiration()

Returns the end point of all transpiration of this cell (a
cmf::water::flux_node) ";

%feature("docstring")  cmf::upslope::Cell::get_surfacewater "cmf::water::flux_node::ptr get_surfacewater()

returns the surface water of this cell. This is either a flux node or
a cmf::upslope::SurfaceWater ";

%feature("docstring")  cmf::upslope::Cell::surfacewater_as_storage "surfacewater_ptr surfacewater_as_storage()

Makes the surfacewater of this cell a cmf::upslope::SurfaceWater
storage. ";

%feature("docstring")  cmf::upslope::Cell::add_storage "cmf::water::WaterStorage::ptr add_storage(std::string Name, char
storage_role='N', bool isopenwater=false)

Adds a new storage to the cell.

Parameters:
-----------

Name:  The name of the storage

storage_role:  A shortcut to describe the functional role of the
storage new storage. Possible Values: 'C' denotes a canopy storage

'S' denotes a snow storage

any other value denotes a storage with an undefined function

isopenwater:  If true, an open water storage with a cmf::river::Prism
height function is created ";

%feature("docstring")  cmf::upslope::Cell::add_storage "ptrdiff_t
add_storage(cmf::water::WaterStorage::ptr storage)

Bounds an existing storage to the cell. ";

%feature("docstring")  cmf::upslope::Cell::remove_storage "void
remove_storage(cmf::water::WaterStorage::ptr storage) ";

%feature("docstring")  cmf::upslope::Cell::storage_count "size_t
storage_count() const ";

%feature("docstring")  cmf::upslope::Cell::get_storage "cmf::water::WaterStorage::ptr get_storage(ptrdiff_t index) const ";

%feature("docstring")  cmf::upslope::Cell::get_canopy "cmf::water::WaterStorage::ptr get_canopy() const ";

%feature("docstring")  cmf::upslope::Cell::get_snow "cmf::water::WaterStorage::ptr get_snow() const ";

%feature("docstring")  cmf::upslope::Cell::snow_coverage "real
snow_coverage() const ";

%feature("docstring")  cmf::upslope::Cell::albedo "real albedo()
const ";

%feature("docstring")  cmf::upslope::Cell::surface_water_coverage "real surface_water_coverage() const

Returns the coverage of the surface water.

The covered fraction (0..1) is simply modelled as a piecewise linear
function of the surface water depth. If the depth is above the
aggregate height, the coverage is 1, below it is given as 

.. math::

     c =
    \\\\frac{h_{water}}{\\\\Delta h_{surface}}

with c the coverage,
:math:`h_{water}` the depth of the surface water and :math:`\\\\Delta h_{surface}`
the amplitude of the surface roughness ";

%feature("docstring")  cmf::upslope::Cell::heat_flux "real
heat_flux(cmf::math::Time t) const

Calculates the surface heat balance.

Parameters:
-----------

t:  Time step ";

%feature("docstring")  cmf::upslope::Cell::leave_wetness "real
leave_wetness() const

Return the fraction of wet leaves in the canopy if a canopy water
storage exists.

If no canopy storage is present, it returns 0.0 (=empty). The fraction
of wet leaves are calculated as the linear filling of the canopy
storage. ";

%feature("docstring")  cmf::upslope::Cell::get_WKB "cmf::bytestring
get_WKB() const ";

%feature("docstring")  cmf::upslope::Cell::set_WKB "void
set_WKB(cmf::bytestring wkb) ";

%feature("docstring")  cmf::upslope::Cell::get_project "cmf::project&
get_project() const ";

%feature("docstring")  cmf::upslope::Cell::get_weather "cmf::atmosphere::Weather get_weather(cmf::math::Time t) const

Returns the current meteorological conditions of the cell at time t.
";

/*  Layers  */

%feature("docstring")  cmf::upslope::Cell::layer_count "size_t
layer_count() const

Returns the number of layers of the cell. ";

%feature("docstring")  cmf::upslope::Cell::get_layer "cmf::upslope::SoilLayer::ptr get_layer(ptrdiff_t ndx) const

Returns the layer at position ndx.

From python this function is masked as a sequence: ";

%feature("docstring")  cmf::upslope::Cell::get_layers "const
layer_list& get_layers() const

Returns the list of layers.

From python this function is masked as a property: ";

%feature("docstring")  cmf::upslope::Cell::add_layer "cmf::upslope::SoilLayer::ptr add_layer(real lowerboundary, const
cmf::upslope::RetentionCurve &r_curve, real saturateddepth=10)

Adds a layer to the cell.

Layers are created using this function

the new layer

Parameters:
-----------

lowerboundary:  The maximum depth of the layer in m. If lowerboundary
is smaller or equal than the lowerboundary of thelowest layer, an
error is raised

r_curve:  A retention curve.See here for a discussion on retention
curves in cmf.

saturateddepth:  The initial potential of the new layer in m below
surface. Default = 10m (=quite dry) ";

%feature("docstring")  cmf::upslope::Cell::add_layer "cmf::upslope::SoilLayer::ptr add_layer(real lowerboundary)

Adds a rather conceptual layer to the cell. Use this version for
conceptual models. The retention curve resambles an empty bucket. ";

%feature("docstring")  cmf::upslope::Cell::remove_last_layer "void
remove_last_layer()

Remove the lowest layer from this cell. ";

%feature("docstring")  cmf::upslope::Cell::remove_layers "void
remove_layers()

Removes all layers from this cell. ";

%feature("docstring")  cmf::upslope::Cell::get_soildepth "double
get_soildepth() const

Returns the lower boundary of the lowest layer in m. ";

%feature("docstring")  cmf::upslope::Cell::~Cell "virtual ~Cell() ";

%feature("docstring")  cmf::upslope::Cell::Cell "Cell(double x,
double y, double z, double area, cmf::project &_project) ";

%feature("docstring")  cmf::upslope::Cell::to_string "std::string
to_string() const ";


// File: classcmf_1_1upslope_1_1cell__const__iterator.xml
%feature("docstring") cmf::upslope::cell_const_iterator "";

%feature("docstring")
cmf::upslope::cell_const_iterator::cell_const_iterator "cell_const_iterator(const cell_const_iterator &copy) ";

%feature("docstring")
cmf::upslope::cell_const_iterator::cell_const_iterator "cell_const_iterator(const cell_iterator &copy) ";

%feature("docstring")  cmf::upslope::cell_const_iterator::as_iterator
"const_iterator as_iterator() ";

%feature("docstring")  cmf::upslope::cell_const_iterator::ptr "const
cmf::upslope::Cell* ptr() const ";


// File: classcmf_1_1upslope_1_1cell__iterator.xml
%feature("docstring") cmf::upslope::cell_iterator "";

%feature("docstring")  cmf::upslope::cell_iterator::cell_iterator "cell_iterator() ";

%feature("docstring")  cmf::upslope::cell_iterator::cell_iterator "cell_iterator(const cell_iterator &copy) ";

%feature("docstring")  cmf::upslope::cell_iterator::as_iterator "iterator as_iterator() ";

%feature("docstring")  cmf::upslope::cell_iterator::ptr "cmf::upslope::Cell* ptr() const ";


// File: classcmf_1_1upslope_1_1cell__vector.xml
%feature("docstring") cmf::upslope::cell_vector "

A cell vector holds a bunch of cells.

C++ includes: cell_vector.h ";

%feature("docstring")  cmf::upslope::cell_vector::cell_vector "cell_vector() ";

%feature("docstring")  cmf::upslope::cell_vector::cell_vector "cell_vector(const cell_vector &copy) ";

%feature("docstring")  cmf::upslope::cell_vector::cell_vector "cell_vector(cell_const_iterator first, cell_const_iterator last) ";

%feature("docstring")  cmf::upslope::cell_vector::~cell_vector "virtual ~cell_vector() ";

%feature("docstring")  cmf::upslope::cell_vector::append "void
append(cmf::upslope::Cell &cell) ";

%feature("docstring")  cmf::upslope::cell_vector::as_set "std::set<Cell*> as_set() ";

%feature("docstring")  cmf::upslope::cell_vector::begin "cell_iterator begin() ";

%feature("docstring")  cmf::upslope::cell_vector::begin "cell_const_iterator begin() const ";

%feature("docstring")  cmf::upslope::cell_vector::contains "bool
contains(const cmf::upslope::Cell &cell) const ";

%feature("docstring")  cmf::upslope::cell_vector::end "cell_iterator
end() ";

%feature("docstring")  cmf::upslope::cell_vector::end "cell_const_iterator end() const ";

%feature("docstring")  cmf::upslope::cell_vector::get_area "double
get_area() const

Returns sum of the area of the cells. ";

%feature("docstring")  cmf::upslope::cell_vector::get_highest "Cell&
get_highest() const

Returns the heighest cell. ";

%feature("docstring")  cmf::upslope::cell_vector::get_lowest "Cell&
get_lowest() const

Returns the cell with the lowest height. ";

%feature("docstring")  cmf::upslope::cell_vector::get_slice "cell_vector get_slice(ptrdiff_t start, ptrdiff_t end, ptrdiff_t
step=1) ";

%feature("docstring")  cmf::upslope::cell_vector::pop "Cell& pop()

Returns and removes the last cell. ";

%feature("docstring")  cmf::upslope::cell_vector::ptr_begin "cellvec::iterator ptr_begin() ";

%feature("docstring")  cmf::upslope::cell_vector::ptr_end "cellvec::iterator ptr_end() ";

%feature("docstring")  cmf::upslope::cell_vector::remove "void
remove(ptrdiff_t index) ";

%feature("docstring")  cmf::upslope::cell_vector::remove "void
remove(const cmf::upslope::Cell &cell) ";

%feature("docstring")  cmf::upslope::cell_vector::size "size_t size()
const ";


// File: classcmf_1_1upslope_1_1_cell_connector.xml
%feature("docstring") cmf::upslope::CellConnector "

A helper class to connect cells with flux_connection objects. This is
generated by flux_connection classes, intended to connect cells.

C++ includes: cell.h ";

%feature("docstring")  cmf::upslope::CellConnector::CellConnector "CellConnector(connectorfunction connector) ";

%feature("docstring")  cmf::upslope::CellConnector::connect "void
connect(cmf::upslope::Cell &cell1, cmf::upslope::Cell &cell2,
ptrdiff_t start_at_layer=0) const ";


// File: classcmf_1_1draw_1_1cellmap_1_1_cell_map.xml
%feature("docstring") cmf::draw::cellmap::CellMap "

Draws a map of the cell geometries. Only functional, when shapely is
installed.  A CellMap is created with cells to show and a function
returning a value from a cell  Usage example: >>>import cmf
>>>p=cmf.project() >>>def saturated_depth(c): ...    return
c.saturated_depth >>>cm = CellMap(p, saturated_depth) ";

%feature("docstring")  cmf::draw::cellmap::CellMap::__init__ "def
__init__(self, cells, value_function, cmap=default_colormap,
hold=True, vmin=None, vmax=None, kwargs)

Creates a new map from cells  :param cells: :param value_function:
:param cmap: :param hold: :param vmin: :param vmax: :param kwargs: ";

%feature("docstring")  cmf::draw::cellmap::CellMap::__call__ "def
__call__(self, recalc_range=False) ";

%feature("docstring")  cmf::draw::cellmap::CellMap::autoscale "def
autoscale(self)

Overwrite base class of maplotlib.cm.ScalarMappable to prevent missuse
";

%feature("docstring")  cmf::draw::cellmap::CellMap::autoscale_None "def autoscale_None(self)

Overwrite base class of maplotlib.cm.ScalarMappable to prevent missuse
";

%feature("docstring")  cmf::draw::cellmap::CellMap::color_values "def
color_values(self) ";

%feature("docstring")  cmf::draw::cellmap::CellMap::f "def f(self) ";

%feature("docstring")  cmf::draw::cellmap::CellMap::f "def f(self,
funct) ";

%feature("docstring")  cmf::draw::cellmap::CellMap::get_artists "def
get_artists(self)

Returns the list of matplotlib.patches.Polygons in the cell map.
Useful for Animations  :return: iterable of polygons ";


// File: classcmf_1_1river_1_1_channel.xml
%feature("docstring") cmf::river::Channel "

A wrapper for channel geometries.

C++ includes: ReachType.h ";

%feature("docstring")  cmf::river::Channel::Channel "Channel() ";

%feature("docstring")  cmf::river::Channel::Channel "Channel(double
length)

Creates a triangular reach of a length. ";

%feature("docstring")  cmf::river::Channel::Channel "Channel(const
IChannel &for_wrapping)

Wraps an existing channel geometry. ";

%feature("docstring")  cmf::river::Channel::Channel "Channel(const
IVolumeHeightFunction &for_casting) ";

%feature("docstring")  cmf::river::Channel::Channel "Channel(const
Channel &for_copy)

Copy constructable. ";

%feature("docstring")  cmf::river::Channel::Channel "Channel(char
typecode, double length, double width=1., double depth=0.25)

Creates a reachtype using a short cut character.

Acceptes one of the following characters: 'T' TriangularReach, width
and depth are ignored.

'R' RectangularReach, depth is ignored

'P' PipeReach, depth is ignored, width is the diameter of the pipe

'S' SWATReachType, a trapezoid flow cross section, as used in the SWAT
model, width (bank width) and depth are used the reach type

Parameters:
-----------

typecode:  Describes the geometry of the reach cross section.

length:  The length of the channel in m

width:  width of the reach cross section in m (ignored for typecode
'T')

depth:  depth of the reach (ignored for typecode 'T','R','P','S') ";

%feature("docstring")  cmf::river::Channel::~Channel "virtual
~Channel() ";

%feature("docstring")  cmf::river::Channel::A "virtual double
A(double V) const

Returns the area of the surface for a given volume. ";

%feature("docstring")  cmf::river::Channel::copy "Channel* copy()
const ";

%feature("docstring")  cmf::river::Channel::get_channel_width "double
get_channel_width(double depth) const

Calculates the flow width from a given actual depth [m] using the
actual IChannel geometry. ";

%feature("docstring")  cmf::river::Channel::get_depth "double
get_depth(double area) const

Calculates the actual depth of the reach using the IChannel geometry.

get_depth of the reach [m]

Parameters:
-----------

area:  Wetted area of a river cross section [m2], can be obtained by
V/l, where V is the stored volume and l is the reach length ";

%feature("docstring")  cmf::river::Channel::get_flux_crossection "double get_flux_crossection(double depth) const

Calculates the wetted area from a given depth using the IChannel
geometry.

In most cases use get_flux_crossection=V/l, where V is the stored
volume and l is the reach length Wetted area of a river cross section
[m2]

Parameters:
-----------

depth:  depth of the reach [m] ";

%feature("docstring")  cmf::river::Channel::get_length "double
get_length() const

Length of the reach. ";

%feature("docstring")  cmf::river::Channel::get_nManning "double
get_nManning() const ";

%feature("docstring")  cmf::river::Channel::get_wetted_perimeter "double get_wetted_perimeter(double depth) const

Calculates the wetted perimeter from a given actual depth [m] using
the actual IChannel geometry. ";

%feature("docstring")  cmf::river::Channel::h "virtual double
h(double V) const

Returns the depth of a given volume. ";

%feature("docstring")  cmf::river::Channel::q "virtual double
q(double h, double slope) const ";

%feature("docstring")  cmf::river::Channel::qManning "virtual double
qManning(double A, double slope) const

Calculates the flow rate from a given water volume in the reach


.. math::

     q_{Manning}&=& A R^{\\\\frac 2 3}
    \\\\sqrt{\\\\frac {\\\\Delta_z} n} \\\\\\\\ A &=& \\\\frac V l
    \\\\mbox{, (Crosssectional area of the wetted crossection, Volume per
    length)} \\\\\\\\ R &=& \\\\frac A {P(d)} \\\\\\\\ P(d) &=& \\\\mbox{
    the perimeter of the wetted crosssection, a function of reach depth}
    \\\\\\\\ d(V) &=& \\\\mbox{ the depth of the reach a function of the
    volume} \\\\\\\\ \\\\Delta_z &=& \\\\frac{z_{max} - z_{min}}{l}
    \\\\mbox{ Slope of the reach} 

.

Flow rate [m3/s]

Parameters:
-----------

A:  The area of the cross section [m2]

slope:  The slope of the reach [m/m] ";

%feature("docstring")  cmf::river::Channel::set_nManning "void
set_nManning(double val) ";

%feature("docstring")  cmf::river::Channel::typecode "char typecode()
const ";

%feature("docstring")  cmf::river::Channel::V "virtual double
V(double h) const ";


// File: classcmf_1_1upslope_1_1connections_1_1_conceptual_infiltration.xml
%feature("docstring")
cmf::upslope::connections::ConceptualInfiltration "

Connects the surfacewater and the most upper layer using a simplified
infiltration model suitable for conceptional models.



.. math::

     q_{inf} = \\\\left(1-e_{sat}\\\\left(W, W_0\\\\right)\\\\right)
    q_{inf,pot} 

where:  :math:`q_{inf}` Effective infiltration from
surface to first layer (soil)

:math:`e_{sat}(W, W_0)` Saturation excess, ranging from 0 (nowhere saturated
soil layer) to 1 (fully saturated). :math:`W` is the average wetness
calculated from the soil layer, :math:`W_0` is a parameter denoting the
wetness, where 50% of the layer is saturated using a sigmoidal
function:  :math:`e_{sat}(W_{soil}, W_0) = \\\\left(1+e^{-(W-W_0)0.2(1-W_0)}\\\\right)^{-1}`

:math:`q_{inf,pot}` is the potential infiltration, given by the incoming
fluxes limited by the saturated conductivity:  :math:`q_{inf,pot} = \\\\min(q_{in}, K_{sat} A)`  :math:`q_{in}` Sum of incoming fluxes to the
surfacewater in :math:`m^3/day`

:math:`K_{sat}` Saturated conductivity in :math:`m/day`

:math:`A` Cell area in :math:`m^2`

C++ includes: infiltration.h ";

%feature("docstring")
cmf::upslope::connections::ConceptualInfiltration::ConceptualInfiltration
"ConceptualInfiltration(cmf::upslope::SoilLayer::ptr soilwater,
cmf::water::flux_node::ptr surfacewater, real W0=0.9)

Creates the connection between surfacewater and first soil layer.

Parameters:
-----------

soilwater:  the infiltration target

surfacewater:  the infiltration source

W0:  the 50% saturation value ";

%feature("docstring")
cmf::upslope::connections::ConceptualInfiltration::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::ConceptualInfiltration::exchange_target "void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")
cmf::upslope::connections::ConceptualInfiltration::get_ptr "ptr
get_ptr() const ";

%feature("docstring")
cmf::upslope::connections::ConceptualInfiltration::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::ConceptualInfiltration::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::ConceptualInfiltration::get_tracer_filter "real get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::ConceptualInfiltration::get_tracer_filter "real get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")
cmf::upslope::connections::ConceptualInfiltration::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::ConceptualInfiltration::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::connections::ConceptualInfiltration::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::ConceptualInfiltration::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::upslope::connections::ConceptualInfiltration::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::ConceptualInfiltration::set_tracer_filter "void set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::ConceptualInfiltration::set_tracer_filter "void set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::connections::ConceptualInfiltration::short_string "virtual std::string short_string() const ";

%feature("docstring")
cmf::upslope::connections::ConceptualInfiltration::to_string "virtual
std::string to_string() const ";


// File: classcmf_1_1upslope_1_1conductable.xml
%feature("docstring") cmf::upslope::conductable "

An abstract interface for all classes providing a conductivity (e.g.

soil layer or aquifer) Each water storage where the concept of
conductivity is applicable should implement (by deriving) the
interface

This class is not part of the Python interface, since a dynamic typed
language can use duck typing instead of this interface

C++ includes: conductable.h ";

%feature("docstring")  cmf::upslope::conductable::get_K "virtual real
get_K(cmf::geometry::point direction) const =0 ";


// File: classcmf_1_1water_1_1connection__list.xml
%feature("docstring") cmf::water::connection_list "

A self sorting list of connections.

C++ includes: flux_connection.h ";

%feature("docstring")  cmf::water::connection_list::~connection_list "virtual ~connection_list() ";

%feature("docstring")  cmf::water::connection_list::append "bool
append(cmf::water::flux_connection::ptr connection) ";

%feature("docstring")  cmf::water::connection_list::at "cmf::water::flux_connection::ptr at(size_t pos) const ";

%feature("docstring")  cmf::water::connection_list::begin "iterator
begin() ";

%feature("docstring")  cmf::water::connection_list::begin "const_iterator begin() const ";

%feature("docstring")  cmf::water::connection_list::contains "bool
contains(cmf::water::flux_connection::ptr connection) const ";

%feature("docstring")  cmf::water::connection_list::do_action "void
do_action(cmf::math::Time t, bool use_OpenMP=true) ";

%feature("docstring")  cmf::water::connection_list::end "iterator
end() ";

%feature("docstring")  cmf::water::connection_list::end "const_iterator end() const ";

%feature("docstring")  cmf::water::connection_list::extend "void
extend(const connection_list &connections) ";

%feature("docstring")  cmf::water::connection_list::remove "bool
remove(cmf::water::flux_connection::ptr connection) ";

%feature("docstring")  cmf::water::connection_list::size "size_t
size() const ";


// File: classcmf_1_1upslope_1_1_e_t_1_1constant_e_tpot.xml
%feature("docstring") cmf::upslope::ET::constantETpot "

A constant evapotranspiration.

Uses a constant measured or elsewhere modelled ETpot. Actual
Evapotranspiration is calculated from rootdepth and actual matrix
potential in the layers using Tact. The value of ETpot can be changed
during runtime

C++ includes: ET.h ";

%feature("docstring")  cmf::upslope::ET::constantETpot::constantETpot
"constantETpot(cmf::upslope::SoilLayer::ptr source,
cmf::water::flux_node::ptr ET_target, double constantETpot_value) ";

%feature("docstring")  cmf::upslope::ET::constantETpot::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")  cmf::upslope::ET::constantETpot::ETpot "virtual real ETpot(cmf::math::Time t) const ";

%feature("docstring")
cmf::upslope::ET::constantETpot::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::ET::constantETpot::get_layer "SoilLayer::ptr get_layer() const ";

%feature("docstring")  cmf::upslope::ET::constantETpot::get_ptr "ptr
get_ptr() const ";

%feature("docstring")  cmf::upslope::ET::constantETpot::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::ET::constantETpot::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::ET::constantETpot::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::ET::constantETpot::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::upslope::ET::constantETpot::GetETpot "real GetETpot(cmf::math::Time t) const ";

%feature("docstring")  cmf::upslope::ET::constantETpot::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::upslope::ET::constantETpot::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::ET::constantETpot::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::ET::constantETpot::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")  cmf::upslope::ET::constantETpot::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::ET::constantETpot::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::ET::constantETpot::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")  cmf::upslope::ET::constantETpot::short_string "virtual std::string short_string() const ";

%feature("docstring")  cmf::upslope::ET::constantETpot::to_string "virtual std::string to_string() const ";


// File: classcmf_1_1water_1_1_constant_flux.xml
%feature("docstring") cmf::water::ConstantFlux "

Produces a constant but changeable flux from a source to a target, if
enough water is present in the source.



.. math::

     q=\\\\begin{cases}0 & V_{source}\\\\le V_{min}\\\\\\\\ q_0
    \\\\frac{V_{source} - V_{min}}{t_{decr} q_{0} - V_{min}} & V_{source}
    \\\\le t_{decr} q_{0}\\\\\\\\ q_{0} & \\\\end{cases}

This is similar to a neumann boundary, however this is not a boundary
condition, but water is taken from the source (left) water storage and
limited by that water storage.

C++ includes: simple_connections.h ";

%feature("docstring")  cmf::water::ConstantFlux::ConstantFlux "ConstantFlux(cmf::water::WaterStorage::ptr source,
cmf::water::flux_node::ptr target, real maximum_flux, real
minimal_state=0, cmf::math::Time flux_decrease_time=cmf::math::h)

Produces a constant but changeable flux from a source to a target, if
enough water is present in the source.

Parameters:
-----------

source:  The source of the water

target:  The target of the water

maximum_flux:  The requested flux :math:`q_{0}`

minimal_state:  Minimal volume of stored water in source

flux_decrease_time:  ( cmf::math::Time) ";

%feature("docstring")  cmf::water::ConstantFlux::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")  cmf::water::ConstantFlux::exchange_target "void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")  cmf::water::ConstantFlux::get_ptr "ptr
get_ptr() const ";

%feature("docstring")  cmf::water::ConstantFlux::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::water::ConstantFlux::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")  cmf::water::ConstantFlux::get_tracer_filter "real get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")  cmf::water::ConstantFlux::get_tracer_filter "real get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::water::ConstantFlux::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::water::ConstantFlux::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::water::ConstantFlux::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::water::ConstantFlux::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")  cmf::water::ConstantFlux::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::water::ConstantFlux::set_tracer_filter "void set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")  cmf::water::ConstantFlux::set_tracer_filter "void set_tracer_filter(solute S, real value) ";

%feature("docstring")  cmf::water::ConstantFlux::short_string "virtual std::string short_string() const ";

%feature("docstring")  cmf::water::ConstantFlux::to_string "virtual
std::string to_string() const ";


// File: classcmf_1_1atmosphere_1_1_constant_meteorology.xml
%feature("docstring") cmf::atmosphere::ConstantMeteorology "

A primitive implementation of the Meteorology interface.

Holds a Weather record and returns it for any date

C++ includes: meteorology.h ";

%feature("docstring")
cmf::atmosphere::ConstantMeteorology::ConstantMeteorology "ConstantMeteorology()

Creates a ConstantMeteorology with a standard weather. ";

%feature("docstring")
cmf::atmosphere::ConstantMeteorology::ConstantMeteorology "ConstantMeteorology(const cmf::atmosphere::Weather &w)

Creates a ConstantMeteorology with weather w. ";

%feature("docstring")
cmf::atmosphere::ConstantMeteorology::ConstantMeteorology "ConstantMeteorology(const cmf::atmosphere::ConstantMeteorology &other)

Copy constructor. ";

%feature("docstring")  cmf::atmosphere::ConstantMeteorology::copy "ConstantMeteorology* copy() const

Creates a new instannce of the ConstantMeteorology with the same
weather. ";

%feature("docstring")
cmf::atmosphere::ConstantMeteorology::get_instrument_height "virtual
real get_instrument_height() const

Returns the height of the instruments above canopy. ";

%feature("docstring")
cmf::atmosphere::ConstantMeteorology::get_weather "virtual
cmf::atmosphere::Weather get_weather(cmf::math::Time t) const

Returns weather, regardless of t. ";


// File: classcmf_1_1atmosphere_1_1_constant_rain_source.xml
%feature("docstring") cmf::atmosphere::ConstantRainSource "

A simple implementation of RainSource.

Returns intensity for any time step.

C++ includes: precipitation.h ";

%feature("docstring")
cmf::atmosphere::ConstantRainSource::ConstantRainSource "ConstantRainSource(cmf::project &_project, cmf::geometry::point
location, real _intensity)

Creates a new ConstantRainSource.

Consider using Cell::set_rainfall for internal creation of a constant
rain source, instead of direct use

Parameters:
-----------

_project:  The project the rain source is belonging to.

location:  The location of the rain source

_intensity:  The constant rainfall intensity in mm/day ";

%feature("docstring")  cmf::atmosphere::ConstantRainSource::conc "virtual real conc(cmf::math::Time t, const cmf::water::solute &Solute)
const

Returns the concentration of a solute in the rainfall at time t. ";

%feature("docstring")
cmf::atmosphere::ConstantRainSource::connection_to "cmf::water::flux_connection* connection_to(const cmf::water::flux_node
&target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::atmosphere::ConstantRainSource::flux3d_to
"cmf::geometry::point flux3d_to(const cmf::water::flux_node &target,
cmf::math::Time t) ";

%feature("docstring")  cmf::atmosphere::ConstantRainSource::flux_to "real flux_to(const cmf::water::flux_node &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from this into target\") ";

%feature("docstring")
cmf::atmosphere::ConstantRainSource::get_3d_flux "cmf::geometry::point get_3d_flux(cmf::math::Time t)

Returns the sum of all flux vectors. ";

%feature("docstring")
cmf::atmosphere::ConstantRainSource::get_connections "cmf::water::connection_list get_connections() const ";

%feature("docstring")
cmf::atmosphere::ConstantRainSource::get_intensity "real
get_intensity(cmf::math::Time t) const

Returns the actual rainfall intensity in mm/day. ";

%feature("docstring")
cmf::atmosphere::ConstantRainSource::get_potential "virtual real
get_potential(cmf::math::Time=cmf::math::never) const

Returns the water potential of the node in m waterhead.

The base class water storage always returns the height of the location
";

%feature("docstring")
cmf::atmosphere::ConstantRainSource::get_project "cmf::project&
get_project() const

Returns the project, this node is part of. ";

%feature("docstring")  cmf::atmosphere::ConstantRainSource::is_empty "virtual double is_empty() const

Returns false. ";

%feature("docstring")  cmf::atmosphere::ConstantRainSource::is_storage
"virtual bool is_storage() const

true, if this is a waterstorage ";

%feature("docstring")
cmf::atmosphere::ConstantRainSource::RecalcFluxes "virtual bool
RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node.

WaterStorage overrides this, since state changes require an update of
the fluxes ";

%feature("docstring")
cmf::atmosphere::ConstantRainSource::remove_connection "bool
remove_connection(cmf::water::flux_node::ptr To)

Remove the connection. ";

%feature("docstring")  cmf::atmosphere::ConstantRainSource::set_conc "void set_conc(const cmf::water::solute &Solute, real value)

Sets the concentration of a solute in the rainfall. ";

%feature("docstring")
cmf::atmosphere::ConstantRainSource::set_potential "virtual void
set_potential(real new_potential)

Sets the potential of this flux node. ";

%feature("docstring")  cmf::atmosphere::ConstantRainSource::to_string
"virtual std::string to_string() const ";

%feature("docstring")
cmf::atmosphere::ConstantRainSource::waterbalance "real
waterbalance(cmf::math::Time t, const flux_connection *Without=0)
const

Returns the sum of all fluxes (positive and negative) at time t.

Single fluxes can be excluded from the calculation

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the waterbalance
(e.g. to prevent closed circuits) ";


// File: classcmf_1_1water_1_1_constant_state_flux.xml
%feature("docstring") cmf::water::ConstantStateFlux "

Calculates a flux to or from a water storage to hold it's state at a
more or less constant level.



.. math::

     q=\\\\frac{h_1 - h_{target}}{t_c [days]} 

where:  :math:`q` the
resulting flux in m3/day

:math:`h_1` the reference state

:math:`h_{target}` the state of the target (right) node

:math:`t_c` the time to reach the target state

C++ includes: simple_connections.h ";

%feature("docstring")
cmf::water::ConstantStateFlux::ConstantStateFlux "ConstantStateFlux(cmf::water::WaterStorage::ptr controlled_storage,
cmf::water::flux_node::ptr other_end, real target_state,
cmf::math::Time reaction_time)

Creates a flux connection to control the state of a storage.

Parameters:
-----------

controlled_storage:  Water storage, to be controlled

other_end:  source of missing water or target of excessive water

target_state:  State the controlled storage should hold (
:math:`h_{target}`)

reaction_time:  Time to reach state ( :math:`t_c`) ";

%feature("docstring")  cmf::water::ConstantStateFlux::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")  cmf::water::ConstantStateFlux::exchange_target
"void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")  cmf::water::ConstantStateFlux::get_ptr "ptr
get_ptr() const ";

%feature("docstring")  cmf::water::ConstantStateFlux::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::water::ConstantStateFlux::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::water::ConstantStateFlux::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::water::ConstantStateFlux::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::water::ConstantStateFlux::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::water::ConstantStateFlux::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::water::ConstantStateFlux::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::water::ConstantStateFlux::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")  cmf::water::ConstantStateFlux::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::water::ConstantStateFlux::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::water::ConstantStateFlux::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")  cmf::water::ConstantStateFlux::short_string "virtual std::string short_string() const ";

%feature("docstring")  cmf::water::ConstantStateFlux::to_string "virtual std::string to_string() const ";


// File: classcmf_1_1water_1_1_constraint_linear_storage_flux.xml
%feature("docstring") cmf::water::ConstraintLinearStorageFlux "

Calculates flux out of a storage as a linear function of its volume,
constraint by the volume stored in the target storage.



.. math::

     q = \\\\frac {1}{t_r} \\\\left({V_{l} - V_{l,min}}\\\\right)
    \\\\cdot \\\\left(\\\\frac{V_{r,max}-V_{r}}{V_{r,max}}\\\\right)

where:  :math:`V_l` The actual volume stored by the left water storage

:math:`V_{l,min} [m^3]` The volume of water not flowing out (default = 0)

:math:`\\\\beta` A parameter to shape the response curve. In case of
:math:`\\\\beta \\\\neq 1`, :math:`t_r` is not a residence time, but just a
parameter.

:math:`t_r [days]` The residence time of the water in this storage in days

:math:`V_{r,max}` The capacity of the right water storage in m3

:math:`V_{r}` The actual volume of the right water storage

:math:`\\\\gamma` A shape parameter for the target capacity constriction

C++ includes: simple_connections.h ";

%feature("docstring")
cmf::water::ConstraintLinearStorageFlux::ConstraintLinearStorageFlux "ConstraintLinearStorageFlux(WaterStorage::ptr source,
WaterStorage::ptr target, real residencetime=1.0, real Vlmin=0.0, real
Vrmax=1.0)

Creates a linear storage connection, constrained by the fill level of
the source 

.. math::

     q = \\\\frac 1 {t_r} {\\\\left(\\\\frac{V -
    V_{residual}}{V_0} \\\\right)^\\\\beta} 

.

Parameters:
-----------

source:  Water storage from which the water flows out. Flux is a
function of source.volume

target:  Target node (boundary condition or storage). Does not
influence the strength of the flow

residencetime:   :math:`t_r [days]` The residence time of the water in this
storage

Vlmin:   :math:`V_{l,min} [m^3]` The volume of water not flowing out
(default = 0)

Vrmax:   :math:`V_{r,max}` Capacity of the target water storage in m3 ";

%feature("docstring")  cmf::water::ConstraintLinearStorageFlux::conc "real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::water::ConstraintLinearStorageFlux::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")
cmf::water::ConstraintLinearStorageFlux::get_ptr "ptr get_ptr() const
";

%feature("docstring")
cmf::water::ConstraintLinearStorageFlux::get_target "flux_node::ptr
get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::water::ConstraintLinearStorageFlux::get_target "flux_node::ptr
get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::water::ConstraintLinearStorageFlux::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::water::ConstraintLinearStorageFlux::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")
cmf::water::ConstraintLinearStorageFlux::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::water::ConstraintLinearStorageFlux::left_node "flux_node::ptr
left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::water::ConstraintLinearStorageFlux::q "real q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::water::ConstraintLinearStorageFlux::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::water::ConstraintLinearStorageFlux::right_node "flux_node::ptr
right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::water::ConstraintLinearStorageFlux::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::water::ConstraintLinearStorageFlux::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::water::ConstraintLinearStorageFlux::short_string "virtual
std::string short_string() const ";

%feature("docstring")
cmf::water::ConstraintLinearStorageFlux::to_string "virtual
std::string to_string() const ";


// File: classcmf_1_1upslope_1_1_e_t_1_1_content_stress.xml
%feature("docstring") cmf::upslope::ET::ContentStress "

A simple water content based stress model based on Feddes (1978)

stress a is piecewise linear function based on water content

if :math:`\\\\theta>\\\\theta_d \\\\rightarrow ET = ET_{pot}`

if :math:`\\\\theta_d>\\\\theta > \\\\theta_{wilt} \\\\rightarrow ET = \\\\frac{\\\\theta_d - \\\\theta}{\\\\theta_d - \\\\theta_{wilt}} ET_{pot}`

if :math:`\\\\theta_{wilt}>\\\\theta \\\\rightarrow ET = 0.0`

The parameters :math:`\\\\theta_{d,w}` can be omitted (or set to a negative
value) to use default values derived from the layer properties. The
critical water content (where stress starts) is then defined (conf.
Feddes 1978) as the center between field capacity ( :math:`\\\\theta_{fc}`)
and the wilting point ( :math:`\\\\theta_{wp}`).  :math:`\\\\theta_w = \\\\theta(pF=4.2)`: Water content at pF=4.2

:math:`\\\\theta_d = 1/2 (\\\\theta(pF=1.8) + \\\\theta(pF=4.2))`

C++ includes: waterstress.h ";

%feature("docstring")  cmf::upslope::ET::ContentStress::ContentStress
"ContentStress(real theta_d=-1, real theta_w=-1)

Creates a new ContentStress for critical water content theta_d in vol
Water/vol Soil and wilting point theta_w. ";

%feature("docstring")  cmf::upslope::ET::ContentStress::~ContentStress
"virtual ~ContentStress() ";

%feature("docstring")  cmf::upslope::ET::ContentStress::copy "cmf::upslope::ET::ContentStress* copy() const

Creates a new copy of this wetness. ";

%feature("docstring")  cmf::upslope::ET::ContentStress::Tact "virtual
real Tact(const cmf::upslope::SoilLayer *soillayer, real Tpot) const

Calculates the water stress for a layer.

Parameters:
-----------

soillayer:  The cmf::upslope::SoilLayer this stress function belongs
to

Tpot:  Potential Transpiration in mm/day (for the full profile)

Actual flux from layer in m3/day ";

%feature("docstring")  cmf::upslope::ET::ContentStress::to_string "std::string to_string() const ";


// File: classcmf_1_1river_1_1_cross_section_reach.xml
%feature("docstring") cmf::river::CrossSectionReach "

Structure for the description of reaches with a freely defined cross
section.

Although double triangular cross section reach are rarely met, a
triangular reach does scale with its water load, and is therefore
preferable in case where nothing about IChannel geometry is known

C++ includes: cross_section_reach.h ";

%feature("docstring")
cmf::river::CrossSectionReach::CrossSectionReach "CrossSectionReach(double l, cmf::math::num_array x,
cmf::math::num_array depth)

Creates a new triangular reach type. ";

%feature("docstring")
cmf::river::CrossSectionReach::CrossSectionReach "CrossSectionReach(const CrossSectionReach &copy) ";

%feature("docstring")
cmf::river::CrossSectionReach::~CrossSectionReach "virtual
~CrossSectionReach() ";

%feature("docstring")  cmf::river::CrossSectionReach::A "virtual
double A(double V) const

Returns the area of the surface for a given volume. ";

%feature("docstring")  cmf::river::CrossSectionReach::check_iterator "void check_iterator(double h) const ";

%feature("docstring")  cmf::river::CrossSectionReach::copy "CrossSectionReach* copy() const ";

%feature("docstring")
cmf::river::CrossSectionReach::get_channel_width "virtual double
get_channel_width(double depth) const

Calculates the flow width from a given actual depth [m] using the
actual IChannel geometry. ";

%feature("docstring")  cmf::river::CrossSectionReach::get_depth "virtual double get_depth(double area) const

Returns the depth at a given crossection area. ";

%feature("docstring")
cmf::river::CrossSectionReach::get_flux_crossection "virtual double
get_flux_crossection(double depth) const

Returns the crossection area at a given depth. ";

%feature("docstring")  cmf::river::CrossSectionReach::get_length "double get_length() const

Length of the reach. ";

%feature("docstring")  cmf::river::CrossSectionReach::get_nManning "virtual double get_nManning() const ";

%feature("docstring")
cmf::river::CrossSectionReach::get_wetted_perimeter "virtual double
get_wetted_perimeter(double depth) const

Returns the wetted perimeter at a given depth. ";

%feature("docstring")  cmf::river::CrossSectionReach::h "virtual
double h(double V) const

Returns the depth of a given volume. ";

%feature("docstring")  cmf::river::CrossSectionReach::q "virtual
double q(double h, double slope) const ";

%feature("docstring")  cmf::river::CrossSectionReach::qManning "virtual double qManning(double A, double slope) const

Calculates the flow rate from a given water volume in the reach


.. math::

     q_{Manning}&=& A R^{\\\\frac 2 3}
    \\\\sqrt{\\\\frac {\\\\Delta_z} n} \\\\\\\\ A &=& \\\\frac V l
    \\\\mbox{, (Crosssectional area of the wetted crossection, Volume per
    length)} \\\\\\\\ R &=& \\\\frac A {P(d)} \\\\\\\\ P(d) &=& \\\\mbox{
    the perimeter of the wetted crosssection, a function of reach depth}
    \\\\\\\\ d(V) &=& \\\\mbox{ the depth of the reach a function of the
    volume} \\\\\\\\ \\\\Delta_z &=& \\\\frac{z_{max} - z_{min}}{l}
    \\\\mbox{ Slope of the reach} 

.

Flow rate [m3/s]

Parameters:
-----------

A:  The area of the cross section [m2]

slope:  The slope of the reach [m/m] ";

%feature("docstring")  cmf::river::CrossSectionReach::set_nManning "virtual void set_nManning(double nManning) ";

%feature("docstring")  cmf::river::CrossSectionReach::typecode "char
typecode() const ";

%feature("docstring")  cmf::river::CrossSectionReach::V "virtual
double V(double h) const ";


// File: classcmf_1_1math_1_1cubicspline.xml
%feature("docstring") cmf::math::cubicspline "

Interpolates points with a cubic spline interpolation.

Code is modified
after:http://ganeshtiwaridotcomdotnp.blogspot.de/2009/12/c-c-code-
cubic- spline-interpolation.html

C++ includes: spline.h ";

%feature("docstring")  cmf::math::cubicspline::cubicspline "cubicspline(const cmf::math::num_array &x, const cmf::math::num_array
&y) ";

%feature("docstring")  cmf::math::cubicspline::generate "void
generate() ";

%feature("docstring")  cmf::math::cubicspline::size "size_t size()
const ";


// File: classcmf_1_1math_1_1_c_vode_adams.xml
%feature("docstring") cmf::math::CVodeAdams "

Explizit multistep solver using CVode.

C++ includes: cvode.h ";

/*  Accuracy parameters  */

/*  model time  */

%feature("docstring")  cmf::math::CVodeAdams::get_t "cmf::math::Time
get_t() const

Returns the current model time. ";

%feature("docstring")  cmf::math::CVodeAdams::set_t "void
set_t(cmf::math::Time val)

Sets the current model time. ";

%feature("docstring")  cmf::math::CVodeAdams::get_dt "cmf::math::Time
get_dt() const

Returns the last time step. ";

/*  Integrate  */

%feature("docstring")  cmf::math::CVodeAdams::integrate_until "void
integrate_until(cmf::math::Time t_max, cmf::math::Time dt=Time(), bool
reset=false)

Integrates the vector of state variables until t_max.

Parameters:
-----------

t_max:   Time, the solver should run to

dt:   Time step (may be omitted)

reset:  If true, solver is reseted before integration starts ";

%feature("docstring")  cmf::math::CVodeAdams::CVodeAdams "CVodeAdams(const cmf::math::state_list &states, real epsilon=1e-9) ";

%feature("docstring")  cmf::math::CVodeAdams::_get_jacobian "virtual
cmf::math::num_array _get_jacobian() const

Returns a continuous 1D array representing the Jacobian columns
concatenated.

In Python, get_jacobian returns the Jacobian as a 2D array ";

%feature("docstring")  cmf::math::CVodeAdams::copy "CVodeBase* copy()
const

Returns a copy of the solver. ";

%feature("docstring")  cmf::math::CVodeAdams::get_error "cmf::math::num_array get_error() const

Error vector of the integrator. ";

%feature("docstring")  cmf::math::CVodeAdams::get_info "CVodeInfo
get_info() const

Returns the current solver statistics. ";

%feature("docstring")  cmf::math::CVodeAdams::initialize "int
initialize()

Initialize the internal memory.

Automatically called, when one starts to integrate ";

%feature("docstring")  cmf::math::CVodeAdams::integrate "virtual int
integrate(cmf::math::Time t_max, cmf::math::Time dt)

Integrates the vector of state variables.

Parameters:
-----------

t_max:  To stop the model (if running in a model framework) at time
steps of value exchange e.g. full hours, the next value exchange time
can be given

dt:  Takes the proposed time step, and changes it into the effectively
used time step according to the local stiffness of the problem and
MaxTime ";

%feature("docstring")  cmf::math::CVodeAdams::reset "virtual void
reset()

Resets the history of the multispte solver and overwrites the internal
state cache. ";

%feature("docstring")  cmf::math::CVodeAdams::set_error_msg "void
set_error_msg(std::string error)

Sets an error message. ";

%feature("docstring")  cmf::math::CVodeAdams::set_system "void
set_system(const state_list &states) ";

%feature("docstring")  cmf::math::CVodeAdams::size "size_t size()
const ";

%feature("docstring")  cmf::math::CVodeAdams::to_string "std::string
to_string() const

Returns a string representation of the solver. ";


// File: classcmf_1_1math_1_1_c_vode_banded.xml
%feature("docstring") cmf::math::CVodeBanded "

implicit BDF CVode solver with a banded Jacobian approximation

C++ includes: cvode.h ";

/*  Accuracy parameters  */

/*  model time  */

%feature("docstring")  cmf::math::CVodeBanded::get_t "cmf::math::Time
get_t() const

Returns the current model time. ";

%feature("docstring")  cmf::math::CVodeBanded::set_t "void
set_t(cmf::math::Time val)

Sets the current model time. ";

%feature("docstring")  cmf::math::CVodeBanded::get_dt "cmf::math::Time get_dt() const

Returns the last time step. ";

/*  Integrate  */

%feature("docstring")  cmf::math::CVodeBanded::integrate_until "void
integrate_until(cmf::math::Time t_max, cmf::math::Time dt=Time(), bool
reset=false)

Integrates the vector of state variables until t_max.

Parameters:
-----------

t_max:   Time, the solver should run to

dt:   Time step (may be omitted)

reset:  If true, solver is reseted before integration starts ";

%feature("docstring")  cmf::math::CVodeBanded::CVodeBanded "CVodeBanded(const cmf::math::state_list &states, real epsilon=1e-9,
int w=5) ";

%feature("docstring")  cmf::math::CVodeBanded::_get_jacobian "virtual
cmf::math::num_array _get_jacobian() const

Returns a continuous 1D array representing the Jacobian columns
concatenated.

In Python, get_jacobian returns the Jacobian as a 2D array ";

%feature("docstring")  cmf::math::CVodeBanded::copy "CVodeBase*
copy() const

Returns a copy of the solver. ";

%feature("docstring")  cmf::math::CVodeBanded::get_error "cmf::math::num_array get_error() const

Error vector of the integrator. ";

%feature("docstring")  cmf::math::CVodeBanded::get_info "CVodeInfo
get_info() const

Returns the current solver statistics. ";

%feature("docstring")  cmf::math::CVodeBanded::initialize "int
initialize()

Initialize the internal memory.

Automatically called, when one starts to integrate ";

%feature("docstring")  cmf::math::CVodeBanded::integrate "virtual int
integrate(cmf::math::Time t_max, cmf::math::Time dt)

Integrates the vector of state variables.

Parameters:
-----------

t_max:  To stop the model (if running in a model framework) at time
steps of value exchange e.g. full hours, the next value exchange time
can be given

dt:  Takes the proposed time step, and changes it into the effectively
used time step according to the local stiffness of the problem and
MaxTime ";

%feature("docstring")  cmf::math::CVodeBanded::reset "virtual void
reset()

Resets the history of the multispte solver and overwrites the internal
state cache. ";

%feature("docstring")  cmf::math::CVodeBanded::set_error_msg "void
set_error_msg(std::string error)

Sets an error message. ";

%feature("docstring")  cmf::math::CVodeBanded::set_system "void
set_system(const state_list &states) ";

%feature("docstring")  cmf::math::CVodeBanded::size "size_t size()
const ";

%feature("docstring")  cmf::math::CVodeBanded::to_string "std::string
to_string() const override

Returns a string representation of the solver. ";


// File: classcmf_1_1math_1_1_c_vode_base.xml
%feature("docstring") cmf::math::CVodeBase "

Abstract base class for different modes of the CVode solver.

Initantiate one of the child classes to gain different modes of the
CVode solver

C++ includes: cvode.h ";

/*  Accuracy parameters  */

/*  model time  */

%feature("docstring")  cmf::math::CVodeBase::get_t "cmf::math::Time
get_t() const

Returns the current model time. ";

%feature("docstring")  cmf::math::CVodeBase::set_t "void
set_t(cmf::math::Time val)

Sets the current model time. ";

%feature("docstring")  cmf::math::CVodeBase::get_dt "cmf::math::Time
get_dt() const

Returns the last time step. ";

/*  Integrate  */

%feature("docstring")  cmf::math::CVodeBase::integrate_until "void
integrate_until(cmf::math::Time t_max, cmf::math::Time dt=Time(), bool
reset=false)

Integrates the vector of state variables until t_max.

Parameters:
-----------

t_max:   Time, the solver should run to

dt:   Time step (may be omitted)

reset:  If true, solver is reseted before integration starts ";

%feature("docstring")  cmf::math::CVodeBase::~CVodeBase "~CVodeBase()
";

%feature("docstring")  cmf::math::CVodeBase::_get_jacobian "virtual
cmf::math::num_array _get_jacobian() const

Returns a continuous 1D array representing the Jacobian columns
concatenated.

In Python, get_jacobian returns the Jacobian as a 2D array ";

%feature("docstring")  cmf::math::CVodeBase::copy "CVodeBase* copy()
const

Returns a copy of the solver. ";

%feature("docstring")  cmf::math::CVodeBase::get_error "cmf::math::num_array get_error() const

Error vector of the integrator. ";

%feature("docstring")  cmf::math::CVodeBase::get_info "CVodeInfo
get_info() const

Returns the current solver statistics. ";

%feature("docstring")  cmf::math::CVodeBase::initialize "int
initialize()

Initialize the internal memory.

Automatically called, when one starts to integrate ";

%feature("docstring")  cmf::math::CVodeBase::integrate "virtual int
integrate(cmf::math::Time t_max, cmf::math::Time dt)

Integrates the vector of state variables.

Parameters:
-----------

t_max:  To stop the model (if running in a model framework) at time
steps of value exchange e.g. full hours, the next value exchange time
can be given

dt:  Takes the proposed time step, and changes it into the effectively
used time step according to the local stiffness of the problem and
MaxTime ";

%feature("docstring")  cmf::math::CVodeBase::reset "virtual void
reset()

Resets the history of the multispte solver and overwrites the internal
state cache. ";

%feature("docstring")  cmf::math::CVodeBase::set_error_msg "void
set_error_msg(std::string error)

Sets an error message. ";

%feature("docstring")  cmf::math::CVodeBase::set_system "void
set_system(const state_list &states) ";

%feature("docstring")  cmf::math::CVodeBase::size "size_t size()
const ";

%feature("docstring")  cmf::math::CVodeBase::to_string "virtual
std::string to_string() const =0

Returns a string representation of the solver. ";


// File: classcmf_1_1math_1_1_c_vode_dense.xml
%feature("docstring") cmf::math::CVodeDense "

implicit BDF CVode solver with full Jacobian approximation

Use this solver for small but stiff systems (<20 state variables)

The solver calculates for each step the full Jacobian matrix of the
system using a difference quotient approximation of the real Jacobian

C++ includes: cvode.h ";

/*  Accuracy parameters  */

/*  model time  */

%feature("docstring")  cmf::math::CVodeDense::get_t "cmf::math::Time
get_t() const

Returns the current model time. ";

%feature("docstring")  cmf::math::CVodeDense::set_t "void
set_t(cmf::math::Time val)

Sets the current model time. ";

%feature("docstring")  cmf::math::CVodeDense::get_dt "cmf::math::Time
get_dt() const

Returns the last time step. ";

/*  Integrate  */

%feature("docstring")  cmf::math::CVodeDense::integrate_until "void
integrate_until(cmf::math::Time t_max, cmf::math::Time dt=Time(), bool
reset=false)

Integrates the vector of state variables until t_max.

Parameters:
-----------

t_max:   Time, the solver should run to

dt:   Time step (may be omitted)

reset:  If true, solver is reseted before integration starts ";

%feature("docstring")  cmf::math::CVodeDense::CVodeDense "CVodeDense(const cmf::math::state_list &states, real epsilon=1e-9)

Creates a new implicit dense CVode solver. ";

%feature("docstring")  cmf::math::CVodeDense::_get_jacobian "virtual
cmf::math::num_array _get_jacobian() const

Returns a continuous 1D array representing the Jacobian columns
concatenated.

In Python, get_jacobian returns the Jacobian as a 2D array ";

%feature("docstring")  cmf::math::CVodeDense::copy "CVodeBase* copy()
const

Returns a copy of the solver. ";

%feature("docstring")  cmf::math::CVodeDense::get_error "cmf::math::num_array get_error() const

Error vector of the integrator. ";

%feature("docstring")  cmf::math::CVodeDense::get_info "CVodeInfo
get_info() const

Returns the current solver statistics. ";

%feature("docstring")  cmf::math::CVodeDense::initialize "int
initialize()

Initialize the internal memory.

Automatically called, when one starts to integrate ";

%feature("docstring")  cmf::math::CVodeDense::integrate "virtual int
integrate(cmf::math::Time t_max, cmf::math::Time dt)

Integrates the vector of state variables.

Parameters:
-----------

t_max:  To stop the model (if running in a model framework) at time
steps of value exchange e.g. full hours, the next value exchange time
can be given

dt:  Takes the proposed time step, and changes it into the effectively
used time step according to the local stiffness of the problem and
MaxTime ";

%feature("docstring")  cmf::math::CVodeDense::reset "virtual void
reset()

Resets the history of the multispte solver and overwrites the internal
state cache. ";

%feature("docstring")  cmf::math::CVodeDense::set_error_msg "void
set_error_msg(std::string error)

Sets an error message. ";

%feature("docstring")  cmf::math::CVodeDense::set_system "void
set_system(const state_list &states) ";

%feature("docstring")  cmf::math::CVodeDense::size "size_t size()
const ";

%feature("docstring")  cmf::math::CVodeDense::to_string "std::string
to_string() const

Returns a string representation of the solver. ";


// File: classcmf_1_1math_1_1_c_vode_diag.xml
%feature("docstring") cmf::math::CVodeDiag "

implicit BDF CVode solver with a one line diagonal Jacobian
approximation

C++ includes: cvode.h ";

/*  Accuracy parameters  */

/*  model time  */

%feature("docstring")  cmf::math::CVodeDiag::get_t "cmf::math::Time
get_t() const

Returns the current model time. ";

%feature("docstring")  cmf::math::CVodeDiag::set_t "void
set_t(cmf::math::Time val)

Sets the current model time. ";

%feature("docstring")  cmf::math::CVodeDiag::get_dt "cmf::math::Time
get_dt() const

Returns the last time step. ";

/*  Integrate  */

%feature("docstring")  cmf::math::CVodeDiag::integrate_until "void
integrate_until(cmf::math::Time t_max, cmf::math::Time dt=Time(), bool
reset=false)

Integrates the vector of state variables until t_max.

Parameters:
-----------

t_max:   Time, the solver should run to

dt:   Time step (may be omitted)

reset:  If true, solver is reseted before integration starts ";

%feature("docstring")  cmf::math::CVodeDiag::CVodeDiag "CVodeDiag(const cmf::math::state_list &states, real epsilon=1e-9) ";

%feature("docstring")  cmf::math::CVodeDiag::_get_jacobian "virtual
cmf::math::num_array _get_jacobian() const

Returns a continuous 1D array representing the Jacobian columns
concatenated.

In Python, get_jacobian returns the Jacobian as a 2D array ";

%feature("docstring")  cmf::math::CVodeDiag::copy "CVodeBase* copy()
const

Returns a copy of the solver. ";

%feature("docstring")  cmf::math::CVodeDiag::get_error "cmf::math::num_array get_error() const

Error vector of the integrator. ";

%feature("docstring")  cmf::math::CVodeDiag::get_info "CVodeInfo
get_info() const

Returns the current solver statistics. ";

%feature("docstring")  cmf::math::CVodeDiag::initialize "int
initialize()

Initialize the internal memory.

Automatically called, when one starts to integrate ";

%feature("docstring")  cmf::math::CVodeDiag::integrate "virtual int
integrate(cmf::math::Time t_max, cmf::math::Time dt)

Integrates the vector of state variables.

Parameters:
-----------

t_max:  To stop the model (if running in a model framework) at time
steps of value exchange e.g. full hours, the next value exchange time
can be given

dt:  Takes the proposed time step, and changes it into the effectively
used time step according to the local stiffness of the problem and
MaxTime ";

%feature("docstring")  cmf::math::CVodeDiag::reset "virtual void
reset()

Resets the history of the multispte solver and overwrites the internal
state cache. ";

%feature("docstring")  cmf::math::CVodeDiag::set_error_msg "void
set_error_msg(std::string error)

Sets an error message. ";

%feature("docstring")  cmf::math::CVodeDiag::set_system "void
set_system(const state_list &states) ";

%feature("docstring")  cmf::math::CVodeDiag::size "size_t size()
const ";

%feature("docstring")  cmf::math::CVodeDiag::to_string "std::string
to_string() const

Returns a string representation of the solver. ";


// File: structcmf_1_1math_1_1_c_vode_info.xml
%feature("docstring") cmf::math::CVodeInfo "

Reports the current state of a CVode solver.

C++ includes: cvode.h ";

%feature("docstring")  cmf::math::CVodeInfo::to_string "std::string
to_string() const ";


// File: classcmf_1_1math_1_1_c_vode_k_l_u.xml
%feature("docstring") cmf::math::CVodeKLU "";

/*  model time  */

%feature("docstring")  cmf::math::CVodeKLU::get_t "cmf::math::Time
get_t() const

Returns the current model time. ";

%feature("docstring")  cmf::math::CVodeKLU::set_t "void
set_t(cmf::math::Time val)

Sets the current model time. ";

%feature("docstring")  cmf::math::CVodeKLU::get_dt "cmf::math::Time
get_dt() const

Returns the last time step. ";

/*  Integrate  */

%feature("docstring")  cmf::math::CVodeKLU::integrate_until "void
integrate_until(cmf::math::Time t_max, cmf::math::Time dt=Time(), bool
reset=false)

Integrates the vector of state variables until t_max.

Parameters:
-----------

t_max:   Time, the solver should run to

dt:   Time step (may be omitted)

reset:  If true, solver is reseted before integration starts ";

/*  Accuracy parameters  */

%feature("docstring")  cmf::math::CVodeKLU::CVodeKLU "CVodeKLU(const
cmf::math::state_list &states, real epsilon=1e-9) ";

%feature("docstring")  cmf::math::CVodeKLU::_get_jacobian "virtual
cmf::math::num_array _get_jacobian() const ";

%feature("docstring")  cmf::math::CVodeKLU::copy "CVodeBase* copy()
const

Returns a copy of the solver. ";

%feature("docstring")  cmf::math::CVodeKLU::get_error "cmf::math::num_array get_error() const

Error vector of the integrator. ";

%feature("docstring")  cmf::math::CVodeKLU::get_info "CVodeInfo
get_info() const

Returns the current solver statistics. ";

%feature("docstring")  cmf::math::CVodeKLU::initialize "int
initialize()

Initialize the internal memory.

Automatically called, when one starts to integrate ";

%feature("docstring")  cmf::math::CVodeKLU::integrate "virtual int
integrate(cmf::math::Time t_max, cmf::math::Time dt)

Integrates the vector of state variables.

Parameters:
-----------

t_max:  To stop the model (if running in a model framework) at time
steps of value exchange e.g. full hours, the next value exchange time
can be given

dt:  Takes the proposed time step, and changes it into the effectively
used time step according to the local stiffness of the problem and
MaxTime ";

%feature("docstring")  cmf::math::CVodeKLU::reset "virtual void
reset()

Resets the history of the multispte solver and overwrites the internal
state cache. ";

%feature("docstring")  cmf::math::CVodeKLU::set_error_msg "void
set_error_msg(std::string error)

Sets an error message. ";

%feature("docstring")  cmf::math::CVodeKLU::set_system "void
set_system(const state_list &states) ";

%feature("docstring")  cmf::math::CVodeKLU::size "size_t size() const
";

%feature("docstring")  cmf::math::CVodeKLU::to_string "std::string
to_string() const ";


// File: classcmf_1_1math_1_1_c_vode_krylov.xml
%feature("docstring") cmf::math::CVodeKrylov "

implicit BDF CVode solver with a Krylov preconditioner

C++ includes: cvode.h ";

/*  Accuracy parameters  */

/*  model time  */

%feature("docstring")  cmf::math::CVodeKrylov::get_t "cmf::math::Time
get_t() const

Returns the current model time. ";

%feature("docstring")  cmf::math::CVodeKrylov::set_t "void
set_t(cmf::math::Time val)

Sets the current model time. ";

%feature("docstring")  cmf::math::CVodeKrylov::get_dt "cmf::math::Time get_dt() const

Returns the last time step. ";

/*  Integrate  */

%feature("docstring")  cmf::math::CVodeKrylov::integrate_until "void
integrate_until(cmf::math::Time t_max, cmf::math::Time dt=Time(), bool
reset=false)

Integrates the vector of state variables until t_max.

Parameters:
-----------

t_max:   Time, the solver should run to

dt:   Time step (may be omitted)

reset:  If true, solver is reseted before integration starts ";

%feature("docstring")  cmf::math::CVodeKrylov::CVodeKrylov "CVodeKrylov(const cmf::math::state_list &states, real epsilon=1e-9,
int w=5, char p='L') ";

%feature("docstring")  cmf::math::CVodeKrylov::_get_jacobian "virtual
cmf::math::num_array _get_jacobian() const

Returns a continuous 1D array representing the Jacobian columns
concatenated.

In Python, get_jacobian returns the Jacobian as a 2D array ";

%feature("docstring")  cmf::math::CVodeKrylov::copy "CVodeBase*
copy() const

Returns a copy of the solver. ";

%feature("docstring")  cmf::math::CVodeKrylov::get_error "cmf::math::num_array get_error() const

Error vector of the integrator. ";

%feature("docstring")  cmf::math::CVodeKrylov::get_info "CVodeInfo
get_info() const

Returns the current solver statistics. ";

%feature("docstring")  cmf::math::CVodeKrylov::initialize "int
initialize()

Initialize the internal memory.

Automatically called, when one starts to integrate ";

%feature("docstring")  cmf::math::CVodeKrylov::integrate "virtual int
integrate(cmf::math::Time t_max, cmf::math::Time dt)

Integrates the vector of state variables.

Parameters:
-----------

t_max:  To stop the model (if running in a model framework) at time
steps of value exchange e.g. full hours, the next value exchange time
can be given

dt:  Takes the proposed time step, and changes it into the effectively
used time step according to the local stiffness of the problem and
MaxTime ";

%feature("docstring")  cmf::math::CVodeKrylov::reset "virtual void
reset()

Resets the history of the multispte solver and overwrites the internal
state cache. ";

%feature("docstring")  cmf::math::CVodeKrylov::set_error_msg "void
set_error_msg(std::string error)

Sets an error message. ";

%feature("docstring")  cmf::math::CVodeKrylov::set_system "void
set_system(const state_list &states) ";

%feature("docstring")  cmf::math::CVodeKrylov::size "size_t size()
const ";

%feature("docstring")  cmf::math::CVodeKrylov::to_string "std::string
to_string() const

Returns a string representation of the solver. ";


// File: structcmf_1_1math_1_1_c_vode_options.xml
%feature("docstring") cmf::math::CVodeOptions "

A set of options for all CVode3 solver.

Negative numbers indicate that this option stays on the default value.
For the meaning of the options see CVODE-UD, section 4.5.6

See Hindmarsh, A., Serban, R. and Reynolds, D.: User Documentation for
cvode v3.1.0, 2017, UCRL-SM-208108

Usage example: >>>solver = CVodeDens(p, 1e-9)
>>>solver.options.max_order = 2

C++ includes: cvode.h ";

%feature("docstring")  cmf::math::CVodeOptions::CVodeOptions "CVodeOptions() ";


// File: classcmf_1_1upslope_1_1connections_1_1_darcy.xml
%feature("docstring") cmf::upslope::connections::Darcy "

Calculates the lateral flow using the gravitational potential gradient
only.



.. math::

     q_{lat} = \\\\frac{\\\\Delta \\\\Psi_G}{\\\\|C_1-C_2\\\\|}
    \\\\frac 12 (T(C_1)+T(C_2)) w 

where  :math:`\\\\Delta \\\\Psi_G` is
the gravitational potential difference

:math:`\\\\|C_1-C_2\\\\|` is the distance from Cell 1 to Cell 2

:math:`T(C)` is the transmissivity of cell C, calculated by
SoilType::Transmissivity

:math:`w` is the width of the connection of the cells

C++ includes: subsurfacefluxes.h ";

%feature("docstring")  cmf::upslope::connections::Darcy::Darcy "Darcy(cmf::upslope::SoilLayer::ptr left, cmf::water::flux_node::ptr
right, real FlowWidth, real Distance=0) ";

%feature("docstring")  cmf::upslope::connections::Darcy::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::Darcy::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::connections::Darcy::get_ptr "ptr
get_ptr() const ";

%feature("docstring")  cmf::upslope::connections::Darcy::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::connections::Darcy::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::Darcy::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::Darcy::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::upslope::connections::Darcy::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::upslope::connections::Darcy::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::connections::Darcy::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::connections::Darcy::refresh "void refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")  cmf::upslope::connections::Darcy::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::Darcy::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::Darcy::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")  cmf::upslope::connections::Darcy::short_string
"virtual std::string short_string() const ";

%feature("docstring")  cmf::upslope::connections::Darcy::to_string "virtual std::string to_string() const ";


// File: classcmf_1_1upslope_1_1connections_1_1_darcy_kinematic.xml
%feature("docstring") cmf::upslope::connections::DarcyKinematic "

A simple kinemtic wave model for subsurface flux.



.. math::

     q = \\\\frac{\\\\Delta z_{surface}}{d} K(\\\\theta)_{source}
    A_{cross} 

C++ includes: subsurfacefluxes.h ";

%feature("docstring")
cmf::upslope::connections::DarcyKinematic::DarcyKinematic "DarcyKinematic(cmf::upslope::SoilLayer::ptr left,
cmf::water::flux_node::ptr right, real FlowWidth, real Distance=0)

Creates the connection.

Parameters:
-----------

left:  Left node of the connection (needs to be soil water storage)

right:  Right node of the connection (can be any node)

FlowWidth:  the width of the connection - is multiplied by layer
thickness to get the interface area

Distance:  the length of the connection. If 0, the distance is
calculated from the position of the nodes ";

%feature("docstring")  cmf::upslope::connections::DarcyKinematic::conc
"real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::DarcyKinematic::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")
cmf::upslope::connections::DarcyKinematic::get_ptr "ptr get_ptr()
const ";

%feature("docstring")
cmf::upslope::connections::DarcyKinematic::get_target "flux_node::ptr
get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::DarcyKinematic::get_target "flux_node::ptr
get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::DarcyKinematic::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::DarcyKinematic::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")
cmf::upslope::connections::DarcyKinematic::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::DarcyKinematic::left_node "flux_node::ptr
left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::connections::DarcyKinematic::q "real q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::DarcyKinematic::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::upslope::connections::DarcyKinematic::right_node "flux_node::ptr
right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::DarcyKinematic::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::DarcyKinematic::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::connections::DarcyKinematic::short_string "virtual
std::string short_string() const ";

%feature("docstring")
cmf::upslope::connections::DarcyKinematic::to_string "virtual
std::string to_string() const ";


// File: structcmf_1_1math_1_1_date.xml
%feature("docstring") cmf::math::Date "

An absolute time, not for calculation. Date and Time are
interchangable.

C++ includes: time.h ";

%feature("docstring")  cmf::math::Date::Date "Date(int _day, int
_month, int _year, int _hour=0, int _minute=0, int _second=0, int
_ms=0)

Creates a new date. ";

%feature("docstring")  cmf::math::Date::Date "Date(const Time &time)

Creates a new date from time (based on the 31.12.1899, like in
Excel(TM)) ";

%feature("docstring")  cmf::math::Date::DOY "double DOY()

Returns the day of year. ";

%feature("docstring")  cmf::math::Date::to_string "std::string
to_string()

Returns a string representing the date. ";

%feature("docstring")  cmf::math::Date::ToTime "Time ToTime()

Converts a date to Time (based on the 31.12.1899, like in Excel(TM) ";


// File: classcmf_1_1upslope_1_1connections_1_1_diffusive_macro_micro_exchange.xml
%feature("docstring")
cmf::upslope::connections::DiffusiveMacroMicroExchange "

A simple first order diffusive water exchange between MacroPore and
matrix ( SoilLayer)



.. math::

     q = \\\\omega (W_{ma} - W_{mi,eff}) V_{soil}

where:
:math:`\\\\omega` is the exchange rate in :math:`day^{-1}`

:math:`W_{ma}` is the filled fraction of the macropore system [-]

:math:`W_{mi,eff}` is the water filled pore space of the micropores above
the residual pF value [-], default 4.2

:math:`V_{soil} = A_{cell} d_{layer}` is the total volume of the soil layer
[ :math:`m^3`]

The residual micropore pF is used to determine a residual water
content of the micropores. Residual in this case means, that above
this pF value, water is not draining to the macro pores, even if they
are empty. Although the default value is at wilting point, lower pF
values are much more sensible, and should be rather lower than field
capacity (pF=1.8 - 2.5). However, since this equation is rather
conceptual than physical, this value can only be estimated or
calibrated.

cf. Simunek et al J. of Hydr. 2003

C++ includes: macropore.h ";

%feature("docstring")
cmf::upslope::connections::DiffusiveMacroMicroExchange::DiffusiveMacroMicroExchange
"DiffusiveMacroMicroExchange(cmf::upslope::MacroPore::ptr left,
cmf::upslope::SoilLayer::ptr right, real omega, real pFrmi=4.2) ";

%feature("docstring")
cmf::upslope::connections::DiffusiveMacroMicroExchange::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::DiffusiveMacroMicroExchange::exchange_target
"void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")
cmf::upslope::connections::DiffusiveMacroMicroExchange::get_ptr "ptr
get_ptr() const ";

%feature("docstring")
cmf::upslope::connections::DiffusiveMacroMicroExchange::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::DiffusiveMacroMicroExchange::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::DiffusiveMacroMicroExchange::get_tracer_filter
"real get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::DiffusiveMacroMicroExchange::get_tracer_filter
"real get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")
cmf::upslope::connections::DiffusiveMacroMicroExchange::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::DiffusiveMacroMicroExchange::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::connections::DiffusiveMacroMicroExchange::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::DiffusiveMacroMicroExchange::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::upslope::connections::DiffusiveMacroMicroExchange::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::DiffusiveMacroMicroExchange::set_tracer_filter
"void set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::DiffusiveMacroMicroExchange::set_tracer_filter
"void set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::connections::DiffusiveMacroMicroExchange::short_string "virtual std::string short_string() const ";

%feature("docstring")
cmf::upslope::connections::DiffusiveMacroMicroExchange::to_string "virtual std::string to_string() const ";


// File: classcmf_1_1upslope_1_1connections_1_1_diffusive_surface_runoff.xml
%feature("docstring")
cmf::upslope::connections::DiffusiveSurfaceRunoff "

A connection to route water from a SurfaceWater storage to another
node following the gradient of the water level.



.. math::

    q_{runoff} = A_{cross} d_{eff}^{2/3}
    \\\\frac{\\\\sqrt{S}}{n}

where:  :math:`q_{runoff}` is the surface
runoff

:math:`A_{cross}` is the wetted crossectional flux area, given as :math:`d_{eff} \\\\cdot w`

:math:`w` is the width of the shared boundary between the surface water
storage and the target node

:math:`d_{eff}` is the effective flow depth of the surface water.The
effective flow depth is defined as either the mean of the effective
depth of the left and the right node (when use_depthmax=false) or the
maximum of the efficitve depth. The effective depth for a surfacewater
is always defined as: 

.. math::

    d_{eff} = \\\\begin{cases}
    V/A-d_{puddle}\\\\ & V/A>d_{puddle} \\\\\\\\ 0.0 & V/A<=d_{puddle}
    \\\\end{cases}

The right node might be not a surfacewater. If the
right node is an OpenWaterStorage, then the effective depth is the
depth of the OWS above the cell height of the left surfacewater, given
by: 

.. math::

    d_{eff,ows} = \\\\Psi_{ows} - z_{cell}

In case of
another node, the right node depth equals the effective depth of the
left node.

:math:`V` the volume of stored water in the surface in :math:`m^3`

:math:`A` the area of the cell in :math:`m^2`

:math:`d_{puddle}=V_{puddle}/A` the average depth of water in the surface
water needed to start run off

:math:`S = \\\\|\\\\frac{\\\\Delta h\\\\|}{d}` the slope between
surfacewater center potential and the target node potential

:math:`n` the manning roughness

The DiffusiveSurfaceRunoff can be used as a cell connecting flux as
in: This results in a connection of the surfacewater storage of each
cell with the surface water storages of its neighborssee

C++ includes: surfacewater.h ";

%feature("docstring")
cmf::upslope::connections::DiffusiveSurfaceRunoff::DiffusiveSurfaceRunoff
"DiffusiveSurfaceRunoff(cmf::upslope::SurfaceWater::ptr left,
cmf::water::flux_node::ptr right, real flowwidth, real distance=-1) ";

%feature("docstring")
cmf::upslope::connections::DiffusiveSurfaceRunoff::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::DiffusiveSurfaceRunoff::exchange_target "void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")
cmf::upslope::connections::DiffusiveSurfaceRunoff::get_ptr "ptr
get_ptr() const ";

%feature("docstring")
cmf::upslope::connections::DiffusiveSurfaceRunoff::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::DiffusiveSurfaceRunoff::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::DiffusiveSurfaceRunoff::get_tracer_filter "real get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::DiffusiveSurfaceRunoff::get_tracer_filter "real get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")
cmf::upslope::connections::DiffusiveSurfaceRunoff::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::DiffusiveSurfaceRunoff::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::connections::DiffusiveSurfaceRunoff::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::DiffusiveSurfaceRunoff::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::upslope::connections::DiffusiveSurfaceRunoff::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::DiffusiveSurfaceRunoff::set_tracer_filter "void set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::DiffusiveSurfaceRunoff::set_tracer_filter "void set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::connections::DiffusiveSurfaceRunoff::short_string "virtual std::string short_string() const ";

%feature("docstring")
cmf::upslope::connections::DiffusiveSurfaceRunoff::to_string "virtual
std::string to_string() const ";


// File: classcmf_1_1water_1_1_dirichlet_boundary.xml
%feature("docstring") cmf::water::DirichletBoundary "

Dirichlet (constant head) boundary condition.

This boundary condition can be used either as a pure sink boundary
condition or as a conditional source / sink boundary condition. The
constant head of the boundary condition is interpreted and handled by
the connections of the boundary condition. Not head aware connections,
should not be used, since they are ignoring the constant head.

C++ includes: boundary_condition.h ";

%feature("docstring")
cmf::water::DirichletBoundary::DirichletBoundary "DirichletBoundary(cmf::project &_p, real potential,
cmf::geometry::point Location=cmf::geometry::point()) ";

%feature("docstring")  cmf::water::DirichletBoundary::conc "virtual
real conc(cmf::math::Time t, const cmf::water::solute &_Solute) const

Returns the water quality of the flux_node, if it is not overridden
this is the mix of the incoming fluxes. ";

%feature("docstring")  cmf::water::DirichletBoundary::connection_to "cmf::water::flux_connection* connection_to(const cmf::water::flux_node
&target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::water::DirichletBoundary::flux3d_to "cmf::geometry::point flux3d_to(const cmf::water::flux_node &target,
cmf::math::Time t) ";

%feature("docstring")  cmf::water::DirichletBoundary::flux_to "real
flux_to(const cmf::water::flux_node &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from this into target\") ";

%feature("docstring")  cmf::water::DirichletBoundary::get_3d_flux "cmf::geometry::point get_3d_flux(cmf::math::Time t)

Returns the sum of all flux vectors. ";

%feature("docstring")  cmf::water::DirichletBoundary::get_connections
"cmf::water::connection_list get_connections() const ";

%feature("docstring")  cmf::water::DirichletBoundary::get_potential "real get_potential(cmf::math::Time t=cmf::math::never) const override

Returns the water potential of the node in m waterhead.

The base class water storage always returns the height of the location
";

%feature("docstring")  cmf::water::DirichletBoundary::get_project "cmf::project& get_project() const

Returns the project, this node is part of. ";

%feature("docstring")  cmf::water::DirichletBoundary::is_empty "double is_empty() const

Returns true if the node has no water. ";

%feature("docstring")  cmf::water::DirichletBoundary::is_storage "virtual bool is_storage() const

true, if this is a waterstorage ";

%feature("docstring")  cmf::water::DirichletBoundary::RecalcFluxes "bool RecalcFluxes(cmf::math::Time t) const ";

%feature("docstring")  cmf::water::DirichletBoundary::RecalcFluxes "virtual bool RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node.

WaterStorage overrides this, since state changes require an update of
the fluxes ";

%feature("docstring")
cmf::water::DirichletBoundary::remove_connection "bool
remove_connection(cmf::water::flux_node::ptr To)

Remove the connection. ";

%feature("docstring")  cmf::water::DirichletBoundary::set_conc "virtual void set_conc(const cmf::water::solute &_Solute, double value)
";

%feature("docstring")
cmf::water::DirichletBoundary::set_dynamic_potential "void
set_dynamic_potential(cmf::math::timeseries ts)

Sets the potential of the boundary condition as a timeseries. ";

%feature("docstring")  cmf::water::DirichletBoundary::set_potential "void set_potential(real new_potential) override

Sets the potential of this flux node. ";

%feature("docstring")  cmf::water::DirichletBoundary::to_string "virtual std::string to_string() const ";

%feature("docstring")  cmf::water::DirichletBoundary::waterbalance "real waterbalance(cmf::math::Time t, const flux_connection *Without=0)
const

Returns the sum of all fluxes (positive and negative) at time t.

Single fluxes can be excluded from the calculation

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the waterbalance
(e.g. to prevent closed circuits) ";


// File: classcmf_1_1upslope_1_1connections_1_1_energy_budget_snow_melt.xml
%feature("docstring") cmf::upslope::connections::EnergyBudgetSnowMelt
"

Calculates snow melt using the surface energy budget method.

C++ includes: surfacefluxes.h ";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::EnergyBudgetSnowMelt
"EnergyBudgetSnowMelt(cmf::water::WaterStorage::ptr snow,
cmf::water::flux_node::ptr surface_water, cmf::upslope::Cell &cell) ";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::exchange_target "void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::get_ptr "ptr
get_ptr() const ";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::get_tracer_filter "real get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::get_tracer_filter "real get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::set_tracer_filter "void set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::set_tracer_filter "void set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::short_string "virtual std::string short_string() const ";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::to_string "virtual
std::string to_string() const ";


// File: classcmf_1_1math_1_1_explicit_euler__fixed.xml
%feature("docstring") cmf::math::ExplicitEuler_fixed "

An explicit Euler integrator, with a fixed time step.

C++ includes: explicit_euler.h ";

/*  Accuracy parameters  */

/*  model time  */

%feature("docstring")  cmf::math::ExplicitEuler_fixed::get_t "cmf::math::Time get_t() const

Returns the current model time. ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::set_t "void
set_t(cmf::math::Time val)

Sets the current model time. ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::get_dt "cmf::math::Time get_dt() const

Returns the last time step. ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::reset "virtual
void reset()

Resets any saved history (for multistep methods) ";

/*  Integrate  */

%feature("docstring")  cmf::math::ExplicitEuler_fixed::integrate_until
"void integrate_until(cmf::math::Time t_max, cmf::math::Time
dt=Time(), bool reset=false)

Integrates the vector of state variables until t_max.

Parameters:
-----------

t_max:   Time, the solver should run to

dt:   Time step (may be omitted)

reset:  If true, solver is reseted before integration starts ";

%feature("docstring")
cmf::math::ExplicitEuler_fixed::ExplicitEuler_fixed "ExplicitEuler_fixed(const cmf::math::state_list &states)

Constructs a new ExplicitEuler_fixed from a pointer to a vector of
state variables.

The Integrator becomes the owner of states

Parameters:
-----------

states:  Statevariables of the system ";

%feature("docstring")
cmf::math::ExplicitEuler_fixed::ExplicitEuler_fixed "ExplicitEuler_fixed()

Constructs a new ExplicitEuler_fixed. ";

%feature("docstring")
cmf::math::ExplicitEuler_fixed::ExplicitEuler_fixed "ExplicitEuler_fixed(const Integrator &copy)

copy constructor ";

%feature("docstring")
cmf::math::ExplicitEuler_fixed::~ExplicitEuler_fixed "virtual
~ExplicitEuler_fixed() ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::copy "virtual
Integrator* copy() const

Polymorphic copy constructor. ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::integrate "int
integrate(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables.

Parameters:
-----------

MaxTime:  (ignored) To stop the model (if running in a model
framework) at time steps of value exchange e.g. full hours, the next
value exchange time can be given

TimeStep:  Takes the proposed time step ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::set_system "void set_system(const state_list &states) ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::size "size_t
size() const ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::to_string "std::string to_string() const override

Desrcibes the iterator. ";


// File: classcmf_1_1water_1_1_exponential_decline_connection.xml
%feature("docstring") cmf::water::ExponentialDeclineConnection "

A conceptual TOPmodel inspired connection.



.. math::

     q = Q_0 \\\\cdot e^{(V-V_0)/m} 

C++ includes: simple_connections.h ";

%feature("docstring")
cmf::water::ExponentialDeclineConnection::ExponentialDeclineConnection
"ExponentialDeclineConnection(WaterStorage::ptr source,
flux_node::ptr target, real Q0, real V0, real m)

creates the exponential decline connection ";

%feature("docstring")  cmf::water::ExponentialDeclineConnection::conc
"real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::water::ExponentialDeclineConnection::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")
cmf::water::ExponentialDeclineConnection::get_ptr "ptr get_ptr()
const ";

%feature("docstring")
cmf::water::ExponentialDeclineConnection::get_target "flux_node::ptr
get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::water::ExponentialDeclineConnection::get_target "flux_node::ptr
get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::water::ExponentialDeclineConnection::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::water::ExponentialDeclineConnection::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")
cmf::water::ExponentialDeclineConnection::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::water::ExponentialDeclineConnection::left_node "flux_node::ptr
left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::water::ExponentialDeclineConnection::q "real q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::water::ExponentialDeclineConnection::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::water::ExponentialDeclineConnection::right_node "flux_node::ptr
right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::water::ExponentialDeclineConnection::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::water::ExponentialDeclineConnection::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::water::ExponentialDeclineConnection::short_string "virtual
std::string short_string() const ";

%feature("docstring")
cmf::water::ExponentialDeclineConnection::to_string "virtual
std::string to_string() const ";


// File: classcmf_1_1water_1_1_externally_controlled_flux.xml
%feature("docstring") cmf::water::ExternallyControlledFlux "

Flux from one node to another, controlled by the user or an external
program, by changing the flux constant.

It is easy to create negative volumes in water storages with this
connection, which can be hazard to the solver, since most connections
rely on a positive volume in a storage. Handle with care!

C++ includes: simple_connections.h ";

%feature("docstring")
cmf::water::ExternallyControlledFlux::ExternallyControlledFlux "ExternallyControlledFlux(flux_node::ptr source, flux_node::ptr target,
real flux_value=0) ";

%feature("docstring")  cmf::water::ExternallyControlledFlux::conc "real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::water::ExternallyControlledFlux::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::water::ExternallyControlledFlux::get_ptr "ptr get_ptr() const ";

%feature("docstring")
cmf::water::ExternallyControlledFlux::get_target "flux_node::ptr
get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::water::ExternallyControlledFlux::get_target "flux_node::ptr
get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::water::ExternallyControlledFlux::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::water::ExternallyControlledFlux::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::water::ExternallyControlledFlux::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::water::ExternallyControlledFlux::left_node
"flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::water::ExternallyControlledFlux::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::water::ExternallyControlledFlux::refresh "void refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::water::ExternallyControlledFlux::right_node "flux_node::ptr
right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::water::ExternallyControlledFlux::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::water::ExternallyControlledFlux::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::water::ExternallyControlledFlux::short_string "virtual
std::string short_string() const ";

%feature("docstring")  cmf::water::ExternallyControlledFlux::to_string
"virtual std::string to_string() const ";


// File: classcmf_1_1fit__retention__curve_1_1_fit_brooks_corey.xml
%feature("docstring") cmf::fit_retention_curve::FitBrooksCorey "";

%feature("docstring")
cmf::fit_retention_curve::FitBrooksCorey::__init__ "def
__init__(self, theta, pF, verbose=False) ";

%feature("docstring")
cmf::fit_retention_curve::FitBrooksCorey::__call__ "def
__call__(self, count=1) ";

%feature("docstring")
cmf::fit_retention_curve::FitBrooksCorey::create_rc "def
create_rc(self, params) ";

%feature("docstring")
cmf::fit_retention_curve::FitBrooksCorey::get_error "def
get_error(self, params) ";


// File: classcmf_1_1fit__retention__curve_1_1_fit_retention_curve.xml
%feature("docstring") cmf::fit_retention_curve::FitRetentionCurve "";

%feature("docstring")
cmf::fit_retention_curve::FitRetentionCurve::__init__ "def
__init__(self, rc_class, theta, pF, verbose=False) ";

%feature("docstring")
cmf::fit_retention_curve::FitRetentionCurve::__call__ "def
__call__(self, count=1) ";

%feature("docstring")
cmf::fit_retention_curve::FitRetentionCurve::create_rc "def
create_rc(self, params) ";

%feature("docstring")
cmf::fit_retention_curve::FitRetentionCurve::get_error "def
get_error(self, params) ";


// File: classcmf_1_1fit__retention__curve_1_1_fit_van_genuchten_mualem.xml
%feature("docstring") cmf::fit_retention_curve::FitVanGenuchtenMualem
"";

%feature("docstring")
cmf::fit_retention_curve::FitVanGenuchtenMualem::__init__ "def
__init__(self, theta, pF, fit_theta_r=False, fit_m=False,
verbose=False) ";

%feature("docstring")
cmf::fit_retention_curve::FitVanGenuchtenMualem::__call__ "def
__call__(self, count=1) ";

%feature("docstring")
cmf::fit_retention_curve::FitVanGenuchtenMualem::create_rc "def
create_rc(self, params) ";

%feature("docstring")
cmf::fit_retention_curve::FitVanGenuchtenMualem::get_error "def
get_error(self, params) ";


// File: classcmf_1_1water_1_1flux__connection.xml
%feature("docstring") cmf::water::flux_connection "

The connections in cmf hold the processes for the calculation of
fluxes between water storages and model boundaries.

Todo Elaborate on this Represents a connection between flux_nodes,
where water fluxes occur.

C++ includes: flux_connection.h ";

%feature("docstring")  cmf::water::flux_connection::flux_connection "flux_connection(flux_node::ptr left, flux_node::ptr right, std::string
_type)

Creates a flux connection between the flux_node left and right.

Parameters:
-----------

left:   flux_node on the one side of the connection

right:   flux_node on the other side of the connection

_type:  Type of the flux connection ";

%feature("docstring")  cmf::water::flux_connection::~flux_connection "virtual ~flux_connection() ";

%feature("docstring")  cmf::water::flux_connection::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")  cmf::water::flux_connection::exchange_target "void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")  cmf::water::flux_connection::get_ptr "ptr
get_ptr() const ";

%feature("docstring")  cmf::water::flux_connection::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::water::flux_connection::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")  cmf::water::flux_connection::get_tracer_filter
"real get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")  cmf::water::flux_connection::get_tracer_filter
"real get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::water::flux_connection::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::water::flux_connection::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::water::flux_connection::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::water::flux_connection::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")  cmf::water::flux_connection::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::water::flux_connection::set_tracer_filter
"void set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")  cmf::water::flux_connection::set_tracer_filter
"void set_tracer_filter(solute S, real value) ";

%feature("docstring")  cmf::water::flux_connection::short_string "virtual std::string short_string() const ";

%feature("docstring")  cmf::water::flux_connection::to_string "virtual std::string to_string() const ";


// File: classcmf_1_1water_1_1flux__integrator.xml
%feature("docstring") cmf::water::flux_integrator "

The flux_integrator is an integratable for precise output of average
fluxes over time.

It can be added to solver (any cmf::math::Integrator), which is then
calling the integrate method at each substep.

C++ includes: flux_connection.h ";

%feature("docstring")  cmf::water::flux_integrator::flux_integrator "flux_integrator(cmf::water::flux_connection &connection)

Creates a flux_integrator from an connection. ";

%feature("docstring")  cmf::water::flux_integrator::flux_integrator "flux_integrator(cmf::water::flux_node::ptr left,
cmf::water::flux_node::ptr right)

Creates a flux_integrator from the endpoints of a connection. Throws
if there is no connection between the endpoints. ";

%feature("docstring")  cmf::water::flux_integrator::avg "double avg()
const

Returns the average flux of the integration time in m3/day. ";

%feature("docstring")  cmf::water::flux_integrator::connection "flux_connection::ptr connection() const

Returns the flux_connection. ";

%feature("docstring")  cmf::water::flux_integrator::integrate "void
integrate(cmf::math::Time until)

Integrates the flux a timestep further. Note: until is an absolute
time. If until is before t0, the integration is initilized again. ";

%feature("docstring")  cmf::water::flux_integrator::integration_t "cmf::math::Time integration_t() const

Returns the duration of the integration. ";

%feature("docstring")  cmf::water::flux_integrator::reset "void
reset(cmf::math::Time t)

Initializes the integration. ";

%feature("docstring")  cmf::water::flux_integrator::sum "double sum()
const

Returns the amount of water along this connection in the integration
time in m3. ";

%feature("docstring")  cmf::water::flux_integrator::t0 "cmf::math::Time t0() const

Returns the start time of the integration. ";


// File: classcmf_1_1water_1_1flux__node.xml
%feature("docstring") cmf::water::flux_node "

Base class for everything that can be connected by fluxes.

Flux nodes can be WaterStorages, flux end points, sinks, sources and
bridges to other model domains (e.g. Ponded water to river system).
The base class can be used where a simple routing, potentially with
mixing, is needed.

C++ includes: flux_node.h ";

%feature("docstring")  cmf::water::flux_node::flux_node "flux_node(cmf::project &_project, cmf::geometry::point
location=cmf::geometry::point()) ";

%feature("docstring")  cmf::water::flux_node::~flux_node "virtual
~flux_node()

The destructor deletes all connections. ";

%feature("docstring")  cmf::water::flux_node::conc "virtual real
conc(cmf::math::Time t, const cmf::water::solute &Solute) const

Returns the water quality of the flux_node, if it is not overridden
this is the mix of the incoming fluxes. ";

%feature("docstring")  cmf::water::flux_node::connection_to "cmf::water::flux_connection* connection_to(const cmf::water::flux_node
&target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::water::flux_node::flux3d_to "cmf::geometry::point flux3d_to(const cmf::water::flux_node &target,
cmf::math::Time t) ";

%feature("docstring")  cmf::water::flux_node::flux_to "real
flux_to(const cmf::water::flux_node &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from this into target\") ";

%feature("docstring")  cmf::water::flux_node::get_3d_flux "cmf::geometry::point get_3d_flux(cmf::math::Time t)

Returns the sum of all flux vectors. ";

%feature("docstring")  cmf::water::flux_node::get_connections "cmf::water::connection_list get_connections() const ";

%feature("docstring")  cmf::water::flux_node::get_potential "virtual
real get_potential(cmf::math::Time=cmf::math::never) const

Returns the water potential of the node in m waterhead.

The base class water storage always returns the height of the location
";

%feature("docstring")  cmf::water::flux_node::get_project "cmf::project& get_project() const

Returns the project, this node is part of. ";

%feature("docstring")  cmf::water::flux_node::is_empty "virtual
double is_empty() const

Returns true if the node has no water. ";

%feature("docstring")  cmf::water::flux_node::is_storage "virtual
bool is_storage() const

true, if this is a waterstorage ";

%feature("docstring")  cmf::water::flux_node::RecalcFluxes "virtual
bool RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node.

WaterStorage overrides this, since state changes require an update of
the fluxes ";

%feature("docstring")  cmf::water::flux_node::remove_connection "bool
remove_connection(cmf::water::flux_node::ptr To)

Remove the connection. ";

%feature("docstring")  cmf::water::flux_node::set_potential "virtual
void set_potential(real new_potential)

Sets the potential of this flux node. ";

%feature("docstring")  cmf::water::flux_node::to_string "virtual
std::string to_string() const ";

%feature("docstring")  cmf::water::flux_node::waterbalance "real
waterbalance(cmf::math::Time t, const flux_connection *Without=0)
const

Returns the sum of all fluxes (positive and negative) at time t.

Single fluxes can be excluded from the calculation

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the waterbalance
(e.g. to prevent closed circuits) ";


// File: classcmf_1_1draw_1_1cellmap_1_1_flux_map.xml
%feature("docstring") cmf::draw::cellmap::FluxMap "

Draws for each cell an arrow indicating the direction and velocity of
flow through the cell (in horizontal direction). Uses plt.quiver  The
FluxMap can be updated by calling it with the new timestep  Usage: >>>
import cmf >>> p = cmf.project() >>> solver = cmf.RKFIntegrator(p,
1e-9) >>> fm = FluxMap(p, cmf.Time()) >>> for t in
solver.run(solver.t, solver.t + cmf.day * 30, cmf.h): ...     fm(t) ";

%feature("docstring")  cmf::draw::cellmap::FluxMap::__init__ "def
__init__(self, cells, t, kwargs)

Creates a new flux map :param cells: The cells to be used :param t:
The current time step :param kwargs: Keyword arguments for plt.quiver
";

%feature("docstring")  cmf::draw::cellmap::FluxMap::__call__ "def
__call__(self, t=None) ";


// File: classcmf_1_1upslope_1_1connections_1_1_free_drainage_percolation.xml
%feature("docstring")
cmf::upslope::connections::FreeDrainagePercolation "

Calculates a free drainage (unit gradient) from a layer to somewhere
else.



.. math::

     q = K(\\\\theta) A

where:  :math:`q` Flux from the layer to the
other side of the connection in :math:`m^3/day`

:math:`K(\\\\theta)` Actual conductivity in :math:`m/day` depending on the water
content of the layer :math:`\\\\theta`

:math:`A` Cell area in :math:`m^2`

C++ includes: Percolation.h ";

%feature("docstring")
cmf::upslope::connections::FreeDrainagePercolation::FreeDrainagePercolation
"FreeDrainagePercolation(cmf::upslope::SoilLayer::ptr left,
cmf::water::flux_node::ptr right) ";

%feature("docstring")
cmf::upslope::connections::FreeDrainagePercolation::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::FreeDrainagePercolation::exchange_target "void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")
cmf::upslope::connections::FreeDrainagePercolation::get_ptr "ptr
get_ptr() const ";

%feature("docstring")
cmf::upslope::connections::FreeDrainagePercolation::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::FreeDrainagePercolation::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::FreeDrainagePercolation::get_tracer_filter
"real get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::FreeDrainagePercolation::get_tracer_filter
"real get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")
cmf::upslope::connections::FreeDrainagePercolation::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::FreeDrainagePercolation::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::connections::FreeDrainagePercolation::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::FreeDrainagePercolation::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::upslope::connections::FreeDrainagePercolation::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::FreeDrainagePercolation::set_tracer_filter
"void set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::FreeDrainagePercolation::set_tracer_filter
"void set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::connections::FreeDrainagePercolation::short_string "virtual std::string short_string() const ";

%feature("docstring")
cmf::upslope::connections::FreeDrainagePercolation::to_string "virtual std::string to_string() const ";


// File: classcmf_1_1water_1_1_freundlich_adsorbtion.xml
%feature("docstring") cmf::water::FreundlichAdsorbtion "

BROKEN: This class calculates the adsorption equilibrium between
sorbat and sorbent using the Freundlich isotherme.

Freundlich isotherme:



.. math::

    \\\\frac{x_{ad}}{m} = K c^n

where  :math:`x_{ad} = x_{tot} - x_{free}` is the adsorbed tracer mass  :math:`x_{tot}` is the total tracer
mass

:math:`x_{free}` is the dissolved tracer mass

:math:`m` is the mass of the sorbent in the same unit as the tracer mass

:math:`K` is the Freundlich sorption coefficient

:math:`c = \\\\frac{x_{free}}{V}` is the concentration of the tracer in
tracer mass per m3

:math:`n` is the Freundlich exponent

CMF stores in a solute storage the total mass of a tracer and needs to
calculate the free tracer mass. The eq. above can not be rearanged to
get :math:`x_{free}` from :math:`x_{tot}`. Instead, the value is iterated
usingregula falsi. If n is near to 1, using LinearAdsorption will
speed up your calculations.

The simplest physically based adsorption model by Langmuir (
LangmuirAdsorption) has also a analytical solution and is hence
calculated faster then Freundlich.

C++ includes: adsorption.h ";

%feature("docstring")
cmf::water::FreundlichAdsorbtion::FreundlichAdsorbtion "FreundlichAdsorbtion(real K, real n, real m, real epsilon=1e-12, int
maxiter=100)

Parameters:
-----------

K:  n:  Freundlich coefficents

m:  Mass of sorbent in units of tracer

epsilon:  Tolerance of regula falsi iteration for the calculation of
dissolved tracer from total trace, default = 1e-12

maxiter:  Maximum number of iterations, default = 100 ";

%feature("docstring")
cmf::water::FreundlichAdsorbtion::FreundlichAdsorbtion "FreundlichAdsorbtion(const FreundlichAdsorbtion &other) ";

%feature("docstring")
cmf::water::FreundlichAdsorbtion::~FreundlichAdsorbtion "virtual
~FreundlichAdsorbtion() ";

%feature("docstring")  cmf::water::FreundlichAdsorbtion::copy "FreundlichAdsorbtion* copy(real m=-1) const

returns a copy of the Adsorption object.

If the adsorption is depending on the sorbent mass, you can give a
positive value for the sorbent mass m. If the value is not given or
negative, m is used from the original object. ";

%feature("docstring")  cmf::water::FreundlichAdsorbtion::freesolute "real freesolute(real xt, real V) const

Returns the mass of dissolved tracer as a function of the total tracer
mass in the solute storage and the water volume.

Parameters:
-----------

xt:   :math:`x_t` the total tracer mass in the storage

V:   :math:`V m^3` the water volume in the storage

:math:`x_f` the dissolved mass of the tracer ";

%feature("docstring")  cmf::water::FreundlichAdsorbtion::totalsolute "real totalsolute(real xf, real V) const

Returns the total mass of the tracer from the dissolved concetration
in tracer unit/m3.

Parameters:
-----------

xf:   :math:`x_f` the dissolved tracer mass in the storage

V:   :math:`V m^3` the water volume in the storage

:math:`x_t` the total mass of the tracer ";


// File: classcmf_1_1upslope_1_1connections_1_1_gradient_macro_flow.xml
%feature("docstring") cmf::upslope::connections::GradientMacroFlow "

Gradient based flux from macro pore to macro pore.

Deprecated The MacroPore model is still very experimental and not
stable. Only for tryouts!



.. math::

     q = K(\\\\theta) \\\\frac{\\\\Delta \\\\Psi}{\\\\Delta z} 

C++ includes: macropore.h ";

%feature("docstring")
cmf::upslope::connections::GradientMacroFlow::GradientMacroFlow "GradientMacroFlow(cmf::upslope::MacroPore::ptr left,
cmf::water::flux_node::ptr right) ";

%feature("docstring")
cmf::upslope::connections::GradientMacroFlow::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::GradientMacroFlow::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")
cmf::upslope::connections::GradientMacroFlow::get_ptr "ptr get_ptr()
const ";

%feature("docstring")
cmf::upslope::connections::GradientMacroFlow::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::GradientMacroFlow::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::GradientMacroFlow::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::GradientMacroFlow::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")
cmf::upslope::connections::GradientMacroFlow::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::GradientMacroFlow::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::connections::GradientMacroFlow::q
"real q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::GradientMacroFlow::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::upslope::connections::GradientMacroFlow::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::GradientMacroFlow::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::GradientMacroFlow::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::connections::GradientMacroFlow::short_string "virtual
std::string short_string() const ";

%feature("docstring")
cmf::upslope::connections::GradientMacroFlow::to_string "virtual
std::string to_string() const ";


// File: classcmf_1_1upslope_1_1connections_1_1_gradient_macro_micro_exchange.xml
%feature("docstring")
cmf::upslope::connections::GradientMacroMicroExchange "

A gradient based exchange term between macropores and micropores,
using a fixed (air-) potential for macropores.



.. math::

    q = K \\\\frac{\\\\Delta\\\\Psi}{d/2} A 

where:  :math:`K` The
conductivity of the aggregate boundary

:math:`\\\\Delta\\\\Psi` The potential difference. Using the air potential
as the constant potential for the macro pores, you get:
:math:`\\\\Delta\\\\Psi = \\\\Psi_M(\\\\theta_{micro})`

:math:`d` the mean aggregate size in m

:math:`A` the crosssection area, given as the flow width (
cmf::upslope::MacroPore::get_flowwidth) times layer thickness

C++ includes: macropore.h ";

%feature("docstring")
cmf::upslope::connections::GradientMacroMicroExchange::GradientMacroMicroExchange
"GradientMacroMicroExchange(cmf::upslope::SoilLayer::ptr left,
cmf::upslope::MacroPore::ptr right) ";

%feature("docstring")
cmf::upslope::connections::GradientMacroMicroExchange::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::GradientMacroMicroExchange::exchange_target
"void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")
cmf::upslope::connections::GradientMacroMicroExchange::get_ptr "ptr
get_ptr() const ";

%feature("docstring")
cmf::upslope::connections::GradientMacroMicroExchange::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::GradientMacroMicroExchange::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::GradientMacroMicroExchange::get_tracer_filter
"real get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::GradientMacroMicroExchange::get_tracer_filter
"real get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")
cmf::upslope::connections::GradientMacroMicroExchange::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::GradientMacroMicroExchange::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::connections::GradientMacroMicroExchange::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::GradientMacroMicroExchange::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::upslope::connections::GradientMacroMicroExchange::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::GradientMacroMicroExchange::set_tracer_filter
"void set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::GradientMacroMicroExchange::set_tracer_filter
"void set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::connections::GradientMacroMicroExchange::short_string "virtual std::string short_string() const ";

%feature("docstring")
cmf::upslope::connections::GradientMacroMicroExchange::to_string "virtual std::string to_string() const ";


// File: classcmf_1_1upslope_1_1connections_1_1_green_ampt_infiltration.xml
%feature("docstring") cmf::upslope::connections::GreenAmptInfiltration
"

Connects the surfacewater and the most upper layer using a Green-Ampt
equation like infiltration.

The Green-Ampt formula is given as: 

.. math::

    q(t) = -K_s \\\\frac{dh}{dz}
    A

where:  :math:`q(t)` is the infiltration rate in m3/day

:math:`K_s` is the saturated conductivity in m/day

:math:`\\\\frac{dh}{dz}` is the hydraulic gradient in the wetting front

:math:`A` is the surface area of the cell

The gradient in the wetting front is calculated as:


.. math::

    \\\\frac{dh}{dz} = \\\\frac{h_f - h_0}{Z_f} =
    \\\\frac{|\\\\Psi_f| + Z_f}{Z_f}

where:  :math:`h_f` is the hydraulic
head at the bottom of the wetting front in m

:math:`h_0` is the hydraulic head at the surface in m

:math:`Z_f` is the length of the wetting front in m

Since :math:`Z_f` is unknown, the depth of the wetting front can be
approximated by: 

.. math::

    Z_f = \\\\frac{F}{\\\\theta_s -
    \\\\theta_i}

with:  :math:`F` the accumulated volume per area of
infiltrated water

:math:`\\\\theta_s, \\\\theta_i` the volumetric water content at saturation
resp. at start of the infiltration

If the surface water is modeled by a distinct water storage, the
actual infiltration is given as the product of the potential
infiltration with the coverage of the surface water
cmf::upslope::Cell::surface_water_coverage 

.. math::

    q_{act} = q_{max}
    \\\\frac{A_{water}}{A_{cell}}

If the surface water is no storage on its own, but just a water
distribution node, the actual infiltration is the minimum of the
potential infiltration and the current inflow (rain, snow melt) to the
surface 

.. math::

    q_{act} = \\\\min\\\\left(q_{max},
    \\\\sum{q_{in,surfacewater}}\\\\right)

C++ includes: infiltration.h ";

%feature("docstring")
cmf::upslope::connections::GreenAmptInfiltration::GreenAmptInfiltration
"GreenAmptInfiltration(cmf::upslope::SoilLayer::ptr soilwater,
cmf::water::flux_node::ptr surfacewater) ";

%feature("docstring")
cmf::upslope::connections::GreenAmptInfiltration::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::GreenAmptInfiltration::exchange_target "void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")
cmf::upslope::connections::GreenAmptInfiltration::get_ptr "ptr
get_ptr() const ";

%feature("docstring")
cmf::upslope::connections::GreenAmptInfiltration::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::GreenAmptInfiltration::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::GreenAmptInfiltration::get_tracer_filter "real get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::GreenAmptInfiltration::get_tracer_filter "real get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")
cmf::upslope::connections::GreenAmptInfiltration::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::GreenAmptInfiltration::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::connections::GreenAmptInfiltration::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::GreenAmptInfiltration::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::upslope::connections::GreenAmptInfiltration::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::GreenAmptInfiltration::set_tracer_filter "void set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::GreenAmptInfiltration::set_tracer_filter "void set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::connections::GreenAmptInfiltration::short_string "virtual std::string short_string() const ";

%feature("docstring")
cmf::upslope::connections::GreenAmptInfiltration::to_string "virtual
std::string to_string() const ";


// File: classcmf_1_1upslope_1_1_e_t_1_1_hargreave_e_t.xml
%feature("docstring") cmf::upslope::ET::HargreaveET "

Calculates the Evapotranspiration using Hargreave's equation.



.. math::

    ET_{rc} = 0.0135 K_T\\\\ s_0 \\\\sqrt{\\\\Delta T} (T +
    17.8)

where:  :math:`ET_{rc}` the reference crop evapotranspiration
in mm/day

:math:`K_T = 0.00185{\\\\Delta T}^2 - 0.0433 \\\\Delta T + 0.4023`
Continentality factor as given in the reference

:math:`\\\\Delta T = |T_{max} - T_{min}|[K]` Daily temperature range

:math:`T [^\\\\circ C]` daily mean temperature

:math:`s_0 = 15.392 d_r \\\\left(\\\\omega_s\\\\sin(\\\\Phi) \\\\sin{\\\\gamma} + \\\\cos{\\\\Phi}\\\\cos{\\\\gamma} * \\\\sin(\\\\omega_s)\\\\right)` the extraterrestrial solar radiation
in mm/day

:math:`d_r = 1 + 0.0033 \\\\cos(DOY\\\\frac{2 \\\\pi}{365})` relative
distance between earth and sun

:math:`\\\\omega_s = \\\\arccos(-\\\\tan{\\\\Phi} \\\\tan{\\\\gamma})`
sunset hour angle (radians)

:math:`\\\\gamma = 0.4039 \\\\sin(DOY\\\\frac{2 \\\\pi}{365} - 1.405)`
solar declination (radians)

:math:`\\\\Phi` geographic latitude (radians)

See:  SAMANI, Zohrab.Estimating solar radiation and evapotranspiration
using minimum climatological data. Journal of Irrigation and Drainage
Engineering, 2000, 126. Jg., Nr. 4, S. 265-267.  Crop specific
potential evapotranspiration is scaled by LAI: :math:`ET_{pot} = ET_{rc} \\\\frac{LAI}{2.88}`.

C++ includes: ET.h ";

%feature("docstring")  cmf::upslope::ET::HargreaveET::HargreaveET "HargreaveET(cmf::upslope::SoilLayer::ptr source,
cmf::water::flux_node::ptr ET_target) ";

%feature("docstring")  cmf::upslope::ET::HargreaveET::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")  cmf::upslope::ET::HargreaveET::ETpot "real
ETpot(cmf::math::Time t) const ";

%feature("docstring")  cmf::upslope::ET::HargreaveET::exchange_target
"void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")  cmf::upslope::ET::HargreaveET::get_layer "SoilLayer::ptr get_layer() const ";

%feature("docstring")  cmf::upslope::ET::HargreaveET::get_ptr "ptr
get_ptr() const ";

%feature("docstring")  cmf::upslope::ET::HargreaveET::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::ET::HargreaveET::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::ET::HargreaveET::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::ET::HargreaveET::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::upslope::ET::HargreaveET::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::upslope::ET::HargreaveET::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::ET::HargreaveET::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::ET::HargreaveET::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")  cmf::upslope::ET::HargreaveET::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::ET::HargreaveET::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::ET::HargreaveET::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")  cmf::upslope::ET::HargreaveET::short_string "virtual std::string short_string() const ";

%feature("docstring")  cmf::upslope::ET::HargreaveET::to_string "virtual std::string to_string() const ";


// File: classcmf_1_1math_1_1_heun_integrator.xml
%feature("docstring") cmf::math::HeunIntegrator "

A simple predictor - corrector solver.

Not tested and very experimentally :math:`y^{n+1} = y^n + \\\\alpha f(y^n + f(y^n)dt)dt + (1-\\\\alpha)f(y^n)dt`

C++ includes: explicit_euler.h ";

/*  Accuracy parameters  */

/*  model time  */

%feature("docstring")  cmf::math::HeunIntegrator::get_t "cmf::math::Time get_t() const

Returns the current model time. ";

%feature("docstring")  cmf::math::HeunIntegrator::set_t "void
set_t(cmf::math::Time val)

Sets the current model time. ";

%feature("docstring")  cmf::math::HeunIntegrator::get_dt "cmf::math::Time get_dt() const

Returns the last time step. ";

/*  Integrate  */

%feature("docstring")  cmf::math::HeunIntegrator::integrate_until "void integrate_until(cmf::math::Time t_max, cmf::math::Time dt=Time(),
bool reset=false)

Integrates the vector of state variables until t_max.

Parameters:
-----------

t_max:   Time, the solver should run to

dt:   Time step (may be omitted)

reset:  If true, solver is reseted before integration starts ";

%feature("docstring")  cmf::math::HeunIntegrator::HeunIntegrator "HeunIntegrator(const cmf::math::state_list &states, real Alpha=0.5)

Constructs a new PredictCorrectSimple from a pointer to a vector of
state variables.

The Integrator becomes the owner of states

Parameters:
-----------

states:  Statevariable owner of the system

Alpha:  Weight factor :math:`\\\\alpha` to weight :math:`f(y^n)` and :math:`f(y^{n+1})`
";

%feature("docstring")  cmf::math::HeunIntegrator::HeunIntegrator "HeunIntegrator(real Alpha=0.5)

Constructs a new Heun integrator.

Parameters:
-----------

Alpha:  Weight factor :math:`\\\\alpha` to weight :math:`f(y^n)` and :math:`f(y^{n+1})`
";

%feature("docstring")  cmf::math::HeunIntegrator::HeunIntegrator "HeunIntegrator(const Integrator &copy)

copy constructor ";

%feature("docstring")  cmf::math::HeunIntegrator::~HeunIntegrator "virtual ~HeunIntegrator() ";

%feature("docstring")  cmf::math::HeunIntegrator::copy "virtual
Integrator* copy() const

Polymorphic copy constructor. ";

%feature("docstring")  cmf::math::HeunIntegrator::integrate "int
integrate(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables.

Parameters:
-----------

MaxTime:  (ignored) To stop the model (if running in a model
framework) at time steps of value exchange e.g. full hours, the next
value exchange time can be given

TimeStep:  Takes the proposed time step ";

%feature("docstring")  cmf::math::HeunIntegrator::reset "void reset()
override

Resets any saved history (for multistep methods) ";

%feature("docstring")  cmf::math::HeunIntegrator::set_system "void
set_system(const state_list &states) ";

%feature("docstring")  cmf::math::HeunIntegrator::size "size_t size()
const ";

%feature("docstring")  cmf::math::HeunIntegrator::to_string "std::string to_string() const override

Desrcibes the iterator. ";


// File: classcmf_1_1draw_1_1hillplot_1_1_hill_plot.xml
%feature("docstring") cmf::draw::hillplot::HillPlot "

Plots a hillslope using colored sheared rectangles for each layer and
arrows (matplotlib.quiver) to show fluxes.  Properties:   -
evalfunction: a function returning the value of a layer to plot. The
value should be a float between 0..1 for scaling.     The default is:
lambda l: l.wetness  - q_sub:  The matplotlib.Quiver object for
subsurface fluxes  - q_surf: The matplotlib.Quiver object for
subsurface fluxes  - title:  Title of the plot  - scale:  The scale of
q_sub and q_surf ";

%feature("docstring")  cmf::draw::hillplot::HillPlot::__init__ "def
__init__(self, cells, t, solute=None, cmap=default_color_map)

Creates a new HillPlot on the active figure, showing the state of each
layer  - cells: The a sequence of cmf cells to use in this hill_plot.
You can   use the whole project if you like  - t:     Current time
step. Needed to retrieve the fluxes  - solute:The solute concentration
to show. If None, the wetness of the    layer will be shown  - cmap: a
matplotlib colormap (see module cm) for coloring ";

%feature("docstring")  cmf::draw::hillplot::HillPlot::__call__ "def
__call__(self, t, text='')

Updates the hill_plot at time t. You can provide a title for the
figure. If bool(title)==False, t is shown. ";

%feature("docstring")  cmf::draw::hillplot::HillPlot::get_animator "def get_animator(self, integration)

Returns a matplotlib.animation.FuncAnimator object that uses the
integration iteratable to advance your model to animate the HillPlot
Usage example:  >>>p=cmf.project() >>>solver = cmf.CVodeIntegrator(p,
1e-9) >>>hp = HillPlot(p, solver.t) >>>animator =
hp.get_animator(solver.run(datetime(2012, 1, 1), datetime(2012, 2, 1),
timedelta(hours=1)))  :param integration: An iterable that advances
the model and yields the current time :return: A
matplotlib.animation.FuncAnimator ";


// File: classcmf_1_1river_1_1_i_channel.xml
%feature("docstring") cmf::river::IChannel "

Structure for the description of structural parameters of a reach
Abstract base class for different IChannel geometries.

C++ includes: ReachType.h ";

%feature("docstring")  cmf::river::IChannel::~IChannel "virtual
~IChannel() ";

%feature("docstring")  cmf::river::IChannel::A "virtual double
A(double V) const

Returns the area of the surface for a given volume. ";

%feature("docstring")  cmf::river::IChannel::copy "virtual IChannel*
copy() const =0 ";

%feature("docstring")  cmf::river::IChannel::get_channel_width "virtual double get_channel_width(double depth) const =0

Calculates the flow width from a given actual depth [m] using the
actual IChannel geometry. ";

%feature("docstring")  cmf::river::IChannel::get_depth "virtual
double get_depth(double area) const =0

Calculates the actual depth of the reach using the IChannel geometry.

get_depth of the reach [m]

Parameters:
-----------

area:  Wetted area of a river cross section [m2], can be obtained by
V/l, where V is the stored volume and l is the reach length ";

%feature("docstring")  cmf::river::IChannel::get_flux_crossection "virtual double get_flux_crossection(double depth) const =0

Calculates the wetted area from a given depth using the IChannel
geometry.

In most cases use get_flux_crossection=V/l, where V is the stored
volume and l is the reach length Wetted area of a river cross section
[m2]

Parameters:
-----------

depth:  depth of the reach [m] ";

%feature("docstring")  cmf::river::IChannel::get_length "virtual
double get_length() const =0

Length of the reach. ";

%feature("docstring")  cmf::river::IChannel::get_nManning "virtual
double get_nManning() const =0 ";

%feature("docstring")  cmf::river::IChannel::get_wetted_perimeter "virtual double get_wetted_perimeter(double depth) const =0

Calculates the wetted perimeter from a given actual depth [m] using
the actual IChannel geometry. ";

%feature("docstring")  cmf::river::IChannel::h "virtual double
h(double V) const

Returns the depth of a given volume. ";

%feature("docstring")  cmf::river::IChannel::q "virtual double
q(double h, double slope) const ";

%feature("docstring")  cmf::river::IChannel::qManning "virtual double
qManning(double A, double slope) const

Calculates the flow rate from a given water volume in the reach


.. math::

     q_{Manning}&=& A R^{\\\\frac 2 3}
    \\\\sqrt{\\\\frac {\\\\Delta_z} n} \\\\\\\\ A &=& \\\\frac V l
    \\\\mbox{, (Crosssectional area of the wetted crossection, Volume per
    length)} \\\\\\\\ R &=& \\\\frac A {P(d)} \\\\\\\\ P(d) &=& \\\\mbox{
    the perimeter of the wetted crosssection, a function of reach depth}
    \\\\\\\\ d(V) &=& \\\\mbox{ the depth of the reach a function of the
    volume} \\\\\\\\ \\\\Delta_z &=& \\\\frac{z_{max} - z_{min}}{l}
    \\\\mbox{ Slope of the reach} 

.

Flow rate [m3/s]

Parameters:
-----------

A:  The area of the cross section [m2]

slope:  The slope of the reach [m/m] ";

%feature("docstring")  cmf::river::IChannel::set_nManning "virtual
void set_nManning(double val)=0 ";

%feature("docstring")  cmf::river::IChannel::typecode "virtual char
typecode() const =0 ";

%feature("docstring")  cmf::river::IChannel::V "virtual double
V(double h) const ";


// File: classcmf_1_1atmosphere_1_1_i_d_w___meteorology.xml
%feature("docstring") cmf::atmosphere::IDW_Meteorology "

Regionalizes meteorological measurements using a simple inverse
distance weighted (IDW) method.

See:  IDW

C++ includes: meteorology.h ";

%feature("docstring")
cmf::atmosphere::IDW_Meteorology::IDW_Meteorology "IDW_Meteorology(const cmf::geometry::point &position, const
MeteoStationList &stations, double z_weight, double power)

Creates an reference to a list of stations and interpolates the
weather using IDW.

Parameters:
-----------

position:  Position of reference

stations:  Meteo stations

z_weight:  Weight of height in IDW procedure

power:  Power of IDW procedure ";

%feature("docstring")
cmf::atmosphere::IDW_Meteorology::IDW_Meteorology "IDW_Meteorology(const IDW_Meteorology &copy)

Copy c'tor. ";

%feature("docstring")  cmf::atmosphere::IDW_Meteorology::copy "virtual IDW_Meteorology* copy() const

Returns a copy of the meteorology object. Pure virtual function, needs
to be implemented. ";

%feature("docstring")
cmf::atmosphere::IDW_Meteorology::get_instrument_height "virtual real
get_instrument_height() const

Returns the height of the instruments above canopy. ";

%feature("docstring")  cmf::atmosphere::IDW_Meteorology::get_weather "virtual Weather get_weather(cmf::math::Time t) const

Returns the Weather at time t. Pure virtual function. Must get
implemented by child functions. ";


// File: classcmf_1_1atmosphere_1_1_i_d_w_rainfall.xml
%feature("docstring") cmf::atmosphere::IDWRainfall "

A RainSource using a spatially interpolated rainfall intensity from
all stations.

Interpolation method is inverse distance weighted (IDW)

C++ includes: precipitation.h ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::conc "real
conc(cmf::math::Time t, const cmf::water::solute &Solute) const

Returns the concentration of a solute in the rainfall at time t. ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::connection_to "cmf::water::flux_connection* connection_to(const cmf::water::flux_node
&target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::flux3d_to "cmf::geometry::point flux3d_to(const cmf::water::flux_node &target,
cmf::math::Time t) ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::flux_to "real
flux_to(const cmf::water::flux_node &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from this into target\") ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::get_3d_flux "cmf::geometry::point get_3d_flux(cmf::math::Time t)

Returns the sum of all flux vectors. ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::get_connections "cmf::water::connection_list get_connections() const ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::get_intensity "real get_intensity(cmf::math::Time t) const

Returns the actual rainfall intensity in mm/day. ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::get_potential "virtual real get_potential(cmf::math::Time=cmf::math::never) const

Returns the water potential of the node in m waterhead.

The base class water storage always returns the height of the location
";

%feature("docstring")  cmf::atmosphere::IDWRainfall::get_project "cmf::project& get_project() const

Returns the project, this node is part of. ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::is_empty "virtual double is_empty() const

Returns false. ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::is_storage "virtual bool is_storage() const

true, if this is a waterstorage ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::RecalcFluxes "virtual bool RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node.

WaterStorage overrides this, since state changes require an update of
the fluxes ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::remove_connection
"bool remove_connection(cmf::water::flux_node::ptr To)

Remove the connection. ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::set_potential "virtual void set_potential(real new_potential)

Sets the potential of this flux node. ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::to_string "virtual std::string to_string() const ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::waterbalance "real waterbalance(cmf::math::Time t, const flux_connection *Without=0)
const

Returns the sum of all fluxes (positive and negative) at time t.

Single fluxes can be excluded from the calculation

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the waterbalance
(e.g. to prevent closed circuits) ";


// File: classcmf_1_1math_1_1_implicit_euler.xml
%feature("docstring") cmf::math::ImplicitEuler "

An implicit (backward) Euler integrator using fixpoint iteration.

C++ includes: implicit_euler.h ";

/*  Internal data storages  */

/*  Accuracy parameters  */

/*  model time  */

%feature("docstring")  cmf::math::ImplicitEuler::get_t "cmf::math::Time get_t() const

Returns the current model time. ";

%feature("docstring")  cmf::math::ImplicitEuler::set_t "void
set_t(cmf::math::Time val)

Sets the current model time. ";

%feature("docstring")  cmf::math::ImplicitEuler::get_dt "cmf::math::Time get_dt() const

Returns the last time step. ";

/*  Integrate  */

%feature("docstring")  cmf::math::ImplicitEuler::integrate_until "void integrate_until(cmf::math::Time t_max, cmf::math::Time dt=Time(),
bool reset=false)

Integrates the vector of state variables until t_max.

Parameters:
-----------

t_max:   Time, the solver should run to

dt:   Time step (may be omitted)

reset:  If true, solver is reseted before integration starts ";

%feature("docstring")  cmf::math::ImplicitEuler::ImplicitEuler "ImplicitEuler(const cmf::math::state_list &states, real epsilon=1e-9,
cmf::math::Time tStepMin=cmf::math::timespan(10))

Constructs a new FixPointImplicitEuler from a pointer to a vector of
state variables.

The Integrator becomes the owner of states

Parameters:
-----------

states:  Statevariables of the system

epsilon:  relative error tolerance per time step (default=1e-9)

tStepMin:  minimum time step (default=10s) ";

%feature("docstring")  cmf::math::ImplicitEuler::ImplicitEuler "ImplicitEuler(real epsilon=1e-9, cmf::math::Time
tStepMin=cmf::math::timespan(10))

Constructs a new FixPointImplicitEuler.

Parameters:
-----------

epsilon:  relative error tolerance per time step (default=1e-9)

tStepMin:  minimum time step (default=10s) ";

%feature("docstring")  cmf::math::ImplicitEuler::ImplicitEuler "ImplicitEuler(const Integrator &)

copy constructor ";

%feature("docstring")  cmf::math::ImplicitEuler::copy "virtual
Integrator* copy() const

Polymorphic copy constructor. ";

%feature("docstring")  cmf::math::ImplicitEuler::integrate "int
integrate(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables.

Parameters:
-----------

MaxTime:  To stop the model (if running in a model framework) at time
steps of value exchange e.g. full hours, the next value exchange time
can be given

TimeStep:  Takes the proposed timestep, and changes it into the
effictivly used timestep according to the local stiffness of the
problem and MaxTime ";

%feature("docstring")  cmf::math::ImplicitEuler::reset "void reset()
override

Resets any saved history (for multistep methods) ";

%feature("docstring")  cmf::math::ImplicitEuler::set_system "void
set_system(const state_list &states) ";

%feature("docstring")  cmf::math::ImplicitEuler::size "size_t size()
const ";

%feature("docstring")  cmf::math::ImplicitEuler::to_string "std::string to_string() const override

Desrcibes the iterator. ";


// File: classcmf_1_1math_1_1integratable.xml
%feature("docstring") cmf::math::integratable "

integratable is a functionality for different classes for integrating
values over time.

Main usage of an integratable is the calculation of average fluxes
over time e.g. 

.. math::

    
    \\\\int_{t_0}^{t_{end}}q\\\\left(t,V_i,V_j\\\\right)dt 

C++ includes: statevariable.h ";

%feature("docstring")  cmf::math::integratable::avg "virtual double
avg() const =0

Returns average of the integrated variable (eg. flux) from the last
reset until the last call of integrate. ";

%feature("docstring")  cmf::math::integratable::integrate "virtual
void integrate(Time t)=0

Integrates the variable until time t. ";

%feature("docstring")  cmf::math::integratable::reset "virtual void
reset(Time t)=0

Sets the start time of the integral. ";

%feature("docstring")  cmf::math::integratable::sum "virtual double
sum() const =0

Get the integral from the last reset until the last call of integrate.
";


// File: classcmf_1_1math_1_1integratable__list.xml
%feature("docstring") cmf::math::integratable_list "

A list of cmf::math::integratable objects.

Todo TODO: Complete collection interface (getitem with slicing etc.)

C++ includes: statevariable.h ";

%feature("docstring")  cmf::math::integratable_list::integratable_list
"integratable_list() ";

%feature("docstring")  cmf::math::integratable_list::integratable_list
"integratable_list(const integratable_list &for_copy) ";

%feature("docstring")  cmf::math::integratable_list::append "void
append(cmf::math::integratable::ptr add)

Adds an integratable to the list. ";

%feature("docstring")  cmf::math::integratable_list::avg "cmf::math::num_array avg() const ";

%feature("docstring")  cmf::math::integratable_list::integrate "void
integrate(Time t) ";

%feature("docstring")  cmf::math::integratable_list::remove "void
remove(cmf::math::integratable::ptr rm)

Removes an integratable from the list. ";

%feature("docstring")  cmf::math::integratable_list::reset "void
reset(Time t) ";

%feature("docstring")  cmf::math::integratable_list::size "size_t
size() const

Number of integratables in the list. ";

%feature("docstring")  cmf::math::integratable_list::sum "cmf::math::num_array sum() const ";


// File: classcmf_1_1math_1_1_integrator.xml
%feature("docstring") cmf::math::Integrator "

Base class for any kind of integrator.

Pure virtual functions: Integrate

copy Please provide a custom copy constructor

C++ includes: integrator.h ";

/*  Accuracy parameters  */

/*  model time  */

%feature("docstring")  cmf::math::Integrator::get_t "cmf::math::Time
get_t() const

Returns the current model time. ";

%feature("docstring")  cmf::math::Integrator::set_t "void
set_t(cmf::math::Time val)

Sets the current model time. ";

%feature("docstring")  cmf::math::Integrator::get_dt "cmf::math::Time
get_dt() const

Returns the last time step. ";

%feature("docstring")  cmf::math::Integrator::reset "virtual void
reset()

Resets any saved history (for multistep methods) ";

/*  Constructors and Destructors  */

%feature("docstring")  cmf::math::Integrator::Integrator "Integrator(real epsilon=1e-9)

Constructs a new Integrator with a new own state vector.

Parameters:
-----------

epsilon:  relative error tolerance per time step (default=1e-9) ";

%feature("docstring")  cmf::math::Integrator::Integrator "Integrator(const state_list &states, real epsilon=1e-9) ";

%feature("docstring")  cmf::math::Integrator::Integrator "Integrator(const cmf::math::Integrator &other) ";

%feature("docstring")  cmf::math::Integrator::copy "virtual
Integrator* copy() const =0

Polymorphic copy constructor. ";

/*  Integrate  */

%feature("docstring")  cmf::math::Integrator::integrate "virtual int
integrate(cmf::math::Time t_max, cmf::math::Time dt)=0

Integrates the vector of state variables.

Parameters:
-----------

t_max:  To stop the model (if running in a model framework) at time
steps of value exchange e.g. full hours, the next value exchange time
can be given

dt:  Takes the proposed time step, and changes it into the effectively
used time step according to the local stiffness of the problem and
MaxTime ";

%feature("docstring")  cmf::math::Integrator::integrate_until "void
integrate_until(cmf::math::Time t_max, cmf::math::Time dt=Time(), bool
reset=false)

Integrates the vector of state variables until t_max.

Parameters:
-----------

t_max:   Time, the solver should run to

dt:   Time step (may be omitted)

reset:  If true, solver is reseted before integration starts ";

%feature("docstring")  cmf::math::Integrator::~Integrator "virtual
~Integrator()=default ";

%feature("docstring")  cmf::math::Integrator::set_system "void
set_system(const state_list &states) ";

%feature("docstring")  cmf::math::Integrator::size "size_t size()
const ";

%feature("docstring")  cmf::math::Integrator::to_string "virtual
std::string to_string() const =0

Desrcibes the iterator. ";


// File: classcmf_1_1math_1_1root__finding_1_1iteration__error.xml
%feature("docstring") cmf::math::root_finding::iteration_error "";

%feature("docstring")
cmf::math::root_finding::iteration_error::iteration_error "iteration_error(const std::string &msg) ";


// File: classcmf_1_1river_1_1_i_volume_height_function.xml
%feature("docstring") cmf::river::IVolumeHeightFunction "

Volume height relations are functional objects, which return a height
and a crosssectional area of a volume for different geometric bodies.

This is the abstract base class, where the geometries derive from

C++ includes: ReachType.h ";

%feature("docstring")
cmf::river::IVolumeHeightFunction::~IVolumeHeightFunction "virtual
~IVolumeHeightFunction() ";

%feature("docstring")  cmf::river::IVolumeHeightFunction::A "virtual
double A(double V) const =0

Returns the area of the surface for a given volume. ";

%feature("docstring")  cmf::river::IVolumeHeightFunction::copy "virtual IVolumeHeightFunction* copy() const =0 ";

%feature("docstring")  cmf::river::IVolumeHeightFunction::h "virtual
double h(double V) const =0

Returns the depth of a given volume. ";

%feature("docstring")  cmf::river::IVolumeHeightFunction::q "virtual
double q(double h, double slope) const ";

%feature("docstring")  cmf::river::IVolumeHeightFunction::V "virtual
double V(double h) const =0 ";


// File: classcmf_1_1jacobian_1_1_jacobian.xml
%feature("docstring") cmf::jacobian::Jacobian "

Approximates the jacobian for a cmf solver J[i,j] =
(dxdt(S_i,t)[j]-dxdt(S,t)[j])/delta S is the state vector S_i equals
S, except for S_i[i]=S[i]+delta delta is the finite difference to
approximate the Jacobian.     delta should be a small number, but big
enough to avoid floating point errors.      1e-6 to 1e-9 should be
nice values  Usage to show the jacobian:     # Allocate memory for the
jacobian     jac = Jacobian(solver,delta)     # Calculate the Jacobian
J = jac()     # Show the Jacobian
imshow(jac(),interpolation='nearest') ";

%feature("docstring")  cmf::jacobian::Jacobian::__init__ "def
__init__(self, solver, delta=1e-6)

solver is a cmf integrator, delta is the amount the state should be
changed ";

%feature("docstring")  cmf::jacobian::Jacobian::__call__ "def
__call__(self) ";

%feature("docstring")  cmf::jacobian::Jacobian::__len__ "def
__len__(self) ";

%feature("docstring")  cmf::jacobian::Jacobian::dxdt "def dxdt(self)

Returns the current right hand side of the ODE for the current states
and the current time ";

%feature("docstring")  cmf::jacobian::Jacobian::t "def t(self) ";


// File: classcmf_1_1upslope_1_1connections_1_1_jarvis_macro_flow.xml
%feature("docstring") cmf::upslope::connections::JarvisMacroFlow "

A physically based macropore to macropore connection according to
Jarvis & Leeds-Harrison 1987, JSS.



.. math::

    q_{i->j} [m/s]= \\\\rho \\\\frac G {12\\\\eta} w^2 \\\\frac{e_v-
    e_r}{1-e_r} S_{c,i}^\\\\beta\\\\ (1-S_{c,j})

where:  :math:`q_{i->j}`
the flow from macro pore layer i to macropore layer j

:math:`\\\\rho=10^{-3} kg/m^3` - the density of water

:math:`G=9.81 m/s^2` the earth acceleration

:math:`\\\\eta=1.0 kg/(m s)` the viscosity of water (at 20 degC)

:math:`w [m]` the crack width, a function of water content and crack
distance

:math:`e_v [-]` the crack porosity

:math:`e_r [-]` crack por

:math:`S_c [-]` the crack saturation of layer i resp. j

:math:`\\\\beta [-]` a conceptional exponent to shape the flow reaction

C++ includes: macropore.h ";

%feature("docstring")
cmf::upslope::connections::JarvisMacroFlow::JarvisMacroFlow "JarvisMacroFlow(cmf::water::WaterStorage::ptr left,
cmf::water::flux_node::ptr right, real beta=1., real
porefraction_r=0.0)

Constructs the connection.

Parameters:
-----------

left:  right:  the connected macropores

beta:  User defined parameter for the swelling reaction

porefraction_r:  Porefraction at which flow starts. For swelling soils
that are closing completely th ";

%feature("docstring")
cmf::upslope::connections::JarvisMacroFlow::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::JarvisMacroFlow::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")
cmf::upslope::connections::JarvisMacroFlow::get_ptr "ptr get_ptr()
const ";

%feature("docstring")
cmf::upslope::connections::JarvisMacroFlow::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::JarvisMacroFlow::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::JarvisMacroFlow::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::JarvisMacroFlow::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")
cmf::upslope::connections::JarvisMacroFlow::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::JarvisMacroFlow::left_node "flux_node::ptr
left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::connections::JarvisMacroFlow::q "real q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::JarvisMacroFlow::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::upslope::connections::JarvisMacroFlow::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::JarvisMacroFlow::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::JarvisMacroFlow::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::connections::JarvisMacroFlow::short_string "virtual
std::string short_string() const ";

%feature("docstring")
cmf::upslope::connections::JarvisMacroFlow::to_string "virtual
std::string to_string() const ";


// File: classcmf_1_1upslope_1_1connections_1_1_kinematic_macro_flow.xml
%feature("docstring") cmf::upslope::connections::KinematicMacroFlow "

Linear storage based flux from macro pore to macro pore.

Deprecated The MacroPore model is still very experimental and not
stable. Only for tryouts!



.. math::

     q = A_{cell} K_{macro}
    \\\\left(\\\\frac{V_{upper}}{C_{upper}}\\\\right)^\\\\beta
    \\\\left(1-\\\\frac{V_{lower}}{C_{lower}}\\\\right) 

where:
:math:`A_{cell}` is the area of the owning cell in m2

:math:`K_{macro}` is the conductivity of the macro pore storage

:math:`V` is the actual stored water volume in the upper resp. lower macro
pore storage

:math:`C` is the capacity of the upper resp. lower macro pore storage

C++ includes: macropore.h ";

%feature("docstring")
cmf::upslope::connections::KinematicMacroFlow::KinematicMacroFlow "KinematicMacroFlow(cmf::water::WaterStorage::ptr left,
cmf::water::flux_node::ptr right, real beta=1.)

Creates the connection.

Parameters:
-----------

left:  right:  the nodes between the connection should be created.

beta:  a conceptional curve shape parameter for the relation between
storage and outflow

Either left or right needs to be a MacroPore, left needs to be a water
storage ";

%feature("docstring")
cmf::upslope::connections::KinematicMacroFlow::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::KinematicMacroFlow::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")
cmf::upslope::connections::KinematicMacroFlow::get_ptr "ptr get_ptr()
const ";

%feature("docstring")
cmf::upslope::connections::KinematicMacroFlow::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::KinematicMacroFlow::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::KinematicMacroFlow::get_tracer_filter "real get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::KinematicMacroFlow::get_tracer_filter "real get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")
cmf::upslope::connections::KinematicMacroFlow::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::KinematicMacroFlow::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::connections::KinematicMacroFlow::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::KinematicMacroFlow::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::upslope::connections::KinematicMacroFlow::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::KinematicMacroFlow::set_tracer_filter "void set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::KinematicMacroFlow::set_tracer_filter "void set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::connections::KinematicMacroFlow::short_string "virtual
std::string short_string() const ";

%feature("docstring")
cmf::upslope::connections::KinematicMacroFlow::to_string "virtual
std::string to_string() const ";


// File: classcmf_1_1upslope_1_1connections_1_1_kinematic_surface_runoff.xml
%feature("docstring")
cmf::upslope::connections::KinematicSurfaceRunoff "

A connection to route water from a SurfaceWater storage to another
node following a topographic gradient.



.. math::

    q_{runoff} = A_{cross} d_{eff}^{2/3}
    \\\\frac{\\\\sqrt{S}}{n}

where:  :math:`q_{runoff}` is the surface
runoff

:math:`A_{cross}` is the wetted crossectional flux area, given as :math:`d_{eff} \\\\cdot w`

:math:`w` is the width of the shared boundary between the surface water
storage and the target node

:math:`d_{eff}` is the effective flow depth of the surface water.The
effective flow depth is defined as 

.. math::

    d_{eff} = \\\\begin{cases}
    V/A-d_{puddle}\\\\ & V/A>d_{puddle} \\\\\\\\ 0.0 & V/A<=d_{puddle}
    \\\\end{cases}

:math:`V` the volume of stored water in the surface in :math:`m^3`

:math:`A` the area of the cell in :math:`m^2`

:math:`d_{puddle}=V_{puddle}/A` the average depth of water in the surface
water needed to start run off

:math:`S = \\\\|\\\\frac{\\\\Delta z\\\\|}{d}` the slope between
surfacewater center and the target node

:math:`n` the manning roughness

The KinematicSurfaceRunoff can be used as a cell connecting flux as
in: This results in a connection of the surfacewater storage of each
cell with the surface water storages of its neighborssee

C++ includes: surfacewater.h ";

%feature("docstring")
cmf::upslope::connections::KinematicSurfaceRunoff::KinematicSurfaceRunoff
"KinematicSurfaceRunoff(cmf::upslope::SurfaceWater::ptr left,
cmf::water::flux_node::ptr right, real flowwidth, real distance=-1)

Creates a KinematicSurfaceRunoff between a SurfaceWater (left) with
another (right) node.

Parameters:
-----------

left:  A surfacewater storage

right:  The target node

flowwidth:  the length of the shared boundary between left and right
in m

distance:  the distance between left and right in m. If d<=0m, the
distance is calculated according to the position of left and right ";

%feature("docstring")
cmf::upslope::connections::KinematicSurfaceRunoff::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::KinematicSurfaceRunoff::exchange_target "void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")
cmf::upslope::connections::KinematicSurfaceRunoff::get_ptr "ptr
get_ptr() const ";

%feature("docstring")
cmf::upslope::connections::KinematicSurfaceRunoff::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::KinematicSurfaceRunoff::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::KinematicSurfaceRunoff::get_tracer_filter "real get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::KinematicSurfaceRunoff::get_tracer_filter "real get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")
cmf::upslope::connections::KinematicSurfaceRunoff::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::KinematicSurfaceRunoff::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::connections::KinematicSurfaceRunoff::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::KinematicSurfaceRunoff::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::upslope::connections::KinematicSurfaceRunoff::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::KinematicSurfaceRunoff::set_tracer_filter "void set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::KinematicSurfaceRunoff::set_tracer_filter "void set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::connections::KinematicSurfaceRunoff::short_string "virtual std::string short_string() const ";

%feature("docstring")
cmf::upslope::connections::KinematicSurfaceRunoff::to_string "virtual
std::string to_string() const ";


// File: classcmf_1_1water_1_1_langmuir_adsorption.xml
%feature("docstring") cmf::water::LangmuirAdsorption "

This class calculates the adsorption equilibrium between sorbat and
sorbent using the Langmuir isotherme.

Langmuir Adsorption:



.. math::

    \\\\frac{x_{ad}}{m} = q = \\\\frac{K c}{1 + K c}

where
:math:`x_{ad} = x_{tot} - x_{free}` is the adsorbed tracer mass  :math:`x_{tot}`
is the total tracer mass

:math:`x_{free}` is the dissolved tracer mass

:math:`m` is the mass of the sorbent in the same unit as the tracer mass

:math:`K` is the Langmuir sorption coefficient

:math:`c = \\\\frac{x_{free}}{V}` is the concentration of the tracer in
tracer mass per m3

CMF stores in a solute storage the total mass of a tracer and needs to
calculate the free tracer mass. The analytical solution for :math:`x_{free}`
from :math:`x_{tot}` is implemented in freesolute and derived usingsympy. If
you really want to see it, look in the code.

http://en.wikipedia.org/wiki/Langmuir_equation

C++ includes: adsorption.h ";

%feature("docstring")
cmf::water::LangmuirAdsorption::LangmuirAdsorption "LangmuirAdsorption(real K, real m) ";

%feature("docstring")
cmf::water::LangmuirAdsorption::LangmuirAdsorption "LangmuirAdsorption(const LangmuirAdsorption &other) ";

%feature("docstring")
cmf::water::LangmuirAdsorption::~LangmuirAdsorption "virtual
~LangmuirAdsorption() ";

%feature("docstring")  cmf::water::LangmuirAdsorption::copy "LangmuirAdsorption* copy(real m=-1) const

returns a copy of the Adsorption object.

If the adsorption is depending on the sorbent mass, you can give a
positive value for the sorbent mass m. If the value is not given or
negative, m is used from the original object. ";

%feature("docstring")  cmf::water::LangmuirAdsorption::freesolute "real freesolute(real xt, real V) const

Returns the mass of dissolved tracer as a function of the total tracer
mass in the solute storage and the water volume.

Parameters:
-----------

xt:   :math:`x_t` the total tracer mass in the storage

V:   :math:`V m^3` the water volume in the storage

:math:`x_f` the dissolved mass of the tracer ";

%feature("docstring")  cmf::water::LangmuirAdsorption::totalsolute "real totalsolute(real xf, real V) const

Returns the total mass of the tracer from the dissolved concetration
in tracer unit/m3.

Parameters:
-----------

xf:   :math:`x_f` the dissolved tracer mass in the storage

V:   :math:`V m^3` the water volume in the storage

:math:`x_t` the total mass of the tracer ";


// File: classcmf_1_1upslope_1_1connections_1_1lateral__sub__surface__flux.xml
%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux "

An abstract base class for lateral subsurface fluxes.

C++ includes: subsurfacefluxes.h ";

%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux::exchange_target "void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux::get_ptr "ptr
get_ptr() const ";

%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux::get_tracer_filter
"real get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux::get_tracer_filter
"real get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux::set_tracer_filter
"void set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux::set_tracer_filter
"void set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux::short_string "virtual std::string short_string() const ";

%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux::to_string "virtual std::string to_string() const ";


// File: classcmf_1_1upslope_1_1layer__list.xml
%feature("docstring") cmf::upslope::layer_list "

A vector of layers, with array access to the properties of the layers,
for fast data exchange.

C++ includes: layer_list.h ";

%feature("docstring")  cmf::upslope::layer_list::layer_list "layer_list(const layer_list &for_copy)

Copy constructor. ";

%feature("docstring")  cmf::upslope::layer_list::layer_list "layer_list()

Creates an empty layer list. ";

%feature("docstring")  cmf::upslope::layer_list::layer_list "layer_list(const cmf::water::node_list &for_copy)

Creates a list of all soil layers from a node_list. ";

%feature("docstring")  cmf::upslope::layer_list::append "layer_list&
append(SoilLayer::ptr l)

Appends a soil layer to the list. ";

%feature("docstring")  cmf::upslope::layer_list::begin "iterator
begin() ";

%feature("docstring")  cmf::upslope::layer_list::begin "const_iterator begin() const ";

%feature("docstring")  cmf::upslope::layer_list::clear "void clear()

Clears the list. ";

%feature("docstring")  cmf::upslope::layer_list::end "iterator end()
";

%feature("docstring")  cmf::upslope::layer_list::end "const_iterator
end() const ";

%feature("docstring")  cmf::upslope::layer_list::extend "layer_list&
extend(const layer_list &ll)

Appends all soil layers of layer_list to this. ";

%feature("docstring")  cmf::upslope::layer_list::extend "layer_list&
extend(const cmf::water::node_list &nl)

Appends all soil layers from a node_list to this. ";

%feature("docstring")
cmf::upslope::layer_list::get_gravitational_potential "cmf::math::num_array get_gravitational_potential() const

Returns an array containing the gravitational potential in m of all
layers in the list. ";

%feature("docstring")  cmf::upslope::layer_list::get_ice_fraction "cmf::math::num_array get_ice_fraction() const ";

%feature("docstring")  cmf::upslope::layer_list::get_K "cmf::math::num_array get_K() const

Returns an array containing the conductivity in m/day of all layers in
the list. ";

%feature("docstring")  cmf::upslope::layer_list::get_Ksat "cmf::math::num_array get_Ksat() const

Returns an array containing the saturated conductivity in m/day of all
layers in the list. ";

%feature("docstring")  cmf::upslope::layer_list::get_lower_boundary "cmf::math::num_array get_lower_boundary() const

Returns an array containing the lower boundary in m below surface of
all layers in the list. ";

%feature("docstring")  cmf::upslope::layer_list::get_matrix_potential
"cmf::math::num_array get_matrix_potential() const

Returns an array containing the matrix potential in m of all layers in
the list. ";

%feature("docstring")  cmf::upslope::layer_list::get_percolation "cmf::math::num_array get_percolation(cmf::math::Time t) const

Returns the flux to each layer from the upper layer, or, in case of
the first layer from the surface water. ";

%feature("docstring")  cmf::upslope::layer_list::get_porosity "cmf::math::num_array get_porosity() const

Returns an array containing the porosity in m3/m3 of all layers in the
list. ";

%feature("docstring")  cmf::upslope::layer_list::get_potential "cmf::math::num_array get_potential() const

Returns an array containing the potentials of all layers in the list.
";

%feature("docstring")  cmf::upslope::layer_list::get_rootfraction "cmf::math::num_array get_rootfraction() const

Returns an array containing the rootfraction of each layer. ";

%feature("docstring")  cmf::upslope::layer_list::get_slice "layer_list get_slice(size_t first=0, size_t last=1000000, size_t
step=1) ";

%feature("docstring")  cmf::upslope::layer_list::get_theta "cmf::math::num_array get_theta() const ";

%feature("docstring")  cmf::upslope::layer_list::get_thickness "cmf::math::num_array get_thickness() const

Returns an array containing the thickness in m of all layers in the
list. ";

%feature("docstring")  cmf::upslope::layer_list::get_upper_boundary "cmf::math::num_array get_upper_boundary() const

Returns an array containing the upper boundary in m below surface of
all layers in the list. ";

%feature("docstring")  cmf::upslope::layer_list::get_volume "cmf::math::num_array get_volume() const

Returns an array containing the volume in m3 of stored water of all
layers in the list. ";

%feature("docstring")  cmf::upslope::layer_list::get_wetness "cmf::math::num_array get_wetness() const

Returns an array containing the wetness in m3 H2O/m3 pore space of all
layers in the list. ";

%feature("docstring")  cmf::upslope::layer_list::pop "SoilLayer::ptr
pop()

Deletes the last layer from the list and returns it. ";

%feature("docstring")  cmf::upslope::layer_list::set_ice_fraction "void set_ice_fraction(const cmf::math::num_array &Value, size_t
offset=0)

Sets the fraction of the ice content of the soil water. ";

%feature("docstring")  cmf::upslope::layer_list::set_potential "void
set_potential(const cmf::math::num_array &Value, size_t offset=0)

Sets the potential (head) in m of layers [offset : arraysize]. ";

%feature("docstring")  cmf::upslope::layer_list::set_rootfraction "void set_rootfraction(const cmf::math::num_array &Value, size_t
offset=0)

Sets the fraction of roots in each layer. ";

%feature("docstring")  cmf::upslope::layer_list::set_theta "void
set_theta(const cmf::math::num_array &Value, size_t offset=0)

Sets the volumetric water content of the soil. ";

%feature("docstring")  cmf::upslope::layer_list::set_volume "void
set_volume(const cmf::math::num_array &Value, size_t offset=0)

Sets the Volume in m3 of layers [offset : arraysize]. ";

%feature("docstring")  cmf::upslope::layer_list::set_wetness "void
set_wetness(const cmf::math::num_array &Value, size_t offset=0)

Sets the wetness in m3/m3 of layers [offset : arraysize]. ";

%feature("docstring")  cmf::upslope::layer_list::size "size_t size()
const

Number of layers in the list. ";


// File: classcmf_1_1upslope_1_1connections_1_1_layer_bypass.xml
%feature("docstring") cmf::upslope::connections::LayerBypass "

A simplification of macro pore flux for swelling soils.

Connects the surfacewater of the cell with deeper layers, assuming the
presence of cracks. At saturation level of the target layer, the
cracks are closed 

.. math::

    q_{crack} = K_{max,crack}
    \\\\left(1-\\\\left(\\\\frac{w-w_0}{1-w_0}\\\\right)^\\\\beta\\\\right)
    A

where:  :math:`q_{crack}` is the flux from the surface water to the
target layer in m3/day

:math:`K_{max,crack}` is the maximum conductivity of the cracks in m/day

:math:`w` is the actual wetness of the target layer

:math:`w_0` is saturation, where the shrinkage of the cracks starts

:math:`\\\\beta` is an empirical shape parameter of the crack size/wetness
relation

:math:`A` is the area of the cell

C++ includes: Percolation.h ";

%feature("docstring")
cmf::upslope::connections::LayerBypass::LayerBypass "LayerBypass(cmf::water::flux_node::ptr left,
cmf::upslope::SoilLayer::ptr right, real Kmax=100., real w0=0.0, real
beta=1.0)

Creates a layer bypass connection.

Parameters:
-----------

left:  The source of the flux, usually the surfacewater of the cell

right:  The target soil layer

Kmax:  maximum conductivity of the cracks in m/day

w0:  is the actual wetness of the target layer

beta:  is an empirical shape parameter of the crack size/wetness
relation ";

%feature("docstring")  cmf::upslope::connections::LayerBypass::conc "real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::LayerBypass::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::connections::LayerBypass::get_ptr
"ptr get_ptr() const ";

%feature("docstring")
cmf::upslope::connections::LayerBypass::get_target "flux_node::ptr
get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::LayerBypass::get_target "flux_node::ptr
get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::LayerBypass::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::LayerBypass::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::upslope::connections::LayerBypass::K "real K(real w)

the actual crack conductivity

Parameters:
-----------

w:  the wetness of the target layer ";

%feature("docstring")  cmf::upslope::connections::LayerBypass::kill_me
"bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::LayerBypass::left_node "flux_node::ptr
left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::connections::LayerBypass::q "real q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::connections::LayerBypass::refresh
"void refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::upslope::connections::LayerBypass::right_node "flux_node::ptr
right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::LayerBypass::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::LayerBypass::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::connections::LayerBypass::short_string "virtual
std::string short_string() const ";

%feature("docstring")
cmf::upslope::connections::LayerBypass::to_string "virtual
std::string to_string() const ";


// File: classcmf_1_1water_1_1linear__scale.xml
%feature("docstring") cmf::water::linear_scale "

A linear scaling functor, with slope and displacement.

C++ includes: boundary_condition.h ";

%feature("docstring")  cmf::water::linear_scale::linear_scale "linear_scale(real _slope=1, real _displacement=0)

Creates a linear scale (by default it is a unity scale, :math:`a=1; b=0`) ";


// File: classcmf_1_1water_1_1_linear_adsorption.xml
%feature("docstring") cmf::water::LinearAdsorption "

This class calculates the adsorption equilibrium between sorbat and
sorbent using the linear (Henry) isotherme.

Linear (Henry) isotherme:



.. math::

    \\\\frac{x_{ad}}{m} = K c

where  :math:`x_{ad} = x_{tot} - x_{free}` is the adsorbed tracer mass  :math:`x_{tot}` is the total tracer
mass

:math:`x_{free}` is the dissolved tracer mass

:math:`m` is the mass of the sorbent in the same unit as the tracer mass

:math:`K` is the Henry sorption coefficient

:math:`c = \\\\frac{x_{free}}{V}` is the concentration of the tracer in
tracer mass per m3

CMF stores in a solute storage the total mass of a tracer and needs to
calculate the free tracer mass. Calculating :math:`x_{free}` from :math:`x_{tot}`
gives from the eq. above: 

.. math::

    x_{free} = x_{tot} \\\\frac{V}{K m +
    V}

C++ includes: adsorption.h ";

%feature("docstring")  cmf::water::LinearAdsorption::LinearAdsorption
"LinearAdsorption(real K, real m) ";

%feature("docstring")  cmf::water::LinearAdsorption::LinearAdsorption
"LinearAdsorption(const LinearAdsorption &other) ";

%feature("docstring")  cmf::water::LinearAdsorption::~LinearAdsorption
"virtual ~LinearAdsorption() ";

%feature("docstring")  cmf::water::LinearAdsorption::copy "LinearAdsorption* copy(real m=-1) const

returns a copy of the Adsorption object.

If the adsorption is depending on the sorbent mass, you can give a
positive value for the sorbent mass m. If the value is not given or
negative, m is used from the original object. ";

%feature("docstring")  cmf::water::LinearAdsorption::freesolute "real
freesolute(real xt, real V) const

Returns the mass of dissolved tracer as a function of the total tracer
mass in the solute storage and the water volume.

Parameters:
-----------

xt:   :math:`x_t` the total tracer mass in the storage

V:   :math:`V m^3` the water volume in the storage

:math:`x_f` the dissolved mass of the tracer ";

%feature("docstring")  cmf::water::LinearAdsorption::totalsolute "real totalsolute(real xf, real V) const

Returns the total mass of the tracer from the dissolved concetration
in tracer unit/m3.

Parameters:
-----------

xf:   :math:`x_f` the dissolved tracer mass in the storage

V:   :math:`V m^3` the water volume in the storage

:math:`x_t` the total mass of the tracer ";


// File: classcmf_1_1water_1_1_linear_gradient_flux.xml
%feature("docstring") cmf::water::LinearGradientFlux "

A generic node-to-node gradient based connection.

This connection is similar to the Darcy-connection, but there are no
restrictions concerning the type of nodes. However, the left side
needs to be a water storage 

.. math::

     q = Q_1 \\\\nabla \\\\Psi 

where:  :math:`q`: the resulting flux in :math:`m^3/day`

:math:`Q_1`: Flux over the connection for a unity gradient ( :math:`\\\\nabla \\\\Psi = 1`) in :math:`\\\\frac{m^3}{day}`

:math:`\\\\nabla \\\\Psi = \\\\frac{\\\\Psi_{l}-\\\\Psi_{r}}{d}`: The
hydraulic gradient of the (l)eft, resp. (r)ight node of the connection

:math:`d`: The topographic length of the connection in m

C++ includes: simple_connections.h ";

%feature("docstring")
cmf::water::LinearGradientFlux::LinearGradientFlux "LinearGradientFlux(cmf::water::flux_node::ptr left,
cmf::water::flux_node::ptr right, real Q1, real d=-1.0)

Creates a generic gradient based flux, if enough water is present in
the source.

Parameters:
-----------

left:  The left node of the connection

right:  The right node of the connection

Q1:  Flux over the connection for a unity gradient ( :math:`\\\\nabla \\\\Psi = 1`) in :math:`\\\\frac{m^3}{day}`

d:  the topographic lenght of the connection in m ";

%feature("docstring")  cmf::water::LinearGradientFlux::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")  cmf::water::LinearGradientFlux::exchange_target
"void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")  cmf::water::LinearGradientFlux::get_ptr "ptr
get_ptr() const ";

%feature("docstring")  cmf::water::LinearGradientFlux::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::water::LinearGradientFlux::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::water::LinearGradientFlux::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::water::LinearGradientFlux::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::water::LinearGradientFlux::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::water::LinearGradientFlux::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::water::LinearGradientFlux::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::water::LinearGradientFlux::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")  cmf::water::LinearGradientFlux::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::water::LinearGradientFlux::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::water::LinearGradientFlux::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")  cmf::water::LinearGradientFlux::short_string "virtual std::string short_string() const ";

%feature("docstring")  cmf::water::LinearGradientFlux::to_string "virtual std::string to_string() const ";


// File: classcmf_1_1upslope_1_1_linear_retention.xml
%feature("docstring") cmf::upslope::LinearRetention "

The linear retention curve provides a simple linear relationship
between storage and head.

Head function (head in m, calculated from upper side control volume)


.. math::

     h(\\\\theta) = -\\\\Delta z \\\\left( 1 - \\\\frac{\\\\theta -
    \\\\theta_r}{\\\\theta_s - \\\\theta_r} \\\\right) 

Conductivity
function 

.. math::

     K(\\\\theta) = K_{sat} \\\\left(\\\\frac{\\\\theta -
    \\\\theta_r}{\\\\theta_s - \\\\theta_r}\\\\right)^\\\\beta 

C++ includes: RetentionCurve.h ";

%feature("docstring")  cmf::upslope::LinearRetention::LinearRetention
"LinearRetention(real ksat, real phi, real thickness, real
residual_wetness=0.1) ";

%feature("docstring")  cmf::upslope::LinearRetention::~LinearRetention
"virtual ~LinearRetention() ";

%feature("docstring")  cmf::upslope::LinearRetention::copy "LinearRetention* copy() const ";

%feature("docstring")  cmf::upslope::LinearRetention::Diffusivity "virtual real Diffusivity(real wetness) const

Returns the Diffusivity of the soil.

Not implemented for all retention curves. Diffusivity is used by
MACROlikeMacroMicroExchange ";

%feature("docstring")  cmf::upslope::LinearRetention::Diffusivity "cmf::math::num_array Diffusivity(cmf::math::num_array &wetness) ";

%feature("docstring")  cmf::upslope::LinearRetention::dPsiM_dW "virtual real dPsiM_dW(real wetness) const ";

%feature("docstring")  cmf::upslope::LinearRetention::dPsiM_dW "cmf::math::num_array dPsiM_dW(const cmf::math::num_array &wetness)
const ";

%feature("docstring")  cmf::upslope::LinearRetention::FillHeight "virtual real FillHeight(real lowerDepth, real Area, real Volume) const

Returns the thickness of a soil column with a certain pore volume. ";

%feature("docstring")  cmf::upslope::LinearRetention::K "cmf::math::num_array K(const cmf::math::num_array &wetness) const ";

%feature("docstring")  cmf::upslope::LinearRetention::K "virtual real
K(real wetness) const

Returns the conductivity in m/day at a certain depth and water
content. ";

%feature("docstring")  cmf::upslope::LinearRetention::MatricPotential
"cmf::math::num_array MatricPotential(const cmf::math::num_array
&wetness) const ";

%feature("docstring")  cmf::upslope::LinearRetention::MatricPotential
"virtual real MatricPotential(real wetness) const

Returns the potential below upper side of the control volume in m. ";

%feature("docstring")  cmf::upslope::LinearRetention::Porosity "virtual real Porosity(real depth) const

Returns the porosity at a certain depth. ";

%feature("docstring")  cmf::upslope::LinearRetention::theta "virtual
real theta(real wetness) const

returns the water content :math:`theta` for a given wetness ";

%feature("docstring")  cmf::upslope::LinearRetention::theta "cmf::math::num_array theta(const cmf::math::num_array &wetness) const
";

%feature("docstring")  cmf::upslope::LinearRetention::Transmissivity "virtual real Transmissivity(real upperDepth, real lowerDepth, real
wetness) const ";

%feature("docstring")  cmf::upslope::LinearRetention::VoidVolume "virtual real VoidVolume(real upperDepth, real lowerDepth, real Area)
const

Returns the void volume of a soil column. ";

%feature("docstring")  cmf::upslope::LinearRetention::Wetness "cmf::math::num_array Wetness(const cmf::math::num_array &suction)
const ";

%feature("docstring")  cmf::upslope::LinearRetention::Wetness "virtual real Wetness(real suction) const

Returns the wetness (water content per pore volume) at a given head.
";

%feature("docstring")  cmf::upslope::LinearRetention::Wetness_eff "virtual real Wetness_eff(real wetness, real pF_r=4.2) const

Returns the effective wetness, using a residual pF value 

.. math::

    w_{eff}
    =
    \\\\frac{w_{act}-w\\\\left(pF_r\\\\right)}{1-w\\\\left(pF_r\\\\right)}

.
";

%feature("docstring")  cmf::upslope::LinearRetention::Wetness_pF "real Wetness_pF(real pF) const

returns the volumetric water content at a given pF value ";

%feature("docstring")  cmf::upslope::LinearRetention::Wetness_pF "cmf::math::num_array Wetness_pF(const cmf::math::num_array &pF) const
";


// File: classcmf_1_1water_1_1_linear_storage_connection.xml
%feature("docstring") cmf::water::LinearStorageConnection "

Calculates flux out of a storage as a linear function of its volume.

This connection serves the same purpose as the old kinematic_wave
connection, but the parameters are easier to explain



.. math::

     q = \\\\frac{V - V_{residual}}{t_r} 

where:  :math:`V` The actual
volume of water stored in source

:math:`V_{residual} [m^3]` The volume of water not flowing out (default = 0)
:math:`t_r` is the residence time in the source.

C++ includes: simple_connections.h ";

%feature("docstring")
cmf::water::LinearStorageConnection::LinearStorageConnection "LinearStorageConnection(WaterStorage::ptr source, flux_node::ptr
target, real residencetime, real residual=0.0)

Creates a linear storage connection or Nash-box.



.. math::

     q = \\\\frac{V - V_{residual}}{t_r}} 

Parameters:
-----------

source:  Water storage from which the water flows out. Flux is a
function of source.volume

target:  Target node (boundary condition or storage). Does not
influence the strength of the flow

residencetime:   :math:`t_r [days]` The residence time of the water in this
storage

residual:   :math:`V_{residual} [m^3]` The volume of water not flowing out
(default = 0) ";

%feature("docstring")  cmf::water::LinearStorageConnection::conc "real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::water::LinearStorageConnection::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::water::LinearStorageConnection::get_ptr "ptr get_ptr() const ";

%feature("docstring")  cmf::water::LinearStorageConnection::get_target
"flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::water::LinearStorageConnection::get_target
"flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::water::LinearStorageConnection::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::water::LinearStorageConnection::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::water::LinearStorageConnection::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::water::LinearStorageConnection::left_node
"flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::water::LinearStorageConnection::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::water::LinearStorageConnection::refresh "void refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")  cmf::water::LinearStorageConnection::right_node
"flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::water::LinearStorageConnection::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::water::LinearStorageConnection::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::water::LinearStorageConnection::short_string "virtual
std::string short_string() const ";

%feature("docstring")  cmf::water::LinearStorageConnection::to_string
"virtual std::string to_string() const ";


// File: classcmf_1_1atmosphere_1_1log__wind__profile.xml
%feature("docstring") cmf::atmosphere::log_wind_profile "

, A logarithmic wind profile

Todo Cite literature for this windprofile and insert equation

C++ includes: ET.h ";

%feature("docstring")
cmf::atmosphere::log_wind_profile::log_wind_profile "log_wind_profile(cmf::upslope::Cell &_cell) ";

%feature("docstring")
cmf::atmosphere::log_wind_profile::~log_wind_profile "virtual
~log_wind_profile() ";

%feature("docstring")
cmf::atmosphere::log_wind_profile::get_aerodynamic_resistance "void
get_aerodynamic_resistance(double &r_ag, double &r_ac, cmf::math::Time
t) const final

aerodynamic resistance from ground to atmosphere (r_ag) and from
canopy to atmosphere (r_ac) ";


// File: classcmf_1_1upslope_1_1connections_1_1_m_a_c_r_olike_macro_micro_exchange.xml
%feature("docstring")
cmf::upslope::connections::MACROlikeMacroMicroExchange "

This connection models the water exchange between macropores and
micropores as in the MACRO Model (Larsbo & Jarvis, 2003), which
follows Gerke & van Genuchten 1996.

WARNING:  Deprecated This connection uses the diffusivity of a soil
given by its retention curve. Since no retention curve provides a
valid value for Diffusivity in case of saturation this connection will
blow up the numerical solution for sure.

The exchange between Macropore and matrix is defined as follows:
(MACRO 5 Tech report, Larsbo & Jarvis 2003)



.. math::

    q = \\\\frac{G_f D_w \\\\gamma_w}{d^2}(\\\\theta_b -
    \\\\theta_{mi}) V_{layer}

where:  :math:`G_f` is the geometry factor.
Use 3 for a rectangular slab geometry

:math:`gamma_w` A scaling factor to fit analytical and numerical solution
(0.4)

:math:`d` is an effective diffusive path length related to aggregate size
and the influence of coatings on the aggregate surfaces in m

:math:`\\\\theta_b` the saturated water content of the matrix

:math:`\\\\theta_{mi}` the actual water content of the matrix

:math:`D_w = \\\\frac12(D(\\\\theta_b)+D(\\\\theta_{mi})W_{ma})` is the
effective water diffusivity in m2/day, as defined below  :math:`W_{ma}` is
the saturation of the macropores

C++ includes: macropore.h ";

%feature("docstring")
cmf::upslope::connections::MACROlikeMacroMicroExchange::MACROlikeMacroMicroExchange
"MACROlikeMacroMicroExchange(cmf::upslope::SoilLayer::ptr left,
cmf::upslope::MacroPore::ptr right, real _gamma_w=0.4, real _Gf=3) ";

%feature("docstring")
cmf::upslope::connections::MACROlikeMacroMicroExchange::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::MACROlikeMacroMicroExchange::exchange_target
"void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")
cmf::upslope::connections::MACROlikeMacroMicroExchange::get_ptr "ptr
get_ptr() const ";

%feature("docstring")
cmf::upslope::connections::MACROlikeMacroMicroExchange::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::MACROlikeMacroMicroExchange::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::MACROlikeMacroMicroExchange::get_tracer_filter
"real get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::MACROlikeMacroMicroExchange::get_tracer_filter
"real get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")
cmf::upslope::connections::MACROlikeMacroMicroExchange::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::MACROlikeMacroMicroExchange::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::connections::MACROlikeMacroMicroExchange::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::MACROlikeMacroMicroExchange::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::upslope::connections::MACROlikeMacroMicroExchange::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::MACROlikeMacroMicroExchange::set_tracer_filter
"void set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::MACROlikeMacroMicroExchange::set_tracer_filter
"void set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::connections::MACROlikeMacroMicroExchange::short_string "virtual std::string short_string() const ";

%feature("docstring")
cmf::upslope::connections::MACROlikeMacroMicroExchange::to_string "virtual std::string to_string() const ";


// File: classcmf_1_1upslope_1_1_macro_pore.xml
%feature("docstring") cmf::upslope::MacroPore "

An additional water storage for a soil layer to model matrix water and
macro pore water seperately.

Deprecated The MacroPore model is still very experimental and not
stable. Only for tryouts!

If present, the soil layer water storage holds the matrix water and
the MacroPore holds the water in the macro pore. Use
cmf::upslope::Macropore::create to create a macropore storage.

Use cmf::upslope::connections::GradientMacroFlow or
cmf::upslope::connections::KinematicMacroFlow to model water flow
between macro pores and a lateral connection ( lateral subsurface
fluxes) like cmf::upslope::connections::Richards_lateral to connect
the macro pore with the matrix.

C++ includes: macropore.h ";

/*  Overrides of flux_node  */

%feature("docstring")  cmf::upslope::MacroPore::RecalcFluxes "virtual
bool RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node.

WaterStorage overrides this, since state changes require an update of
the fluxes ";

%feature("docstring")  cmf::upslope::MacroPore::is_empty "virtual
double is_empty() const

Returns true if the node has no water. ";

%feature("docstring")  cmf::upslope::MacroPore::to_string "virtual
std::string to_string() const ";

%feature("docstring")  cmf::upslope::MacroPore::add_connected_states "virtual void add_connected_states(cmf::math::StateVariable::list
&states) ";

%feature("docstring")  cmf::upslope::MacroPore::conc "real conc(const
cmf::water::solute &_Solute) const

Returns the concentration of the given solute. ";

%feature("docstring")  cmf::upslope::MacroPore::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute) const
override

Returns the current WaterQuality (concentration of all solutes) ";

%feature("docstring")  cmf::upslope::MacroPore::conc "void conc(const
cmf::water::solute &_Solute, real NewConcetration)

Sets a new concentration. ";

%feature("docstring")  cmf::upslope::MacroPore::connection_to "cmf::water::flux_connection* connection_to(const cmf::water::flux_node
&target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::upslope::MacroPore::dxdt "virtual real
dxdt(const cmf::math::Time &time)

Returns the derivate of the state variable at time time. ";

%feature("docstring")  cmf::upslope::MacroPore::flux3d_to "cmf::geometry::point flux3d_to(const cmf::water::flux_node &target,
cmf::math::Time t) ";

%feature("docstring")  cmf::upslope::MacroPore::flux_to "real
flux_to(const cmf::water::flux_node &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from this into target\") ";

%feature("docstring")  cmf::upslope::MacroPore::get_3d_flux "cmf::geometry::point get_3d_flux(cmf::math::Time t)

Returns the sum of all flux vectors. ";

%feature("docstring")  cmf::upslope::MacroPore::get_abs_errtol "virtual real get_abs_errtol(real rel_errtol) const ";

%feature("docstring")  cmf::upslope::MacroPore::get_capacity "real
get_capacity() const

Returns the capacity of the macropores in m3.



.. math::

    V_{max} = \\\\Phi_{macro} A \\\\Delta z

where:  :math:`V_{max}` is
the water capacity of the macropore

:math:`\\\\Phi_{macro}` is the fraction of macro pores in the soil in :math:`m^3 macro pores/m^3 soil`

:math:`A \\\\Delta z` is the total volume of the soil layer (including all
pores) in :math:`m^3` ";

%feature("docstring")  cmf::upslope::MacroPore::get_cell "cmf::upslope::Cell& get_cell() const

The cell of this macropore. ";

%feature("docstring")  cmf::upslope::MacroPore::get_connections "cmf::water::connection_list get_connections() const ";

%feature("docstring")  cmf::upslope::MacroPore::get_crackwidth "virtual real get_crackwidth() const

Returns the crack width for a prismatic crackstructure.

For a prismatic crack structure, the porefraction in m3/m3 equals the
vertical crack area in m2/m2. The length of equally spaced cracks is
in one direction the inverse of the density and twice the length for
two directions. 

.. math::

     l_{crack} [m/m^2]= 2 \\\\frac {1}{d[m]}

If
we again ignore the fact that the spacing of the cracking crossings is
counted double, the crack width is: 

.. math::

     w_{crack}[m] =
    \\\\frac{A_{crack}[m^2/m^2]}{l_{crack}[m/m^2]} 

Combining both
eq. above: 

.. math::

     w_{crack}[m] = A_{crack}[m^2/m^2]\\\\frac{d[m]}{2}
    

";

%feature("docstring")  cmf::upslope::MacroPore::get_filled_fraction "real get_filled_fraction() const

Get the relative water content in the macro pore :math:`\\\\theta_{macro} = V_{macro}/V_{max}`. ";

%feature("docstring")  cmf::upslope::MacroPore::get_flowwidth "real
get_flowwidth() const

The approximate length of the aggregate boundaries.



.. math::

    l = \\\\frac{2}{d_{macro}} A

where:  :math:`l` is the length of
the aggregate boundaries (in m)

:math:`2` is the number of directions

:math:`d_{macro}` is the mean distance between macropores (density) in m

:math:`A` is the area of the cell ";

%feature("docstring")  cmf::upslope::MacroPore::get_K "virtual real
get_K() const

Returns the actual conductivity. ";

%feature("docstring")  cmf::upslope::MacroPore::get_K "virtual real
get_K(cmf::geometry::point direction) const

Returns the actual anisotropic conductivity along a direction :math:`K = (k_f \\\\cdot d) K`. ";

%feature("docstring")  cmf::upslope::MacroPore::get_layer "SoilLayer::ptr get_layer() const

Gets the soil layer (matrix water storage) for this macropore storage.
";

%feature("docstring")  cmf::upslope::MacroPore::get_porefraction "real get_porefraction() const

The fraction of the macro pores in m3/m3. This adds to the porosity of
the layer. ";

%feature("docstring")  cmf::upslope::MacroPore::get_potential "real
get_potential(cmf::math::Time t=cmf::math::never) const

Returns the actual water level in the macropore in m above reference.
";

%feature("docstring")  cmf::upslope::MacroPore::get_project "cmf::project& get_project() const

Returns the project, this node is part of. ";

%feature("docstring")  cmf::upslope::MacroPore::get_state "real
get_state() const

Returns the current state of the variable. ";

%feature("docstring")
cmf::upslope::MacroPore::get_state_variable_content "char
get_state_variable_content() const

A character indicating the integrated variable (either 'V' for Volume
or 'h' for head) ";

%feature("docstring")  cmf::upslope::MacroPore::get_volume "real
get_volume() const

Returns the actual stored volume in this macropore in m3. ";

%feature("docstring")  cmf::upslope::MacroPore::is_connected "virtual
bool is_connected(const cmf::math::StateVariable &other) const

Returns True if this waterstorage is effected by another state. ";

%feature("docstring")  cmf::upslope::MacroPore::is_storage "virtual
bool is_storage() const

Returns true, since this is a storage. ";

%feature("docstring")  cmf::upslope::MacroPore::remove_connection "bool remove_connection(cmf::water::flux_node::ptr To)

Remove the connection. ";

%feature("docstring")  cmf::upslope::MacroPore::set_potential "void
set_potential(real waterhead)

Sets the water level in the macropore. Be aware of not setting it
below the lower boundary. ";

%feature("docstring")  cmf::upslope::MacroPore::set_state "void
set_state(real newState)

Gives access to the state variable. ";

%feature("docstring")
cmf::upslope::MacroPore::set_state_variable_content "void
set_state_variable_content(char content)

A character indicating the integrated variable (either 'V' for Volume
or 'h' for head) ";

%feature("docstring")  cmf::upslope::MacroPore::set_volume "void
set_volume(real volume)

Sets the volume of stored water in m3. ";

%feature("docstring")  cmf::upslope::MacroPore::Solute "SoluteStorage& Solute(const cmf::water::solute _Solute)

Returns the water quality of the water storage. ";

%feature("docstring")  cmf::upslope::MacroPore::Solute "const
SoluteStorage& Solute(const cmf::water::solute _Solute) const ";

%feature("docstring")  cmf::upslope::MacroPore::waterbalance "real
waterbalance(cmf::math::Time t, const flux_connection *Without=0)
const

Returns the sum of all fluxes (positive and negative) at time t.

Single fluxes can be excluded from the calculation

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the waterbalance
(e.g. to prevent closed circuits) ";


// File: classcmf_1_1river_1_1_manning.xml
%feature("docstring") cmf::river::Manning "

Calculates the flux between two open water bodies, using Manning's
equation.

This is the base class for a kinematic wave approach (topography
driven) and a diffusive wave approach (water table driven). The only
difference between both approaches is the calculation of the flux
driving slope. For the model, one of ManningKinematic or
ManningDiffusive connection is selected 

.. math::

     v = R^{\\\\frac 2 3}
    \\\\frac{\\\\sqrt{\\\\Delta_z}}{n} \\\\\\\\ q = v \\\\cdot A 

Where:  :math:`A = \\\\frac V l`: Crosssectional area of the wetted
crossection, Volume per length

:math:`R = \\\\frac A{P(d)}`: The hydraulic radius

:math:`P(d)`: the perimeter of the wetted crosssection, a function of reach
depth

:math:`d(V)`: the depth of the reach, a function of the volume

:math:`\\\\Delta_z = \\\\frac{|z_1 - z_2|}{l}`: Slope of the reach

:math:`n`: Manning friction number

For the kinematic wave the slope of the river bed is used as slope:


.. math::

    \\\\Delta_z = \\\\frac{|z_1 - z_2|}{l}

while for the diffusive wave the slope is calculated from the actual
water head: 

.. math::

    \\\\Delta_z = \\\\frac{|h_1 - h_2|}{l}

C++ includes: ManningConnection.h ";

%feature("docstring")  cmf::river::Manning::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")  cmf::river::Manning::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::river::Manning::get_ptr "ptr get_ptr()
const ";

%feature("docstring")  cmf::river::Manning::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::river::Manning::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")  cmf::river::Manning::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")  cmf::river::Manning::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::river::Manning::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::river::Manning::left_node "flux_node::ptr
left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::river::Manning::q "real q(const flux_node
&inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::river::Manning::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")  cmf::river::Manning::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::river::Manning::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")  cmf::river::Manning::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")  cmf::river::Manning::short_string "virtual
std::string short_string() const ";

%feature("docstring")  cmf::river::Manning::to_string "virtual
std::string to_string() const ";


// File: classcmf_1_1river_1_1_manning___diffusive.xml
%feature("docstring") cmf::river::Manning_Diffusive "

Connecting surface water bodies using a diffusive wave.

This approach might not be numerical stable for deep water with small
gradient cmf for experimental reasons 

.. math::

    
    q_{Manning}&=& A R^{\\\\frac 2 3} \\\\sqrt{\\\\frac {\\\\Delta_z} n}
    \\\\\\\\ A &=& \\\\frac V l \\\\mbox{, (Crosssectional area of the
    wetted crossection, Volume per length)} \\\\\\\\ R &=& \\\\frac A
    {P(d)} \\\\\\\\ P(d) &=& \\\\mbox{ the perimeter of the wetted
    crosssection, a function of reach depth} \\\\\\\\ d(V) &=& \\\\mbox{
    the depth of the reach a function of the volume} \\\\\\\\ \\\\Delta_z
    = \\\\|\\\\frac{h_1 - h_2}{l} \\\\mbox{ Slope of the reach
    waterlevels} \\\\\\\\ n&=&\\\\mbox{Manning friction number}
    



C++ includes: ManningConnection.h ";

%feature("docstring")
cmf::river::Manning_Diffusive::Manning_Diffusive "Manning_Diffusive(cmf::river::OpenWaterStorage::ptr left,
cmf::water::flux_node::ptr right, const cmf::river::IChannel
&reachtype)

Creates a diffusive wave connection between to open water storages.

Parameters:
-----------

left:  right:  The nodes to be connected by the diffusive wave. Left
needs to be an open water storage

reachtype:  The channel geometry ";

%feature("docstring")  cmf::river::Manning_Diffusive::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")  cmf::river::Manning_Diffusive::exchange_target
"void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")  cmf::river::Manning_Diffusive::get_ptr "ptr
get_ptr() const ";

%feature("docstring")  cmf::river::Manning_Diffusive::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::river::Manning_Diffusive::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::river::Manning_Diffusive::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::river::Manning_Diffusive::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::river::Manning_Diffusive::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::river::Manning_Diffusive::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::river::Manning_Diffusive::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::river::Manning_Diffusive::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")  cmf::river::Manning_Diffusive::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::river::Manning_Diffusive::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::river::Manning_Diffusive::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")  cmf::river::Manning_Diffusive::short_string "virtual std::string short_string() const ";

%feature("docstring")  cmf::river::Manning_Diffusive::to_string "virtual std::string to_string() const ";


// File: classcmf_1_1river_1_1_manning___kinematic.xml
%feature("docstring") cmf::river::Manning_Kinematic "

Connecting surface water bodies using a kinematic wave.

Note the fixed gradient :math:`\\\\Delta_z` 

.. math::

    
    q_{Manning}&=& A R^{\\\\frac 2 3} \\\\sqrt{\\\\frac {\\\\Delta_z} n}
    \\\\\\\\ A &=& \\\\frac V l \\\\mbox{, (Crosssectional area of the
    wetted crossection, Volume per length)} \\\\\\\\ R &=& \\\\frac A
    {P(d)} \\\\\\\\ P(d) &=& \\\\mbox{ the perimeter of the wetted
    crosssection, a function of reach depth} \\\\\\\\ d(V) &=& \\\\mbox{
    the depth of the reach a function of the volume} \\\\\\\\ \\\\Delta_z
    &=& \\\\frac{\\\\|z_1 - z_2\\\\|}{l} \\\\mbox{ Slope of the reach}
    \\\\\\\\ n&=&\\\\mbox{Manning friction number} 



C++ includes: ManningConnection.h ";

%feature("docstring")
cmf::river::Manning_Kinematic::Manning_Kinematic "Manning_Kinematic(cmf::river::OpenWaterStorage::ptr left,
cmf::water::flux_node::ptr right, const cmf::river::IChannel
&reachtype)

Creates a kinematic wave connection between to open water storages.

Parameters:
-----------

left:  right:  The nodes to be connected by the kinematic wave. Left
needs to be an open water storage

reachtype:  The channel geometry ";

%feature("docstring")  cmf::river::Manning_Kinematic::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")  cmf::river::Manning_Kinematic::exchange_target
"void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")  cmf::river::Manning_Kinematic::get_ptr "ptr
get_ptr() const ";

%feature("docstring")  cmf::river::Manning_Kinematic::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::river::Manning_Kinematic::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::river::Manning_Kinematic::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::river::Manning_Kinematic::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::river::Manning_Kinematic::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::river::Manning_Kinematic::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::river::Manning_Kinematic::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::river::Manning_Kinematic::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")  cmf::river::Manning_Kinematic::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::river::Manning_Kinematic::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::river::Manning_Kinematic::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")  cmf::river::Manning_Kinematic::short_string "virtual std::string short_string() const ";

%feature("docstring")  cmf::river::Manning_Kinematic::to_string "virtual std::string to_string() const ";


// File: classcmf_1_1maps_1_1_map.xml
%feature("docstring") cmf::maps::Map "

A Map is the base class for different spatial data distributions. The
base version contains returns always the default value  A Map should
implement the following functions: __iter__, returns an iterator over
the items of a map values(), returns an iterator over the different
objects of a map __call__(x,y,z), returns the object belonging to the
position x,y,z ";

%feature("docstring")  cmf::maps::Map::__init__ "def __init__(self,
default=None) ";

%feature("docstring")  cmf::maps::Map::__call__ "def __call__(self,
x, y, z=0)

returns default, regardless of the position given ";

%feature("docstring")  cmf::maps::Map::__iter__ "def __iter__(self)
";

%feature("docstring")  cmf::maps::Map::__nonzero__ "def
__nonzero__(self) ";

%feature("docstring")  cmf::maps::Map::values "def values(self) ";


// File: classcmf_1_1upslope_1_1connections_1_1_matrix_infiltration.xml
%feature("docstring") cmf::upslope::connections::MatrixInfiltration "

Connects the surfacewater and the most upper layer using a Richards
equation like infiltration model.

The potential infiltration is calculated according to the Richards
equation. The gradient is from the cell surface to the center of the
first layer and the conductivity is the geometric mean of the wetted
surface ( :math:`K_{sat}`) and the conductivity of the layer center (
:math:`K(\\\\theta_{layer})` 

.. math::

     q_{max} &=&
    \\\\frac{\\\\Psi_{surface} - \\\\Psi_{soil}}{\\\\Delta z} K A_{cell}
    \\\\\\\\ K &=& \\\\sqrt{K\\\\left(\\\\theta_{layer}\\\\right)K_{sat}}
    \\\\\\\\ \\\\Delta z &=& z_{cell} - z_{layer center}
    



If the surface water is modeled by a distinct water storage, the
actual infiltration is given as the product of the potential
infiltration with the coverage of the surface water
cmf::upslope::Cell::surface_water_coverage 

.. math::

    q_{act} = q_{max}
    \\\\frac{A_{water}}{A_{cell}}

If the surface water is no storage on its own, but just a water
distribution node, the actual infiltration is the minimum of the
potential infiltration and the current inflow (rain, snow melt) to the
surface 

.. math::

    q_{act} = \\\\min\\\\left(q_{max},
    \\\\sum{q_{in,surfacewater}}\\\\right)

C++ includes: infiltration.h ";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::MatrixInfiltration "MatrixInfiltration(cmf::upslope::SoilLayer::ptr soilwater,
cmf::water::flux_node::ptr surfacewater) ";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::get_ptr "ptr get_ptr()
const ";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::get_tracer_filter "real get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::get_tracer_filter "real get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::set_tracer_filter "void set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::set_tracer_filter "void set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::short_string "virtual
std::string short_string() const ";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::to_string "virtual
std::string to_string() const ";


// File: classcmf_1_1river_1_1_mean_channel.xml
%feature("docstring") cmf::river::MeanChannel "

A combination of two channel geometries.

C++ includes: ReachType.h ";

%feature("docstring")  cmf::river::MeanChannel::MeanChannel "MeanChannel(const IChannel &channel1, const IChannel &channel2)

Creates the mean geometry from the two channel geometries. ";

%feature("docstring")  cmf::river::MeanChannel::MeanChannel "MeanChannel(const MeanChannel &meanChannel) ";

%feature("docstring")  cmf::river::MeanChannel::~MeanChannel "virtual
~MeanChannel() ";

%feature("docstring")  cmf::river::MeanChannel::A "virtual double
A(double V) const

Returns the area of the surface for a given volume. ";

%feature("docstring")  cmf::river::MeanChannel::copy "MeanChannel*
copy() const ";

%feature("docstring")  cmf::river::MeanChannel::get_channel_width "double get_channel_width(double depth) const

Calculates the flow width from a given actual depth [m] using the
actual IChannel geometry. ";

%feature("docstring")  cmf::river::MeanChannel::get_depth "double
get_depth(double area) const

Calculates the actual depth of the reach using the IChannel geometry.

get_depth of the reach [m]

Parameters:
-----------

area:  Wetted area of a river cross section [m2], can be obtained by
V/l, where V is the stored volume and l is the reach length ";

%feature("docstring")  cmf::river::MeanChannel::get_flux_crossection "double get_flux_crossection(double depth) const

Calculates the wetted area from a given depth using the IChannel
geometry.

In most cases use get_flux_crossection=V/l, where V is the stored
volume and l is the reach length Wetted area of a river cross section
[m2]

Parameters:
-----------

depth:  depth of the reach [m] ";

%feature("docstring")  cmf::river::MeanChannel::get_length "double
get_length() const

Length of the reach. ";

%feature("docstring")  cmf::river::MeanChannel::get_nManning "virtual
double get_nManning() const ";

%feature("docstring")  cmf::river::MeanChannel::get_wetted_perimeter "double get_wetted_perimeter(double depth) const

Calculates the wetted perimeter from a given actual depth [m] using
the actual IChannel geometry. ";

%feature("docstring")  cmf::river::MeanChannel::h "virtual double
h(double V) const

Returns the depth of a given volume. ";

%feature("docstring")  cmf::river::MeanChannel::q "virtual double
q(double h, double slope) const ";

%feature("docstring")  cmf::river::MeanChannel::qManning "virtual
double qManning(double A, double slope) const

Calculates the flow rate from a given water volume in the reach


.. math::

     q_{Manning}&=& A R^{\\\\frac 2 3}
    \\\\sqrt{\\\\frac {\\\\Delta_z} n} \\\\\\\\ A &=& \\\\frac V l
    \\\\mbox{, (Crosssectional area of the wetted crossection, Volume per
    length)} \\\\\\\\ R &=& \\\\frac A {P(d)} \\\\\\\\ P(d) &=& \\\\mbox{
    the perimeter of the wetted crosssection, a function of reach depth}
    \\\\\\\\ d(V) &=& \\\\mbox{ the depth of the reach a function of the
    volume} \\\\\\\\ \\\\Delta_z &=& \\\\frac{z_{max} - z_{min}}{l}
    \\\\mbox{ Slope of the reach} 

.

Flow rate [m3/s]

Parameters:
-----------

A:  The area of the cross section [m2]

slope:  The slope of the reach [m/m] ";

%feature("docstring")  cmf::river::MeanChannel::set_nManning "virtual
void set_nManning(double nManning) ";

%feature("docstring")  cmf::river::MeanChannel::typecode "char
typecode() const ";

%feature("docstring")  cmf::river::MeanChannel::V "virtual double
V(double h) const ";


// File: classcmf_1_1atmosphere_1_1_meteorology.xml
%feature("docstring") cmf::atmosphere::Meteorology "

An abstract class, for objects generating Weather records at a
specific time.

C++ includes: meteorology.h ";

%feature("docstring")  cmf::atmosphere::Meteorology::~Meteorology "virtual ~Meteorology() ";

%feature("docstring")  cmf::atmosphere::Meteorology::copy "virtual
Meteorology* copy() const =0

Returns a copy of the meteorology object. Pure virtual function, needs
to be implemented. ";

%feature("docstring")
cmf::atmosphere::Meteorology::get_instrument_height "virtual real
get_instrument_height() const =0

Returns the height of the instruments above canopy. ";

%feature("docstring")  cmf::atmosphere::Meteorology::get_weather "virtual cmf::atmosphere::Weather get_weather(cmf::math::Time t) const
=0

Returns the Weather at time t. Pure virtual function. Must get
implemented by child functions. ";


// File: classcmf_1_1atmosphere_1_1_meteo_station.xml
%feature("docstring") cmf::atmosphere::MeteoStation "

A meteorological station holding timeseries to create Weather records.

In order to calculate ETpot with cmf a big amount of meteorological
data is needed, more data than usually available. The MeteoStation
class can estimate missing data from a minimal set. As more data, as
one provides, the better the calculation of ETpot becomes. The minimal
data needed is Tmin and Tmax (daily) and precipitation. To calculate
the global radiation (although measured global radiation could be
inserted), the position of meteorological station in geographic
coordinates has to be set.

A meteorological station is created by
cmf::atmosphere::MeteoStationList::add_station . Usage from python:

There are two modes for the meteorology: daily=true and daily=false.
If daily=true, Radiation is given as a daily mean value. If
daily=false, Radiation is given as an hourly mean value, which shows
the dial ETpot variation but results in erronous results if the
timestep is daily.

C++ includes: meteorology.h ";

/*  Location and behaviour properties  */

%feature("docstring")  cmf::atmosphere::MeteoStation::get_position "cmf::geometry::point get_position() const ";

/*  Data access methods  */

%feature("docstring")  cmf::atmosphere::MeteoStation::get_data "cmf::atmosphere::Weather get_data(cmf::math::Time t, double height)
const

Returns the current Atmosphere state. Uses default values for missing
timeseries. ";

%feature("docstring")  cmf::atmosphere::MeteoStation::use_for_cell "void use_for_cell(cmf::upslope::Cell &c)

Connects this meteostation as a meteo data provider with the cell. ";

%feature("docstring")
cmf::atmosphere::MeteoStation::SetSunshineFraction "void
SetSunshineFraction(cmf::math::timeseries sunshine_duration)

Calculates a timeseries of the sunshine fraction (to put into
Sunshine) from a timeseries of absolute sunshine duration

seehttp://www.fao.org/docrep/X0490E/x0490e07.htm#radiation


.. math::

     \\\\phi &=& \\\\frac{(\\\\mbox{geogr.
    Latitude})^\\\\circ \\\\pi}{180^\\\\circ} \\\\mbox{ Latitude in }rad
    \\\\\\\\ \\\\delta &=& 0.409 \\\\sin\\\\left(\\\\frac{2\\\\pi}{365}DOY
    - 1.39\\\\right) \\\\mbox{ Declination, DOY is day of year}\\\\\\\\
    \\\\omega_s &=& \\\\arccos(-\\\\tan\\\\phi\\\\tan\\\\delta) \\\\mbox{
    Sunset angle in }rad \\\\\\\\ N &=& \\\\frac{24}{\\\\pi}\\\\omega_s
    \\\\mbox{ potential duration of sunshine in }h \\\\\\\\ \\\\frac n N
    &=& n\\\\mbox{ absolute sunshine duration in }h 

 ";

/*  Timeseries of meteorological data  */

%feature("docstring")  cmf::atmosphere::MeteoStation::MeteoStation "MeteoStation(const cmf::atmosphere::MeteoStation &other)

Copy c'tor. ";


// File: classcmf_1_1atmosphere_1_1_meteo_station_list.xml
%feature("docstring") cmf::atmosphere::MeteoStationList "

A list of meteorological stations.

Can find the nearest station for a position and calculate the
temperature lapse

C++ includes: meteorology.h ";

%feature("docstring")
cmf::atmosphere::MeteoStationList::MeteoStationList "MeteoStationList()

Create empty list. ";

%feature("docstring")
cmf::atmosphere::MeteoStationList::MeteoStationList "MeteoStationList(const MeteoStationList &copy)

Copy c'tor. ";

%feature("docstring")  cmf::atmosphere::MeteoStationList::add_station
"MeteoStation::ptr add_station(std::string name, cmf::geometry::point
position, double latitude=51.0, double longitude=8.0, double tz=1.0,
cmf::math::Time startTime=cmf::math::Time(1, 1, 2001), cmf::math::Time
timestep=cmf::math::day)

Creates a meteorological station at a certain position and adds it to
the list.

Parameters:
-----------

name:  Name of the station

position:  The location of the station in map coordinates

latitude:  Latitude of the study area (for solar radiation)

longitude:  Longitude of the study area (for solar time)

tz:  Time zone of the study area (e.g Germany +1,U.S. Pacific time -8

startTime:  Date of the beginning of the climatic data (may be changed
for each time series later)

timestep:  Frequency of climatic data (may be changed for each time
series later) ";

%feature("docstring")
cmf::atmosphere::MeteoStationList::calculate_Temp_lapse "double
calculate_Temp_lapse(cmf::math::Time begin, cmf::math::Time step,
cmf::math::Time end)

Calculates the temperature lapse from all stations in the list and
sets the T_lapse attribute of each station.

Returns the average lapse over the whole period. ";

%feature("docstring")
cmf::atmosphere::MeteoStationList::reference_to_nearest "MeteoStationReference reference_to_nearest(const cmf::geometry::point
&position, double z_weight=0) const

Creates a MeteoStationReference from the nearest station to position
at position.

The distance is calculated as :math:`d=\\\\sqrt{(x_{s} - x_{l})^2 + (y_{s} - y_{l})^2} + \\\\lambda_z\\\\|z_{s} - z_{l}\\\\|` Where :math:`s` is the
station and :math:`l` is the locatable A Meteorology using the data of the
nearest station to position

Parameters:
-----------

position:  The position (any locatable, like e.g. Cell possible) to
look for the station. The reference should be owned by the locatable

z_weight:  The weight of the height difference :math:`\\\\lambda_z` ";

%feature("docstring")
cmf::atmosphere::MeteoStationList::remove_station "ptrdiff_t
remove_station(ptrdiff_t index)

Removes a station and returns the number of remaining references to
the removed station. If the station is deleted, 0 is returned. ";

%feature("docstring")  cmf::atmosphere::MeteoStationList::size "size_t size() const

Returns the number of stations. ";


// File: classcmf_1_1atmosphere_1_1_meteo_station_reference.xml
%feature("docstring") cmf::atmosphere::MeteoStationReference "

A reference to a meteorological station.

Returns the weather at a given time for its place using
MeteoStation::T_lapse

C++ includes: meteorology.h ";

%feature("docstring")
cmf::atmosphere::MeteoStationReference::MeteoStationReference "MeteoStationReference(MeteoStation::ptr station, cmf::geometry::point
location)

Create a located reference to a meteo station.

Parameters:
-----------

station:   MeteoStation

location:  Location of the reference ";

%feature("docstring")
cmf::atmosphere::MeteoStationReference::MeteoStationReference "MeteoStationReference(const MeteoStationReference &copy) ";

%feature("docstring")  cmf::atmosphere::MeteoStationReference::copy "MeteoStationReference* copy() const

Returns a copy of the meteorology object. Pure virtual function, needs
to be implemented. ";

%feature("docstring")
cmf::atmosphere::MeteoStationReference::get_instrument_height "real
get_instrument_height() const

Creates a reference for a MeteoStation at a location. ";

%feature("docstring")
cmf::atmosphere::MeteoStationReference::get_position "cmf::geometry::point get_position() const

Returns the position of the reference. ";

%feature("docstring")
cmf::atmosphere::MeteoStationReference::get_station "MeteoStation::ptr get_station() const

Returns the station referenced. ";

%feature("docstring")
cmf::atmosphere::MeteoStationReference::get_weather "cmf::atmosphere::Weather get_weather(cmf::math::Time t) const

Returns the weather at the time t. ";


// File: classcmf_1_1math_1_1_multi_integrator.xml
%feature("docstring") cmf::math::MultiIntegrator "

The MultiIntegrator is a wrapper for a bunch integrators. The states
of the integrators should not have direct connections over integrator
boundaries.

C++ includes: multiintegrator.h ";

/*  Accuracy parameters  */

/*  model time  */

%feature("docstring")  cmf::math::MultiIntegrator::get_t "cmf::math::Time get_t() const

Returns the current model time. ";

%feature("docstring")  cmf::math::MultiIntegrator::set_t "void
set_t(cmf::math::Time val)

Sets the current model time. ";

%feature("docstring")  cmf::math::MultiIntegrator::get_dt "cmf::math::Time get_dt() const

Returns the last time step. ";

/*  Integrate  */

%feature("docstring")  cmf::math::MultiIntegrator::integrate_until "void integrate_until(cmf::math::Time t_max, cmf::math::Time dt=Time(),
bool reset=false)

Integrates the vector of state variables until t_max.

Parameters:
-----------

t_max:   Time, the solver should run to

dt:   Time step (may be omitted)

reset:  If true, solver is reseted before integration starts ";

%feature("docstring")  cmf::math::MultiIntegrator::MultiIntegrator "MultiIntegrator(const cmf::math::Integrator &template_integrator, int
count)

Creates a new MultiIntegrator.

Parameters:
-----------

template_integrator:  Template for the integrators

count:  Number of integrators ";

%feature("docstring")  cmf::math::MultiIntegrator::copy "virtual
cmf::math::MultiIntegrator* copy() const

Polymorphic copy constructor. ";

%feature("docstring")  cmf::math::MultiIntegrator::integrate "int
integrate(cmf::math::Time t_max, cmf::math::Time dt)

Integrates the vector of state variables.

Parameters:
-----------

t_max:  To stop the model (if running in a model framework) at time
steps of value exchange e.g. full hours, the next value exchange time
can be given

dt:  Takes the proposed time step, and changes it into the effectively
used time step according to the local stiffness of the problem and
MaxTime ";

%feature("docstring")  cmf::math::MultiIntegrator::reset "virtual
void reset()

Resets the integrator. ";

%feature("docstring")  cmf::math::MultiIntegrator::set_system "void
set_system(const state_list &states) ";

%feature("docstring")  cmf::math::MultiIntegrator::size "size_t
size() const ";

%feature("docstring")  cmf::math::MultiIntegrator::to_string "std::string to_string() const override

Desrcibes the iterator. ";


// File: classcmf_1_1maps_1_1_nearest_neighbor_map.xml
%feature("docstring") cmf::maps::NearestNeighborMap "

A map (spatial distribution of data) returning the nearest neighbor to
the queried position Stores position referenced objects.  z_weight is
a weight, how important vertical differences are for neighborhood. 0
means only horizontal distance and a high value only uses the height
as a distance measure. The distance to be minimized is calculated as:
sqrt((x1-x2)**2+(y1-y2)**2)+abs(z1-z2)*z_weight ";

%feature("docstring")  cmf::maps::NearestNeighborMap::__init__ "def
__init__(self, z_weight=0) ";

%feature("docstring")  cmf::maps::NearestNeighborMap::__call__ "def
__call__(self, x, y, z=0)

returns the nearest neighbor object to the given position     The
distance to be minimized is calculated as:
sqrt((x1-x2)**2+(y1-y2)**2)+abs(z1-z2)*z_weight ";

%feature("docstring")  cmf::maps::NearestNeighborMap::__iter__ "def
__iter__(self) ";

%feature("docstring")  cmf::maps::NearestNeighborMap::__nonzero__ "def __nonzero__(self) ";

%feature("docstring")  cmf::maps::NearestNeighborMap::append "def
append(self, position, object) ";

%feature("docstring")  cmf::maps::NearestNeighborMap::remove "def
remove(self, position) ";

%feature("docstring")  cmf::maps::NearestNeighborMap::values "def
values(self) ";


// File: classcmf_1_1upslope_1_1neighbor__iterator.xml
%feature("docstring") cmf::upslope::neighbor_iterator "

A class to iterate through the neighbors of a cell (const). Not needed
from the Python side, use the generator cell.neighbors instead.

C++ includes: Topology.h ";

%feature("docstring")
cmf::upslope::neighbor_iterator::neighbor_iterator "neighbor_iterator(cmf::upslope::Topology &topo) ";

%feature("docstring")
cmf::upslope::neighbor_iterator::neighbor_iterator "neighbor_iterator(cmf::upslope::Cell *cell) ";

%feature("docstring")  cmf::upslope::neighbor_iterator::cell "Cell&
cell() ";

%feature("docstring")  cmf::upslope::neighbor_iterator::flowwidth "double flowwidth() ";

%feature("docstring")  cmf::upslope::neighbor_iterator::next_neighbor
"neighbor_iterator& next_neighbor()

Points the iterator to the next neighbor. ";

%feature("docstring")  cmf::upslope::neighbor_iterator::valid "bool
valid() const ";


// File: classcmf_1_1water_1_1_neumann_boundary.xml
%feature("docstring") cmf::water::NeumannBoundary "

A Neumann boundary condition (constant flux boundary condition)

The flux is a timeseries, but can be used as a scalar. To scale the
timeseries to the specific conditions of this boundary condition the
linear_scale flux_scale can be used.

C++ includes: boundary_condition.h ";

%feature("docstring")  cmf::water::NeumannBoundary::NeumannBoundary "NeumannBoundary(cmf::project &_project, cmf::math::timeseries _flux,
cmf::water::SoluteTimeseries
_concentration=cmf::water::SoluteTimeseries(), cmf::geometry::point
loc=cmf::geometry::point())

Ctor of the Neumann boundary.

Parameters:
-----------

_project:  The project this boundary condition belongs to

_flux:  The flux timeseries (a scalar is converted to a timeseries
automatically)

_concentration:  The concentration timeseries

loc:  The location of the boundary condition ";

%feature("docstring")  cmf::water::NeumannBoundary::NeumannBoundary "NeumannBoundary(cmf::project &_project, cmf::geometry::point
loc=cmf::geometry::point()) ";

%feature("docstring")  cmf::water::NeumannBoundary::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute) const

Returns the solute concentrations of the flux at a given time. ";

%feature("docstring")  cmf::water::NeumannBoundary::connect_to "void
connect_to(cmf::water::flux_node::ptr target) ";

%feature("docstring")  cmf::water::NeumannBoundary::connection_to "cmf::water::flux_connection* connection_to(const cmf::water::flux_node
&target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::water::NeumannBoundary::flux3d_to "cmf::geometry::point flux3d_to(const cmf::water::flux_node &target,
cmf::math::Time t) ";

%feature("docstring")  cmf::water::NeumannBoundary::flux_to "real
flux_to(const cmf::water::flux_node &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from this into target\") ";

%feature("docstring")  cmf::water::NeumannBoundary::get_3d_flux "cmf::geometry::point get_3d_flux(cmf::math::Time t)

Returns the sum of all flux vectors. ";

%feature("docstring")  cmf::water::NeumannBoundary::get_connections "cmf::water::connection_list get_connections() const ";

%feature("docstring")  cmf::water::NeumannBoundary::get_flux "cmf::math::timeseries get_flux()

The timeseries of the boundary flux. ";

%feature("docstring")  cmf::water::NeumannBoundary::get_potential "virtual real get_potential(cmf::math::Time=cmf::math::never) const

Returns the water potential of the node in m waterhead.

The base class water storage always returns the height of the location
";

%feature("docstring")  cmf::water::NeumannBoundary::get_project "cmf::project& get_project() const

Returns the project, this node is part of. ";

%feature("docstring")  cmf::water::NeumannBoundary::is_empty "double
is_empty() const

Returns true if the node has no water. ";

%feature("docstring")  cmf::water::NeumannBoundary::is_storage "virtual bool is_storage() const

true, if this is a waterstorage ";

%feature("docstring")  cmf::water::NeumannBoundary::RecalcFluxes "bool RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node.

WaterStorage overrides this, since state changes require an update of
the fluxes ";

%feature("docstring")  cmf::water::NeumannBoundary::remove_connection
"bool remove_connection(cmf::water::flux_node::ptr To)

Remove the connection. ";

%feature("docstring")  cmf::water::NeumannBoundary::set_flux "void
set_flux(cmf::math::timeseries new_flux)

Set a timeseries as the boundary flux. ";

%feature("docstring")  cmf::water::NeumannBoundary::set_flux "void
set_flux(double new_flux)

Set a constant as the boundary flux. ";

%feature("docstring")  cmf::water::NeumannBoundary::set_potential "virtual void set_potential(real new_potential)

Sets the potential of this flux node. ";

%feature("docstring")  cmf::water::NeumannBoundary::to_string "virtual std::string to_string() const ";

%feature("docstring")  cmf::water::NeumannBoundary::waterbalance "real waterbalance(cmf::math::Time t, const flux_connection *Without=0)
const

Returns the sum of all fluxes (positive and negative) at time t.

Single fluxes can be excluded from the calculation

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the waterbalance
(e.g. to prevent closed circuits) ";


// File: classcmf_1_1water_1_1_neumann_boundary__list.xml
%feature("docstring") cmf::water::NeumannBoundary_list "

Provides fast access to Neumann boundaries for flux update.

If many Neumann boundary conditions are present in a project, a fast
data exchange to update the fluxes might be needed.

With this specialized list a num_array can be passed to the boundary
conditions for a fast flux update If a multiple system layout for the
cmf setup is chosen, we might have a node_list Dirichlet boundary
conditions (dbc), a corresponding NeumannBoundary_list (nbc) of
Neumann boundaries and a node_list containing the storages connected
with the NeumannBoundary_list (storages). The fast data exchange is
written in Python as:

C++ includes: collections.h ";

%feature("docstring")
cmf::water::NeumannBoundary_list::NeumannBoundary_list "NeumannBoundary_list() ";

%feature("docstring")
cmf::water::NeumannBoundary_list::NeumannBoundary_list "NeumannBoundary_list(const cmf::water::node_list &copy) ";

%feature("docstring")
cmf::water::NeumannBoundary_list::NeumannBoundary_list "NeumannBoundary_list(const NeumannBoundary_list &copy) ";

%feature("docstring")  cmf::water::NeumannBoundary_list::append "void
append(NeumannBoundary::ptr nbc)

Appends a neumann boundary to this list. ";

%feature("docstring")  cmf::water::NeumannBoundary_list::get "NeumannBoundary::ptr get(ptrdiff_t index) const

Returns the Neumann boundary condition at position index.

From Python you can use [] ";

%feature("docstring")  cmf::water::NeumannBoundary_list::get_fluxes "cmf::math::num_array get_fluxes(cmf::math::Time t=cmf::math::Time())
const

Returns the fluxes of the items as an array.

get_fluxes and set_fluxes are wrapped with the Python property fluxes
";

%feature("docstring")
cmf::water::NeumannBoundary_list::global_water_balance "real
global_water_balance(cmf::math::Time t) const

Returns the sum of the water balances of the nodes


.. math::

    \\\\sigma_{global} =
    \\\\sum_{i=0}^N{\\\\sum_{j=0}^{C_i}{q_{ij}(t)}} 

.

Replaces slow Python code like: ";

%feature("docstring")  cmf::water::NeumannBoundary_list::set_fluxes "void set_fluxes(cmf::math::num_array values)

Sets the fluxes of the items from an array.

get_fluxes and set_fluxes are wrapped with the Python property fluxes
";

%feature("docstring")  cmf::water::NeumannBoundary_list::size "size_t
size() const

returns the number of stored boundary conditions ";

%feature("docstring")  cmf::water::NeumannBoundary_list::to_node_list
"cmf::water::node_list to_node_list() const

Creates a node_list from this NeumannBoundary_list. ";

%feature("docstring")  cmf::water::NeumannBoundary_list::water_balance
"cmf::math::num_array water_balance(cmf::math::Time t) const

Returns the water balance of each vector as a vector 

.. math::

     \\\\sigma_i
    = \\\\sum_{j=0}^{C_i}{q_{ij}(t)} 

.

Replaces slow Python code like: ";


// File: classcmf_1_1water_1_1_neumann_flux.xml
%feature("docstring") cmf::water::NeumannFlux "

Connection between Neumann-boundary and a flux node.

This flux_connection is created, when connecting a Neumann boundary
condition with a state variable using Neumann::connect_to

C++ includes: boundary_condition.h ";

%feature("docstring")  cmf::water::NeumannFlux::NeumannFlux "NeumannFlux(std::shared_ptr< NeumannBoundary > left,
cmf::water::flux_node::ptr right) ";

%feature("docstring")  cmf::water::NeumannFlux::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")  cmf::water::NeumannFlux::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::water::NeumannFlux::get_ptr "ptr
get_ptr() const ";

%feature("docstring")  cmf::water::NeumannFlux::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::water::NeumannFlux::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")  cmf::water::NeumannFlux::get_tracer_filter "real get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")  cmf::water::NeumannFlux::get_tracer_filter "real get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::water::NeumannFlux::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::water::NeumannFlux::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::water::NeumannFlux::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::water::NeumannFlux::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")  cmf::water::NeumannFlux::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::water::NeumannFlux::set_tracer_filter "void set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")  cmf::water::NeumannFlux::set_tracer_filter "void set_tracer_filter(solute S, real value) ";

%feature("docstring")  cmf::water::NeumannFlux::short_string "virtual
std::string short_string() const ";

%feature("docstring")  cmf::water::NeumannFlux::to_string "virtual
std::string to_string() const ";


// File: classcmf_1_1water_1_1node__list.xml
%feature("docstring") cmf::water::node_list "

A collection of nodes for fast access of the waterbalance.

In setups with many storages and rather fast computations, the speed
of data access for output generation can take a high portion of the
total run time. To accelerate data access, one can use the node_list
object

Todo Add a get_volume / set_volume function pair, to complement
get_potential / set_potential

C++ includes: collections.h ";

%feature("docstring")  cmf::water::node_list::node_list "node_list()

Creates an empty node_lust. ";

%feature("docstring")  cmf::water::node_list::node_list "node_list(const cmf::water::node_list &forcopy)

Copy the node_list. ";

%feature("docstring")  cmf::water::node_list::~node_list "virtual
~node_list() ";

%feature("docstring")  cmf::water::node_list::append "void
append(flux_node::ptr node)

Adds a flux node to the list. ";

%feature("docstring")  cmf::water::node_list::begin "node_vector::iterator begin() ";

%feature("docstring")  cmf::water::node_list::begin "node_vector::const_iterator begin() const ";

%feature("docstring")  cmf::water::node_list::conc "cmf::math::num_array conc(cmf::math::Time t, const cmf::water::solute
&_Solute) const

Returns an array holding the concentration of all the flux nodes for
the given solute. ";

%feature("docstring")  cmf::water::node_list::end "node_vector::iterator end() ";

%feature("docstring")  cmf::water::node_list::end "node_vector::const_iterator end() const ";

%feature("docstring")  cmf::water::node_list::get "flux_node::ptr
get(ptrdiff_t index) const

Returns a node in the node_list. ";

%feature("docstring")  cmf::water::node_list::get_fluxes3d "cmf::geometry::point_vector get_fluxes3d(cmf::math::Time t) const

Returns the current flow vector for each node. ";

%feature("docstring")  cmf::water::node_list::get_fluxes3d_to "cmf::geometry::point_vector get_fluxes3d_to(const
cmf::water::node_list &targets, cmf::math::Time t) const

Returns the flux vectors to the nodes of a given target node_list. ";

%feature("docstring")  cmf::water::node_list::get_fluxes_to "cmf::math::num_array get_fluxes_to(const cmf::water::node_list
&targets, cmf::math::Time t) const

A fast method to perform flux queries as a batch.

The node lists left and right should have the same length.

The vector containing the flux from left to right at the same position

Parameters:
-----------

targets:  A node_list containing the source nodes

t:  The time for the fluxes

Replaces slow Python code like: ";

%feature("docstring")  cmf::water::node_list::get_positions "cmf::geometry::point_vector get_positions() const

Returns the positions of the nodes. ";

%feature("docstring")  cmf::water::node_list::get_potentials "cmf::math::num_array get_potentials(cmf::math::Time
t=cmf::math::never)

Returns the potential of the nodes. ";

%feature("docstring")  cmf::water::node_list::getslice "node_list
getslice(ptrdiff_t begin, ptrdiff_t end, ptrdiff_t step=1) const

Returns a slice of the node_list. ";

%feature("docstring")  cmf::water::node_list::global_water_balance "real global_water_balance(cmf::math::Time t) const

Returns the sum of the water balances of the nodes.



.. math::

    \\\\sigma_{global} =
    \\\\sum_{i=0}^N{\\\\sum_{j=0}^{C_i}{q_{ij}(t)}} 

Replaces slow Python code like: ";

%feature("docstring")  cmf::water::node_list::remove "bool
remove(flux_node::ptr node)

Removes a flux node from the list, returns true if successful. ";

%feature("docstring")  cmf::water::node_list::set_potentials "ptrdiff_t set_potentials(const cmf::math::num_array &potentials)

Sets the potentials of the node_list.

If nodes do not have changeable potentials, they are skipped silently
The number of nodes with changed potential ";

%feature("docstring")  cmf::water::node_list::set_solute_source "ptrdiff_t set_solute_source(const cmf::water::solute &_Solute,
cmf::math::num_array source_fluxes)

Sets the source flux of a solute storage associated with a node (node
has to be a water storage) ";

%feature("docstring")  cmf::water::node_list::size "size_t size()
const

The number of nodes. ";

%feature("docstring")  cmf::water::node_list::water_balance "cmf::math::num_array water_balance(cmf::math::Time t) const

Returns the water balance of each vector as a vector.



.. math::

     \\\\sigma_i = \\\\sum_{j=0}^{C_i}{q_{ij}(t)} 

Replaces slow Python code like: ";


// File: classcmf_1_1math_1_1root__finding_1_1not__finite__error.xml
%feature("docstring") cmf::math::root_finding::not_finite_error "";

%feature("docstring")
cmf::math::root_finding::not_finite_error::not_finite_error "not_finite_error(const std::string &msg) ";


// File: classcmf_1_1water_1_1_null_adsorption.xml
%feature("docstring") cmf::water::NullAdsorption "

A class for tracers without interaction with the storage container.
freesolute returns xt.

C++ includes: adsorption.h ";

%feature("docstring")  cmf::water::NullAdsorption::~NullAdsorption "virtual ~NullAdsorption() ";

%feature("docstring")  cmf::water::NullAdsorption::copy "virtual
NullAdsorption* copy(real m=-1) const

returns a copy of the Adsorption object.

If the adsorption is depending on the sorbent mass, you can give a
positive value for the sorbent mass m. If the value is not given or
negative, m is used from the original object. ";

%feature("docstring")  cmf::water::NullAdsorption::freesolute "virtual real freesolute(real xt, real V) const

Returns the mass of dissolved tracer as a function of the total tracer
mass in the solute storage and the water volume.

Parameters:
-----------

xt:   :math:`x_t` the total tracer mass in the storage

V:   :math:`V m^3` the water volume in the storage

:math:`x_f` the dissolved mass of the tracer ";

%feature("docstring")  cmf::water::NullAdsorption::totalsolute "virtual real totalsolute(real xf, real V) const

Returns the total mass of the tracer from the dissolved concetration
in tracer unit/m3.

Parameters:
-----------

xf:   :math:`x_f` the dissolved tracer mass in the storage

V:   :math:`V m^3` the water volume in the storage

:math:`x_t` the total mass of the tracer ";


// File: classcmf_1_1math_1_1num__array.xml
%feature("docstring") cmf::math::num_array "

A valarray kind of vector implementation with OpenMP capabilities.

C++ includes: num_array.h ";

%feature("docstring")  cmf::math::num_array::num_array "num_array()

constructors ";

%feature("docstring")  cmf::math::num_array::num_array "num_array(const num_array &Vector) ";

%feature("docstring")  cmf::math::num_array::num_array "num_array(const_iterator begin, const_iterator end) ";

%feature("docstring")  cmf::math::num_array::num_array "num_array(ptrdiff_t count, real Value=0.0) ";

%feature("docstring")  cmf::math::num_array::num_array "num_array(size_t count, real Value=0.0) ";

%feature("docstring")  cmf::math::num_array::num_array "num_array(size_t count, real *data) ";

%feature("docstring")  cmf::math::num_array::~num_array "~num_array()

destructor ";

%feature("docstring")  cmf::math::num_array::apply "num_array
apply(real funct(real)) const ";

%feature("docstring")  cmf::math::num_array::axpy "void axpy(const
real fac, const num_array &other) ";

%feature("docstring")  cmf::math::num_array::begin "iterator begin()
const ";

%feature("docstring")  cmf::math::num_array::dot "real dot(const
num_array &) const ";

%feature("docstring")  cmf::math::num_array::end "iterator end()
const ";

%feature("docstring")  cmf::math::num_array::is_nan "bool is_nan()
const ";

%feature("docstring")  cmf::math::num_array::max "real max() const ";

%feature("docstring")  cmf::math::num_array::mean "real mean() const
";

%feature("docstring")  cmf::math::num_array::min "real min() const ";

%feature("docstring")  cmf::math::num_array::norm "real norm(int
normtype=0) const ";

%feature("docstring")  cmf::math::num_array::power "num_array
power(const num_array &) const ";

%feature("docstring")  cmf::math::num_array::power "num_array
power(real exponent) const ";

%feature("docstring")  cmf::math::num_array::release "iterator
release() ";

%feature("docstring")  cmf::math::num_array::resize "void
resize(ptrdiff_t count) ";

%feature("docstring")  cmf::math::num_array::scale "void scale(const
real fac) ";

%feature("docstring")  cmf::math::num_array::scale_add "void
scale_add(const real fac, const num_array &other) ";

%feature("docstring")  cmf::math::num_array::set "void set(const
num_array &other)

Prevents deletion of the stored data.

Use with care, and only if you know what you are doing. ";

%feature("docstring")  cmf::math::num_array::set "void set(const real
fac, const num_array &other) ";

%feature("docstring")  cmf::math::num_array::size "ptrdiff_t size()
const ";

%feature("docstring")  cmf::math::num_array::sum "real sum() const ";


// File: classcmf_1_1math_1_1_o_d_esystem.xml
%feature("docstring") cmf::math::ODEsystem "";

%feature("docstring")  cmf::math::ODEsystem::ODEsystem "ODEsystem(const cmf::math::ODEsystem &for_copy) ";

%feature("docstring")  cmf::math::ODEsystem::ODEsystem "ODEsystem()=default ";

%feature("docstring")  cmf::math::ODEsystem::ODEsystem "ODEsystem(const cmf::math::state_list &for_copy, int use_OpenMP=-1) ";

%feature("docstring")  cmf::math::ODEsystem::~ODEsystem "~ODEsystem()=default ";

%feature("docstring")  cmf::math::ODEsystem::add_values_to_states "void add_values_to_states(const num_array &operands)

Adds the values in operands to the current states. ";

%feature("docstring")  cmf::math::ODEsystem::begin "state_list::iterator begin() ";

%feature("docstring")  cmf::math::ODEsystem::begin "state_list::const_iterator begin() const ";

%feature("docstring")  cmf::math::ODEsystem::copy_dxdt "void
copy_dxdt(Time time, num_array &destination, real factor=1) const

Copies the derivatives at time step \"time\" to a numeric vector using
use_OpenMP.

Parameters:
-----------

time:   Time at which the derivatives should be calculated

destination:  Vector to be overwritten by the results

factor:  A factor that is multiplied to the derivate (e.g. unit
conversion or integration length) ";

%feature("docstring")  cmf::math::ODEsystem::copy_dxdt "void
copy_dxdt(Time time, real *destination, real factor=1) const

Copies the derivatives at time step \"time\" to an preallocated c
array.

Parameters:
-----------

time:   Time at which the derivatives should be calculated

destination:  Allocated c array

factor:  A factor that is multiplied to the derivate (e.g. unit
conversion or integration length) ";

%feature("docstring")  cmf::math::ODEsystem::copy_states "void
copy_states(num_array &destination) const ";

%feature("docstring")  cmf::math::ODEsystem::copy_states "void
copy_states(real *destination) const ";

%feature("docstring")  cmf::math::ODEsystem::end "state_list::iterator end() ";

%feature("docstring")  cmf::math::ODEsystem::end "state_list::const_iterator end() const ";

%feature("docstring")  cmf::math::ODEsystem::get_dxdt "cmf::math::num_array get_dxdt(Time time) const ";

%feature("docstring")  cmf::math::ODEsystem::get_state_value "real
get_state_value(ptrdiff_t index) const ";

%feature("docstring")  cmf::math::ODEsystem::get_state_values "cmf::math::num_array get_state_values() const ";

%feature("docstring")  cmf::math::ODEsystem::set_state_value "void
set_state_value(ptrdiff_t index, real value) const ";

%feature("docstring")  cmf::math::ODEsystem::set_states "void
set_states(const num_array &newStates)

Copies the new states to the actual states. ";

%feature("docstring")  cmf::math::ODEsystem::set_states "void
set_states(real *newStates) ";

%feature("docstring")  cmf::math::ODEsystem::size "size_t size()
const ";


// File: classcmf_1_1river_1_1_open_water_storage.xml
%feature("docstring") cmf::river::OpenWaterStorage "

An open water body.

The potential is calculated from the stored water using a water table
function

C++ includes: OpenWaterStorage.h ";

/*  Overrides of flux_node  */

%feature("docstring")  cmf::river::OpenWaterStorage::RecalcFluxes "virtual bool RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node.

WaterStorage overrides this, since state changes require an update of
the fluxes ";

%feature("docstring")  cmf::river::OpenWaterStorage::is_empty "virtual double is_empty() const

Returns true if the node has no water. ";

%feature("docstring")  cmf::river::OpenWaterStorage::to_string "virtual std::string to_string() const ";

%feature("docstring")
cmf::river::OpenWaterStorage::add_connected_states "virtual void
add_connected_states(cmf::math::StateVariable::list &states) ";

%feature("docstring")  cmf::river::OpenWaterStorage::conc "real
conc(cmf::math::Time t, const cmf::water::solute &solute) const

Returns the water quality of the flux_node, if it is not overridden
this is the mix of the incoming fluxes. ";

%feature("docstring")  cmf::river::OpenWaterStorage::conc "real
conc(const cmf::water::solute &_Solute) const

Returns the concentration of the given solute. ";

%feature("docstring")  cmf::river::OpenWaterStorage::conc "void
conc(const cmf::water::solute &_Solute, real NewConcetration)

Sets a new concentration. ";

%feature("docstring")  cmf::river::OpenWaterStorage::connection_to "cmf::water::flux_connection* connection_to(const cmf::water::flux_node
&target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::river::OpenWaterStorage::dxdt "virtual
real dxdt(const cmf::math::Time &time)

Returns the derivate of the state variable at time time. ";

%feature("docstring")  cmf::river::OpenWaterStorage::flux3d_to "cmf::geometry::point flux3d_to(const cmf::water::flux_node &target,
cmf::math::Time t) ";

%feature("docstring")  cmf::river::OpenWaterStorage::flux_to "real
flux_to(const cmf::water::flux_node &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from this into target\") ";

%feature("docstring")  cmf::river::OpenWaterStorage::get_3d_flux "cmf::geometry::point get_3d_flux(cmf::math::Time t)

Returns the sum of all flux vectors. ";

%feature("docstring")  cmf::river::OpenWaterStorage::get_abs_errtol "real get_abs_errtol(real rel_errtol) const ";

%feature("docstring")  cmf::river::OpenWaterStorage::get_connections "cmf::water::connection_list get_connections() const ";

%feature("docstring")  cmf::river::OpenWaterStorage::get_depth "real
get_depth() const

Returns the water table depth. ";

%feature("docstring")
cmf::river::OpenWaterStorage::get_height_function "virtual const
IVolumeHeightFunction& get_height_function() const

The functional relation between volume, depth and exposed area. ";

%feature("docstring")  cmf::river::OpenWaterStorage::get_potential "real get_potential(cmf::math::Time t=cmf::math::never) const override

Returns the water potential of the node in m waterhead.

The base class water storage always returns the height of the location
";

%feature("docstring")  cmf::river::OpenWaterStorage::get_project "cmf::project& get_project() const

Returns the project, this node is part of. ";

%feature("docstring")  cmf::river::OpenWaterStorage::get_state "real
get_state() const

Returns the current state of the variable. ";

%feature("docstring")
cmf::river::OpenWaterStorage::get_state_variable_content "char
get_state_variable_content() const

A character indicating the integrated variable (either 'V' for Volume
or 'h' for head) ";

%feature("docstring")  cmf::river::OpenWaterStorage::get_volume "virtual real get_volume() const

Returns the volume of water in this storage in m3 ";

%feature("docstring")  cmf::river::OpenWaterStorage::is_connected "virtual bool is_connected(const cmf::math::StateVariable &other) const

Returns True if this waterstorage is effected by another state. ";

%feature("docstring")  cmf::river::OpenWaterStorage::is_storage "virtual bool is_storage() const

Returns true, since this is a storage. ";

%feature("docstring")  cmf::river::OpenWaterStorage::remove_connection
"bool remove_connection(cmf::water::flux_node::ptr To)

Remove the connection. ";

%feature("docstring")  cmf::river::OpenWaterStorage::set_depth "void
set_depth(real new_depth) ";

%feature("docstring")
cmf::river::OpenWaterStorage::set_height_function "virtual void
set_height_function(const IVolumeHeightFunction &val) ";

%feature("docstring")  cmf::river::OpenWaterStorage::set_potential "void set_potential(real newpotential) override

Sets the potential of this flux node. ";

%feature("docstring")  cmf::river::OpenWaterStorage::set_state "void
set_state(real newState)

Gives access to the state variable. ";

%feature("docstring")
cmf::river::OpenWaterStorage::set_state_variable_content "void
set_state_variable_content(char content)

A character indicating the integrated variable (either 'V' for Volume
or 'h' for head) ";

%feature("docstring")  cmf::river::OpenWaterStorage::set_volume "virtual void set_volume(real newwatercontent)

Sets the volume of water in this storage in m3 ";

%feature("docstring")  cmf::river::OpenWaterStorage::Solute "SoluteStorage& Solute(const cmf::water::solute _Solute)

Returns the water quality of the water storage. ";

%feature("docstring")  cmf::river::OpenWaterStorage::Solute "const
SoluteStorage& Solute(const cmf::water::solute _Solute) const ";

%feature("docstring")  cmf::river::OpenWaterStorage::waterbalance "real waterbalance(cmf::math::Time t, const flux_connection *Without=0)
const

Returns the sum of all fluxes (positive and negative) at time t.

Single fluxes can be excluded from the calculation

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the waterbalance
(e.g. to prevent closed circuits) ";

%feature("docstring")  cmf::river::OpenWaterStorage::wet_area "real
wet_area() const

Returns the exposed surface area in m2. ";


// File: classcmf_1_1upslope_1_1_e_t_1_1_oudin_e_t.xml
%feature("docstring") cmf::upslope::ET::OudinET "

Calculates ETpot after Oudin et al 2005.

https://doi.org/10.1016/j.jhydrol.2004.08.026 This ETpot formula is a
generalization of two older approaches by introducing parameters to
shape the dependency of ETpot from temperature and extraterrestrial
radiation

:math:`\\\\lambda ET_{pot} = R_{a} \\\\cdot \\\\frac{T_{a} + K_2}{K_1}`

Oudin et al (2005) found an optimum for :math:`K_1=100, K_2=5`. The origin
of this formula lays in Jensen & Haise (1963) with :math:`K_1=40, K_2=0` and
McGuiness-Bordne (1972) with :math:`K_1=68, K_2=5`.

C++ includes: ET.h ";

%feature("docstring")  cmf::upslope::ET::OudinET::OudinET "OudinET(cmf::upslope::SoilLayer::ptr source,
cmf::water::flux_node::ptr ET_target, double K1=100, double K2=5) ";

%feature("docstring")  cmf::upslope::ET::OudinET::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")  cmf::upslope::ET::OudinET::ETpot "real
ETpot(cmf::math::Time t) const ";

%feature("docstring")  cmf::upslope::ET::OudinET::exchange_target "void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")  cmf::upslope::ET::OudinET::get_layer "SoilLayer::ptr get_layer() const ";

%feature("docstring")  cmf::upslope::ET::OudinET::get_ptr "ptr
get_ptr() const ";

%feature("docstring")  cmf::upslope::ET::OudinET::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::ET::OudinET::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")  cmf::upslope::ET::OudinET::get_tracer_filter "real get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")  cmf::upslope::ET::OudinET::get_tracer_filter "real get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::upslope::ET::OudinET::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::upslope::ET::OudinET::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::ET::OudinET::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::ET::OudinET::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")  cmf::upslope::ET::OudinET::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::upslope::ET::OudinET::set_tracer_filter "void set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")  cmf::upslope::ET::OudinET::set_tracer_filter "void set_tracer_filter(solute S, real value) ";

%feature("docstring")  cmf::upslope::ET::OudinET::short_string "virtual std::string short_string() const ";

%feature("docstring")  cmf::upslope::ET::OudinET::to_string "virtual
std::string to_string() const ";


// File: classcmf_1_1upslope_1_1_e_t_1_1_penman_evaporation.xml
%feature("docstring") cmf::upslope::ET::PenmanEvaporation "

Calculates evaporation from an open water body.

C++ includes: ET.h ";

%feature("docstring")
cmf::upslope::ET::PenmanEvaporation::PenmanEvaporation "PenmanEvaporation(cmf::river::OpenWaterStorage::ptr source,
cmf::water::flux_node::ptr Evap_target, const
cmf::atmosphere::Meteorology &meteo) ";

%feature("docstring")  cmf::upslope::ET::PenmanEvaporation::conc "real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::ET::PenmanEvaporation::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::ET::PenmanEvaporation::get_ptr "ptr get_ptr() const ";

%feature("docstring")  cmf::upslope::ET::PenmanEvaporation::get_target
"flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::ET::PenmanEvaporation::get_target
"flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::ET::PenmanEvaporation::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::ET::PenmanEvaporation::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::upslope::ET::PenmanEvaporation::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::upslope::ET::PenmanEvaporation::left_node
"flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::ET::PenmanEvaporation::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::ET::PenmanEvaporation::refresh "void refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")  cmf::upslope::ET::PenmanEvaporation::right_node
"flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::ET::PenmanEvaporation::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::ET::PenmanEvaporation::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::ET::PenmanEvaporation::short_string "virtual
std::string short_string() const ";

%feature("docstring")  cmf::upslope::ET::PenmanEvaporation::to_string
"virtual std::string to_string() const ";


// File: classcmf_1_1upslope_1_1_e_t_1_1_penman_monteith_e_t.xml
%feature("docstring") cmf::upslope::ET::PenmanMonteithET "

Calculates the potential evapotranspiration according to FAO(1998)

Governing equations: 

.. math::

     \\\\lambda ET &=&
    \\\\frac{\\\\Delta\\\\left(R_n - G\\\\right)+\\\\rho_a c_p
    \\\\frac{e_s - e_a}{r_a}}{\\\\Delta +
    \\\\gamma\\\\left(1+\\\\frac{r_s}{r_a}\\\\right)} \\\\mbox{ FAO 1998,
    Eq. 3} \\\\\\\\ \\\\mbox{With:} \\\\\\\\ \\\\Delta &=& 4098
    \\\\frac{0.6108 e^{17.27 T}}{(T+237.3)^2} \\\\frac{kPa}{^\\\\circ C}
    \\\\mbox{ (FAO 1998, Eq. 13)} \\\\\\\\ T &=& \\\\mbox{Actual
    Temperature in } ^\\\\circ C \\\\\\\\ R_n &=& \\\\mbox{net Radiation
    (see Atmosphere) in } \\\\frac{MJ}{m^2day} \\\\\\\\ G &=& 0 \\\\
    \\\\frac{MJ}{m^2day} \\\\mbox{ if daily average (FAO 1998, Eq. 42)}
    \\\\\\\\ && 0.1 R_n \\\\ \\\\mbox{ if day time (FAO 1998, Eq. 45)}
    \\\\\\\\ && 0.5 R_n \\\\ \\\\mbox{ if night time (FAO 1998, Eq. 46)}
    \\\\\\\\ \\\\gamma &=& \\\\frac{c_p P}{\\\\epsilon \\\\lambda}
    \\\\mbox{ (FAO 1998,Eq. 8): Psychrometric constant }
    \\\\frac{kPa}{^\\\\circ C} \\\\\\\\ c_p &=& 0.001013
    \\\\frac{MJ}{kg\\\\ ^\\\\circ C}\\\\mbox{ specific heat at constant
    pressure } \\\\\\\\ P &=& 101.3
    \\\\left(\\\\frac{293-0.0065z}{293}\\\\right)^{5.26} \\\\mbox{ (FAO
    1998,Eq. 7)} \\\\\\\\ \\\\epsilon &=& 0.622 \\\\frac{mol/g \\\\mbox{
    vapor}}{mol/g \\\\mbox{ liquid water}} \\\\\\\\ \\\\lambda &=& 2.45
    \\\\frac{MJ}{kg} \\\\mbox{ (FAO 1998,Eq. 8)} \\\\\\\\ R &=& 0.287
    \\\\frac{kJ}{kg\\\\ k}\\\\mbox{ Spec. gas const.} \\\\\\\\ \\\\rho_a
    &=& \\\\frac{P}{1.01(T+273)R} \\\\mbox{ (FAO 1998,Box. 6)} \\\\\\\\
    e_s &=& \\\\mbox{ Sat. vapor press. } [kPa] \\\\\\\\ e_a &=& \\\\mbox{
    Act. vapor press. } [kPa] \\\\\\\\ r_a &=&
    \\\\frac{\\\\ln\\\\left(\\\\frac{2-d}{z_{om}}\\\\right)\\\\ln\\\\left(\\\\frac{2-d}{z_{oh}}\\\\right)}{k^2
    u_2} \\\\mbox{ (FAO 1998, Eq. 4/Box 4} \\\\frac s m \\\\\\\\ &&
    d=\\\\frac 2 3 h,z_{om}=0.123 h,z_{oh}=0.1 z_{om}, k=0.41 \\\\\\\\ h
    &=& \\\\mbox{ Vegetation height in }m \\\\\\\\ u_2 &=& \\\\mbox{
    Windspeed in 2m above canopy } \\\\frac m s \\\\\\\\ r_s &=&
    \\\\frac{r_l}{LAI_{Active}} \\\\mbox{ (FAO 1998, Eq. 5/Box 5)}
    \\\\frac s m \\\\\\\\ && r_l=100 \\\\frac s m, LAI_{Active}=0.5 LAI
    



C++ includes: ET.h ";

%feature("docstring")
cmf::upslope::ET::PenmanMonteithET::PenmanMonteithET "PenmanMonteithET(cmf::upslope::SoilLayer::ptr source,
cmf::water::flux_node::ptr ET_target) ";

%feature("docstring")  cmf::upslope::ET::PenmanMonteithET::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")  cmf::upslope::ET::PenmanMonteithET::ETpot "virtual real ETpot(cmf::math::Time t) const ";

%feature("docstring")
cmf::upslope::ET::PenmanMonteithET::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::ET::PenmanMonteithET::get_layer "SoilLayer::ptr get_layer() const ";

%feature("docstring")  cmf::upslope::ET::PenmanMonteithET::get_ptr "ptr get_ptr() const ";

%feature("docstring")  cmf::upslope::ET::PenmanMonteithET::get_target
"flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::ET::PenmanMonteithET::get_target
"flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::ET::PenmanMonteithET::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::ET::PenmanMonteithET::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::upslope::ET::PenmanMonteithET::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::upslope::ET::PenmanMonteithET::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::ET::PenmanMonteithET::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::ET::PenmanMonteithET::refresh "void refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")  cmf::upslope::ET::PenmanMonteithET::right_node
"flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::ET::PenmanMonteithET::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::ET::PenmanMonteithET::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::ET::PenmanMonteithET::short_string "virtual std::string
short_string() const ";

%feature("docstring")  cmf::upslope::ET::PenmanMonteithET::to_string "virtual std::string to_string() const ";


// File: classcmf_1_1river_1_1_pipe_reach.xml
%feature("docstring") cmf::river::PipeReach "

Describes the geometry of a closed pipe.

C++ includes: ReachType.h ";

%feature("docstring")  cmf::river::PipeReach::PipeReach "PipeReach(double l, double diameter)

Creates a tube IChannel with diameter [m]. ";

%feature("docstring")  cmf::river::PipeReach::PipeReach "PipeReach(const PipeReach &copy) ";

%feature("docstring")  cmf::river::PipeReach::~PipeReach "virtual
~PipeReach() ";

%feature("docstring")  cmf::river::PipeReach::A "virtual double
A(double V) const

Returns the area of the surface for a given volume. ";

%feature("docstring")  cmf::river::PipeReach::copy "PipeReach* copy()
const ";

%feature("docstring")  cmf::river::PipeReach::get_channel_width "virtual double get_channel_width(double depth) const



.. math::

    w=2\\\\sqrt{\\\\left|r^2-(r-d)^2\\\\right|} 

";

%feature("docstring")  cmf::river::PipeReach::get_depth "virtual
double get_depth(double area) const



.. math::

    d=r\\\\left(1-\\\\cos{\\\\frac{A}{r^2}}\\\\right) 

";

%feature("docstring")  cmf::river::PipeReach::get_flux_crossection "virtual double get_flux_crossection(double depth) const



.. math::

    A=r^2\\\\arccos{\\\\frac{r-d}{r}{r}} 

";

%feature("docstring")  cmf::river::PipeReach::get_length "double
get_length() const

Length of the reach. ";

%feature("docstring")  cmf::river::PipeReach::get_nManning "virtual
double get_nManning() const ";

%feature("docstring")  cmf::river::PipeReach::get_wetted_perimeter "virtual double get_wetted_perimeter(double depth) const



.. math::

    P=r\\\\arccos{\\\\frac{r-d}{r}} 

";

%feature("docstring")  cmf::river::PipeReach::h "virtual double
h(double V) const

Returns the depth of a given volume. ";

%feature("docstring")  cmf::river::PipeReach::q "virtual double
q(double h, double slope) const ";

%feature("docstring")  cmf::river::PipeReach::qManning "virtual
double qManning(double A, double slope) const

Calculates the flow rate from a given water volume in the reach


.. math::

     q_{Manning}&=& A R^{\\\\frac 2 3}
    \\\\sqrt{\\\\frac {\\\\Delta_z} n} \\\\\\\\ A &=& \\\\frac V l
    \\\\mbox{, (Crosssectional area of the wetted crossection, Volume per
    length)} \\\\\\\\ R &=& \\\\frac A {P(d)} \\\\\\\\ P(d) &=& \\\\mbox{
    the perimeter of the wetted crosssection, a function of reach depth}
    \\\\\\\\ d(V) &=& \\\\mbox{ the depth of the reach a function of the
    volume} \\\\\\\\ \\\\Delta_z &=& \\\\frac{z_{max} - z_{min}}{l}
    \\\\mbox{ Slope of the reach} 

.

Flow rate [m3/s]

Parameters:
-----------

A:  The area of the cross section [m2]

slope:  The slope of the reach [m/m] ";

%feature("docstring")  cmf::river::PipeReach::set_nManning "virtual
void set_nManning(double nManning) ";

%feature("docstring")  cmf::river::PipeReach::typecode "char
typecode() const ";

%feature("docstring")  cmf::river::PipeReach::V "virtual double
V(double h) const ";


// File: classcmf_1_1geometry_1_1point.xml
%feature("docstring") cmf::geometry::point "

2D-Point Class.

Used as location property anywhere in the text Calculation of
distances

+,-,-=,*= Operators overloaded

C++ includes: geometry.h ";

%feature("docstring")  cmf::geometry::point::point "point()

Empty Constructor. Creates an (0 0) location. ";

%feature("docstring")  cmf::geometry::point::point "point(const point
&p)

Copy Constructor. ";

%feature("docstring")  cmf::geometry::point::point "point(double x_,
double y_, double z_=0.0)

Creates a point from two doubles. ";

%feature("docstring")  cmf::geometry::point::angleToXAxis "double
angleToXAxis(point p) const

Returns the angle between the line :math:`\\\\overline{this,p}` to the
x-Axis in degrees. ";

%feature("docstring")  cmf::geometry::point::azimuth "double
azimuth(point p) const

Returns the azimuth angle of the line :math:`\\\\overline{this,p}` to the
Azimuth in degrees. ";

%feature("docstring")  cmf::geometry::point::distance3DTo "double
distance3DTo(point p) const

Returns the euclidian distance in space to another point p. ";

%feature("docstring")  cmf::geometry::point::distance_max "double
distance_max(point p) const

Returns the distance by the maximum orthogonal offset. ";

%feature("docstring")  cmf::geometry::point::distanceTo "double
distanceTo(point p) const

Returns the horizontal euclidian distance to another point p.

:math:`\\\\sqrt{(this.x-p.x)^2+(this.y-p.y)^2}` ";

%feature("docstring")  cmf::geometry::point::length "double length()
const ";

%feature("docstring")  cmf::geometry::point::sum "double sum() const

Returns x+y+z. ";

%feature("docstring")  cmf::geometry::point::z_weight_distance "double z_weight_distance(point p, double z_weight) const

Returns the horizontal euclidian distance plus the absolute of the
height difference times a factor.

:math:`\\\\sqrt{(this.x-p.x)^2+(this.y-p.y)^2}\\\\ +\\\\ w_{z}|this.z-p.z|`
";


// File: classcmf_1_1geometry_1_1point__vector.xml
%feature("docstring") cmf::geometry::point_vector "

Holds three arrays x,y and z for fast access of point coordinates.

C++ includes: geometry.h ";

%feature("docstring")  cmf::geometry::point_vector::point_vector "point_vector(ptrdiff_t size)

Create a point vector of a specific size. ";

%feature("docstring")  cmf::geometry::point_vector::get "point
get(ptrdiff_t index) const

Return a point at index. ";

%feature("docstring")  cmf::geometry::point_vector::set "void
set(ptrdiff_t index, cmf::geometry::point p)

Change the point at index. ";

%feature("docstring")  cmf::geometry::point_vector::size "size_t
size() const

Return the number of points in the point_vector. ";


// File: classcmf_1_1maps_1_1_polygon_map.xml
%feature("docstring") cmf::maps::PolygonMap "

A map of polygons. Each object is referenced with a shapely polygon.
Returns the object of the first polygon, within the query position
lays. ";

%feature("docstring")  cmf::maps::PolygonMap::__init__ "def
__init__(self, quad_tree_raster_size=20) ";

%feature("docstring")  cmf::maps::PolygonMap::__call__ "def
__call__(self, x, y, z=0) ";

%feature("docstring")  cmf::maps::PolygonMap::__iter__ "def
__iter__(self) ";

%feature("docstring")  cmf::maps::PolygonMap::__nonzero__ "def
__nonzero__(self) ";

%feature("docstring")  cmf::maps::PolygonMap::append "def
append(self, polygon, object) ";

%feature("docstring")  cmf::maps::PolygonMap::remove "def
remove(self, polygon) ";

%feature("docstring")  cmf::maps::PolygonMap::values "def
values(self) ";


// File: classcmf_1_1water_1_1_power_law_connection.xml
%feature("docstring") cmf::water::PowerLawConnection "

Calculates flux out of a storage as a linear function of its volume to
a power.

This connection serves the same purpose as the old kinematic_wave
connection, but the parameters are easier to explain.



.. math::

     q = Q_0 {\\\\left(\\\\frac{V - V_{residual}}{V_0}
    \\\\right)^\\\\beta} 

where:  :math:`V` The actual volume of water
stored in source

:math:`V_{residual} [m^3]` The volume of water not flowing out (default = 0)

:math:`V_0` A reference volume to scale the outflux. One can see :math:`V_0` as
the inflection point of the outflow curve

:math:`\\\\beta` A parameter to shape the response curve. :math:`Q_0` is the
outflow from the source in :math:`\\\\frac{m^3}{day}`, when :math:`V = V_0`.

WARNING:   :math:`\\\\beta < 0.5` may lead to numerical troubles and have a
dubious hydrological meaning. Please avoid.

C++ includes: simple_connections.h ";

%feature("docstring")
cmf::water::PowerLawConnection::PowerLawConnection "PowerLawConnection(WaterStorage::ptr source, flux_node::ptr target,
real Q0, real V0, real beta=1.0, real residual=0.0)

Creates a power law connection.

Parameters:
-----------

source:  Water storage from which the water flows out. Flux is a
function of source.volume

target:  Target node (boundary condition or storage). Does not
influence the strength of the flow

Q0:  Reference flow :math:`Q_0 = q(V_0)` Outflow when the source storage
equals the reference volume

V0:  Reference volume :math:`V_0` The reference volume to scale the exponent

beta:   :math:`\\\\beta [-]` An empirical exponent to shape the flux
function (default = 1 (linear function))

residual:   :math:`V_{residual} [m^3]` The volume of water not flowing out
(default = 0) ";

%feature("docstring")  cmf::water::PowerLawConnection::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")  cmf::water::PowerLawConnection::exchange_target
"void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")  cmf::water::PowerLawConnection::get_ptr "ptr
get_ptr() const ";

%feature("docstring")  cmf::water::PowerLawConnection::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::water::PowerLawConnection::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::water::PowerLawConnection::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::water::PowerLawConnection::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::water::PowerLawConnection::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::water::PowerLawConnection::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::water::PowerLawConnection::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::water::PowerLawConnection::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")  cmf::water::PowerLawConnection::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::water::PowerLawConnection::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::water::PowerLawConnection::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")  cmf::water::PowerLawConnection::short_string "virtual std::string short_string() const ";

%feature("docstring")  cmf::water::PowerLawConnection::to_string "virtual std::string to_string() const ";


// File: classcmf_1_1math_1_1precalculatable.xml
%feature("docstring") cmf::math::precalculatable "";

%feature("docstring")  cmf::math::precalculatable::do_action "virtual
void do_action(Time t, bool use_OpenMP=true)=0 ";


// File: classcmf_1_1upslope_1_1_e_t_1_1_priestley_taylor_e_t.xml
%feature("docstring") cmf::upslope::ET::PriestleyTaylorET "

Calculates the Evapotranspiration using Priestley-Taylor equation.



.. math::

    lambda ET = \\\\alpha \\\\frac{\\\\Delta}{\\\\Delta + \\\\gamma}
    \\\\left(R_n - G\\\\right)

where:  :math:`\\\\Delta = 4098 \\\\frac{0.6108 e^{17.27 T}}{(T+237.3)^2} \\\\frac{kPa}{^\\\\circ C}`,
the slope of the vapor pressure/ temperature curve

:math:`\\\\gamma = \\\\frac{c_p P}{\\\\epsilon \\\\lambda} \\\\frac{kPa}{^\\\\circ C}` Psychrometric constant

:math:`\\\\lambda = 2.45 \\\\frac{MJ}{kg}` the latent heat of vaporization

:math:`R_n \\\\frac{MJ}{m^2day}` net Radiation (see Atmosphere)

:math:`G` Ground heat flux

:math:`\\\\alpha` the Priestley-Taylor constant (default 1.26 for humid
climates)

C++ includes: ET.h ";

%feature("docstring")
cmf::upslope::ET::PriestleyTaylorET::PriestleyTaylorET "PriestleyTaylorET(cmf::upslope::SoilLayer::ptr source,
cmf::water::flux_node::ptr ET_target, real alpha=1.26) ";

%feature("docstring")  cmf::upslope::ET::PriestleyTaylorET::conc "real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")  cmf::upslope::ET::PriestleyTaylorET::ETpot "real ETpot(cmf::math::Time t) const ";

%feature("docstring")
cmf::upslope::ET::PriestleyTaylorET::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::ET::PriestleyTaylorET::get_layer
"SoilLayer::ptr get_layer() const ";

%feature("docstring")  cmf::upslope::ET::PriestleyTaylorET::get_ptr "ptr get_ptr() const ";

%feature("docstring")  cmf::upslope::ET::PriestleyTaylorET::get_target
"flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::ET::PriestleyTaylorET::get_target
"flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::ET::PriestleyTaylorET::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::ET::PriestleyTaylorET::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::upslope::ET::PriestleyTaylorET::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::upslope::ET::PriestleyTaylorET::left_node
"flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::ET::PriestleyTaylorET::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::ET::PriestleyTaylorET::refresh "void refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")  cmf::upslope::ET::PriestleyTaylorET::right_node
"flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::ET::PriestleyTaylorET::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::ET::PriestleyTaylorET::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::ET::PriestleyTaylorET::short_string "virtual
std::string short_string() const ";

%feature("docstring")  cmf::upslope::ET::PriestleyTaylorET::to_string
"virtual std::string to_string() const ";


// File: classcmf_1_1river_1_1_prism.xml
%feature("docstring") cmf::river::Prism "

the height of a volume in a Prism with a defined base area

C++ includes: ReachType.h ";

%feature("docstring")  cmf::river::Prism::Prism "Prism(double
base_area, double thickness_of_rough_ground=0.01) ";

%feature("docstring")  cmf::river::Prism::~Prism "~Prism() ";

%feature("docstring")  cmf::river::Prism::A "double A(double V) const

:math:`A(V) = A_{base}` ";

%feature("docstring")  cmf::river::Prism::copy "Prism* copy() const
";

%feature("docstring")  cmf::river::Prism::h "double h(double V) const

:math:`h(V) = \\\\frac V A_{base}` ";

%feature("docstring")  cmf::river::Prism::q "virtual double q(double
h, double slope) const ";

%feature("docstring")  cmf::river::Prism::V "double V(double h) const
";


// File: classcmf_1_1project.xml
%feature("docstring") cmf::project "

The study area, holding all cells, outlets and streams.

Todo Describe tracers

C++ includes: project.h ";

%feature("docstring")  cmf::project::project "project(std::string
solute_names=\"\")

Creates a new project.

Parameters:
-----------

solute_names:  A string representing the names of the solutes to be
used in the project. Sepereate solute names with space. ";

%feature("docstring")  cmf::project::~project "~project() ";

%feature("docstring")  cmf::project::get_cell "upslope::Cell&
get_cell(ptrdiff_t index)

Returns the reference to the cell at index in the project. ";

%feature("docstring")  cmf::project::get_cells "const
upslope::cell_vector& get_cells() const

Returns the cells in the project. ";

%feature("docstring")  cmf::project::get_node "cmf::water::flux_node::ptr get_node(ptrdiff_t index)

Returns the node from the project nodes at index. ";

%feature("docstring")  cmf::project::get_reach "cmf::river::Reach::ptr get_reach(ptrdiff_t index)

Returns the reach at index. ";

%feature("docstring")  cmf::project::get_storages "cmf::water::node_list get_storages()

Returns a list of all storages of this project. ";

%feature("docstring")  cmf::project::NewCell "cmf::upslope::Cell*
NewCell(double x, double y, double z, double area, bool
with_surfacewater=false)

Creates a new cell.

A new cell, owned by the project

Parameters:
-----------

x:  y:  z:  Position of the cell center in project coordinates (m)

area:  Area of the cell in m^2

with_surfacewater:  If true, the cell will own a surfacewater storage
upon creation ";

%feature("docstring")  cmf::project::NewNeumannBoundary "cmf::water::NeumannBoundary::ptr NewNeumannBoundary(std::string name,
cmf::water::WaterStorage::ptr target)

Creates a new Neumann boundary for a target water storage.

The boundary is stored with the project A new Neumann boundary

Parameters:
-----------

name:  The name of the boundary condition

target:  The water storage to which the Neumann boundary is bound ";

%feature("docstring")  cmf::project::NewOpenStorage "cmf::river::OpenWaterStorage::ptr NewOpenStorage(std::string name,
double x, double y, double z, double area)

Creates a new open water storage with a prism geometry.

The open water storage is added to the project nodes A new open water
storage, owned by the project

Parameters:
-----------

name:  Name of the open water storage for output

x:  y:  z:  Position of the open water storage in project coordinates

area:  Surface area of the open water storage ";

%feature("docstring")  cmf::project::NewOutlet "cmf::water::DirichletBoundary::ptr NewOutlet(std::string name, double
x=0, double y=0, double z=0)

Creates a new Dirichlet boundary condition and adds it to the list of
outlets The potential of the Dirichlet boundary equals z, but can be
changed.

Parameters:
-----------

name:  Name of the boundary condition for output

x:  y:  z:  Position of the boundary condition in project coordinates
";

%feature("docstring")  cmf::project::NewReach "cmf::river::Reach::ptr
NewReach(double x, double y, double z, cmf::river::IChannel &shape,
bool diffusive=false)

Creates a new reach.

A new reach, owned by the project

Parameters:
-----------

x:  y:  z:  Position of the reach in project coordinates

shape:  Crossectional geometry of the river. Any class inheriting from
cmf::water::IChannel

diffusive:  If true, this reach uses by default a diffusive wave
connection ";

%feature("docstring")  cmf::project::NewStorage "cmf::water::WaterStorage::ptr NewStorage(std::string name, double x=0,
double y=0, double z=0)

Creates a new generic water storage at position x,y,z.

The storage is added to the project nodes A new water storage, owned
by the project

Parameters:
-----------

name:  Name of the generic water storage for output

x:  y:  z:  Position of the generic water storage condition in project
coordinates ";

%feature("docstring")  cmf::project::node_count "size_t node_count()
const

Returns the number of nodes saved with this project. ";

%feature("docstring")  cmf::project::reach_count "size_t
reach_count() const

Returns the number of reaches in this project. ";

%feature("docstring")  cmf::project::remove_node "size_t
remove_node(cmf::water::flux_node::ptr node)

Removes a node from the repository.

Removes a node (boundary condition or water storage) from the node
repository of the project. NOTE: If you have other references to this
node, the node is not deleted. If you are creating a new solver, the
node will not be part of the solver. ";

%feature("docstring")  cmf::project::size "size_t size() const

The number of cells in the project. ";

%feature("docstring")  cmf::project::use_IDW_meteo "void
use_IDW_meteo(double z_weight=0, double power=2)

Uses IDW interpolation to generate meterological data for each cell of
project.

Creates a meteo-data source for each cell, using spatial interpolated
data from all meteorological stations of the project using Inverse
Distance Weighted (IDW) interpolation. The meteorolgical value f is
calculated with IDW for position x,y,z and time t as follows:


.. math::

     f(x,y,z,t) &=& \\\\sum^N_{i=1}{f_i(t) w_i(x,y,z)}
    \\\\\\\\ w_i(x,y,z) &=&
    \\\\frac{d_i(x,y,z)^{-p}}{\\\\sum^N_{j=0}{d_j(x,y,z)^{-p}}} \\\\\\\\
    d_i(x,y,z) &=& w_z \\\\left|z-z_i\\\\right| +
    \\\\sqrt{\\\\left(x-x_i\\\\right)^2 + \\\\left(y-y_i\\\\right)^2}
    

  :math:`N` is the number of stations

:math:`f_i(t)` the meteorological value at time t, eg. Temperature, Humidity

:math:`w_i` is the weight of station i

:math:`d_i` is the distance from x,y,z to station i

:math:`p` the power of the weighting function, usually 2.

:math:`x_i,y_i,z_i` is the position of station i in space

:math:`w_z` is a factor to weight the vertical distance between stations and
the cell. 0 results in a pure horizontal interpolation (normal IDW).
If :math:`w_z=1`, height difference is as important as horizontal distance,
and with :math:`w_z>1` the height difference is weighted more important than
horizontal distance See:  IDW_Meteorology

Parameters:
-----------

z_weight:   :math:`w_z` the weight of height difference between cell and
station

power:  the power of the distance weight ";

%feature("docstring")  cmf::project::use_IDW_rainfall "void
use_IDW_rainfall(double z_weight=0, double power=2)

Uses IDW interpolation to generate rainfall data for each cell of
project.

Creates a rainfall-data source for each cell, using spatial
interpolated data from all meteorological stations of the project
using Inverse Distance Weighted (IDW) interpolation. The rainfall
intensity P is calculated with IDW for position x,y,z and time t as
follows: 

.. math::

     P(x,y,z,t) &=& \\\\sum^N_{i=1}{P_i(t)
    w_i(x,y,z)} \\\\\\\\ w_i(x,y,z) &=&
    \\\\frac{d_i(x,y,z)^{-p}}{\\\\sum^N_{j=0}{d_j(x,y,z)^{-p}}} \\\\\\\\
    d_i(x,y,z) &=& w_z \\\\left|z-z_i\\\\right| +
    \\\\sqrt{\\\\left(x-x_i\\\\right)^2 + \\\\left(y-y_i\\\\right)^2}
    

  :math:`N` is the number of stations

:math:`P_i(t)` the meteorological value at time t, eg. Temperature, Humidity

:math:`w_i` is the weight of station i

:math:`d_i` is the distance from x,y,z to station i

:math:`p` the power of the weighting function, usually 2.

:math:`x_i,y_i,z_i` is the position of station i in space

:math:`w_z` is a factor to weight the vertical distance between stations and
the cell. 0 results in a pure horizontal interpolation (normal IDW).
If :math:`w_z=1`, height difference is as important as horizontal distance,
and with :math:`w_z>1` the height difference is weighted more important than
horizontal distance See:  IDW_Meteorology

Parameters:
-----------

z_weight:   :math:`w_z` the weight of height difference between cell and
station

power:  the power of the distance weight ";

%feature("docstring")  cmf::project::use_nearest_meteo "void
use_nearest_meteo(double z_weight=0)

Connects all cells of the project with its nearest meteorological
station.

Distance is calculated as follows: 

.. math::

    d_i(x,y,z) = w_z
    \\\\left|z-z_i\\\\right| + \\\\sqrt{\\\\left(x-x_i\\\\right)^2 +
    \\\\left(y-y_i\\\\right)^2} 

:math:`d_i` is the distance from x,y,z to
station i

:math:`p` the power of the weighting function, usually 2.

:math:`x_i,y_i,z_i` is the position of station i in space

:math:`w_z` is a factor to weight the vertical distance between stations and
the cell. 0 results in a pure horizontal interpolation (normal IDW).
If :math:`w_z=1`, height difference is as important as horizontal distance,
and with :math:`w_z>1` the height difference is weighted more important than
horizontal distance

Parameters:
-----------

z_weight:   :math:`w_z` the weight of height difference between cell and
station ";

%feature("docstring")  cmf::project::use_nearest_rainfall "void
use_nearest_rainfall(double z_weight=0)

Connects all cells of the project with its nearest rainfall station.

Distance is calculated as follows: 

.. math::

    d_i(x,y,z) = w_z
    \\\\left|z-z_i\\\\right| + \\\\sqrt{\\\\left(x-x_i\\\\right)^2 +
    \\\\left(y-y_i\\\\right)^2} 

:math:`d_i` is the distance from x,y,z to
station i

:math:`p` the power of the weighting function, usually 2.

:math:`x_i,y_i,z_i` is the position of station i in space

:math:`w_z` is a factor to weight the vertical distance between stations and
the cell. 0 results in a pure horizontal interpolation (normal IDW).
If :math:`w_z=1`, height difference is as important as horizontal distance,
and with :math:`w_z>1` the height difference is weighted more important than
horizontal distance

Parameters:
-----------

z_weight:   :math:`w_z` the weight of height difference between cell and
station ";


// File: classcmf_1_1geometry_1_1qtree_1_1_quadtree.xml
%feature("docstring") cmf::geometry::qtree::Quadtree "

A simple quad tree to check if the boundaries of geometries overlap or
not ";

%feature("docstring")  cmf::geometry::qtree::Quadtree::__init__ "def
__init__(self, area, divisions=50)

:param area: The total area of the study area in m2 :param divisions:
The number of divisions of the area, default is 20 ";

%feature("docstring")  cmf::geometry::qtree::Quadtree::__call__ "def
__call__(self, bounds)

Get all objects in the given boundaries :param bounds: The boundary
rectangle of the tree. A 4 item sequence (xmin, ymin, xmax, ymax)
:return: ";

%feature("docstring")  cmf::geometry::qtree::Quadtree::append "def
append(self, obj, bounds)

Appends an object to the qtree using the given bounds. :param obj: An
object to append to the tree, must be hashable :param bounds: The
boundary rectangle of the object. A 4 item sequence (xmin, ymin, xmax,
ymax) ";

%feature("docstring")  cmf::geometry::qtree::Quadtree::extend "def
extend(self, objects_with_boundaries)

Extends the Quadtree with objects :param objects_with_boundaries: an
iterable of (object, bounds) tuples :return: ";

%feature("docstring")  cmf::geometry::qtree::Quadtree::iterbounds "def iterbounds(self, bounds)

Iterates through all positions in the boundaries :param bounds: The
boundary rectangle. A 4 item sequence (xmin, ymin, xmax, ymax)
:return: (i,j) position tuple ";


// File: classcmf_1_1upslope_1_1connections_1_1_rainfall.xml
%feature("docstring") cmf::upslope::connections::Rainfall "

A connection routing rainfall to surface water and to an eventually
existing canopy storage.

C++ includes: AtmosphericFluxes.h ";

%feature("docstring")  cmf::upslope::connections::Rainfall::Rainfall "Rainfall(cmf::water::flux_node::ptr target, cmf::upslope::Cell &cell,
bool getthroughfall=true, bool getintercepted=true)

Creates a new Rainfall connection. ";

%feature("docstring")  cmf::upslope::connections::Rainfall::conc "real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::Rainfall::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::connections::Rainfall::get_ptr "ptr get_ptr() const ";

%feature("docstring")  cmf::upslope::connections::Rainfall::get_target
"flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::connections::Rainfall::get_target
"flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::Rainfall::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::Rainfall::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::upslope::connections::Rainfall::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::upslope::connections::Rainfall::left_node
"flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::connections::Rainfall::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::connections::Rainfall::refresh "void refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")  cmf::upslope::connections::Rainfall::right_node
"flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::Rainfall::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::Rainfall::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::connections::Rainfall::short_string "virtual
std::string short_string() const ";

%feature("docstring")  cmf::upslope::connections::Rainfall::to_string
"virtual std::string to_string() const ";


// File: classcmf_1_1atmosphere_1_1_rainfall_station.xml
%feature("docstring") cmf::atmosphere::RainfallStation "

RainfallStation describes a rainfall timeseries in mm/day at a certain
place.

Use RainfallStationReference or IDWRainfall to distribute the data
into space

C++ includes: precipitation.h ";

%feature("docstring")
cmf::atmosphere::RainfallStation::RainfallStation "RainfallStation(const RainfallStation &copy)

copy c'tor ";

%feature("docstring")  cmf::atmosphere::RainfallStation::tostring "std::string tostring() const

Returns the name and the mean yearly rainfall. ";

%feature("docstring")  cmf::atmosphere::RainfallStation::use_for_cell
"void use_for_cell(cmf::upslope::Cell &c)

Connects a cell with this rainfall station. ";


// File: classcmf_1_1atmosphere_1_1_rainfall_station_list.xml
%feature("docstring") cmf::atmosphere::RainfallStationList "

A list of rainfall stations.

C++ includes: precipitation.h ";

%feature("docstring")  cmf::atmosphere::RainfallStationList::add "RainfallStation::ptr add(std::string Name, cmf::math::timeseries Data,
cmf::geometry::point Position)

Creates a new RainfallStation and adds it to the list.

Usage: The position of the rainfall station will be used as identifier
A new rainfall station

Parameters:
-----------

Name:  Name of the station

Data:  Rainfall timeseries

Position:  Spatial position of the new station ";

%feature("docstring")  cmf::atmosphere::RainfallStationList::remove "void remove(ptrdiff_t index)

Removes the station at index from this list. ";

%feature("docstring")  cmf::atmosphere::RainfallStationList::size "size_t size() const

Returns the number of rainfall stations. ";


// File: classcmf_1_1atmosphere_1_1_rainfall_station_reference.xml
%feature("docstring") cmf::atmosphere::RainfallStationReference "

References a single RainfallStation to provide rainfall intensity
data.

C++ includes: precipitation.h ";

%feature("docstring")  cmf::atmosphere::RainfallStationReference::conc
"real conc(cmf::math::Time t, const cmf::water::solute &Solute) const

Returns the concentration of a solute in [solute unit]/m3 at time t in
rainfall. ";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::connection_to "cmf::water::flux_connection* connection_to(const cmf::water::flux_node
&target)

Returns the connection between this and target. ";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::flux3d_to "cmf::geometry::point flux3d_to(const cmf::water::flux_node &target,
cmf::math::Time t) ";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::flux_to "real
flux_to(const cmf::water::flux_node &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from this into target\") ";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::get_3d_flux "cmf::geometry::point get_3d_flux(cmf::math::Time t)

Returns the sum of all flux vectors. ";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::get_connections "cmf::water::connection_list get_connections() const ";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::get_intensity "real
get_intensity(cmf::math::Time t) const

Returns the rainfall intensity in mm/day at time t. ";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::get_potential "virtual
real get_potential(cmf::math::Time=cmf::math::never) const

Returns the water potential of the node in m waterhead.

The base class water storage always returns the height of the location
";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::get_project "cmf::project&
get_project() const

Returns the project, this node is part of. ";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::is_empty "virtual double
is_empty() const

Returns false. ";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::is_storage "virtual bool
is_storage() const

true, if this is a waterstorage ";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::RecalcFluxes "virtual bool
RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node.

WaterStorage overrides this, since state changes require an update of
the fluxes ";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::remove_connection "bool
remove_connection(cmf::water::flux_node::ptr To)

Remove the connection. ";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::set_potential "virtual
void set_potential(real new_potential)

Sets the potential of this flux node. ";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::to_string "virtual
std::string to_string() const ";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::waterbalance "real
waterbalance(cmf::math::Time t, const flux_connection *Without=0)
const

Returns the sum of all fluxes (positive and negative) at time t.

Single fluxes can be excluded from the calculation

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the waterbalance
(e.g. to prevent closed circuits) ";


// File: classcmf_1_1atmosphere_1_1_rain_source.xml
%feature("docstring") cmf::atmosphere::RainSource "

An abstract class for different types of rainfall sources.

C++ includes: precipitation.h ";

%feature("docstring")  cmf::atmosphere::RainSource::conc "virtual
real conc(cmf::math::Time t, const cmf::water::solute &Solute) const
=0

Returns the concentration of a solute in the rainfall at time t. ";

%feature("docstring")  cmf::atmosphere::RainSource::connection_to "cmf::water::flux_connection* connection_to(const cmf::water::flux_node
&target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::atmosphere::RainSource::flux3d_to "cmf::geometry::point flux3d_to(const cmf::water::flux_node &target,
cmf::math::Time t) ";

%feature("docstring")  cmf::atmosphere::RainSource::flux_to "real
flux_to(const cmf::water::flux_node &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from this into target\") ";

%feature("docstring")  cmf::atmosphere::RainSource::get_3d_flux "cmf::geometry::point get_3d_flux(cmf::math::Time t)

Returns the sum of all flux vectors. ";

%feature("docstring")  cmf::atmosphere::RainSource::get_connections "cmf::water::connection_list get_connections() const ";

%feature("docstring")  cmf::atmosphere::RainSource::get_intensity "virtual real get_intensity(cmf::math::Time t) const =0

Returns the actual rainfall intensity in mm/day. ";

%feature("docstring")  cmf::atmosphere::RainSource::get_potential "virtual real get_potential(cmf::math::Time=cmf::math::never) const

Returns the water potential of the node in m waterhead.

The base class water storage always returns the height of the location
";

%feature("docstring")  cmf::atmosphere::RainSource::get_project "cmf::project& get_project() const

Returns the project, this node is part of. ";

%feature("docstring")  cmf::atmosphere::RainSource::is_empty "virtual
double is_empty() const

Returns false. ";

%feature("docstring")  cmf::atmosphere::RainSource::is_storage "virtual bool is_storage() const

true, if this is a waterstorage ";

%feature("docstring")  cmf::atmosphere::RainSource::RecalcFluxes "virtual bool RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node.

WaterStorage overrides this, since state changes require an update of
the fluxes ";

%feature("docstring")  cmf::atmosphere::RainSource::remove_connection
"bool remove_connection(cmf::water::flux_node::ptr To)

Remove the connection. ";

%feature("docstring")  cmf::atmosphere::RainSource::set_potential "virtual void set_potential(real new_potential)

Sets the potential of this flux node. ";

%feature("docstring")  cmf::atmosphere::RainSource::to_string "virtual std::string to_string() const ";

%feature("docstring")  cmf::atmosphere::RainSource::waterbalance "real waterbalance(cmf::math::Time t, const flux_connection *Without=0)
const

Returns the sum of all fluxes (positive and negative) at time t.

Single fluxes can be excluded from the calculation

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the waterbalance
(e.g. to prevent closed circuits) ";


// File: classcmf_1_1river_1_1_reach.xml
%feature("docstring") cmf::river::Reach "

A reach represents the section of a riover and is a specialization of
an open water storage.

The OpenWaterStorage attributes and methods are extended by
topological features, for the creation of a network of reaches.

C++ includes: Reach.h ";

/*  Overrides of flux_node  */

%feature("docstring")  cmf::river::Reach::RecalcFluxes "virtual bool
RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node.

WaterStorage overrides this, since state changes require an update of
the fluxes ";

%feature("docstring")  cmf::river::Reach::is_empty "virtual double
is_empty() const

Returns true if the node has no water. ";

%feature("docstring")  cmf::river::Reach::to_string "virtual
std::string to_string() const ";

%feature("docstring")  cmf::river::Reach::~Reach "virtual ~Reach() ";

%feature("docstring")  cmf::river::Reach::add_connected_states "virtual void add_connected_states(cmf::math::StateVariable::list
&states) ";

%feature("docstring")  cmf::river::Reach::conc "real
conc(cmf::math::Time t, const cmf::water::solute &solute) const

Returns the water quality of the flux_node, if it is not overridden
this is the mix of the incoming fluxes. ";

%feature("docstring")  cmf::river::Reach::conc "real conc(const
cmf::water::solute &_Solute) const

Returns the concentration of the given solute. ";

%feature("docstring")  cmf::river::Reach::conc "void conc(const
cmf::water::solute &_Solute, real NewConcetration)

Sets a new concentration. ";

%feature("docstring")  cmf::river::Reach::connect_to_surfacewater "void connect_to_surfacewater(cmf::upslope::Cell *cell, real width,
bool diffusive)

Connects the surfacewater of a cell with this reach.

Parameters:
-----------

cell:  The cell with the surface water to be connected with this reach

width:  The flow width from cell to this reach

diffusive:  If diffusive is false, a ManningKinematic connection is
used, else a ManningDiffusive connection ";

%feature("docstring")  cmf::river::Reach::connection_to "cmf::water::flux_connection* connection_to(const cmf::water::flux_node
&target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::river::Reach::distance_to_cell "double
distance_to_cell(cmf::upslope::Cell *cell) const

Returns the distance (d) for connections between this reach and a
cell.

If the effective inner cell distance (defined as :math:`R_{Cell} = 0.5\\\\frac{\\\\sqrt{A}}{\\\\pi}`) is smaller than the distance
between the center points, the cell radius is returned ";

%feature("docstring")  cmf::river::Reach::dxdt "virtual real
dxdt(const cmf::math::Time &time)

Returns the derivate of the state variable at time time. ";

%feature("docstring")  cmf::river::Reach::flux3d_to "cmf::geometry::point flux3d_to(const cmf::water::flux_node &target,
cmf::math::Time t) ";

%feature("docstring")  cmf::river::Reach::flux_to "real flux_to(const
cmf::water::flux_node &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from this into target\") ";

%feature("docstring")  cmf::river::Reach::get_3d_flux "cmf::geometry::point get_3d_flux(cmf::math::Time t)

Returns the sum of all flux vectors. ";

%feature("docstring")  cmf::river::Reach::get_abs_errtol "real
get_abs_errtol(real rel_errtol) const ";

%feature("docstring")  cmf::river::Reach::get_connections "cmf::water::connection_list get_connections() const ";

%feature("docstring")  cmf::river::Reach::get_depth "real get_depth()
const

Returns the water table depth. ";

%feature("docstring")  cmf::river::Reach::get_diffusive "bool
get_diffusive() const

Returns if flow is calculated using a diffusive wave model. ";

%feature("docstring")  cmf::river::Reach::get_downstream "cmf::water::flux_node::ptr get_downstream() const

Returns the reach downstream of this (or null if there is no reach
downstream) ";

%feature("docstring")  cmf::river::Reach::get_height_function "virtual const IVolumeHeightFunction& get_height_function() const

The functional relation between volume, depth and exposed area. ";

%feature("docstring")  cmf::river::Reach::get_length "real
get_length() const

Returns the length of the reach. ";

%feature("docstring")  cmf::river::Reach::get_potential "real
get_potential(cmf::math::Time t=cmf::math::never) const override

Returns the water potential of the node in m waterhead.

The base class water storage always returns the height of the location
";

%feature("docstring")  cmf::river::Reach::get_project "cmf::project&
get_project() const

Returns the project, this node is part of. ";

%feature("docstring")  cmf::river::Reach::get_reachtype "Channel
get_reachtype() const

Returns the channel shape. ";

%feature("docstring")  cmf::river::Reach::get_root "ptr get_root()

Returns the reach most downstream from this reach. ";

%feature("docstring")  cmf::river::Reach::get_state "real get_state()
const

Returns the current state of the variable. ";

%feature("docstring")  cmf::river::Reach::get_state_variable_content "char get_state_variable_content() const

A character indicating the integrated variable (either 'V' for Volume
or 'h' for head) ";

%feature("docstring")  cmf::river::Reach::get_upstream "ptr
get_upstream(int index) const

Returns a reach upstream of this. ";

%feature("docstring")  cmf::river::Reach::get_volume "virtual real
get_volume() const

Returns the volume of water in this storage in m3 ";

%feature("docstring")  cmf::river::Reach::get_width "real get_width()
const

Returns the average width of the reach. ";

%feature("docstring")  cmf::river::Reach::is_connected "virtual bool
is_connected(const cmf::math::StateVariable &other) const

Returns True if this waterstorage is effected by another state. ";

%feature("docstring")  cmf::river::Reach::is_storage "virtual bool
is_storage() const

Returns true, since this is a storage. ";

%feature("docstring")  cmf::river::Reach::remove_connection "bool
remove_connection(cmf::water::flux_node::ptr To)

Remove the connection. ";

%feature("docstring")  cmf::river::Reach::set_dead_end "void
set_dead_end()

Deletes any downstream connection. ";

%feature("docstring")  cmf::river::Reach::set_depth "void
set_depth(real new_depth) ";

%feature("docstring")  cmf::river::Reach::set_diffusive "void
set_diffusive(bool use_diffusive_wave)

Sets all Manning kind connections to either diffusive or kinematic. ";

%feature("docstring")  cmf::river::Reach::set_downstream "void
set_downstream(ptr new_downstream, bool use_meanchannel=false)

Connects the reach to another one downstream. ";

%feature("docstring")  cmf::river::Reach::set_height_function "virtual void set_height_function(const IVolumeHeightFunction &val) ";

%feature("docstring")  cmf::river::Reach::set_height_function "void
set_height_function(const IChannel &val)

Sets the channel shape. ";

%feature("docstring")  cmf::river::Reach::set_outlet "void
set_outlet(cmf::water::flux_node::ptr outlet)

Connects the reach to an outlet, e.g. a boundary condition. ";

%feature("docstring")  cmf::river::Reach::set_potential "void
set_potential(real newpotential) override

Sets the potential of this flux node. ";

%feature("docstring")  cmf::river::Reach::set_state "void
set_state(real newState)

Gives access to the state variable. ";

%feature("docstring")  cmf::river::Reach::set_state_variable_content "void set_state_variable_content(char content)

A character indicating the integrated variable (either 'V' for Volume
or 'h' for head) ";

%feature("docstring")  cmf::river::Reach::set_volume "virtual void
set_volume(real newwatercontent)

Sets the volume of water in this storage in m3 ";

%feature("docstring")  cmf::river::Reach::Solute "SoluteStorage&
Solute(const cmf::water::solute _Solute)

Returns the water quality of the water storage. ";

%feature("docstring")  cmf::river::Reach::Solute "const
SoluteStorage& Solute(const cmf::water::solute _Solute) const ";

%feature("docstring")  cmf::river::Reach::upstream_count "int
upstream_count() const

Returns the number of reaches upstream of this. ";

%feature("docstring")  cmf::river::Reach::waterbalance "real
waterbalance(cmf::math::Time t, const flux_connection *Without=0)
const

Returns the sum of all fluxes (positive and negative) at time t.

Single fluxes can be excluded from the calculation

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the waterbalance
(e.g. to prevent closed circuits) ";

%feature("docstring")  cmf::river::Reach::wet_area "real wet_area()
const

Returns the exposed surface area in m2. ";


// File: classcmf_1_1river_1_1_reach_iterator.xml
%feature("docstring") cmf::river::ReachIterator "

An iterator over every upstream reach from a start reach.

Implements both the Python and the C++ iterator interface Usage C++:
Usage Python:

C++ includes: Reach.h ";

%feature("docstring")  cmf::river::ReachIterator::ReachIterator "ReachIterator(Reach::ptr first)

Creates a ReachIterator from a first reach. ";

%feature("docstring")  cmf::river::ReachIterator::next "Reach::ptr
next()

Returns the next reach in the upstream queue. ";

%feature("docstring")  cmf::river::ReachIterator::position "double
position() const

Returns the distance to the root reach. ";

%feature("docstring")  cmf::river::ReachIterator::reach "Reach::ptr
reach() const

Returns the current reach. ";

%feature("docstring")  cmf::river::ReachIterator::valid "bool valid()
const

Returns true, if reaches are left to iterate over. ";


// File: classcmf_1_1river_1_1_rectangular_reach.xml
%feature("docstring") cmf::river::RectangularReach "

Describes a IChannel with a rectangular crosssection.

C++ includes: ReachType.h ";

%feature("docstring")  cmf::river::RectangularReach::RectangularReach
"RectangularReach(double l, double width)

Creates a new rectangular reach type with width [m]. ";

%feature("docstring")  cmf::river::RectangularReach::RectangularReach
"RectangularReach(const RectangularReach &copy) ";

%feature("docstring")  cmf::river::RectangularReach::~RectangularReach
"virtual ~RectangularReach() ";

%feature("docstring")  cmf::river::RectangularReach::A "virtual
double A(double V) const

Returns the area of the surface for a given volume. ";

%feature("docstring")  cmf::river::RectangularReach::copy "RectangularReach* copy() const ";

%feature("docstring")  cmf::river::RectangularReach::get_channel_width
"virtual double get_channel_width(double depth) const

Returns the width of the stream at a given depth 

.. math::

    
    w &=& const 

. ";

%feature("docstring")  cmf::river::RectangularReach::get_depth "virtual double get_depth(double area) const

Returns the depth at a given crossection area 

.. math::

     d
    &=& \\\\frac A w 

. ";

%feature("docstring")
cmf::river::RectangularReach::get_flux_crossection "virtual double
get_flux_crossection(double depth) const

Returns the crossection area at a given depth 

.. math::

     A
    &=& d w 

. ";

%feature("docstring")  cmf::river::RectangularReach::get_length "double get_length() const

Length of the reach. ";

%feature("docstring")  cmf::river::RectangularReach::get_nManning "virtual double get_nManning() const ";

%feature("docstring")
cmf::river::RectangularReach::get_wetted_perimeter "virtual double
get_wetted_perimeter(double depth) const

Returns the wetted perimeter at a given depth 

.. math::

     P
    &=& 2 d + w 

. ";

%feature("docstring")  cmf::river::RectangularReach::h "virtual
double h(double V) const

Returns the depth of a given volume. ";

%feature("docstring")  cmf::river::RectangularReach::q "virtual
double q(double h, double slope) const ";

%feature("docstring")  cmf::river::RectangularReach::qManning "virtual double qManning(double A, double slope) const

Calculates the flow rate from a given water volume in the reach


.. math::

     q_{Manning}&=& A R^{\\\\frac 2 3}
    \\\\sqrt{\\\\frac {\\\\Delta_z} n} \\\\\\\\ A &=& \\\\frac V l
    \\\\mbox{, (Crosssectional area of the wetted crossection, Volume per
    length)} \\\\\\\\ R &=& \\\\frac A {P(d)} \\\\\\\\ P(d) &=& \\\\mbox{
    the perimeter of the wetted crosssection, a function of reach depth}
    \\\\\\\\ d(V) &=& \\\\mbox{ the depth of the reach a function of the
    volume} \\\\\\\\ \\\\Delta_z &=& \\\\frac{z_{max} - z_{min}}{l}
    \\\\mbox{ Slope of the reach} 

.

Flow rate [m3/s]

Parameters:
-----------

A:  The area of the cross section [m2]

slope:  The slope of the reach [m/m] ";

%feature("docstring")  cmf::river::RectangularReach::set_nManning "virtual void set_nManning(double nManning) ";

%feature("docstring")  cmf::river::RectangularReach::typecode "char
typecode() const ";

%feature("docstring")  cmf::river::RectangularReach::V "virtual
double V(double h) const ";


// File: classcmf_1_1upslope_1_1_retention_curve.xml
%feature("docstring") cmf::upslope::RetentionCurve "

Abstract base class for different types of retention curves.

This class, and its children uses wetness instead of volumetric water
content. The wetness of a soil is defined as water content per void
volume

C++ includes: RetentionCurve.h ";

%feature("docstring")  cmf::upslope::RetentionCurve::~RetentionCurve "virtual ~RetentionCurve() ";

%feature("docstring")  cmf::upslope::RetentionCurve::copy "virtual
RetentionCurve* copy() const =0 ";

%feature("docstring")  cmf::upslope::RetentionCurve::Diffusivity "virtual real Diffusivity(real wetness) const

Returns the Diffusivity of the soil.

Not implemented for all retention curves. Diffusivity is used by
MACROlikeMacroMicroExchange ";

%feature("docstring")  cmf::upslope::RetentionCurve::Diffusivity "cmf::math::num_array Diffusivity(cmf::math::num_array &wetness) ";

%feature("docstring")  cmf::upslope::RetentionCurve::dPsiM_dW "virtual real dPsiM_dW(real wetness) const ";

%feature("docstring")  cmf::upslope::RetentionCurve::dPsiM_dW "cmf::math::num_array dPsiM_dW(const cmf::math::num_array &wetness)
const ";

%feature("docstring")  cmf::upslope::RetentionCurve::FillHeight "virtual real FillHeight(real lowerDepth, real Area, real Volume) const

Returns the thickness of a soil column with a certain pore volume. ";

%feature("docstring")  cmf::upslope::RetentionCurve::K "virtual real
K(real wetness) const

Returns the conductivity in m/day at a certain depth and water
content. ";

%feature("docstring")  cmf::upslope::RetentionCurve::K "cmf::math::num_array K(const cmf::math::num_array &wetness) const ";

%feature("docstring")  cmf::upslope::RetentionCurve::MatricPotential "virtual real MatricPotential(real wetness) const

returns the wetness of the soil at given water content ";

%feature("docstring")  cmf::upslope::RetentionCurve::MatricPotential "cmf::math::num_array MatricPotential(const cmf::math::num_array
&wetness) const ";

%feature("docstring")  cmf::upslope::RetentionCurve::Porosity "virtual real Porosity(real depth=0.0) const =0

Returns the porosity at a certain depth. ";

%feature("docstring")  cmf::upslope::RetentionCurve::theta "virtual
real theta(real wetness) const

returns the water content :math:`theta` for a given wetness ";

%feature("docstring")  cmf::upslope::RetentionCurve::theta "cmf::math::num_array theta(const cmf::math::num_array &wetness) const
";

%feature("docstring")  cmf::upslope::RetentionCurve::VoidVolume "virtual real VoidVolume(real upperDepth, real lowerDepth, real Area)
const

Returns the void volume of a soil column. ";

%feature("docstring")  cmf::upslope::RetentionCurve::Wetness "virtual
real Wetness(real suction) const

returns the wetness (volumetric water content per pore space) at a
given suction pressure ";

%feature("docstring")  cmf::upslope::RetentionCurve::Wetness "cmf::math::num_array Wetness(const cmf::math::num_array &suction)
const ";

%feature("docstring")  cmf::upslope::RetentionCurve::Wetness_eff "virtual real Wetness_eff(real wetness, real pF_r=4.2) const

Returns the effective wetness, using a residual pF value 

.. math::

    w_{eff}
    =
    \\\\frac{w_{act}-w\\\\left(pF_r\\\\right)}{1-w\\\\left(pF_r\\\\right)}

.
";

%feature("docstring")  cmf::upslope::RetentionCurve::Wetness_pF "real
Wetness_pF(real pF) const

returns the volumetric water content at a given pF value ";

%feature("docstring")  cmf::upslope::RetentionCurve::Wetness_pF "cmf::math::num_array Wetness_pF(const cmf::math::num_array &pF) const
";


// File: classcmf_1_1upslope_1_1connections_1_1_richards.xml
%feature("docstring") cmf::upslope::connections::Richards "

Calculates flow according to the Richards equation.



.. math::

     q_{Richards} &=&
    \\\\frac{\\\\Delta\\\\Psi_{tot}}{d} K(\\\\theta) A \\\\\\\\
    \\\\Psi_{tot} &= &\\\\Psi_{M}(\\\\theta) + h 

 where
:math:`\\\\Delta\\\\Psi_{tot} [m]` is the difference of the total water
potentials of the two soil layers

:math:`d [m]` is the distance between the two soil layers

:math:`K(\\\\theta)\\\\left[\\\\frac m{day}\\\\right]` is the geometric
mean conductivity (see SoilType::Kunsat)

:math:`A [m^2]` is the crosssectional area of the flux

:math:`\\\\Psi_M(\\\\theta) [m]` is the matrix potential (see
SoilType::MatrixPotential)

:math:`h [m]` is the height of a soil layer above sea level

C++ includes: Percolation.h ";

%feature("docstring")  cmf::upslope::connections::Richards::Richards "Richards(cmf::upslope::SoilLayer::ptr left, cmf::water::flux_node::ptr
right)

Creates a Richards equation connection between two soil layers (left
and right, rather top and bottom) of the same cell. ";

%feature("docstring")  cmf::upslope::connections::Richards::conc "real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::Richards::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::connections::Richards::get_ptr "ptr get_ptr() const ";

%feature("docstring")  cmf::upslope::connections::Richards::get_target
"flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::connections::Richards::get_target
"flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::Richards::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::Richards::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::upslope::connections::Richards::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::upslope::connections::Richards::left_node
"flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::connections::Richards::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::connections::Richards::refresh "void refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")  cmf::upslope::connections::Richards::right_node
"flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::Richards::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::Richards::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::connections::Richards::short_string "virtual
std::string short_string() const ";

%feature("docstring")  cmf::upslope::connections::Richards::to_string
"virtual std::string to_string() const ";


// File: classcmf_1_1upslope_1_1connections_1_1_richards__lateral.xml
%feature("docstring") cmf::upslope::connections::Richards_lateral "

Calculates the flux using Richard's equation for adjacent layers 

.. math::

    
    q_{lat} = \\\\frac{\\\\Psi_1 - \\\\Psi_2}{\\\\|C_1-C_2\\\\|}
    K(\\\\theta) A 

where:

:math:`q_{lat}` the lateral flow in :math:`m^3/day`

:math:`\\\\Psi_i` the head of node i

:math:`\\\\|C_1-C_2\\\\|` is the distance from Cell 1 to Cell 2

:math:`K(\\\\theta_{1,2}) = \\\\sqrt{K(\\\\theta_1) K(\\\\theta_2)}`

:math:`A` the crosssectional area of the interface between storages 1 and 2

C++ includes: subsurfacefluxes.h ";

%feature("docstring")
cmf::upslope::connections::Richards_lateral::Richards_lateral "Richards_lateral(cmf::upslope::SoilLayer::ptr left,
cmf::water::flux_node::ptr right, real FlowWidth=0, real Distance=0)

Creates the connection.

Parameters:
-----------

left:  Left node of the connection (needs to be soil water storage)

right:  Right node of the connection (can be any node)

FlowWidth:  the width of the connection - is multiplied by layer
thickness to get the interface area

Distance:  the length of the connection. If 0, the distance is
calculated from the position of the nodes ";

%feature("docstring")
cmf::upslope::connections::Richards_lateral::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::Richards_lateral::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")
cmf::upslope::connections::Richards_lateral::get_ptr "ptr get_ptr()
const ";

%feature("docstring")
cmf::upslope::connections::Richards_lateral::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::Richards_lateral::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::Richards_lateral::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::Richards_lateral::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")
cmf::upslope::connections::Richards_lateral::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::Richards_lateral::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::connections::Richards_lateral::q
"real q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::Richards_lateral::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::upslope::connections::Richards_lateral::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::Richards_lateral::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::Richards_lateral::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::connections::Richards_lateral::short_string "virtual
std::string short_string() const ";

%feature("docstring")
cmf::upslope::connections::Richards_lateral::to_string "virtual
std::string to_string() const ";


// File: classcmf_1_1math_1_1_r_k_f_integrator.xml
%feature("docstring") cmf::math::RKFIntegrator "

Integrates a vector of cmf::math::StateVariable with the Runge-Kutta-
Fehlberg (RKF54) method.

C++ includes: RKFintegrator.h ";

/*  Accuracy parameters  */

/*  model time  */

%feature("docstring")  cmf::math::RKFIntegrator::get_t "cmf::math::Time get_t() const

Returns the current model time. ";

%feature("docstring")  cmf::math::RKFIntegrator::set_t "void
set_t(cmf::math::Time val)

Sets the current model time. ";

%feature("docstring")  cmf::math::RKFIntegrator::get_dt "cmf::math::Time get_dt() const

Returns the last time step. ";

/*  Integrate  */

%feature("docstring")  cmf::math::RKFIntegrator::integrate_until "void integrate_until(cmf::math::Time t_max, cmf::math::Time dt=Time(),
bool reset=false)

Integrates the vector of state variables until t_max.

Parameters:
-----------

t_max:   Time, the solver should run to

dt:   Time step (may be omitted)

reset:  If true, solver is reseted before integration starts ";

%feature("docstring")  cmf::math::RKFIntegrator::RKFIntegrator "RKFIntegrator(const cmf::math::state_list &states, real epsilon=1e-9,
cmf::math::Time dt_min=cmf::math::timespan(1000))

Constructs a new RKFIntegrator from a pointer to a vector of state
variables.

The RKF Integrator becomes the owner of states

Parameters:
-----------

states:  Statevariables of the system

epsilon:  relative error tolerance per time step (default=1e-9)

dt_min:  minimum time step (default=1s) ";

%feature("docstring")  cmf::math::RKFIntegrator::RKFIntegrator "RKFIntegrator(real epsilon=1e-9, cmf::math::Time
dt_min=cmf::math::timespan(1000))

Constructs a new RKFIntegrator.

Parameters:
-----------

epsilon:  relative error tolerance per time step (default=1e-9)

dt_min:  minimum time step (default=1s) ";

%feature("docstring")  cmf::math::RKFIntegrator::copy "virtual
Integrator* copy() const

Polymorphic copy constructor. ";

%feature("docstring")  cmf::math::RKFIntegrator::integrate "int
integrate(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables.

Parameters:
-----------

MaxTime:  To stop the model (if running in a model framework) at time
steps of value exchange e.g. full hours, the next value exchange time
can be given

TimeStep:  Takes the proposed timestep, and changes it into the
effictivly used timestep according to the local stiffness of the
problem and MaxTime ";

%feature("docstring")  cmf::math::RKFIntegrator::reset "void reset()
override

Resets any saved history (for multistep methods) ";

%feature("docstring")  cmf::math::RKFIntegrator::set_system "void
set_system(const state_list &states) ";

%feature("docstring")  cmf::math::RKFIntegrator::size "size_t size()
const ";

%feature("docstring")  cmf::math::RKFIntegrator::to_string "std::string to_string() const override

Desrcibes the iterator. ";


// File: classcmf_1_1upslope_1_1_e_t_1_1_root_uptake_stress_function.xml
%feature("docstring") cmf::upslope::ET::RootUptakeStressFunction "

An abstract class to calculate the actual transpiration from potential
transpiration.

Implementations of WaterStressFunction are used by ET connections
derived from cmf::upslope::ET::stressedET

C++ includes: waterstress.h ";

%feature("docstring")
cmf::upslope::ET::RootUptakeStressFunction::~RootUptakeStressFunction
"virtual ~RootUptakeStressFunction() ";

%feature("docstring")
cmf::upslope::ET::RootUptakeStressFunction::copy "virtual
cmf::upslope::ET::RootUptakeStressFunction* copy() const =0

Creates a new copy of this wetness. ";

%feature("docstring")
cmf::upslope::ET::RootUptakeStressFunction::Tact "virtual real
Tact(const cmf::upslope::SoilLayer *soillayer, real Tpot) const =0

Calculates the water stress for a layer.

Parameters:
-----------

soillayer:  The cmf::upslope::SoilLayer this stress function belongs
to

Tpot:  Potential Transpiration in mm/day (for the full profile)

Actual flux from layer in m3/day ";

%feature("docstring")
cmf::upslope::ET::RootUptakeStressFunction::to_string "virtual
std::string to_string() const =0 ";


// File: classcmf_1_1upslope_1_1connections_1_1_rutter_interception.xml
%feature("docstring") cmf::upslope::connections::RutterInterception "

Interception storage overflow according to the Rutter and Morton
(1977) model.

Calculates the interception overflow as a storage depending fraction
of incoming rainfall The Rutter model of interception reads as follows
after Meuser, A., 1990. Effects of afforestation on run-off
characteristics. Agric. For. Meteorol. 50: 125-138.:


.. math::

    \\\\frac{dI_C(t)}{dt}=P(t)(1-p_F-p_S)-P(t)(1-p_F-
    p_S)\\\\frac{I_C(t)}{I_CMAX}-f_I(E-e)(t)

With :math:`I_C` the current
canopy storage and :math:`P(t)` the current rainfall.

The second term of the equation denotes the flux from the canopy to
the ground. The implemented formula for canopy storage overflow reads
then as: 

.. math::

    q_{CO}(t) =
    P_{net}(t)\\\\frac{V_{act}[mm]}{V_{max}[mm]}

With:
:math:`q_{CO}(t)[\\\\frac{m^3}{day}]`: The flux from canopy to the ground

:math:`P_{net}(t)[\\\\frac{m^3}{day}]`: The flux from the rain to the canopy

:math:`V_{act}[mm]=1000 [mm/m] \\\\frac{V_{canopy}[m^3]}{A_{cell} [m^2]}`
The stored water of the canopy in mm

:math:`V_{max}[mm]=c_{LAI}[mm]\\\\cdot LAI` The capacity of the canopy in
mm, defined by the factor CanopyCapacityPerLAI [mm/LAI], and the leaf
area index LAI. (see: cmf::upslope::vegetation::Vegetation)

C++ includes: surfacefluxes.h ";

%feature("docstring")
cmf::upslope::connections::RutterInterception::RutterInterception "RutterInterception(cmf::water::WaterStorage::ptr Canopy,
cmf::water::flux_node::ptr target, cmf::upslope::Cell &cell) ";

%feature("docstring")
cmf::upslope::connections::RutterInterception::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::RutterInterception::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")
cmf::upslope::connections::RutterInterception::get_ptr "ptr get_ptr()
const ";

%feature("docstring")
cmf::upslope::connections::RutterInterception::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::RutterInterception::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::RutterInterception::get_tracer_filter "real get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::RutterInterception::get_tracer_filter "real get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")
cmf::upslope::connections::RutterInterception::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::RutterInterception::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::connections::RutterInterception::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::RutterInterception::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::upslope::connections::RutterInterception::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::RutterInterception::set_tracer_filter "void set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::RutterInterception::set_tracer_filter "void set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::connections::RutterInterception::short_string "virtual
std::string short_string() const ";

%feature("docstring")
cmf::upslope::connections::RutterInterception::to_string "virtual
std::string to_string() const ";


// File: classcmf_1_1draw_1_1shapemap_1_1_shape_map.xml
%feature("docstring") cmf::draw::shapemap::ShapeMap "

self.fc_function: A callable taking a feature and returning a color
(fillcolor) self.lw_function: A callable taking a feature and
returning a scalar (line width) ";

%feature("docstring")  cmf::draw::shapemap::ShapeMap::__init__ "def
__init__(self, features, kwargs) ";

%feature("docstring")  cmf::draw::shapemap::ShapeMap::refresh "def
refresh(self) ";


// File: classcmf_1_1upslope_1_1_e_t_1_1_shuttleworth_wallace.xml
%feature("docstring") cmf::upslope::ET::ShuttleworthWallace "

Calculates the sum of soil evaporation and transpiration according to
Shuttleworth & Wallace 1985, as implemented in BROOK 90 (Federer 1990)

The difference to BROOK90 is, that the actual transpiration is not
calculated by plant resitance and potential gradient between plant and
soil, but by an piecewise linear function of the pF value :math:`pF = \\\\log_{10}\\\\left(-\\\\Psi [hPa]\\\\right)`: 

.. math::

    
    \\\\frac{T_{act}}{T_{pot}} = \\\\begin{cases}1 & pF \\\\le 3.35
    \\\\\\\\ \\\\frac{pF - 4.2}{3.35 - 4.2} & pF \\\\in [3.35 .. 4.2]
    \\\\\\\\ 0 & pF \\\\ge 4.2\\\\end{cases} 

Calculation procedure, as in BROOK 90:

Evapotranspiration from the canopy: :math:`\\\\lambda ET_{canopy} = \\\\frac{r_{ac} \\\\Delta\\\\ R_{n,canopy} + c_p\\\\rho D_0}{\\\\Delta \\\\gamma r_{ac} + \\\\gamma r_{sc}}`

Evaporation from the ground: :math:`\\\\lambda E_{ground} = \\\\frac{r_{as} \\\\Delta\\\\ R_{n,ground} + c_p\\\\rho D_0}{\\\\Delta \\\\gamma r_{as} + \\\\gamma r_{ss}}`

In case of a complete surface water covered ground, the surface
resistance :math:`r_{ss}` becomes 0. (GIR)

with  :math:`\\\\Delta = \\\\frac{de_s}{dT} = 4098\\\\ 0.6108 \\\\exp\\\\left(\\\\frac{17.27 T}{T+237.3}\\\\right)(T+237.3)^{-2}`,
the slope of the sat. vap. press. T function

:math:`R_{n,ground} = R_n \\\\exp(-C_R LAI)`, the net radiation flux to
the ground

:math:`R_{n,canopy} = R_n - R_{n,ground}`, the net radiation flux to the
canopy

:math:`\\\\lambda,c_p\\\\rho,\\\\gamma` latent heat of vaporization, heat
capacity of air, psychrometer constant

:math:`D_0` vapor pressure deficit at effective source height, see
function D0

:math:`r_{ac}, r_{sc}, r_{as}, r_{ss}` Resistances for the vapor pressure
(see below)

C++ includes: ShuttleworthWallace.h ";

%feature("docstring")
cmf::upslope::ET::ShuttleworthWallace::ShuttleworthWallace "ShuttleworthWallace(cmf::upslope::Cell &cell, bool allow_dew=true)

Calculates the transpiration and the soil evaporation from dry
surfaces. ";

%feature("docstring")
cmf::upslope::ET::ShuttleworthWallace::~ShuttleworthWallace "~ShuttleworthWallace() ";

%feature("docstring")
cmf::upslope::ET::ShuttleworthWallace::evap_from_canopy "double
evap_from_canopy(cmf::water::WaterStorage::ptr canopy, cmf::math::Time
t) ";

%feature("docstring")
cmf::upslope::ET::ShuttleworthWallace::evap_from_layer "double
evap_from_layer(cmf::upslope::SoilLayer::ptr sl, cmf::math::Time t) ";

%feature("docstring")
cmf::upslope::ET::ShuttleworthWallace::evap_from_snow "double
evap_from_snow(cmf::water::WaterStorage::ptr snow, cmf::math::Time t)
";

%feature("docstring")
cmf::upslope::ET::ShuttleworthWallace::evap_from_surfacewater "double
evap_from_surfacewater(cmf::river::OpenWaterStorage::ptr ows,
cmf::math::Time t) ";

%feature("docstring")
cmf::upslope::ET::ShuttleworthWallace::get_aerodynamic_resistance "virtual void get_aerodynamic_resistance(double &r_ag, double &r_ac,
cmf::math::Time t) const

aerodynamic resistance from ground to atmosphere (r_ag) and from
canopy to atmosphere (r_ac) ";

%feature("docstring")  cmf::upslope::ET::ShuttleworthWallace::refresh
"void refresh(cmf::math::Time t)

Calculates all the values. ";

%feature("docstring")  cmf::upslope::ET::ShuttleworthWallace::refresh
"void refresh() ";

%feature("docstring")
cmf::upslope::ET::ShuttleworthWallace::transp_from_layer "double
transp_from_layer(cmf::upslope::SoilLayer::ptr sl, cmf::math::Time t)
";


// File: classcmf_1_1math_1_1root__finding_1_1sign__error.xml
%feature("docstring") cmf::math::root_finding::sign_error "";

%feature("docstring")  cmf::math::root_finding::sign_error::sign_error
"sign_error(const std::string &msg) ";


// File: classcmf_1_1maps_1_1_simple_quad_tree.xml
%feature("docstring") cmf::maps::SimpleQuadTree "";

%feature("docstring")  cmf::maps::SimpleQuadTree::__init__ "def
__init__(self, dx=20, dy=20) ";

%feature("docstring")  cmf::maps::SimpleQuadTree::add_object "def
add_object(self, object, bounds) ";

%feature("docstring")  cmf::maps::SimpleQuadTree::get_objects "def
get_objects(self, bounds) ";


// File: classcmf_1_1upslope_1_1connections_1_1_snowfall.xml
%feature("docstring") cmf::upslope::connections::Snowfall "

A connection routing snowfall (precipitation below freezing Temp) to
the snow pack.

C++ includes: AtmosphericFluxes.h ";

%feature("docstring")  cmf::upslope::connections::Snowfall::Snowfall "Snowfall(cmf::water::flux_node::ptr target, cmf::upslope::Cell &cell)
";

%feature("docstring")  cmf::upslope::connections::Snowfall::conc "real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::Snowfall::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::connections::Snowfall::get_ptr "ptr get_ptr() const ";

%feature("docstring")  cmf::upslope::connections::Snowfall::get_target
"flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::connections::Snowfall::get_target
"flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::Snowfall::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::Snowfall::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::upslope::connections::Snowfall::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::upslope::connections::Snowfall::left_node
"flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::connections::Snowfall::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::connections::Snowfall::refresh "void refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")  cmf::upslope::connections::Snowfall::right_node
"flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::Snowfall::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::Snowfall::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::connections::Snowfall::short_string "virtual
std::string short_string() const ";

%feature("docstring")  cmf::upslope::connections::Snowfall::to_string
"virtual std::string to_string() const ";


// File: classcmf_1_1upslope_1_1_soil_layer.xml
%feature("docstring") cmf::upslope::SoilLayer "

A representation of a SoilLayer.

C++ includes: SoilLayer.h ";

/*  Overrides of flux_node  */

%feature("docstring")  cmf::upslope::SoilLayer::RecalcFluxes "virtual
bool RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node.

WaterStorage overrides this, since state changes require an update of
the fluxes ";

%feature("docstring")  cmf::upslope::SoilLayer::is_empty "virtual
double is_empty() const

Returns true if the node has no water. ";

%feature("docstring")  cmf::upslope::SoilLayer::to_string "virtual
std::string to_string() const ";

%feature("docstring")  cmf::upslope::SoilLayer::add_connected_states "virtual void add_connected_states(cmf::math::StateVariable::list
&states) ";

%feature("docstring")  cmf::upslope::SoilLayer::conc "real conc(const
cmf::water::solute &_Solute) const

Returns the concentration of the given solute. ";

%feature("docstring")  cmf::upslope::SoilLayer::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute) const
override

Returns the current WaterQuality (concentration of all solutes) ";

%feature("docstring")  cmf::upslope::SoilLayer::conc "void conc(const
cmf::water::solute &_Solute, real NewConcetration)

Sets a new concentration. ";

%feature("docstring")  cmf::upslope::SoilLayer::connection_to "cmf::water::flux_connection* connection_to(const cmf::water::flux_node
&target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::upslope::SoilLayer::dxdt "virtual real
dxdt(const cmf::math::Time &time)

Returns the derivate of the state variable at time time. ";

%feature("docstring")  cmf::upslope::SoilLayer::flux3d_to "cmf::geometry::point flux3d_to(const cmf::water::flux_node &target,
cmf::math::Time t) ";

%feature("docstring")  cmf::upslope::SoilLayer::flux_to "real
flux_to(const cmf::water::flux_node &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from this into target\") ";

%feature("docstring")  cmf::upslope::SoilLayer::get_3d_flux "cmf::geometry::point get_3d_flux(cmf::math::Time t)

Returns the sum of all flux vectors. ";

%feature("docstring")  cmf::upslope::SoilLayer::get_abs_errtol "real
get_abs_errtol(real rel_errtol) const ";

%feature("docstring")  cmf::upslope::SoilLayer::get_capacity "virtual
real get_capacity() const

Returns the capacity of the water storage in m3. ";

%feature("docstring")  cmf::upslope::SoilLayer::get_connections "cmf::water::connection_list get_connections() const ";

%feature("docstring")  cmf::upslope::SoilLayer::get_flow_crosssection
"real get_flow_crosssection(const cmf::upslope::SoilLayer &target,
bool HorizontalLayers=false) const

Calculates the shared crosssectional area of this and another soil
water storage.

If both layers belong to the same cell, the area of the cell is
returned, if they belong to different cells the area of the vertical
shared boundary is returned get_area in m2

Parameters:
-----------

target:  The other soil water storage

HorizontalLayers:  If true, the layers are assumed to be parallel to
the gravitational potential, otherwise they are assumed to be parallel
to the ground topography ";

%feature("docstring")
cmf::upslope::SoilLayer::get_gravitational_potential "real
get_gravitational_potential() const

Gravitational get_potential in m, reference height is sea level.

If the layer is saturated, it returns the saturated depth above sea
level, otherwise it returns the upperboundary of the layer 

.. math::

    
    \\\\Psi_G=h 

";

%feature("docstring")  cmf::upslope::SoilLayer::get_ice_fraction "real get_ice_fraction() const

real (Ice_fraction) ";

%feature("docstring")  cmf::upslope::SoilLayer::get_K "real get_K()
const

Returns the actual isotropic conductivity using the function from soil
:math:`\\\\frac{m}{day}`. ";

%feature("docstring")  cmf::upslope::SoilLayer::get_K "virtual real
get_K(cmf::geometry::point direction) const

Returns the actual anisotropic conductivity along a direction :math:`K = (k_f \\\\cdot d) K`. ";

%feature("docstring")  cmf::upslope::SoilLayer::get_Ksat "real
get_Ksat() const ";

%feature("docstring")  cmf::upslope::SoilLayer::get_lower "ptr
get_lower() const ";

%feature("docstring")  cmf::upslope::SoilLayer::get_lower_boundary "virtual real get_lower_boundary() const

Returns the lower boundary of the water storage below ground in m. ";

%feature("docstring")  cmf::upslope::SoilLayer::get_matrix_potential "virtual real get_matrix_potential() const

Calls RetentionCurve::Matrixpotential. ";

%feature("docstring")  cmf::upslope::SoilLayer::get_porosity "real
get_porosity() const

Returns the mean porosity in the layer. ";

%feature("docstring")  cmf::upslope::SoilLayer::get_potential "real
get_potential(cmf::math::Time t=cmf::math::never) const override

Returns the total potential in m 

.. math::

     \\\\Psi = \\\\Psi_M + \\\\Psi_G
    

. ";

%feature("docstring")  cmf::upslope::SoilLayer::get_project "cmf::project& get_project() const

Returns the project, this node is part of. ";

%feature("docstring")  cmf::upslope::SoilLayer::get_rootfraction "real get_rootfraction() const

Returns the root fraction of the layer. If it is not explicitly set,
it uses the parameters of the vegetation object of the cell. ";

%feature("docstring")  cmf::upslope::SoilLayer::get_saturated_depth "virtual real get_saturated_depth() const

Returns the depth for saturation 

.. math::

     z_{sat,this} =
    \\\\left\\\\{z_{cell}-\\\\Psi_{tot} \\\\mbox{ if } W<1 \\\\\\\\
    z_{sat,upper layer} \\\\right. 

. ";

%feature("docstring")  cmf::upslope::SoilLayer::get_soil "virtual
cmf::upslope::RetentionCurve& get_soil() const

Returns the soil properties of the water storage. ";

%feature("docstring")  cmf::upslope::SoilLayer::get_state "real
get_state() const

Returns the current state of the variable. ";

%feature("docstring")
cmf::upslope::SoilLayer::get_state_variable_content "char
get_state_variable_content() const

A character indicating the integrated variable (either 'V' for Volume
or 'h' for head) ";

%feature("docstring")  cmf::upslope::SoilLayer::get_Tact "double
get_Tact(double Tpot)

Returns a factor to indicate the draught stress to be multiplied with
ETpot.

1 = no stress, 0 = no uptake possible ";

%feature("docstring")  cmf::upslope::SoilLayer::get_theta "virtual
real get_theta() const

Returns the actual volumetric water content of the water storage. ";

%feature("docstring")  cmf::upslope::SoilLayer::get_thickness "real
get_thickness() const ";

%feature("docstring")  cmf::upslope::SoilLayer::get_upper "ptr
get_upper() const ";

%feature("docstring")  cmf::upslope::SoilLayer::get_upper_boundary "virtual real get_upper_boundary() const

Returns the upper boundary of the water storage below ground in m. ";

%feature("docstring")  cmf::upslope::SoilLayer::get_volume "virtual
real get_volume() const

Returns the volume of water in this storage in m3 ";

%feature("docstring")  cmf::upslope::SoilLayer::get_wetness "virtual
real get_wetness() const

Returns the wetness of the soil :math:`\\\\frac{V_{H_2O}}{V_{pores}}`. ";

%feature("docstring")  cmf::upslope::SoilLayer::is_connected "virtual
bool is_connected(const cmf::math::StateVariable &other) const

Returns True if this waterstorage is effected by another state. ";

%feature("docstring")  cmf::upslope::SoilLayer::is_storage "virtual
bool is_storage() const

Returns true, since this is a storage. ";

%feature("docstring")  cmf::upslope::SoilLayer::remove_connection "bool remove_connection(cmf::water::flux_node::ptr To)

Remove the connection. ";

%feature("docstring")  cmf::upslope::SoilLayer::set_ice_fraction "void set_ice_fraction(real val)

Parameters:
-----------

val:  Ice_fraction (real) ";

%feature("docstring")  cmf::upslope::SoilLayer::set_potential "virtual void set_potential(real waterhead)

Sets the potential of this soil water storage. ";

%feature("docstring")
cmf::upslope::SoilLayer::set_root_uptake_stress_function "void
set_root_uptake_stress_function(const
cmf::upslope::ET::RootUptakeStressFunction &stressfunction)

Sets the root uptake stress function. ";

%feature("docstring")  cmf::upslope::SoilLayer::set_rootfraction "void set_rootfraction(real rootfraction)

Sets the root fraction in this layer explicitly. ";

%feature("docstring")  cmf::upslope::SoilLayer::set_soil "virtual
void set_soil(const cmf::upslope::RetentionCurve &r_curve) ";

%feature("docstring")  cmf::upslope::SoilLayer::set_state "void
set_state(real newState)

Gives access to the state variable. ";

%feature("docstring")
cmf::upslope::SoilLayer::set_state_variable_content "void
set_state_variable_content(char content)

A character indicating the integrated variable (either 'V' for Volume
or 'h' for head) ";

%feature("docstring")  cmf::upslope::SoilLayer::set_theta "virtual
void set_theta(real Value) ";

%feature("docstring")  cmf::upslope::SoilLayer::set_volume "virtual
void set_volume(real newwatercontent)

Sets the volume of water in this storage in m3 ";

%feature("docstring")  cmf::upslope::SoilLayer::set_wetness "virtual
void set_wetness(real wetness) ";

%feature("docstring")  cmf::upslope::SoilLayer::Solute "SoluteStorage& Solute(const cmf::water::solute _Solute)

Returns the water quality of the water storage. ";

%feature("docstring")  cmf::upslope::SoilLayer::Solute "const
SoluteStorage& Solute(const cmf::water::solute _Solute) const ";

%feature("docstring")  cmf::upslope::SoilLayer::waterbalance "real
waterbalance(cmf::math::Time t, const flux_connection *Without=0)
const

Returns the sum of all fluxes (positive and negative) at time t.

Single fluxes can be excluded from the calculation

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the waterbalance
(e.g. to prevent closed circuits) ";


// File: structcmf_1_1water_1_1solute.xml
%feature("docstring") cmf::water::solute "

A structure to identify a solute.

C++ includes: Solute.h ";

%feature("docstring")  cmf::water::solute::solute "solute(const
solute &copy) ";


// File: classcmf_1_1water_1_1solute__vector.xml
%feature("docstring") cmf::water::solute_vector "

Manages the solutes of the model.

C++ includes: Solute.h ";

%feature("docstring")  cmf::water::solute_vector::solute_vector "solute_vector(std::string str)

Creates a solute vector from solute names, separated by whitespace.
E.g. solutes= solute_vector(\"dO18 dH2\") ";

%feature("docstring")  cmf::water::solute_vector::begin "const_iterator begin() const ";

%feature("docstring")  cmf::water::solute_vector::end "const_iterator
end() const ";

%feature("docstring")  cmf::water::solute_vector::get_solute "cmf::water::solute* get_solute(int position) ";

%feature("docstring")  cmf::water::solute_vector::size "size_t size()
const ";


// File: classcmf_1_1water_1_1_solute_storage.xml
%feature("docstring") cmf::water::SoluteStorage "

A class for the storage of any tracer.

The state is the amount (mol, kg etc. see cmf::water) of the tracer in
the storage.

The derivative function is given by: 

.. math::

    
    \\\\frac{dX}{dt}&=&\\\\sum_{f=1}^{F}\\\\left( q_f [X]_f\\\\right) +
    X_{in} - r^-X \\\\left[\\\\frac{mol}{day}\\\\right]\\\\\\\\ F&=&
    \\\\mbox{Number of fluxes in water storage} \\\\\\\\ q_f&=&
    \\\\mbox{Water flux in } \\\\frac{m^3}{day} \\\\\\\\
    \\\\left[X\\\\right]_f &=& \\\\mbox{Concentration of solute X in flux
    }q_f \\\\mbox{ in } \\\\frac{mol}{m^3} \\\\\\\\ X_{in} &=&
    \\\\mbox{Absolute source or sink term} \\\\frac{mol}{day} \\\\\\\\ r^-
    &=& \\\\mbox{Decay rate} \\\\frac 1{day} \\\\\\\\ V &=&
    \\\\mbox{Volume of water in water storage }\\\\left[m^3\\\\right]
    



C++ includes: SoluteStorage.h ";

%feature("docstring")  cmf::water::SoluteStorage::add_connected_states
"virtual void add_connected_states(cmf::math::StateVariable::list
&states) ";

%feature("docstring")  cmf::water::SoluteStorage::dxdt "virtual real
dxdt(const cmf::math::Time &time)

Returns the derivate of the state variable at time time. ";

%feature("docstring")  cmf::water::SoluteStorage::get_abs_errtol "virtual real get_abs_errtol(real rel_errtol) const ";

%feature("docstring")  cmf::water::SoluteStorage::get_conc "real
get_conc() const

Returns the concentration of the solute. ";

%feature("docstring")  cmf::water::SoluteStorage::get_state "real
get_state() const

Returns the current state of the variable. ";

%feature("docstring")  cmf::water::SoluteStorage::get_water "const
WaterStorage& get_water() const

get the waterstorge of the solute storage ";

%feature("docstring")  cmf::water::SoluteStorage::is_connected "virtual bool is_connected(const cmf::math::StateVariable &other) const

Returns True if this solute storage is effected by another state. ";

%feature("docstring")  cmf::water::SoluteStorage::set_adsorption "void set_adsorption(const Adsorption &newadsorption, real m=-1) ";

%feature("docstring")  cmf::water::SoluteStorage::set_conc "void
set_conc(real NewConcentration)

set a new concentration of dissolved tracers.

In case of adsorption functions, the isotherm is used ";

%feature("docstring")  cmf::water::SoluteStorage::set_state "void
set_state(real newState)

Gives access to the state variable. ";

%feature("docstring")  cmf::water::SoluteStorage::to_string "virtual
std::string to_string() const ";


// File: classcmf_1_1water_1_1_solute_timeseries.xml
%feature("docstring") cmf::water::SoluteTimeseries "

A map of concentration time series for solutes.

C++ includes: Solute.h ";

%feature("docstring")  cmf::water::SoluteTimeseries::SoluteTimeseries
"SoluteTimeseries() ";

%feature("docstring")  cmf::water::SoluteTimeseries::SoluteTimeseries
"SoluteTimeseries(const solute_vector &solutes, cmf::math::Time
begin, cmf::math::Time step) ";

%feature("docstring")  cmf::water::SoluteTimeseries::SoluteTimeseries
"SoluteTimeseries(const cmf::water::SoluteTimeseries &sts) ";

%feature("docstring")  cmf::water::SoluteTimeseries::begin "iterator
begin() ";

%feature("docstring")  cmf::water::SoluteTimeseries::begin "const_iterator begin() const ";

%feature("docstring")  cmf::water::SoluteTimeseries::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute) const ";

%feature("docstring")  cmf::water::SoluteTimeseries::end "iterator
end() ";

%feature("docstring")  cmf::water::SoluteTimeseries::end "const_iterator end() const ";

%feature("docstring")  cmf::water::SoluteTimeseries::size "size_t
size() const

Returns the number of solutes in the solution. ";


// File: classcmf_1_1math_1_1_solute_water_integrator.xml
%feature("docstring") cmf::math::SoluteWaterIntegrator "

A SoluteWaterIntegrator implements the cmf::math::Integrator
interface, but consists of two independent ODE-solvers.

Added statevariables are sorted by waterstorages and solute storages
and assigned to the correct solver.

C++ includes: WaterSoluteIntegrator.h ";

/*  Accuracy parameters  */

/*  model time  */

%feature("docstring")  cmf::math::SoluteWaterIntegrator::get_t "cmf::math::Time get_t() const

Returns the current model time. ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::set_t "void
set_t(cmf::math::Time val)

Sets the current model time. ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::get_dt "cmf::math::Time get_dt() const

Returns the last time step. ";

/*  Integrate  */

%feature("docstring")
cmf::math::SoluteWaterIntegrator::integrate_until "void
integrate_until(cmf::math::Time t_max, cmf::math::Time dt=Time(), bool
reset=false)

Integrates the vector of state variables until t_max.

Parameters:
-----------

t_max:   Time, the solver should run to

dt:   Time step (may be omitted)

reset:  If true, solver is reseted before integration starts ";

%feature("docstring")
cmf::math::SoluteWaterIntegrator::SoluteWaterIntegrator "SoluteWaterIntegrator(const cmf::water::solute_vector &solutes, const
cmf::math::Integrator &water_integrator, const cmf::math::Integrator
&solute_integrator)

Creates a new SoluteWaterIntegrator.

Parameters:
-----------

solutes:  Solutes of the project

water_integrator:  Template for the integrator of WaterStorage state
variables

solute_integrator:  Template for the integrator of soluteStorage state
variables ";

%feature("docstring")
cmf::math::SoluteWaterIntegrator::SoluteWaterIntegrator "SoluteWaterIntegrator(cmf::water::solute_vector solutes, const
cmf::math::Integrator &water_integrator, const cmf::math::Integrator
&solute_integrator, const cmf::math::state_list &states)

Creates a new SoluteWaterIntegrator.

Parameters:
-----------

solutes:  Solutes of the project

water_integrator:  Template for the integrator of WaterStorage state
variables

solute_integrator:  Template for the integrator of soluteStorage state
variables

states:  States to be added to the integrators ";

%feature("docstring")
cmf::math::SoluteWaterIntegrator::~SoluteWaterIntegrator "~SoluteWaterIntegrator() ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::copy "virtual cmf::math::SoluteWaterIntegrator* copy() const

Polymorphic copy constructor. ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::integrate "int integrate(cmf::math::Time t_max, cmf::math::Time dt)

Add state variables from a cmf::math::state_list. ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::reset "virtual void reset()

Resets the integrators (only needed for multistep methods) ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::set_system "void set_system(const state_list &states) ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::size "size_t
size() const ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::to_string "std::string to_string() const override

Desrcibes the iterator. ";


// File: structcmf_1_1math_1_1sparse__structure.xml
%feature("docstring") cmf::math::sparse_structure "";

%feature("docstring")  cmf::math::sparse_structure::sparse_structure "sparse_structure()

Creates the sparse row compressed structure from states variable list.
";

%feature("docstring")  cmf::math::sparse_structure::generate "size_t
generate(const cmf::math::state_list &states) ";


// File: classcmf_1_1math_1_1state__list.xml
%feature("docstring") cmf::math::state_list "";

%feature("docstring")  cmf::math::state_list::state_list "state_list() ";

%feature("docstring")  cmf::math::state_list::state_list "state_list(const state_list &other) ";

%feature("docstring")  cmf::math::state_list::append "void
append(StateVariable::ptr sv) ";

%feature("docstring")  cmf::math::state_list::begin "iterator begin()
";

%feature("docstring")  cmf::math::state_list::begin "const_iterator
begin() const ";

%feature("docstring")  cmf::math::state_list::end "iterator end() ";

%feature("docstring")  cmf::math::state_list::end "const_iterator
end() const ";

%feature("docstring")  cmf::math::state_list::extend "state_list&
extend(const state_list &svl) ";

%feature("docstring")  cmf::math::state_list::size "size_t size()
const ";


// File: classcmf_1_1math_1_1_state_variable.xml
%feature("docstring") cmf::math::StateVariable "

Abstract class state variable.

Simple exponential system class header implementing a state variable:

C++ includes: statevariable.h ";

%feature("docstring")  cmf::math::StateVariable::StateVariable "StateVariable(real InitialState=0, real scale=1)

ctor ";

%feature("docstring")  cmf::math::StateVariable::~StateVariable "virtual ~StateVariable() ";

%feature("docstring")  cmf::math::StateVariable::add_connected_states
"virtual void add_connected_states(list &states) ";

%feature("docstring")  cmf::math::StateVariable::dxdt "virtual real
dxdt(const cmf::math::Time &time)=0

Returns the derivate of the state variable at time time. ";

%feature("docstring")  cmf::math::StateVariable::get_abs_errtol "virtual real get_abs_errtol(real rel_errtol) const ";

%feature("docstring")  cmf::math::StateVariable::get_state "real
get_state() const

Returns the current state of the variable. ";

%feature("docstring")  cmf::math::StateVariable::is_connected "virtual bool is_connected(const cmf::math::StateVariable &other) const
";

%feature("docstring")  cmf::math::StateVariable::set_state "void
set_state(real newState)

Gives access to the state variable. ";

%feature("docstring")  cmf::math::StateVariable::to_string "virtual
std::string to_string() const =0 ";


// File: classcmf_1_1timetools_1_1_stop_watch.xml
%feature("docstring") cmf::timetools::StopWatch "

A stopwatch to estimated the total time of a process    Creating a
StopWatch: >>>stopwatch=StopWatch(start, stop) Start and end are
indicators to describe the progress of a process.  Start is the
indicator value at the beginning of the process. As default they are
0.0 and 1.0.  Starting the StopWatch again: >>>stopwatch.restart()
Getting the elapsed time, the total time and the remaining time of the
process in seconds: >>>elapsed, total, remaining = stopwatch(progress)
Where progress is a process progress indicator matching start and stop
Example: stopwatch=StopWatch(0,10) for i in range(10): time.sleep(1)
print('elapsed = %0.2fs, total= %0.2fs, remaining = %0.2fs' %
stopwatch(i+1)) ";

%feature("docstring")  cmf::timetools::StopWatch::__init__ "def
__init__(self, start=0.0, stop=1.0) ";

%feature("docstring")  cmf::timetools::StopWatch::__call__ "def
__call__(self, progress)

Returns the progress in wall clock time :param progress: The current
position in the stopwatch :return: A 3-tuple of elapsed, total and
remaining seconds of the task ";

%feature("docstring")  cmf::timetools::StopWatch::restart "def
restart(self)

Restarts the stopwatch ";


// File: classcmf_1_1upslope_1_1_e_t_1_1stressed_e_t.xml
%feature("docstring") cmf::upslope::ET::stressedET "

An abstract base class for ET Methods with a WaterStressFunction.

C++ includes: ET.h ";

%feature("docstring")  cmf::upslope::ET::stressedET::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")  cmf::upslope::ET::stressedET::ETpot "virtual
real ETpot(cmf::math::Time t) const ";

%feature("docstring")  cmf::upslope::ET::stressedET::exchange_target "void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")  cmf::upslope::ET::stressedET::get_layer "SoilLayer::ptr get_layer() const ";

%feature("docstring")  cmf::upslope::ET::stressedET::get_ptr "ptr
get_ptr() const ";

%feature("docstring")  cmf::upslope::ET::stressedET::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::ET::stressedET::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")  cmf::upslope::ET::stressedET::get_tracer_filter
"real get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")  cmf::upslope::ET::stressedET::get_tracer_filter
"real get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::upslope::ET::stressedET::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::upslope::ET::stressedET::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::ET::stressedET::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::ET::stressedET::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")  cmf::upslope::ET::stressedET::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::upslope::ET::stressedET::set_tracer_filter
"void set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")  cmf::upslope::ET::stressedET::set_tracer_filter
"void set_tracer_filter(solute S, real value) ";

%feature("docstring")  cmf::upslope::ET::stressedET::short_string "virtual std::string short_string() const ";

%feature("docstring")  cmf::upslope::ET::stressedET::to_string "virtual std::string to_string() const ";


// File: classcmf_1_1upslope_1_1subcatchment.xml
%feature("docstring") cmf::upslope::subcatchment "

A class to structure cells in a project using their main outlets.

C++ includes: algorithm.h ";

%feature("docstring")  cmf::upslope::subcatchment::subcatchment "subcatchment(cmf::upslope::Cell &pourpoint, double
area_threshold=1e308)

Creates a subcatchment from a pourpoint cell.

Parameters:
-----------

pourpoint:  The pourpoint (outlet of the subcatchment)

area_threshold:  Minimum contributing area size to form a
subcatchment. When area_threshold > area(cells), all upslope cells of
pourpoint are used. ";


// File: classcmf_1_1upslope_1_1_e_t_1_1_suction_stress.xml
%feature("docstring") cmf::upslope::ET::SuctionStress "

The classical suction depending transpiration Stress curve after
Feddes.

The ability of roots to take water from the soil up is limited by the
suction pressure (matrix potential) of the soil. The stress is defined
using a trapezoid function, with P0 being the pressure, where water
uptake is prohibited by saturation. Between P0 and P1 the saturation
stress gets lower. Between P1 and P2, no stress occurs ( ET=ETpot) and
with a pressure below P2, water uptake is limited by drought. P3
indicates the highest suction (lowest pressure) to which plants can
extract water (wilting point).

Default values: :math:`P_0 = \\\\infty, P_1=\\\\infty, P_2=-5m, P_3=-160m`,
resulting in no saturation stress and draught stress starting at -5m
and a wilting point of -160m.

If one would like to include saturation stress, typical values for P0
and P1 are 0.0m and 0.1m. By changing P2 and P3, you can account for
different drought sensibility for different plant types.

C++ includes: waterstress.h ";

%feature("docstring")  cmf::upslope::ET::SuctionStress::SuctionStress
"SuctionStress(real P0=1e308, real P1=1e307, real P2=-5, real
P3=-160)

Creates a new suction stress function. ";

%feature("docstring")  cmf::upslope::ET::SuctionStress::SuctionStress
"SuctionStress(const SuctionStress &other) ";

%feature("docstring")  cmf::upslope::ET::SuctionStress::~SuctionStress
"virtual ~SuctionStress() ";

%feature("docstring")  cmf::upslope::ET::SuctionStress::copy "cmf::upslope::ET::SuctionStress* copy() const

Creates a new copy of this wetness. ";

%feature("docstring")  cmf::upslope::ET::SuctionStress::Tact "virtual
real Tact(const cmf::upslope::SoilLayer *soillayer, real Tpot) const

Calculates the water stress for a layer.

Parameters:
-----------

soillayer:  The cmf::upslope::SoilLayer this stress function belongs
to

Tpot:  Potential Transpiration in mm/day (for the full profile)

Actual flux from layer in m3/day ";

%feature("docstring")  cmf::upslope::ET::SuctionStress::to_string "std::string to_string() const ";


// File: classcmf_1_1upslope_1_1_surface_water.xml
%feature("docstring") cmf::upslope::SurfaceWater "

A child class of OpenWaterStorage to model surface water on a cell.

Specially created to connect a SurfaceWater with other nodes, is the
flux connection cmf::upslope::connections::KinematicSurfaceRunoff

C++ includes: surfacewater.h ";

/*  Overrides of flux_node  */

%feature("docstring")  cmf::upslope::SurfaceWater::RecalcFluxes "virtual bool RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node.

WaterStorage overrides this, since state changes require an update of
the fluxes ";

%feature("docstring")  cmf::upslope::SurfaceWater::is_empty "virtual
double is_empty() const

Returns true if the node has no water. ";

%feature("docstring")  cmf::upslope::SurfaceWater::to_string "virtual
std::string to_string() const ";

%feature("docstring")
cmf::upslope::SurfaceWater::add_connected_states "virtual void
add_connected_states(cmf::math::StateVariable::list &states) ";

%feature("docstring")  cmf::upslope::SurfaceWater::conc "real
conc(cmf::math::Time t, const cmf::water::solute &solute) const

Returns the water quality of the flux_node, if it is not overridden
this is the mix of the incoming fluxes. ";

%feature("docstring")  cmf::upslope::SurfaceWater::conc "real
conc(const cmf::water::solute &_Solute) const

Returns the concentration of the given solute. ";

%feature("docstring")  cmf::upslope::SurfaceWater::conc "void
conc(const cmf::water::solute &_Solute, real NewConcetration)

Sets a new concentration. ";

%feature("docstring")  cmf::upslope::SurfaceWater::connection_to "cmf::water::flux_connection* connection_to(const cmf::water::flux_node
&target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::upslope::SurfaceWater::dxdt "virtual real
dxdt(const cmf::math::Time &time)

Returns the derivate of the state variable at time time. ";

%feature("docstring")  cmf::upslope::SurfaceWater::flux3d_to "cmf::geometry::point flux3d_to(const cmf::water::flux_node &target,
cmf::math::Time t) ";

%feature("docstring")  cmf::upslope::SurfaceWater::flux_to "real
flux_to(const cmf::water::flux_node &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from this into target\") ";

%feature("docstring")  cmf::upslope::SurfaceWater::get_3d_flux "cmf::geometry::point get_3d_flux(cmf::math::Time t)

Returns the sum of all flux vectors. ";

%feature("docstring")  cmf::upslope::SurfaceWater::get_abs_errtol "real get_abs_errtol(real rel_errtol) const ";

%feature("docstring")  cmf::upslope::SurfaceWater::get_cell "Cell&
get_cell() const

Get the cell of the surface. ";

%feature("docstring")  cmf::upslope::SurfaceWater::get_connections "cmf::water::connection_list get_connections() const ";

%feature("docstring")  cmf::upslope::SurfaceWater::get_coverage "double get_coverage() const

Get surface coverage as a function of the actual volume. ";

%feature("docstring")  cmf::upslope::SurfaceWater::get_depth "real
get_depth() const

Returns the water table depth. ";

%feature("docstring")  cmf::upslope::SurfaceWater::get_height_function
"virtual const cmf::river::Prism& get_height_function() const

Gets the height function (a cmf::river::Prism) for further reference.
";

%feature("docstring")  cmf::upslope::SurfaceWater::get_nManning "real
get_nManning() const

get Manning roughness (n) of the surface

From Python use this as a property: ";

%feature("docstring")  cmf::upslope::SurfaceWater::get_potential "real get_potential(cmf::math::Time t=cmf::math::never) const override

Returns the water potential of the node in m waterhead.

The base class water storage always returns the height of the location
";

%feature("docstring")  cmf::upslope::SurfaceWater::get_project "cmf::project& get_project() const

Returns the project, this node is part of. ";

%feature("docstring")  cmf::upslope::SurfaceWater::get_puddledepth "real get_puddledepth() const

Get water depth at which runoff starts.

From Python use this as a property: ";

%feature("docstring")  cmf::upslope::SurfaceWater::get_state "real
get_state() const

Returns the current state of the variable. ";

%feature("docstring")
cmf::upslope::SurfaceWater::get_state_variable_content "char
get_state_variable_content() const

A character indicating the integrated variable (either 'V' for Volume
or 'h' for head) ";

%feature("docstring")  cmf::upslope::SurfaceWater::get_volume "virtual real get_volume() const

Returns the volume of water in this storage in m3 ";

%feature("docstring")  cmf::upslope::SurfaceWater::is_connected "virtual bool is_connected(const cmf::math::StateVariable &other) const

Returns True if this waterstorage is effected by another state. ";

%feature("docstring")  cmf::upslope::SurfaceWater::is_storage "virtual bool is_storage() const

Returns true, since this is a storage. ";

%feature("docstring")  cmf::upslope::SurfaceWater::remove_connection "bool remove_connection(cmf::water::flux_node::ptr To)

Remove the connection. ";

%feature("docstring")  cmf::upslope::SurfaceWater::set_depth "void
set_depth(real new_depth) ";

%feature("docstring")  cmf::upslope::SurfaceWater::set_height_function
"virtual void set_height_function(const
cmf::river::IVolumeHeightFunction &val)

Prevents the change of the height function. A SurfaceWater storage has
always a Prism height function. ";

%feature("docstring")  cmf::upslope::SurfaceWater::set_nManning "void
set_nManning(real n)

set Manning roughness (n) of the surface

From Python use this as a property: ";

%feature("docstring")  cmf::upslope::SurfaceWater::set_potential "void set_potential(real newpotential) override

Sets the potential of this flux node. ";

%feature("docstring")  cmf::upslope::SurfaceWater::set_puddledepth "void set_puddledepth(real puddledepth)

Set water depth at which runoff starts.

From Python use this as a property: ";

%feature("docstring")  cmf::upslope::SurfaceWater::set_state "void
set_state(real newState)

Gives access to the state variable. ";

%feature("docstring")
cmf::upslope::SurfaceWater::set_state_variable_content "void
set_state_variable_content(char content)

A character indicating the integrated variable (either 'V' for Volume
or 'h' for head) ";

%feature("docstring")  cmf::upslope::SurfaceWater::set_volume "virtual void set_volume(real newwatercontent)

Sets the volume of water in this storage in m3 ";

%feature("docstring")  cmf::upslope::SurfaceWater::Solute "SoluteStorage& Solute(const cmf::water::solute _Solute)

Returns the water quality of the water storage. ";

%feature("docstring")  cmf::upslope::SurfaceWater::Solute "const
SoluteStorage& Solute(const cmf::water::solute _Solute) const ";

%feature("docstring")  cmf::upslope::SurfaceWater::waterbalance "real
waterbalance(cmf::math::Time t, const flux_connection *Without=0)
const

Returns the sum of all fluxes (positive and negative) at time t.

Single fluxes can be excluded from the calculation

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the waterbalance
(e.g. to prevent closed circuits) ";

%feature("docstring")  cmf::upslope::SurfaceWater::wet_area "real
wet_area() const

Returns the exposed surface area in m2. ";


// File: classcmf_1_1upslope_1_1_e_t_1_1_s_w__evap__from__canopy.xml
%feature("docstring") cmf::upslope::ET::SW_evap_from_canopy "

Connection for Shuttleworth-Wallace canopy interception evaporation.

C++ includes: ShuttleworthWallace.h ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_canopy::SW_evap_from_canopy "SW_evap_from_canopy(cmf::water::WaterStorage::ptr source,
cmf::water::flux_node::ptr ET_target, ShuttleworthWallace::ptr owner)
";

%feature("docstring")  cmf::upslope::ET::SW_evap_from_canopy::conc "real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_canopy::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::ET::SW_evap_from_canopy::get_ptr
"ptr get_ptr() const ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_canopy::get_target "flux_node::ptr
get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_canopy::get_target "flux_node::ptr
get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_canopy::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_canopy::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::upslope::ET::SW_evap_from_canopy::kill_me
"bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_canopy::left_node "flux_node::ptr
left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::ET::SW_evap_from_canopy::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::ET::SW_evap_from_canopy::refresh
"void refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_canopy::right_node "flux_node::ptr
right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_canopy::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_canopy::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_canopy::short_string "virtual
std::string short_string() const ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_canopy::to_string "virtual std::string
to_string() const ";


// File: classcmf_1_1upslope_1_1_e_t_1_1_s_w__evap__from__layer.xml
%feature("docstring") cmf::upslope::ET::SW_evap_from_layer "

Connection for Shuttleworth-Wallace ground evaporation.

C++ includes: ShuttleworthWallace.h ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_layer::SW_evap_from_layer "SW_evap_from_layer(cmf::upslope::SoilLayer::ptr source,
cmf::water::flux_node::ptr ET_target, ShuttleworthWallace::ptr owner)
";

%feature("docstring")  cmf::upslope::ET::SW_evap_from_layer::conc "real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_layer::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::ET::SW_evap_from_layer::get_ptr "ptr get_ptr() const ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_layer::get_target "flux_node::ptr
get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_layer::get_target "flux_node::ptr
get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_layer::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_layer::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::upslope::ET::SW_evap_from_layer::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::upslope::ET::SW_evap_from_layer::left_node
"flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::ET::SW_evap_from_layer::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::ET::SW_evap_from_layer::refresh "void refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_layer::right_node "flux_node::ptr
right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_layer::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_layer::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_layer::short_string "virtual
std::string short_string() const ";

%feature("docstring")  cmf::upslope::ET::SW_evap_from_layer::to_string
"virtual std::string to_string() const ";


// File: classcmf_1_1upslope_1_1_e_t_1_1_s_w__evap__from__snow.xml
%feature("docstring") cmf::upslope::ET::SW_evap_from_snow "

Connection for Shuttleworth-Wallace canopy interception evaporation.

C++ includes: ShuttleworthWallace.h ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_snow::SW_evap_from_snow "SW_evap_from_snow(cmf::water::WaterStorage::ptr source,
cmf::water::flux_node::ptr ET_target, ShuttleworthWallace::ptr owner)
";

%feature("docstring")  cmf::upslope::ET::SW_evap_from_snow::conc "real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_snow::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::ET::SW_evap_from_snow::get_ptr "ptr get_ptr() const ";

%feature("docstring")  cmf::upslope::ET::SW_evap_from_snow::get_target
"flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::ET::SW_evap_from_snow::get_target
"flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_snow::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_snow::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::upslope::ET::SW_evap_from_snow::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::upslope::ET::SW_evap_from_snow::left_node
"flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::ET::SW_evap_from_snow::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::ET::SW_evap_from_snow::refresh "void refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")  cmf::upslope::ET::SW_evap_from_snow::right_node
"flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_snow::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_snow::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_snow::short_string "virtual
std::string short_string() const ";

%feature("docstring")  cmf::upslope::ET::SW_evap_from_snow::to_string
"virtual std::string to_string() const ";


// File: classcmf_1_1upslope_1_1_e_t_1_1_s_w__evap__from__surfacewater.xml
%feature("docstring") cmf::upslope::ET::SW_evap_from_surfacewater "

Connection for Shuttleworth-Wallace canopy interception evaporation.

C++ includes: ShuttleworthWallace.h ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_surfacewater::SW_evap_from_surfacewater
"SW_evap_from_surfacewater(cmf::river::OpenWaterStorage::ptr source,
cmf::water::flux_node::ptr ET_target, ShuttleworthWallace::ptr owner)
";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_surfacewater::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_surfacewater::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_surfacewater::get_ptr "ptr get_ptr()
const ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_surfacewater::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_surfacewater::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_surfacewater::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_surfacewater::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_surfacewater::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_surfacewater::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::ET::SW_evap_from_surfacewater::q
"real q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_surfacewater::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_surfacewater::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_surfacewater::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_surfacewater::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_surfacewater::short_string "virtual
std::string short_string() const ";

%feature("docstring")
cmf::upslope::ET::SW_evap_from_surfacewater::to_string "virtual
std::string to_string() const ";


// File: classcmf_1_1upslope_1_1_e_t_1_1_s_w__transpiration.xml
%feature("docstring") cmf::upslope::ET::SW_transpiration "

Connection for Shuttleworth-Wallace transpiration.

C++ includes: ShuttleworthWallace.h ";

%feature("docstring")
cmf::upslope::ET::SW_transpiration::SW_transpiration "SW_transpiration(cmf::upslope::SoilLayer::ptr source,
cmf::water::flux_node::ptr ET_target, ShuttleworthWallace::ptr owner)
";

%feature("docstring")  cmf::upslope::ET::SW_transpiration::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::ET::SW_transpiration::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::ET::SW_transpiration::get_ptr "ptr get_ptr() const ";

%feature("docstring")  cmf::upslope::ET::SW_transpiration::get_target
"flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::ET::SW_transpiration::get_target
"flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::ET::SW_transpiration::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::ET::SW_transpiration::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::upslope::ET::SW_transpiration::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::upslope::ET::SW_transpiration::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::ET::SW_transpiration::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::ET::SW_transpiration::refresh "void refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")  cmf::upslope::ET::SW_transpiration::right_node
"flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::ET::SW_transpiration::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::ET::SW_transpiration::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::ET::SW_transpiration::short_string "virtual std::string
short_string() const ";

%feature("docstring")  cmf::upslope::ET::SW_transpiration::to_string "virtual std::string to_string() const ";


// File: classcmf_1_1upslope_1_1connections_1_1_s_w_a_t_percolation.xml
%feature("docstring") cmf::upslope::connections::SWATPercolation "

A tipping bucket percolation approach similar to the approach in SWAT.



.. math::

     q_{perc} &=&
    V_{H_2O,drain}\\\\left(1-e^{-\\\\frac 1{-TT_{perc}}}\\\\right)
    \\\\\\\\ TT_{perc} &=& \\\\frac{V_{pores,drain} - V_{field
    cap.}}{K_{sat}} \\\\\\\\ V_{x,drain} &=& V_{x,drain} - V_{field cap.}
    



C++ includes: Percolation.h ";

%feature("docstring")
cmf::upslope::connections::SWATPercolation::SWATPercolation "SWATPercolation(cmf::upslope::SoilLayer::ptr upperLayer,
cmf::upslope::SoilLayer::ptr lowerLayer) ";

%feature("docstring")
cmf::upslope::connections::SWATPercolation::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::SWATPercolation::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")
cmf::upslope::connections::SWATPercolation::get_ptr "ptr get_ptr()
const ";

%feature("docstring")
cmf::upslope::connections::SWATPercolation::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::SWATPercolation::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::SWATPercolation::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::SWATPercolation::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")
cmf::upslope::connections::SWATPercolation::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::SWATPercolation::left_node "flux_node::ptr
left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::connections::SWATPercolation::q "real q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::SWATPercolation::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::upslope::connections::SWATPercolation::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::SWATPercolation::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::SWATPercolation::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::connections::SWATPercolation::short_string "virtual
std::string short_string() const ";

%feature("docstring")
cmf::upslope::connections::SWATPercolation::to_string "virtual
std::string to_string() const ";


// File: classcmf_1_1river_1_1_s_w_a_t_reach_type.xml
%feature("docstring") cmf::river::SWATReachType "

Structure for the description of structural parameters of a reach.

Uses the SWAT IChannel geometry (see SWAT Theoretical Documentation,
Version 2005 (ch. 7:1.1), Neitsch et al. 2005), in this class
referenced as SWATtheoDoc. Differences to the SWAT geometry: The flood
plain is not plain, but has a small slope=0.5%, but has an infinite
width

C++ includes: ReachType.h ";

%feature("docstring")  cmf::river::SWATReachType::SWATReachType "SWATReachType(const SWATReachType &copy) ";

%feature("docstring")  cmf::river::SWATReachType::SWATReachType "SWATReachType(double l)

Creates a new reach structure with standard values (small natural
river) BottomWidth = 3m, ChannelDepth = 0.5m, BankSlope = 2, nManning
= 0.0035, FloodPlainSlope = 200. ";

%feature("docstring")  cmf::river::SWATReachType::SWATReachType "SWATReachType(double l, double BankWidth, double Depth)

Creates a new reach structure from a give width and depth.

Parameters:
-----------

l:  length of the channel [m]

BankWidth:  get_channel_width of the reach from bank to bank [m]

Depth:  Depth of the reach [m] ";

%feature("docstring")  cmf::river::SWATReachType::~SWATReachType "virtual ~SWATReachType() ";

%feature("docstring")  cmf::river::SWATReachType::A "virtual double
A(double V) const

Returns the area of the surface for a given volume. ";

%feature("docstring")  cmf::river::SWATReachType::copy "SWATReachType* copy() const ";

%feature("docstring")  cmf::river::SWATReachType::get_channel_width "virtual double get_channel_width(double depth) const

Calculates the flow width from a given actual depth [m] using the
actual IChannel geometry 

.. math::

     \\\\mbox{If } d\\\\le
    d_{IChannel} && \\\\\\\\ w &=& w_{bottom} + 2 \\\\Delta_{bank} d
    \\\\\\\\ \\\\mbox{else, } && \\\\mbox{if the river floods the flood
    plain} \\\\\\\\ w &=& w_{bank} + 2 \\\\Delta_{Floodplain}
    (d-d_{IChannel} \\\\\\\\ 

.

See:  SWATtheoDoc eq. 7:1.1.3 ";

%feature("docstring")  cmf::river::SWATReachType::get_depth "virtual
double get_depth(double area) const

Calculates the actual depth of the reach using the IChannel geometry


.. math::

     d &=& \\\\sqrt{\\\\frac{A}{\\\\Delta_{bank}} +
    \\\\frac{{w_{bottom}}^2}{4 {\\\\Delta_{bank}}^2}} -
    \\\\frac{w_{bottom}}{2 \\\\Delta_{bank}} \\\\\\\\ \\\\mbox{If }
    d>d_{IChannel} &&\\\\\\\\
    d&=&d_{IChannel}+\\\\sqrt{\\\\frac{A-A(d_{IChannel})}{\\\\Delta_{flood\\\\
    plain}} + \\\\frac{{w(d_{IChannel})}^2}{4 {\\\\Delta_{flood\\\\
    plain}}^2}} - \\\\frac{w(d_{IChannel})}{2 \\\\Delta_{flood\\\\ plain}}
    \\\\\\\\ 

.

See:  SWATtheoDoc eq. 7:1.2.4

get_depth of the reach [m]

Parameters:
-----------

area:  Wetted area of a river cross section [m2], can be obtained by
V/l, where V is the stored volume and l is the reach length ";

%feature("docstring")  cmf::river::SWATReachType::get_flux_crossection
"virtual double get_flux_crossection(double depth) const

Calculates the wetted area from a given depth using the IChannel
geometry.

In most cases use get_flux_crossection=V/l, where V is the stored
volume and l is the reach length 

.. math::

     \\\\mbox{If }
    d>d_{IChannel} &&\\\\\\\\ A &=& \\\\left(w_{bottom} + \\\\Delta_{bank}
    d\\\\right) d \\\\\\\\ \\\\mbox{else, } && \\\\mbox{if the river
    floods the flood plain} \\\\\\\\ A &=& A(d_{IChannel}) +
    \\\\left(w(d_{IChannel} + \\\\Delta_{flood\\\\ plain}
    \\\\left(d-d_{IChannel}\\\\right)\\\\right) (d-d_{IChannel}) \\\\\\\\
    

 See:  SWATtheoDoc eq. 7:1.1.4

Wetted area of a river cross section [m2]

Parameters:
-----------

depth:  Depth of the reach [m] ";

%feature("docstring")  cmf::river::SWATReachType::get_length "double
get_length() const

Length of the reach. ";

%feature("docstring")  cmf::river::SWATReachType::get_nManning "virtual double get_nManning() const ";

%feature("docstring")  cmf::river::SWATReachType::get_wetted_perimeter
"virtual double get_wetted_perimeter(double depth) const

Calculates the wetted perimeter from a given actual depth [m] using
the actual IChannel geometry 

.. math::

     \\\\mbox{If }
    d\\\\le d_{IChannel} && \\\\\\\\ P &=& w_{bottom} + 2 \\\\sqrt{1+
    {\\\\Delta_{bank}}^2} d \\\\\\\\ \\\\mbox{else, } && \\\\mbox{if the
    river floods the flood plain} \\\\\\\\ P &=& P(d_{IChannel} + 2
    \\\\sqrt{1+ {\\\\Delta_{flood\\\\ plain}}^2} (d-d_{IChannel}) \\\\\\\\
    

.

See:  SWATtheoDoc eq. 7:1.1.5

Parameters:
-----------

depth:  Actual depth of the reach [m] ";

%feature("docstring")  cmf::river::SWATReachType::h "virtual double
h(double V) const

Returns the depth of a given volume. ";

%feature("docstring")  cmf::river::SWATReachType::q "virtual double
q(double h, double slope) const ";

%feature("docstring")  cmf::river::SWATReachType::qManning "virtual
double qManning(double A, double slope) const

Calculates the flow rate from a given water volume in the reach


.. math::

     q_{Manning}&=& A R^{\\\\frac 2 3}
    \\\\sqrt{\\\\frac {\\\\Delta_z} n} \\\\\\\\ A &=& \\\\frac V l
    \\\\mbox{, (Crosssectional area of the wetted crossection, Volume per
    length)} \\\\\\\\ R &=& \\\\frac A {P(d)} \\\\\\\\ P(d) &=& \\\\mbox{
    the perimeter of the wetted crosssection, a function of reach depth}
    \\\\\\\\ d(V) &=& \\\\mbox{ the depth of the reach a function of the
    volume} \\\\\\\\ \\\\Delta_z &=& \\\\frac{z_{max} - z_{min}}{l}
    \\\\mbox{ Slope of the reach} 

.

Flow rate [m3/s]

Parameters:
-----------

A:  The area of the cross section [m2]

slope:  The slope of the reach [m/m] ";

%feature("docstring")  cmf::river::SWATReachType::set_nManning "virtual void set_nManning(double nManning) ";

%feature("docstring")  cmf::river::SWATReachType::typecode "char
typecode() const ";

%feature("docstring")  cmf::river::SWATReachType::V "virtual double
V(double h) const ";


// File: classcmf_1_1water_1_1_system_bridge.xml
%feature("docstring") cmf::water::SystemBridge "

A SystemBridge is an advanced feature for tuning of the calculation
time.

A SystemBridge can be used to replace an existing connection between
nodes. It is created using the system_bridge function. After
installation, the two nodes can more safely be added to different
integrator systems. One node (called upper) is connected with the
system bridge with the connection formerly connecting the nodes, the
second node (called lower) is connected to the system bridge with as a
Neumann boundary condition. The flux equals the average flux of the
connection upper <-> SystemBridge. Therefore, the downward flux needs
to be integrated over time by the solver the upper node belongs to.
Use as an upper system (system upper node is belonging to) the faster
reacting system. For the connection between upper and SystemBridge,
the SystemBridge reacts as an Dirichlet boundary condition, providing
the potential of the lower node.

The following example code creates a system bridge between the nodes
upper and lower. To integrate the flux over each timestep
automatically, the systembridge is added to the solver of upper, as an
integratable

C++ includes: system_bridge.h ";

%feature("docstring")  cmf::water::SystemBridge::conc "virtual real
conc(cmf::math::Time t, const cmf::water::solute &Solute) const

Returns the water quality of the flux_node, if it is not overridden
this is the mix of the incoming fluxes. ";

%feature("docstring")  cmf::water::SystemBridge::connection_to "cmf::water::flux_connection* connection_to(const cmf::water::flux_node
&target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::water::SystemBridge::down_flux_integrator
"flux_integrator::ptr down_flux_integrator() const ";

%feature("docstring")  cmf::water::SystemBridge::flux3d_to "cmf::geometry::point flux3d_to(const cmf::water::flux_node &target,
cmf::math::Time t) ";

%feature("docstring")  cmf::water::SystemBridge::flux_to "real
flux_to(const cmf::water::flux_node &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from this into target\") ";

%feature("docstring")  cmf::water::SystemBridge::get_3d_flux "cmf::geometry::point get_3d_flux(cmf::math::Time t)

Returns the sum of all flux vectors. ";

%feature("docstring")  cmf::water::SystemBridge::get_connections "cmf::water::connection_list get_connections() const ";

%feature("docstring")  cmf::water::SystemBridge::get_down_flux "double get_down_flux() const

Returns the currently integrated flux to the lower node. ";

%feature("docstring")  cmf::water::SystemBridge::get_lower_node "flux_node::ptr get_lower_node() const

Returns the lower node. ";

%feature("docstring")  cmf::water::SystemBridge::get_potential "double get_potential(cmf::math::Time t) const

Returns the water potential of the node in m waterhead.

The base class water storage always returns the height of the location
";

%feature("docstring")  cmf::water::SystemBridge::get_project "cmf::project& get_project() const

Returns the project, this node is part of. ";

%feature("docstring")  cmf::water::SystemBridge::get_upper_node "flux_node::ptr get_upper_node() const

Returns the upper node. ";

%feature("docstring")  cmf::water::SystemBridge::is_empty "virtual
double is_empty() const

Returns true if the node has no water. ";

%feature("docstring")  cmf::water::SystemBridge::is_storage "virtual
bool is_storage() const

true, if this is a waterstorage ";

%feature("docstring")  cmf::water::SystemBridge::RecalcFluxes "virtual bool RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node.

WaterStorage overrides this, since state changes require an update of
the fluxes ";

%feature("docstring")  cmf::water::SystemBridge::remove_connection "bool remove_connection(cmf::water::flux_node::ptr To)

Remove the connection. ";

%feature("docstring")  cmf::water::SystemBridge::set_potential "virtual void set_potential(real new_potential)

Sets the potential of this flux node. ";

%feature("docstring")  cmf::water::SystemBridge::to_string "virtual
std::string to_string() const ";

%feature("docstring")  cmf::water::SystemBridge::waterbalance "real
waterbalance(cmf::math::Time t, const flux_connection *Without=0)
const

Returns the sum of all fluxes (positive and negative) at time t.

Single fluxes can be excluded from the calculation

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the waterbalance
(e.g. to prevent closed circuits) ";


// File: classcmf_1_1water_1_1_system_bridge_connection.xml
%feature("docstring") cmf::water::SystemBridgeConnection "

Connects a system bridge with its lower node. Is created automatically
when creating a SystemBridge.

C++ includes: system_bridge.h ";

%feature("docstring")  cmf::water::SystemBridgeConnection::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::water::SystemBridgeConnection::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::water::SystemBridgeConnection::get_ptr "ptr get_ptr() const ";

%feature("docstring")  cmf::water::SystemBridgeConnection::get_target
"flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::water::SystemBridgeConnection::get_target
"flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::water::SystemBridgeConnection::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::water::SystemBridgeConnection::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::water::SystemBridgeConnection::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::water::SystemBridgeConnection::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::water::SystemBridgeConnection::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::water::SystemBridgeConnection::refresh "void refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")  cmf::water::SystemBridgeConnection::right_node
"flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::water::SystemBridgeConnection::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::water::SystemBridgeConnection::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::water::SystemBridgeConnection::short_string "virtual std::string
short_string() const ";

%feature("docstring")  cmf::water::SystemBridgeConnection::to_string "virtual std::string to_string() const ";


// File: classcmf_1_1upslope_1_1connections_1_1_temp_index_snow_melt.xml
%feature("docstring") cmf::upslope::connections::TempIndexSnowMelt "

Calculates snow melt using a simple degree day method.



.. math::

     q_{melt} [mm/day] = (T-T_{thres}) * r 

Usage:

C++ includes: surfacefluxes.h ";

%feature("docstring")
cmf::upslope::connections::TempIndexSnowMelt::TempIndexSnowMelt "TempIndexSnowMelt(cmf::water::WaterStorage::ptr snow,
cmf::water::flux_node::ptr surface_water, cmf::upslope::Cell &cell,
real rate=7.0)

Creates a new snow melt connection.

Parameters:
-----------

snow:  Snow storage, usually cel.snow

surface_water:  target of the melted water (usually cell.surfacewater)

cell:  The cell, needed to get weather and area

rate:  The rate of snow melt, given in mm/(degC day), default = 7.0 ";

%feature("docstring")
cmf::upslope::connections::TempIndexSnowMelt::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::TempIndexSnowMelt::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")
cmf::upslope::connections::TempIndexSnowMelt::get_ptr "ptr get_ptr()
const ";

%feature("docstring")
cmf::upslope::connections::TempIndexSnowMelt::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::TempIndexSnowMelt::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::TempIndexSnowMelt::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::TempIndexSnowMelt::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")
cmf::upslope::connections::TempIndexSnowMelt::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::TempIndexSnowMelt::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::connections::TempIndexSnowMelt::q
"real q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::TempIndexSnowMelt::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::upslope::connections::TempIndexSnowMelt::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::TempIndexSnowMelt::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::TempIndexSnowMelt::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::connections::TempIndexSnowMelt::short_string "virtual
std::string short_string() const ";

%feature("docstring")
cmf::upslope::connections::TempIndexSnowMelt::to_string "virtual
std::string to_string() const ";


// File: classcmf_1_1math_1_1_time.xml
%feature("docstring") cmf::math::Time "

A time class, used to pass around current modelling times.

Timespans and dates in cmf are used with a special object, called
Time. An extra class has the advantage, that the user does not have to
remember, which unit of time he or she uses or what time unit is
accepted by a specific function of the model. Arithmetic and boolean
operators are supported by Time. Internally the time classes stores
the time as integer milliseconds, therefore rounding issues will only
appear at very small time ranges. Absolute time (like dates) are
represented as milliseconds gone by from Dec, 31st 1899. Microsoft
Excel dates are represented as days from that time, using floating
point numbers, therefore it is very simple to convert Excel time
representations to cmf time.

Another object is Date, which is doesn't provide the operators, but
has a nice printed version and some special date functions, like day
of year (DOY) and provides access to the current hour of day and so
on, which only applyto dates and not to time spans. You can convert
Time to Date an vice versa. The printing is not culture aware and uses
the European representation. If you use the Python standard library
datetime, conversion between Python time and cmf time is possible

Creating absolute time values (dates) Creating time spans

In principle, there are three ways to create time spans. One is to use
one of the static functions, another is to multiply an existing time
span (like one of the build in constants) or to substrate two absolute
times.

Available constants  : 4.1 seconds

: 2.3 hours (138 min)

: 2.3 hours (138 min)

: 60 hours (2.5 days)

: 7 days

: 365/12 days (30.4167 days)

: 365 days

Available operators:

time + time = time, time - time = time

time * float = time ,time / float = time

time/time=float

>, <, ==, !=

Conversions

Converting to python datetime

Converting to numbers

t.AsMilliseconds()

t.AsSeconds()

t.AsMinutes()

t.AsHours()

t.AsDays()

t.AsYears()

Creating time ranges

C++ includes: time.h ";

/*  Constructors  */

%feature("docstring")  cmf::math::Time::Time "Time(int day, int
month, int year, int hour=0, int minute=0, int second=0, int ms=0)

Construction from date and stores the time as Excel-Time (0 =
31.12.1899 0:00:00) ";

%feature("docstring")  cmf::math::Time::Time "Time(Date date)

Conversion constructor. ";

%feature("docstring")  cmf::math::Time::Time "Time(const Time &t)

Copy constructor. ";

%feature("docstring")  cmf::math::Time::Time "Time()

Standard constructor. ";

%feature("docstring")  cmf::math::Time::Time "Time(long long
milliseconds) ";

/*  Time unit conversion  */

%feature("docstring")  cmf::math::Time::AsDays "double AsDays() const

Time in days. ";

%feature("docstring")  cmf::math::Time::AsHours "double AsHours()
const

Time in hours. ";

%feature("docstring")  cmf::math::Time::AsMinutes "double AsMinutes()
const

Time in minutes. ";

%feature("docstring")  cmf::math::Time::AsSeconds "double AsSeconds()
const

Time in seconds. ";

%feature("docstring")  cmf::math::Time::AsMilliseconds "long long
AsMilliseconds() const

Time in milliseconds. ";

%feature("docstring")  cmf::math::Time::AsDate "Date AsDate() const
";

%feature("docstring")  cmf::math::Time::to_string "std::string
to_string(char seperator=':') ";

%feature("docstring")  cmf::math::Time::is_not_0 "bool is_not_0()
const ";

%feature("docstring")  cmf::math::Time::DOY "double DOY() const ";

/*  Arithmetic operators  */

%feature("docstring")  cmf::math::Time::times_in "long long
times_in(const Time &t1) const

Returns the number of times this is included in t1. ";

/*  Boolean Operators  */

%feature("docstring")  cmf::math::Time::long_time_if_zero "cmf::math::Time long_time_if_zero() const ";


// File: classcmf_1_1timetools_1_1timerange.xml
%feature("docstring") cmf::timetools::timerange "

Creates a generator of cmf.Time, similar to the Python range function
";

%feature("docstring")  cmf::timetools::timerange::__init__ "def
__init__(self, start, stop, step=day) ";

%feature("docstring")  cmf::timetools::timerange::__getitem__ "def
__getitem__(self, item) ";

%feature("docstring")  cmf::timetools::timerange::__iter__ "def
__iter__(self) ";

%feature("docstring")  cmf::timetools::timerange::__len__ "def
__len__(self) ";

%feature("docstring")  cmf::timetools::timerange::__repr__ "def
__repr__(self) ";


// File: classcmf_1_1math_1_1timeseries.xml
%feature("docstring") cmf::math::timeseries "

A timeseries is a list of values, equally distributed over time.

To create one, one have to provide as start date and a step size. The
end time is calculated from the number of values. Values queried for
times before the start time are returned as the first item, values
after the end time equal the last item. A timeseries with only one
item reacts like a scalar value.

Creating a time series

With this technique it is simple to read files or databases to fill
timeseries.<b> Using a timeseries</b>

C++ includes: timeseries.h ";

/*  Operators  */

/* Binary operators defined as free operators:

x = {+,-,*,/}

Defined for (x is one of the operators above): timeseries = timeseries
x timeseries

timeseries = double x timeseries

timeseries = timeseries x double

*/

%feature("docstring")  cmf::math::timeseries::inv "timeseries inv()
const ";

%feature("docstring")  cmf::math::timeseries::reduce_min "timeseries
reduce_min(cmf::math::Time begin, cmf::math::Time step) const

Creates a timeseries with a bigger timestep, containing the minimum.
";

%feature("docstring")  cmf::math::timeseries::reduce_max "timeseries
reduce_max(cmf::math::Time begin, cmf::math::Time step) const

Creates a timeseries with a bigger timestep, containing the maximum.
";

%feature("docstring")  cmf::math::timeseries::reduce_sum "timeseries
reduce_sum(cmf::math::Time begin, cmf::math::Time step) const

Creates a timeseries with a bigger timestep, containing the sum. ";

%feature("docstring")  cmf::math::timeseries::reduce_avg "timeseries
reduce_avg(cmf::math::Time begin, cmf::math::Time step) const

Creates a timeseries with a bigger timestep, containing the average.
";

%feature("docstring")  cmf::math::timeseries::floating_avg "timeseries floating_avg(cmf::math::Time window_width) const ";

%feature("docstring")  cmf::math::timeseries::floating_avg "timeseries floating_avg(size_t window_size) const ";

%feature("docstring")  cmf::math::timeseries::floating_max "timeseries floating_max(cmf::math::Time window_width) const ";

%feature("docstring")  cmf::math::timeseries::floating_min "timeseries floating_min(cmf::math::Time window_width) const ";

%feature("docstring")  cmf::math::timeseries::mean "double mean()
const ";

%feature("docstring")  cmf::math::timeseries::min "double min() const
";

%feature("docstring")  cmf::math::timeseries::max "double max() const
";

%feature("docstring")  cmf::math::timeseries::log "timeseries log()
const ";

%feature("docstring")  cmf::math::timeseries::log10 "timeseries
log10() const ";

%feature("docstring")  cmf::math::timeseries::power "timeseries
power(double exponent) const ";

%feature("docstring")  cmf::math::timeseries::exp "timeseries exp()
const ";

%feature("docstring")  cmf::math::timeseries::timeseries "timeseries(cmf::math::Time begin=cmf::math::Time(), cmf::math::Time
step=cmf::math::day, int interpolationmethod=1, size_t count=0)

Constructor of a time series.

Parameters:
-----------

begin:  First date of measurement

step:   Time between measurements

interpolationmethod:  Method for the interpolation (0 - Nearest
neighbor, 1- linear, 2 - cubic spline (not implemented yet)

count:  Initial number of items. Items are filled with 0.0 ";

%feature("docstring")  cmf::math::timeseries::timeseries "timeseries(const cmf::math::timeseries &ts) ";

%feature("docstring")  cmf::math::timeseries::add "void add(double
Value)

Appends a measurement. ";

%feature("docstring")  cmf::math::timeseries::adress "size_t adress()
const ";

%feature("docstring")  cmf::math::timeseries::as_array "cmf::math::num_array as_array() const ";

%feature("docstring")  cmf::math::timeseries::begin "cmf::math::Time
begin() const

First date of measurement. ";

%feature("docstring")  cmf::math::timeseries::clear "void clear() ";

%feature("docstring")  cmf::math::timeseries::copy "timeseries copy()
const ";

%feature("docstring")  cmf::math::timeseries::count_values "size_t
count_values() const

Number of valid values (=size - # of NaN's) ";

%feature("docstring")  cmf::math::timeseries::end "cmf::math::Time
end() const

Last date of measurements. ";

%feature("docstring")  cmf::math::timeseries::get_i "double
get_i(ptrdiff_t i) const ";

%feature("docstring")  cmf::math::timeseries::get_slice "cmf::math::timeseries get_slice(cmf::math::Time _begin,
cmf::math::Time _end, cmf::math::Time _step=cmf::math::Time()) ";

%feature("docstring")  cmf::math::timeseries::get_slice "cmf::math::timeseries get_slice(ptrdiff_t _begin, ptrdiff_t _end,
ptrdiff_t step=1) ";

%feature("docstring")  cmf::math::timeseries::get_t "double
get_t(cmf::math::Time t) const ";

%feature("docstring")  cmf::math::timeseries::interpolationpower "int
interpolationpower() const

Method for the interpolation (0 - Nearest neighbor, 1- linear, 2 -
cubic spline (not implemented yet) ";

%feature("docstring")  cmf::math::timeseries::is_empty "bool
is_empty() const

returns true if no values are added to the timeseries ";

%feature("docstring")  cmf::math::timeseries::remove_nodata "void
remove_nodata(double nodata_value) ";

%feature("docstring")  cmf::math::timeseries::set_begin "void
set_begin(cmf::math::Time new_begin) ";

%feature("docstring")  cmf::math::timeseries::set_i "void
set_i(ptrdiff_t i, double value) ";

%feature("docstring")  cmf::math::timeseries::set_interpolationpower "void set_interpolationpower(int new_ip) ";

%feature("docstring")  cmf::math::timeseries::set_slice "void
set_slice(cmf::math::Time _begin, cmf::math::Time _end,
cmf::math::timeseries values) ";

%feature("docstring")  cmf::math::timeseries::set_slice "void
set_slice(ptrdiff_t _begin, ptrdiff_t _end, cmf::math::timeseries
_values) ";

%feature("docstring")  cmf::math::timeseries::set_step "void
set_step(cmf::math::Time new_step) ";

%feature("docstring")  cmf::math::timeseries::set_t "void
set_t(cmf::math::Time t, double value) ";

%feature("docstring")  cmf::math::timeseries::size "size_t size()
const

Number of items in the timeseries. ";

%feature("docstring")  cmf::math::timeseries::step "cmf::math::Time
step() const

Time between the measurements. ";


// File: structcmf_1_1math_1_1timeseries_1_1timeseries__data.xml


// File: classcmf_1_1upslope_1_1_e_t_1_1timeseries_e_tpot.xml
%feature("docstring") cmf::upslope::ET::timeseriesETpot "

A timeseries driven evapotranspiration.

Uses a timeseries of measured or elsewhere modelled ETpot. Actual
Evapotranspiration is calculated from rootdepth and actual matrix
potential in the layers using Tact. The value of ETpot can be changed
during runtime

C++ includes: ET.h ";

%feature("docstring")
cmf::upslope::ET::timeseriesETpot::timeseriesETpot "timeseriesETpot(cmf::upslope::SoilLayer::ptr source,
cmf::water::flux_node::ptr ET_target, cmf::math::timeseries
ETpot_values) ";

%feature("docstring")  cmf::upslope::ET::timeseriesETpot::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")  cmf::upslope::ET::timeseriesETpot::ETpot "virtual real ETpot(cmf::math::Time t) const ";

%feature("docstring")
cmf::upslope::ET::timeseriesETpot::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::ET::timeseriesETpot::get_layer "SoilLayer::ptr get_layer() const ";

%feature("docstring")  cmf::upslope::ET::timeseriesETpot::get_ptr "ptr get_ptr() const ";

%feature("docstring")  cmf::upslope::ET::timeseriesETpot::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::ET::timeseriesETpot::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::ET::timeseriesETpot::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::ET::timeseriesETpot::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::upslope::ET::timeseriesETpot::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::upslope::ET::timeseriesETpot::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::ET::timeseriesETpot::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::ET::timeseriesETpot::refresh "void refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")  cmf::upslope::ET::timeseriesETpot::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::ET::timeseriesETpot::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::ET::timeseriesETpot::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")  cmf::upslope::ET::timeseriesETpot::short_string
"virtual std::string short_string() const ";

%feature("docstring")  cmf::upslope::ET::timeseriesETpot::to_string "virtual std::string to_string() const ";


// File: classcmf_1_1atmosphere_1_1_timeseries_rain_source.xml
%feature("docstring") cmf::atmosphere::TimeseriesRainSource "

A rainsource with a timeseries.

Simpler to use than a rainfall station if there are only few cells in
the project

C++ includes: precipitation.h ";

%feature("docstring")  cmf::atmosphere::TimeseriesRainSource::conc "virtual real conc(cmf::math::Time t, const cmf::water::solute &Solute)
const

Returns the concentration of a solute in the rainfall at time t. ";

%feature("docstring")
cmf::atmosphere::TimeseriesRainSource::connection_to "cmf::water::flux_connection* connection_to(const cmf::water::flux_node
&target)

Returns the connection between this and target. ";

%feature("docstring")
cmf::atmosphere::TimeseriesRainSource::flux3d_to "cmf::geometry::point flux3d_to(const cmf::water::flux_node &target,
cmf::math::Time t) ";

%feature("docstring")  cmf::atmosphere::TimeseriesRainSource::flux_to
"real flux_to(const cmf::water::flux_node &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from this into target\") ";

%feature("docstring")
cmf::atmosphere::TimeseriesRainSource::get_3d_flux "cmf::geometry::point get_3d_flux(cmf::math::Time t)

Returns the sum of all flux vectors. ";

%feature("docstring")
cmf::atmosphere::TimeseriesRainSource::get_connections "cmf::water::connection_list get_connections() const ";

%feature("docstring")
cmf::atmosphere::TimeseriesRainSource::get_intensity "real
get_intensity(cmf::math::Time t) const

Returns the actual rainfall intensity in mm/day. ";

%feature("docstring")
cmf::atmosphere::TimeseriesRainSource::get_potential "virtual real
get_potential(cmf::math::Time=cmf::math::never) const

Returns the water potential of the node in m waterhead.

The base class water storage always returns the height of the location
";

%feature("docstring")
cmf::atmosphere::TimeseriesRainSource::get_project "cmf::project&
get_project() const

Returns the project, this node is part of. ";

%feature("docstring")  cmf::atmosphere::TimeseriesRainSource::is_empty
"virtual double is_empty() const

Returns false. ";

%feature("docstring")
cmf::atmosphere::TimeseriesRainSource::is_storage "virtual bool
is_storage() const

true, if this is a waterstorage ";

%feature("docstring")
cmf::atmosphere::TimeseriesRainSource::RecalcFluxes "virtual bool
RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node.

WaterStorage overrides this, since state changes require an update of
the fluxes ";

%feature("docstring")
cmf::atmosphere::TimeseriesRainSource::remove_connection "bool
remove_connection(cmf::water::flux_node::ptr To)

Remove the connection. ";

%feature("docstring")
cmf::atmosphere::TimeseriesRainSource::set_potential "virtual void
set_potential(real new_potential)

Sets the potential of this flux node. ";

%feature("docstring")
cmf::atmosphere::TimeseriesRainSource::to_string "virtual std::string
to_string() const ";

%feature("docstring")
cmf::atmosphere::TimeseriesRainSource::waterbalance "real
waterbalance(cmf::math::Time t, const flux_connection *Without=0)
const

Returns the sum of all fluxes (positive and negative) at time t.

Single fluxes can be excluded from the calculation

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the waterbalance
(e.g. to prevent closed circuits) ";


// File: classcmf_1_1upslope_1_1connections_1_1_t_o_p_model_flow.xml
%feature("docstring") cmf::upslope::connections::TOPModelFlow "

Calculates a flux from a soil layer using TOPMODELs (Beven & Kirkby
1979) exponential transmissivity concept.



.. math::

     T = T_0 \\\\exp(-D_i/m) 

where:  :math:`T` is the actual
transmissivity of the profile in :math:`m^2/day`

:math:`T0` is the transmissivity of the profile at saturation

:math:`D_i` is the drained depth in m, calculated as :math:`(C-V)/A`, the capacity
of the layer - volume per area

:math:`m` a scaling factor in m

By using the transmissivity in Darcy's law and assuming the GW
gradient to be parallel to the topographic slope we get for the flow:


.. math::

     q = T_0 \\\\exp(-D_i/m) w s

where:  :math:`q` is the flux in
:math:`m^3/day`

:math:`w` is the flow width (unit contour length)

:math:`s` is the topographic slope between layer and outlet

TOPMODEL is based on the concept of drained depth, not, as cmf on the
concept of stored volume. Hence, negative volumes can occur if

C++ includes: subsurfacefluxes.h ";

%feature("docstring")
cmf::upslope::connections::TOPModelFlow::TOPModelFlow "TOPModelFlow(cmf::upslope::SoilLayer::ptr left,
cmf::water::flux_node::ptr right, real T0, real m, real flowwidth,
real distance=0) ";

%feature("docstring")  cmf::upslope::connections::TOPModelFlow::conc "real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::TOPModelFlow::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")
cmf::upslope::connections::TOPModelFlow::get_ptr "ptr get_ptr() const
";

%feature("docstring")
cmf::upslope::connections::TOPModelFlow::get_target "flux_node::ptr
get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::TOPModelFlow::get_target "flux_node::ptr
get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::TOPModelFlow::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::TOPModelFlow::get_tracer_filter "real
get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")
cmf::upslope::connections::TOPModelFlow::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::TOPModelFlow::left_node "flux_node::ptr
left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::connections::TOPModelFlow::q "real q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::TOPModelFlow::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::upslope::connections::TOPModelFlow::right_node "flux_node::ptr
right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::TOPModelFlow::set_tracer_filter "void
set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::TOPModelFlow::set_tracer_filter "void
set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::connections::TOPModelFlow::short_string "virtual
std::string short_string() const ";

%feature("docstring")
cmf::upslope::connections::TOPModelFlow::to_string "virtual
std::string to_string() const ";


// File: classcmf_1_1upslope_1_1connections_1_1_topographic_gradient_darcy.xml
%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy "

Calculates the lateral flow using the topographic gradient.



.. math::

     q_{lat} = \\\\frac{z_1 - z_2}{\\\\|C_1-C_2\\\\|} \\\\frac 12
    (T(C_1)+T(C_2)) w 

where  :math:`z_1 - z_2` is the topographic height
difference

:math:`\\\\|C_1-C_2\\\\|` is the distance from Cell 1 to Cell 2

:math:`T(C)` is the transmissivity of cell C, calculated by
SoilType::Transmissivity

:math:`w` is the width of the connection of the cells

C++ includes: subsurfacefluxes.h ";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::TopographicGradientDarcy
"TopographicGradientDarcy(cmf::upslope::SoilLayer::ptr left,
cmf::water::flux_node::ptr right, real FlowWidth, real Distance=0)

Creates the connection.

Parameters:
-----------

left:  Left node of the connection (needs to be soil water storage)

right:  Right node of the connection (can be any node)

FlowWidth:  the width of the connection - is multiplied by layer
thickness to get the interface area

Distance:  the length of the connection. If 0, the distance is
calculated from the position of the nodes ";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::exchange_target "void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::get_ptr "ptr
get_ptr() const ";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::get_tracer_filter
"real get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::get_tracer_filter
"real get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::set_tracer_filter
"void set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::set_tracer_filter
"void set_tracer_filter(solute S, real value) ";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::short_string "virtual std::string short_string() const ";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::to_string "virtual std::string to_string() const ";


// File: classcmf_1_1upslope_1_1_topology.xml
%feature("docstring") cmf::upslope::Topology "

represents the connectivity of cells to each other

C++ includes: Topology.h ";

%feature("docstring")  cmf::upslope::Topology::AddNeighbor "void
AddNeighbor(Cell &target, double flowwidth)

Adds a neighbor cell to the topology with flowwidth. ";

%feature("docstring")  cmf::upslope::Topology::AddNeighbor "void
AddNeighbor(Topology &target, double flowwidth) ";

%feature("docstring")  cmf::upslope::Topology::ContributingArea "double ContributingArea() const

Get the contributing area (steepest path upwards) ";

%feature("docstring")  cmf::upslope::Topology::flowwidth "double
flowwidth(Cell &target)

returns the flow width between this cell and the target cell. Returns
0 if no topology is defined ";

%feature("docstring")  cmf::upslope::Topology::flowwidth "double
flowwidth(Topology &target) ";

%feature("docstring")  cmf::upslope::Topology::get_position "cmf::geometry::point get_position() const

Returns the center of the cell. ";

%feature("docstring")  cmf::upslope::Topology::MainOutlet "Cell*
MainOutlet(bool forceRecalc=false)

Returns the mainoutlet (steepest lower neighbor) ";

%feature("docstring")  cmf::upslope::Topology::neighbor_count "size_t
neighbor_count() const

Returns the number of neighbors. ";

%feature("docstring")  cmf::upslope::Topology::RemoveNeighbor "void
RemoveNeighbor(Topology &target)

Removes the topological relation to the given cell. ";


// File: classcmf_1_1river_1_1_triangular_reach.xml
%feature("docstring") cmf::river::TriangularReach "

Structure for the description of reaches with a triangular cross
section.

Although double triangular cross section reach are rarely met, a
triangular reach does scale with its water load, and is therefore
preferable in case where nothing about IChannel geometry is known

C++ includes: ReachType.h ";

%feature("docstring")  cmf::river::TriangularReach::TriangularReach "TriangularReach(double l, double bankSlope=2)

Creates a new triangular reach type. ";

%feature("docstring")  cmf::river::TriangularReach::TriangularReach "TriangularReach(const TriangularReach &copy) ";

%feature("docstring")  cmf::river::TriangularReach::~TriangularReach "virtual ~TriangularReach() ";

%feature("docstring")  cmf::river::TriangularReach::A "virtual double
A(double V) const

Returns the area of the surface for a given volume. ";

%feature("docstring")  cmf::river::TriangularReach::copy "TriangularReach* copy() const ";

%feature("docstring")  cmf::river::TriangularReach::get_channel_width
"virtual double get_channel_width(double depth) const

Returns the width of the stream at a given depth 

.. math::

    
    w &=& 2 \\\\Delta\\\\ d 

. ";

%feature("docstring")  cmf::river::TriangularReach::get_depth "virtual double get_depth(double area) const

Returns the depth at a given crossection area 

.. math::

     d
    &=& \\\\sqrt{\\\\frac{A}{\\\\Delta}} 

. ";

%feature("docstring")
cmf::river::TriangularReach::get_flux_crossection "virtual double
get_flux_crossection(double depth) const

Returns the crossection area at a given depth 

.. math::

     A
    &=& d^2 \\\\Delta 

. ";

%feature("docstring")  cmf::river::TriangularReach::get_length "double get_length() const

Length of the reach. ";

%feature("docstring")  cmf::river::TriangularReach::get_nManning "virtual double get_nManning() const ";

%feature("docstring")
cmf::river::TriangularReach::get_wetted_perimeter "virtual double
get_wetted_perimeter(double depth) const

Returns the wetted perimeter at a given depth 

.. math::

     P
    &=& 2 d \\\\sqrt{1+\\\\Delta^2} 

. ";

%feature("docstring")  cmf::river::TriangularReach::h "virtual double
h(double V) const

Returns the depth of a given volume. ";

%feature("docstring")  cmf::river::TriangularReach::q "virtual double
q(double h, double slope) const ";

%feature("docstring")  cmf::river::TriangularReach::qManning "virtual
double qManning(double A, double slope) const

Calculates the flow rate from a given water volume in the reach


.. math::

     q_{Manning}&=& A R^{\\\\frac 2 3}
    \\\\sqrt{\\\\frac {\\\\Delta_z} n} \\\\\\\\ A &=& \\\\frac V l
    \\\\mbox{, (Crosssectional area of the wetted crossection, Volume per
    length)} \\\\\\\\ R &=& \\\\frac A {P(d)} \\\\\\\\ P(d) &=& \\\\mbox{
    the perimeter of the wetted crosssection, a function of reach depth}
    \\\\\\\\ d(V) &=& \\\\mbox{ the depth of the reach a function of the
    volume} \\\\\\\\ \\\\Delta_z &=& \\\\frac{z_{max} - z_{min}}{l}
    \\\\mbox{ Slope of the reach} 

.

Flow rate [m3/s]

Parameters:
-----------

A:  The area of the cross section [m2]

slope:  The slope of the reach [m/m] ";

%feature("docstring")  cmf::river::TriangularReach::set_nManning "virtual void set_nManning(double nManning) ";

%feature("docstring")  cmf::river::TriangularReach::typecode "char
typecode() const ";

%feature("docstring")  cmf::river::TriangularReach::V "virtual double
V(double h) const ";


// File: classcmf_1_1upslope_1_1_e_t_1_1_turc_e_t.xml
%feature("docstring") cmf::upslope::ET::TurcET "

Calculates ETpot after Turc (DVWK).

:math:`ET_{act}` is calculated using a WaterStressFunction



.. math::

     ET_{pot,Turc} = 0.0031 C(rH) (R_G + 209) \\\\frac{T}{T + 15}
    

where:  :math:`T` is the mean daily temperature

:math:`C(rH) = \\\\begin{cases} 1 + \\\\frac{50\\\\% - rH}{70\\\\%}, & rH < 50\\\\% \\\\\\\\ 1 & rH > 50\\\\% \\\\end{cases}`, a modification
parameter for low humidity

:math:`rH` relative Humidity in %

:math:`R_G` global radiation in :math:`J/cm^2`

C++ includes: ET.h ";

%feature("docstring")  cmf::upslope::ET::TurcET::TurcET "TurcET(cmf::upslope::SoilLayer::ptr source, cmf::water::flux_node::ptr
ET_target) ";

%feature("docstring")  cmf::upslope::ET::TurcET::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")  cmf::upslope::ET::TurcET::ETpot "real
ETpot(cmf::math::Time t) const ";

%feature("docstring")  cmf::upslope::ET::TurcET::exchange_target "void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")  cmf::upslope::ET::TurcET::get_layer "SoilLayer::ptr get_layer() const ";

%feature("docstring")  cmf::upslope::ET::TurcET::get_ptr "ptr
get_ptr() const ";

%feature("docstring")  cmf::upslope::ET::TurcET::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::ET::TurcET::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")  cmf::upslope::ET::TurcET::get_tracer_filter "real get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")  cmf::upslope::ET::TurcET::get_tracer_filter "real get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::upslope::ET::TurcET::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::upslope::ET::TurcET::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::ET::TurcET::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::ET::TurcET::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")  cmf::upslope::ET::TurcET::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::upslope::ET::TurcET::set_tracer_filter "void set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")  cmf::upslope::ET::TurcET::set_tracer_filter "void set_tracer_filter(solute S, real value) ";

%feature("docstring")  cmf::upslope::ET::TurcET::short_string "virtual std::string short_string() const ";

%feature("docstring")  cmf::upslope::ET::TurcET::to_string "virtual
std::string to_string() const ";


// File: classcmf_1_1upslope_1_1_van_genuchten_mualem.xml
%feature("docstring") cmf::upslope::VanGenuchtenMualem "

Provides the use of the Van Genuchten - Mualem retention curve (Van
Genuchten 1980)

Head - moisture relationship: 

.. math::

     W(\\\\theta) &=&
    \\\\frac{\\\\theta - \\\\theta_r}{\\\\theta_s - \\\\theta_r} \\\\\\\\
    K(W) &=& K_{sat} \\\\sqrt{W}
    \\\\left(1-\\\\left(1-W^{1/m}\\\\right)^m\\\\right)^2 \\\\\\\\ m &=&
    1-\\\\frac 1 n \\\\\\\\ \\\\Psi(W) &=& 0.01 \\\\frac{m}{cm}
    \\\\frac{{\\\\left(1-{W}^{\\\\frac{1}{m}}\\\\right)
    }^{\\\\frac{1}{n}}}{\\\\alpha\\\\,{W}^{\\\\frac{1}{m\\\\,n}}} \\\\\\\\
    W(\\\\Psi) &=&
    \\\\left(1+\\\\left(\\\\alpha\\\\,100\\\\frac{cm}{m}\\\\Psi\\\\right)^n\\\\right)^{-m}
    

 where:  :math:`K` is the conductivity in :math:`\\\\frac m{day}`

:math:`W` is the wetness (Volume of soil water per volume of pores)

:math:`n` is a shape parameter of the retention curve

:math:`\\\\alpha` is inverse of the air entry potential in :math:`cm^{-1}`

:math:`\\\\Psi(W)` is the matric potential in :math:`m H_2O` at wetness W

C++ includes: RetentionCurve.h ";

%feature("docstring")
cmf::upslope::VanGenuchtenMualem::VanGenuchtenMualem "VanGenuchtenMualem(real Ksat=15, real phi=0.5, real alpha=0.2178, real
n=1.211, real m=-1, real theta_r=0.0, real w0=0.99)

Creates a van Genuchten-Mualem retention curve.

Parameters:
-----------

Ksat:  Saturated conductivity in :math:`\\\\frac m{day}`

phi:  Porosity in :math:`\\\\frac{m^3 Pores}{m^3 Soil}`

alpha:  Van Genuchten :math:`\\\\alpha` in :math:`\\\\frac 1{cm}`

n:  Van Genuchten n

m:  Van Genuchten m parameter, if negative m is calculated as :math:`1-\\\\frac 1 n`

theta_r:  Water content for :math:`\\\\lim\\\\limits_{\\\\Psi_M \\\\rightarrow -\\\\infty}{\\\\theta(\\\\Psi_M)}`

w0:  Wetness above the parabolic extrapolation is used instead of the
Van Genuchten curve (usually calculated with fit_w0) ";

%feature("docstring")
cmf::upslope::VanGenuchtenMualem::~VanGenuchtenMualem "virtual
~VanGenuchtenMualem() ";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::copy "VanGenuchtenMualem* copy() const ";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::Diffusivity "cmf::math::num_array Diffusivity(cmf::math::num_array &wetness) ";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::Diffusivity "virtual real Diffusivity(real wetness) const

Returns the diffusivity of the soil according to its wetness as given
by VanGenuchten 1980.

Deprecated The current implementation goes to infinity at saturation,
as noted by VanGenuchten. Diffusivity is therefore currently not
usable in any model.



.. math::

    D(W) = K(W)\\\\left|\\\\frac{d\\\\Psi}{d\\\\theta}\\\\right|\\\\
    eq. 10

where:  :math:`D(W)` Diffusivity in :math:`m^2/day`

:math:`K(W)` Conductivity as a function of saturation W in m/day

:math:`\\\\Psi` Pressure head

:math:`\\\\theta` water content of the soil

Applying Van Genuchten theory (Van Genuchten 1980) yields to:


.. math::

    D(W) = \\\\frac{(1-m)K_{sat}}{\\\\alpha m \\\\Phi}
    W^{l-1/m}\\\\left(\\\\left(1-W^{1/m}\\\\right)^{-m} +
    \\\\left(1-W^{1/m}\\\\right)^{m} -2\\\\right)

where:  :math:`m = 1 - \\\\frac 1 n` acc. Mualem theory

:math:`K_{sat}` saturated conductivity in m/day

:math:`\\\\alpha` inverse water entry potential in 1/m. Note :math:`\\\\alpha` is
given in cmf in 1/cm

:math:`\\\\Phi` porosity

:math:`W = \\\\frac{\\\\theta - \\\\theta_r}{\\\\Phi - \\\\theta_r}`
saturation of the soil ";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::dPsiM_dW "cmf::math::num_array dPsiM_dW(const cmf::math::num_array &wetness)
const ";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::dPsiM_dW "virtual real dPsiM_dW(real wetness) const

returns :math:`\\\\tfrac{d\\\\Psi_M}{dW}`



.. math::

    \\\\frac{0.01 w^{\\\\frac{1}{m}} w^{- \\\\frac{1}{m n}}
    \\\\left(- w^{\\\\frac{1}{m}} +
    1\\\\right)^{\\\\frac{1}{n}}}{\\\\alpha m n w \\\\left(-
    w^{\\\\frac{1}{m}} + 1\\\\right)} + \\\\frac{0.01 w^{- \\\\frac{1}{m
    n}} \\\\left(- w^{\\\\frac{1}{m}} +
    1\\\\right)^{\\\\frac{1}{n}}}{\\\\alpha m n w}

";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::FillHeight "virtual real FillHeight(real lowerDepth, real Area, real Volume) const

Returns the thickness of a soil column with a certain pore volume. ";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::fit_w0 "real
fit_w0(real w1=1.01, real Psi_p=1.0, real tolerance=0.05)

Fits the break point wetness w0, to ensure a specific oversaturation
at a given hydrostatic potential.

Parameters:
-----------

w1:  The oversaturation wetness to archieve (>1), default = 1.01

Psi_p:  the hydrostatic potential for w1, default = +1.0 m

tolerance:  ";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::K "cmf::math::num_array K(const cmf::math::num_array &wetness) const ";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::K "virtual
real K(real wetness) const

returns the conductivity of the soil at a given saturation



.. math::

    K(W) = K_{sat} \\\\sqrt{W}
    \\\\left(1-\\\\left(1-W^{1/m}\\\\right)^m\\\\right)^2 

";

%feature("docstring")
cmf::upslope::VanGenuchtenMualem::MatricPotential "cmf::math::num_array MatricPotential(const cmf::math::num_array
&wetness) const ";

%feature("docstring")
cmf::upslope::VanGenuchtenMualem::MatricPotential "virtual real
MatricPotential(real wetness) const

returns the matrix potential at a given saturation



.. math::

    \\\\Psi(W) = 0.01 \\\\frac{m}{cm}
    \\\\frac{{\\\\left(1-{W}^{\\\\frac{1}{m}}\\\\right)
    }^{\\\\frac{1}{n}}}{\\\\alpha\\\\,{W}^{\\\\frac{1}{m\\\\,n}}} 

";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::Porosity "virtual real Porosity(real depth) const

Returns the porosity at a certain depth. ";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::theta "cmf::math::num_array theta(const cmf::math::num_array &wetness) const
";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::theta "virtual real theta(real wetness) const

returns the water content :math:`theta` for a given wetness ";

%feature("docstring")
cmf::upslope::VanGenuchtenMualem::Transmissivity "virtual real
Transmissivity(real upperDepth, real lowerDepth, real wetness) const
";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::VoidVolume "virtual real VoidVolume(real upperDepth, real lowerDepth, real Area)
const

Returns the void volume of a soil column. ";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::Wetness "cmf::math::num_array Wetness(const cmf::math::num_array &suction)
const ";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::Wetness "virtual real Wetness(real suction) const

returns the saturation at a given suction (matrix potential).



.. math::

     W(\\\\Psi) =
    \\\\left(1+\\\\left(\\\\alpha\\\\,100\\\\frac{cm}{m}\\\\Psi\\\\right)^n\\\\right)^{-m}
    

";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::Wetness_eff "virtual real Wetness_eff(real wetness, real pF_r=4.2) const

Returns the effective wetness, using a residual pF value 

.. math::

    w_{eff}
    =
    \\\\frac{w_{act}-w\\\\left(pF_r\\\\right)}{1-w\\\\left(pF_r\\\\right)}

.
";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::Wetness_pF "real Wetness_pF(real pF) const

returns the volumetric water content at a given pF value ";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::Wetness_pF "cmf::math::num_array Wetness_pF(const cmf::math::num_array &pF) const
";


// File: classcmf_1_1upslope_1_1vegetation_1_1_vegetation.xml
%feature("docstring") cmf::upslope::vegetation::Vegetation "

Holds the vegetation parameters for the calculation of ET and
fractionating rainfall. Not every ET method uses all parameters.

C++ includes: StructVegetation.h ";

%feature("docstring")
cmf::upslope::vegetation::Vegetation::Vegetation "Vegetation(double
_LAI=2.88, double _Height=0.12, double _RootDepth=0.25, double
_StomatalResistance=100, double _albedo=0.23, double _CanopyClosure=1,
double _CanopyCapacityPerLAI=0.1, double _fraction_at_rootdepth=1.0)
";

%feature("docstring")
cmf::upslope::vegetation::Vegetation::RootFraction "double
RootFraction(double upperBoundary, double lowerBoundary) const

Returns the fraction of root mass in a segment of the soil column per
total root mass.

Parameters:
-----------

upperBoundary:  lowerBoundary:  Upper and lower depth below ground of
the segment The method to claculate the distribution of roots in the
soil depends on the fraction_at_rootdepth :math:`f_{r0}` parameter. If
:math:`f_{r0}=1`, a uniform distribution from the ground to the root depth
is assumed. If :math:`f_{r0}<1`, an exponential decline of root density with
depth is assumed, that :math:`f_{r0}` of the roots are above the root depth
r0. 

.. math::

    f_{r}(z_u,z_l) = (\\\\frac 1 {1-f_{r0}})^{z_l-r0} - (\\\\frac
    1 {1-f_{r0}})^{z_u-r0}

";

%feature("docstring")
cmf::upslope::vegetation::Vegetation::RootFraction "cmf::math::num_array RootFraction(const cmf::math::num_array
&thickness) const ";

%feature("docstring")
cmf::upslope::vegetation::Vegetation::RootLength "double RootLength()
const

Returns the average root length in m/m2. ";


// File: classcmf_1_1river_1_1volume__height__function.xml
%feature("docstring") cmf::river::volume_height_function "

A wrapper class for volume / height functional relations.

C++ includes: ReachType.h ";

%feature("docstring")
cmf::river::volume_height_function::volume_height_function "volume_height_function(const volume_height_function &for_copy)

Copy constructable. ";

%feature("docstring")
cmf::river::volume_height_function::volume_height_function "volume_height_function(const IVolumeHeightFunction &for_copy)

Wrapper for any IVolumeHeightFunction. ";

%feature("docstring")
cmf::river::volume_height_function::~volume_height_function "virtual
~volume_height_function() ";

%feature("docstring")  cmf::river::volume_height_function::A "double
A(double V) const

Returns the area of the surface for a given volume. ";

%feature("docstring")  cmf::river::volume_height_function::copy "volume_height_function* copy() const ";

%feature("docstring")  cmf::river::volume_height_function::h "double
h(double V) const

Returns the depth of a given volume. ";

%feature("docstring")  cmf::river::volume_height_function::q "double
q(double h, double slope) const ";

%feature("docstring")  cmf::river::volume_height_function::V "double
V(double h) const ";


// File: classcmf_1_1upslope_1_1_e_t_1_1_volume_stress.xml
%feature("docstring") cmf::upslope::ET::VolumeStress "

A WaterStressFunction based on the stored water volume of a layer.

If the layer contains more water than V1, ET is not limited (
ET=ETpot). Below V1 ET goes linear to 0.0 at V0

C++ includes: waterstress.h ";

%feature("docstring")  cmf::upslope::ET::VolumeStress::VolumeStress "VolumeStress(real V1, real V0)

Creates a new water stress function for water volume.

Parameters:
-----------

V1:  Water content in :math:`m^3` without stress ( ET=ETpot)

V0:  Water content in :math:`m^3` with full stress ( ET=0.0) ";

%feature("docstring")  cmf::upslope::ET::VolumeStress::VolumeStress "VolumeStress(const VolumeStress &other) ";

%feature("docstring")  cmf::upslope::ET::VolumeStress::~VolumeStress "virtual ~VolumeStress() ";

%feature("docstring")  cmf::upslope::ET::VolumeStress::copy "cmf::upslope::ET::VolumeStress* copy() const

Creates a new copy of this wetness. ";

%feature("docstring")  cmf::upslope::ET::VolumeStress::Tact "virtual
real Tact(const cmf::upslope::SoilLayer *soillayer, real Tpot) const

Calculates the water stress for a layer.

Parameters:
-----------

soillayer:  The cmf::upslope::SoilLayer this stress function belongs
to

Tpot:  Potential Transpiration in mm/day (for the full profile)

Actual flux from layer in m3/day ";

%feature("docstring")  cmf::upslope::ET::VolumeStress::to_string "std::string to_string() const ";


// File: classcmf_1_1water_1_1waterbalance__integrator.xml
%feature("docstring") cmf::water::waterbalance_integrator "

The waterbalance_integrator is an integratable for precise output of
the average water balance of a flux_node over time.

It can be added to a solver (any cmf::math::Integrator), which is than
calling the integrate method at each substep.

C++ includes: flux_node.h ";

%feature("docstring")
cmf::water::waterbalance_integrator::waterbalance_integrator "waterbalance_integrator(cmf::water::flux_node::ptr node) ";

%feature("docstring")  cmf::water::waterbalance_integrator::avg "double avg() const

Returns the average flux over the integration time in m3/day. ";

%feature("docstring")  cmf::water::waterbalance_integrator::get_node "flux_node::ptr get_node() const

Returns the node of this integrator. ";

%feature("docstring")  cmf::water::waterbalance_integrator::integrate
"void integrate(cmf::math::Time until)

Integrates the flux a timestep further. Note: until is an absolut
time. If until is before t0, the integration is initilized again. ";

%feature("docstring")
cmf::water::waterbalance_integrator::integration_t "cmf::math::Time
integration_t() const

Returns the duration of the integration. ";

%feature("docstring")  cmf::water::waterbalance_integrator::reset "void reset(cmf::math::Time t)

Initializes the integration. ";

%feature("docstring")  cmf::water::waterbalance_integrator::set_node "void set_node(cmf::water::flux_node::ptr node) ";

%feature("docstring")  cmf::water::waterbalance_integrator::sum "double sum() const

Returns the total absolute waterbalance of the node in integration
time [m3]. ";

%feature("docstring")  cmf::water::waterbalance_integrator::t0 "cmf::math::Time t0() const

Returns the start time of the integration. ";


// File: classcmf_1_1water_1_1_waterbalance_flux.xml
%feature("docstring") cmf::water::WaterbalanceFlux "

Routes the sum of all other fluxes to a target.



.. math::

     q_{1,0} = \\\\sum_{i=2}^N{q_{1,i}(V_1,V_i,t)}

where:
:math:`q_{i,j}` is the flux between the two node i and j. Subscript 0 is the
right node, subscript 1 is the left node and 2..N are the nodes
connected to the left node, except for the right node

C++ includes: simple_connections.h ";

%feature("docstring")  cmf::water::WaterbalanceFlux::WaterbalanceFlux
"WaterbalanceFlux(flux_node::ptr source, flux_node::ptr target) ";

%feature("docstring")  cmf::water::WaterbalanceFlux::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux.

If not overridden, it returns the concentration of the source of the
flux (direction depending) ";

%feature("docstring")  cmf::water::WaterbalanceFlux::exchange_target "void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")  cmf::water::WaterbalanceFlux::get_ptr "ptr
get_ptr() const ";

%feature("docstring")  cmf::water::WaterbalanceFlux::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::water::WaterbalanceFlux::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")  cmf::water::WaterbalanceFlux::get_tracer_filter
"real get_tracer_filter()

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")  cmf::water::WaterbalanceFlux::get_tracer_filter
"real get_tracer_filter(solute S)

A value ranging from 0 to 1 to filter tracers out of the water flux.
";

%feature("docstring")  cmf::water::WaterbalanceFlux::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::water::WaterbalanceFlux::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::water::WaterbalanceFlux::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::water::WaterbalanceFlux::refresh "void
refresh(cmf::math::Time t)

Performes a new calculation of the flux. ";

%feature("docstring")  cmf::water::WaterbalanceFlux::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::water::WaterbalanceFlux::set_tracer_filter
"void set_tracer_filter(real value)

A value ranging from 0 to 1 to filter tracers out of the water flux.

1.0 is no filter and 0.0 means no solute is crossing this connection
";

%feature("docstring")  cmf::water::WaterbalanceFlux::set_tracer_filter
"void set_tracer_filter(solute S, real value) ";

%feature("docstring")  cmf::water::WaterbalanceFlux::short_string "virtual std::string short_string() const ";

%feature("docstring")  cmf::water::WaterbalanceFlux::to_string "virtual std::string to_string() const ";


// File: classcmf_1_1water_1_1_water_storage.xml
%feature("docstring") cmf::water::WaterStorage "

A state variable for the storage of water.

A class for the storage of water. The state is the volume of water
stored in :math:`m^3` The derivative function is given by:


.. math::

     \\\\frac{dV}{dt}&=&\\\\sum_{f=1}^{F} q_f \\\\\\\\
    F&=& \\\\mbox{Number of fluxes in water storage} \\\\\\\\ q_f&=&
    \\\\mbox{Water flux in } \\\\frac{m^3}{day} \\\\\\\\
    



Todo Check the head based state mode

C++ includes: WaterStorage.h ";

/*  Overrides of flux_node  */

%feature("docstring")  cmf::water::WaterStorage::RecalcFluxes "virtual bool RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node.

WaterStorage overrides this, since state changes require an update of
the fluxes ";

%feature("docstring")  cmf::water::WaterStorage::is_empty "virtual
double is_empty() const

Returns true if the node has no water. ";

%feature("docstring")  cmf::water::WaterStorage::to_string "virtual
std::string to_string() const ";

%feature("docstring")  cmf::water::WaterStorage::WaterStorage "WaterStorage(cmf::project &project, const std::string &Name=\"\",
double InitialState=0, double scale=1)

creates a water storage (abstract class)

Parameters:
-----------

project:  The project the waterstorage belongs to

Name:  Name of the water storage

InitialState:  Initial water content in m3

scale:  A kind of \"standard size\" in m3 of the water storage to
scale tolerances, default 1m3 ";

%feature("docstring")  cmf::water::WaterStorage::add_connected_states
"virtual void add_connected_states(cmf::math::StateVariable::list
&states) ";

%feature("docstring")  cmf::water::WaterStorage::conc "real
conc(const cmf::water::solute &_Solute) const

Returns the concentration of the given solute. ";

%feature("docstring")  cmf::water::WaterStorage::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute) const
override

Returns the current WaterQuality (concentration of all solutes) ";

%feature("docstring")  cmf::water::WaterStorage::conc "void
conc(const cmf::water::solute &_Solute, real NewConcetration)

Sets a new concentration. ";

%feature("docstring")  cmf::water::WaterStorage::connection_to "cmf::water::flux_connection* connection_to(const cmf::water::flux_node
&target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::water::WaterStorage::dxdt "virtual real
dxdt(const cmf::math::Time &time)

Returns the derivate of the state variable at time time. ";

%feature("docstring")  cmf::water::WaterStorage::flux3d_to "cmf::geometry::point flux3d_to(const cmf::water::flux_node &target,
cmf::math::Time t) ";

%feature("docstring")  cmf::water::WaterStorage::flux_to "real
flux_to(const cmf::water::flux_node &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from this into target\") ";

%feature("docstring")  cmf::water::WaterStorage::get_3d_flux "cmf::geometry::point get_3d_flux(cmf::math::Time t)

Returns the sum of all flux vectors. ";

%feature("docstring")  cmf::water::WaterStorage::get_abs_errtol "virtual real get_abs_errtol(real rel_errtol) const ";

%feature("docstring")  cmf::water::WaterStorage::get_connections "cmf::water::connection_list get_connections() const ";

%feature("docstring")  cmf::water::WaterStorage::get_potential "real
get_potential(cmf::math::Time t=cmf::math::never) const override

Returns the water potential of the node in m waterhead.

The base class water storage always returns the height of the location
";

%feature("docstring")  cmf::water::WaterStorage::get_project "cmf::project& get_project() const

Returns the project, this node is part of. ";

%feature("docstring")  cmf::water::WaterStorage::get_state "real
get_state() const

Returns the current state of the variable. ";

%feature("docstring")
cmf::water::WaterStorage::get_state_variable_content "char
get_state_variable_content() const

A character indicating the integrated variable (either 'V' for Volume
or 'h' for head) ";

%feature("docstring")  cmf::water::WaterStorage::get_volume "virtual
real get_volume() const

Returns the volume of water in this storage in m3 ";

%feature("docstring")  cmf::water::WaterStorage::is_connected "virtual bool is_connected(const cmf::math::StateVariable &other) const

Returns True if this waterstorage is effected by another state. ";

%feature("docstring")  cmf::water::WaterStorage::is_storage "virtual
bool is_storage() const

Returns true, since this is a storage. ";

%feature("docstring")  cmf::water::WaterStorage::remove_connection "bool remove_connection(cmf::water::flux_node::ptr To)

Remove the connection. ";

%feature("docstring")  cmf::water::WaterStorage::set_potential "void
set_potential(real newpotential) override

Sets the potential of this flux node. ";

%feature("docstring")  cmf::water::WaterStorage::set_state "void
set_state(real newState)

Gives access to the state variable. ";

%feature("docstring")
cmf::water::WaterStorage::set_state_variable_content "void
set_state_variable_content(char content)

A character indicating the integrated variable (either 'V' for Volume
or 'h' for head) ";

%feature("docstring")  cmf::water::WaterStorage::set_volume "virtual
void set_volume(real newwatercontent)

Sets the volume of water in this storage in m3 ";

%feature("docstring")  cmf::water::WaterStorage::Solute "SoluteStorage& Solute(const cmf::water::solute _Solute)

Returns the water quality of the water storage. ";

%feature("docstring")  cmf::water::WaterStorage::Solute "const
SoluteStorage& Solute(const cmf::water::solute _Solute) const ";

%feature("docstring")  cmf::water::WaterStorage::waterbalance "real
waterbalance(cmf::math::Time t, const flux_connection *Without=0)
const

Returns the sum of all fluxes (positive and negative) at time t.

Single fluxes can be excluded from the calculation

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the waterbalance
(e.g. to prevent closed circuits) ";


// File: structcmf_1_1atmosphere_1_1_weather.xml
%feature("docstring") cmf::atmosphere::Weather "

A structure holding meteorological information, excluding
precipitation

C++ includes: Weather.h ";

%feature("docstring")  cmf::atmosphere::Weather::Weather "Weather(double T=15.0, double Tmax=17.0, double Tmin=13.0, double
rH=70.0, double wind=2.0, double sunshine=0.5, double Rs=15, double
Ra=30, double daylength=12)

Creates a \"weather\" from given data.

Parameters:
-----------

T:  actual Temperature in deg C

Tmax:  daily maximum Temperature in deg C

Tmin:  daily minimum Temperature in deg C

rH:  actual relative humidity in % [0..100]

wind:  actual wind speed in m/s

sunshine:  actual fraction of sunshine duration per potential sunshine
duration in h/h

Rs:  actual incoming shortwave global radiation in MJ/(m2 day)

Ra:  actual extraterrestrial shortwave global radiation in MJ/(m2 day)

daylength:  length of the day in h ";

%feature("docstring")  cmf::atmosphere::Weather::Rn "double Rn(double
albedo, bool daily=false) const

Calculates the net radiation flux :math:`R_n \\\\left[\\\\frac{MJ}{m^2 day}\\\\right]`.



.. math::

     R_{n} = R_{ns} - R_{nl} \\\\\\\\ \\\\mbox{ Net
    short wave radiation: }R_{ns} = (1-\\\\alpha) R_s \\\\\\\\ \\\\mbox{
    Net long wave radiation: }R_{nl} = R_{black}\\\\ \\\\beta_{v}\\\\
    \\\\beta_{c} \\\\\\\\ \\\\mbox{Black body radiation: } R_{black} =
    \\\\left\\\\{\\\\begin{array}{cl} \\\\sigma T^4 & \\\\mbox{for less
    than daily time steps} \\\\\\\\ \\\\sigma \\\\frac {T_{max}^4 +
    T_{min}^4} 2 & \\\\mbox{for daily time steps} \\\\end{array}
    \\\\right. \\\\\\\\ T = \\\\mbox{Temperature }[K] \\\\\\\\ \\\\sigma =
    4.903\\\\ 10^{-9} \\\\frac{MJ}{K^4 m^2 day} \\\\mbox{ Stefan-Boltzmann
    constant } \\\\\\\\ \\\\mbox{Long wave reflectance: } \\\\\\\\
    \\\\mbox{by water vapor: }\\\\beta_{v} = 0.34 - 0.14 \\\\sqrt{e_a}
    \\\\\\\\ \\\\mbox{ by clouds: }\\\\beta_{c} = 0.1 + 0.9 \\\\frac n N
    



Parameters:
-----------

albedo:  the albedo :math:`\\\\alpha` of the surface

daily:  If true, the net radiation for daily averages will be
calculated ";

%feature("docstring")  cmf::atmosphere::Weather::to_string "std::string to_string() const

Returns a string representation. ";


// File: structcmf_1_1upslope_1_1_soil_layer_1_1wet.xml


// File: namespacecmf.xml
%feature("docstring")  cmf::atmosphere::connect_cells_with_flux "def
cmf.connect_cells_with_flux(cells, connection, start_at_layer=0)

Connects all cells in cells (sequence or generator) with a flux
connection connection is an subclass of cmf.FluxConnection which
exposes the cell_connector callable                 (e.g. lateral
subsurface fluxes and surface manning flux) start_at_layer : if the
flux connection should only be used for deeper layers ";


// File: namespacecmf_1_1atmosphere.xml
%feature("docstring")  cmf::atmosphere::extraterrestrial_radiation "double cmf::atmosphere::extraterrestrial_radiation(cmf::math::Time t,
double longitude=8, double latitude=51, double time_zone=1, bool
daily=false)

Calculates the global radiation in MJ/(m2 day) from the sun position
and the sunshine fraction.

Parameters:
-----------

t:  actual time step

longitude:  latitude:  Geographical position in degree. Latitude is
only taken into acount for subdaily calculation

time_zone:  Offset by timezone from GMT, eg. central Europe=1 US west
coast = -8

daily:  If true, the average radiation for the whole day is given
(therefore latitude and time zone ignored), otherwise the average of
the current hour is returned

The calculation of the global radiation
followshttp://www.fao.org/docrep/X0490E/x0490e07.htm#radiation.

The following formula is used:  :math:`\\\\phi` Latitude in :math:`rad`

:math:`\\\\delta = 0.409 \\\\sin\\\\left(\\\\frac{2\\\\pi}{365}DOY - 1.39\\\\right)` Declination, DOY is day of year

:math:`\\\\omega_s = \\\\arccos(-\\\\tan\\\\phi\\\\tan\\\\delta)` Sunset
angle

:math:`G_{sc} = 0.0802 \\\\frac{MJ}{m^2min}` Solar constant

:math:`d_r = 1+0.033 \\\\cos\\\\left(\\\\frac{2\\\\pi}{365}DOY\\\\right)`
Inverse relative distance Earth-Sun

:math:`b = \\\\frac{2\\\\pi(DOY-81)}{364}`

:math:`S_c = 0.1645\\\\sin(2b)-0.1255\\\\cos(b)-0.025\\\\sin(b)` Seasonal
correction for solar time

:math:`\\\\omega = \\\\frac{\\\\pi}{12} \\\\left(t_h+\\\\frac{(\\\\mbox{geogr. Longitude})^\\\\circ}{15^\\\\circ}-\\\\mbox{Timezone}+S_c-12\\\\right)` solar time in :math:`rad`

If daily: :math:`R_a = \\\\frac{24\\\\ 60}{\\\\pi}G_{sc}\\\\ d_r \\\\left(\\\\omega_s \\\\sin\\\\phi \\\\sin\\\\delta + \\\\cos\\\\phi \\\\cos\\\\delta \\\\sin\\\\omega_s\\\\right)`

If sub daily: :math:`R_a = \\\\frac{12\\\\ 24\\\\ 60}{\\\\pi}G_{sc}\\\\ d_r \\\\left(\\\\left(\\\\omega^+ -\\\\omega^-\\\\right) \\\\sin\\\\phi \\\\sin\\\\delta + \\\\cos\\\\phi \\\\cos\\\\delta \\\\left(\\\\sin\\\\omega^+ - \\\\sin\\\\omega^-\\\\right)\\\\right)`

:math:`\\\\omega^+,\\\\omega^- = \\\\omega \\\\pm\\\\frac{\\\\pi}{24}` ";

%feature("docstring")  cmf::atmosphere::global_radiation "double
cmf::atmosphere::global_radiation(double Ra, double height, double
sunshine_fraction)

Calculates the global radiation in MJ/(m2 day) from the sun position
and the sunshine fraction.

Parameters:
-----------

Ra:  extra terrestrial radiation

height:  Height above sea level

sunshine_fraction:  Fraction of sunshine hours per potential sunshine
duration in h/h

The calculation of the global radiation
followshttp://www.fao.org/docrep/X0490E/x0490e07.htm#radiation.

The following formula is used:  :math:`R_a(t, \\\\phi, \\\\lambda)`
Extraterrestrial radiation (
cmf::atmosphere::extraterrestrial_radiation) :math:`\\\\frac{MJ}{m^2 day}`

:math:`\\\\frac n N` Fractional sunshine duration

:math:`R_s = \\\\left(0.25+\\\\left(0.5+2\\\\ 10^{-5}z\\\\right)\\\\frac{n}{N}\\\\right)R_a` Global radiation in
:math:`\\\\frac{MJ}{m^2 day}`

:math:`z`: Height a.s.l. in m ";

%feature("docstring")  cmf::atmosphere::MJ_to_watts "double
cmf::atmosphere::MJ_to_watts(double MJ)

Transforms an energy flux from MJ/day to W. ";

%feature("docstring")  cmf::atmosphere::Pressure "double
cmf::atmosphere::Pressure(double height)

Returns the average air pressure for a height (m a.s.l.) ";

%feature("docstring")  cmf::atmosphere::rH_from_vpd "double
cmf::atmosphere::rH_from_vpd(double T, double vpd)

Returns the rel.

humidity in % for temperature T [degC] and vapor pressure deficit vpd
[Pa]

the rel. humidity is calculated from the vapor pressure deficit :math:`vpd = e_s - e_a` as: 

.. math::

    rH = 100 * \\\\frac{e_a}{e_s(T)}, e_a = e_s(T) -
    vpd

The definition is
fromhttp://www.fao.org/docrep/X0490E/x0490e07.htm#concepts

Parameters:
-----------

T:  Air temperature in degC

vpd:  Vapor pressure deficit in Pa

:math:`e_s(T)` is calculated using cmf::atmosphere::vapor_pressure(double)
";

%feature("docstring")  cmf::atmosphere::vapour_pressure "double
cmf::atmosphere::vapour_pressure(double T)

Returns the saturated vapor pressure in Pa for temperature T [degC].

The saturated vapor pressure :math:`e_s` is calculated follwing the
following formula 

.. math::

    e_s = 0.6108 \\\\exp{\\\\frac{17.27
    T}{T+237.3}}

The definition is
fromhttp://www.fao.org/docrep/X0490E/x0490e07.htm#concepts ";

%feature("docstring")  cmf::atmosphere::vpd_from_rH "double
cmf::atmosphere::vpd_from_rH(double T, double rH)

Returns the vapor pressure deficit in Pa for temperature T [degC] and
rel.

humidity rH [%]

The vapor pressure deficit :math:`e_s - e_a` is calculated from rel.
humidity as: 

.. math::

    e_s - e_a = (1-rH/100) * e_s(T)

The definition
is fromhttp://www.fao.org/docrep/X0490E/x0490e07.htm#concepts

Parameters:
-----------

T:  Air temperature in degC

rH:  Rel. humidity in %

:math:`e_s(T)` is calculated using cmf::atmosphere::vapor_pressure(double)
";

%feature("docstring")  cmf::atmosphere::watts_to_MJ "double
cmf::atmosphere::watts_to_MJ(double Watts)

Transforms an energy flux from W to MJ/day. ";


// File: namespacecmf_1_1convert__1__to__2.xml
%feature("docstring")  cmf::convert_1_to_2::convert_1_to_2 "def
cmf.convert_1_to_2.convert_1_to_2(stream_in=sys.stdin,
stream_out=sys.stdout, fn='') ";

%feature("docstring")  cmf::convert_1_to_2::get_pattern "def
cmf.convert_1_to_2.get_pattern() ";

%feature("docstring")  cmf::convert_1_to_2::regex_escape "def
cmf.convert_1_to_2.regex_escape(seq) ";

%feature("docstring")  cmf::convert_1_to_2::repl_or_warn "def
cmf.convert_1_to_2.repl_or_warn";


// File: namespacecmf_1_1describe.xml
%feature("docstring")  cmf::describe::describe "def
cmf.describe.describe(cmfobject, out=None)

Describes a cmf object in a file like object or returns the
description.  If no special description method for the class of
cmfobject is available, the function writes the string representation
:param cmfobject: any cmf object eg. project, Cell, timeseries :param
out: filelike object, if None function returns the description as
string :return: If out is None it returns the description as string,
else no return ";


// File: namespacecmf_1_1draw.xml


// File: namespacecmf_1_1draw_1_1cellmap.xml


// File: namespacecmf_1_1draw_1_1draw__misc.xml
%feature("docstring")  cmf::draw::draw_misc::bar_timeseries "def
cmf.draw.draw_misc.bar_timeseries(data, kwargs)

Makes a bar graph from a cmf.timeseries using pylab.bar :param data:
cmf.timeseries :param kwargs: Keyword arguments of pylab.bar :return:
as pylab.bar ";

%feature("docstring")  cmf::draw::draw_misc::plot_image "def
cmf.draw.draw_misc.plot_image(filename, kwargs)

Plots an image with an ESRI Worldfile as a map background. Uses
matplotlib.pylab.imshow :param filename: Filename of the image. :param
kwargs: Keyword arguments to imshow :return: Image from imshow ";

%feature("docstring")  cmf::draw::draw_misc::plot_timeseries "def
cmf.draw.draw_misc.plot_timeseries(data, style='-', kwargs)

Plots a cmf.timeseries as a line using pylab.plot :param data:
cmf.timeseries :param style: Style code for pylab.plot :param kwargs:
Keyword arguments for pylab.plot :return: matplotlib line object ";


// File: namespacecmf_1_1draw_1_1hillplot.xml


// File: namespacecmf_1_1draw_1_1shapemap.xml


// File: namespacecmf_1_1fit__retention__curve.xml
%feature("docstring")  cmf::fit_retention_curve::fit_bc "def
cmf.fit_retention_curve.fit_bc(pF, theta, count=1, verbose=False)

Fits the vanGenuchten Mualem retention curve into measured soilphysics
values :param pF: A sequence of pF values :param theta: A sequence of
the volumetric water content in l water/l soil at the respective pF
value :param count: Times to repeat the minimization to avoid local
minima :param verbose: If True: Print the values in between :return:
(Retentioncurve, RMSE) ";

%feature("docstring")  cmf::fit_retention_curve::fit_vgm "def
cmf.fit_retention_curve.fit_vgm(pF, theta, fit_m=False,
fit_theta_r=False, count=1, verbose=False)

Fits the vanGenuchten Mualem retention curve into measured soilphysics
values :param pF: A sequence of pF values :param theta: A sequence of
the volumetric water content in l water/l soil at the respective pF
value :param fit_m: If True, the retention curve is fitted with a
variable m (deprecated by vanGenuchten) :param fit_theta_r:  If True,
the retention curve is fitted using a theta_r (kind of deprecated by
vanGenuchten) :param count: Times to repeat the minimization to avoid
local minima :param verbose: If True: Print the values in between
:return: (Retentioncurve, RMSE) ";


// File: namespacecmf_1_1geometry.xml
%feature("docstring")  cmf::geometry::geocell::distance "double
cmf::geometry::distance(const point &p1, const point &p2) ";

%feature("docstring")  cmf::geometry::geocell::dot "double
cmf::geometry::dot(const point &p1, const point &p2) ";


// File: namespacecmf_1_1geometry_1_1geocell.xml
%feature("docstring")  cmf::geometry::geocell::add_geometry_property "def cmf.geometry.geocell.add_geometry_property()

Extends the Cell class with a geometry attribute :return: ";

%feature("docstring")  cmf::geometry::geocell::create_cell "def
cmf.geometry.geocell.create_cell(project, polygon, height, id=None,
with_surfacewater=True)

Creates a cell from a shapely polygon and stores the geometry in
cell.geometry  :param project: the cmf project of the cell :param
polygon: the shapely Polygon :param height: the height of the cell
:param id: the id of the cell, only set if not None :param
with_surfacewater: True, if a surfacewater storage will be created
:return: The new cell ";

%feature("docstring")  cmf::geometry::geocell::mesh_project "def
cmf.geometry.geocell.mesh_project(project, min_intersection=0,
verbose=False)

Get the topologcial information from the geometry This may take some
time :param project: The cmf project. The cells of the project need to
have geometry :param min_intersection: Minimum intersection length in
m :param verbose: Set True for report of action and additional
warnings :return: ";


// File: namespacecmf_1_1geometry_1_1qtree.xml


// File: namespacecmf_1_1jacobian.xml
%feature("docstring")  cmf::jacobian::connected_states "def
cmf.jacobian.connected_states(states)

Get a set of all connected nodes, indicated by the node_id's  :param
states: An iterable of states :return: a set of node_id tuples ";

%feature("docstring")  cmf::jacobian::connector_matrix "def
cmf.jacobian.connector_matrix(states, compression_factor=1)

Returns a matrix that shows the connectivity between the given states
:param states: A sequence of states to create the matrix :param
compression_factor: Large matrices can compressed with a factor.
:return: A symmetric 2d matrix with 1 for connected states and 0 for
unconnected states. Compressed matrices contain larger numbers for the
count of connection in the compressed field ";


// File: namespacecmf_1_1maps.xml
%feature("docstring")  cmf::maps::distance "def cmf.maps.distance(p1,
p2) ";


// File: namespacecmf_1_1math.xml
%feature("docstring")  cmf::math::root_finding::get_parallel_threads "int cmf::math::get_parallel_threads()

Returns the max number of threads used by OpenMP in parallel sections
of the code. ";

%feature("docstring")  cmf::math::root_finding::nash_sutcliffe "double cmf::math::nash_sutcliffe(const cmf::math::timeseries &model,
const cmf::math::timeseries &observation)

Calculates the Nash-Sutcliffe efficiency of a modeled timeseries in
comparison with an observed timeseries.

The Nash-Sutcliffe efficiancy is defined as: 

.. math::

     E = 1 -
    \\\\frac{\\\\sum_{t=1}^{T}(M_t - O_t)^2}{\\\\sum_{t=1}^{T}(O_t -
    \\\\overline{O})^2}

where  :math:`T` is the number of observation time
steps

:math:`M` is the timeseries of model results matchinig O

:math:`O` is the timeseries containing observations

:math:`\\\\overline{O}` is the arithmetic mean of observations ";

%feature("docstring")  cmf::math::root_finding::set_parallel_threads "int cmf::math::set_parallel_threads(int numthreads)

Set the number of threads used by OpenMP in parallel sections of the
code. ";

%feature("docstring")  cmf::math::root_finding::timespan "Time
cmf::math::timespan(long long ms) ";


// File: namespacecmf_1_1math_1_1root__finding.xml


// File: namespacecmf_1_1river.xml
%feature("docstring")  cmf::river::make_river_gap "double
cmf::river::make_river_gap(Reach::ptr root_reach)

Ensures that rivers have a monotone downward flow direction.

Reaches with a bottom higher than any upstream reach are lowered to
the minimum height of any (possibly distant) upstream reach. ";


// File: namespacecmf_1_1timetools.xml
%feature("docstring")  cmf::timetools::datetime_to_cmf "def
cmf.timetools.datetime_to_cmf(date)

Converts a python datetime to cmf.Time ";


// File: namespacecmf_1_1upslope.xml
%feature("docstring")  cmf::upslope::connections::cell2cellflux "cmf::math::num_array cmf::upslope::cell2cellflux(cmf::upslope::Cell
&source, cmf::upslope::Cell &target, cmf::math::Time t, bool
subsurface_only=false)

Calculates the flux in m3/day that flows from a source cell to a
target cell.

Parameters:
-----------

source:  the cell to investigate

target:  - the cell where the water is flowing

t:  Time of query

subsurface_only:  if true, fluxes from the surfacewater to the
surfacewater are ignored. Default is false

A num_array of size source.layer_count() containing all fluxes from
the layers of source to the layers of target. If subsurface_only is
false the array has the size source.layer_count()+1 and contains the
surfacewater flux in position 0 ";

%feature("docstring")  cmf::upslope::connections::cell_distance "double cmf::upslope::cell_distance(cmf::upslope::Cell &c1,
cmf::upslope::Cell &c2) ";

%feature("docstring")  cmf::upslope::connections::cell_flux_directions
"cmf::geometry::point_vector
cmf::upslope::cell_flux_directions(cmf::upslope::cells_ref cells,
cmf::math::Time) ";

%feature("docstring")  cmf::upslope::connections::cell_positions "cmf::geometry::point_vector
cmf::upslope::cell_positions(cmf::upslope::cells_ref cells) ";

%feature("docstring")
cmf::upslope::connections::connect_cells_with_flux "void
cmf::upslope::connect_cells_with_flux(cells_ref cells, const
cmf::upslope::CellConnector &connect, int start_at_layer=0)

Connects all adjacent cells with a flux connection, implementing the
CellConnector protocol. ";

%feature("docstring")  cmf::upslope::connections::fill_sinks "int
cmf::upslope::fill_sinks(cells_ref cells, double min_difference=0.001)

Fills the sinks in a network. Unstable! Might create an infinite loop.
";

%feature("docstring")  cmf::upslope::connections::find_cell "cmf::upslope::Cell* cmf::upslope::find_cell(cells_ref,
cmf::geometry::point p, double max_dist=1e20)

Finds the nearest cell for a position. ";

%feature("docstring")  cmf::upslope::connections::get_area "double
cmf::upslope::get_area(cells_ref cells) ";

%feature("docstring")  cmf::upslope::connections::get_boundary_cells "cell_vector cmf::upslope::get_boundary_cells(cells_ref cells)

Gets the outer cells of a network. Unstable! Might create an infinite
loop. ";

%feature("docstring")  cmf::upslope::connections::get_center "cmf::geometry::point cmf::upslope::get_center(cells_ref cells) ";

%feature("docstring")  cmf::upslope::connections::get_connections "cmf::water::connection_list cmf::upslope::get_connections(cells_ref
cells)

Gets all connections of a bunch of cells. ";

%feature("docstring")  cmf::upslope::connections::pF_to_waterhead "double cmf::upslope::pF_to_waterhead(double pF)

Converts a pF value to a height of a water column in m. ";

%feature("docstring")
cmf::upslope::connections::pressure_to_waterhead "double
cmf::upslope::pressure_to_waterhead(double Pressure)

Converts a pressure in Pa to a length of a water column in m. ";

%feature("docstring")  cmf::upslope::connections::waterhead_to_pF "double cmf::upslope::waterhead_to_pF(double waterhead)

Converts a height of a water column to a pF value. ";

%feature("docstring")
cmf::upslope::connections::waterhead_to_pressure "double
cmf::upslope::waterhead_to_pressure(double waterhead)

Converts a height of a water column in m to a pressure in Pa. ";


// File: namespacecmf_1_1upslope_1_1connections.xml
%feature("docstring")  cmf::upslope::connections::snowfraction "real
cmf::upslope::connections::snowfraction(real T)

A function to calculate the snow fraction of the precipitation
according to the air temperature.

Returns 0.0 for T>+1 degC, 1.0 for T<-1degC and interpolates linear
between. Values between 0 and 1 can be interpreted as sleet ";


// File: namespacecmf_1_1upslope_1_1_e_t.xml
%feature("docstring")  cmf::upslope::ET::PenmanMonteith "real
cmf::upslope::ET::PenmanMonteith(real Rn, real ra, real rs, real T,
real vap_press_deficit)

Returns the potential ET after Penman-Monteith using some
simplifications for a given Radiation balance, aerodynamic and surface
resistances, and a vapor pressure deficit.



.. math::

     ET = \\\\frac{\\\\Delta R_n}{\\\\lambda \\\\Delta + \\\\gamma +
    \\\\gamma \\\\frac{r_s}{r_a}} + \\\\frac{c_p\\\\rho_a}{\\\\Delta +
    \\\\gamma + \\\\gamma \\\\frac{r_s}{r_a}} \\\\frac{e_s - e_a}{r_a}
    

where  :math:`ET` is the evapotranspiration in :math:`\\\\frac{kg}{m^2 day}\\\\approx \\\\frac{mm}{day}`

:math:`\\\\Delta \\\\left[\\\\frac{kPa}{k}\\\\right]= 4098\\\\ 0.618 \\\\exp\\\\left(\\\\frac{17.27 T}{T+237.3}\\\\right (T+237.3)^{-2}`
is the slope of vapor pressure

:math:`R_n \\\\left[\\\\frac{MJ}{m^2 day}\\\\right]` is the radiation
balance

:math:`r_s \\\\left[\\\\frac s m\\\\right]` is the surface resistance

:math:`r_a \\\\left[\\\\frac s m\\\\right]` is the aerodynamic resistance

:math:`\\\\gamma = 0.067 \\\\left[\\\\frac{kPa}{k}\\\\right]` is the
psychrometer constant

:math:`e_s - e_a \\\\left[kPa\\\\right]` is the vapor pressure deficit

Parameters:
-----------

Rn:  Radiation balance in :math:`\\\\frac{MJ}{m^2 day}`

ra:  Aerodynamic resistance in :math:`\\\\frac s m`

rs:  Surface resistance in :math:`\\\\frac s m`, is 0 for free water

T:  Actual Temperature in :math:`^\\\\circ C`

vap_press_deficit:  Deficit of vapor pressure :math:`kPa` ";

%feature("docstring")  cmf::upslope::ET::PenmanMonteith "real
cmf::upslope::ET::PenmanMonteith(cmf::atmosphere::Weather A, const
cmf::upslope::vegetation::Vegetation &veg, double h)

Returns the potential ET after Penman-Monteith using some
simplifications for a weather and a vegetation object.

aerodynamic and surface resistances, and a vapor pressure deficit

Parameters:
-----------

A:  Current weather

veg:  Vegetation data

h:  Height above sea level in m (for air pressure estimation) ";


// File: namespacecmf_1_1upslope_1_1vegetation.xml


// File: namespacecmf_1_1water.xml
%feature("docstring")  cmf::water::can_set_flux "bool
cmf::water::can_set_flux(flux_node::ptr source, flux_node::ptr target)

Checks if a constant flux between two nodes can be set.

Returns true if the nodes are connected by an
external_control_connection ";

%feature("docstring")  cmf::water::count_node_references "int
cmf::water::count_node_references(flux_node::ptr node) ";

%feature("docstring")  cmf::water::get_higher_node "flux_node::ptr
cmf::water::get_higher_node(flux_node::ptr node1, flux_node::ptr
node2) ";

%feature("docstring")  cmf::water::get_lower_node "flux_node::ptr
cmf::water::get_lower_node(flux_node::ptr node1, flux_node::ptr node2)
";

%feature("docstring")  cmf::water::replace_node "int
cmf::water::replace_node(cmf::water::flux_node::ptr oldnode,
cmf::water::flux_node::ptr newnode) ";

%feature("docstring")  cmf::water::set_flux "void
cmf::water::set_flux(flux_node::ptr source, flux_node::ptr target,
real flux_value)

Sets a constant flux between two nodes, if an
external_control_connection exists. ";

%feature("docstring")  cmf::water::system_bridge "SystemBridge::ptr
cmf::water::system_bridge(cmf::project &p, flux_node::ptr upper,
flux_node::ptr lower)

Creates a SystemBridge object.

This is an advanced feature for tuning of the calculation time.

A SystemBridge can be used to replace an existing connection between
nodes. After installation, the two nodes can more safely be added to
different integrator systems. One node (called upper) is connected
with the system bridge with the connection formerly connecting the
nodes, the second node (called lower) is connected to the system
bridge with as a Neumann boundary condition. The flux equals the
average flux of the connection upper <-> SystemBridge. Therefore, the
system bridge must become an integratable of the integrator system the
upper node belongs to. Use as an upper system (system upper node is
belonging to) the faster reacting system. For the connection between
upper and SystemBridge, the SystemBridge reacts as an Dirichlet
boundary condition, providing the potential of the lower node. ";


// File: namespacecmf__core.xml


// File: namespacestd.xml


// File: namespacetextwrap.xml


// File: ____init_____8py.xml


// File: draw_2____init_____8py.xml


// File: geometry_2____init_____8py.xml


// File: meteorology_8h.xml


// File: precipitation_8h.xml


// File: _weather_8h.xml


// File: geometry_8h.xml


// File: bdf2_8h.xml


// File: cvode_8h.xml


// File: explicit__euler_8h.xml


// File: implicit__euler_8h.xml


// File: integrator_8h.xml


// File: multiintegrator_8h.xml


// File: _r_k_fintegrator_8h.xml


// File: _water_solute_integrator_8h.xml


// File: irregular__timeseries_8h.xml


// File: num__array_8h.xml


// File: odesystem_8h.xml


// File: real_8h.xml
%feature("docstring")  cmf::boltzmann "real boltzmann(real x, real
x_half, real tau)

The boltzmann function, used in cmf at several places where a s-shaped
curve is needed.



.. math::

    f(x,x_{1/2},\\\\tau)=\\\\frac{1}{1+e^{-\\\\frac{x-x_{1/2}}{tau}}}

";

%feature("docstring")  cmf::geo_mean "real geo_mean(real a, real b)
";

%feature("docstring")  cmf::harmonic_mean "real harmonic_mean(real a,
real b) ";

%feature("docstring")  cmf::maximum "real maximum(real a, real b) ";

%feature("docstring")  cmf::mean "real mean(real a, real b) ";

%feature("docstring")  cmf::minimum "real minimum(real a, real b)

Returns the minimum of two values. ";

%feature("docstring")  cmf::minmax "real minmax(real x, real min,
real max) ";

%feature("docstring")  cmf::piecewise_linear "real
piecewise_linear(real x, real xmin, real xmax, real ymin=0, real
ymax=1) ";

%feature("docstring")  cmf::sign "real sign(real x) ";

%feature("docstring")  cmf::square "real square(real x) ";


// File: root__finding_8h.xml


// File: sparse__struct_8h.xml


// File: spline_8h.xml


// File: statevariable_8h.xml


// File: time_8h.xml


// File: timeseries_8h.xml


// File: project_8h.xml


// File: cross__section__reach_8h.xml


// File: _manning_connection_8h.xml


// File: _open_water_storage_8h.xml


// File: _reach_8h.xml


// File: _reach_type_8h.xml


// File: algorithm_8h.xml


// File: cell_8h.xml


// File: cell__vector_8h.xml


// File: conductable_8h.xml


// File: _atmospheric_fluxes_8h.xml


// File: infiltration_8h.xml


// File: _percolation_8h.xml


// File: subsurfacefluxes_8h.xml


// File: surfacefluxes_8h.xml


// File: groundwater_8h.xml


// File: layer__list_8h.xml


// File: macropore_8h.xml


// File: _retention_curve_8h.xml


// File: _soil_layer_8h.xml


// File: surfacewater_8h.xml


// File: _topology_8h.xml


// File: _e_t_8h.xml


// File: _shuttleworth_wallace_8h.xml


// File: _struct_vegetation_8h.xml


// File: waterstress_8h.xml


// File: adsorption_8h.xml


// File: boundary__condition_8h.xml


// File: collections_8h.xml


// File: flux__connection_8h.xml


// File: flux__node_8h.xml


// File: simple__connections_8h.xml


// File: _solute_8h.xml


// File: _solute_storage_8h.xml


// File: system__bridge_8h.xml


// File: _water_storage_8h.xml


// File: convert__1__to__2_8py.xml


// File: describe_8py.xml


// File: cellmap_8py.xml


// File: draw__misc_8py.xml


// File: hillplot_8py.xml


// File: shapemap_8py.xml


// File: fit__retention__curve_8py.xml


// File: geocell_8py.xml


// File: qtree_8py.xml


// File: jacobian_8py.xml


// File: maps_8py.xml


// File: timetools_8py.xml


// File: _cmf_introduction_8md.xml


// File: _cmf_recommended_software_environment_8md.xml


// File: contrib_cpp_8md.xml


// File: contrib_docs_8md.xml


// File: contrib_py_8md.xml


// File: overview_8md.xml


// File: ems-2011-paper_8md.xml


// File: _finite_volume_method_8md.xml


// File: index_8md.xml


// File: _cmf_install_8md.xml


// File: _cmf_install_hpc_8md.xml


// File: _cmf_install_ubuntu_8md.xml


// File: _cmf_install_windows_8md.xml


// File: _publication_list_8md.xml


// File: atmosphere_8md.xml


// File: _cmf_tut_e_t_8md.xml


// File: _cmf_tut_intercept_8md.xml


// File: _cmf_tut_meteostation_8md.xml


// File: _cmf_tut_shuttleworth_wallace_8md.xml


// File: _cmf_tut_snow_8md.xml


// File: _cmf_tut_test_data_8md.xml


// File: _cmf_tut_start_8md.xml


// File: _cmf__lumped__simple_8md.xml


// File: _c_m_f__lumped__without__spotpy_8md.xml


// File: _cmf_tut_abstract_fluxes_8md.xml


// File: _cmf_tut_kinematic_wave_8md.xml


// File: conceptual_8md.xml


// File: semi__distributed_8md.xml


// File: _simple___infiltration_8md.xml


// File: _cmf_tut_boundary_8md.xml


// File: _cmf_tut_cell_8md.xml


// File: _cmf_tut_first_model_8md.xml


// File: _cmf_tut_fluxes_8md.xml


// File: _cmf_tut_need_to_know_8md.xml


// File: _cmf_tut_project_8md.xml


// File: _cmf_tut_solver_8md.xml


// File: _cmf_tut_space_time_8md.xml


// File: cmf_tut_structure_8md.xml


// File: _cmf_tut_units_8md.xml


// File: gettingstarted_8md.xml


// File: _install_check_8md.xml


// File: _descriptor_8md.xml


// File: _cmf_tut1d_8md.xml


// File: _cmf_tut2d_8md.xml


// File: _cmf_tut3d_8md.xml


// File: _cmf_tut_boundary2_8md.xml


// File: _cmf_tut_channel_8md.xml


// File: _cmf_tut_darcian_lateral_flow_8md.xml


// File: _cmf_tut_e_tphys_8md.xml


// File: _cmf_tut_retentioncurve_8md.xml


// File: _cmf_tut_solute_transport1_d_8md.xml


// File: _cmf_tut_surface_runoff_8md.xml


// File: _cmf_tut_volume_height_8md.xml


// File: physical_8md.xml


// File: group__boundary.xml


// File: group__surfacefluxes.xml


// File: group___e_t.xml


// File: group__connections.xml


// File: group__infiltration.xml


// File: group___macro_pore.xml


// File: group__meteo.xml


// File: group__perc.xml


// File: group__storages.xml


// File: group__manning.xml


// File: group__nodes.xml


// File: group__latflux.xml


// File: cmf_introduction.xml


// File: cmf_recommended_software_environment.xml


// File: contrib_cpp.xml


// File: contrib_docs.xml


// File: contrib_py.xml


// File: contrib_overview.xml


// File: contrib_issues.xml


// File: ems2011.xml


// File: finite_volume_method.xml


// File: cmf_install.xml


// File: cmf_install_hpc.xml


// File: cmf_install_ubuntu.xml


// File: cmf_install_windows.xml


// File: publication_list.xml


// File: atmosphere.xml


// File: cmf_tut_e_t.xml


// File: cmf_tut_intercept.xml


// File: cmf_tut_meteostation.xml


// File: cmf_tut_shuttleworth_wallace.xml


// File: cmf_tut_snow.xml


// File: cmf_tut_test_data.xml


// File: tutorial.xml


// File: cmf_lumped_simple.xml


// File: cmf_lumped_without_spotpy.xml


// File: cmf_tut_abstract_fluxes.xml


// File: cmf_tut_kinematic_wave.xml


// File: conceptual.xml


// File: semi_distributed.xml


// File: simple_infiltration.xml


// File: cmf_tut_boundary.xml


// File: cmf_tut_cell.xml


// File: cmf_tut_first_model.xml


// File: cmf_tut_fluxes.xml


// File: cmf_tut_need_to_know.xml


// File: cmf_tut_project.xml


// File: cmf_tut_solver.xml


// File: cmf_tut_space_time.xml


// File: cmf_tut_structure.xml


// File: cmf_tut_units.xml


// File: gettingstarted.xml


// File: install_check.xml


// File: descriptor.xml


// File: cmf_tut1d.xml


// File: cmf_tut2d.xml


// File: cmf_tut3d.xml


// File: cmf_tut_boundary2.xml


// File: cmf_tut_channel.xml


// File: cmf_tut_darcian_lateral_flow.xml


// File: cmf_tut_e_tphys.xml


// File: cmf_tut_retentioncurve.xml


// File: cmf_tut_solute_transport1_d.xml


// File: cmf_tut_surface_runoff.xml


// File: cmf_tut_volume_height.xml


// File: physical.xml


// File: todo.xml


// File: deprecated.xml


// File: dir_a32c301defa8fda793cb40af11415647.xml


// File: dir_58e40ea5f41f41b625255f0e93c0210e.xml


// File: dir_899d2e127fb6f8de6dda9777a2d11fc4.xml


// File: dir_276c0bb4402aea34a5542f6741d7a1ee.xml


// File: dir_e99589850f294dbf4b725494ab1c642e.xml


// File: dir_12788de71013d9dcf17a564ce1b123ce.xml


// File: dir_6e764f1be992d70f16adadc29c086796.xml


// File: dir_fb2b4d745793dfd5bb69790f90a5a545.xml


// File: dir_707fbb85f5f5a03168ae42ef8ba0d65d.xml


// File: dir_73a48ad96f7ed71bd28db957498e0435.xml


// File: dir_8d4c975dc4db6fbc37636ae91790b381.xml


// File: dir_1e82fb26092adfe48cd921c6b8a3056a.xml


// File: dir_77005acaedd4c0a991f05071f95b4942.xml


// File: indexpage.xml


