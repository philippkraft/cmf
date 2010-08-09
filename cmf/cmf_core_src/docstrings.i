
// File: index.xml

// File: classcmf_1_1atmosphere_1_1aerodynamic__resistance.xml
%feature("docstring") cmf::atmosphere::aerodynamic_resistance "

Abstract class. Child classes can be used to calculate aerodynamic
resistances against turbulent heat fluxes.

C++ includes: meteorology.h ";

%feature("docstring")
cmf::atmosphere::aerodynamic_resistance::get_aerodynamic_resistance "virtual void get_aerodynamic_resistance(double &r_ag, double &r_ac,
cmf::math::Time t) const =0

aerodynamic resistance from ground to atmosphere (r_ag) and from
canopy to atmosphere (r_ac) ";


// File: classcmf_1_1math_1_1_b_d_f2.xml
%feature("docstring") cmf::math::BDF2 "

An order 2 BDF-Method with fixed-point iteration and variable step
size

Recommended integrator for CMF (so far)

Derived from Roussel C. and Roussel M. (2003) \"Generic Object-
Oriented Differential Equation Integrators\", C/C++ User Journal, Nov.
2003,http://www.ddj.com/cpp/184401724?pgno=8 and

Eckert S., Baaser H., Gross D. and Scherf O. (2004) \"A BDF2
integration method with step size control for elasto-plasticity\",
Computational Mechanics 34, 377 - 386, DOI: 10.1007/s00466-004-0581-1

Most important function: Integrate

C++ includes: bdf2.h ";

/*  Gear functions: The following functions evaluate the Gear formulas
of specified order, loading the results into compareStates.  */

/*

*/

/*  Internal data storages  */

/*

*/

/*  The state variables to integrate  */

/*

*/

%feature("docstring")  cmf::math::BDF2::CopyStates "void
CopyStates(num_array &destination) const

Copies the states to a numeric vector using use_OpenMP. ";

%feature("docstring")  cmf::math::BDF2::CopyStates "void
CopyStates(real *destination) const ";

%feature("docstring")  cmf::math::BDF2::SetStates "void
SetStates(const num_array &newStates)

Copies the new states to the actual states. ";

%feature("docstring")  cmf::math::BDF2::SetStates "void
SetStates(real *newStates) ";

%feature("docstring")  cmf::math::BDF2::CopyDerivs "void
CopyDerivs(Time time, num_array &destination, real factor=1) const

Copies the derivatives at time step \"time\" to a numeric vector using
use_OpenMP

Parameters:
-----------

time:   Time at which the derivatives should be calculated

destination:  Vector to be overwritten by the results

factor:  A factor that is multiplied to the derivate (e.g. unit
conversion or integration length) ";

%feature("docstring")  cmf::math::BDF2::CopyDerivs "void
CopyDerivs(Time time, real *destination, real factor=1) const

Copies the derivatives at time step \"time\" to an preallocated c
array

Parameters:
-----------

time:   Time at which the derivatives should be calculated

destination:  Allocated c array

factor:  A factor that is multiplied to the derivate (e.g. unit
conversion or integration length) ";

%feature("docstring")  cmf::math::BDF2::GetStates "num_array
GetStates() const

Returns the states in a numeric vector using :CopyStates, but is
slower because of additional memory allocation. ";

%feature("docstring")  cmf::math::BDF2::GetDerivs "num_array
GetDerivs(Time time) const

Returns the derivatives at time step \"time\" in a numeric vector
using :CopyDerivs, but is slower because of additional memory
allocation. ";

%feature("docstring")  cmf::math::BDF2::AddValuesToStates "void
AddValuesToStates(const num_array &operands) ";

%feature("docstring")  cmf::math::BDF2::AddState "virtual void
AddState(cmf::math::StateVariable::ptr state) ";

/*  model time  */

/*

*/

%feature("docstring")  cmf::math::BDF2::ModelTime "cmf::math::Time
ModelTime() const

Returns the current model time. ";

%feature("docstring")  cmf::math::BDF2::ModelTime "void
ModelTime(cmf::math::Time val)

Sets the current model time. ";

%feature("docstring")  cmf::math::BDF2::TimeStep "cmf::math::Time
TimeStep() const

Returns the last time step. ";

%feature("docstring")  cmf::math::BDF2::NextTimeStep "cmf::math::Time
NextTimeStep() const

Returns the next time step width. ";

%feature("docstring")  cmf::math::BDF2::NextTimeStep "void
NextTimeStep(cmf::math::Time val)

Sets the next time step width. ";

%feature("docstring")  cmf::math::BDF2::MinTimestep "const
cmf::math::Time MinTimestep() const

Returns The minimal allowed time step length. ";

%feature("docstring")  cmf::math::BDF2::Iterations "int Iterations()
const ";

%feature("docstring")  cmf::math::BDF2::ResetIterations "void
ResetIterations() ";

%feature("docstring")  cmf::math::BDF2::Reset "virtual void Reset()
";

/*  Constructors and Destructors  */

/*

*/

/*  Integrate  */

/*

*/

%feature("docstring")  cmf::math::BDF2::Integrate "int
Integrate(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::BDF2::IntegrateUntil "void
IntegrateUntil(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::BDF2::IntegrateUntil "void
IntegrateUntil(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables until MaxTime. ";

/*  Accuracy parameters  */

/*

*/

%feature("docstring")  cmf::math::BDF2::AddStatesFromOwner "void
AddStatesFromOwner(cmf::math::StateVariableOwner &stateOwner) ";

%feature("docstring")  cmf::math::BDF2::get_error_position "int
get_error_position() const ";

%feature("docstring")  cmf::math::BDF2::BDF2 "BDF2(real epsilon=1e-9,
cmf::math::Time tStepMin=Time::Milliseconds(10))

Constructs a new Gears_var_Step

Parameters:
-----------

epsilon:  relative error tolerance per time step (default=1e-9)

tStepMin:  minimum time step (default=10s) ";

%feature("docstring")  cmf::math::BDF2::BDF2 "BDF2(cmf::math::StateVariableOwner &states, real epsilon=1e-9,
cmf::math::Time tStepMin=Time::Milliseconds(10)) ";

%feature("docstring")  cmf::math::BDF2::BDF2 "BDF2(const Integrator
&templ) ";

%feature("docstring")  cmf::math::BDF2::Copy "virtual Integrator*
Copy() const

Returns a new Integrator, based on this (without the state variables),
e.g. same type, epsilon, model time etc. ";

%feature("docstring")  cmf::math::BDF2::Integrate "int
Integrate(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables

if $ t_{max}-t_{now}\\\\leq 2h_n $ then $ h_{n+1}= t_{max}-t_{now} $
else $ h_{n+1}= 2h_n $

Uses pastStatesArray to store $ y_n,y_{n-1} $

if more than one step is taken start iterating:  $
y_{n+1}^i=\\\\frac{1}{1+2\\\\rho }\\\\left( (\\\\rho
+1)^{2}y_{n}-\\\\rho ^{2}y_{n-1}+(\\\\rho
+1)h\\\\frac{dy}{dt}(y_{n+1}^{i-1},t_{n+1} \\\\right), y_{n+1}^0=y_n $
with $ \\\\rho = \\\\frac{h_{n+1}}{h_n} $

until $ y_{n+1}^{i} - y_{n+1}^{i-1} \\\\leq \\\\ \\\\|\\\\epsilon +
\\\\epsilon y_{n+1}\\\\|_{\\\\infty} $

If too many iterations are needed, or the error is rising, repeat
iteration with $ h_{n+1} = \\\\frac{h_{n+1}}{2} $

Parameters:
-----------

MaxTime:  To stop the model (if running in a model framework) at time
steps of value exchange e.g. full hours, the next value exchange time
can be given

TimeStep:  Takes the proposed timestep, ignored by this solver ";

%feature("docstring")  cmf::math::BDF2::size "int size() const

returns the number of state variables ";

%feature("docstring")  cmf::math::BDF2::get_state "real get_state(int
position) const

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::BDF2::set_state "void set_state(int
position, real newState)

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::BDF2::get_states "state_queue
get_states()

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";


// File: classcmf_1_1upslope_1_1_brooks_corey_retention_curve.xml
%feature("docstring") cmf::upslope::BrooksCoreyRetentionCurve "

Provides the use of the Brooks-Corey retention curve

\\\\begin{eqnarray*} /// W(\\\\theta) &=& \\\\frac{\\\\theta -
\\\\theta_r}{\\\\theta_s - \\\\theta_r} \\\\\\\\ /// K(W) &=& K_{sat}
W^{2+3b} \\\\\\\\ /// \\\\Psi(W) &=& \\\\Psi_X
\\\\left(\\\\frac{W}{W_X}\\\\right)^{-b} \\\\\\\\ /// W(\\\\Psi) &=&
{\\\\left( \\\\frac{\\\\Psi_X}{\\\\Psi}\\\\right)
}^{\\\\frac{1}{b}}\\\\ W_X /// \\\\end{eqnarray*} where:  $K$ is the
conductivity in $\\\\frac m {day}$

$W$ is the wetness (Volume of soil water per volume of pores)

$b$ is the shape of the retention curve (usually between 4 (sand) and
14 (clay))

$\\\\Psi(W)$ is the matric potential in $m H_2O$ at wetness W

$\\\\Psi_X$ is a matric potential at a known wetness in $m H_2O$

$W_X$ is the wetness with a known matric potential for dynamic changes
with depth, exponential decays of porosity and saturated conductivity
are used The decay function is: $ v(d)=v(0) (1+a)^{-d} $, where v is
the value ( $ K_{sat},\\\\Phi$), d is the depth in m and a is the
fractional decay per m. E.g. 0.1 means the value has in 1 m depth 90%
of the value at the surface

C++ includes: RetentionCurve.h ";

%feature("docstring")  cmf::upslope::BrooksCoreyRetentionCurve::K "virtual real K(real wetness) const

Returns the conductivity in m/day at a certain depth and water
content. ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::Porosity "real Porosity(real
depth) const

real (Porosity) ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::SetPorosity "void
SetPorosity(real porosity, real porosity_decay=0)

Sets the porosity (Volume of pores per volume of soil) and the
exponential porosity decline with depth. ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::VoidVolume "real
VoidVolume(real upperDepth, real lowerDepth, real Area) const

Returns the pore volume in a region of a soil column

If there is a porosity decay, the void volume is the integral of
porosity over depth times area ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::FillHeight "real
FillHeight(real lowerDepth, real Area, real Volume) const

Returns the thickness of a soil column with a certain pore volume. ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::Transmissivity "real
Transmissivity(real upperDepth, real lowerDepth, real theta) const ";

%feature("docstring")  cmf::upslope::BrooksCoreyRetentionCurve::get_b
"real get_b() const

Retention curve shape parameter. ";

%feature("docstring")  cmf::upslope::BrooksCoreyRetentionCurve::set_b
"void set_b(real new_b) ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::MatricPotential "real
MatricPotential(real wetness) const

Returns the suction pressure in m, use conversion functions
waterhead_to_pressure and waterhead_to_pF fro conversions. ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::Wetness "real Wetness(real
suction) const

returns the wetness (volumetric water content per pore space) at a
given suction pressure ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::BrooksCoreyRetentionCurve "BrooksCoreyRetentionCurve(real ksat=15, real porosity=0.5, real _b=5,
real theta_x=0.2, real psi_x=pF_to_waterhead(2.5), real
porosity_decay=0)

Creates a brooks corey retention curve

Parameters:
-----------

ksat:  Saturated conductivity $\\\\frac{m}{day}$

porosity:   $\\\\frac {m^3 Pores}{m^3 Soil}$

_b:  Shape of the retention curve (if you do not know how to
parameterize this, take a look at the other constructor)

theta_x:   $\\\\theta_X$ Water content at a specific suction pressure

psi_x:  Suction pressure for $\\\\theta_X$ in m water column, use the
conversion functions pF_to_waterhead, pressure_to_waterhead to convert
pressure in to waterhead height (default pF=2.5)

porosity_decay:  Relative decay of porosity with depth, e.g. 0.1 means
conductivity gets 10% smaller per meter ";

%feature("docstring")  cmf::upslope::BrooksCoreyRetentionCurve::copy "virtual BrooksCoreyRetentionCurve* copy() const ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::Wetness_eff "virtual real
Wetness_eff(real wetness, real pF_r=4.2) const

Returns the effective wetness, using a residual pF value \\\\[w_{eff}
=
\\\\frac{w_{act}-w\\\\left(pF_r\\\\right)}{1-w\\\\left(pF_r\\\\right)}\\\\]
";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::Wetness_pF "real
Wetness_pF(real pF) const

returns the volumetric water content at a given pF value ";


// File: classcmf_1_1upslope_1_1_e_t_1_1canopy__evaporation.xml
%feature("docstring") cmf::upslope::ET::canopy_evaporation "

Flux connection using a canopy_evaporation_method.

C++ includes: ET.h ";

%feature("docstring")
cmf::upslope::ET::canopy_evaporation::canopy_evaporation "canopy_evaporation(cmf::water::WaterStorage::ptr source,
cmf::water::flux_node::ptr ET_target, std::tr1::shared_ptr<
canopy_evaporation_method > _method, std::string method_name) ";

%feature("docstring")  cmf::upslope::ET::canopy_evaporation::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::ET::canopy_evaporation::left_node
"flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::ET::canopy_evaporation::right_node "flux_node::ptr
right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::upslope::ET::canopy_evaporation::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::ET::canopy_evaporation::get_target "flux_node::ptr
get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::ET::canopy_evaporation::get_target "flux_node::ptr
get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::ET::canopy_evaporation::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::ET::canopy_evaporation::conc "real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::upslope::ET::canopy_evaporation::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::ET::canopy_evaporation::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")  cmf::upslope::ET::canopy_evaporation::to_string
"std::string to_string() const ";

%feature("docstring")
cmf::upslope::ET::canopy_evaporation::short_string "std::string
short_string() const ";


// File: classcmf_1_1upslope_1_1_e_t_1_1canopy__evaporation__method.xml
%feature("docstring") cmf::upslope::ET::canopy_evaporation_method "

Abstract class. Child classes are defining a method for intercepted
canopy water evaporation calculation.

C++ includes: ET.h ";

%feature("docstring")
cmf::upslope::ET::canopy_evaporation_method::evap_from_canopy "virtual double evap_from_canopy(cmf::water::WaterStorage::ptr,
cmf::math::Time)=0 ";


// File: classcmf_1_1upslope_1_1connections_1_1_canopy_overflow.xml
%feature("docstring") cmf::upslope::connections::CanopyOverflow "

Calculates the overflow of a canopy storage

C++ includes: surfacefluxes.h ";

%feature("docstring")
cmf::upslope::connections::CanopyOverflow::CanopyOverflow "CanopyOverflow(cmf::water::WaterStorage::ptr Canopy,
cmf::water::flux_node::ptr target, cmf::upslope::Cell &cell) ";

%feature("docstring")  cmf::upslope::connections::CanopyOverflow::q "real q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::CanopyOverflow::left_node "flux_node::ptr
left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::connections::CanopyOverflow::right_node "flux_node::ptr
right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::CanopyOverflow::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

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
cmf::upslope::connections::CanopyOverflow::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::connections::CanopyOverflow::conc
"real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::CanopyOverflow::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::connections::CanopyOverflow::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")
cmf::upslope::connections::CanopyOverflow::to_string "std::string
to_string() const ";

%feature("docstring")
cmf::upslope::connections::CanopyOverflow::short_string "std::string
short_string() const ";


// File: classcmf_1_1upslope_1_1_e_t_1_1_canopy_storage_evaporation.xml
%feature("docstring") cmf::upslope::ET::CanopyStorageEvaporation "

Calculates the evaporation from a canopy storage

C++ includes: ET.h ";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::CanopyStorageEvaporation "CanopyStorageEvaporation(cmf::water::WaterStorage::ptr CanopyStorage,
cmf::water::flux_node::ptr ET_target, cmf::upslope::Cell &cell) ";

%feature("docstring")  cmf::upslope::ET::CanopyStorageEvaporation::q "real q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::left_node "flux_node::ptr
left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::to_string "std::string
to_string() const ";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::short_string "std::string
short_string() const ";


// File: classcmf_1_1upslope_1_1_cell.xml
%feature("docstring") cmf::upslope::Cell "

This class is the basic landscape object. It is the owner of water
storages, and the upper and lower boundary conditions of the system
(rainfall, atmospheric vapor, deep groundwater)

C++ includes: cell.h ";

/*  Flux nodes of the cell  */

/*

*/

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

%feature("docstring")  cmf::upslope::Cell::get_evaporation "cmf::water::flux_node::ptr get_evaporation()

Returns the end point of all evaporation of this cell. ";

%feature("docstring")  cmf::upslope::Cell::get_transpiration "cmf::water::flux_node::ptr get_transpiration()

Returns the end point of all transpiration of this cell. ";

%feature("docstring")  cmf::upslope::Cell::get_surfacewater "cmf::water::flux_node::ptr get_surfacewater()

returns the surface water of this cell ";

%feature("docstring")  cmf::upslope::Cell::surfacewater_as_storage "void surfacewater_as_storage() ";

%feature("docstring")  cmf::upslope::Cell::add_storage "cmf::water::WaterStorage::ptr add_storage(std::string Name, char
storage_role='N', bool isopenwater=false) ";

%feature("docstring")  cmf::upslope::Cell::remove_storage "void
remove_storage(cmf::water::WaterStorage &storage) ";

%feature("docstring")  cmf::upslope::Cell::storage_count "int
storage_count() const ";

%feature("docstring")  cmf::upslope::Cell::get_storage "cmf::water::WaterStorage::ptr get_storage(int index) const ";

%feature("docstring")  cmf::upslope::Cell::get_canopy "cmf::water::WaterStorage::ptr get_canopy() const ";

%feature("docstring")  cmf::upslope::Cell::get_snow "cmf::water::WaterStorage::ptr get_snow() const ";

%feature("docstring")  cmf::upslope::Cell::snow_coverage "real
snow_coverage() const ";

%feature("docstring")  cmf::upslope::Cell::albedo "real albedo()
const ";

%feature("docstring")  cmf::upslope::Cell::surface_water_coverage "real surface_water_coverage() const ";

%feature("docstring")  cmf::upslope::Cell::heat_flux "real
heat_flux(cmf::math::Time t) const

Calculates the surface heat balance

Parameters:
-----------

t:  Time step

Tground:  Ground temperature, default is Tground of weather (usually
Tground = Tair) ";

%feature("docstring")  cmf::upslope::Cell::has_wet_leaves "bool
has_wet_leaves() const ";

%feature("docstring")  cmf::upslope::Cell::has_surface_water "bool
has_surface_water() const ";

%feature("docstring")  cmf::upslope::Cell::get_project "const
cmf::project& get_project() const ";

%feature("docstring")  cmf::upslope::Cell::get_weather "cmf::atmosphere::Weather get_weather(cmf::math::Time t) const ";

/*  Location  */

/*

*/

%feature("docstring")  cmf::upslope::Cell::get_topology "cmf::upslope::Topology& get_topology() ";

%feature("docstring")  cmf::upslope::Cell::get_position "cmf::geometry::point get_position() const

Returns the location of the cell. ";

%feature("docstring")  cmf::upslope::Cell::get_area "double
get_area() const

Returns the area of the cell. ";

/*  Layers  */

/*

*/

%feature("docstring")  cmf::upslope::Cell::layer_count "int
layer_count() const ";

%feature("docstring")  cmf::upslope::Cell::get_layer "cmf::upslope::SoilLayer::ptr get_layer(int ndx) const ";

%feature("docstring")  cmf::upslope::Cell::get_layers "const
layer_list& get_layers() const ";

%feature("docstring")  cmf::upslope::Cell::add_layer "void
add_layer(real lowerboundary, const cmf::upslope::RetentionCurve
&r_curve, real saturateddepth=10) ";

%feature("docstring")  cmf::upslope::Cell::remove_last_layer "void
remove_last_layer() ";

%feature("docstring")  cmf::upslope::Cell::remove_layers "void
remove_layers() ";

%feature("docstring")  cmf::upslope::Cell::~Cell "virtual ~Cell() ";

/*  Saturation  */

/*

*/

%feature("docstring")  cmf::upslope::Cell::InvalidateSatDepth "void
InvalidateSatDepth() const

Marks the saturated depth as unvalid. ";

%feature("docstring")  cmf::upslope::Cell::get_saturated_depth "real
get_saturated_depth() const ";

%feature("docstring")  cmf::upslope::Cell::set_saturated_depth "void
set_saturated_depth(real depth) ";

%feature("docstring")  cmf::upslope::Cell::Cell "Cell(double x,
double y, double z, double area, cmf::project &_project) ";

%feature("docstring")  cmf::upslope::Cell::to_string "std::string
to_string() ";

%feature("docstring")  cmf::upslope::Cell::get_states "cmf::math::state_queue get_states()

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";

%feature("docstring")  cmf::upslope::Cell::set_position "virtual void
set_position(cmf::geometry::point p)

Sets the location. If not implemented by the child class, an exception
is thrown. ";

%feature("docstring")  cmf::upslope::Cell::get_distance_to "double
get_distance_to(const Locatable &cmp)

Returns the distance between two locatable objects. ";

%feature("docstring")  cmf::upslope::Cell::get_direction_to "cmf::geometry::point get_direction_to(const Locatable &cmp)

Returns a vector with length=1 pointing in the direction of another
Locatable. ";


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
%feature("docstring") cmf::upslope::cell_vector "";

%feature("docstring")  cmf::upslope::cell_vector::cell_vector "cell_vector() ";

%feature("docstring")  cmf::upslope::cell_vector::cell_vector "cell_vector(const cell_vector &copy) ";

%feature("docstring")  cmf::upslope::cell_vector::get_states "cmf::math::state_queue get_states()

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";

%feature("docstring")  cmf::upslope::cell_vector::append "void
append(cmf::upslope::Cell &cell) ";

%feature("docstring")  cmf::upslope::cell_vector::size "size_t size()
const ";

%feature("docstring")  cmf::upslope::cell_vector::get_slice "cell_vector get_slice(int start, int end, int step) ";

%feature("docstring")  cmf::upslope::cell_vector::cell_vector "cell_vector(cell_const_iterator first, cell_const_iterator last) ";

%feature("docstring")  cmf::upslope::cell_vector::begin "cell_iterator begin() ";

%feature("docstring")  cmf::upslope::cell_vector::end "cell_iterator
end() ";

%feature("docstring")  cmf::upslope::cell_vector::ptr_begin "cellvec::iterator ptr_begin() ";

%feature("docstring")  cmf::upslope::cell_vector::ptr_end "cellvec::iterator ptr_end() ";

%feature("docstring")  cmf::upslope::cell_vector::begin "cell_const_iterator begin() const ";

%feature("docstring")  cmf::upslope::cell_vector::end "cell_const_iterator end() const ";

%feature("docstring")  cmf::upslope::cell_vector::as_set "std::set<Cell*> as_set() ";


// File: classcmf_1_1upslope_1_1_cell_connector.xml
%feature("docstring") cmf::upslope::CellConnector "

A helper class to connect cells with flux_connection objects. This is
generated by flux_connection classes, intended to connect cells.

C++ includes: cell.h ";

%feature("docstring")  cmf::upslope::CellConnector::CellConnector "CellConnector(connectorfunction connector) ";

%feature("docstring")  cmf::upslope::CellConnector::connect "void
connect(cmf::upslope::Cell &cell1, cmf::upslope::Cell &cell2, int
start_at_layer=0) const ";


// File: classcmf_1_1river_1_1_channel.xml
%feature("docstring") cmf::river::Channel "

A wrapper for channel geometries.

C++ includes: ReachType.h ";

%feature("docstring")  cmf::river::Channel::get_length "double
get_length() const

Length of the reach. ";

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

Creates a reachtype using a short cut character. Acceptes one of the
following characters: 'T' TriangularReach, width and depth are
ignored.

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

%feature("docstring")  cmf::river::Channel::typecode "char typecode()
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
geometry. In most cases use get_flux_crossection=V/l, where V is the
stored volume and l is the reach length.

Wetted area of a river cross section [m2]

Parameters:
-----------

depth:  get_depth of the reach [m] ";

%feature("docstring")  cmf::river::Channel::get_wetted_perimeter "double get_wetted_perimeter(double depth) const

Calculates the wetted perimeter from a given actual depth [m] using
the actual IChannel geometry. ";

%feature("docstring")  cmf::river::Channel::copy "Channel* copy()
const ";

%feature("docstring")  cmf::river::Channel::set_nManning "void
set_nManning(double val) ";

%feature("docstring")  cmf::river::Channel::get_nManning "double
get_nManning() const ";

%feature("docstring")  cmf::river::Channel::h "virtual double
h(double V) const

Returns the depth of a given volume. ";

%feature("docstring")  cmf::river::Channel::A "virtual double
A(double V) const

Returns the area of the surface for a given volume. ";

%feature("docstring")  cmf::river::Channel::V "virtual double
V(double h) const ";

%feature("docstring")  cmf::river::Channel::qManning "double
qManning(double A, double slope) const

Calculates the flow rate from a given water volume in the reach
\\\\begin{eqnarray*} /// q_{Manning}&=& A R^{\\\\frac 2 3}
\\\\sqrt{\\\\frac {\\\\Delta_z} n} \\\\\\\\ /// A &=& \\\\frac V l
\\\\mbox{, (Crosssectional area of the wetted crossection, Volume per
length)} \\\\\\\\ /// R &=& \\\\frac A {P(d)} \\\\\\\\ /// P(d) &=&
\\\\mbox{ the perimeter of the wetted crosssection, a function of
reach depth} \\\\\\\\ /// d(V) &=& \\\\mbox{ the depth of the reach a
function of the volume} \\\\\\\\ /// \\\\Delta_z &=& \\\\frac{z_{max}
- z_{min}}{l} \\\\mbox{ Slope of the reach} /// \\\\end{eqnarray*}
Flow rate [m3/s]

Parameters:
-----------

A:  The area of the cross section [m2]

slope:  The slope of the reach [m/m] ";


// File: classcmf_1_1upslope_1_1connections_1_1_complete_infiltration.xml
%feature("docstring") cmf::upslope::connections::CompleteInfiltration
"

Connection for infiltration with saturated conductivity

C++ includes: infiltration.h ";

%feature("docstring")
cmf::upslope::connections::CompleteInfiltration::CompleteInfiltration
"CompleteInfiltration(cmf::upslope::SoilLayer::ptr soilwater,
cmf::water::flux_node::ptr surfacewater) ";

%feature("docstring")
cmf::upslope::connections::CompleteInfiltration::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::CompleteInfiltration::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::connections::CompleteInfiltration::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::CompleteInfiltration::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::CompleteInfiltration::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::CompleteInfiltration::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::CompleteInfiltration::exchange_target "void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")
cmf::upslope::connections::CompleteInfiltration::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::CompleteInfiltration::get_tracer_filter "real get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::connections::CompleteInfiltration::set_tracer_filter "void set_tracer_filter(real value) ";

%feature("docstring")
cmf::upslope::connections::CompleteInfiltration::to_string "std::string to_string() const ";

%feature("docstring")
cmf::upslope::connections::CompleteInfiltration::short_string "std::string short_string() const ";


// File: classcmf_1_1upslope_1_1_e_t_1_1constant_e_tpot.xml
%feature("docstring") cmf::upslope::ET::constantETpot "

A constant evapotranspiration

C++ includes: ET.h ";

%feature("docstring")  cmf::upslope::ET::constantETpot::GetETpot "real GetETpot(cmf::math::Time t) const ";

%feature("docstring")  cmf::upslope::ET::constantETpot::constantETpot
"constantETpot(cmf::upslope::SoilLayer::ptr source,
cmf::water::flux_node::ptr ET_target, double constantETpot_value) ";

%feature("docstring")  cmf::upslope::ET::constantETpot::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::ET::constantETpot::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::ET::constantETpot::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::upslope::ET::constantETpot::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::upslope::ET::constantETpot::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::ET::constantETpot::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::ET::constantETpot::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::ET::constantETpot::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::upslope::ET::constantETpot::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::ET::constantETpot::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")  cmf::upslope::ET::constantETpot::to_string "std::string to_string() const ";

%feature("docstring")  cmf::upslope::ET::constantETpot::short_string "std::string short_string() const ";


// File: classcmf_1_1atmosphere_1_1_constant_meteorology.xml
%feature("docstring") cmf::atmosphere::ConstantMeteorology "

A primitive implementation of the Meteorology interface. Holds a
Weather record and returns it for any date.

C++ includes: meteorology.h ";

%feature("docstring")
cmf::atmosphere::ConstantMeteorology::get_weather "virtual
cmf::atmosphere::Weather get_weather(cmf::math::Time t) const

Returns weather, regardless of t. ";

%feature("docstring")
cmf::atmosphere::ConstantMeteorology::get_instrument_height "virtual
real get_instrument_height() const ";

%feature("docstring")
cmf::atmosphere::ConstantMeteorology::ConstantMeteorology "ConstantMeteorology()

Creates a ConstantMeteorology with a standard weather. ";

%feature("docstring")
cmf::atmosphere::ConstantMeteorology::ConstantMeteorology "ConstantMeteorology(const cmf::atmosphere::Weather &w)

Creates a ConstantMeteorology with weather w. ";

%feature("docstring")
cmf::atmosphere::ConstantMeteorology::ConstantMeteorology "ConstantMeteorology(const cmf::atmosphere::ConstantMeteorology &other)
";

%feature("docstring")  cmf::atmosphere::ConstantMeteorology::copy "ConstantMeteorology* copy() const

Creates a new instannce of the ConstantMeteorology with the same
weather. ";


// File: classcmf_1_1atmosphere_1_1_constant_rain_source.xml
%feature("docstring") cmf::atmosphere::ConstantRainSource "

A simple implementation of RainSource. Returns intensity for any time
step.

C++ includes: precipitation.h ";

%feature("docstring")
cmf::atmosphere::ConstantRainSource::ConstantRainSource "ConstantRainSource(const cmf::project &_project, cmf::geometry::point
location, real _intensity)

Creates a new ConstantRainSource. Consider using Cell::set_rainfall
for internal creation of a constant rain source, instead of direct use

Parameters:
-----------

_project:  The project the rain source is belonging to.

location:  The location of the rain source

_intensity:  The constant rainfall intensity in mm/day ";

%feature("docstring")
cmf::atmosphere::ConstantRainSource::get_intensity "real
get_intensity(cmf::math::Time t) const

Returns the actual rainfall intensity in mm/day. ";

%feature("docstring")  cmf::atmosphere::ConstantRainSource::conc "virtual real conc(cmf::math::Time t, const cmf::water::solute &Solute)

Returns the concentration of a solute in the rainfall at time t. ";

%feature("docstring")  cmf::atmosphere::ConstantRainSource::set_conc "void set_conc(const cmf::water::solute &Solute, real value) ";

%feature("docstring")  cmf::atmosphere::ConstantRainSource::conc "virtual real conc(cmf::math::Time t, const cmf::water::solute &Solute)
const

Returns the water quality of the flux_node, if it is not overridden
this is the mix of the incoming fluxes. ";

%feature("docstring")
cmf::atmosphere::ConstantRainSource::RecalcFluxes "virtual bool
RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node. WaterStorage overrides this, since
state changes require an update of the fluxes ";

%feature("docstring")  cmf::atmosphere::ConstantRainSource::is_empty "virtual bool is_empty() const ";

%feature("docstring")  cmf::atmosphere::ConstantRainSource::project "const cmf::project& project() const

Returns the project, this node is part of. ";

%feature("docstring")  cmf::atmosphere::ConstantRainSource::is_storage
"virtual bool is_storage() const

true, if this is a waterstorage ";

%feature("docstring")  cmf::atmosphere::ConstantRainSource::to_string
"virtual std::string to_string() const ";

%feature("docstring")
cmf::atmosphere::ConstantRainSource::get_connections "cmf::water::connection_vector get_connections() const ";

%feature("docstring")
cmf::atmosphere::ConstantRainSource::get_connection "cmf::water::flux_connection* get_connection(const
cmf::water::flux_node &target)

Returns the connection between this and target. ";

%feature("docstring")
cmf::atmosphere::ConstantRainSource::remove_connection "bool
remove_connection(cmf::water::flux_node::ptr To)

Remove the connection. ";

%feature("docstring")  cmf::atmosphere::ConstantRainSource::flux_to "real flux_to(const cmf::water::flux_node &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from target into this\"). ";

%feature("docstring")  cmf::atmosphere::ConstantRainSource::flux3d_to
"cmf::geometry::point flux3d_to(const cmf::water::flux_node &target,
cmf::math::Time t) ";

%feature("docstring")
cmf::atmosphere::ConstantRainSource::get_3d_flux "cmf::geometry::point get_3d_flux(cmf::math::Time t) ";

%feature("docstring")
cmf::atmosphere::ConstantRainSource::water_balance "real
water_balance(cmf::math::Time t, const flux_connection *Without=0)
const

Returns the sum of all fluxes (positive and negative) at time t.
Single fluxes can be excluded from the calculation

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the water_balance
(e.g. to prevent closed circuits) ";

%feature("docstring")
cmf::atmosphere::ConstantRainSource::get_potential "virtual real
get_potential() const

Returns the water potential of the node in m waterhead The base class
water storage always returns the height of the location ";

%feature("docstring")
cmf::atmosphere::ConstantRainSource::set_potential "virtual void
set_potential(real new_potential) ";


// File: classcmf_1_1math_1_1_c_vode_integrator.xml
%feature("docstring") cmf::math::CVodeIntegrator "

A wrapper class for the CVODE integrator from the SUNDIALS library

https://computation.llnl.gov/casc/sundials/main.html

C++ includes: cvodeintegrator.h ";

/*  The state variables to integrate  */

/*

*/

%feature("docstring")  cmf::math::CVodeIntegrator::CopyStates "void
CopyStates(num_array &destination) const

Copies the states to a numeric vector using use_OpenMP. ";

%feature("docstring")  cmf::math::CVodeIntegrator::CopyStates "void
CopyStates(real *destination) const ";

%feature("docstring")  cmf::math::CVodeIntegrator::SetStates "void
SetStates(const num_array &newStates)

Copies the new states to the actual states. ";

%feature("docstring")  cmf::math::CVodeIntegrator::SetStates "void
SetStates(real *newStates) ";

%feature("docstring")  cmf::math::CVodeIntegrator::CopyDerivs "void
CopyDerivs(Time time, num_array &destination, real factor=1) const

Copies the derivatives at time step \"time\" to a numeric vector using
use_OpenMP

Parameters:
-----------

time:   Time at which the derivatives should be calculated

destination:  Vector to be overwritten by the results

factor:  A factor that is multiplied to the derivate (e.g. unit
conversion or integration length) ";

%feature("docstring")  cmf::math::CVodeIntegrator::CopyDerivs "void
CopyDerivs(Time time, real *destination, real factor=1) const

Copies the derivatives at time step \"time\" to an preallocated c
array

Parameters:
-----------

time:   Time at which the derivatives should be calculated

destination:  Allocated c array

factor:  A factor that is multiplied to the derivate (e.g. unit
conversion or integration length) ";

%feature("docstring")  cmf::math::CVodeIntegrator::GetStates "num_array GetStates() const

Returns the states in a numeric vector using :CopyStates, but is
slower because of additional memory allocation. ";

%feature("docstring")  cmf::math::CVodeIntegrator::GetDerivs "num_array GetDerivs(Time time) const

Returns the derivatives at time step \"time\" in a numeric vector
using :CopyDerivs, but is slower because of additional memory
allocation. ";

%feature("docstring")  cmf::math::CVodeIntegrator::AddValuesToStates "void AddValuesToStates(const num_array &operands) ";

%feature("docstring")  cmf::math::CVodeIntegrator::AddStatesFromOwner
"virtual void AddStatesFromOwner(cmf::math::StateVariableOwner
&stateOwner) ";

%feature("docstring")  cmf::math::CVodeIntegrator::AddState "virtual
void AddState(cmf::math::StateVariable::ptr state) ";

/*  model time  */

/*

*/

%feature("docstring")  cmf::math::CVodeIntegrator::ModelTime "cmf::math::Time ModelTime() const

Returns the current model time. ";

%feature("docstring")  cmf::math::CVodeIntegrator::ModelTime "void
ModelTime(cmf::math::Time val)

Sets the current model time. ";

%feature("docstring")  cmf::math::CVodeIntegrator::TimeStep "cmf::math::Time TimeStep() const

Returns the last time step. ";

%feature("docstring")  cmf::math::CVodeIntegrator::NextTimeStep "cmf::math::Time NextTimeStep() const

Returns the next time step width. ";

%feature("docstring")  cmf::math::CVodeIntegrator::NextTimeStep "void
NextTimeStep(cmf::math::Time val)

Sets the next time step width. ";

%feature("docstring")  cmf::math::CVodeIntegrator::MinTimestep "const
cmf::math::Time MinTimestep() const

Returns The minimal allowed time step length. ";

%feature("docstring")  cmf::math::CVodeIntegrator::Iterations "int
Iterations() const ";

%feature("docstring")  cmf::math::CVodeIntegrator::ResetIterations "void ResetIterations() ";

/*  Constructors and Destructors  */

/*

*/

/*  Integrate  */

/*

*/

%feature("docstring")  cmf::math::CVodeIntegrator::Integrate "int
Integrate(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::CVodeIntegrator::IntegrateUntil "void IntegrateUntil(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::CVodeIntegrator::IntegrateUntil "void IntegrateUntil(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables until MaxTime. ";

/*  Accuracy parameters  */

/*

*/

%feature("docstring")  cmf::math::CVodeIntegrator::GetOrder "int
GetOrder()

Returns the last order of the solver used, may be smaller than
MaxOrder due to the number of steps already taken or to fullfill
stability limit. ";

%feature("docstring")  cmf::math::CVodeIntegrator::ReInit "void
ReInit(Time initdt, real epsilon=0) ";

%feature("docstring")  cmf::math::CVodeIntegrator::Initialize "void
Initialize()

Initializes the solver. Do not add or remove statevariables after
initialization. ";

%feature("docstring")  cmf::math::CVodeIntegrator::Integrate "virtual
int Integrate(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables

Parameters:
-----------

MaxTime:  To stop the model (if running in a model framework) at time
steps of value exchange e.g. full hours, the next value exchange time
can be given

TimeStep:  Takes the proposed timestep, and changes it into the
effictivly used timestep according to the local stiffness of the
problem and MaxTime ";

%feature("docstring")  cmf::math::CVodeIntegrator::Reset "virtual
void Reset() ";

%feature("docstring")  cmf::math::CVodeIntegrator::CVodeIntegrator "CVodeIntegrator(real epsilon=1e-9, char _preconditioner='R')

Create a new CVODE integrator

Parameters:
-----------

epsilon:  relative and absolute error tolerance

tStepMin:  Minimal timestep ";

%feature("docstring")  cmf::math::CVodeIntegrator::CVodeIntegrator "CVodeIntegrator(cmf::math::StateVariableOwner &states, real
epsilon=1e-9, char _preconditioner='R') ";

%feature("docstring")  cmf::math::CVodeIntegrator::CVodeIntegrator "CVodeIntegrator(const CVodeIntegrator &templ)

Copy constructor, creates a new CVODE integrator similiar to the
given, but without statevariables. ";

%feature("docstring")  cmf::math::CVodeIntegrator::get_error "cmf::math::num_array get_error() const ";

%feature("docstring")  cmf::math::CVodeIntegrator::Copy "CVodeIntegrator* Copy() const

Returns a new Integrator, based on this (without the state variables),
e.g. same type, epsilon, model time etc. ";

%feature("docstring")  cmf::math::CVodeIntegrator::~CVodeIntegrator "virtual ~CVodeIntegrator() ";

%feature("docstring")  cmf::math::CVodeIntegrator::size "int size()
const

returns the number of state variables ";

%feature("docstring")  cmf::math::CVodeIntegrator::get_state "real
get_state(int position) const

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::CVodeIntegrator::set_state "void
set_state(int position, real newState)

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::CVodeIntegrator::get_states "state_queue get_states()

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";


// File: classcmf_1_1upslope_1_1connections_1_1_darcy.xml
%feature("docstring") cmf::upslope::connections::Darcy "

Calculates the lateral flow using the gravitational potential gradient
only

\\\\[ q_{lat} = \\\\frac{\\\\Delta \\\\Psi_G}{\\\\|C_1-C_2\\\\|}
\\\\frac 12 (T(C_1)+T(C_2)) w \\\\] where  $ \\\\Delta \\\\Psi_G $ is
the gravitational potential difference

$ \\\\|C_1-C_2\\\\| $ is the distance from Cell 1 to Cell 2

$ T(C) $ is the transmissivity of cell C, calculated by
SoilType::Transmissivity

$ w $ is the width of the connection of the cells

C++ includes: subsurfacefluxes.h ";

%feature("docstring")  cmf::upslope::connections::Darcy::Darcy "Darcy(cmf::upslope::SoilLayer::ptr left, cmf::water::flux_node::ptr
right, real FlowWidth, real Distance=0) ";

%feature("docstring")  cmf::upslope::connections::Darcy::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::connections::Darcy::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::connections::Darcy::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::upslope::connections::Darcy::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::upslope::connections::Darcy::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::connections::Darcy::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::Darcy::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::connections::Darcy::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::Darcy::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::connections::Darcy::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")  cmf::upslope::connections::Darcy::to_string "std::string to_string() const ";

%feature("docstring")  cmf::upslope::connections::Darcy::short_string
"std::string short_string() const ";


// File: classcmf_1_1upslope_1_1connections_1_1_darcy_kinematic.xml
%feature("docstring") cmf::upslope::connections::DarcyKinematic "

A simple kinemtic wave model for subsurface flux

\\\\[ q = \\\\frac{\\\\Delta z_{surface}}{d} K(\\\\theta)_{source}
A_{cross} \\\\]

C++ includes: subsurfacefluxes.h ";

%feature("docstring")
cmf::upslope::connections::DarcyKinematic::DarcyKinematic "DarcyKinematic(cmf::upslope::SoilLayer::ptr left,
cmf::water::flux_node::ptr right, real FlowWidth, real Distance=0) ";

%feature("docstring")  cmf::upslope::connections::DarcyKinematic::q "real q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::DarcyKinematic::left_node "flux_node::ptr
left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::connections::DarcyKinematic::right_node "flux_node::ptr
right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::DarcyKinematic::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

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
cmf::upslope::connections::DarcyKinematic::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::connections::DarcyKinematic::conc
"real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::DarcyKinematic::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::connections::DarcyKinematic::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")
cmf::upslope::connections::DarcyKinematic::to_string "std::string
to_string() const ";

%feature("docstring")
cmf::upslope::connections::DarcyKinematic::short_string "std::string
short_string() const ";


// File: structcmf_1_1math_1_1_date.xml
%feature("docstring") cmf::math::Date "

An absolute time, not for calculation. Date and Time are
interchangable.

C++ includes: time.h ";

%feature("docstring")  cmf::math::Date::Date "Date(int _day, int
_month, int _year=2001, int _hour=0, int _minute=0, int _second=0, int
_ms=0)

Creates a new date. ";

%feature("docstring")  cmf::math::Date::Date "Date(const Time &time)

Creates a new date from time (based on the 31.12.1899, like in
Excel(TM)). ";

%feature("docstring")  cmf::math::Date::ToTime "Time ToTime()

Converts a date to Time (based on the 31.12.1899, like in Excel(TM).
";

%feature("docstring")  cmf::math::Date::DOY "double DOY()

Returns the day of year. ";

%feature("docstring")  cmf::math::Date::to_string "std::string
to_string()

Returns a string representing the date. ";


// File: classcmf_1_1water_1_1_drichelet_boundary.xml
%feature("docstring") cmf::water::DricheletBoundary "

Drichelet (constant head) boundary condition

This boundary condition can be used either as a pure sink boundary
condition or as a conditional source / sink boundary condition. The
constant head of the boundary condition is interpreted and handled by
the connections of the boundary condition. Not head aware connections,
should not be used, since they are ignoring the constant head.

C++ includes: boundary_condition.h ";

%feature("docstring")  cmf::water::DricheletBoundary::get_potential "real get_potential() const

Returns the water potential of the node in m waterhead The base class
water storage always returns the height of the location ";

%feature("docstring")  cmf::water::DricheletBoundary::set_potential "void set_potential(real new_potential) ";

%feature("docstring")  cmf::water::DricheletBoundary::set_conc "virtual void set_conc(const cmf::water::solute &_Solute, double value)
";

%feature("docstring")  cmf::water::DricheletBoundary::conc "virtual
real conc(cmf::math::Time t, const cmf::water::solute &_Solute) const

Returns the water quality of the flux_node, if it is not overridden
this is the mix of the incoming fluxes. ";

%feature("docstring")  cmf::water::DricheletBoundary::is_empty "bool
is_empty() const ";

%feature("docstring")  cmf::water::DricheletBoundary::RecalcFluxes "bool RecalcFluxes(cmf::math::Time t) const ";

%feature("docstring")
cmf::water::DricheletBoundary::DricheletBoundary "DricheletBoundary(const cmf::project &_p, real potential,
cmf::geometry::point Location=cmf::geometry::point()) ";

%feature("docstring")  cmf::water::DricheletBoundary::project "const
cmf::project& project() const

Returns the project, this node is part of. ";

%feature("docstring")  cmf::water::DricheletBoundary::is_storage "virtual bool is_storage() const

true, if this is a waterstorage ";

%feature("docstring")  cmf::water::DricheletBoundary::to_string "virtual std::string to_string() const ";

%feature("docstring")  cmf::water::DricheletBoundary::get_connections
"cmf::water::connection_vector get_connections() const ";

%feature("docstring")  cmf::water::DricheletBoundary::RecalcFluxes "virtual bool RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node. WaterStorage overrides this, since
state changes require an update of the fluxes ";

%feature("docstring")  cmf::water::DricheletBoundary::get_connection "cmf::water::flux_connection* get_connection(const
cmf::water::flux_node &target)

Returns the connection between this and target. ";

%feature("docstring")
cmf::water::DricheletBoundary::remove_connection "bool
remove_connection(cmf::water::flux_node::ptr To)

Remove the connection. ";

%feature("docstring")  cmf::water::DricheletBoundary::flux_to "real
flux_to(const cmf::water::flux_node &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from target into this\"). ";

%feature("docstring")  cmf::water::DricheletBoundary::flux3d_to "cmf::geometry::point flux3d_to(const cmf::water::flux_node &target,
cmf::math::Time t) ";

%feature("docstring")  cmf::water::DricheletBoundary::get_3d_flux "cmf::geometry::point get_3d_flux(cmf::math::Time t) ";

%feature("docstring")  cmf::water::DricheletBoundary::water_balance "real water_balance(cmf::math::Time t, const flux_connection
*Without=0) const

Returns the sum of all fluxes (positive and negative) at time t.
Single fluxes can be excluded from the calculation

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the water_balance
(e.g. to prevent closed circuits) ";


// File: classcmf_1_1upslope_1_1connections_1_1_energy_budget_snow_melt.xml
%feature("docstring") cmf::upslope::connections::EnergyBudgetSnowMelt
"

Calculates snow melt using the surface energy budget method

C++ includes: surfacefluxes.h ";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::EnergyBudgetSnowMelt
"EnergyBudgetSnowMelt(cmf::water::WaterStorage::ptr snow,
cmf::water::flux_node::ptr surface_water, cmf::upslope::Cell &cell) ";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::exchange_target "void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::get_tracer_filter "real get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::set_tracer_filter "void set_tracer_filter(real value) ";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::to_string "std::string to_string() const ";

%feature("docstring")
cmf::upslope::connections::EnergyBudgetSnowMelt::short_string "std::string short_string() const ";


// File: classcmf_1_1math_1_1_explicit_euler__fixed.xml
%feature("docstring") cmf::math::ExplicitEuler_fixed "

An explicit Euler integrator, with a fixed time step.

C++ includes: explicit_euler.h ";

/*  The state variables to integrate  */

/*

*/

%feature("docstring")  cmf::math::ExplicitEuler_fixed::CopyStates "void CopyStates(num_array &destination) const

Copies the states to a numeric vector using use_OpenMP. ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::CopyStates "void CopyStates(real *destination) const ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::SetStates "void SetStates(const num_array &newStates)

Copies the new states to the actual states. ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::SetStates "void SetStates(real *newStates) ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::CopyDerivs "void CopyDerivs(Time time, num_array &destination, real factor=1)
const

Copies the derivatives at time step \"time\" to a numeric vector using
use_OpenMP

Parameters:
-----------

time:   Time at which the derivatives should be calculated

destination:  Vector to be overwritten by the results

factor:  A factor that is multiplied to the derivate (e.g. unit
conversion or integration length) ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::CopyDerivs "void CopyDerivs(Time time, real *destination, real factor=1) const

Copies the derivatives at time step \"time\" to an preallocated c
array

Parameters:
-----------

time:   Time at which the derivatives should be calculated

destination:  Allocated c array

factor:  A factor that is multiplied to the derivate (e.g. unit
conversion or integration length) ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::GetStates "num_array GetStates() const

Returns the states in a numeric vector using :CopyStates, but is
slower because of additional memory allocation. ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::GetDerivs "num_array GetDerivs(Time time) const

Returns the derivatives at time step \"time\" in a numeric vector
using :CopyDerivs, but is slower because of additional memory
allocation. ";

%feature("docstring")
cmf::math::ExplicitEuler_fixed::AddValuesToStates "void
AddValuesToStates(const num_array &operands) ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::AddState "virtual void AddState(cmf::math::StateVariable::ptr state) ";

/*  model time  */

/*

*/

%feature("docstring")  cmf::math::ExplicitEuler_fixed::ModelTime "cmf::math::Time ModelTime() const

Returns the current model time. ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::ModelTime "void ModelTime(cmf::math::Time val)

Sets the current model time. ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::TimeStep "cmf::math::Time TimeStep() const

Returns the last time step. ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::NextTimeStep "cmf::math::Time NextTimeStep() const

Returns the next time step width. ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::NextTimeStep "void NextTimeStep(cmf::math::Time val)

Sets the next time step width. ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::MinTimestep "const cmf::math::Time MinTimestep() const

Returns The minimal allowed time step length. ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::Iterations "int Iterations() const ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::ResetIterations
"void ResetIterations() ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::Reset "virtual
void Reset() ";

/*  Constructors and Destructors  */

/*

*/

/*  Integrate  */

/*

*/

%feature("docstring")  cmf::math::ExplicitEuler_fixed::Integrate "int
Integrate(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::IntegrateUntil
"void IntegrateUntil(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::IntegrateUntil
"void IntegrateUntil(cmf::math::Time MaxTime, cmf::math::Time
TimeStep)

Integrates the vector of state variables until MaxTime. ";

/*  Accuracy parameters  */

/*

*/

%feature("docstring")
cmf::math::ExplicitEuler_fixed::AddStatesFromOwner "void
AddStatesFromOwner(cmf::math::StateVariableOwner &stateOwner) ";

%feature("docstring")
cmf::math::ExplicitEuler_fixed::ExplicitEuler_fixed "ExplicitEuler_fixed(StateVariableOwner &states, real epsilon=1e-9,
cmf::math::Time tStepMin=Time::Seconds(10))

Constructs a new ExplicitEuler_fixed from a pointer to a vector of
state variables The Integrator becomes the owner of states

Parameters:
-----------

states:  Statevariables of the system

epsilon:  (ignored) relative error tolerance per time step
(default=1e-9)

tStepMin:  (ignored) minimum time step (default=10s) ";

%feature("docstring")
cmf::math::ExplicitEuler_fixed::ExplicitEuler_fixed "ExplicitEuler_fixed(real epsilon=1e-9, cmf::math::Time
tStepMin=Time::Seconds(10))

Constructs a new ExplicitEuler_fixed

Parameters:
-----------

epsilon:  (ignored) relative error tolerance per time step
(default=1e-9)

tStepMin:  (ignored) minimum time step (default=10s) ";

%feature("docstring")
cmf::math::ExplicitEuler_fixed::ExplicitEuler_fixed "ExplicitEuler_fixed(const Integrator &copy)

Copy constructor. ";

%feature("docstring")
cmf::math::ExplicitEuler_fixed::~ExplicitEuler_fixed "virtual
~ExplicitEuler_fixed() ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::Copy "virtual
Integrator* Copy() const

Returns a new Integrator, based on this (without the state variables),
e.g. same type, epsilon, model time etc. ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::Integrate "int
Integrate(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables

Parameters:
-----------

MaxTime:  (ignored) To stop the model (if running in a model
framework) at time steps of value exchange e.g. full hours, the next
value exchange time can be given

TimeStep:  Takes the proposed time step ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::size "int
size() const

returns the number of state variables ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::get_state "real get_state(int position) const

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::set_state "void set_state(int position, real newState)

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::get_states "state_queue get_states()

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";


// File: classcmf_1_1water_1_1external__control__connection.xml
%feature("docstring") cmf::water::external_control_connection "

Flux from one node to another, controlled by the user or an external
program, by changing the flux constant

C++ includes: flux_connection.h ";

%feature("docstring")
cmf::water::external_control_connection::external_control_connection "external_control_connection(flux_node::ptr source, flux_node::ptr
target, real flux_value=0) ";

%feature("docstring")  cmf::water::external_control_connection::q "real q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::water::external_control_connection::left_node "flux_node::ptr
left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::water::external_control_connection::right_node "flux_node::ptr
right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::water::external_control_connection::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::water::external_control_connection::get_target "flux_node::ptr
get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::water::external_control_connection::get_target "flux_node::ptr
get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::water::external_control_connection::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::water::external_control_connection::conc "real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::water::external_control_connection::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::water::external_control_connection::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")
cmf::water::external_control_connection::to_string "std::string
to_string() const ";

%feature("docstring")
cmf::water::external_control_connection::short_string "std::string
short_string() const ";


// File: classcmf_1_1water_1_1flux__connection.xml
%feature("docstring") cmf::water::flux_connection "

Represents a connection between flux_nodes, where water fluxes occur.

C++ includes: flux_connection.h ";

%feature("docstring")  cmf::water::flux_connection::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::water::flux_connection::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::water::flux_connection::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::water::flux_connection::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::water::flux_connection::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")  cmf::water::flux_connection::exchange_target "void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")  cmf::water::flux_connection::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::water::flux_connection::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")  cmf::water::flux_connection::get_tracer_filter
"real get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")  cmf::water::flux_connection::set_tracer_filter
"void set_tracer_filter(real value) ";

%feature("docstring")  cmf::water::flux_connection::to_string "std::string to_string() const ";

%feature("docstring")  cmf::water::flux_connection::short_string "std::string short_string() const ";

%feature("docstring")  cmf::water::flux_connection::flux_connection "flux_connection(flux_node::ptr left, flux_node::ptr right, std::string
_type)

Creates a flux connection between the flux_node left and right

Parameters:
-----------

left:   flux_node on the one side of the connection

right:   flux_node on the other side of the connection

_type:  Type of the flux connection ";

%feature("docstring")  cmf::water::flux_connection::~flux_connection "virtual ~flux_connection() ";


// File: classcmf_1_1water_1_1flux__node.xml
%feature("docstring") cmf::water::flux_node "

Base class for everything that can be connected by fluxes.

Flux nodes can be WaterStorages, flux end points, sinks, sources and
bridges to other model domains (e.g. Ponded water to river system).
The base class can be used where a simple routing, potentially with
mixing, is needed.

C++ includes: flux_node.h ";

%feature("docstring")  cmf::water::flux_node::project "const
cmf::project& project() const

Returns the project, this node is part of. ";

%feature("docstring")  cmf::water::flux_node::is_storage "virtual
bool is_storage() const

true, if this is a waterstorage ";

%feature("docstring")  cmf::water::flux_node::to_string "virtual
std::string to_string() const ";

%feature("docstring")  cmf::water::flux_node::get_connections "cmf::water::connection_vector get_connections() const ";

%feature("docstring")  cmf::water::flux_node::~flux_node "virtual
~flux_node()

The destructor deletes all connections. ";

%feature("docstring")  cmf::water::flux_node::RecalcFluxes "virtual
bool RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node. WaterStorage overrides this, since
state changes require an update of the fluxes ";

%feature("docstring")  cmf::water::flux_node::get_connection "cmf::water::flux_connection* get_connection(const
cmf::water::flux_node &target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::water::flux_node::remove_connection "bool
remove_connection(cmf::water::flux_node::ptr To)

Remove the connection. ";

%feature("docstring")  cmf::water::flux_node::flux_to "real
flux_to(const cmf::water::flux_node &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from target into this\"). ";

%feature("docstring")  cmf::water::flux_node::flux3d_to "cmf::geometry::point flux3d_to(const cmf::water::flux_node &target,
cmf::math::Time t) ";

%feature("docstring")  cmf::water::flux_node::get_3d_flux "cmf::geometry::point get_3d_flux(cmf::math::Time t) ";

%feature("docstring")  cmf::water::flux_node::water_balance "real
water_balance(cmf::math::Time t, const flux_connection *Without=0)
const

Returns the sum of all fluxes (positive and negative) at time t.
Single fluxes can be excluded from the calculation

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the water_balance
(e.g. to prevent closed circuits) ";

%feature("docstring")  cmf::water::flux_node::conc "virtual real
conc(cmf::math::Time t, const cmf::water::solute &Solute) const

Returns the water quality of the flux_node, if it is not overridden
this is the mix of the incoming fluxes. ";

%feature("docstring")  cmf::water::flux_node::get_potential "virtual
real get_potential() const

Returns the water potential of the node in m waterhead The base class
water storage always returns the height of the location ";

%feature("docstring")  cmf::water::flux_node::set_potential "virtual
void set_potential(real new_potential) ";

%feature("docstring")  cmf::water::flux_node::is_empty "virtual bool
is_empty() const ";

%feature("docstring")  cmf::water::flux_node::flux_node "flux_node(const cmf::project &_project, cmf::geometry::point
location=cmf::geometry::point()) ";


// File: classcmf_1_1upslope_1_1_e_t_1_1_hargreave_e_t.xml
%feature("docstring") cmf::upslope::ET::HargreaveET "

Calculates the Evapotranspiration using Hargreave's equation

Todo document Hargreave

C++ includes: ET.h ";

%feature("docstring")  cmf::upslope::ET::HargreaveET::HargreaveET "HargreaveET(cmf::upslope::SoilLayer::ptr source,
cmf::water::flux_node::ptr ET_target) ";

%feature("docstring")  cmf::upslope::ET::HargreaveET::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::ET::HargreaveET::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::ET::HargreaveET::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::upslope::ET::HargreaveET::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::upslope::ET::HargreaveET::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::ET::HargreaveET::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")  cmf::upslope::ET::HargreaveET::exchange_target
"void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")  cmf::upslope::ET::HargreaveET::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::upslope::ET::HargreaveET::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::ET::HargreaveET::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")  cmf::upslope::ET::HargreaveET::to_string "std::string to_string() const ";

%feature("docstring")  cmf::upslope::ET::HargreaveET::short_string "std::string short_string() const ";


// File: classcmf_1_1upslope_1_1connections_1_1_h_b_vlateral.xml
%feature("docstring") cmf::upslope::connections::HBVlateral "

Lateral flow using a simple linear storage, can be directed to another
cell (HBV D like) or to a reach (HBV like)

\\\\[ q = kV \\\\]

C++ includes: HBVflow.h ";

%feature("docstring")
cmf::upslope::connections::HBVlateral::HBVlateral "HBVlateral(cmf::upslope::SoilLayer::ptr left,
cmf::water::flux_node::ptr right, real width, real dist) ";

%feature("docstring")  cmf::upslope::connections::HBVlateral::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::HBVlateral::left_node "flux_node::ptr
left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::connections::HBVlateral::right_node "flux_node::ptr
right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::upslope::connections::HBVlateral::kill_me
"bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::HBVlateral::get_target "flux_node::ptr
get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::HBVlateral::get_target "flux_node::ptr
get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::HBVlateral::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::connections::HBVlateral::conc "real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::HBVlateral::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::connections::HBVlateral::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")
cmf::upslope::connections::HBVlateral::to_string "std::string
to_string() const ";

%feature("docstring")
cmf::upslope::connections::HBVlateral::short_string "std::string
short_string() const ";


// File: structcmf_1_1upslope_1_1connections_1_1_h_b_vparameters.xml
%feature("docstring") cmf::upslope::connections::HBVparameters "

This class holds the properties for all HBV like connections
Implements the RetentionCurve interface is only partly, therefore it
is not suitable for other connection types

\\\\begin{eqnarray*} /// K(w,d) &=& PERC w \\\\\\\\ /// \\\\Psi_{M}
&=& 10 - \\\\frac{10}{w}\\\\\\\\ /// w(\\\\Psi_{M}) &=&
\\\\frac{10}{10 - \\\\Psi_M} /// \\\\end{eqnarray*}

C++ includes: HBVflow.h ";

%feature("docstring")
cmf::upslope::connections::HBVparameters::HBVparameters "HBVparameters(double _k0=1, double _k1=0.25, double _k2=0.005, double
_perc=0.05, double _fc=0.3, double _beta=4.0, double _uplim=.35,
double _lowlim=1.0, double _cfmax=2, double _sfcf=0.6, double
_cwh=0.1, double _cfr=0.05) ";

%feature("docstring")  cmf::upslope::connections::HBVparameters::K "virtual real K(real wetness) const

Returns the conductivity in m/day at a certain depth and water
content. ";

%feature("docstring")
cmf::upslope::connections::HBVparameters::Porosity "virtual real
Porosity(real depth) const

Returns the porosity at a certain depth. ";

%feature("docstring")
cmf::upslope::connections::HBVparameters::VoidVolume "virtual real
VoidVolume(real upperDepth, real lowerDepth, real Area) const

Returns the void volume of a soil column. ";

%feature("docstring")
cmf::upslope::connections::HBVparameters::Wetness "virtual real
Wetness(real suction) const

returns the wetness (volumetric water content per pore space) at a
given suction pressure ";

%feature("docstring")
cmf::upslope::connections::HBVparameters::MatricPotential "virtual
real MatricPotential(real wetness) const

returns the wetness of the soil at given water content ";

%feature("docstring")  cmf::upslope::connections::HBVparameters::copy
"virtual HBVparameters* copy() const ";

%feature("docstring")
cmf::upslope::connections::HBVparameters::Wetness_eff "virtual real
Wetness_eff(real wetness, real pF_r=4.2) const

Returns the effective wetness, using a residual pF value \\\\[w_{eff}
=
\\\\frac{w_{act}-w\\\\left(pF_r\\\\right)}{1-w\\\\left(pF_r\\\\right)}\\\\]
";

%feature("docstring")
cmf::upslope::connections::HBVparameters::FillHeight "virtual real
FillHeight(real lowerDepth, real Area, real Volume) const

Returns the thickness of a soil column with a certain pore volume. ";

%feature("docstring")
cmf::upslope::connections::HBVparameters::Wetness_pF "real
Wetness_pF(real pF) const

returns the volumetric water content at a given pF value ";


// File: classcmf_1_1upslope_1_1connections_1_1_h_b_vpercolation.xml
%feature("docstring") cmf::upslope::connections::HBVpercolation "

Calculates the percolation as in the HBV model

\\\\[ Q = \\\\left\\\\{Q_{in}
\\\\left(\\\\frac{V}{FC}\\\\right)^\\\\beta\\\\mbox{ if first layer}
\\\\\\\\ /// min(PERC, V_{lower} - V_{lower,max}) \\\\right. \\\\]

C++ includes: HBVflow.h ";

%feature("docstring")
cmf::upslope::connections::HBVpercolation::HBVpercolation "HBVpercolation(cmf::upslope::SoilLayer::ptr left,
cmf::water::flux_node::ptr right) ";

%feature("docstring")  cmf::upslope::connections::HBVpercolation::q "real q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::HBVpercolation::left_node "flux_node::ptr
left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::connections::HBVpercolation::right_node "flux_node::ptr
right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::HBVpercolation::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::HBVpercolation::get_target "flux_node::ptr
get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::HBVpercolation::get_target "flux_node::ptr
get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::HBVpercolation::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::connections::HBVpercolation::conc
"real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::HBVpercolation::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::connections::HBVpercolation::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")
cmf::upslope::connections::HBVpercolation::to_string "std::string
to_string() const ";

%feature("docstring")
cmf::upslope::connections::HBVpercolation::short_string "std::string
short_string() const ";


// File: classcmf_1_1river_1_1_i_channel.xml
%feature("docstring") cmf::river::IChannel "

Structure for the description of structural parameters of a reach
Abstract base class for different IChannel geometries.

C++ includes: ReachType.h ";

%feature("docstring")  cmf::river::IChannel::get_nManning "virtual
double get_nManning() const ";

%feature("docstring")  cmf::river::IChannel::set_nManning "virtual
void set_nManning(double val) ";

%feature("docstring")  cmf::river::IChannel::get_length "virtual
double get_length() const =0

Length of the reach. ";

%feature("docstring")  cmf::river::IChannel::typecode "virtual char
typecode() const =0 ";

%feature("docstring")  cmf::river::IChannel::get_channel_width "virtual double get_channel_width(double depth) const =0

Calculates the flow width from a given actual depth [m] using the
actual IChannel geometry. ";

%feature("docstring")  cmf::river::IChannel::get_wetted_perimeter "virtual double get_wetted_perimeter(double depth) const =0

Calculates the wetted perimeter from a given actual depth [m] using
the actual IChannel geometry. ";

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
geometry. In most cases use get_flux_crossection=V/l, where V is the
stored volume and l is the reach length.

Wetted area of a river cross section [m2]

Parameters:
-----------

depth:  get_depth of the reach [m] ";

%feature("docstring")  cmf::river::IChannel::h "virtual double
h(double V) const

Returns the depth of a given volume. ";

%feature("docstring")  cmf::river::IChannel::A "virtual double
A(double V) const

Returns the area of the surface for a given volume. ";

%feature("docstring")  cmf::river::IChannel::V "virtual double
V(double h) const ";

%feature("docstring")  cmf::river::IChannel::copy "virtual IChannel*
copy() const =0 ";

%feature("docstring")  cmf::river::IChannel::qManning "double
qManning(double A, double slope) const

Calculates the flow rate from a given water volume in the reach
\\\\begin{eqnarray*} /// q_{Manning}&=& A R^{\\\\frac 2 3}
\\\\sqrt{\\\\frac {\\\\Delta_z} n} \\\\\\\\ /// A &=& \\\\frac V l
\\\\mbox{, (Crosssectional area of the wetted crossection, Volume per
length)} \\\\\\\\ /// R &=& \\\\frac A {P(d)} \\\\\\\\ /// P(d) &=&
\\\\mbox{ the perimeter of the wetted crosssection, a function of
reach depth} \\\\\\\\ /// d(V) &=& \\\\mbox{ the depth of the reach a
function of the volume} \\\\\\\\ /// \\\\Delta_z &=& \\\\frac{z_{max}
- z_{min}}{l} \\\\mbox{ Slope of the reach} /// \\\\end{eqnarray*}
Flow rate [m3/s]

Parameters:
-----------

A:  The area of the cross section [m2]

slope:  The slope of the reach [m/m] ";

%feature("docstring")  cmf::river::IChannel::IChannel "IChannel(double manning_n=0.035) ";


// File: classcmf_1_1atmosphere_1_1_i_d_w___meteorology.xml
%feature("docstring") cmf::atmosphere::IDW_Meteorology "

Regionalizes meteorological measurements using a simple inverse
distance weighted (IDW) method See:  IDW

C++ includes: meteorology.h ";

%feature("docstring")
cmf::atmosphere::IDW_Meteorology::IDW_Meteorology "IDW_Meteorology(const cmf::geometry::Locatable &position, const
MeteoStationList &stations, double z_weight, double power) ";

%feature("docstring")
cmf::atmosphere::IDW_Meteorology::IDW_Meteorology "IDW_Meteorology(const IDW_Meteorology &copy) ";

%feature("docstring")  cmf::atmosphere::IDW_Meteorology::get_weather "virtual Weather get_weather(cmf::math::Time t) const

Returns the Weather at time t. Pure virtual function. Must get
implemented by child functions. ";

%feature("docstring")  cmf::atmosphere::IDW_Meteorology::copy "virtual IDW_Meteorology* copy() const

Returns a copy of the meteorology object. Pure virtual function, needs
to be implemented. ";

%feature("docstring")
cmf::atmosphere::IDW_Meteorology::get_instrument_height "virtual real
get_instrument_height() const ";


// File: classcmf_1_1atmosphere_1_1_i_d_w_rainfall.xml
%feature("docstring") cmf::atmosphere::IDWRainfall "

A RainSource using an spatially interpolated rainfall intensity from
all stations. Interpolation method is inverse distance weighted (IDW)

C++ includes: precipitation.h ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::get_intensity "real get_intensity(cmf::math::Time t) const

Returns the actual rainfall intensity in mm/day. ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::conc "real
conc(cmf::math::Time t, const cmf::water::solute &Solute)

Returns the concentration of a solute in the rainfall at time t. ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::conc "virtual
real conc(cmf::math::Time t, const cmf::water::solute &Solute) const

Returns the water quality of the flux_node, if it is not overridden
this is the mix of the incoming fluxes. ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::RecalcFluxes "virtual bool RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node. WaterStorage overrides this, since
state changes require an update of the fluxes ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::is_empty "virtual bool is_empty() const ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::project "const
cmf::project& project() const

Returns the project, this node is part of. ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::is_storage "virtual bool is_storage() const

true, if this is a waterstorage ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::to_string "virtual std::string to_string() const ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::get_connections "cmf::water::connection_vector get_connections() const ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::get_connection "cmf::water::flux_connection* get_connection(const
cmf::water::flux_node &target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::remove_connection
"bool remove_connection(cmf::water::flux_node::ptr To)

Remove the connection. ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::flux_to "real
flux_to(const cmf::water::flux_node &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from target into this\"). ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::flux3d_to "cmf::geometry::point flux3d_to(const cmf::water::flux_node &target,
cmf::math::Time t) ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::get_3d_flux "cmf::geometry::point get_3d_flux(cmf::math::Time t) ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::water_balance "real water_balance(cmf::math::Time t, const flux_connection
*Without=0) const

Returns the sum of all fluxes (positive and negative) at time t.
Single fluxes can be excluded from the calculation

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the water_balance
(e.g. to prevent closed circuits) ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::get_potential "virtual real get_potential() const

Returns the water potential of the node in m waterhead The base class
water storage always returns the height of the location ";

%feature("docstring")  cmf::atmosphere::IDWRainfall::set_potential "virtual void set_potential(real new_potential) ";


// File: classcmf_1_1math_1_1_implicit_euler.xml
%feature("docstring") cmf::math::ImplicitEuler "

An implicit (backward) Euler integrator using fixpoint iteration.

C++ includes: implicit_euler.h ";

/*  Internal data storages  */

/*

*/

/*  The state variables to integrate  */

/*

*/

%feature("docstring")  cmf::math::ImplicitEuler::CopyStates "void
CopyStates(num_array &destination) const

Copies the states to a numeric vector using use_OpenMP. ";

%feature("docstring")  cmf::math::ImplicitEuler::CopyStates "void
CopyStates(real *destination) const ";

%feature("docstring")  cmf::math::ImplicitEuler::SetStates "void
SetStates(const num_array &newStates)

Copies the new states to the actual states. ";

%feature("docstring")  cmf::math::ImplicitEuler::SetStates "void
SetStates(real *newStates) ";

%feature("docstring")  cmf::math::ImplicitEuler::CopyDerivs "void
CopyDerivs(Time time, num_array &destination, real factor=1) const

Copies the derivatives at time step \"time\" to a numeric vector using
use_OpenMP

Parameters:
-----------

time:   Time at which the derivatives should be calculated

destination:  Vector to be overwritten by the results

factor:  A factor that is multiplied to the derivate (e.g. unit
conversion or integration length) ";

%feature("docstring")  cmf::math::ImplicitEuler::CopyDerivs "void
CopyDerivs(Time time, real *destination, real factor=1) const

Copies the derivatives at time step \"time\" to an preallocated c
array

Parameters:
-----------

time:   Time at which the derivatives should be calculated

destination:  Allocated c array

factor:  A factor that is multiplied to the derivate (e.g. unit
conversion or integration length) ";

%feature("docstring")  cmf::math::ImplicitEuler::GetStates "num_array
GetStates() const

Returns the states in a numeric vector using :CopyStates, but is
slower because of additional memory allocation. ";

%feature("docstring")  cmf::math::ImplicitEuler::GetDerivs "num_array
GetDerivs(Time time) const

Returns the derivatives at time step \"time\" in a numeric vector
using :CopyDerivs, but is slower because of additional memory
allocation. ";

%feature("docstring")  cmf::math::ImplicitEuler::AddValuesToStates "void AddValuesToStates(const num_array &operands) ";

%feature("docstring")  cmf::math::ImplicitEuler::AddState "virtual
void AddState(cmf::math::StateVariable::ptr state) ";

/*  model time  */

/*

*/

%feature("docstring")  cmf::math::ImplicitEuler::ModelTime "cmf::math::Time ModelTime() const

Returns the current model time. ";

%feature("docstring")  cmf::math::ImplicitEuler::ModelTime "void
ModelTime(cmf::math::Time val)

Sets the current model time. ";

%feature("docstring")  cmf::math::ImplicitEuler::TimeStep "cmf::math::Time TimeStep() const

Returns the last time step. ";

%feature("docstring")  cmf::math::ImplicitEuler::NextTimeStep "cmf::math::Time NextTimeStep() const

Returns the next time step width. ";

%feature("docstring")  cmf::math::ImplicitEuler::NextTimeStep "void
NextTimeStep(cmf::math::Time val)

Sets the next time step width. ";

%feature("docstring")  cmf::math::ImplicitEuler::MinTimestep "const
cmf::math::Time MinTimestep() const

Returns The minimal allowed time step length. ";

%feature("docstring")  cmf::math::ImplicitEuler::Iterations "int
Iterations() const ";

%feature("docstring")  cmf::math::ImplicitEuler::ResetIterations "void ResetIterations() ";

%feature("docstring")  cmf::math::ImplicitEuler::Reset "virtual void
Reset() ";

/*  Constructors and Destructors  */

/*

*/

/*  Integrate  */

/*

*/

%feature("docstring")  cmf::math::ImplicitEuler::Integrate "int
Integrate(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::ImplicitEuler::IntegrateUntil "void
IntegrateUntil(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::ImplicitEuler::IntegrateUntil "void
IntegrateUntil(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables until MaxTime. ";

/*  Accuracy parameters  */

/*

*/

%feature("docstring")  cmf::math::ImplicitEuler::AddStatesFromOwner "void AddStatesFromOwner(cmf::math::StateVariableOwner &stateOwner) ";

%feature("docstring")  cmf::math::ImplicitEuler::ImplicitEuler "ImplicitEuler(StateVariableOwner &states, real epsilon=1e-9,
cmf::math::Time tStepMin=Time::Seconds(10))

Constructs a new FixPointImplicitEuler from a pointer to a vector of
state variables The Integrator becomes the owner of states

Parameters:
-----------

states:  Statevariables of the system

epsilon:  relative error tolerance per time step (default=1e-9)

tStepMin:  minimum time step (default=10s) ";

%feature("docstring")  cmf::math::ImplicitEuler::ImplicitEuler "ImplicitEuler(real epsilon=1e-9, cmf::math::Time
tStepMin=Time::Seconds(10))

Constructs a new FixPointImplicitEuler

Parameters:
-----------

epsilon:  relative error tolerance per time step (default=1e-9)

tStepMin:  minimum time step (default=10s) ";

%feature("docstring")  cmf::math::ImplicitEuler::ImplicitEuler "ImplicitEuler(const Integrator &)

Copy constructor. ";

%feature("docstring")  cmf::math::ImplicitEuler::Copy "virtual
Integrator* Copy() const

Returns a new Integrator, based on this (without the state variables),
e.g. same type, epsilon, model time etc. ";

%feature("docstring")  cmf::math::ImplicitEuler::Integrate "int
Integrate(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables

Parameters:
-----------

MaxTime:  To stop the model (if running in a model framework) at time
steps of value exchange e.g. full hours, the next value exchange time
can be given

TimeStep:  Takes the proposed timestep, and changes it into the
effictivly used timestep according to the local stiffness of the
problem and MaxTime ";

%feature("docstring")  cmf::math::ImplicitEuler::size "int size()
const

returns the number of state variables ";

%feature("docstring")  cmf::math::ImplicitEuler::get_state "real
get_state(int position) const

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::ImplicitEuler::set_state "void
set_state(int position, real newState)

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::ImplicitEuler::get_states "state_queue get_states()

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";


// File: structcmf_1_1math_1_1integratable.xml
%feature("docstring") cmf::math::integratable "";

%feature("docstring")  cmf::math::integratable::integratable "integratable(real _x, Time _t) ";

%feature("docstring")  cmf::math::integratable::integratable "integratable(const integratable &cpy) ";

%feature("docstring")  cmf::math::integratable::integratable "integratable() ";

%feature("docstring")  cmf::math::integratable::integrate "void
integrate(real dxdt, Time dt) ";

%feature("docstring")  cmf::math::integratable::integrate_until "void
integrate_until(real dxdt, Time until) ";


// File: classcmf_1_1math_1_1_integrator.xml
%feature("docstring") cmf::math::Integrator "

Base class for any kind of integrator

Pure virtual functions: Integrate

Copy Please provide a custom copy constructorTodo Put the methods of
StateVariableVector here, and delete StateVariableVector

C++ includes: integrator.h ";

/*  The state variables to integrate  */

/*

*/

%feature("docstring")  cmf::math::Integrator::CopyStates "void
CopyStates(num_array &destination) const

Copies the states to a numeric vector using use_OpenMP. ";

%feature("docstring")  cmf::math::Integrator::CopyStates "void
CopyStates(real *destination) const ";

%feature("docstring")  cmf::math::Integrator::SetStates "void
SetStates(const num_array &newStates)

Copies the new states to the actual states. ";

%feature("docstring")  cmf::math::Integrator::SetStates "void
SetStates(real *newStates) ";

%feature("docstring")  cmf::math::Integrator::CopyDerivs "void
CopyDerivs(Time time, num_array &destination, real factor=1) const

Copies the derivatives at time step \"time\" to a numeric vector using
use_OpenMP

Parameters:
-----------

time:   Time at which the derivatives should be calculated

destination:  Vector to be overwritten by the results

factor:  A factor that is multiplied to the derivate (e.g. unit
conversion or integration length) ";

%feature("docstring")  cmf::math::Integrator::CopyDerivs "void
CopyDerivs(Time time, real *destination, real factor=1) const

Copies the derivatives at time step \"time\" to an preallocated c
array

Parameters:
-----------

time:   Time at which the derivatives should be calculated

destination:  Allocated c array

factor:  A factor that is multiplied to the derivate (e.g. unit
conversion or integration length) ";

%feature("docstring")  cmf::math::Integrator::GetStates "num_array
GetStates() const

Returns the states in a numeric vector using :CopyStates, but is
slower because of additional memory allocation. ";

%feature("docstring")  cmf::math::Integrator::GetDerivs "num_array
GetDerivs(Time time) const

Returns the derivatives at time step \"time\" in a numeric vector
using :CopyDerivs, but is slower because of additional memory
allocation. ";

%feature("docstring")  cmf::math::Integrator::AddValuesToStates "void
AddValuesToStates(const num_array &operands) ";

%feature("docstring")  cmf::math::Integrator::AddStatesFromOwner "virtual void AddStatesFromOwner(cmf::math::StateVariableOwner
&stateOwner) ";

%feature("docstring")  cmf::math::Integrator::AddState "virtual void
AddState(cmf::math::StateVariable::ptr state) ";

/*  Accuracy parameters  */

/*

*/

/*  model time  */

/*

*/

%feature("docstring")  cmf::math::Integrator::ModelTime "cmf::math::Time ModelTime() const

Returns the current model time. ";

%feature("docstring")  cmf::math::Integrator::ModelTime "void
ModelTime(cmf::math::Time val)

Sets the current model time. ";

%feature("docstring")  cmf::math::Integrator::TimeStep "cmf::math::Time TimeStep() const

Returns the last time step. ";

%feature("docstring")  cmf::math::Integrator::NextTimeStep "cmf::math::Time NextTimeStep() const

Returns the next time step width. ";

%feature("docstring")  cmf::math::Integrator::NextTimeStep "void
NextTimeStep(cmf::math::Time val)

Sets the next time step width. ";

%feature("docstring")  cmf::math::Integrator::MinTimestep "const
cmf::math::Time MinTimestep() const

Returns The minimal allowed time step length. ";

%feature("docstring")  cmf::math::Integrator::Iterations "int
Iterations() const ";

%feature("docstring")  cmf::math::Integrator::ResetIterations "void
ResetIterations() ";

%feature("docstring")  cmf::math::Integrator::Reset "virtual void
Reset() ";

/*  Constructors and Destructors  */

/*

*/

%feature("docstring")  cmf::math::Integrator::Integrator "Integrator(real epsilon=1e-9, cmf::math::Time
tStepMin=Time::Seconds(10))

Constructs a new Integrator with a new own state vector

Parameters:
-----------

epsilon:  relative error tolerance per time step (default=1e-9)

tStepMin:  minimum time step (default=10s) ";

%feature("docstring")  cmf::math::Integrator::Integrator "Integrator(cmf::math::StateVariableOwner &states, real epsilon=1e-9,
cmf::math::Time tStepMin=cmf::math::sec *10.0) ";

%feature("docstring")  cmf::math::Integrator::Integrator "Integrator(const Integrator &forCopy)

Copy constructor, does not copy the state variables. ";

%feature("docstring")  cmf::math::Integrator::Copy "virtual
cmf::math::Integrator* Copy() const =0

Returns a new Integrator, based on this (without the state variables),
e.g. same type, epsilon, model time etc. ";

/*  Integrate  */

/*

*/

%feature("docstring")  cmf::math::Integrator::Integrate "virtual int
Integrate(cmf::math::Time MaxTime, cmf::math::Time TimeStep)=0

Integrates the vector of state variables

Parameters:
-----------

MaxTime:  To stop the model (if running in a model framework) at time
steps of value exchange e.g. full hours, the next value exchange time
can be given

TimeStep:  Takes the proposed timestep, and changes it into the
effictivly used timestep according to the local stiffness of the
problem and MaxTime ";

%feature("docstring")  cmf::math::Integrator::Integrate "int
Integrate(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::Integrator::IntegrateUntil "void
IntegrateUntil(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::Integrator::IntegrateUntil "void
IntegrateUntil(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables until MaxTime. ";

%feature("docstring")  cmf::math::Integrator::size "int size() const

returns the number of state variables ";

%feature("docstring")  cmf::math::Integrator::get_state "real
get_state(int position) const

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::Integrator::set_state "void
set_state(int position, real newState)

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::Integrator::get_states "state_queue
get_states()

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";


// File: classcmf_1_1river_1_1_i_volume_height_function.xml
%feature("docstring") cmf::river::IVolumeHeightFunction "

Volume height relations are functional objects, which return a height
and a crosssectional area of a volume for different geometric bodies.
This is the abstract base class, where the geometries derive from

C++ includes: ReachType.h ";

%feature("docstring")  cmf::river::IVolumeHeightFunction::h "virtual
double h(double V) const =0

Returns the depth of a given volume. ";

%feature("docstring")  cmf::river::IVolumeHeightFunction::A "virtual
double A(double V) const =0

Returns the area of the surface for a given volume. ";

%feature("docstring")  cmf::river::IVolumeHeightFunction::V "virtual
double V(double h) const =0 ";

%feature("docstring")  cmf::river::IVolumeHeightFunction::copy "virtual IVolumeHeightFunction* copy() const =0 ";


// File: classcmf_1_1water_1_1kinematic__wave.xml
%feature("docstring") cmf::water::kinematic_wave "

Calculates flux out of a storage as a linear function of its volume to
a power

\\\\[ q = \\\\frac {V_{mobile}^\\\\beta}{t_r} \\\\] where:
$V_{mobile} [m^3] = V - V_{residual}$ the stored mobile volume

$\\\\beta [-]$ An empirical exponent to shape the flux function

$t_r [days]$ The residence time of the water in this storage in days

C++ includes: WaterStorage.h ";

%feature("docstring")  cmf::water::kinematic_wave::kinematic_wave "kinematic_wave(WaterStorage::ptr source, flux_node::ptr target, real
residencetime, real exponent=1.0, real residual_volume=0.0)

Creates a kinematic wave connection \\\\[ q = \\\\frac {\\\\left(V -
V_{residual}\\\\right)^\\\\beta}{t_r} \\\\]

Parameters:
-----------

source:  Water storage from which the water flows out. Flux is a
function of source.volume

target:  Target node (boundary condition or storage). Does not
influence the strength of the flow

residencetime:   $t_r [days]$ The residence time of the water in this
storage

exponent:   $\\\\beta [-]$ An empirical exponent to shape the flux
function (default = 1 (linear function))

residual_volume:   $V_{residual} [m^3]$ The volume of water not
flowing out (default = 0 m3) ";

%feature("docstring")  cmf::water::kinematic_wave::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::water::kinematic_wave::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::water::kinematic_wave::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::water::kinematic_wave::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::water::kinematic_wave::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::water::kinematic_wave::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")  cmf::water::kinematic_wave::exchange_target "void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")  cmf::water::kinematic_wave::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")  cmf::water::kinematic_wave::get_tracer_filter "real get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")  cmf::water::kinematic_wave::set_tracer_filter "void set_tracer_filter(real value) ";

%feature("docstring")  cmf::water::kinematic_wave::to_string "std::string to_string() const ";

%feature("docstring")  cmf::water::kinematic_wave::short_string "std::string short_string() const ";


// File: classcmf_1_1upslope_1_1connections_1_1lateral__sub__surface__flux.xml
%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux "

An abstract base class for lateral subsurface fluxes

C++ includes: subsurfacefluxes.h ";

%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux::exchange_target "void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux::get_tracer_filter
"real get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux::set_tracer_filter
"void set_tracer_filter(real value) ";

%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux::to_string "std::string to_string() const ";

%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux::short_string "std::string short_string() const ";


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

%feature("docstring")  cmf::upslope::layer_list::begin "iterator
begin() ";

%feature("docstring")  cmf::upslope::layer_list::begin "const_iterator begin() const ";

%feature("docstring")  cmf::upslope::layer_list::end "iterator end()
";

%feature("docstring")  cmf::upslope::layer_list::end "const_iterator
end() const ";

%feature("docstring")  cmf::upslope::layer_list::pop "SoilLayer::ptr
pop()

Deletes the last layer from the list and returns it. ";

%feature("docstring")  cmf::upslope::layer_list::append "layer_list&
append(SoilLayer::ptr l)

Appends a soil layer to the list. ";

%feature("docstring")  cmf::upslope::layer_list::append "layer_list&
append(const layer_list &ll)

Appends all soil layers of layer_list to this. ";

%feature("docstring")  cmf::upslope::layer_list::append "layer_list&
append(const cmf::water::node_list &nl)

Appends all soil layers from a node_list to this. ";

%feature("docstring")  cmf::upslope::layer_list::get_slice "layer_list get_slice(size_t first=0, size_t last=1000000, size_t
step=1) ";

%feature("docstring")  cmf::upslope::layer_list::clear "void clear()

Clears the list. ";

%feature("docstring")  cmf::upslope::layer_list::size "size_t size()
const

Number of layers in the list. ";

%feature("docstring")  cmf::upslope::layer_list::set_wetness "void
set_wetness(const cmf::math::num_array &Value, size_t offset=0)

Sets the wetness in m3/m3 of layers [offset : arraysize]. ";

%feature("docstring")  cmf::upslope::layer_list::get_potential "cmf::math::num_array get_potential() const

Returns an array containing the potentials of all layers in the list.
";

%feature("docstring")  cmf::upslope::layer_list::set_potential "void
set_potential(const cmf::math::num_array &Value, size_t offset=0)

Sets the potential (head) in m of layers [offset : arraysize]. ";

%feature("docstring")  cmf::upslope::layer_list::get_wetness "cmf::math::num_array get_wetness() const

Returns an array containing the wetness in m3 H2O/m3 pore space of all
layers in the list. ";

%feature("docstring")  cmf::upslope::layer_list::set_volume "void
set_volume(const cmf::math::num_array &Value, size_t offset=0)

Sets the Volume in m3 of layers [offset : arraysize]. ";

%feature("docstring")  cmf::upslope::layer_list::get_matrix_potential
"cmf::math::num_array get_matrix_potential() const

Returns an array containing the matrix potential in m of all layers in
the list. ";

%feature("docstring")  cmf::upslope::layer_list::get_porosity "cmf::math::num_array get_porosity() const

Returns an array containing the porosity in m3/m3 of all layers in the
list. ";

%feature("docstring")  cmf::upslope::layer_list::get_volume "cmf::math::num_array get_volume() const

Returns an array containing the volume in m3 of stored water of all
layers in the list. ";

%feature("docstring")  cmf::upslope::layer_list::get_thickness "cmf::math::num_array get_thickness() const

Returns an array containing the thickness in m of all layers in the
list. ";

%feature("docstring")
cmf::upslope::layer_list::get_gravitational_potential "cmf::math::num_array get_gravitational_potential() const

Returns an array containing the gravitational potential in m of all
layers in the list. ";

%feature("docstring")  cmf::upslope::layer_list::get_K "cmf::math::num_array get_K() const

Returns an array containing the conductivity in m/day of all layers in
the list. ";

%feature("docstring")  cmf::upslope::layer_list::get_Ksat "cmf::math::num_array get_Ksat() const

Returns an array containing the saturated conductivity in m/day of all
layers in the list. ";

%feature("docstring")  cmf::upslope::layer_list::get_lower_boundary "cmf::math::num_array get_lower_boundary() const

Returns an array containing the lower boundary in m below surface of
all layers in the list. ";

%feature("docstring")  cmf::upslope::layer_list::get_upper_boundary "cmf::math::num_array get_upper_boundary() const

Returns an array containing the upper boundary in m below surface of
all layers in the list. ";

%feature("docstring")  cmf::upslope::layer_list::get_ice_fraction "cmf::math::num_array get_ice_fraction() const ";

%feature("docstring")  cmf::upslope::layer_list::set_ice_fraction "void set_ice_fraction(const cmf::math::num_array &Value, size_t
offset=0) ";


// File: classcmf_1_1water_1_1linear__scale.xml
%feature("docstring") cmf::water::linear_scale "

A linear scaling functor, with slope and displacement.

C++ includes: boundary_condition.h ";

%feature("docstring")  cmf::water::linear_scale::linear_scale "linear_scale(real _slope=1, real _displacement=0)

Creates a linear scale (by default it is a unity scale, $a=1; b=0$).
";


// File: classcmf_1_1upslope_1_1_linear_retention.xml
%feature("docstring") cmf::upslope::LinearRetention "

The linear retention curve provides a simple linear relationship
between storage and head

Head function (head in m, calculated from upper side control volume)
\\\\[ h(\\\\theta) = -\\\\Delta z \\\\left( 1 - \\\\frac{\\\\theta -
\\\\theta_r}{\\\\theta_s - \\\\theta_r} \\\\right) \\\\] Conductivity
function \\\\[ K(\\\\theta) = K_{sat} \\\\left(\\\\frac{\\\\theta -
\\\\theta_r}{\\\\theta_s - \\\\theta_r}\\\\right)^\\\\beta \\\\]

C++ includes: RetentionCurve.h ";

%feature("docstring")  cmf::upslope::LinearRetention::MatricPotential
"virtual real MatricPotential(real wetness) const

Returns the potential below upper side of the control volume in m. ";

%feature("docstring")  cmf::upslope::LinearRetention::Wetness "virtual real Wetness(real suction) const

Returns the wetness (water content per pore volume) at a given head.
";

%feature("docstring")  cmf::upslope::LinearRetention::K "virtual real
K(real wetness) const

Returns the conductivity in m/day at a certain depth and water
content. ";

%feature("docstring")  cmf::upslope::LinearRetention::VoidVolume "virtual real VoidVolume(real upperDepth, real lowerDepth, real Area)
const

Returns the void volume of a soil column. ";

%feature("docstring")  cmf::upslope::LinearRetention::Transmissivity "virtual real Transmissivity(real upperDepth, real lowerDepth, real
wetness) const ";

%feature("docstring")  cmf::upslope::LinearRetention::Porosity "virtual real Porosity(real depth) const

Returns the porosity at a certain depth. ";

%feature("docstring")  cmf::upslope::LinearRetention::FillHeight "virtual real FillHeight(real lowerDepth, real Area, real Volume) const

Returns the thickness of a soil column with a certain pore volume. ";

%feature("docstring")  cmf::upslope::LinearRetention::copy "LinearRetention* copy() const ";

%feature("docstring")  cmf::upslope::LinearRetention::LinearRetention
"LinearRetention(real _Ksat, real _Phi, real _thickness) ";

%feature("docstring")  cmf::upslope::LinearRetention::Wetness_eff "virtual real Wetness_eff(real wetness, real pF_r=4.2) const

Returns the effective wetness, using a residual pF value \\\\[w_{eff}
=
\\\\frac{w_{act}-w\\\\left(pF_r\\\\right)}{1-w\\\\left(pF_r\\\\right)}\\\\]
";

%feature("docstring")  cmf::upslope::LinearRetention::Wetness_pF "real Wetness_pF(real pF) const

returns the volumetric water content at a given pF value ";


// File: classcmf_1_1geometry_1_1_locatable.xml
%feature("docstring") cmf::geometry::Locatable "

An interface for objects having a location in space.

C++ includes: geometry.h ";

%feature("docstring")  cmf::geometry::Locatable::get_position "virtual cmf::geometry::point get_position() const =0

Pure virtual function. Should return the position of the locatable. ";

%feature("docstring")  cmf::geometry::Locatable::set_position "virtual void set_position(cmf::geometry::point p)

Sets the location. If not implemented by the child class, an exception
is thrown. ";

%feature("docstring")  cmf::geometry::Locatable::get_distance_to "double get_distance_to(const Locatable &cmp)

Returns the distance between two locatable objects. ";

%feature("docstring")  cmf::geometry::Locatable::get_direction_to "cmf::geometry::point get_direction_to(const Locatable &cmp)

Returns a vector with length=1 pointing in the direction of another
Locatable. ";


// File: classcmf_1_1geometry_1_1_location.xml
%feature("docstring") cmf::geometry::Location "

A minimal implementation of Locatable.

C++ includes: geometry.h ";

%feature("docstring")  cmf::geometry::Location::get_position "cmf::geometry::point get_position() const

Pure virtual function. Should return the position of the locatable. ";

%feature("docstring")  cmf::geometry::Location::Location "Location(cmf::geometry::point position) ";

%feature("docstring")  cmf::geometry::Location::Location "Location(double x, double y, double z) ";

%feature("docstring")  cmf::geometry::Location::set_position "virtual
void set_position(cmf::geometry::point p)

Sets the location. If not implemented by the child class, an exception
is thrown. ";

%feature("docstring")  cmf::geometry::Location::get_distance_to "double get_distance_to(const Locatable &cmp)

Returns the distance between two locatable objects. ";

%feature("docstring")  cmf::geometry::Location::get_direction_to "cmf::geometry::point get_direction_to(const Locatable &cmp)

Returns a vector with length=1 pointing in the direction of another
Locatable. ";


// File: classcmf_1_1atmosphere_1_1log__wind__profile.xml
%feature("docstring") cmf::atmosphere::log_wind_profile "

, A logarithmic wind profileTodo Cite literature for this windprofile
and insert equation

C++ includes: ET.h ";

%feature("docstring")
cmf::atmosphere::log_wind_profile::log_wind_profile "log_wind_profile(cmf::upslope::Cell &_cell) ";

%feature("docstring")
cmf::atmosphere::log_wind_profile::get_aerodynamic_resistance "virtual void get_aerodynamic_resistance(double &r_ag, double &r_ac,
cmf::math::Time t) const

aerodynamic resistance from ground to atmosphere (r_ag) and from
canopy to atmosphere (r_ac) ";


// File: classcmf_1_1river_1_1_manning.xml
%feature("docstring") cmf::river::Manning "

Calculates the flux between two open water bodies, using Manning's
equation

C++ includes: ManningConnection.h ";

%feature("docstring")  cmf::river::Manning::q "real q(const flux_node
&inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::river::Manning::left_node "flux_node::ptr
left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::river::Manning::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::river::Manning::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::river::Manning::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::river::Manning::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")  cmf::river::Manning::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::river::Manning::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")  cmf::river::Manning::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")  cmf::river::Manning::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")  cmf::river::Manning::to_string "std::string
to_string() const ";

%feature("docstring")  cmf::river::Manning::short_string "std::string
short_string() const ";


// File: classcmf_1_1river_1_1_manning___diffusive.xml
%feature("docstring") cmf::river::Manning_Diffusive "

Connecting surface water bodies using a diffusive wave. Not stable
deep water with small gradient

C++ includes: ManningConnection.h ";

%feature("docstring")
cmf::river::Manning_Diffusive::Manning_Diffusive "Manning_Diffusive(cmf::river::OpenWaterStorage::ptr left,
cmf::water::flux_node::ptr right, cmf::river::Channel reachtype) ";

%feature("docstring")  cmf::river::Manning_Diffusive::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::river::Manning_Diffusive::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::river::Manning_Diffusive::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::river::Manning_Diffusive::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::river::Manning_Diffusive::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::river::Manning_Diffusive::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")  cmf::river::Manning_Diffusive::exchange_target
"void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")  cmf::river::Manning_Diffusive::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::river::Manning_Diffusive::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::river::Manning_Diffusive::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")  cmf::river::Manning_Diffusive::to_string "std::string to_string() const ";

%feature("docstring")  cmf::river::Manning_Diffusive::short_string "std::string short_string() const ";


// File: classcmf_1_1river_1_1_manning___kinematic.xml
%feature("docstring") cmf::river::Manning_Kinematic "

Connecting surface water bodies using a kinematic wave. Note the fixed
gradient

C++ includes: ManningConnection.h ";

%feature("docstring")
cmf::river::Manning_Kinematic::Manning_Kinematic "Manning_Kinematic(cmf::river::OpenWaterStorage::ptr left,
cmf::water::flux_node::ptr right, cmf::river::Channel reachtype) ";

%feature("docstring")  cmf::river::Manning_Kinematic::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::river::Manning_Kinematic::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::river::Manning_Kinematic::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::river::Manning_Kinematic::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::river::Manning_Kinematic::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::river::Manning_Kinematic::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")  cmf::river::Manning_Kinematic::exchange_target
"void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")  cmf::river::Manning_Kinematic::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::river::Manning_Kinematic::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::river::Manning_Kinematic::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")  cmf::river::Manning_Kinematic::to_string "std::string to_string() const ";

%feature("docstring")  cmf::river::Manning_Kinematic::short_string "std::string short_string() const ";


// File: classcmf_1_1upslope_1_1connections_1_1_matrix_infiltration.xml
%feature("docstring") cmf::upslope::connections::MatrixInfiltration "

Connects the surfacewater and the most upper layer

If UpslopeCell::InfiltrationExcess and Cell is not saturated \\\\[ K_I
= \\\\min\\\\left(\\\\frac{\\\\rho_{wg} \\\\Delta
z-\\\\Psi_M}{\\\\Delta z \\\\rho_{wg}}
K\\\\left(\\\\theta\\\\right),\\\\sum q_{surface\\\\ in}\\\\right)
\\\\] else \\\\[ K_I = \\\\sum q_{surface\\\\ in} \\\\]

C++ includes: infiltration.h ";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::MatrixInfiltration "MatrixInfiltration(cmf::upslope::SoilLayer::ptr soilwater,
cmf::water::flux_node::ptr surfacewater) ";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::get_tracer_filter "real get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::set_tracer_filter "void set_tracer_filter(real value) ";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::to_string "std::string
to_string() const ";

%feature("docstring")
cmf::upslope::connections::MatrixInfiltration::short_string "std::string short_string() const ";


// File: classcmf_1_1river_1_1_mean_channel.xml
%feature("docstring") cmf::river::MeanChannel "

A combination of two channel geometries.

C++ includes: ReachType.h ";

%feature("docstring")  cmf::river::MeanChannel::get_length "double
get_length() const

Length of the reach. ";

%feature("docstring")  cmf::river::MeanChannel::MeanChannel "MeanChannel(const IChannel &channel1, const IChannel &channel2)

Creates the mean geometry from the two channel geometries. ";

%feature("docstring")  cmf::river::MeanChannel::MeanChannel "MeanChannel(const MeanChannel &meanChannel) ";

%feature("docstring")  cmf::river::MeanChannel::typecode "char
typecode() const ";

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
geometry. In most cases use get_flux_crossection=V/l, where V is the
stored volume and l is the reach length.

Wetted area of a river cross section [m2]

Parameters:
-----------

depth:  get_depth of the reach [m] ";

%feature("docstring")  cmf::river::MeanChannel::get_wetted_perimeter "double get_wetted_perimeter(double depth) const

Calculates the wetted perimeter from a given actual depth [m] using
the actual IChannel geometry. ";

%feature("docstring")  cmf::river::MeanChannel::copy "MeanChannel*
copy() const ";

%feature("docstring")  cmf::river::MeanChannel::get_nManning "virtual
double get_nManning() const ";

%feature("docstring")  cmf::river::MeanChannel::set_nManning "virtual
void set_nManning(double val) ";

%feature("docstring")  cmf::river::MeanChannel::h "virtual double
h(double V) const

Returns the depth of a given volume. ";

%feature("docstring")  cmf::river::MeanChannel::A "virtual double
A(double V) const

Returns the area of the surface for a given volume. ";

%feature("docstring")  cmf::river::MeanChannel::V "virtual double
V(double h) const ";

%feature("docstring")  cmf::river::MeanChannel::qManning "double
qManning(double A, double slope) const

Calculates the flow rate from a given water volume in the reach
\\\\begin{eqnarray*} /// q_{Manning}&=& A R^{\\\\frac 2 3}
\\\\sqrt{\\\\frac {\\\\Delta_z} n} \\\\\\\\ /// A &=& \\\\frac V l
\\\\mbox{, (Crosssectional area of the wetted crossection, Volume per
length)} \\\\\\\\ /// R &=& \\\\frac A {P(d)} \\\\\\\\ /// P(d) &=&
\\\\mbox{ the perimeter of the wetted crosssection, a function of
reach depth} \\\\\\\\ /// d(V) &=& \\\\mbox{ the depth of the reach a
function of the volume} \\\\\\\\ /// \\\\Delta_z &=& \\\\frac{z_{max}
- z_{min}}{l} \\\\mbox{ Slope of the reach} /// \\\\end{eqnarray*}
Flow rate [m3/s]

Parameters:
-----------

A:  The area of the cross section [m2]

slope:  The slope of the reach [m/m] ";


// File: classcmf_1_1atmosphere_1_1_meteorology.xml
%feature("docstring") cmf::atmosphere::Meteorology "

An abstract class, for objects generating Weather records at a
specific time.

C++ includes: meteorology.h ";

%feature("docstring")  cmf::atmosphere::Meteorology::get_weather "virtual cmf::atmosphere::Weather get_weather(cmf::math::Time t) const
=0

Returns the Weather at time t. Pure virtual function. Must get
implemented by child functions. ";

%feature("docstring")  cmf::atmosphere::Meteorology::copy "virtual
Meteorology* copy() const =0

Returns a copy of the meteorology object. Pure virtual function, needs
to be implemented. ";

%feature("docstring")
cmf::atmosphere::Meteorology::get_instrument_height "virtual real
get_instrument_height() const =0 ";


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

There are two modes for the meteorology: daily=true and daily=false.
If daily=true, Radiation is given as a daily mean value. If
daily=false, Radiation is given as an hourly mean value, which shows
the dial ETpot variation but results in erronous results if the
timestep is daily.

In the following, we will assume a meteorological station with given
Tmin, Tmax, precipitation and daily mean relative humidity. To use
other meteorological data, please consult the description of the
MeteoStation class in the API documentationCreating a meteorological
station

import CMFlib as cmf latitude=51.2 # Latitude of station in decimal
degrees longitude=8.1 # Longitude of station in decimal degrees (only
needed for daily=false)                     timezone=1    # Timezone,
pos. values mean east of GMT, negative west (Germany=1, Pacific
time=-8, only needed for daily=false) start=cmf.Time(1,1,2001) #
Creates all timeseries with this start time, one can change them later
step=cmf.day # s. start                     name=\"Giessen\"
# A name for the station (optional)
meteo=cmf.MeteoStation(latitude,longitude,timezone,start,step,name)

The daily flag is automatically set to true, since the step width is
&ge cmf.dayLoading data into the meteorological station

# MeteoData.txt is tab seperated file containing # Tmin [deg C],Tmax
[deg C],rHmean [%] and precipitation [mm/day] values for every day
f=file('MeteoData.txt') for line in file:
meteo.Tmin.Add(float(line.split('\\\\t')[0]))
meteo.Tmax.Add(float(line.split('\\\\t')[1]))
meteo.rHmean.Add(float(line.split('\\\\t')[2]))
meteo.Prec.Add(float(line.split('\\\\t')[3]))Using a meteorological
station

weather=meteo.get_data(cmf.Time(3,2,2009,14)) # Weather at Feb. 3rd,
2009, 2pm                     print 'Global Radiation: ',weather.Rs
# Daily mean Rs, since daily=true print 'Temperature:',weather.T
# Daily mean T, since nothing else in known

C++ includes: meteorology.h ";

/*  Location and behaviour properties  */

/*

*/

%feature("docstring")  cmf::atmosphere::MeteoStation::get_position "virtual cmf::geometry::point get_position() const

Pure virtual function. Should return the position of the locatable. ";

/*  Timeseries of meteorological data  */

/*

*/

/*  Data access methods  */

/*

*/

%feature("docstring")  cmf::atmosphere::MeteoStation::get_data "cmf::atmosphere::Weather get_data(cmf::math::Time t, double height)
const

Returns the current Atmosphere state. Uses default values for missing
timeseries. ";

%feature("docstring")  cmf::atmosphere::MeteoStation::use_for_cell "void use_for_cell(cmf::upslope::Cell &c) ";

%feature("docstring")
cmf::atmosphere::MeteoStation::SetSunshineFraction "void
SetSunshineFraction(cmf::math::timeseries sunshine_duration)

Returns the global radiation at a given time step $ R_s
\\\\frac{MJ}{m^2day}$,
seehttp://www.fao.org/docrep/X0490E/x0490e07.htm#radiation
\\\\begin{eqnarray*} \\\\phi &=& \\\\frac{(\\\\mbox{geogr.
Latitude})^\\\\circ \\\\pi}{180^\\\\circ} \\\\mbox{ Latitude in }rad
\\\\\\\\ \\\\delta &=& 0.409 \\\\sin\\\\left(\\\\frac{2\\\\pi}{365}DOY
- 1.39\\\\right) \\\\mbox{ Declination, DOY is day of year}\\\\\\\\
\\\\omega_s &=& \\\\arccos(-\\\\tan\\\\phi\\\\tan\\\\delta) \\\\mbox{
Sunset angle} \\\\\\\\ G_{sc} &=& 0.0802 \\\\frac{MJ}{m^2min}
\\\\mbox{Solar constant} \\\\\\\\ d_r &=& 1+0.033
\\\\cos\\\\left(\\\\frac{2\\\\pi}{365}DOY\\\\right) \\\\mbox{Inverse
relative distance Earth-Sun} \\\\\\\\ b &=&
\\\\frac{2\\\\pi(DOY-81)}{364}\\\\\\\\ S_c &=&
0.1645\\\\sin(2b)-0.1255\\\\cos(b)-0.025\\\\sin(b) \\\\mbox{ Seasonal
correction for solar time} \\\\\\\\ \\\\omega &=& \\\\frac {\\\\pi}
{12} \\\\left(t_h+\\\\frac{(\\\\mbox{geogr.
Longitude})^\\\\circ}{15}-\\\\mbox{Timezone}+S_c-12\\\\right)
\\\\mbox{ solar time in }rad \\\\\\\\ \\\\mbox{If daily} \\\\\\\\ R_a
&=& \\\\frac{24\\\\ 60}{\\\\pi}G_{sc}\\\\ d_r \\\\left(\\\\omega_s
\\\\sin\\\\phi \\\\sin\\\\delta + \\\\cos\\\\phi \\\\cos\\\\delta
\\\\sin\\\\omega_s\\\\right) \\\\mbox{Extraterrestrial radiation }
\\\\frac{MJ}{m^2 day} \\\\\\\\ \\\\mbox{If hourly} \\\\\\\\ R_a &=&
\\\\frac{12\\\\ 24\\\\ 60}{\\\\pi}G_{sc}\\\\ d_r
\\\\left(\\\\left(\\\\omega^+ -\\\\omega^-\\\\right) \\\\sin\\\\phi
\\\\sin\\\\delta + \\\\cos\\\\phi \\\\cos\\\\delta
\\\\left(\\\\sin\\\\omega^+ - \\\\sin\\\\omega^-\\\\right)\\\\right)
\\\\\\\\ && \\\\omega^+,\\\\omega^- = \\\\omega
\\\\pm\\\\frac{\\\\pi}{24} \\\\\\\\ \\\\frac n N &=&
\\\\mbox{Fractional sunshine duration} \\\\\\\\ R_s &=&
\\\\left(0.25+\\\\left(0.5+2\\\\
10^{-5}z\\\\right)\\\\frac{n}{N}\\\\right)R_a \\\\mbox{Global
radiation in }\\\\frac{MJ}{m^2 day} \\\\\\\\ && z=\\\\mbox{Height
a.s.l. in }m \\\\\\\\ \\\\end{eqnarray*} double
get_global_radiation(cmf::math::Time t,double height,double
sunshine_fraction) const;

Calculates a timeseries of the sunshine fraction (to put into
Sunshine) from a timeseries of absolute sunshine duration, using the
potential sunshine duration in hours,
seehttp://www.fao.org/docrep/X0490E/x0490e07.htm#radiation
\\\\begin{eqnarray*} \\\\phi &=& \\\\frac{(\\\\mbox{geogr.
Latitude})^\\\\circ \\\\pi}{180^\\\\circ} \\\\mbox{ Latitude in }rad
\\\\\\\\ \\\\delta &=& 0.409 \\\\sin\\\\left(\\\\frac{2\\\\pi}{365}DOY
- 1.39\\\\right) \\\\mbox{ Declination, DOY is day of year}\\\\\\\\
\\\\omega_s &=& \\\\arccos(-\\\\tan\\\\phi\\\\tan\\\\delta) \\\\mbox{
Sunset angle} \\\\\\\\ N &=& \\\\frac{24}{\\\\pi}\\\\omega_s \\\\mbox{
potential duration of sunshine in }h \\\\\\\\ \\\\frac n N &&
n\\\\mbox{Absolute sunshine duration} \\\\end{eqnarray*} ";

%feature("docstring")  cmf::atmosphere::MeteoStation::MeteoStation "MeteoStation(const cmf::atmosphere::MeteoStation &other) ";

%feature("docstring")  cmf::atmosphere::MeteoStation::set_position "virtual void set_position(cmf::geometry::point p)

Sets the location. If not implemented by the child class, an exception
is thrown. ";

%feature("docstring")  cmf::atmosphere::MeteoStation::get_distance_to
"double get_distance_to(const Locatable &cmp)

Returns the distance between two locatable objects. ";

%feature("docstring")  cmf::atmosphere::MeteoStation::get_direction_to
"cmf::geometry::point get_direction_to(const Locatable &cmp)

Returns a vector with length=1 pointing in the direction of another
Locatable. ";


// File: classcmf_1_1atmosphere_1_1_meteo_station_list.xml
%feature("docstring") cmf::atmosphere::MeteoStationList "

A list of meteorological stations.

Can find the nearest station for a position and calculate the
temperature lapse

C++ includes: meteorology.h ";

%feature("docstring")  cmf::atmosphere::MeteoStationList::size "int
size() const

Returns the number of stations. ";

%feature("docstring")
cmf::atmosphere::MeteoStationList::calculate_Temp_lapse "double
calculate_Temp_lapse(cmf::math::Time begin, cmf::math::Time step,
cmf::math::Time end)

Calculates the temperature lapse from all stations in the list and
sets the T_lapse attribute of each station.

Returns the average lapse over the whole period. ";

%feature("docstring")  cmf::atmosphere::MeteoStationList::add_station
"MeteoStation::ptr add_station(std::string name, double latitude=51,
double longitude=8, double timezone=1, double elevation=0,
cmf::math::Time startTime=cmf::math::Time(1, 1, 2001), cmf::math::Time
timestep=cmf::math::day)

Creates a meteorological station and adds it to the list. ";

%feature("docstring")  cmf::atmosphere::MeteoStationList::add_station
"MeteoStation::ptr add_station(std::string name, cmf::geometry::point
position, double latitude=51, double longitude=8, double timezone=1,
cmf::math::Time startTime=cmf::math::Time(1, 1, 2001), cmf::math::Time
timestep=cmf::math::day)

Creates a meteorological station at a certain position and adds it to
the list

Parameters:
-----------

name:  Name of the station

position:  The location of the station in map coordinates

latitude:  Latitude of the study area (for solar radiation)

longitude:  Longitude of the study area (for solar time)

timezone:  Time zone of the study area (e.g Germany +1,U.S. Pacific
time -8

startTime:  Date of the beginning of the climatic data (may be changed
for each time series later)

timestep:  Frequency of climatic data (may be changed for each time
series later) ";

%feature("docstring")
cmf::atmosphere::MeteoStationList::remove_station "int
remove_station(int index)

Removes a station and returns the number of remaining references to
the removed station. If the station is deleted, 0 is returned. ";

%feature("docstring")
cmf::atmosphere::MeteoStationList::MeteoStationList "MeteoStationList() ";

%feature("docstring")
cmf::atmosphere::MeteoStationList::MeteoStationList "MeteoStationList(const MeteoStationList &copy) ";

%feature("docstring")
cmf::atmosphere::MeteoStationList::reference_to_nearest "MeteoStationReference reference_to_nearest(const
cmf::geometry::Locatable &position, double z_weight=0) const

Creates a MeteoStationReference from the nearest station to position
at position

The distance is calculated as $ d=\\\\sqrt{(x_{s} - x_{l})^2 + (y_{s}
- y_{l})^2} + \\\\lambda_z\\\\|z_{s} - z_{l}\\\\| $ Where $s$ is the
station and $l$ is the locatable A Meteorology using the data of the
nearest station to position

Parameters:
-----------

position:  The position (any locatable, like e.g. Cell possible) to
look for the station. The reference should be owned by the locatable

z_weight:  The weight of the height difference $\\\\lambda_z$ ";


// File: classcmf_1_1atmosphere_1_1_meteo_station_reference.xml
%feature("docstring") cmf::atmosphere::MeteoStationReference "

A reference to a meteorological station. Returns the weather at a
given time for its place using MeteoStation::T_lapse.

C++ includes: meteorology.h ";

%feature("docstring")
cmf::atmosphere::MeteoStationReference::get_station "MeteoStation::ptr get_station() const

Returns the station referenced. ";

%feature("docstring")
cmf::atmosphere::MeteoStationReference::get_position "cmf::geometry::point get_position() const

Returns the position of the reference. ";

%feature("docstring")
cmf::atmosphere::MeteoStationReference::get_weather "cmf::atmosphere::Weather get_weather(cmf::math::Time t) const

Returns the weather at the time t. ";

%feature("docstring")
cmf::atmosphere::MeteoStationReference::get_instrument_height "real
get_instrument_height() const

Creates a reference for a MeteoStation at a location. ";

%feature("docstring")
cmf::atmosphere::MeteoStationReference::MeteoStationReference "MeteoStationReference(MeteoStation::ptr station, const
cmf::geometry::Locatable &location) ";

%feature("docstring")
cmf::atmosphere::MeteoStationReference::MeteoStationReference "MeteoStationReference(MeteoStation::ptr station, cmf::geometry::point
location) ";

%feature("docstring")
cmf::atmosphere::MeteoStationReference::MeteoStationReference "MeteoStationReference(const MeteoStationReference &copy) ";

%feature("docstring")  cmf::atmosphere::MeteoStationReference::copy "MeteoStationReference* copy() const

Returns a copy of the meteorology object. Pure virtual function, needs
to be implemented. ";

%feature("docstring")
cmf::atmosphere::MeteoStationReference::~MeteoStationReference "virtual ~MeteoStationReference() ";

%feature("docstring")
cmf::atmosphere::MeteoStationReference::set_position "virtual void
set_position(cmf::geometry::point p)

Sets the location. If not implemented by the child class, an exception
is thrown. ";

%feature("docstring")
cmf::atmosphere::MeteoStationReference::get_distance_to "double
get_distance_to(const Locatable &cmp)

Returns the distance between two locatable objects. ";

%feature("docstring")
cmf::atmosphere::MeteoStationReference::get_direction_to "cmf::geometry::point get_direction_to(const Locatable &cmp)

Returns a vector with length=1 pointing in the direction of another
Locatable. ";


// File: classcmf_1_1math_1_1_multi_integrator.xml
%feature("docstring") cmf::math::MultiIntegrator "

The MultiIntegrator is a wrapper for a bunch integrators. The states
of the integrators should not have direct connections over integrator
boundaries.

C++ includes: multiintegrator.h ";

/*  The state variables to integrate  */

/*

*/

%feature("docstring")  cmf::math::MultiIntegrator::CopyStates "void
CopyStates(num_array &destination) const

Copies the states to a numeric vector using use_OpenMP. ";

%feature("docstring")  cmf::math::MultiIntegrator::CopyStates "void
CopyStates(real *destination) const ";

%feature("docstring")  cmf::math::MultiIntegrator::SetStates "void
SetStates(const num_array &newStates)

Copies the new states to the actual states. ";

%feature("docstring")  cmf::math::MultiIntegrator::SetStates "void
SetStates(real *newStates) ";

%feature("docstring")  cmf::math::MultiIntegrator::CopyDerivs "void
CopyDerivs(Time time, num_array &destination, real factor=1) const

Copies the derivatives at time step \"time\" to a numeric vector using
use_OpenMP

Parameters:
-----------

time:   Time at which the derivatives should be calculated

destination:  Vector to be overwritten by the results

factor:  A factor that is multiplied to the derivate (e.g. unit
conversion or integration length) ";

%feature("docstring")  cmf::math::MultiIntegrator::CopyDerivs "void
CopyDerivs(Time time, real *destination, real factor=1) const

Copies the derivatives at time step \"time\" to an preallocated c
array

Parameters:
-----------

time:   Time at which the derivatives should be calculated

destination:  Allocated c array

factor:  A factor that is multiplied to the derivate (e.g. unit
conversion or integration length) ";

%feature("docstring")  cmf::math::MultiIntegrator::GetStates "num_array GetStates() const

Returns the states in a numeric vector using :CopyStates, but is
slower because of additional memory allocation. ";

%feature("docstring")  cmf::math::MultiIntegrator::GetDerivs "num_array GetDerivs(Time time) const

Returns the derivatives at time step \"time\" in a numeric vector
using :CopyDerivs, but is slower because of additional memory
allocation. ";

%feature("docstring")  cmf::math::MultiIntegrator::AddValuesToStates "void AddValuesToStates(const num_array &operands) ";

%feature("docstring")  cmf::math::MultiIntegrator::AddState "virtual
void AddState(cmf::math::StateVariable::ptr state) ";

/*  model time  */

/*

*/

%feature("docstring")  cmf::math::MultiIntegrator::ModelTime "cmf::math::Time ModelTime() const

Returns the current model time. ";

%feature("docstring")  cmf::math::MultiIntegrator::ModelTime "void
ModelTime(cmf::math::Time val)

Sets the current model time. ";

%feature("docstring")  cmf::math::MultiIntegrator::TimeStep "cmf::math::Time TimeStep() const

Returns the last time step. ";

%feature("docstring")  cmf::math::MultiIntegrator::NextTimeStep "cmf::math::Time NextTimeStep() const

Returns the next time step width. ";

%feature("docstring")  cmf::math::MultiIntegrator::NextTimeStep "void
NextTimeStep(cmf::math::Time val)

Sets the next time step width. ";

%feature("docstring")  cmf::math::MultiIntegrator::MinTimestep "const
cmf::math::Time MinTimestep() const

Returns The minimal allowed time step length. ";

%feature("docstring")  cmf::math::MultiIntegrator::Iterations "int
Iterations() const ";

%feature("docstring")  cmf::math::MultiIntegrator::ResetIterations "void ResetIterations() ";

/*  Constructors and Destructors  */

/*

*/

/*  Integrate  */

/*

*/

%feature("docstring")  cmf::math::MultiIntegrator::Integrate "int
Integrate(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::MultiIntegrator::IntegrateUntil "void IntegrateUntil(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::MultiIntegrator::IntegrateUntil "void IntegrateUntil(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables until MaxTime. ";

/*  Accuracy parameters  */

/*

*/

%feature("docstring")  cmf::math::MultiIntegrator::Integrate "int
Integrate(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables

Parameters:
-----------

MaxTime:  To stop the model (if running in a model framework) at time
steps of value exchange e.g. full hours, the next value exchange time
can be given

TimeStep:  Takes the proposed timestep, and changes it into the
effictivly used timestep according to the local stiffness of the
problem and MaxTime ";

%feature("docstring")  cmf::math::MultiIntegrator::Copy "virtual
cmf::math::MultiIntegrator* Copy() const

Returns a new Integrator, based on this (without the state variables),
e.g. same type, epsilon, model time etc. ";

%feature("docstring")  cmf::math::MultiIntegrator::Reset "virtual
void Reset() ";

%feature("docstring")  cmf::math::MultiIntegrator::AddStatesFromOwner
"void AddStatesFromOwner(cmf::math::StateVariableOwner &stateOwner)
";

%feature("docstring")
cmf::math::MultiIntegrator::add_states_to_integrator "void
add_states_to_integrator(cmf::math::StateVariableOwner &stateOwner,
int integrator_position) ";

%feature("docstring")  cmf::math::MultiIntegrator::MultiIntegrator "MultiIntegrator(const cmf::math::Integrator &template_integrator, int
count) ";

%feature("docstring")  cmf::math::MultiIntegrator::size "int size()
const

returns the number of state variables ";

%feature("docstring")  cmf::math::MultiIntegrator::get_state "real
get_state(int position) const

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::MultiIntegrator::set_state "void
set_state(int position, real newState)

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::MultiIntegrator::get_states "state_queue get_states()

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";


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

%feature("docstring")  cmf::upslope::neighbor_iterator::valid "bool
valid() const ";

%feature("docstring")  cmf::upslope::neighbor_iterator::next "neighbor_iterator& next()

Points the iterator to the next neighbor. ";


// File: classcmf_1_1water_1_1_neumann_boundary.xml
%feature("docstring") cmf::water::NeumannBoundary "

A Neumann boundary condition (constant flux boundary condition)

The flux is a timeseries, but can be used as a scalar. To scale the
timeseries to the specific conditions of this boundary condition the
linear_scale flux_scale can be used.

C++ includes: boundary_condition.h ";

%feature("docstring")  cmf::water::NeumannBoundary::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute) const

Returns the solute concentrations of the flux at a given time. ";

%feature("docstring")  cmf::water::NeumannBoundary::is_empty "bool
is_empty() const ";

%feature("docstring")  cmf::water::NeumannBoundary::RecalcFluxes "bool RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node. WaterStorage overrides this, since
state changes require an update of the fluxes ";

%feature("docstring")  cmf::water::NeumannBoundary::connect_to "void
connect_to(cmf::water::flux_node::ptr target) ";

%feature("docstring")  cmf::water::NeumannBoundary::NeumannBoundary "NeumannBoundary(const cmf::project &_project, cmf::math::timeseries
_flux, cmf::water::SoluteTimeseries
_concentration=cmf::water::SoluteTimeseries(), cmf::geometry::point
loc=cmf::geometry::point())

Ctor of the Neumann boundary

Parameters:
-----------

_project:  The project this boundary condition belongs to

_flux:  The flux timeseries (a scalar is converted to a timeseries
automatically)

_concentration:  The concentration timeseries

loc:  The location of the boundary condition ";

%feature("docstring")  cmf::water::NeumannBoundary::NeumannBoundary "NeumannBoundary(const cmf::project &_project, cmf::geometry::point
loc=cmf::geometry::point()) ";

%feature("docstring")  cmf::water::NeumannBoundary::project "const
cmf::project& project() const

Returns the project, this node is part of. ";

%feature("docstring")  cmf::water::NeumannBoundary::is_storage "virtual bool is_storage() const

true, if this is a waterstorage ";

%feature("docstring")  cmf::water::NeumannBoundary::to_string "virtual std::string to_string() const ";

%feature("docstring")  cmf::water::NeumannBoundary::get_connections "cmf::water::connection_vector get_connections() const ";

%feature("docstring")  cmf::water::NeumannBoundary::get_connection "cmf::water::flux_connection* get_connection(const
cmf::water::flux_node &target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::water::NeumannBoundary::remove_connection
"bool remove_connection(cmf::water::flux_node::ptr To)

Remove the connection. ";

%feature("docstring")  cmf::water::NeumannBoundary::flux_to "real
flux_to(const cmf::water::flux_node &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from target into this\"). ";

%feature("docstring")  cmf::water::NeumannBoundary::flux3d_to "cmf::geometry::point flux3d_to(const cmf::water::flux_node &target,
cmf::math::Time t) ";

%feature("docstring")  cmf::water::NeumannBoundary::get_3d_flux "cmf::geometry::point get_3d_flux(cmf::math::Time t) ";

%feature("docstring")  cmf::water::NeumannBoundary::water_balance "real water_balance(cmf::math::Time t, const flux_connection
*Without=0) const

Returns the sum of all fluxes (positive and negative) at time t.
Single fluxes can be excluded from the calculation

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the water_balance
(e.g. to prevent closed circuits) ";

%feature("docstring")  cmf::water::NeumannBoundary::get_potential "virtual real get_potential() const

Returns the water potential of the node in m waterhead The base class
water storage always returns the height of the location ";

%feature("docstring")  cmf::water::NeumannBoundary::set_potential "virtual void set_potential(real new_potential) ";


// File: classcmf_1_1water_1_1_neumann_boundary__list.xml
%feature("docstring") cmf::water::NeumannBoundary_list "

If many Neumann boundary conditions are present in a project, a fast
data exchange to update the fluxes might be needed.

With this specialized list a num_array can be passed to the boundary
conditions for a fast flux update If a multiple system layout for the
cmf setup is chosen, we might have a node_list Drichelet boundary
conditions (dbc), a corresponding NeumannBoundary_list (nbc) of
Neumann boundaries and a node_list containing the storages connected
with the NeumannBoundary_list (storages). The fast data exchange is
written in Python as:

C++ includes: collections.h ";

%feature("docstring")  cmf::water::NeumannBoundary_list::get "NeumannBoundary::ptr get(int index) const

Returns the Neumann boundary condition at position index. ";

%feature("docstring")  cmf::water::NeumannBoundary_list::get_fluxes "cmf::math::num_array get_fluxes(cmf::math::Time t=cmf::math::Time())
const

Returns the fluxes of the items as an array. ";

%feature("docstring")  cmf::water::NeumannBoundary_list::set_fluxes "void set_fluxes(cmf::math::num_array values) ";

%feature("docstring")  cmf::water::NeumannBoundary_list::append "void
append(NeumannBoundary::ptr nbc) ";

%feature("docstring")  cmf::water::NeumannBoundary_list::size "size_t
size() const ";

%feature("docstring")
cmf::water::NeumannBoundary_list::NeumannBoundary_list "NeumannBoundary_list() ";

%feature("docstring")
cmf::water::NeumannBoundary_list::NeumannBoundary_list "NeumannBoundary_list(const cmf::water::node_list &copy) ";

%feature("docstring")
cmf::water::NeumannBoundary_list::NeumannBoundary_list "NeumannBoundary_list(const NeumannBoundary_list &copy) ";

%feature("docstring")  cmf::water::NeumannBoundary_list::to_node_list
"cmf::water::node_list to_node_list() const

Creates a node_list from this NeumannBoundary_list. ";

%feature("docstring")
cmf::water::NeumannBoundary_list::global_water_balance "real
global_water_balance(cmf::math::Time t) const

Returns the sum of the water balances of the nodes
\\\\[\\\\sigma_{global} =
\\\\sum_{i=0}^N{\\\\sum_{j=0}^{C_i}{q_{ij}(t)}} \\\\]

Replaces slow Python code like: ";

%feature("docstring")  cmf::water::NeumannBoundary_list::water_balance
"cmf::math::num_array water_balance(cmf::math::Time t) const

Returns the water balance of each vector as a vector \\\\[ \\\\sigma_i
= \\\\sum_{j=0}^{C_i}{q_{ij}(t)} \\\\]

Replaces slow Python code like: ";


// File: classcmf_1_1water_1_1_neumann_flux.xml
%feature("docstring") cmf::water::NeumannFlux "

This flux_connection is created, when connecting a Neumann boundary
condition with a state variable using Neumann::connect_to

C++ includes: boundary_condition.h ";

%feature("docstring")  cmf::water::NeumannFlux::NeumannFlux "NeumannFlux(std::tr1::shared_ptr< NeumannBoundary > left,
cmf::water::flux_node::ptr right) ";

%feature("docstring")  cmf::water::NeumannFlux::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::water::NeumannFlux::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::water::NeumannFlux::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::water::NeumannFlux::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::water::NeumannFlux::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::water::NeumannFlux::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")  cmf::water::NeumannFlux::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::water::NeumannFlux::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")  cmf::water::NeumannFlux::get_tracer_filter "real get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")  cmf::water::NeumannFlux::set_tracer_filter "void set_tracer_filter(real value) ";

%feature("docstring")  cmf::water::NeumannFlux::to_string "std::string to_string() const ";

%feature("docstring")  cmf::water::NeumannFlux::short_string "std::string short_string() const ";


// File: classcmf_1_1water_1_1node__list.xml
%feature("docstring") cmf::water::node_list "

A collection of nodes for fast access of the waterbalance

In setups with many storages and rather fast computations, the speed
of data access for output generation can take a high portion of the
total run time. To accelerate data access, one can use the node_list
object

Todo Add a get_volume / set_volume function pair, to complement
get_potential / set_potential

C++ includes: collections.h ";

%feature("docstring")  cmf::water::node_list::size "int size() const

The number of nodes. ";

%feature("docstring")  cmf::water::node_list::begin "node_vector::iterator begin() ";

%feature("docstring")  cmf::water::node_list::end "node_vector::iterator end() ";

%feature("docstring")  cmf::water::node_list::begin "node_vector::const_iterator begin() const ";

%feature("docstring")  cmf::water::node_list::end "node_vector::const_iterator end() const ";

%feature("docstring")  cmf::water::node_list::node_list "node_list()

Creates an empty node_lust. ";

%feature("docstring")  cmf::water::node_list::node_list "node_list(const node_list &forcopy)

Copy the node_list. ";

%feature("docstring")  cmf::water::node_list::get "flux_node::ptr
get(int index) const

Returns a node in the node_list. ";

%feature("docstring")  cmf::water::node_list::get "node_list get(int
begin, int end, int step=1) const

Returns a slice of the node_list. ";

%feature("docstring")  cmf::water::node_list::get_states "cmf::math::state_queue get_states()

Implements StateVariableOwner. ";

%feature("docstring")  cmf::water::node_list::append "void
append(flux_node::ptr node)

Adds a flux node to the list. ";

%feature("docstring")  cmf::water::node_list::set_potentials "int
set_potentials(const cmf::math::num_array &potentials)

Sets the potentials of the node_lists

If node do not have changeabe potentials, they are skipped The number
of nodes with changed potential ";

%feature("docstring")  cmf::water::node_list::get_potentials "cmf::math::num_array get_potentials() ";

%feature("docstring")  cmf::water::node_list::global_water_balance "real global_water_balance(cmf::math::Time t) const

Returns the sum of the water balances of the nodes

\\\\[\\\\sigma_{global} =
\\\\sum_{i=0}^N{\\\\sum_{j=0}^{C_i}{q_{ij}(t)}} \\\\]

Replaces slow Python code like: ";

%feature("docstring")  cmf::water::node_list::water_balance "cmf::math::num_array water_balance(cmf::math::Time t) const

Returns the water balance of each vector as a vector

\\\\[ \\\\sigma_i = \\\\sum_{j=0}^{C_i}{q_{ij}(t)} \\\\]

Replaces slow Python code like: ";

%feature("docstring")  cmf::water::node_list::conc "cmf::math::num_array conc(cmf::math::Time t, const cmf::water::solute
&_Solute) const

Returns an array holding the concentration of all the flux nodes for
the given solute. ";

%feature("docstring")  cmf::water::node_list::set_solute_source "int
set_solute_source(const cmf::water::solute &_Solute,
cmf::math::num_array source_fluxes)

Sets the source flux of a solute storage associated with a node (node
has to be a water storage). ";

%feature("docstring")  cmf::water::node_list::get_fluxes_to "cmf::math::num_array get_fluxes_to(const cmf::water::node_list
&targets, cmf::math::Time t) const

A fast method to perform flux queries as a batch. The node lists left
and right should have the same length.

The vector containing the flux from left to right at the same position

Parameters:
-----------

targets:  A node_list containing the source nodes

t:  The time for the fluxes

Replaces slow Python code like: ";

%feature("docstring")  cmf::water::node_list::get_fluxes3d_to "cmf::geometry::point_vector get_fluxes3d_to(const
cmf::water::node_list &targets, cmf::math::Time t) const

Returns the flux vectors to the nodes of a given target node_list. ";

%feature("docstring")  cmf::water::node_list::get_fluxes3d "cmf::geometry::point_vector get_fluxes3d(cmf::math::Time t) const

Returns the current flow vector for each node. ";

%feature("docstring")  cmf::water::node_list::get_positions "cmf::geometry::point_vector get_positions() const

Returns the positions of the nodes. ";


// File: classcmf_1_1math_1_1num__array.xml
%feature("docstring") cmf::math::num_array "

A valarray kind of vector implementation with OpenMP capabilities.

C++ includes: num_array.h ";

/*  Constructors & Destructors  */

/*

*/

%feature("docstring")  cmf::math::num_array::num_array "num_array(int
count, real Value=0)

Creates a new vector with size count

Parameters:
-----------

count:  size of new vector

Value:  Value of each element (default=0) ";

%feature("docstring")  cmf::math::num_array::num_array "num_array(size_t count, real Value=0) ";

%feature("docstring")  cmf::math::num_array::num_array "num_array()

Creates a vector of size 1 with value {0}. ";

%feature("docstring")  cmf::math::num_array::num_array "num_array(const num_array &Vector)

Copy constructor. ";

%feature("docstring")  cmf::math::num_array::num_array "num_array(double *data, size_t count)

Creates a vector from data. After creation is vector the owner of
data. ";

%feature("docstring")  cmf::math::num_array::num_array "num_array(const_iterator begin, const_iterator end)

Create a vector from an iterator range. Copies the data. ";

%feature("docstring")  cmf::math::num_array::~num_array "~num_array()

Destructor. ";

/*  Operators  */

/*  Binary operators defined as free operators:

x = {+,-,*,/}

Defined for (x is one of the operators above): vector = vector x
vector

vector = real x vector

vector = vector x real

*/

%feature("docstring")  cmf::math::num_array::power "num_array
power(const num_array &) const

Return each element to the power of each element of vector. ";

%feature("docstring")  cmf::math::num_array::power "num_array
power(real exponent) const

Return each element to the power \"exponent\". ";

%feature("docstring")  cmf::math::num_array::apply "num_array
apply(real funct(real)) const

Return funct(this)

Parameters:
-----------

funct:  a function of signature real funct(real)

Example: Return the sinus of each element of vector x ";

/*  Summarizing functions  */

/*

*/

%feature("docstring")  cmf::math::num_array::dot "real dot(const
num_array &) const

Return dot product of this and vector. ";

%feature("docstring")  cmf::math::num_array::sum "real sum() const

Return sum of vector. ";

%feature("docstring")  cmf::math::num_array::max "real max() const

Return max of vector. ";

%feature("docstring")  cmf::math::num_array::min "real min() const

Return min of vector. ";

%feature("docstring")  cmf::math::num_array::mean "real mean() const

< Return mean of vector ";

%feature("docstring")  cmf::math::num_array::norm "real norm(int
normtype=0) const

Returns a norm of the vector, implemented norms: 1 - sum of abs, 2 -
Euclidean distance, 0 Maximum of abs

Parameters:
-----------

normtype:  An integer indicating the type of norm ";

%feature("docstring")  cmf::math::num_array::begin "iterator begin()
const ";

%feature("docstring")  cmf::math::num_array::end "iterator end()
const ";

%feature("docstring")  cmf::math::num_array::size "int size() const

Size of the vector. ";

%feature("docstring")  cmf::math::num_array::resize "void resize(int
count)

Changes the size of the vector. ";

%feature("docstring")  cmf::math::num_array::resize "void
resize(size_t count) ";

%feature("docstring")  cmf::math::num_array::release "iterator
release()

Prevents deletion of the stored data. Use with care, and only if you
know what you are doing. ";


// File: classcmf_1_1upslope_1_1connections_1_1_o_h_d_i_sflow.xml
%feature("docstring") cmf::upslope::connections::OHDISflow "

A connection similar to OHDIS-KWMSS (OHymos-based DIStributed model -
with Kinematic Wave Method for Surface and Subsurface runoff)

C++ includes: subsurfacefluxes.h ";

%feature("docstring")  cmf::upslope::connections::OHDISflow::OHDISflow
"OHDISflow(cmf::upslope::SoilLayer::ptr left,
cmf::water::flux_node::ptr right, real FlowWidth, real Distance=0) ";

%feature("docstring")  cmf::upslope::connections::OHDISflow::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::connections::OHDISflow::left_node
"flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::connections::OHDISflow::right_node "flux_node::ptr
right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::upslope::connections::OHDISflow::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::OHDISflow::get_target "flux_node::ptr
get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::OHDISflow::get_target "flux_node::ptr
get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::OHDISflow::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::connections::OHDISflow::conc "real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::OHDISflow::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::connections::OHDISflow::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")  cmf::upslope::connections::OHDISflow::to_string
"std::string to_string() const ";

%feature("docstring")
cmf::upslope::connections::OHDISflow::short_string "std::string
short_string() const ";


// File: classcmf_1_1river_1_1_open_water_storage.xml
%feature("docstring") cmf::river::OpenWaterStorage "

An open water body. The potential is calculated from the stored water
using a water table function

C++ includes: OpenWaterStorage.h ";

/*  Overrides of flux_node  */

/*

*/

%feature("docstring")  cmf::river::OpenWaterStorage::RecalcFluxes "virtual bool RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node. WaterStorage overrides this, since
state changes require an update of the fluxes ";

%feature("docstring")  cmf::river::OpenWaterStorage::is_empty "virtual bool is_empty() const ";

%feature("docstring")
cmf::river::OpenWaterStorage::get_height_function "virtual const
IVolumeHeightFunction& get_height_function() const

The functional relation between volume, depth and exposed area. ";

%feature("docstring")
cmf::river::OpenWaterStorage::set_height_function "virtual void
set_height_function(const IVolumeHeightFunction &val) ";

%feature("docstring")  cmf::river::OpenWaterStorage::get_depth "real
get_depth() const

Returns the water table depth. ";

%feature("docstring")  cmf::river::OpenWaterStorage::set_depth "void
set_depth(real new_depth) ";

%feature("docstring")  cmf::river::OpenWaterStorage::wet_area "real
wet_area() const

Returns the exposed surface area in m2. ";

%feature("docstring")  cmf::river::OpenWaterStorage::get_potential "real get_potential() const

Returns the gravitational potential \\\\[ \\\\Psi_G = z + h(V) \\\\]
";

%feature("docstring")  cmf::river::OpenWaterStorage::conc "real
conc(cmf::math::Time t, const cmf::water::solute &solute) const

Returns the current WaterQuality (concentration of all solutes). ";

%feature("docstring")
cmf::river::OpenWaterStorage::get_state_variable_content "char
get_state_variable_content() const

A character indicating the integrated variable (either 'V' for Volume
or 'h' for head). ";

%feature("docstring")
cmf::river::OpenWaterStorage::set_state_variable_content "void
set_state_variable_content(char content)

A character indicating the integrated variable (either 'V' for Volume
or 'h' for head). ";

%feature("docstring")  cmf::river::OpenWaterStorage::is_storage "virtual bool is_storage() const

Returns true, since this is a storage. ";

%feature("docstring")  cmf::river::OpenWaterStorage::get_states "cmf::math::state_queue get_states()

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";

%feature("docstring")  cmf::river::OpenWaterStorage::Solute "SoluteStorage& Solute(const cmf::water::solute &_Solute)

Returns the water quality of the water storage. ";

%feature("docstring")  cmf::river::OpenWaterStorage::Solute "const
SoluteStorage& Solute(const cmf::water::solute &_Solute) const ";

%feature("docstring")  cmf::river::OpenWaterStorage::conc "real
conc(const cmf::water::solute &_Solute) const

Returns the concentration of the given solute. ";

%feature("docstring")  cmf::river::OpenWaterStorage::conc "void
conc(const cmf::water::solute &_Solute, real NewConcetration)

Sets a new concentration. ";

%feature("docstring")  cmf::river::OpenWaterStorage::get_volume "virtual real get_volume() const

Returns the volume of water in this storage in m3. ";

%feature("docstring")  cmf::river::OpenWaterStorage::set_volume "virtual void set_volume(real newwatercontent)

Sets the volume of water in this storage in m3. ";

%feature("docstring")  cmf::river::OpenWaterStorage::Derivate "virtual real Derivate(const cmf::math::Time &time)

Returns the derivate of the state variable at time time. ";

%feature("docstring")  cmf::river::OpenWaterStorage::get_state "real
get_state() const

Returns the current state of the variable. ";

%feature("docstring")  cmf::river::OpenWaterStorage::set_state "void
set_state(real newState)

Gives access to the state variable. ";

%feature("docstring")  cmf::river::OpenWaterStorage::StateIsChanged "bool StateIsChanged()

Returns if the state was currently updated. ";

%feature("docstring")  cmf::river::OpenWaterStorage::project "const
cmf::project& project() const

Returns the project, this node is part of. ";

%feature("docstring")  cmf::river::OpenWaterStorage::to_string "virtual std::string to_string() const ";

%feature("docstring")  cmf::river::OpenWaterStorage::get_connections "cmf::water::connection_vector get_connections() const ";

%feature("docstring")  cmf::river::OpenWaterStorage::get_connection "cmf::water::flux_connection* get_connection(const
cmf::water::flux_node &target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::river::OpenWaterStorage::remove_connection
"bool remove_connection(cmf::water::flux_node::ptr To)

Remove the connection. ";

%feature("docstring")  cmf::river::OpenWaterStorage::flux_to "real
flux_to(const cmf::water::flux_node &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from target into this\"). ";

%feature("docstring")  cmf::river::OpenWaterStorage::flux3d_to "cmf::geometry::point flux3d_to(const cmf::water::flux_node &target,
cmf::math::Time t) ";

%feature("docstring")  cmf::river::OpenWaterStorage::get_3d_flux "cmf::geometry::point get_3d_flux(cmf::math::Time t) ";

%feature("docstring")  cmf::river::OpenWaterStorage::water_balance "real water_balance(cmf::math::Time t, const flux_connection
*Without=0) const

Returns the sum of all fluxes (positive and negative) at time t.
Single fluxes can be excluded from the calculation

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the water_balance
(e.g. to prevent closed circuits) ";

%feature("docstring")  cmf::river::OpenWaterStorage::set_potential "virtual void set_potential(real new_potential) ";


// File: classcmf_1_1upslope_1_1_e_t_1_1_penman_evaporation.xml
%feature("docstring") cmf::upslope::ET::PenmanEvaporation "

Calculates evaporation from an open water body

C++ includes: ET.h ";

%feature("docstring")
cmf::upslope::ET::PenmanEvaporation::PenmanEvaporation "PenmanEvaporation(cmf::river::OpenWaterStorage::ptr source,
cmf::water::flux_node::ptr Evap_target, const
cmf::atmosphere::Meteorology &meteo) ";

%feature("docstring")  cmf::upslope::ET::PenmanEvaporation::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::ET::PenmanEvaporation::left_node
"flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::ET::PenmanEvaporation::right_node
"flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::upslope::ET::PenmanEvaporation::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::upslope::ET::PenmanEvaporation::get_target
"flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::ET::PenmanEvaporation::get_target
"flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::ET::PenmanEvaporation::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::ET::PenmanEvaporation::conc "real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::upslope::ET::PenmanEvaporation::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::ET::PenmanEvaporation::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")  cmf::upslope::ET::PenmanEvaporation::to_string
"std::string to_string() const ";

%feature("docstring")
cmf::upslope::ET::PenmanEvaporation::short_string "std::string
short_string() const ";


// File: classcmf_1_1upslope_1_1_e_t_1_1_penman_monteith_e_t.xml
%feature("docstring") cmf::upslope::ET::PenmanMonteithET "

Calculates the potential evapotranspiration according to FAO(1998)

Gouverning equations: \\\\begin{eqnarray*} /// \\\\lambda
get_evaporation &=& \\\\frac{\\\\Delta\\\\left(R_n -
G\\\\right)+\\\\rho_a c_p \\\\frac{e_s - e_a}{r_a}}{\\\\Delta +
\\\\gamma\\\\left(1+\\\\frac{r_s}{r_a}\\\\right)} \\\\mbox{ FAO 1998,
Eq. 3} \\\\\\\\ /// \\\\mbox{With:} \\\\\\\\ /// \\\\Delta &=& 4098
\\\\frac{0.6108 e^{17.27 T}}{(T+237.3)^2} \\\\frac{kPa}{^\\\\circ C}
\\\\mbox{ (FAO 1998, Eq. 13): Slope of vapor pressure } \\\\\\\\ /// T
&=& \\\\mbox{Actual Temperature in } ^\\\\circ C \\\\\\\\ /// R_n &=&
\\\\mbox{net Radiation (see Atmosphere) in } \\\\frac{MJ}{m^2day}
\\\\\\\\ /// G &=& 0 \\\\ \\\\frac{MJ}{m^2day} \\\\mbox{ if daily
average (FAO 1998, Eq. 42)} \\\\\\\\ /// && 0.1 R_n \\\\ \\\\mbox{ if
day time (FAO 1998, Eq. 45)} \\\\\\\\ /// && 0.5 R_n \\\\ \\\\mbox{ if
night time (FAO 1998, Eq. 46)} \\\\\\\\ /// \\\\gamma &=& \\\\frac{c_p
P}{\\\\epsilon \\\\lambda} \\\\mbox{ (FAO 1998,Eq. 8): Psychrometric
constant } \\\\frac{kPa}{^\\\\circ C} \\\\\\\\ /// c_p &=& 0.001013
\\\\frac{MJ}{kg\\\\ ^\\\\circ C}\\\\mbox{ specific heat at constant
pressure } \\\\\\\\ /// P &=& 101.3
\\\\left(\\\\frac{293-0.0065z}{293}\\\\right)^{5.26} \\\\mbox{ (FAO
1998,Eq. 7): Mean pressure kPa as a function of elevation above sea
level in m} \\\\\\\\ /// \\\\epsilon &=& 0.622 \\\\mbox{ ratio
molecular weight of water vapour/dry air} \\\\\\\\ /// \\\\lambda &=&
2.45 \\\\frac{MJ}{kg} \\\\mbox{ (FAO 1998,Eq. 8): latent heat of
vaporization} \\\\\\\\ /// R &=& 0.287 \\\\frac{kJ}{kg\\\\ k}\\\\mbox{
Specific gas constant } \\\\\\\\ /// \\\\rho_a &=&
\\\\frac{P}{1.01(T+273)R} \\\\mbox{ (FAO 1998,Box. 6): Mean air
density at constant pressure} \\\\\\\\ /// e_s &=& \\\\mbox{ Saturated
vapor pressure (see Atmosphere) in } kPa \\\\\\\\ /// e_a &=&
\\\\mbox{ Actual vapor pressure (see Atmosphere) in } kPa \\\\\\\\ ///
r_a &=&
\\\\frac{\\\\ln\\\\left(\\\\frac{2-d}{z_{om}}\\\\right)\\\\ln\\\\left(\\\\frac{2-d}{z_{oh}}\\\\right)}{k^2
u_2} \\\\mbox{ (FAO 1998, Eq. 4/Box 4): Aerodynamic resitance in }
\\\\frac s m \\\\\\\\ /// && d=\\\\frac 2 3 h,z_{om}=0.123
h,z_{oh}=0.1 z_{om}, k=0.41 \\\\\\\\ /// h &=& \\\\mbox{ Vegetation
height in }m \\\\\\\\ /// u_2 &=& \\\\mbox{ Windspeed in 2m above
ground (see Atmosphere) } \\\\frac m s \\\\\\\\ /// r_s &=&
\\\\frac{r_l}{LAI_{Active}} \\\\mbox{ (FAO 1998, Eq. 5/Box 5): bulk
surface resistance} \\\\frac s m \\\\\\\\ /// && r_l=100 \\\\frac s m,
LAI_{Active}=0.5 LAI /// \\\\end{eqnarray*}

C++ includes: ET.h ";

%feature("docstring")
cmf::upslope::ET::PenmanMonteithET::PenmanMonteithET "PenmanMonteithET(cmf::upslope::SoilLayer::ptr source,
cmf::water::flux_node::ptr ET_target) ";

%feature("docstring")  cmf::upslope::ET::PenmanMonteithET::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::ET::PenmanMonteithET::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::ET::PenmanMonteithET::right_node
"flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::upslope::ET::PenmanMonteithET::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::upslope::ET::PenmanMonteithET::get_target
"flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::ET::PenmanMonteithET::get_target
"flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::ET::PenmanMonteithET::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::ET::PenmanMonteithET::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::upslope::ET::PenmanMonteithET::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::ET::PenmanMonteithET::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")  cmf::upslope::ET::PenmanMonteithET::to_string "std::string to_string() const ";

%feature("docstring")
cmf::upslope::ET::PenmanMonteithET::short_string "std::string
short_string() const ";


// File: classcmf_1_1river_1_1_pipe_reach.xml
%feature("docstring") cmf::river::PipeReach "

Describes the geometry of a closed pipe.

C++ includes: ReachType.h ";

%feature("docstring")  cmf::river::PipeReach::get_length "double
get_length() const

Length of the reach. ";

%feature("docstring")  cmf::river::PipeReach::typecode "char
typecode() const ";

%feature("docstring")  cmf::river::PipeReach::get_channel_width "virtual double get_channel_width(double depth) const

\\\\[w=2\\\\sqrt{\\\\left|r^2-(r-d)^2\\\\right|} \\\\] ";

%feature("docstring")  cmf::river::PipeReach::get_wetted_perimeter "virtual double get_wetted_perimeter(double depth) const

\\\\[P=r\\\\arccos{\\\\frac{r-d}{r}} \\\\] ";

%feature("docstring")  cmf::river::PipeReach::get_depth "virtual
double get_depth(double area) const

\\\\[d=r\\\\left(1-\\\\cos{\\\\frac{A}{r^2}}\\\\right) \\\\] ";

%feature("docstring")  cmf::river::PipeReach::get_flux_crossection "virtual double get_flux_crossection(double depth) const

\\\\[A=r^2\\\\arccos{\\\\frac{r-d}{r}{r}} \\\\] ";

%feature("docstring")  cmf::river::PipeReach::PipeReach "PipeReach(double l, double diameter)

Creates a tube IChannel with diameter [m]. ";

%feature("docstring")  cmf::river::PipeReach::copy "PipeReach* copy()
const ";

%feature("docstring")  cmf::river::PipeReach::get_nManning "virtual
double get_nManning() const ";

%feature("docstring")  cmf::river::PipeReach::set_nManning "virtual
void set_nManning(double val) ";

%feature("docstring")  cmf::river::PipeReach::h "virtual double
h(double V) const

Returns the depth of a given volume. ";

%feature("docstring")  cmf::river::PipeReach::A "virtual double
A(double V) const

Returns the area of the surface for a given volume. ";

%feature("docstring")  cmf::river::PipeReach::V "virtual double
V(double h) const ";

%feature("docstring")  cmf::river::PipeReach::qManning "double
qManning(double A, double slope) const

Calculates the flow rate from a given water volume in the reach
\\\\begin{eqnarray*} /// q_{Manning}&=& A R^{\\\\frac 2 3}
\\\\sqrt{\\\\frac {\\\\Delta_z} n} \\\\\\\\ /// A &=& \\\\frac V l
\\\\mbox{, (Crosssectional area of the wetted crossection, Volume per
length)} \\\\\\\\ /// R &=& \\\\frac A {P(d)} \\\\\\\\ /// P(d) &=&
\\\\mbox{ the perimeter of the wetted crosssection, a function of
reach depth} \\\\\\\\ /// d(V) &=& \\\\mbox{ the depth of the reach a
function of the volume} \\\\\\\\ /// \\\\Delta_z &=& \\\\frac{z_{max}
- z_{min}}{l} \\\\mbox{ Slope of the reach} /// \\\\end{eqnarray*}
Flow rate [m3/s]

Parameters:
-----------

A:  The area of the cross section [m2]

slope:  The slope of the reach [m/m] ";


// File: classcmf_1_1geometry_1_1point.xml
%feature("docstring") cmf::geometry::point "

2D-Point Class.

Used as location property anywhere in the text Calculation of
distances

+,-,-=,*= Operators overloaded

C++ includes: geometry.h ";

%feature("docstring")  cmf::geometry::point::Center "point Center()
const ";

%feature("docstring")  cmf::geometry::point::point "point()

Empty Constructor. Creates an (0 0) location. ";

%feature("docstring")  cmf::geometry::point::point "point(const point
&p)

Copy Constructor. ";

%feature("docstring")  cmf::geometry::point::point "point(double x_,
double y_, double z_=0.0)

Creates a point from two doubles. ";

%feature("docstring")  cmf::geometry::point::distanceTo "double
distanceTo(point p) const

Returns the euclidian distance to another point.
$\\\\sqrt{(this.x-p.x)^2+(this.y-p.y)^2}$ ";

%feature("docstring")  cmf::geometry::point::z_weight_distance "double z_weight_distance(point p, double z_weight) const ";

%feature("docstring")  cmf::geometry::point::distance3DTo "double
distance3DTo(point p) const ";

%feature("docstring")  cmf::geometry::point::distance_max "double
distance_max(point p) const

Returns the distance by the maximum orthogonal offset. ";

%feature("docstring")  cmf::geometry::point::azimuth "double
azimuth(point p) const

Returns the azimuth angle of the line $ \\\\overline{this,p} $ to the
Azimuth in degrees. ";

%feature("docstring")  cmf::geometry::point::angleToXAxis "double
angleToXAxis(point p) const

Returns the angle between the line $ \\\\overline{this,p} $ to the
x-Axis in degrees. ";


// File: classcmf_1_1geometry_1_1point__vector.xml
%feature("docstring") cmf::geometry::point_vector "

Holds three arrays x,y and z for fast access of point coordinates.

C++ includes: geometry.h ";

%feature("docstring")  cmf::geometry::point_vector::point_vector "point_vector(int size) ";

%feature("docstring")  cmf::geometry::point_vector::get "point
get(int index) const ";

%feature("docstring")  cmf::geometry::point_vector::set "void set(int
index, cmf::geometry::point p) ";

%feature("docstring")  cmf::geometry::point_vector::size "int size()
const ";


// File: classcmf_1_1math_1_1_predict_correct_simple.xml
%feature("docstring") cmf::math::PredictCorrectSimple "

A simple predictor - corrector solver $ y^{n+1} = y^n + \\\\alpha
f(y^n + f(y^n)dt)dt + (1-\\\\alpha)f(y^n)dt $

C++ includes: explicit_euler.h ";

/*  The state variables to integrate  */

/*

*/

%feature("docstring")  cmf::math::PredictCorrectSimple::CopyStates "void CopyStates(num_array &destination) const

Copies the states to a numeric vector using use_OpenMP. ";

%feature("docstring")  cmf::math::PredictCorrectSimple::CopyStates "void CopyStates(real *destination) const ";

%feature("docstring")  cmf::math::PredictCorrectSimple::SetStates "void SetStates(const num_array &newStates)

Copies the new states to the actual states. ";

%feature("docstring")  cmf::math::PredictCorrectSimple::SetStates "void SetStates(real *newStates) ";

%feature("docstring")  cmf::math::PredictCorrectSimple::CopyDerivs "void CopyDerivs(Time time, num_array &destination, real factor=1)
const

Copies the derivatives at time step \"time\" to a numeric vector using
use_OpenMP

Parameters:
-----------

time:   Time at which the derivatives should be calculated

destination:  Vector to be overwritten by the results

factor:  A factor that is multiplied to the derivate (e.g. unit
conversion or integration length) ";

%feature("docstring")  cmf::math::PredictCorrectSimple::CopyDerivs "void CopyDerivs(Time time, real *destination, real factor=1) const

Copies the derivatives at time step \"time\" to an preallocated c
array

Parameters:
-----------

time:   Time at which the derivatives should be calculated

destination:  Allocated c array

factor:  A factor that is multiplied to the derivate (e.g. unit
conversion or integration length) ";

%feature("docstring")  cmf::math::PredictCorrectSimple::GetStates "num_array GetStates() const

Returns the states in a numeric vector using :CopyStates, but is
slower because of additional memory allocation. ";

%feature("docstring")  cmf::math::PredictCorrectSimple::GetDerivs "num_array GetDerivs(Time time) const

Returns the derivatives at time step \"time\" in a numeric vector
using :CopyDerivs, but is slower because of additional memory
allocation. ";

%feature("docstring")
cmf::math::PredictCorrectSimple::AddValuesToStates "void
AddValuesToStates(const num_array &operands) ";

%feature("docstring")  cmf::math::PredictCorrectSimple::AddState "virtual void AddState(cmf::math::StateVariable::ptr state) ";

/*  model time  */

/*

*/

%feature("docstring")  cmf::math::PredictCorrectSimple::ModelTime "cmf::math::Time ModelTime() const

Returns the current model time. ";

%feature("docstring")  cmf::math::PredictCorrectSimple::ModelTime "void ModelTime(cmf::math::Time val)

Sets the current model time. ";

%feature("docstring")  cmf::math::PredictCorrectSimple::TimeStep "cmf::math::Time TimeStep() const

Returns the last time step. ";

%feature("docstring")  cmf::math::PredictCorrectSimple::NextTimeStep "cmf::math::Time NextTimeStep() const

Returns the next time step width. ";

%feature("docstring")  cmf::math::PredictCorrectSimple::NextTimeStep "void NextTimeStep(cmf::math::Time val)

Sets the next time step width. ";

%feature("docstring")  cmf::math::PredictCorrectSimple::MinTimestep "const cmf::math::Time MinTimestep() const

Returns The minimal allowed time step length. ";

%feature("docstring")  cmf::math::PredictCorrectSimple::Iterations "int Iterations() const ";

%feature("docstring")
cmf::math::PredictCorrectSimple::ResetIterations "void
ResetIterations() ";

%feature("docstring")  cmf::math::PredictCorrectSimple::Reset "virtual void Reset() ";

/*  Constructors and Destructors  */

/*

*/

/*  Integrate  */

/*

*/

%feature("docstring")  cmf::math::PredictCorrectSimple::Integrate "int Integrate(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::PredictCorrectSimple::IntegrateUntil
"void IntegrateUntil(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::PredictCorrectSimple::IntegrateUntil
"void IntegrateUntil(cmf::math::Time MaxTime, cmf::math::Time
TimeStep)

Integrates the vector of state variables until MaxTime. ";

/*  Accuracy parameters  */

/*

*/

%feature("docstring")
cmf::math::PredictCorrectSimple::AddStatesFromOwner "void
AddStatesFromOwner(cmf::math::StateVariableOwner &stateOwner) ";

%feature("docstring")
cmf::math::PredictCorrectSimple::PredictCorrectSimple "PredictCorrectSimple(StateVariableOwner &states, real Alpha)

Constructs a new PredictCorrectSimple from a pointer to a vector of
state variables The Integrator becomes the owner of states

Parameters:
-----------

states:  Statevariable owner of the system

Alpha:  Weight factor $\\\\alpha$ to weight $f(y^n)$ and $f(y^{n+1})$
";

%feature("docstring")
cmf::math::PredictCorrectSimple::PredictCorrectSimple "PredictCorrectSimple(real Alpha=0.5)

Constructs a new PredictCorrectSimple

Parameters:
-----------

Alpha:  Weight factor $\\\\alpha$ to weight $f(y^n)$ and $f(y^{n+1})$
";

%feature("docstring")
cmf::math::PredictCorrectSimple::PredictCorrectSimple "PredictCorrectSimple(const Integrator &copy)

Copy constructor. ";

%feature("docstring")  cmf::math::PredictCorrectSimple::Copy "virtual
Integrator* Copy() const

Returns a new Integrator, based on this (without the state variables),
e.g. same type, epsilon, model time etc. ";

%feature("docstring")  cmf::math::PredictCorrectSimple::Integrate "int Integrate(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables

Parameters:
-----------

MaxTime:  (ignored) To stop the model (if running in a model
framework) at time steps of value exchange e.g. full hours, the next
value exchange time can be given

TimeStep:  Takes the proposed time step ";

%feature("docstring")  cmf::math::PredictCorrectSimple::size "int
size() const

returns the number of state variables ";

%feature("docstring")  cmf::math::PredictCorrectSimple::get_state "real get_state(int position) const

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::PredictCorrectSimple::set_state "void set_state(int position, real newState)

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::PredictCorrectSimple::get_states "state_queue get_states()

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";


// File: classcmf_1_1river_1_1_prism.xml
%feature("docstring") cmf::river::Prism "

Returns the height of a volume in a Prism with a defined base area.

C++ includes: ReachType.h ";

%feature("docstring")  cmf::river::Prism::Prism "Prism(double
base_area) ";

%feature("docstring")  cmf::river::Prism::copy "Prism* copy() const
";

%feature("docstring")  cmf::river::Prism::h "double h(double V) const

$ h(V) = \\\\frac V A_{base} $ ";

%feature("docstring")  cmf::river::Prism::A "double A(double V) const

$ A(V) = A_{base} $ ";

%feature("docstring")  cmf::river::Prism::V "double V(double h) const
";


// File: classcmf_1_1project.xml
%feature("docstring") cmf::project "

The study area, holding all cells, outlets and streams.

Todo Where the fuck do you describe those stinkin tracers? They should
be here!

C++ includes: project.h ";

%feature("docstring")  cmf::project::get_states "cmf::math::state_queue get_states()

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";

%feature("docstring")  cmf::project::use_IDW_meteo "void
use_IDW_meteo(double z_weight=0, double power=2) ";

%feature("docstring")  cmf::project::use_nearest_meteo "void
use_nearest_meteo(double z_weight=0) ";

%feature("docstring")  cmf::project::use_IDW_rainfall "void
use_IDW_rainfall(double z_weight=0, double power=2) ";

%feature("docstring")  cmf::project::use_nearest_rainfall "void
use_nearest_rainfall(double z_weight=0) ";

%feature("docstring")  cmf::project::get_cells "const
upslope::cell_vector& get_cells() const

Returns the vector of cells in the project. ";

%feature("docstring")  cmf::project::get_cell "upslope::Cell&
get_cell(int index)

Returns the reference to the cell at index in the project. ";

%feature("docstring")  cmf::project::size "int size() const

The number of cells in the project. ";

%feature("docstring")  cmf::project::project "project(std::string
solute_names=\"\")

Creates a new project. ";

%feature("docstring")  cmf::project::~project "~project() ";

%feature("docstring")  cmf::project::NewCell "cmf::upslope::Cell*
NewCell(double x, double y, double z, double Area)

Creates a new cell. ";

%feature("docstring")  cmf::project::NewOutlet "cmf::water::DricheletBoundary::ptr NewOutlet(std::string name, double
x, double y, double z)

Creates a new Drichelet boundary condition and adds it to the list of
outlets The potential of the Drichelet boundary equals p.z ";

%feature("docstring")  cmf::project::get_reach "cmf::river::Reach::ptr get_reach(int index)

Returns the reach at index. ";

%feature("docstring")  cmf::project::reach_count "int reach_count()
const

Returns the number of reaches in this project. ";

%feature("docstring")  cmf::project::get_storages "cmf::water::node_list get_storages()

Returns any storages of this project. ";

%feature("docstring")  cmf::project::NewReach "cmf::river::Reach::ptr
NewReach(double x, double y, double z, double length, char Type='T',
double width=0.5, double depth=0.1, bool diffusive=false)

Creates a new reach. ";


// File: classcmf_1_1upslope_1_1connections_1_1_rainfall.xml
%feature("docstring") cmf::upslope::connections::Rainfall "

A connection routing rainfall to surface water and to an eventually
existing canopy storage

C++ includes: AtmosphericFluxes.h ";

%feature("docstring")  cmf::upslope::connections::Rainfall::Rainfall "Rainfall(cmf::water::flux_node::ptr target, cmf::upslope::Cell &cell,
bool getthroughfall=true, bool getintercepted=true) ";

%feature("docstring")  cmf::upslope::connections::Rainfall::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::connections::Rainfall::left_node
"flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::connections::Rainfall::right_node
"flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::upslope::connections::Rainfall::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::upslope::connections::Rainfall::get_target
"flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::connections::Rainfall::get_target
"flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::Rainfall::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::connections::Rainfall::conc "real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::Rainfall::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::connections::Rainfall::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")  cmf::upslope::connections::Rainfall::to_string
"std::string to_string() const ";

%feature("docstring")
cmf::upslope::connections::Rainfall::short_string "std::string
short_string() const ";


// File: classcmf_1_1atmosphere_1_1_rainfall_station.xml
%feature("docstring") cmf::atmosphere::RainfallStation "

RainfallStation describes a rainfall timeseries in mm/day at a certain
place. Use RainfallStationReference or IDWRainfall to distribute the
data into space

C++ includes: precipitation.h ";

%feature("docstring")  cmf::atmosphere::RainfallStation::tostring "std::string tostring() const

Returns the name and the mean yearly rainfall. ";

%feature("docstring")  cmf::atmosphere::RainfallStation::get_position
"cmf::geometry::point get_position() const

Returns the position of the object. ";

%feature("docstring")  cmf::atmosphere::RainfallStation::use_for_cell
"void use_for_cell(cmf::upslope::Cell &c) ";

%feature("docstring")
cmf::atmosphere::RainfallStation::RainfallStation "RainfallStation(const RainfallStation &copy) ";


// File: classcmf_1_1atmosphere_1_1_rainfall_station_list.xml
%feature("docstring") cmf::atmosphere::RainfallStationList "

A list of rainfall stations

C++ includes: precipitation.h ";

%feature("docstring")  cmf::atmosphere::RainfallStationList::size "size_t size() const

Returns the number of rainfall stations. ";

%feature("docstring")  cmf::atmosphere::RainfallStationList::add "RainfallStation::ptr add(std::string Name, cmf::math::timeseries Data,
cmf::geometry::point Position)

Creates a new RainfallStation and adds it to the list. Usage: The
position of the rainfall station will be used as identifier A new
rainfall station

Parameters:
-----------

Name:  Name of the station

Data:  Rainfall timeseries

position:  Spatial position of the new station ";

%feature("docstring")  cmf::atmosphere::RainfallStationList::remove "void remove(int index) ";


// File: classcmf_1_1atmosphere_1_1_rainfall_station_reference.xml
%feature("docstring") cmf::atmosphere::RainfallStationReference "

References a single RainfallStation to provide rainfall intensity data

C++ includes: precipitation.h ";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::get_intensity "real
get_intensity(cmf::math::Time t) const

Returns the actual rainfall intensity in mm/day. ";

%feature("docstring")  cmf::atmosphere::RainfallStationReference::conc
"real conc(cmf::math::Time t, const cmf::water::solute &Solute)

Returns the concentration of a solute in the rainfall at time t. ";

%feature("docstring")  cmf::atmosphere::RainfallStationReference::conc
"virtual real conc(cmf::math::Time t, const cmf::water::solute
&Solute) const

Returns the water quality of the flux_node, if it is not overridden
this is the mix of the incoming fluxes. ";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::RecalcFluxes "virtual bool
RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node. WaterStorage overrides this, since
state changes require an update of the fluxes ";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::is_empty "virtual bool
is_empty() const ";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::project "const
cmf::project& project() const

Returns the project, this node is part of. ";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::is_storage "virtual bool
is_storage() const

true, if this is a waterstorage ";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::to_string "virtual
std::string to_string() const ";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::get_connections "cmf::water::connection_vector get_connections() const ";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::get_connection "cmf::water::flux_connection* get_connection(const
cmf::water::flux_node &target)

Returns the connection between this and target. ";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::remove_connection "bool
remove_connection(cmf::water::flux_node::ptr To)

Remove the connection. ";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::flux_to "real
flux_to(const cmf::water::flux_node &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from target into this\"). ";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::flux3d_to "cmf::geometry::point flux3d_to(const cmf::water::flux_node &target,
cmf::math::Time t) ";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::get_3d_flux "cmf::geometry::point get_3d_flux(cmf::math::Time t) ";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::water_balance "real
water_balance(cmf::math::Time t, const flux_connection *Without=0)
const

Returns the sum of all fluxes (positive and negative) at time t.
Single fluxes can be excluded from the calculation

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the water_balance
(e.g. to prevent closed circuits) ";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::get_potential "virtual
real get_potential() const

Returns the water potential of the node in m waterhead The base class
water storage always returns the height of the location ";

%feature("docstring")
cmf::atmosphere::RainfallStationReference::set_potential "virtual
void set_potential(real new_potential) ";


// File: classcmf_1_1atmosphere_1_1_rain_source.xml
%feature("docstring") cmf::atmosphere::RainSource "

An abstract class for different types of rainfall sources

C++ includes: precipitation.h ";

%feature("docstring")  cmf::atmosphere::RainSource::get_intensity "virtual real get_intensity(cmf::math::Time t) const =0

Returns the actual rainfall intensity in mm/day. ";

%feature("docstring")  cmf::atmosphere::RainSource::conc "virtual
real conc(cmf::math::Time t, const cmf::water::solute &Solute)=0

Returns the concentration of a solute in the rainfall at time t. ";

%feature("docstring")  cmf::atmosphere::RainSource::RecalcFluxes "virtual bool RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node. WaterStorage overrides this, since
state changes require an update of the fluxes ";

%feature("docstring")  cmf::atmosphere::RainSource::is_empty "virtual
bool is_empty() const ";

%feature("docstring")  cmf::atmosphere::RainSource::project "const
cmf::project& project() const

Returns the project, this node is part of. ";

%feature("docstring")  cmf::atmosphere::RainSource::is_storage "virtual bool is_storage() const

true, if this is a waterstorage ";

%feature("docstring")  cmf::atmosphere::RainSource::to_string "virtual std::string to_string() const ";

%feature("docstring")  cmf::atmosphere::RainSource::get_connections "cmf::water::connection_vector get_connections() const ";

%feature("docstring")  cmf::atmosphere::RainSource::get_connection "cmf::water::flux_connection* get_connection(const
cmf::water::flux_node &target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::atmosphere::RainSource::remove_connection
"bool remove_connection(cmf::water::flux_node::ptr To)

Remove the connection. ";

%feature("docstring")  cmf::atmosphere::RainSource::flux_to "real
flux_to(const cmf::water::flux_node &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from target into this\"). ";

%feature("docstring")  cmf::atmosphere::RainSource::flux3d_to "cmf::geometry::point flux3d_to(const cmf::water::flux_node &target,
cmf::math::Time t) ";

%feature("docstring")  cmf::atmosphere::RainSource::get_3d_flux "cmf::geometry::point get_3d_flux(cmf::math::Time t) ";

%feature("docstring")  cmf::atmosphere::RainSource::water_balance "real water_balance(cmf::math::Time t, const flux_connection
*Without=0) const

Returns the sum of all fluxes (positive and negative) at time t.
Single fluxes can be excluded from the calculation

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the water_balance
(e.g. to prevent closed circuits) ";

%feature("docstring")  cmf::atmosphere::RainSource::conc "virtual
real conc(cmf::math::Time t, const cmf::water::solute &Solute) const

Returns the water quality of the flux_node, if it is not overridden
this is the mix of the incoming fluxes. ";

%feature("docstring")  cmf::atmosphere::RainSource::get_potential "virtual real get_potential() const

Returns the water potential of the node in m waterhead The base class
water storage always returns the height of the location ";

%feature("docstring")  cmf::atmosphere::RainSource::set_potential "virtual void set_potential(real new_potential) ";


// File: classcmf_1_1river_1_1_reach.xml
%feature("docstring") cmf::river::Reach "

A reach is a specialization of an open water storage.

The OpenWaterStorage attributes and methods are extended by
topological features, for the creation of a network of reaches.

C++ includes: Reach.h ";

/*  Overrides of flux_node  */

/*

*/

%feature("docstring")  cmf::river::Reach::RecalcFluxes "virtual bool
RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node. WaterStorage overrides this, since
state changes require an update of the fluxes ";

%feature("docstring")  cmf::river::Reach::is_empty "virtual bool
is_empty() const ";

%feature("docstring")  cmf::river::Reach::get_height_function "const
IChannel& get_height_function() const

Returns the channel shape. ";

%feature("docstring")  cmf::river::Reach::set_height_function "void
set_height_function(const IChannel &val)

Sets the channel shape. ";

%feature("docstring")  cmf::river::Reach::get_length "real
get_length() const

Returns the length of the reach. ";

%feature("docstring")  cmf::river::Reach::set_outlet "void
set_outlet(cmf::water::flux_node::ptr outlet)

Connects the reach to an outlet, e.g. a boundary condition. ";

%feature("docstring")  cmf::river::Reach::set_dead_end "void
set_dead_end() ";

%feature("docstring")  cmf::river::Reach::set_downstream "void
set_downstream(ptr new_downstream)

Connects the reach to another one downstream. ";

%feature("docstring")  cmf::river::Reach::get_downstream "cmf::water::flux_node::ptr get_downstream() const

Returns the reach downstream of this (or null if there is no reach
downstream). ";

%feature("docstring")  cmf::river::Reach::upstream_count "int
upstream_count() const

Returns the number of reaches upstream of this. ";

%feature("docstring")  cmf::river::Reach::get_upstream "ptr
get_upstream(int index) const

Returns a reach upstream of this. ";

%feature("docstring")  cmf::river::Reach::connect_to_surfacewater "void connect_to_surfacewater(cmf::upslope::Cell *cell, real width,
bool diffusive)

Connects the surfacewater of a cell with this reach

Parameters:
-----------

cell:  The cell with the surface water to be connected with this reach

soil_cut:  True, if this reach might have inflow through the bank

distance:  The average distance from cell to this reach

width:  The flow width from cell to this reach ";

%feature("docstring")  cmf::river::Reach::distance_to_cell "double
distance_to_cell(cmf::upslope::Cell *cell) const

Returns the distance (d) for connections between this reach and a
cell. If the effective inner cell distance (defined as $ R_{Cell} =
0.5\\\\frac{\\\\sqrt{A}}{\\\\pi}$) is smaller than the distance
between the center points, the cell radius is returned ";

%feature("docstring")  cmf::river::Reach::get_diffusive "bool
get_diffusive() const

Returns if flow is calculated using a diffusive wave model. ";

%feature("docstring")  cmf::river::Reach::set_diffusive "void
set_diffusive(bool use_diffusive_wave)

Sets all Manning kind connections to either diffusive or kinematic. ";

%feature("docstring")  cmf::river::Reach::get_root "ptr get_root()

Returns the reach most downstream from this reach. ";

%feature("docstring")  cmf::river::Reach::~Reach "virtual ~Reach() ";

%feature("docstring")  cmf::river::Reach::set_height_function "virtual void set_height_function(const IVolumeHeightFunction &val) ";

%feature("docstring")  cmf::river::Reach::get_depth "real get_depth()
const

Returns the water table depth. ";

%feature("docstring")  cmf::river::Reach::set_depth "void
set_depth(real new_depth) ";

%feature("docstring")  cmf::river::Reach::wet_area "real wet_area()
const

Returns the exposed surface area in m2. ";

%feature("docstring")  cmf::river::Reach::get_potential "real
get_potential() const

Returns the gravitational potential \\\\[ \\\\Psi_G = z + h(V) \\\\]
";

%feature("docstring")  cmf::river::Reach::conc "real
conc(cmf::math::Time t, const cmf::water::solute &solute) const

Returns the current WaterQuality (concentration of all solutes). ";

%feature("docstring")  cmf::river::Reach::conc "real conc(const
cmf::water::solute &_Solute) const

Returns the concentration of the given solute. ";

%feature("docstring")  cmf::river::Reach::conc "void conc(const
cmf::water::solute &_Solute, real NewConcetration)

Sets a new concentration. ";

%feature("docstring")  cmf::river::Reach::get_state_variable_content "char get_state_variable_content() const

A character indicating the integrated variable (either 'V' for Volume
or 'h' for head). ";

%feature("docstring")  cmf::river::Reach::set_state_variable_content "void set_state_variable_content(char content)

A character indicating the integrated variable (either 'V' for Volume
or 'h' for head). ";

%feature("docstring")  cmf::river::Reach::is_storage "virtual bool
is_storage() const

Returns true, since this is a storage. ";

%feature("docstring")  cmf::river::Reach::get_states "cmf::math::state_queue get_states()

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";

%feature("docstring")  cmf::river::Reach::Solute "SoluteStorage&
Solute(const cmf::water::solute &_Solute)

Returns the water quality of the water storage. ";

%feature("docstring")  cmf::river::Reach::Solute "const
SoluteStorage& Solute(const cmf::water::solute &_Solute) const ";

%feature("docstring")  cmf::river::Reach::get_volume "virtual real
get_volume() const

Returns the volume of water in this storage in m3. ";

%feature("docstring")  cmf::river::Reach::set_volume "virtual void
set_volume(real newwatercontent)

Sets the volume of water in this storage in m3. ";

%feature("docstring")  cmf::river::Reach::Derivate "virtual real
Derivate(const cmf::math::Time &time)

Returns the derivate of the state variable at time time. ";

%feature("docstring")  cmf::river::Reach::get_state "real get_state()
const

Returns the current state of the variable. ";

%feature("docstring")  cmf::river::Reach::set_state "void
set_state(real newState)

Gives access to the state variable. ";

%feature("docstring")  cmf::river::Reach::StateIsChanged "bool
StateIsChanged()

Returns if the state was currently updated. ";

%feature("docstring")  cmf::river::Reach::project "const
cmf::project& project() const

Returns the project, this node is part of. ";

%feature("docstring")  cmf::river::Reach::to_string "virtual
std::string to_string() const ";

%feature("docstring")  cmf::river::Reach::get_connections "cmf::water::connection_vector get_connections() const ";

%feature("docstring")  cmf::river::Reach::get_connection "cmf::water::flux_connection* get_connection(const
cmf::water::flux_node &target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::river::Reach::remove_connection "bool
remove_connection(cmf::water::flux_node::ptr To)

Remove the connection. ";

%feature("docstring")  cmf::river::Reach::flux_to "real flux_to(const
cmf::water::flux_node &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from target into this\"). ";

%feature("docstring")  cmf::river::Reach::flux3d_to "cmf::geometry::point flux3d_to(const cmf::water::flux_node &target,
cmf::math::Time t) ";

%feature("docstring")  cmf::river::Reach::get_3d_flux "cmf::geometry::point get_3d_flux(cmf::math::Time t) ";

%feature("docstring")  cmf::river::Reach::water_balance "real
water_balance(cmf::math::Time t, const flux_connection *Without=0)
const

Returns the sum of all fluxes (positive and negative) at time t.
Single fluxes can be excluded from the calculation

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the water_balance
(e.g. to prevent closed circuits) ";

%feature("docstring")  cmf::river::Reach::set_potential "virtual void
set_potential(real new_potential) ";


// File: classcmf_1_1river_1_1_reach_iterator.xml
%feature("docstring") cmf::river::ReachIterator "

An iterator over every upstream reach from a start reach. Implements
both the Python and the C++ iterator interface Usage C++: Usage
Python:

C++ includes: Reach.h ";

%feature("docstring")  cmf::river::ReachIterator::next "Reach::ptr
next()

Returns the next reach in the upstream queue. ";

%feature("docstring")  cmf::river::ReachIterator::valid "bool valid()
const

Returns true, if reaches are left to iterate over. ";

%feature("docstring")  cmf::river::ReachIterator::ReachIterator "ReachIterator(Reach::ptr first)

Creates a ReachIterator from a first reach. ";

%feature("docstring")  cmf::river::ReachIterator::reach "Reach::ptr
reach() const

Returns the current reach. ";

%feature("docstring")  cmf::river::ReachIterator::position "double
position() const

Returns the distance to the root reach. ";


// File: classcmf_1_1river_1_1_rectangular_reach.xml
%feature("docstring") cmf::river::RectangularReach "

Describes a IChannel with a rectangular crosssection.

C++ includes: ReachType.h ";

%feature("docstring")  cmf::river::RectangularReach::get_length "double get_length() const

Length of the reach. ";

%feature("docstring")  cmf::river::RectangularReach::typecode "char
typecode() const ";

%feature("docstring")  cmf::river::RectangularReach::get_channel_width
"virtual double get_channel_width(double depth) const

Returns the width of the stream at a given depth \\\\begin{eqnarray*}
/// w &=& const /// \\\\end{eqnarray*} ";

%feature("docstring")
cmf::river::RectangularReach::get_wetted_perimeter "virtual double
get_wetted_perimeter(double depth) const

Returns the wetted perimeter at a given depth \\\\begin{eqnarray*} ///
P &=& 2 d + w /// \\\\end{eqnarray*} ";

%feature("docstring")  cmf::river::RectangularReach::get_depth "virtual double get_depth(double area) const

Returns the depth at a given crossection area \\\\begin{eqnarray*} ///
d &=& \\\\frac A w /// \\\\end{eqnarray*} ";

%feature("docstring")
cmf::river::RectangularReach::get_flux_crossection "virtual double
get_flux_crossection(double depth) const

Returns the crossection area at a given depth \\\\begin{eqnarray*} ///
A &=& d w /// \\\\end{eqnarray*} ";

%feature("docstring")  cmf::river::RectangularReach::RectangularReach
"RectangularReach(double l, double width)

Creates a new rectangular reach type with width [m]. ";

%feature("docstring")  cmf::river::RectangularReach::copy "RectangularReach* copy() const ";

%feature("docstring")  cmf::river::RectangularReach::get_nManning "virtual double get_nManning() const ";

%feature("docstring")  cmf::river::RectangularReach::set_nManning "virtual void set_nManning(double val) ";

%feature("docstring")  cmf::river::RectangularReach::h "virtual
double h(double V) const

Returns the depth of a given volume. ";

%feature("docstring")  cmf::river::RectangularReach::A "virtual
double A(double V) const

Returns the area of the surface for a given volume. ";

%feature("docstring")  cmf::river::RectangularReach::V "virtual
double V(double h) const ";

%feature("docstring")  cmf::river::RectangularReach::qManning "double
qManning(double A, double slope) const

Calculates the flow rate from a given water volume in the reach
\\\\begin{eqnarray*} /// q_{Manning}&=& A R^{\\\\frac 2 3}
\\\\sqrt{\\\\frac {\\\\Delta_z} n} \\\\\\\\ /// A &=& \\\\frac V l
\\\\mbox{, (Crosssectional area of the wetted crossection, Volume per
length)} \\\\\\\\ /// R &=& \\\\frac A {P(d)} \\\\\\\\ /// P(d) &=&
\\\\mbox{ the perimeter of the wetted crosssection, a function of
reach depth} \\\\\\\\ /// d(V) &=& \\\\mbox{ the depth of the reach a
function of the volume} \\\\\\\\ /// \\\\Delta_z &=& \\\\frac{z_{max}
- z_{min}}{l} \\\\mbox{ Slope of the reach} /// \\\\end{eqnarray*}
Flow rate [m3/s]

Parameters:
-----------

A:  The area of the cross section [m2]

slope:  The slope of the reach [m/m] ";


// File: classcmf_1_1upslope_1_1_retention_curve.xml
%feature("docstring") cmf::upslope::RetentionCurve "

Abstract base class for different types of retention curves

This class, and its children uses wetness instead of volumetric water
content. The wetness of a soil is defined as water content per void
volume

C++ includes: RetentionCurve.h ";

%feature("docstring")  cmf::upslope::RetentionCurve::K "virtual real
K(real wetness) const =0

Returns the conductivity in m/day at a certain depth and water
content. ";

%feature("docstring")  cmf::upslope::RetentionCurve::Wetness_eff "virtual real Wetness_eff(real wetness, real pF_r=4.2) const

Returns the effective wetness, using a residual pF value \\\\[w_{eff}
=
\\\\frac{w_{act}-w\\\\left(pF_r\\\\right)}{1-w\\\\left(pF_r\\\\right)}\\\\]
";

%feature("docstring")  cmf::upslope::RetentionCurve::Porosity "virtual real Porosity(real depth) const =0

Returns the porosity at a certain depth. ";

%feature("docstring")  cmf::upslope::RetentionCurve::VoidVolume "virtual real VoidVolume(real upperDepth, real lowerDepth, real Area)
const

Returns the void volume of a soil column. ";

%feature("docstring")  cmf::upslope::RetentionCurve::FillHeight "virtual real FillHeight(real lowerDepth, real Area, real Volume) const

Returns the thickness of a soil column with a certain pore volume. ";

%feature("docstring")  cmf::upslope::RetentionCurve::Wetness "virtual
real Wetness(real suction) const

returns the wetness (volumetric water content per pore space) at a
given suction pressure ";

%feature("docstring")  cmf::upslope::RetentionCurve::Wetness_pF "real
Wetness_pF(real pF) const

returns the volumetric water content at a given pF value ";

%feature("docstring")  cmf::upslope::RetentionCurve::MatricPotential "virtual real MatricPotential(real wetness) const

returns the wetness of the soil at given water content ";

%feature("docstring")  cmf::upslope::RetentionCurve::copy "virtual
RetentionCurve* copy() const =0 ";


// File: classcmf_1_1upslope_1_1connections_1_1_richards.xml
%feature("docstring") cmf::upslope::connections::Richards "

Calculates flow according to the Richards equation

\\\\begin{eqnarray*} /// q_{Richards} &=&
\\\\frac{\\\\Delta\\\\Psi_{tot}}{d} K(\\\\theta) A \\\\\\\\ ///
\\\\Psi_{tot} &= &\\\\Psi_{M}(\\\\theta) + h /// \\\\end{eqnarray*}
where  $ \\\\Delta\\\\Psi_{tot} [m]$ is the difference of the total
water potentials of the two soil layers

$ d [m]$ is the distance between the two soil layers

$ K(\\\\theta)\\\\left[\\\\frac m{day}\\\\right]$ is the geometric
mean conductivity (see SoilType::Kunsat)

$ A [m^2]$ is the crosssectional area of the flux

$ \\\\Psi_M(\\\\theta) [m]$ is the matrix potential (see
SoilType::MatrixPotential)

$ h [m]$ is the height of a soil layer above sea level

C++ includes: Percolation.h ";

%feature("docstring")  cmf::upslope::connections::Richards::Richards "Richards(cmf::upslope::SoilLayer::ptr left, cmf::water::flux_node::ptr
right) ";

%feature("docstring")  cmf::upslope::connections::Richards::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::connections::Richards::left_node
"flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::connections::Richards::right_node
"flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::upslope::connections::Richards::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::upslope::connections::Richards::get_target
"flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::connections::Richards::get_target
"flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::Richards::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::connections::Richards::conc "real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::Richards::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::connections::Richards::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")  cmf::upslope::connections::Richards::to_string
"std::string to_string() const ";

%feature("docstring")
cmf::upslope::connections::Richards::short_string "std::string
short_string() const ";


// File: classcmf_1_1upslope_1_1connections_1_1_richards__lateral.xml
%feature("docstring") cmf::upslope::connections::Richards_lateral "

Calculates the flux using Richard's equation for adjecent layers

C++ includes: subsurfacefluxes.h ";

%feature("docstring")
cmf::upslope::connections::Richards_lateral::Richards_lateral "Richards_lateral(cmf::upslope::SoilLayer::ptr left,
cmf::water::flux_node::ptr right, real FlowWidth=0, real Distance=0)
";

%feature("docstring")  cmf::upslope::connections::Richards_lateral::q
"real q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::Richards_lateral::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::connections::Richards_lateral::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::Richards_lateral::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::Richards_lateral::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::Richards_lateral::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::Richards_lateral::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")
cmf::upslope::connections::Richards_lateral::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::Richards_lateral::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::connections::Richards_lateral::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")
cmf::upslope::connections::Richards_lateral::to_string "std::string
to_string() const ";

%feature("docstring")
cmf::upslope::connections::Richards_lateral::short_string "std::string short_string() const ";


// File: classcmf_1_1math_1_1_r_k_f_integrator.xml
%feature("docstring") cmf::math::RKFIntegrator "

Integrates a vector of cmf::math::StateVariable with the Runge-Kutta-
Fehlberg (RKF54) method.

C++ includes: RKFintegrator.h ";

/*  The state variables to integrate  */

/*

*/

%feature("docstring")  cmf::math::RKFIntegrator::CopyStates "void
CopyStates(num_array &destination) const

Copies the states to a numeric vector using use_OpenMP. ";

%feature("docstring")  cmf::math::RKFIntegrator::CopyStates "void
CopyStates(real *destination) const ";

%feature("docstring")  cmf::math::RKFIntegrator::SetStates "void
SetStates(const num_array &newStates)

Copies the new states to the actual states. ";

%feature("docstring")  cmf::math::RKFIntegrator::SetStates "void
SetStates(real *newStates) ";

%feature("docstring")  cmf::math::RKFIntegrator::CopyDerivs "void
CopyDerivs(Time time, num_array &destination, real factor=1) const

Copies the derivatives at time step \"time\" to a numeric vector using
use_OpenMP

Parameters:
-----------

time:   Time at which the derivatives should be calculated

destination:  Vector to be overwritten by the results

factor:  A factor that is multiplied to the derivate (e.g. unit
conversion or integration length) ";

%feature("docstring")  cmf::math::RKFIntegrator::CopyDerivs "void
CopyDerivs(Time time, real *destination, real factor=1) const

Copies the derivatives at time step \"time\" to an preallocated c
array

Parameters:
-----------

time:   Time at which the derivatives should be calculated

destination:  Allocated c array

factor:  A factor that is multiplied to the derivate (e.g. unit
conversion or integration length) ";

%feature("docstring")  cmf::math::RKFIntegrator::GetStates "num_array
GetStates() const

Returns the states in a numeric vector using :CopyStates, but is
slower because of additional memory allocation. ";

%feature("docstring")  cmf::math::RKFIntegrator::GetDerivs "num_array
GetDerivs(Time time) const

Returns the derivatives at time step \"time\" in a numeric vector
using :CopyDerivs, but is slower because of additional memory
allocation. ";

%feature("docstring")  cmf::math::RKFIntegrator::AddValuesToStates "void AddValuesToStates(const num_array &operands) ";

%feature("docstring")  cmf::math::RKFIntegrator::AddState "virtual
void AddState(cmf::math::StateVariable::ptr state) ";

/*  model time  */

/*

*/

%feature("docstring")  cmf::math::RKFIntegrator::ModelTime "cmf::math::Time ModelTime() const

Returns the current model time. ";

%feature("docstring")  cmf::math::RKFIntegrator::ModelTime "void
ModelTime(cmf::math::Time val)

Sets the current model time. ";

%feature("docstring")  cmf::math::RKFIntegrator::TimeStep "cmf::math::Time TimeStep() const

Returns the last time step. ";

%feature("docstring")  cmf::math::RKFIntegrator::NextTimeStep "cmf::math::Time NextTimeStep() const

Returns the next time step width. ";

%feature("docstring")  cmf::math::RKFIntegrator::NextTimeStep "void
NextTimeStep(cmf::math::Time val)

Sets the next time step width. ";

%feature("docstring")  cmf::math::RKFIntegrator::MinTimestep "const
cmf::math::Time MinTimestep() const

Returns The minimal allowed time step length. ";

%feature("docstring")  cmf::math::RKFIntegrator::Iterations "int
Iterations() const ";

%feature("docstring")  cmf::math::RKFIntegrator::ResetIterations "void ResetIterations() ";

%feature("docstring")  cmf::math::RKFIntegrator::Reset "virtual void
Reset() ";

/*  Constructors and Destructors  */

/*

*/

/*  Integrate  */

/*

*/

%feature("docstring")  cmf::math::RKFIntegrator::Integrate "int
Integrate(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::RKFIntegrator::IntegrateUntil "void
IntegrateUntil(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::RKFIntegrator::IntegrateUntil "void
IntegrateUntil(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables until MaxTime. ";

/*  Accuracy parameters  */

/*

*/

%feature("docstring")  cmf::math::RKFIntegrator::AddStatesFromOwner "void AddStatesFromOwner(cmf::math::StateVariableOwner &stateOwner) ";

%feature("docstring")  cmf::math::RKFIntegrator::RKFIntegrator "RKFIntegrator(StateVariableOwner &states, real epsilon=1e-9,
cmf::math::Time tStepMin=Time::Seconds(10))

Constructs a new RKFIntegrator from a pointer to a vector of state
variables The RKF Integrator becomes the owner of states

Parameters:
-----------

states:  Statevariables of the system

epsilon:  relative error tolerance per time step (default=1e-9)

tStepMin:  minimum time step (default=10s) ";

%feature("docstring")  cmf::math::RKFIntegrator::RKFIntegrator "RKFIntegrator(real epsilon=1e-9, cmf::math::Time
tStepMin=Time::Seconds(10))

Constructs a new RKFIntegrator

Parameters:
-----------

epsilon:  relative error tolerance per time step (default=1e-9)

tStepMin:  minimum time step (default=10s) ";

%feature("docstring")  cmf::math::RKFIntegrator::RKFIntegrator "RKFIntegrator(const Integrator &forCopy)

Copy constructor, does not copy. ";

%feature("docstring")  cmf::math::RKFIntegrator::Copy "virtual
Integrator* Copy() const

Returns a new Integrator, based on this (without the state variables),
e.g. same type, epsilon, model time etc. ";

%feature("docstring")  cmf::math::RKFIntegrator::Integrate "int
Integrate(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables

Parameters:
-----------

MaxTime:  To stop the model (if running in a model framework) at time
steps of value exchange e.g. full hours, the next value exchange time
can be given

TimeStep:  Takes the proposed timestep, and changes it into the
effictivly used timestep according to the local stiffness of the
problem and MaxTime ";

%feature("docstring")  cmf::math::RKFIntegrator::size "int size()
const

returns the number of state variables ";

%feature("docstring")  cmf::math::RKFIntegrator::get_state "real
get_state(int position) const

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::RKFIntegrator::set_state "void
set_state(int position, real newState)

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::RKFIntegrator::get_states "state_queue get_states()

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";


// File: classcmf_1_1upslope_1_1_e_t_1_1_shuttleworth_wallace.xml
%feature("docstring") cmf::upslope::ET::ShuttleworthWallace "

Calculates the sum of soil evaporation and transpiration according to
Shuttleworth & Wallace 1985, as implemented in BROOK 90 (Federer 1990)

The difference to BROOK90 is, that the actual transpiration is not
calculated by plant resitance and potential gradient between plant and
soil, but by an piecewise linear function of the pF value $ pF =
\\\\log_{10}\\\\left(-\\\\Psi [hPa]\\\\right) $: \\\\[
\\\\frac{T_{act}}{T_{pot}} = \\\\left\\\\{\\\\begin{array}{cl} /// 1 &
\\\\mbox{if $pF \\\\le 3.35$} \\\\\\\\ /// \\\\frac{pF - 4.2}{3.35 -
4.2} & \\\\mbox{if $pF \\\\in [3.35 .. 4.2] $} \\\\\\\\ /// 0 &
\\\\mbox{if $pF \\\\ge 4.2$} \\\\end{array}\\\\right. \\\\]

Calculation procedure, as in BROOK 90:

Evapotranspiration from the canopy: $\\\\lambda ET_{canopy} = \\\\frac
{r_{ac} \\\\Delta\\\\ R_{n,canopy} + c_p\\\\rho D_0}{\\\\Delta
\\\\gamma r_{ac} + \\\\gamma r_{sc}} $

Evaporation from the ground: $\\\\lambda E_{ground} = \\\\frac {r_{as}
\\\\Delta\\\\ R_{n,ground} + c_p\\\\rho D_0}{\\\\Delta \\\\gamma
r_{as} + \\\\gamma r_{ss}} $

with  $ \\\\Delta = \\\\frac{de_s}{dT} = 4098\\\\ 0.6108
\\\\exp\\\\left(\\\\frac{17.27 T}{T+237.3}\\\\right)(T+237.3)^{-2} $,
the slope of the sat. vap. press. T function

$ R_{n,ground} = R_n \\\\exp(-C_R LAI) $, the net radiation flux in
the ground

$ R_{n_canopy} = R_n - R_{n,ground} $, the net radiation flux in the
canopy

$ \\\\lambda,c_p\\\\rho,\\\\gamma,C_R $ constants lambda, c_p_rho,
gamma, C_R

$ D_0 $ vapor pressure deficit at effective source height, see
function D0

$ r_{ac}, r_{sc}, r_{as}, r_{ss} $ Resistances for the vapor pressure
(see below)

C++ includes: ShuttleworthWallace.h ";

%feature("docstring")  cmf::upslope::ET::ShuttleworthWallace::refresh
"void refresh(cmf::math::Time t)

Calculates all the values. ";

%feature("docstring")  cmf::upslope::ET::ShuttleworthWallace::refresh
"void refresh() ";

%feature("docstring")
cmf::upslope::ET::ShuttleworthWallace::transp_from_layer "virtual
double transp_from_layer(cmf::upslope::SoilLayer::ptr sl,
cmf::math::Time t)

returns the transpiration rate from one layer in m3/day ";

%feature("docstring")
cmf::upslope::ET::ShuttleworthWallace::evap_from_layer "virtual
double evap_from_layer(cmf::upslope::SoilLayer::ptr sl,
cmf::math::Time t)

returns the soil evaporation rate from one layer in m3/day ";

%feature("docstring")
cmf::upslope::ET::ShuttleworthWallace::evap_from_openwater "virtual
double evap_from_openwater(cmf::river::OpenWaterStorage::ptr ows,
cmf::math::Time t) ";

%feature("docstring")
cmf::upslope::ET::ShuttleworthWallace::evap_from_canopy "virtual
double evap_from_canopy(cmf::water::WaterStorage::ptr canopy,
cmf::math::Time t) ";

%feature("docstring")
cmf::upslope::ET::ShuttleworthWallace::evap_from_snow "virtual double
evap_from_snow(cmf::water::WaterStorage::ptr snow, cmf::math::Time t)
";

%feature("docstring")
cmf::upslope::ET::ShuttleworthWallace::get_aerodynamic_resistance "virtual void get_aerodynamic_resistance(double &r_ag, double &r_ac,
cmf::math::Time t) const

aerodynamic resistance from ground to atmosphere (r_ag) and from
canopy to atmosphere (r_ac) ";

%feature("docstring")
cmf::upslope::ET::ShuttleworthWallace::ShuttleworthWallace "ShuttleworthWallace(cmf::upslope::Cell &cell)

Calculates the transpiration and the soil evaporation from dry
surfaces

Parameters:
-----------

w:  A meteorological data record

soilwater_matrixpotential:  The suction of the soil water in m water
column

veg:  The parameters of the vegetation

CanopyStoresWater:  Flag indicating wet leaves. If true, a canopy
surface resistance of 0 is assumed

measurement_height_above_canopy:  The height of the wind speed above
the canopy in m ";


// File: classcmf_1_1upslope_1_1connections_1_1_simple_tindex_snow_melt.xml
%feature("docstring") cmf::upslope::connections::SimpleTindexSnowMelt
"

Calculates snow melt using a simple degree day method

C++ includes: surfacefluxes.h ";

%feature("docstring")
cmf::upslope::connections::SimpleTindexSnowMelt::SimpleTindexSnowMelt
"SimpleTindexSnowMelt(cmf::water::WaterStorage::ptr snow,
cmf::water::flux_node::ptr surface_water, cmf::upslope::Cell &cell) ";

%feature("docstring")
cmf::upslope::connections::SimpleTindexSnowMelt::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::SimpleTindexSnowMelt::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::connections::SimpleTindexSnowMelt::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::SimpleTindexSnowMelt::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::SimpleTindexSnowMelt::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::SimpleTindexSnowMelt::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::SimpleTindexSnowMelt::exchange_target "void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")
cmf::upslope::connections::SimpleTindexSnowMelt::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::SimpleTindexSnowMelt::get_tracer_filter "real get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::connections::SimpleTindexSnowMelt::set_tracer_filter "void set_tracer_filter(real value) ";

%feature("docstring")
cmf::upslope::connections::SimpleTindexSnowMelt::to_string "std::string to_string() const ";

%feature("docstring")
cmf::upslope::connections::SimpleTindexSnowMelt::short_string "std::string short_string() const ";


// File: classcmf_1_1upslope_1_1connections_1_1_simpl_richards.xml
%feature("docstring") cmf::upslope::connections::SimplRichards "

Calculates flow according to a simplified Richards equation

\\\\begin{eqnarray*} /// q_{Richards} &=& (K(\\\\theta) -
K(\\\\theta_r)) A \\\\\\\\ /// \\\\end{eqnarray*} where  $ d [m]$ is
the distance between the two soil layers

$ K(\\\\theta)\\\\left[\\\\frac m{day}\\\\right]$ is the geometric
mean conductivity (see SoilType::Kunsat)

$ A [m^2]$ is the crosssectional area of the flux

C++ includes: Percolation.h ";

%feature("docstring")
cmf::upslope::connections::SimplRichards::SimplRichards "SimplRichards(cmf::upslope::SoilLayer::ptr left,
cmf::water::flux_node::ptr right) ";

%feature("docstring")  cmf::upslope::connections::SimplRichards::q "real q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::SimplRichards::left_node "flux_node::ptr
left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::connections::SimplRichards::right_node "flux_node::ptr
right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::SimplRichards::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::SimplRichards::get_target "flux_node::ptr
get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::SimplRichards::get_target "flux_node::ptr
get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::SimplRichards::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::connections::SimplRichards::conc
"real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::SimplRichards::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::connections::SimplRichards::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")
cmf::upslope::connections::SimplRichards::to_string "std::string
to_string() const ";

%feature("docstring")
cmf::upslope::connections::SimplRichards::short_string "std::string
short_string() const ";


// File: classcmf_1_1upslope_1_1_e_t_1_1snow__evaporation.xml
%feature("docstring") cmf::upslope::ET::snow_evaporation "

Flux connection using a snow_evaporation_method.

C++ includes: ET.h ";

%feature("docstring")
cmf::upslope::ET::snow_evaporation::snow_evaporation "snow_evaporation(cmf::water::WaterStorage::ptr source,
cmf::water::flux_node::ptr ET_target, std::tr1::shared_ptr<
snow_evaporation_method > _method, std::string method_name) ";

%feature("docstring")  cmf::upslope::ET::snow_evaporation::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::ET::snow_evaporation::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::ET::snow_evaporation::right_node
"flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::upslope::ET::snow_evaporation::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::upslope::ET::snow_evaporation::get_target
"flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::ET::snow_evaporation::get_target
"flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::ET::snow_evaporation::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::ET::snow_evaporation::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::upslope::ET::snow_evaporation::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::ET::snow_evaporation::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")  cmf::upslope::ET::snow_evaporation::to_string "std::string to_string() const ";

%feature("docstring")
cmf::upslope::ET::snow_evaporation::short_string "std::string
short_string() const ";


// File: classcmf_1_1upslope_1_1_e_t_1_1snow__evaporation__method.xml
%feature("docstring") cmf::upslope::ET::snow_evaporation_method "

Abstract class. Child classes are defining a method for snow
evaporation calculation.

C++ includes: ET.h ";

%feature("docstring")
cmf::upslope::ET::snow_evaporation_method::evap_from_snow "virtual
double evap_from_snow(cmf::water::WaterStorage::ptr snow,
cmf::math::Time t)=0 ";


// File: classcmf_1_1upslope_1_1connections_1_1_snowfall.xml
%feature("docstring") cmf::upslope::connections::Snowfall "

A connection routing snowfall (precipitation below freezing Temp) to
the snow pack

C++ includes: AtmosphericFluxes.h ";

%feature("docstring")  cmf::upslope::connections::Snowfall::Snowfall "Snowfall(cmf::water::flux_node::ptr target, cmf::upslope::Cell &cell)
";

%feature("docstring")  cmf::upslope::connections::Snowfall::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::connections::Snowfall::left_node
"flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::connections::Snowfall::right_node
"flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::upslope::connections::Snowfall::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::upslope::connections::Snowfall::get_target
"flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::connections::Snowfall::get_target
"flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::Snowfall::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::connections::Snowfall::conc "real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::Snowfall::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::connections::Snowfall::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")  cmf::upslope::connections::Snowfall::to_string
"std::string to_string() const ";

%feature("docstring")
cmf::upslope::connections::Snowfall::short_string "std::string
short_string() const ";


// File: classcmf_1_1upslope_1_1_e_t_1_1soil__evaporation.xml
%feature("docstring") cmf::upslope::ET::soil_evaporation "

Flux_connection using a soil_evaporation_method.

C++ includes: ET.h ";

%feature("docstring")
cmf::upslope::ET::soil_evaporation::soil_evaporation "soil_evaporation(cmf::upslope::SoilLayer::ptr source,
cmf::water::flux_node::ptr ET_target, std::tr1::shared_ptr<
soil_evaporation_method > _method, std::string method_name) ";

%feature("docstring")  cmf::upslope::ET::soil_evaporation::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::ET::soil_evaporation::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::ET::soil_evaporation::right_node
"flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::upslope::ET::soil_evaporation::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::upslope::ET::soil_evaporation::get_target
"flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::ET::soil_evaporation::get_target
"flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::ET::soil_evaporation::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::ET::soil_evaporation::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::upslope::ET::soil_evaporation::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::ET::soil_evaporation::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")  cmf::upslope::ET::soil_evaporation::to_string "std::string to_string() const ";

%feature("docstring")
cmf::upslope::ET::soil_evaporation::short_string "std::string
short_string() const ";


// File: classcmf_1_1upslope_1_1_e_t_1_1soil__evaporation__method.xml
%feature("docstring") cmf::upslope::ET::soil_evaporation_method "

Abstract class. Child classes are defining a method for soil
evaporation calculation.

C++ includes: ET.h ";

%feature("docstring")
cmf::upslope::ET::soil_evaporation_method::evap_from_layer "virtual
double evap_from_layer(cmf::upslope::SoilLayer::ptr, cmf::math::Time
t)=0

returns the soil evaporation rate from one layer in m3/day ";


// File: classcmf_1_1upslope_1_1_soil_layer.xml
%feature("docstring") cmf::upslope::SoilLayer "

A representation of a SoilLayer

C++ includes: SoilLayer.h ";

/*  Overrides of flux_node  */

/*

*/

%feature("docstring")  cmf::upslope::SoilLayer::RecalcFluxes "virtual
bool RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node. WaterStorage overrides this, since
state changes require an update of the fluxes ";

%feature("docstring")  cmf::upslope::SoilLayer::is_empty "virtual
bool is_empty() const ";

%feature("docstring")  cmf::upslope::SoilLayer::get_ice_fraction "real get_ice_fraction() const

real (Ice_fraction) ";

%feature("docstring")  cmf::upslope::SoilLayer::set_ice_fraction "void set_ice_fraction(real val)

Parameters:
-----------

val:  Ice_fraction (real) ";

%feature("docstring")  cmf::upslope::SoilLayer::get_upper_boundary "virtual real get_upper_boundary() const

Returns the upper boundary of the water storage below ground in m. ";

%feature("docstring")  cmf::upslope::SoilLayer::get_lower_boundary "virtual real get_lower_boundary() const

Returns the lower boundary of the water storage below ground in m. ";

%feature("docstring")  cmf::upslope::SoilLayer::get_upper "ptr
get_upper() const ";

%feature("docstring")  cmf::upslope::SoilLayer::get_lower "ptr
get_lower() const ";

%feature("docstring")  cmf::upslope::SoilLayer::get_thickness "real
get_thickness() const ";

%feature("docstring")  cmf::upslope::SoilLayer::get_soil "virtual
cmf::upslope::RetentionCurve& get_soil() const

Returns the soil properties of the water storage. ";

%feature("docstring")  cmf::upslope::SoilLayer::set_soil "virtual
void set_soil(const cmf::upslope::RetentionCurve &r_curve) ";

%feature("docstring")  cmf::upslope::SoilLayer::get_theta "virtual
real get_theta() const

Returns the actual volumetric water content of the water storage. ";

%feature("docstring")  cmf::upslope::SoilLayer::set_theta "virtual
void set_theta(real Value) ";

%feature("docstring")  cmf::upslope::SoilLayer::get_volume "virtual
real get_volume() const

Returns the volume of water in this storage in m3. ";

%feature("docstring")  cmf::upslope::SoilLayer::get_K "real get_K()
const

Returns the actual conductivity $\\\\frac{m}{day}$. ";

%feature("docstring")  cmf::upslope::SoilLayer::get_Ksat "real
get_Ksat() const ";

%feature("docstring")  cmf::upslope::SoilLayer::get_wetness "virtual
real get_wetness() const

Returns the wetness of the soil $ \\\\frac{V_{H_2O}}{V_{pores}} $. ";

%feature("docstring")  cmf::upslope::SoilLayer::set_wetness "virtual
void set_wetness(real wetness) ";

%feature("docstring")  cmf::upslope::SoilLayer::get_matrix_potential "virtual real get_matrix_potential() const

Calls RetentionCurve::Matrixpotential. ";

%feature("docstring")
cmf::upslope::SoilLayer::get_gravitational_potential "real
get_gravitational_potential() const

Gravitational get_potential in m, reference height is sea level. If
the layer is saturated, it returns the saturated depth above sea
level, otherwise it returns the upperboundary of the layer \\\\[
\\\\Psi_G=h \\\\] ";

%feature("docstring")  cmf::upslope::SoilLayer::get_porosity "real
get_porosity() const

Returns the mean porosity in the layer. ";

%feature("docstring")  cmf::upslope::SoilLayer::get_capacity "virtual
real get_capacity() const

Returns the capacity of the water storage in m3. ";

%feature("docstring")  cmf::upslope::SoilLayer::set_potential "virtual void set_potential(real waterhead)

Sets the potential of this soil water storage. ";

%feature("docstring")  cmf::upslope::SoilLayer::get_potential "virtual real get_potential() const

Returns the total potential in m \\\\[ \\\\Psi = \\\\Psi_M + \\\\Psi_G
\\\\] ";

%feature("docstring")  cmf::upslope::SoilLayer::get_saturated_depth "virtual real get_saturated_depth() const

Returns the depth for saturation \\\\[ z_{sat,this} =
\\\\left\\\\{z_{cell}-\\\\Psi_{tot} \\\\mbox{ if } W<1 \\\\\\\\
z_{sat,upper layer} \\\\right. \\\\] ";

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
cmf::upslope::SoilLayer::get_state_variable_content "char
get_state_variable_content() const

A character indicating the integrated variable (either 'V' for Volume
or 'h' for head). ";

%feature("docstring")
cmf::upslope::SoilLayer::set_state_variable_content "void
set_state_variable_content(char content)

A character indicating the integrated variable (either 'V' for Volume
or 'h' for head). ";

%feature("docstring")  cmf::upslope::SoilLayer::is_storage "virtual
bool is_storage() const

Returns true, since this is a storage. ";

%feature("docstring")  cmf::upslope::SoilLayer::get_states "cmf::math::state_queue get_states()

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";

%feature("docstring")  cmf::upslope::SoilLayer::Solute "SoluteStorage& Solute(const cmf::water::solute &_Solute)

Returns the water quality of the water storage. ";

%feature("docstring")  cmf::upslope::SoilLayer::Solute "const
SoluteStorage& Solute(const cmf::water::solute &_Solute) const ";

%feature("docstring")  cmf::upslope::SoilLayer::conc "real conc(const
cmf::water::solute &_Solute) const

Returns the concentration of the given solute. ";

%feature("docstring")  cmf::upslope::SoilLayer::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute) const

Returns the current WaterQuality (concentration of all solutes). ";

%feature("docstring")  cmf::upslope::SoilLayer::conc "void conc(const
cmf::water::solute &_Solute, real NewConcetration)

Sets a new concentration. ";

%feature("docstring")  cmf::upslope::SoilLayer::set_volume "virtual
void set_volume(real newwatercontent)

Sets the volume of water in this storage in m3. ";

%feature("docstring")  cmf::upslope::SoilLayer::Derivate "virtual
real Derivate(const cmf::math::Time &time)

Returns the derivate of the state variable at time time. ";

%feature("docstring")  cmf::upslope::SoilLayer::get_state "real
get_state() const

Returns the current state of the variable. ";

%feature("docstring")  cmf::upslope::SoilLayer::set_state "void
set_state(real newState)

Gives access to the state variable. ";

%feature("docstring")  cmf::upslope::SoilLayer::StateIsChanged "bool
StateIsChanged()

Returns if the state was currently updated. ";

%feature("docstring")  cmf::upslope::SoilLayer::project "const
cmf::project& project() const

Returns the project, this node is part of. ";

%feature("docstring")  cmf::upslope::SoilLayer::to_string "virtual
std::string to_string() const ";

%feature("docstring")  cmf::upslope::SoilLayer::get_connections "cmf::water::connection_vector get_connections() const ";

%feature("docstring")  cmf::upslope::SoilLayer::get_connection "cmf::water::flux_connection* get_connection(const
cmf::water::flux_node &target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::upslope::SoilLayer::remove_connection "bool remove_connection(cmf::water::flux_node::ptr To)

Remove the connection. ";

%feature("docstring")  cmf::upslope::SoilLayer::flux_to "real
flux_to(const cmf::water::flux_node &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from target into this\"). ";

%feature("docstring")  cmf::upslope::SoilLayer::flux3d_to "cmf::geometry::point flux3d_to(const cmf::water::flux_node &target,
cmf::math::Time t) ";

%feature("docstring")  cmf::upslope::SoilLayer::get_3d_flux "cmf::geometry::point get_3d_flux(cmf::math::Time t) ";

%feature("docstring")  cmf::upslope::SoilLayer::water_balance "real
water_balance(cmf::math::Time t, const flux_connection *Without=0)
const

Returns the sum of all fluxes (positive and negative) at time t.
Single fluxes can be excluded from the calculation

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the water_balance
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

%feature("docstring")  cmf::water::solute_vector::size "size_t size()
const ";

%feature("docstring")  cmf::water::solute_vector::solute_vector "solute_vector(std::string str)

Creates a solute vector from solute names, separated by whitespace.
E.g. solutes=solute_vector(\"dO18 dH2\"). ";

%feature("docstring")  cmf::water::solute_vector::begin "const_iterator begin() const ";

%feature("docstring")  cmf::water::solute_vector::end "const_iterator
end() const ";

%feature("docstring")  cmf::water::solute_vector::get_solute "cmf::water::solute* get_solute(int position) ";


// File: classcmf_1_1water_1_1_solute_storage.xml
%feature("docstring") cmf::water::SoluteStorage "

A class for the storage of any tracer. The state is the amount (mol,
kg etc. see cmf::water) of the tracer in the storage.

The derivative function is given by: \\\\begin{eqnarray*} ///
\\\\frac{dX}{dt}&=&\\\\sum_{f=1}^{F}\\\\left( q_f [X]_f\\\\right) +
X_{in} - r^-X \\\\left[\\\\frac{mol}{day}\\\\right]\\\\\\\\ /// F&=&
\\\\mbox{Number of fluxes in water storage} \\\\\\\\ /// q_f&=&
\\\\mbox{Water flux in } \\\\frac{m^3}{day} \\\\\\\\ ///
\\\\left[X\\\\right]_f &=& \\\\mbox{Concentration of solute X in flux
}q_f \\\\mbox{ in } \\\\frac{mol}{m^3} \\\\\\\\ /// X_{in} &=&
\\\\mbox{Absolute source or sink term} \\\\frac{mol}{day} \\\\\\\\ ///
r^- &=& \\\\mbox{Decay rate} \\\\frac 1{day} \\\\\\\\ /// V &=&
\\\\mbox{Volume of water in water storage }\\\\left[m^3\\\\right] ///
\\\\end{eqnarray*}

C++ includes: SoluteStorage.h ";

%feature("docstring")  cmf::water::SoluteStorage::conc "real conc()
const

Returns the concentration of the solute. ";

%feature("docstring")  cmf::water::SoluteStorage::Derivate "virtual
real Derivate(const cmf::math::Time &time)

Returns the derivate of the state variable at time time. ";

%feature("docstring")  cmf::water::SoluteStorage::get_state "real
get_state() const

Returns the current state of the variable. ";

%feature("docstring")  cmf::water::SoluteStorage::set_state "void
set_state(real newState)

Gives access to the state variable. ";

%feature("docstring")  cmf::water::SoluteStorage::StateIsChanged "bool StateIsChanged()

Returns if the state was currently updated. ";


// File: classcmf_1_1water_1_1_solute_timeseries.xml
%feature("docstring") cmf::water::SoluteTimeseries "

A map of concentration time series for solutes.

C++ includes: Solute.h ";

%feature("docstring")  cmf::water::SoluteTimeseries::begin "iterator
begin() ";

%feature("docstring")  cmf::water::SoluteTimeseries::end "iterator
end() ";

%feature("docstring")  cmf::water::SoluteTimeseries::begin "const_iterator begin() const ";

%feature("docstring")  cmf::water::SoluteTimeseries::end "const_iterator end() const ";

%feature("docstring")  cmf::water::SoluteTimeseries::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute) const ";

%feature("docstring")  cmf::water::SoluteTimeseries::size "size_t
size() const

Returns the number of solutes in the solution. ";

%feature("docstring")  cmf::water::SoluteTimeseries::SoluteTimeseries
"SoluteTimeseries() ";

%feature("docstring")  cmf::water::SoluteTimeseries::SoluteTimeseries
"SoluteTimeseries(const solute_vector &solutes, cmf::math::Time
begin, cmf::math::Time step) ";

%feature("docstring")  cmf::water::SoluteTimeseries::SoluteTimeseries
"SoluteTimeseries(const cmf::water::SoluteTimeseries &sts) ";


// File: classcmf_1_1math_1_1_solute_water_integrator.xml
%feature("docstring") cmf::math::SoluteWaterIntegrator "

A SoluteWaterIntegrator implements the cmf::math::Integrator
interface, but consists of two independent ODE-solvers. Added
statevariables are sorted by waterstorages and solute storages and
assigned to the correct solver.

C++ includes: WaterSoluteIntegrator.h ";

/*  The state variables to integrate  */

/*

*/

%feature("docstring")  cmf::math::SoluteWaterIntegrator::CopyStates "void CopyStates(num_array &destination) const

Copies the states to a numeric vector using use_OpenMP. ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::CopyStates "void CopyStates(real *destination) const ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::SetStates "void SetStates(const num_array &newStates)

Copies the new states to the actual states. ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::SetStates "void SetStates(real *newStates) ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::CopyDerivs "void CopyDerivs(Time time, num_array &destination, real factor=1)
const

Copies the derivatives at time step \"time\" to a numeric vector using
use_OpenMP

Parameters:
-----------

time:   Time at which the derivatives should be calculated

destination:  Vector to be overwritten by the results

factor:  A factor that is multiplied to the derivate (e.g. unit
conversion or integration length) ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::CopyDerivs "void CopyDerivs(Time time, real *destination, real factor=1) const

Copies the derivatives at time step \"time\" to an preallocated c
array

Parameters:
-----------

time:   Time at which the derivatives should be calculated

destination:  Allocated c array

factor:  A factor that is multiplied to the derivate (e.g. unit
conversion or integration length) ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::GetStates "num_array GetStates() const

Returns the states in a numeric vector using :CopyStates, but is
slower because of additional memory allocation. ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::GetDerivs "num_array GetDerivs(Time time) const

Returns the derivatives at time step \"time\" in a numeric vector
using :CopyDerivs, but is slower because of additional memory
allocation. ";

%feature("docstring")
cmf::math::SoluteWaterIntegrator::AddValuesToStates "void
AddValuesToStates(const num_array &operands) ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::AddState "virtual void AddState(cmf::math::StateVariable::ptr state) ";

/*  Accuracy parameters  */

/*

*/

/*  model time  */

/*

*/

%feature("docstring")  cmf::math::SoluteWaterIntegrator::ModelTime "cmf::math::Time ModelTime() const

Returns the current model time. ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::ModelTime "void ModelTime(cmf::math::Time val)

Sets the current model time. ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::TimeStep "cmf::math::Time TimeStep() const

Returns the last time step. ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::NextTimeStep
"cmf::math::Time NextTimeStep() const

Returns the next time step width. ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::NextTimeStep
"void NextTimeStep(cmf::math::Time val)

Sets the next time step width. ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::MinTimestep "const cmf::math::Time MinTimestep() const

Returns The minimal allowed time step length. ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::Iterations "int Iterations() const ";

%feature("docstring")
cmf::math::SoluteWaterIntegrator::ResetIterations "void
ResetIterations() ";

/*  Constructors and Destructors  */

/*

*/

/*  Integrate  */

/*

*/

%feature("docstring")  cmf::math::SoluteWaterIntegrator::Integrate "int Integrate(cmf::math::Time MaxTime) ";

%feature("docstring")
cmf::math::SoluteWaterIntegrator::IntegrateUntil "void
IntegrateUntil(cmf::math::Time MaxTime) ";

%feature("docstring")
cmf::math::SoluteWaterIntegrator::IntegrateUntil "void
IntegrateUntil(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables until MaxTime. ";

%feature("docstring")
cmf::math::SoluteWaterIntegrator::get_water_integrator "cmf::math::Integrator* get_water_integrator() const ";

%feature("docstring")
cmf::math::SoluteWaterIntegrator::get_solute_integrator "cmf::math::Integrator* get_solute_integrator() const ";

%feature("docstring")
cmf::math::SoluteWaterIntegrator::set_water_integrator "void
set_water_integrator(cmf::math::Integrator *templ) ";

%feature("docstring")
cmf::math::SoluteWaterIntegrator::set_solute_integrator "void
set_solute_integrator(cmf::math::Integrator *templ) ";

%feature("docstring")
cmf::math::SoluteWaterIntegrator::AddStatesFromOwner "void
AddStatesFromOwner(cmf::math::StateVariableOwner &stateOwner) ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::Integrate "int Integrate(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables

Parameters:
-----------

MaxTime:  To stop the model (if running in a model framework) at time
steps of value exchange e.g. full hours, the next value exchange time
can be given

TimeStep:  Takes the proposed timestep, and changes it into the
effictivly used timestep according to the local stiffness of the
problem and MaxTime ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::Copy "virtual cmf::math::SoluteWaterIntegrator* Copy() const

Returns a new Integrator, based on this (without the state variables),
e.g. same type, epsilon, model time etc. ";

%feature("docstring")
cmf::math::SoluteWaterIntegrator::SoluteWaterIntegrator "SoluteWaterIntegrator(const cmf::math::Integrator &water_integrator,
const cmf::math::Integrator &solute_integrator) ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::Reset "virtual void Reset() ";

%feature("docstring")
cmf::math::SoluteWaterIntegrator::SoluteWaterIntegrator "SoluteWaterIntegrator(const cmf::math::Integrator &water_integrator,
const cmf::math::Integrator &solute_integrator,
cmf::math::StateVariableOwner &states) ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::size "int
size() const

returns the number of state variables ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::get_state "real get_state(int position) const

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::set_state "void set_state(int position, real newState)

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::get_states "state_queue get_states()

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";


// File: classcmf_1_1math_1_1state__queue.xml
%feature("docstring") cmf::math::state_queue "";

%feature("docstring")  cmf::math::state_queue::begin "state_deque::iterator begin() ";

%feature("docstring")  cmf::math::state_queue::end "state_deque::iterator end() ";

%feature("docstring")  cmf::math::state_queue::begin "state_deque::const_iterator begin() const ";

%feature("docstring")  cmf::math::state_queue::end "state_deque::const_iterator end() const ";

%feature("docstring")  cmf::math::state_queue::push "void
push(StateVariable::ptr sv) ";

%feature("docstring")  cmf::math::state_queue::push "void
push(StateVariableOwner &svo) ";

%feature("docstring")  cmf::math::state_queue::pop "StateVariable::ptr pop() ";

%feature("docstring")  cmf::math::state_queue::front "StateVariable::ptr front() const ";

%feature("docstring")  cmf::math::state_queue::eat "void
eat(state_queue &food) ";

%feature("docstring")  cmf::math::state_queue::size "size_t size()
const ";


// File: classcmf_1_1math_1_1_state_variable.xml
%feature("docstring") cmf::math::StateVariable "

Abstract class state variable

Simple exponential system class header implementing a state variable:

C++ includes: statevariable.h ";

%feature("docstring")  cmf::math::StateVariable::Derivate "virtual
real Derivate(const cmf::math::Time &time)=0

Returns the derivate of the state variable at time time. ";

%feature("docstring")  cmf::math::StateVariable::get_state "real
get_state() const

Returns the current state of the variable. ";

%feature("docstring")  cmf::math::StateVariable::set_state "void
set_state(real newState)

Gives access to the state variable. ";

%feature("docstring")  cmf::math::StateVariable::StateIsChanged "bool
StateIsChanged()

Returns if the state was currently updated. ";

%feature("docstring")  cmf::math::StateVariable::StateVariable "StateVariable(real InitialState=0)

ctor ";


// File: classcmf_1_1math_1_1_state_variable_owner.xml
%feature("docstring") cmf::math::StateVariableOwner "

An abstract class, that owns one or more state variables, that can add
them to a vector of state variables in a certain order.

C++ includes: statevariable.h ";

%feature("docstring")  cmf::math::StateVariableOwner::get_states "virtual state_queue get_states()=0

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";


// File: classcmf_1_1upslope_1_1subcatchment.xml
%feature("docstring") cmf::upslope::subcatchment "

A class to structure cells in a project using their main outlets.

C++ includes: algorithm.h ";

%feature("docstring")  cmf::upslope::subcatchment::subcatchment "subcatchment(cmf::upslope::Cell &pourpoint, double
area_threshold=1e308)

Creates a subcatchment from a pourpoint cell

Parameters:
-----------

pourpoint:  The pourpoint (outlet of the subcatchment)

area_threshold:  Minimum contributing area size to form a
subcatchment. When area_threshold > area(cells), all upslope cells of
pourpoint are used. ";


// File: classcmf_1_1upslope_1_1_e_t_1_1surface__water__evaporation.xml
%feature("docstring") cmf::upslope::ET::surface_water_evaporation "

Flux connection using an surface_water_evaporation_method.

C++ includes: ET.h ";

%feature("docstring")
cmf::upslope::ET::surface_water_evaporation::surface_water_evaporation
"surface_water_evaporation(cmf::river::OpenWaterStorage::ptr source,
cmf::water::flux_node::ptr ET_target, std::tr1::shared_ptr<
surface_water_evaporation_method > _method, std::string method_name)
";

%feature("docstring")  cmf::upslope::ET::surface_water_evaporation::q
"real q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::ET::surface_water_evaporation::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::ET::surface_water_evaporation::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::ET::surface_water_evaporation::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::ET::surface_water_evaporation::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::ET::surface_water_evaporation::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::ET::surface_water_evaporation::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")
cmf::upslope::ET::surface_water_evaporation::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::upslope::ET::surface_water_evaporation::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::ET::surface_water_evaporation::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")
cmf::upslope::ET::surface_water_evaporation::to_string "std::string
to_string() const ";

%feature("docstring")
cmf::upslope::ET::surface_water_evaporation::short_string "std::string short_string() const ";


// File: classcmf_1_1upslope_1_1_e_t_1_1surface__water__evaporation__method.xml
%feature("docstring")
cmf::upslope::ET::surface_water_evaporation_method "

Abstract class. Child classes are defining a method for surface water
evaporation calculation.

C++ includes: ET.h ";

%feature("docstring")
cmf::upslope::ET::surface_water_evaporation_method::evap_from_openwater
"virtual double
evap_from_openwater(cmf::river::OpenWaterStorage::ptr,
cmf::math::Time)=0 ";


// File: classcmf_1_1upslope_1_1connections_1_1_s_w_a_t_percolation.xml
%feature("docstring") cmf::upslope::connections::SWATPercolation "

A tipping bucket percolation approach similar to the approach in SWAT

\\\\begin{eqnarray*} q_{perc} &=&
V_{H_2O,drain}\\\\left(1-e^{-\\\\frac 1{-TT_{perc}}}\\\\right)
\\\\\\\\ /// TT_{perc} &=& \\\\frac{V_{pores,drain} - V_{field
cap.}}{K_{sat}} \\\\\\\\ /// V_{x,drain} &=& V_{x,drain} - V_{field
cap.} \\\\end{eqnarray*}

C++ includes: Percolation.h ";

%feature("docstring")
cmf::upslope::connections::SWATPercolation::SWATPercolation "SWATPercolation(cmf::upslope::SoilLayer::ptr upperLayer,
cmf::upslope::SoilLayer::ptr lowerLayer) ";

%feature("docstring")  cmf::upslope::connections::SWATPercolation::q "real q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::SWATPercolation::left_node "flux_node::ptr
left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::connections::SWATPercolation::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::SWATPercolation::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::SWATPercolation::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::SWATPercolation::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::SWATPercolation::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")
cmf::upslope::connections::SWATPercolation::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::SWATPercolation::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::connections::SWATPercolation::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")
cmf::upslope::connections::SWATPercolation::to_string "std::string
to_string() const ";

%feature("docstring")
cmf::upslope::connections::SWATPercolation::short_string "std::string
short_string() const ";


// File: classcmf_1_1river_1_1_s_w_a_t_reach_type.xml
%feature("docstring") cmf::river::SWATReachType "

Structure for the description of structural parameters of a reach.

Uses the SWAT IChannel geometry (see SWAT Theoretical Documentation,
Version 2005 (ch. 7:1.1), Neitsch et al. 2005), in this class
referenced as SWATtheoDoc. Differences to the SWAT geometry: The flood
plain is not plain, but has a small slope=0.5%, but has an infinite
width

C++ includes: ReachType.h ";

%feature("docstring")  cmf::river::SWATReachType::get_length "double
get_length() const

Length of the reach. ";

%feature("docstring")  cmf::river::SWATReachType::typecode "char
typecode() const ";

%feature("docstring")  cmf::river::SWATReachType::get_channel_width "virtual double get_channel_width(double depth) const

Calculates the flow width from a given actual depth [m] using the
actual IChannel geometry \\\\begin{eqnarray*} /// \\\\mbox{If }
d\\\\le d_{IChannel} && \\\\\\\\ /// w &=& w_{bottom} + 2
\\\\Delta_{bank} d \\\\\\\\ /// \\\\mbox{else, } && \\\\mbox{if the
river floods the flood plain} \\\\\\\\ /// w &=& w_{bank} + 2
\\\\Delta_{Floodplain} (d-d_{IChannel} \\\\\\\\ ///
\\\\end{eqnarray*}.

See:  SWATtheoDoc eq. 7:1.1.3 ";

%feature("docstring")  cmf::river::SWATReachType::get_wetted_perimeter
"virtual double get_wetted_perimeter(double depth) const

Calculates the wetted perimeter from a given actual depth [m] using
the actual IChannel geometry \\\\begin{eqnarray*} /// \\\\mbox{If }
d\\\\le d_{IChannel} && \\\\\\\\ /// P &=& w_{bottom} + 2 \\\\sqrt{1+
{\\\\Delta_{bank}}^2} d \\\\\\\\ /// \\\\mbox{else, } && \\\\mbox{if
the river floods the flood plain} \\\\\\\\ /// P &=& P(d_{IChannel} +
2 \\\\sqrt{1+ {\\\\Delta_{flood\\\\ plain}}^2} (d-d_{IChannel})
\\\\\\\\ /// \\\\end{eqnarray*}.

See:  SWATtheoDoc eq. 7:1.1.5

Parameters:
-----------

depth:  Actual depth of the reach [m] ";

%feature("docstring")  cmf::river::SWATReachType::get_depth "virtual
double get_depth(double area) const

Calculates the actual depth of the reach using the IChannel geometry
\\\\begin{eqnarray*} /// d &=& \\\\sqrt{\\\\frac{A}{\\\\Delta_{bank}}
+ \\\\frac{{w_{bottom}}^2}{4 {\\\\Delta_{bank}}^2}} -
\\\\frac{w_{bottom}}{2 \\\\Delta_{bank}} \\\\\\\\ /// \\\\mbox{If }
d>d_{IChannel} &&\\\\\\\\ ///
d&=&d_{IChannel}+\\\\sqrt{\\\\frac{A-A(d_{IChannel})}{\\\\Delta_{flood\\\\
plain}} + \\\\frac{{w(d_{IChannel})}^2}{4 {\\\\Delta_{flood\\\\
plain}}^2}} - \\\\frac{w(d_{IChannel})}{2 \\\\Delta_{flood\\\\ plain}}
\\\\\\\\ /// \\\\end{eqnarray*}.

See:  SWATtheoDoc eq. 7:1.2.4

get_depth of the reach [m]

Parameters:
-----------

area:  Wetted area of a river cross section [m2], can be obtained by
V/l, where V is the stored volume and l is the reach length ";

%feature("docstring")  cmf::river::SWATReachType::get_flux_crossection
"virtual double get_flux_crossection(double depth) const

Calculates the wetted area from a given depth using the IChannel
geometry. In most cases use get_flux_crossection=V/l, where V is the
stored volume and l is the reach length \\\\begin{eqnarray*} ///
\\\\mbox{If } d>d_{IChannel} &&\\\\\\\\ /// A &=& \\\\left(w_{bottom}
+ \\\\Delta_{bank} d\\\\right) d \\\\\\\\ /// \\\\mbox{else, } &&
\\\\mbox{if the river floods the flood plain} \\\\\\\\ /// A &=&
A(d_{IChannel}) + \\\\left(w(d_{IChannel} + \\\\Delta_{flood\\\\
plain} \\\\left(d-d_{IChannel}\\\\right)\\\\right) (d-d_{IChannel})
\\\\\\\\ /// \\\\end{eqnarray*}.

See:  SWATtheoDoc eq. 7:1.1.4

Wetted area of a river cross section [m2]

Parameters:
-----------

depth:  Depth of the reach [m] ";

%feature("docstring")  cmf::river::SWATReachType::SWATReachType "SWATReachType(double l)

Creates a new reach structure with standard values (small natural
river) BottomWidth = 3m, ChannelDepth = 0.5m, BankSlope = 2, nManning
= 0.0035, FloodPlainSlope = 200 ";

%feature("docstring")  cmf::river::SWATReachType::SWATReachType "SWATReachType(double l, double BankWidth, double Depth)

Creates a new reach structure from a give width and depth

Parameters:
-----------

l:  length of the channel [m]

BankWidth:  get_channel_width of the reach from bank to bank [m]

Depth:  Depth of the reach [m] ";

%feature("docstring")  cmf::river::SWATReachType::copy "SWATReachType* copy() const ";

%feature("docstring")  cmf::river::SWATReachType::get_nManning "virtual double get_nManning() const ";

%feature("docstring")  cmf::river::SWATReachType::set_nManning "virtual void set_nManning(double val) ";

%feature("docstring")  cmf::river::SWATReachType::h "virtual double
h(double V) const

Returns the depth of a given volume. ";

%feature("docstring")  cmf::river::SWATReachType::A "virtual double
A(double V) const

Returns the area of the surface for a given volume. ";

%feature("docstring")  cmf::river::SWATReachType::V "virtual double
V(double h) const ";

%feature("docstring")  cmf::river::SWATReachType::qManning "double
qManning(double A, double slope) const

Calculates the flow rate from a given water volume in the reach
\\\\begin{eqnarray*} /// q_{Manning}&=& A R^{\\\\frac 2 3}
\\\\sqrt{\\\\frac {\\\\Delta_z} n} \\\\\\\\ /// A &=& \\\\frac V l
\\\\mbox{, (Crosssectional area of the wetted crossection, Volume per
length)} \\\\\\\\ /// R &=& \\\\frac A {P(d)} \\\\\\\\ /// P(d) &=&
\\\\mbox{ the perimeter of the wetted crosssection, a function of
reach depth} \\\\\\\\ /// d(V) &=& \\\\mbox{ the depth of the reach a
function of the volume} \\\\\\\\ /// \\\\Delta_z &=& \\\\frac{z_{max}
- z_{min}}{l} \\\\mbox{ Slope of the reach} /// \\\\end{eqnarray*}
Flow rate [m3/s]

Parameters:
-----------

A:  The area of the cross section [m2]

slope:  The slope of the reach [m/m] ";


// File: classcmf_1_1water_1_1_technical_flux.xml
%feature("docstring") cmf::water::TechnicalFlux "

Produces a constant but changeable flux from a source to a target, if
enough water is present in the source

$ q=\\\\left\\\\{0 \\\\mbox{ if }V_{source}\\\\le V_{min}\\\\\\\\
\\\\frac{V_{source} - V_{min}}{t_{decr} q_{0} - V_{min}}\\\\mbox{ if }
V_{source} t_{decr} q_{0}\\\\\\\\ q_{0} \\\\mbox{ else}\\\\le
\\\\right. $

C++ includes: boundary_condition.h ";

%feature("docstring")  cmf::water::TechnicalFlux::TechnicalFlux "TechnicalFlux(std::tr1::shared_ptr< cmf::water::WaterStorage >
&source, std::tr1::shared_ptr< cmf::water::flux_node > target, real
maximum_flux, real minimal_state=0, cmf::math::Time
flux_decrease_time=cmf::math::h)

Produces a constant but changeable flux from a source to a target, if
enough water is present in the source

Parameters:
-----------

source:  The source of the water

target:  The target of the water

maximum_flux:  The requested flux $q_{0}$

minimal_state:  Minimal volume of stored water in source

flux_decrease_time:  ( cmf::math::Time) ";

%feature("docstring")  cmf::water::TechnicalFlux::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::water::TechnicalFlux::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::water::TechnicalFlux::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::water::TechnicalFlux::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::water::TechnicalFlux::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::water::TechnicalFlux::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")  cmf::water::TechnicalFlux::exchange_target "void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")  cmf::water::TechnicalFlux::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")  cmf::water::TechnicalFlux::get_tracer_filter "real get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")  cmf::water::TechnicalFlux::set_tracer_filter "void set_tracer_filter(real value) ";

%feature("docstring")  cmf::water::TechnicalFlux::to_string "std::string to_string() const ";

%feature("docstring")  cmf::water::TechnicalFlux::short_string "std::string short_string() const ";


// File: classcmf_1_1math_1_1_time.xml
%feature("docstring") cmf::math::Time "

A time class, used to pass around current modelling times

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

&gt, &lt, ==, !=

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

/*

*/

%feature("docstring")  cmf::math::Time::Time "Time(double days)

Conversion constructor. ";

%feature("docstring")  cmf::math::Time::Time "Time(int day, int
month, int year=2001, int hour=0, int minute=0, int second=0, int
ms=0)

Construction from date and stores the time as Excel-Time (0 =
31.12.1899 0:00:00). ";

%feature("docstring")  cmf::math::Time::Time "Time(Date date)

Conversion constructor. ";

%feature("docstring")  cmf::math::Time::Time "Time(const Time &t)

Copy constructor. ";

%feature("docstring")  cmf::math::Time::Time "Time()

Standard constructor. ";

/*  Time unit conversion  */

/*

*/

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

/*

*/

%feature("docstring")  cmf::math::Time::times_in "long long
times_in(const Time &t1) const

Returns the number of times this is included in t1. ";

/*  Boolean Operators  */

/*

*/


// File: classcmf_1_1math_1_1timeseries.xml
%feature("docstring") cmf::math::timeseries "

A timeseries is a list of values, equally distributed over time. To
create one, one have to provide as start date and a step size. The end
time is calculated from the number of values. Values queried for times
before the start time are returned as the first item, values after the
end time equal the last item. A timeseries with only one item reacts
like a scalar value.Creating a time series

import CMFLib as cmf                      Start date is the January
5th 2001 at 2:30 pm start=cmf.Time(5,1,2001,14,30) time step of the
timeseries is 20 minutes step=cmf.min*20                      Type of
interpolation between values                      0 - Nearest
neighbor, 1 - Linear,                      2 - Squared,
3 - Cubic, etc.                    interpolation=1 Create timeseries
timeseries=cmf.timeseries(start,step,interpolation) add data
timeseries.add(0.1) # Value at 2001/5/1 2:30pm is 0.1
timeseries.add(0.2) # Value at 2001/5/1 2:50pm is 0.2
timeseries.add(0.1) # Value at 2001/5/1 3:10pm is 0.1

With this technic it is simple to read files or databases to fill
timeseries.Using a timeseries

Query every minute between 2:15 and 3:14 pm for t in
cmf.timerange(start,start+cmf.h,cmf.min): print
\"Time:\",t.AsDate(),\"Value:\",timeseries[t] Query a specific
position of the timeseries                    print timeseries[2]

C++ includes: timeseries.h ";

/*  Operators  */

/*  Binary operators defined as free operators:

x = {+,-,*,/}

Defined for (x is one of the operators above): timeseries = timeseries
x timeseries

timeseries = double x timeseries

timeseries = timeseries x double

*/

%feature("docstring")  cmf::math::timeseries::inv "timeseries inv()
const ";

%feature("docstring")  cmf::math::timeseries::reduce_min "timeseries
reduce_min(cmf::math::Time begin, cmf::math::Time step) const ";

%feature("docstring")  cmf::math::timeseries::reduce_max "timeseries
reduce_max(cmf::math::Time begin, cmf::math::Time step) const

Creates a timeseries with a bigger timestep, containing the minimum.
";

%feature("docstring")  cmf::math::timeseries::reduce_sum "timeseries
reduce_sum(cmf::math::Time begin, cmf::math::Time step) const

Creates a timeseries with a bigger timestep, containing the maximum.
";

%feature("docstring")  cmf::math::timeseries::reduce_avg "timeseries
reduce_avg(cmf::math::Time begin, cmf::math::Time step) const

Creates a timeseries with a bigger timestep, containing the sum. ";

%feature("docstring")  cmf::math::timeseries::floating_avg "timeseries floating_avg(cmf::math::Time window_width) const

Creates a timeseries with a bigger timestep, containing the average.
";

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

%feature("docstring")  cmf::math::timeseries::begin "cmf::math::Time
begin() const

Order of the interpolation.

First date of measurement ";

%feature("docstring")  cmf::math::timeseries::step "cmf::math::Time
step() const

Time between the measurements. ";

%feature("docstring")  cmf::math::timeseries::end "cmf::math::Time
end() const

Last date of measurements. ";

%feature("docstring")  cmf::math::timeseries::interpolationpower "int
interpolationpower() const

Method for the interpolation (0 - Nearest neighbor, 1- linear, 2 -
cubic spline (not implemented yet). ";

%feature("docstring")  cmf::math::timeseries::add "void add(double
Value)

Appends a measurement. ";

%feature("docstring")  cmf::math::timeseries::is_empty "bool
is_empty() const

returns true if no values are added to the timeseries ";

%feature("docstring")  cmf::math::timeseries::clear "void clear() ";

%feature("docstring")  cmf::math::timeseries::adress "size_t adress()
const ";

%feature("docstring")  cmf::math::timeseries::copy "timeseries copy()
const ";

%feature("docstring")  cmf::math::timeseries::size "int size() const
";

%feature("docstring")  cmf::math::timeseries::timeseries "timeseries(cmf::math::Time _begin, cmf::math::Time _step, int
_interpolationmethod=1)

Constructor of a time series

Parameters:
-----------

_begin:  First date of measurement

_step:   Time between measurements

_interpolationmethod:  Method for the interpolation (0 - Nearest
neighbor, 1- linear, 2 - cubic spline (not implemented yet) ";

%feature("docstring")  cmf::math::timeseries::timeseries "timeseries() ";

%feature("docstring")  cmf::math::timeseries::timeseries "timeseries(const cmf::math::timeseries &ts) ";

%feature("docstring")  cmf::math::timeseries::timeseries "timeseries(double scalar) ";

%feature("docstring")  cmf::math::timeseries::get_t "double
get_t(cmf::math::Time t) const ";

%feature("docstring")  cmf::math::timeseries::get_i "double get_i(int
i) const ";

%feature("docstring")  cmf::math::timeseries::set_t "void
set_t(cmf::math::Time t, double value) ";

%feature("docstring")  cmf::math::timeseries::set_i "void set_i(int
i, double value) ";

%feature("docstring")  cmf::math::timeseries::get_slice "cmf::math::timeseries get_slice(cmf::math::Time _begin,
cmf::math::Time _end, cmf::math::Time _step=cmf::math::Time()) ";

%feature("docstring")  cmf::math::timeseries::set_slice "void
set_slice(cmf::math::Time _begin, cmf::math::Time _end,
cmf::math::timeseries values) ";

%feature("docstring")  cmf::math::timeseries::get_slice "cmf::math::timeseries get_slice(int _begin, int _end, int step=1) ";

%feature("docstring")  cmf::math::timeseries::set_slice "void
set_slice(int _begin, int _end, cmf::math::timeseries _values) ";

%feature("docstring")  cmf::math::timeseries::remove_nodata "void
remove_nodata(double nodata_value) ";


// File: structcmf_1_1math_1_1timeseries_1_1timeseries__data.xml


// File: classcmf_1_1upslope_1_1connections_1_1_topographic_gradient_darcy.xml
%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy "

Calculates the lateral flow using the topographic gradient

\\\\[ q_{lat} = \\\\frac{z_1 - z_2}{\\\\|C_1-C_2\\\\|} \\\\frac 12
(T(C_1)+T(C_2)) w \\\\] where  $ z_1 - z_2 $ is the topographic height
difference

$ \\\\|C_1-C_2\\\\| $ is the distance from Cell 1 to Cell 2

$ T(C) $ is the transmissivity of cell C, calculated by
SoilType::Transmissivity

$ w $ is the width of the connection of the cells

C++ includes: subsurfacefluxes.h ";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::TopographicGradientDarcy
"TopographicGradientDarcy(cmf::upslope::SoilLayer::ptr left,
cmf::water::flux_node::ptr right, real FlowWidth, real Distance=0) ";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::q "real q(const
flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::exchange_target "void exchange_target(flux_node::ptr oldtarget, flux_node::ptr
newTarget) ";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::get_tracer_filter
"real get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::set_tracer_filter
"void set_tracer_filter(real value) ";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::to_string "std::string to_string() const ";

%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy::short_string "std::string short_string() const ";


// File: classcmf_1_1upslope_1_1_topology.xml
%feature("docstring") cmf::upslope::Topology "

represents the connectivity of cells to each other

C++ includes: Topology.h ";

%feature("docstring")  cmf::upslope::Topology::get_position "cmf::geometry::point get_position() const

Returns the center of the cell. ";

%feature("docstring")  cmf::upslope::Topology::flowwidth "double
flowwidth(Cell &target) ";

%feature("docstring")  cmf::upslope::Topology::flowwidth "double
flowwidth(Topology &target) ";

%feature("docstring")  cmf::upslope::Topology::AddNeighbor "void
AddNeighbor(Cell &target, double flowwidth) ";

%feature("docstring")  cmf::upslope::Topology::AddNeighbor "void
AddNeighbor(Topology &target, double flowwidth) ";

%feature("docstring")  cmf::upslope::Topology::RemoveNeighbor "void
RemoveNeighbor(Topology &target) ";

%feature("docstring")  cmf::upslope::Topology::neighbor_count "size_t
neighbor_count() const ";

%feature("docstring")  cmf::upslope::Topology::MainOutlet "Cell*
MainOutlet(bool forceRecalc=false) ";

%feature("docstring")  cmf::upslope::Topology::ContributingArea "double ContributingArea() const ";

%feature("docstring")  cmf::upslope::Topology::set_position "virtual
void set_position(cmf::geometry::point p)

Sets the location. If not implemented by the child class, an exception
is thrown. ";

%feature("docstring")  cmf::upslope::Topology::get_distance_to "double get_distance_to(const Locatable &cmp)

Returns the distance between two locatable objects. ";

%feature("docstring")  cmf::upslope::Topology::get_direction_to "cmf::geometry::point get_direction_to(const Locatable &cmp)

Returns a vector with length=1 pointing in the direction of another
Locatable. ";


// File: classcmf_1_1upslope_1_1_e_t_1_1transpiration.xml
%feature("docstring") cmf::upslope::ET::transpiration "

Flux connection using a transpiration_method.

C++ includes: ET.h ";

%feature("docstring")  cmf::upslope::ET::transpiration::transpiration
"transpiration(cmf::upslope::SoilLayer::ptr source,
cmf::water::flux_node::ptr ET_target, std::tr1::shared_ptr<
transpiration_method > _method, std::string method_name) ";

%feature("docstring")  cmf::upslope::ET::transpiration::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::ET::transpiration::left_node "flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::upslope::ET::transpiration::right_node "flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::upslope::ET::transpiration::kill_me "bool
kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::upslope::ET::transpiration::get_target "flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::ET::transpiration::get_target "flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::ET::transpiration::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::ET::transpiration::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::upslope::ET::transpiration::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::ET::transpiration::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")  cmf::upslope::ET::transpiration::to_string "std::string to_string() const ";

%feature("docstring")  cmf::upslope::ET::transpiration::short_string "std::string short_string() const ";


// File: classcmf_1_1upslope_1_1_e_t_1_1transpiration__method.xml
%feature("docstring") cmf::upslope::ET::transpiration_method "

Abstract class. Child classes are defining a method for transpiration
calculation.

C++ includes: ET.h ";

%feature("docstring")
cmf::upslope::ET::transpiration_method::transp_from_layer "virtual
double transp_from_layer(cmf::upslope::SoilLayer::ptr, cmf::math::Time
t)=0

returns the transpiration rate from one layer in m3/day ";


// File: classcmf_1_1river_1_1_triangular_reach.xml
%feature("docstring") cmf::river::TriangularReach "

Structure for the description of reaches with a triangular cross
section.

Although double triangular cross section reach are rarely met, a
triangular reach does scale with its water load, and is therefore
preferable in case where nothing about IChannel geometry is known

C++ includes: ReachType.h ";

%feature("docstring")  cmf::river::TriangularReach::get_length "double get_length() const

Length of the reach. ";

%feature("docstring")  cmf::river::TriangularReach::typecode "char
typecode() const ";

%feature("docstring")  cmf::river::TriangularReach::get_channel_width
"virtual double get_channel_width(double depth) const

Returns the width of the stream at a given depth \\\\begin{eqnarray*}
/// w &=& 2 \\\\Delta\\\\ d /// \\\\end{eqnarray*} ";

%feature("docstring")
cmf::river::TriangularReach::get_wetted_perimeter "virtual double
get_wetted_perimeter(double depth) const

Returns the wetted perimeter at a given depth \\\\begin{eqnarray*} ///
P &=& 2 d \\\\sqrt{1+\\\\Delta^2} /// \\\\end{eqnarray*} ";

%feature("docstring")  cmf::river::TriangularReach::get_depth "virtual double get_depth(double area) const

Returns the depth at a given crossection area \\\\begin{eqnarray*} ///
d &=& \\\\sqrt{\\\\frac{A}{\\\\Delta}} /// \\\\end{eqnarray*} ";

%feature("docstring")
cmf::river::TriangularReach::get_flux_crossection "virtual double
get_flux_crossection(double depth) const

Returns the crossection area at a given depth \\\\begin{eqnarray*} ///
A &=& d^2 \\\\Delta /// \\\\end{eqnarray*} ";

%feature("docstring")  cmf::river::TriangularReach::TriangularReach "TriangularReach(double l, double bankSlope=2)

Creates a new triangular reach type. ";

%feature("docstring")  cmf::river::TriangularReach::copy "TriangularReach* copy() const ";

%feature("docstring")  cmf::river::TriangularReach::get_nManning "virtual double get_nManning() const ";

%feature("docstring")  cmf::river::TriangularReach::set_nManning "virtual void set_nManning(double val) ";

%feature("docstring")  cmf::river::TriangularReach::h "virtual double
h(double V) const

Returns the depth of a given volume. ";

%feature("docstring")  cmf::river::TriangularReach::A "virtual double
A(double V) const

Returns the area of the surface for a given volume. ";

%feature("docstring")  cmf::river::TriangularReach::V "virtual double
V(double h) const ";

%feature("docstring")  cmf::river::TriangularReach::qManning "double
qManning(double A, double slope) const

Calculates the flow rate from a given water volume in the reach
\\\\begin{eqnarray*} /// q_{Manning}&=& A R^{\\\\frac 2 3}
\\\\sqrt{\\\\frac {\\\\Delta_z} n} \\\\\\\\ /// A &=& \\\\frac V l
\\\\mbox{, (Crosssectional area of the wetted crossection, Volume per
length)} \\\\\\\\ /// R &=& \\\\frac A {P(d)} \\\\\\\\ /// P(d) &=&
\\\\mbox{ the perimeter of the wetted crosssection, a function of
reach depth} \\\\\\\\ /// d(V) &=& \\\\mbox{ the depth of the reach a
function of the volume} \\\\\\\\ /// \\\\Delta_z &=& \\\\frac{z_{max}
- z_{min}}{l} \\\\mbox{ Slope of the reach} /// \\\\end{eqnarray*}
Flow rate [m3/s]

Parameters:
-----------

A:  The area of the cross section [m2]

slope:  The slope of the reach [m/m] ";


// File: classcmf_1_1upslope_1_1_van_genuchten_mualem.xml
%feature("docstring") cmf::upslope::VanGenuchtenMualem "

Provides the use of the Van Genuchten - Mualem retention curve (Van
Genuchten 1980)

Head - moisture realtionship: \\\\begin{eqnarray*} /// W(\\\\theta)
&=& \\\\frac{\\\\theta - \\\\theta_r}{\\\\theta_s - \\\\theta_r}
\\\\\\\\ /// K(W) &=& K_{sat} \\\\sqrt{W}
\\\\left(\\\\left(1-w^{1/m}\\\\right)^m\\\\right)^2 \\\\\\\\ /// m &=&
1-\\\\frac 1 n \\\\\\\\ /// \\\\Psi(W) &=& 0.01 \\\\frac{m}{cm}
\\\\frac{{\\\\left(1-{W}^{\\\\frac{1}{m}}\\\\right)
}^{\\\\frac{1}{n}}}{\\\\alpha\\\\,{W}^{\\\\frac{1}{m\\\\,n}}} \\\\\\\\
/// W(\\\\Psi) &=&
\\\\left(1+\\\\left(\\\\alpha\\\\,100\\\\frac{cm}{m}\\\\Psi\\\\right)^n\\\\right)^{-m}
/// \\\\end{eqnarray*} where:  $K$ is the conductivity in $\\\\frac m
{day}$

$W$ is the wetness (Volume of soil water per volume of pores)

$n$ is a shape parameter of the retention curve

$\\\\alpha$ is inverse of the air entry potential in $cm^{-1}$

$\\\\Psi(W)$ is the matric potential in $m H_2O$ at wetness W

C++ includes: RetentionCurve.h ";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::Wetness "virtual real Wetness(real suction) const

\\\\[ W(\\\\Psi) =
\\\\left(1+\\\\left(\\\\alpha\\\\,100\\\\frac{cm}{m}\\\\Psi\\\\right)^n\\\\right)^{-m}
\\\\] ";

%feature("docstring")
cmf::upslope::VanGenuchtenMualem::MatricPotential "virtual real
MatricPotential(real wetness) const

\\\\[\\\\Psi(W) = 0.01 \\\\frac{m}{cm}
\\\\frac{{\\\\left(1-{W}^{\\\\frac{1}{m}}\\\\right)
}^{\\\\frac{1}{n}}}{\\\\alpha\\\\,{W}^{\\\\frac{1}{m\\\\,n}}} \\\\] ";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::K "virtual
real K(real wetness) const

\\\\[K(W) = K_{sat} \\\\sqrt{W}
\\\\left(\\\\left(1-w^{1/m}\\\\right)^m\\\\right)^2 \\\\] ";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::VoidVolume "virtual real VoidVolume(real upperDepth, real lowerDepth, real Area)
const

\\\\[V_{void}=A\\\\,\\\\left(d_{lower}-d_{upper}\\\\right)\\\\] ";

%feature("docstring")
cmf::upslope::VanGenuchtenMualem::Transmissivity "virtual real
Transmissivity(real upperDepth, real lowerDepth, real wetness) const

\\\\[T=K(W)\\\\,\\\\left(d_{lower}-d_{upper}\\\\right)\\\\] ";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::Porosity "virtual real Porosity(real depth) const

\\\\[\\\\Phi(d)=const\\\\] ";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::FillHeight "virtual real FillHeight(real lowerDepth, real Area, real Volume) const

Returns the thickness of a soil column with a certain pore volume. ";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::copy "VanGenuchtenMualem* copy() const ";

%feature("docstring")
cmf::upslope::VanGenuchtenMualem::VanGenuchtenMualem "VanGenuchtenMualem() ";

%feature("docstring")
cmf::upslope::VanGenuchtenMualem::VanGenuchtenMualem "VanGenuchtenMualem(real _Ksat, real _phi, real _alpha, real _n, real
_m=-1)

Creates a van Genuchten-Mualem retention curve

Parameters:
-----------

_Ksat:  Saturated conductivity in $\\\\frac m{day}$

_phi:  Porosity in $\\\\frac{m^3 Pores}{m^3 Soil}$

_alpha:  Van Genuchten $\\\\alpha$ in $\\\\frac 1{cm}$

_n:  Van Genuchten n

_m:  m parameter, if negative m is calculated as $ 1-\\\\frac 1 n$ ";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::Wetness_eff "virtual real Wetness_eff(real wetness, real pF_r=4.2) const

Returns the effective wetness, using a residual pF value \\\\[w_{eff}
=
\\\\frac{w_{act}-w\\\\left(pF_r\\\\right)}{1-w\\\\left(pF_r\\\\right)}\\\\]
";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::Wetness_pF "real Wetness_pF(real pF) const

returns the volumetric water content at a given pF value ";


// File: structcmf_1_1upslope_1_1vegetation_1_1_vegetation.xml
%feature("docstring") cmf::upslope::vegetation::Vegetation "

Holds the vegetation parameters for the calculation of ET and
fractionating rainfall. Not every ET method uses all parameters.

C++ includes: StructVegetation.h ";

%feature("docstring")
cmf::upslope::vegetation::Vegetation::RootLength "double RootLength()
const

Returns the average root length in m/m2 \\\\[ l_R
\\\\left[\\\\frac{m}{m^2}\\\\right]= \\\\frac{ c_R
\\\\left[\\\\frac{kg}{m^3}\\\\right] z_R \\\\left[m\\\\right] }
{0.5\\\\left[\\\\frac{kg}{m^3}\\\\right] \\\\pi
\\\\left(d_R/2\\\\right)^2 \\\\left[m^2\\\\right]} \\\\] ";

%feature("docstring")
cmf::upslope::vegetation::Vegetation::RootFraction "virtual double
RootFraction(double upperBoundary, double lowerBoundary) const ";

%feature("docstring")
cmf::upslope::vegetation::Vegetation::RootFraction "cmf::math::num_array RootFraction(const cmf::math::num_array
&thickness) const ";

%feature("docstring")
cmf::upslope::vegetation::Vegetation::Vegetation "Vegetation(double
_LAI=2.88, double _Height=0.12, double _RootDepth=0.25, double
_StomatalResistance=100, double _albedo=0.23, double _CanopyClosure=1,
double _CanopyCapacityPerLAI=0.1, double _fraction_at_rootdepth=1.0)
";


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

%feature("docstring")  cmf::river::volume_height_function::copy "volume_height_function* copy() const ";

%feature("docstring")  cmf::river::volume_height_function::h "double
h(double V) const

Returns the depth of a given volume. ";

%feature("docstring")  cmf::river::volume_height_function::A "double
A(double V) const

Returns the area of the surface for a given volume. ";

%feature("docstring")  cmf::river::volume_height_function::V "double
V(double h) const ";


// File: classcmf_1_1water_1_1waterbalance__connection.xml
%feature("docstring") cmf::water::waterbalance_connection "

Routes the sum of all other fluxes to a target

C++ includes: flux_connection.h ";

%feature("docstring")
cmf::water::waterbalance_connection::waterbalance_connection "waterbalance_connection(flux_node::ptr source, flux_node::ptr target)
";

%feature("docstring")  cmf::water::waterbalance_connection::q "real
q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::water::waterbalance_connection::left_node
"flux_node::ptr left_node() const

Returns the left node of this connection. ";

%feature("docstring")  cmf::water::waterbalance_connection::right_node
"flux_node::ptr right_node() const

returns the right node of this connection ";

%feature("docstring")  cmf::water::waterbalance_connection::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")  cmf::water::waterbalance_connection::get_target
"flux_node::ptr get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::water::waterbalance_connection::get_target
"flux_node::ptr get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::water::waterbalance_connection::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::water::waterbalance_connection::conc "real conc(cmf::math::Time t, const cmf::water::solute &_Solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending) ";

%feature("docstring")
cmf::water::waterbalance_connection::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::water::waterbalance_connection::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")  cmf::water::waterbalance_connection::to_string
"std::string to_string() const ";

%feature("docstring")
cmf::water::waterbalance_connection::short_string "std::string
short_string() const ";


// File: classcmf_1_1water_1_1_water_storage.xml
%feature("docstring") cmf::water::WaterStorage "

A state variable for the storage of water

A class for the storage of water. The state is the volume of water
stored in $m^3$ The derivative function is given by:
\\\\begin{eqnarray*} /// \\\\frac{dV}{dt}&=&\\\\sum_{f=1}^{F} q_f
\\\\\\\\ /// F&=& \\\\mbox{Number of fluxes in water storage} \\\\\\\\
/// q_f&=& \\\\mbox{Water flux in } \\\\frac{m^3}{day} \\\\\\\\ ///
\\\\end{eqnarray*}

Todo Check the head based state mode

C++ includes: WaterStorage.h ";

/*  Overrides of flux_node  */

/*

*/

%feature("docstring")  cmf::water::WaterStorage::RecalcFluxes "virtual bool RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node. WaterStorage overrides this, since
state changes require an update of the fluxes ";

%feature("docstring")  cmf::water::WaterStorage::is_empty "virtual
bool is_empty() const ";

%feature("docstring")
cmf::water::WaterStorage::get_state_variable_content "char
get_state_variable_content() const

A character indicating the integrated variable (either 'V' for Volume
or 'h' for head). ";

%feature("docstring")
cmf::water::WaterStorage::set_state_variable_content "void
set_state_variable_content(char content)

A character indicating the integrated variable (either 'V' for Volume
or 'h' for head). ";

%feature("docstring")  cmf::water::WaterStorage::is_storage "virtual
bool is_storage() const

Returns true, since this is a storage. ";

%feature("docstring")  cmf::water::WaterStorage::get_states "cmf::math::state_queue get_states()

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";

%feature("docstring")  cmf::water::WaterStorage::WaterStorage "WaterStorage(const cmf::project &_project, const std::string &Name,
double InitialState=0)

creates a water storage (abstract class)

Parameters:
-----------

_project:  The project the waterstorage belongs to

InitialState:  Initial water content in m3 ";

%feature("docstring")  cmf::water::WaterStorage::Solute "SoluteStorage& Solute(const cmf::water::solute &_Solute)

Returns the water quality of the water storage. ";

%feature("docstring")  cmf::water::WaterStorage::Solute "const
SoluteStorage& Solute(const cmf::water::solute &_Solute) const ";

%feature("docstring")  cmf::water::WaterStorage::conc "real
conc(const cmf::water::solute &_Solute) const

Returns the concentration of the given solute. ";

%feature("docstring")  cmf::water::WaterStorage::conc "real
conc(cmf::math::Time t, const cmf::water::solute &_Solute) const

Returns the current WaterQuality (concentration of all solutes). ";

%feature("docstring")  cmf::water::WaterStorage::conc "void
conc(const cmf::water::solute &_Solute, real NewConcetration)

Sets a new concentration. ";

%feature("docstring")  cmf::water::WaterStorage::get_volume "virtual
real get_volume() const

Returns the volume of water in this storage in m3. ";

%feature("docstring")  cmf::water::WaterStorage::set_volume "virtual
void set_volume(real newwatercontent)

Sets the volume of water in this storage in m3. ";

%feature("docstring")  cmf::water::WaterStorage::Derivate "virtual
real Derivate(const cmf::math::Time &time)

Returns the derivate of the state variable at time time. ";

%feature("docstring")  cmf::water::WaterStorage::get_state "real
get_state() const

Returns the current state of the variable. ";

%feature("docstring")  cmf::water::WaterStorage::set_state "void
set_state(real newState)

Gives access to the state variable. ";

%feature("docstring")  cmf::water::WaterStorage::StateIsChanged "bool
StateIsChanged()

Returns if the state was currently updated. ";

%feature("docstring")  cmf::water::WaterStorage::project "const
cmf::project& project() const

Returns the project, this node is part of. ";

%feature("docstring")  cmf::water::WaterStorage::to_string "virtual
std::string to_string() const ";

%feature("docstring")  cmf::water::WaterStorage::get_connections "cmf::water::connection_vector get_connections() const ";

%feature("docstring")  cmf::water::WaterStorage::get_connection "cmf::water::flux_connection* get_connection(const
cmf::water::flux_node &target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::water::WaterStorage::remove_connection "bool remove_connection(cmf::water::flux_node::ptr To)

Remove the connection. ";

%feature("docstring")  cmf::water::WaterStorage::flux_to "real
flux_to(const cmf::water::flux_node &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from target into this\"). ";

%feature("docstring")  cmf::water::WaterStorage::flux3d_to "cmf::geometry::point flux3d_to(const cmf::water::flux_node &target,
cmf::math::Time t) ";

%feature("docstring")  cmf::water::WaterStorage::get_3d_flux "cmf::geometry::point get_3d_flux(cmf::math::Time t) ";

%feature("docstring")  cmf::water::WaterStorage::water_balance "real
water_balance(cmf::math::Time t, const flux_connection *Without=0)
const

Returns the sum of all fluxes (positive and negative) at time t.
Single fluxes can be excluded from the calculation

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the water_balance
(e.g. to prevent closed circuits) ";

%feature("docstring")  cmf::water::WaterStorage::get_potential "virtual real get_potential() const

Returns the water potential of the node in m waterhead The base class
water storage always returns the height of the location ";

%feature("docstring")  cmf::water::WaterStorage::set_potential "virtual void set_potential(real new_potential) ";


// File: structcmf_1_1atmosphere_1_1_weather.xml
%feature("docstring") cmf::atmosphere::Weather "

A structure holding meteorological information, excluding
precipitation

C++ includes: meteorology.h ";

%feature("docstring")  cmf::atmosphere::Weather::Rn "double Rn(double
albedo, bool daily=false) const

Calculates the net radiation flux $R_n \\\\left[\\\\frac{MJ}{m^2
day}\\\\right]$

\\\\begin{eqnarray*} R_{n} &=& R_{ns} - R_{nl} \\\\\\\\ \\\\mbox{ Net
short wave radiation: }R_{ns} &=& (1-\\\\alpha) R_s \\\\\\\\ \\\\mbox{
Net long wave radiation: }R_{nl} &=& R_{black}\\\\ \\\\beta_{v}\\\\
\\\\beta_{c} \\\\\\\\ \\\\mbox{Black body radiation: } R_{black} &=&
\\\\left\\\\{\\\\begin{array}{cl} \\\\sigma T^4 & \\\\mbox{for less
than daily time steps} \\\\\\\\ \\\\sigma \\\\frac {T_{max}^4 +
T_{min}^4} 2 & \\\\mbox{for daily time steps} \\\\end{array}
\\\\right. \\\\\\\\ T &=& \\\\mbox{Temperature }[K] \\\\\\\\ \\\\sigma
&=& 4.903\\\\ 10^{-9} \\\\frac{MJ}{K^4 m^2 day} \\\\mbox{ Stefan-
Boltzmann constant } \\\\\\\\ \\\\mbox{Long wave reflectance: }
\\\\\\\\ \\\\mbox{by water vapor: }\\\\beta_{v} &=& 0.34 - 0.14
\\\\sqrt{e_a} \\\\\\\\ \\\\mbox{ by clouds: }\\\\beta_{c} &=& 0.1 +
0.9 \\\\frac n N \\\\end{eqnarray*}

Parameters:
-----------

albedo:  the albedo $\\\\alpha$ of the surface

daily:  If true, the net radiation for daily averages will be
calculated ";

%feature("docstring")  cmf::atmosphere::Weather::Weather "Weather()

Calculates the mean pressure for a specific height. ";

%feature("docstring")  cmf::atmosphere::Weather::Weather "Weather(double _T, double _Tmax, double _Tmin, double _rH, double
_wind=2, double _sunshine=0.5, double _Rs=15, double _daylength=12)

Creates a \"weather\" from given data

Parameters:
-----------

_T:  actual Temperature in deg C

_Tmax:  daily maximum Temperature in deg C

_Tmin:  daily minimum Temperature in deg C

_rH:  actual relative humidity in % [0..100]

_wind:  actual wind speed in m/s

_sunshine:  actual fraction of sunshine duration per potential
sunshine duration in h/h

_Rs:  actual incoming shortwave global radiation in MJ/(m2 day) ";

%feature("docstring")  cmf::atmosphere::Weather::to_string "std::string to_string() const ";


// File: structcmf_1_1upslope_1_1_soil_layer_1_1wet.xml


// File: namespacecmf.xml


// File: namespacecmf_1_1atmosphere.xml
%feature("docstring")  cmf::atmosphere::vapour_pressure "double
cmf::atmosphere::vapour_pressure(double T) ";

%feature("docstring")  cmf::atmosphere::vpd_from_rH "double
cmf::atmosphere::vpd_from_rH(double T, double rH) ";

%feature("docstring")  cmf::atmosphere::rH_from_vpd "double
cmf::atmosphere::rH_from_vpd(double T, double vpd) ";

%feature("docstring")  cmf::atmosphere::global_radiation "double
cmf::atmosphere::global_radiation(cmf::math::Time t, double height,
double sunshine_fraction, double longitude=8, double latitude=51,
double time_zone=1, bool daily=0) ";

%feature("docstring")  cmf::atmosphere::Pressure "double
cmf::atmosphere::Pressure(double height) ";


// File: namespacecmf_1_1geometry.xml


// File: namespacecmf_1_1math.xml
%feature("docstring")  cmf::math::minimum_t "Time
cmf::math::minimum_t(Time t1, Time t2) ";

%feature("docstring")  cmf::math::maximum_t "Time
cmf::math::maximum_t(Time t1, Time t2) ";

%feature("docstring")  cmf::math::nash_sutcliff "double
cmf::math::nash_sutcliff(const cmf::math::timeseries &model, const
cmf::math::timeseries &observation) ";

%feature("docstring")  cmf::math::R2 "double cmf::math::R2(const
cmf::math::timeseries &model, const cmf::math::timeseries
&observation) ";


// File: namespacecmf_1_1river.xml
%feature("docstring")  cmf::river::make_river_gap "double
cmf::river::make_river_gap(Reach::ptr root_reach)

Ensures that rivers have a monotone downward flow direction. Reaches
with a bottom higher than any upstream reach are lowered to the
minimum height of any (possibly distant) upstream reach. ";


// File: namespacecmf_1_1upslope.xml
%feature("docstring")  cmf::upslope::connections::find_cell "cmf::upslope::Cell* cmf::upslope::find_cell(cells_ref,
cmf::geometry::point p, double max_dist=1e20)

Finds the nearest cell for a position. ";

%feature("docstring")  cmf::upslope::connections::get_boundary_cells "cell_vector cmf::upslope::get_boundary_cells(cells_ref cells)

Gets the outer cells of a network. Unstable! Might create an infinite
loop. ";

%feature("docstring")  cmf::upslope::connections::get_connections "cmf::water::connection_set cmf::upslope::get_connections(cells_ref
cells)

Gets all connections of a bunch of cells. ";

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

%feature("docstring")  cmf::upslope::connections::cell_positions "cmf::geometry::point_vector
cmf::upslope::cell_positions(cmf::upslope::cells_ref cells) ";

%feature("docstring")  cmf::upslope::connections::cell_flux_directions
"cmf::geometry::point_vector
cmf::upslope::cell_flux_directions(cmf::upslope::cells_ref cells,
cmf::math::Time) ";

%feature("docstring")
cmf::upslope::connections::pressure_to_waterhead "double
cmf::upslope::pressure_to_waterhead(double Pressure)

Converts a pressure in Pa to a lenght of a water column in m. ";

%feature("docstring")
cmf::upslope::connections::waterhead_to_pressure "double
cmf::upslope::waterhead_to_pressure(double waterhead)

Converts a height of a water column in m to a pressure in Pa. ";

%feature("docstring")  cmf::upslope::connections::pF_to_waterhead "double cmf::upslope::pF_to_waterhead(double pF)

Converts a pF value to a height of a water column in m. ";

%feature("docstring")  cmf::upslope::connections::waterhead_to_pF "double cmf::upslope::waterhead_to_pF(double waterhead)

Converts a height of a water column to a pF value. ";


// File: namespacecmf_1_1upslope_1_1connections.xml
%feature("docstring")  cmf::upslope::connections::HBVinstall "void
cmf::upslope::connections::HBVinstall(HBVparameters parameters,
cmf::upslope::Cell &cell)

Installs an HBV like model setup at a cell Reference: Bergstroem et al
1995, in Singh (1995): Computer models of watershed hydrology

Removes the layers of a cell and adds three layers with 1m thickness
and the HBVparameters as retention curve. Installs HBVpercolation
between the layers

Parameters:
-----------

parameters:  The parameters for the HBV like setup

cell:   Cell where HBV like percolation is to be calculated ";


// File: namespacecmf_1_1upslope_1_1_e_t.xml
%feature("docstring")  cmf::upslope::ET::PenmanMonteith "real
cmf::upslope::ET::PenmanMonteith(real Rn, real ra, real rs, real T,
real vap_press_deficit)

Returns the potential ET after Penman-Monteith using some
simplifications for a given Radiation balance, aerodynamic and surface
resistances, and a vapor pressure deficit

\\\\[ ET = \\\\frac{\\\\Delta R_n}{\\\\lambda \\\\Delta + \\\\gamma +
\\\\gamma \\\\frac{r_s}{r_a}} /// + \\\\frac{c_p\\\\rho_a}{\\\\Delta +
\\\\gamma + \\\\gamma \\\\frac{r_s}{r_a}} \\\\frac{e_s - e_a}{r_a}
\\\\] where  $ ET $ is the evapotranspiration in $\\\\frac{kg}{m^2
day}\\\\approx \\\\frac{mm}{day}$

$ \\\\Delta \\\\left[\\\\frac{kPa}{k}\\\\right]= 4098\\\\ 0.618
\\\\exp\\\\left(\\\\frac{17.27 T}{T+237.3}\\\\right (T+237.3)^{-2} $
is the slope of vapor pressure

$ R_n \\\\left[\\\\frac{MJ}{m^2 day}\\\\right]$ is the radiation
balance

$ r_s \\\\left[\\\\frac s m\\\\right] $ is the surface resistance

$ r_a \\\\left[\\\\frac s m\\\\right] $ is the aerodynamic resistance

$ \\\\gamma = 0.067 \\\\left[\\\\frac{kPa}{k}\\\\right] $ is the
psychrometer constant

$ e_s - e_a \\\\left[kPa\\\\right]$ is the vapor pressure deficit

Parameters:
-----------

Rn:  Radiation balance in $ \\\\frac{MJ}{m^2 day} $

ra:  Aerodynamic resistance in $ \\\\frac s m $

rs:  Surface resistance in $ \\\\frac s m $, is 0 for free water

T:  Actual Temperature in $ ^\\\\circ C $

vap_press_deficit:  Deficit of vapor pressure $ kPa $ ";

%feature("docstring")  cmf::upslope::ET::PenmanMonteith "real
cmf::upslope::ET::PenmanMonteith(cmf::atmosphere::Weather A, const
cmf::upslope::vegetation::Vegetation &veg, double h) ";

%feature("docstring")  cmf::upslope::ET::Tact "real
cmf::upslope::ET::Tact(real Tpot, const cmf::upslope::SoilLayer &sw,
const cmf::upslope::vegetation::Vegetation &veg) ";


// File: namespacecmf_1_1upslope_1_1vegetation.xml


// File: namespacecmf_1_1water.xml
%feature("docstring")  cmf::water::replace_node "int
cmf::water::replace_node(cmf::water::flux_node::ptr oldnode,
cmf::water::flux_node::ptr newnode) ";

%feature("docstring")  cmf::water::set_flux "static void
cmf::water::set_flux(flux_node::ptr source, flux_node::ptr target,
real flux_value)

Sets a constant flux between two nodes, if an
external_control_connection exists. ";

%feature("docstring")  cmf::water::can_set_flux "static bool
cmf::water::can_set_flux(flux_node::ptr source, flux_node::ptr target)

Checks if a constant flux between two nodes can be set. Returns true
if the nodes are connected by an external_control_connection ";

%feature("docstring")  cmf::water::count_node_references "int
cmf::water::count_node_references(flux_node::ptr node) ";

%feature("docstring")  cmf::water::get_higher_node "flux_node::ptr
cmf::water::get_higher_node(flux_node::ptr node1, flux_node::ptr
node2) ";

%feature("docstring")  cmf::water::get_lower_node "flux_node::ptr
cmf::water::get_lower_node(flux_node::ptr node1, flux_node::ptr node2)
";


// File: meteorology_8h.xml


// File: precipitation_8h.xml


// File: implicitconv_8h.xml
%feature("docstring")  feature "feature(\"compactdefaultargs\") ";

%feature("docstring")  feature "feature(\"autodoc\",\"1\") ";


// File: _f_v_m_8dox.xml


// File: _i_d_w_8dox.xml


// File: _landscape_model_8dox.xml


// File: license_8dox.xml


// File: mainpage_8dox.xml


// File: geometry_8h.xml


// File: bdf2_8h.xml


// File: cvodeintegrator_8h.xml


// File: explicit__euler_8h.xml


// File: implicit__euler_8h.xml


// File: integrator_8h.xml


// File: multiintegrator_8h.xml


// File: _r_k_fintegrator_8h.xml


// File: _water_solute_integrator_8h.xml


// File: num__array_8h.xml


// File: real_8h.xml
%feature("docstring")  minimum "real minimum(real a, real b)

Returns the minimum of two values. ";

%feature("docstring")  maximum "real maximum(real a, real b) ";

%feature("docstring")  minmax "real minmax(real x, real min, real
max) ";

%feature("docstring")  mean "real mean(real a, real b) ";

%feature("docstring")  geo_mean "real geo_mean(real a, real b) ";

%feature("docstring")  harmonic_mean "real harmonic_mean(real a, real
b) ";

%feature("docstring")  piecewise_linear "real piecewise_linear(real
x, real xmin, real xmax, real ymin=0, real ymax=1) ";

%feature("docstring")  boltzmann "real boltzmann(real x, real x_half,
real tau) ";

%feature("docstring")  sign "real sign(real x) ";

%feature("docstring")  square "real square(real x) ";


// File: statevariable_8h.xml


// File: time_8h.xml


// File: timeseries_8h.xml


// File: project_8h.xml


// File: _manning_connection_8h.xml


// File: _open_water_storage_8h.xml


// File: _reach_8h.xml


// File: _reach_type_8h.xml


// File: algorithm_8h.xml


// File: cell_8h.xml


// File: cell__vector_8h.xml


// File: _atmospheric_fluxes_8h.xml


// File: _h_b_vflow_8h.xml


// File: infiltration_8h.xml


// File: _percolation_8h.xml


// File: subsurfacefluxes_8h.xml


// File: surfacefluxes_8h.xml


// File: layer__list_8h.xml


// File: _retention_curve_8h.xml


// File: _soil_layer_8h.xml


// File: _topology_8h.xml


// File: _e_t_8h.xml


// File: _shuttleworth_wallace_8h.xml


// File: _struct_vegetation_8h.xml


// File: boundary__condition_8h.xml


// File: collections_8h.xml


// File: flux__connection_8h.xml


// File: flux__node_8h.xml


// File: _solute_8h.xml


// File: _solute_storage_8h.xml


// File: _water_storage_8h.xml


// File: group__boundary.xml


// File: group__surfacefluxes.xml


// File: group___e_t.xml


// File: group__connections.xml


// File: group___h_b_v.xml


// File: group__infiltration.xml


// File: group__meteo.xml


// File: group__perc.xml


// File: group__storages.xml


// File: group__manning.xml


// File: group__nodes.xml


// File: group__latflux.xml


// File: _f_v_m.xml


// File: _i_d_w.xml


// File: _l_m.xml


// File: license.xml


// File: todo.xml


// File: dir_9592ab4aaa0d9c46a819f7f81e09220a.xml


// File: dir_a16da2e77d7736c8ef82aa4fb58f4d7a.xml


// File: dir_98acbbad275b391c1ade88ff097e61c2.xml


// File: dir_8410d30186ae5b9b3ffde0ee7f4d44e7.xml


// File: dir_a908fbf2eb3f8c8152fb61ab6347c49b.xml


// File: dir_ea86885cf3f09bc8723d5eddb49a3fb1.xml


// File: dir_ace73d892420b0c6d251d9a3a47aa20d.xml


// File: dir_245345c6834d8cbe65c8078f132e59b7.xml


// File: dir_cc7bcbd64709af57d5efb77b5a4368ed.xml


// File: dir_95270cf67a60bfc82738cbf6d50d39ed.xml


// File: dir_6133eb82dc0ddcdba471fd038299ba04.xml


// File: dir_006f6fc26db23124fae960566a4f6405.xml


// File: dir_031051fb4244b6bc74c5e3edb1f57762.xml


// File: main.xml

