
// File: index.xml

// File: a00470.xml
%feature("docstring") std::allocator "

STL class. ";


// File: a00471.xml
%feature("docstring") std::auto_ptr "

STL class. ";


// File: a00472.xml
%feature("docstring") std::bad_alloc "

STL class. ";


// File: a00473.xml
%feature("docstring") std::bad_cast "

STL class. ";


// File: a00474.xml
%feature("docstring") std::bad_exception "

STL class. ";


// File: a00475.xml
%feature("docstring") std::bad_typeid "

STL class. ";


// File: a00476.xml
%feature("docstring") std::basic_fstream "

STL class. ";


// File: a00479.xml
%feature("docstring") std::basic_ifstream "

STL class. ";


// File: a00482.xml
%feature("docstring") std::basic_ios "

STL class. ";


// File: a00487.xml
%feature("docstring") std::basic_iostream "

STL class. ";


// File: a00483.xml
%feature("docstring") std::basic_istream "

STL class. ";


// File: a00489.xml
%feature("docstring") std::basic_istringstream "

STL class. ";


// File: a00494.xml
%feature("docstring") std::basic_ofstream "

STL class. ";


// File: a00484.xml
%feature("docstring") std::basic_ostream "

STL class. ";


// File: a00497.xml
%feature("docstring") std::basic_ostringstream "

STL class. ";


// File: a00502.xml
%feature("docstring") std::basic_string "

STL class. ";


// File: a00488.xml
%feature("docstring") std::basic_stringstream "

STL class. ";


// File: a00001.xml
%feature("docstring") cmf::math::BDF2 "

An order 2 BDF-Method with fixed-point iteration and variable step
size.

Recommended integrator for CMF (so far)

Derived from Roussel C. and Roussel M. (2003) \"Generic Object-
Oriented Differential Equation Integrators\", C/C++ User Journal, Nov.
2003,http://www.ddj.com/cpp/184401724?pgno=8 and

Eckert S., Baaser H., Gross D. and Scherf O. (2004) \"A BDF2
integration method with step size control for elasto-plasticity\",
Computational Mechanics 34, 377 - 386, DOI: 10.1007/s00466-004-0581-1

Most important function: Integrate

C++ includes: BDF2.h ";

/*  Gear functions: The following functions evaluate the Gear formulas
of specified order, loading the results into compareStates.  */

/*  Internal data storages  */

/*  Assessment of state variables for integration  */

%feature("docstring")  cmf::math::BDF2::AddState "virtual void
AddState(cmf::math::StateVariable &statevar) ";

/*  model time  */

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

/*  Integrate  */

%feature("docstring")  cmf::math::BDF2::Integrate "int
Integrate(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::BDF2::IntegrateUntil "void
IntegrateUntil(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::BDF2::IntegrateUntil "void
IntegrateUntil(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables until MaxTime. ";

/*  The state variables to integrate  */

/*  Accuracy parameters  */

%feature("docstring")  cmf::math::BDF2::AddStatesFromOwner "void
AddStatesFromOwner(cmf::math::StateVariableOwner &stateOwner)

Adds the state variables of a StateVariableOwner to the state
variables of the solver. ";

%feature("docstring")  cmf::math::BDF2::get_error_position "int
get_error_position() const ";

%feature("docstring")  cmf::math::BDF2::BDF2 "BDF2(const
StateVariableVector &states, real epsilon=1e-9, cmf::math::Time
tStepMin=Time::Milliseconds(10))

Constructs a new Gears_var_Step from a pointer to a vector of state
variables.

The Integrator becomes the owner of states

Parameters:
-----------

states:  Statevariables of the system

epsilon:  relative error tolerance per time step (default=1e-9)

tStepMin:  minimum time step (default=10s) ";

%feature("docstring")  cmf::math::BDF2::BDF2 "BDF2(real epsilon=1e-9,
cmf::math::Time tStepMin=Time::Milliseconds(10))

Constructs a new Gears_var_Step.

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

Integrates the vector of state variables.

if (see Formula in C++ documentation)  then (see Formula in C++
documentation)  else (see Formula in C++ documentation)

Uses pastStatesArray to store (see Formula in C++ documentation)

if more than one step is taken start iterating:  (see Formula in C++
documentation)  with (see Formula in C++ documentation)

until (see Formula in C++ documentation)

If too many iterations are needed, or the error is rising, repeat
iteration with (see Formula in C++ documentation)

Parameters:
-----------

MaxTime:  To stop the model (if running in a model framework) at time
steps of value exchange e.g. full hours, the next value exchange time
can be given

TimeStep:  Takes the proposed timestep, ignored by this solver ";

%feature("docstring")  cmf::math::BDF2::count "int count() const

returns the number of state variables ";

%feature("docstring")  cmf::math::BDF2::state "real state(int
position) const

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::BDF2::state "void state(int
position, real newState)

Simplifies the assessment of state variables. ";


// File: a00511.xml
%feature("docstring") std::bitset "

STL class. ";


// File: a00002.xml
%feature("docstring") cmf::upslope::BrooksCoreyRetentionCurve "

Provides the use of the Brooks-Corey retention curve.

(see Formula in C++ documentation)  where:  (see Formula in C++
documentation)  is the conductivity in (see Formula in C++
documentation)

(see Formula in C++ documentation)  is the wetness (Volume of soil
water per volume of pores)

(see Formula in C++ documentation)  is the shape of the retention
curve (usually between 4 (sand) and 14 (clay))

(see Formula in C++ documentation)  is the matric potential in (see
Formula in C++ documentation)  at wetness W

(see Formula in C++ documentation)  is a matric potential at a known
wetness in (see Formula in C++ documentation)

(see Formula in C++ documentation)  is the wetness with a known matric
potential for dynamic changes with depth, exponential decays of
porosity and saturated conductivity are used The decay function is:
(see Formula in C++ documentation) , where v is the value ( (see
Formula in C++ documentation) ), d is the depth in m and a is the
fractional decay per m. E.g. 0.1 means the value has in 1 m depth 90%
of the value at the surface

C++ includes: RetentionCurve.h ";

%feature("docstring")  cmf::upslope::BrooksCoreyRetentionCurve::K "virtual real K(real wetness, real depth) const

Returns the conductivity in m/day at a certain depth and water
content. ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::SetKsat "void SetKsat(real
ksat, real ksat_decay) ";

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

Returns the pore volume in a region of a soil column.

If there is a porosity decay, the void volume is the integral of
porosity over depth times area ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::FillHeight "real
FillHeight(real lowerDepth, real Area, real Volume) const

Returns the thickness of a soil column with a certain pore volume. ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::Transmissivity "real
Transmissivity(real upperDepth, real lowerDepth, real theta) const

Returns the transmissivity of a part of a soil column. ";

%feature("docstring")  cmf::upslope::BrooksCoreyRetentionCurve::b "real b() const

Retention curve shape parameter. ";

%feature("docstring")  cmf::upslope::BrooksCoreyRetentionCurve::Set_b
"void Set_b(real new_b) ";

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
real theta_x=0.2, real psi_x=pF_to_waterhead(2.5), real ksat_decay=0,
real porosity_decay=0)

Creates a brooks corey retention curve.

Parameters:
-----------

ksat:  Saturated conductivity (see Formula in C++ documentation)

porosity:   (see Formula in C++ documentation)

_b:  Shape of the retention curve (if you do not know how to
parameterize this, take a look at the other constructor)

theta_x:   (see Formula in C++ documentation)  Water content at a
specific suction pressure

psi_x:  Suction pressure for (see Formula in C++ documentation)  in m
water column, use the conversion functions pF_to_waterhead,
pressure_to_waterhead to convert pressure in to waterhead height
(default pF=2.5)

ksat_decay:  Relative decay of conductivity with depth, e.g. 0.1 means
conductivity gets 10% smaller per meter

porosity_decay:  Relative decay of porosity with depth, e.g. 0.1 means
conductivity gets 10% smaller per meter ";

%feature("docstring")  cmf::upslope::BrooksCoreyRetentionCurve::copy "virtual BrooksCoreyRetentionCurve* copy() const ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::Wetness_eff "virtual real
Wetness_eff(real wetness, real pF_r=4.2) const

Returns the effective wetness, using a residual pF value (see Formula
in C++ documentation) . ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::Wetness_pF "real
Wetness_pF(real pF) const

returns the volumetric water content at a given pF value ";


// File: a00003.xml
%feature("docstring") cmf::upslope::connections::CanopyOverflow "

Calculates the overflow of a canopy storage.

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
"real conc(cmf::math::Time t, const cmf::water::solute &solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";

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


// File: a00004.xml
%feature("docstring") cmf::upslope::ET::CanopyStorageEvaporation "

Calculates the evaporation from a canopy storage.

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
conc(cmf::math::Time t, const cmf::water::solute &solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";

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


// File: a00005.xml
%feature("docstring") cmf::upslope::Cell "

This class is the basic landscape object. It is the owner of water
storages, and the upper and lower boundary conditions of the system
(rainfall, atmospheric vapor, deep groundwater).

C++ includes: cell.h ";

/*  Flux nodes of the cell  */

%feature("docstring")  cmf::upslope::Cell::get_meteorology "cmf::atmosphere::Meteorology& get_meteorology() const ";

%feature("docstring")  cmf::upslope::Cell::set_meteorology "void
set_meteorology(const cmf::atmosphere::Meteorology &new_meteo) ";

%feature("docstring")  cmf::upslope::Cell::get_rainfall "std::tr1::shared_ptr<cmf::atmosphere::RainCloud> get_rainfall() const
";

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

%feature("docstring")  cmf::upslope::Cell::has_wet_leaves "bool
has_wet_leaves() const ";

%feature("docstring")  cmf::upslope::Cell::has_surface_water "bool
has_surface_water() const ";

%feature("docstring")  cmf::upslope::Cell::get_vegetation "cmf::upslope::vegetation::Vegetation get_vegetation() const ";

%feature("docstring")  cmf::upslope::Cell::set_vegetation "void
set_vegetation(cmf::upslope::vegetation::Vegetation val) ";

%feature("docstring")  cmf::upslope::Cell::project "const
cmf::project& project() const ";

%feature("docstring")  cmf::upslope::Cell::get_weather "cmf::atmosphere::Weather get_weather(cmf::math::Time t) const ";

/*  Layers  */

%feature("docstring")  cmf::upslope::Cell::layer_count "int
layer_count() const ";

%feature("docstring")  cmf::upslope::Cell::get_layer "cmf::upslope::layer_ptr get_layer(int ndx) const ";

%feature("docstring")  cmf::upslope::Cell::add_layer "void
add_layer(real lowerboundary, const cmf::upslope::RetentionCurve
&r_curve, real saturateddepth=10) ";

%feature("docstring")  cmf::upslope::Cell::remove_last_layer "void
remove_last_layer() ";

%feature("docstring")  cmf::upslope::Cell::remove_layers "void
remove_layers() ";

%feature("docstring")  cmf::upslope::Cell::~Cell "virtual ~Cell() ";

/*  Location  */

%feature("docstring")  cmf::upslope::Cell::get_topology "cmf::upslope::Topology& get_topology() ";

%feature("docstring")  cmf::upslope::Cell::get_position "cmf::geometry::point get_position() const

Returns the location of the cell. ";

%feature("docstring")  cmf::upslope::Cell::get_area "double
get_area() const

Returns the area of the cell. ";

/*  Saturation  */

%feature("docstring")  cmf::upslope::Cell::InvalidateSatDepth "void
InvalidateSatDepth()

Marks the saturated depth as unvalid. ";

%feature("docstring")  cmf::upslope::Cell::get_saturated_depth "real
get_saturated_depth() ";

%feature("docstring")  cmf::upslope::Cell::set_saturated_depth "void
set_saturated_depth(real depth) ";

%feature("docstring")  cmf::upslope::Cell::Cell "Cell(double x,
double y, double z, double area, cmf::project &_project) ";

%feature("docstring")  cmf::upslope::Cell::to_string "std::string
to_string() ";

%feature("docstring")  cmf::upslope::Cell::AddStateVariables "void
AddStateVariables(cmf::math::StateVariableVector &vector)

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


// File: a00512.xml


// File: a00006.xml
%feature("docstring") cmf::upslope::CellConnector "

A helper class to connect cells with flux_connection objects. This is
generated by flux_connection classes, intended to connect cells.

C++ includes: cell.h ";

%feature("docstring")  cmf::upslope::CellConnector::CellConnector "CellConnector(connectorfunction connector) ";

%feature("docstring")  cmf::upslope::CellConnector::connect "void
connect(cmf::upslope::Cell &cell1, cmf::upslope::Cell &cell2, int
start_at_layer=0) const ";


// File: a00007.xml
%feature("docstring") cmf::river::Channel "

A wrapper for channel geometries.

C++ includes: ReachType.h ";

%feature("docstring")  cmf::river::Channel::Channel "Channel(double
length=1.0)

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
model, width (bank width) and depth are used.

the reach type

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

Calculates the flow rate from a given water volume in the reach (see
Formula in C++ documentation) .

Flow rate [m3/s]

Parameters:
-----------

A:  The area of the cross section [m2]

slope:  The slope of the reach [m/m] ";


// File: a00513.xml
%feature("docstring") cmf::upslope::connections::CompleteInfiltration
"";

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
conc(cmf::math::Time t, const cmf::water::solute &solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";

%feature("docstring")
cmf::upslope::connections::CompleteInfiltration::get_tracer_filter "real get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::connections::CompleteInfiltration::set_tracer_filter "void set_tracer_filter(real value) ";

%feature("docstring")
cmf::upslope::connections::CompleteInfiltration::to_string "std::string to_string() const ";

%feature("docstring")
cmf::upslope::connections::CompleteInfiltration::short_string "std::string short_string() const ";


// File: a00514.xml
%feature("docstring") std::complex "

STL class. ";


// File: a00505.xml
%feature("docstring") std::basic_string::const_iterator "

STL iterator class. ";


// File: a00515.xml
%feature("docstring") std::string::const_iterator "

STL iterator class. ";


// File: a00516.xml
%feature("docstring") std::wstring::const_iterator "

STL iterator class. ";


// File: a00517.xml
%feature("docstring") std::deque::const_iterator "

STL iterator class. ";


// File: a00518.xml
%feature("docstring") std::list::const_iterator "

STL iterator class. ";


// File: a00519.xml
%feature("docstring") std::map::const_iterator "

STL iterator class. ";


// File: a00520.xml
%feature("docstring") std::multimap::const_iterator "

STL iterator class. ";


// File: a00521.xml
%feature("docstring") std::set::const_iterator "

STL iterator class. ";


// File: a00522.xml
%feature("docstring") std::multiset::const_iterator "

STL iterator class. ";


// File: a00523.xml
%feature("docstring") std::vector::const_iterator "

STL iterator class. ";


// File: a00506.xml
%feature("docstring") std::basic_string::const_reverse_iterator "

STL iterator class. ";


// File: a00524.xml
%feature("docstring") std::string::const_reverse_iterator "

STL iterator class. ";


// File: a00525.xml
%feature("docstring") std::wstring::const_reverse_iterator "

STL iterator class. ";


// File: a00526.xml
%feature("docstring") std::deque::const_reverse_iterator "

STL iterator class. ";


// File: a00527.xml
%feature("docstring") std::list::const_reverse_iterator "

STL iterator class. ";


// File: a00528.xml
%feature("docstring") std::map::const_reverse_iterator "

STL iterator class. ";


// File: a00529.xml
%feature("docstring") std::multimap::const_reverse_iterator "

STL iterator class. ";


// File: a00530.xml
%feature("docstring") std::set::const_reverse_iterator "

STL iterator class. ";


// File: a00531.xml
%feature("docstring") std::multiset::const_reverse_iterator "

STL iterator class. ";


// File: a00532.xml
%feature("docstring") std::vector::const_reverse_iterator "

STL iterator class. ";


// File: a00533.xml
%feature("docstring") cmf::upslope::ET::constantETpot "";

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
conc(cmf::math::Time t, const cmf::water::solute &solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";

%feature("docstring")
cmf::upslope::ET::constantETpot::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::ET::constantETpot::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")  cmf::upslope::ET::constantETpot::to_string "std::string to_string() const ";

%feature("docstring")  cmf::upslope::ET::constantETpot::short_string "std::string short_string() const ";


// File: a00008.xml
%feature("docstring") cmf::atmosphere::ConstantMeteorology "

A primitive implementation of the Meteorology interface. Holds a
Weather record and returns it for any date.

C++ includes: Meteorology.h ";

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


// File: a00009.xml
%feature("docstring") cmf::math::CVodeIntegrator "

A wrapper class for the CVODE integrator from the SUNDIALS library.

https://computation.llnl.gov/casc/sundials/main.html

C++ includes: cvodeIntegrator.h ";

/*  Assessment of state variables for integration  */

%feature("docstring")  cmf::math::CVodeIntegrator::AddStatesFromOwner
"virtual void AddStatesFromOwner(cmf::math::StateVariableOwner
&stateOwner)

Adds the state variables of a StateVariableOwner to the state
variables of the solver. ";

%feature("docstring")  cmf::math::CVodeIntegrator::AddState "virtual
void AddState(cmf::math::StateVariable &statevar) ";

/*  model time  */

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

/*  Integrate  */

%feature("docstring")  cmf::math::CVodeIntegrator::Integrate "int
Integrate(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::CVodeIntegrator::IntegrateUntil "void IntegrateUntil(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::CVodeIntegrator::IntegrateUntil "void IntegrateUntil(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables until MaxTime. ";

/*  The state variables to integrate  */

/*  Accuracy parameters  */

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

Integrates the vector of state variables.

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

%feature("docstring")  cmf::math::CVodeIntegrator::CVodeIntegrator "CVodeIntegrator(real epsilon=1e-9, cmf::math::Time
tStepMin=Time::Milliseconds(50))

Create a new CVODE integrator.

Parameters:
-----------

epsilon:  relative and absolute error tolerance

tStepMin:  Minimal timestep ";

%feature("docstring")  cmf::math::CVodeIntegrator::CVodeIntegrator "CVodeIntegrator(cmf::math::StateVariableOwner &states, real
epsilon=1e-9, cmf::math::Time tStepMin=Time::Milliseconds(50)) ";

%feature("docstring")  cmf::math::CVodeIntegrator::CVodeIntegrator "CVodeIntegrator(const CVodeIntegrator &templ)

Copy constructor, creates a new CVODE integrator similiar to the
given, but without statevariables. ";

%feature("docstring")  cmf::math::CVodeIntegrator::Copy "CVodeIntegrator* Copy() const

Returns a new Integrator, based on this (without the state variables),
e.g. same type, epsilon, model time etc. ";

%feature("docstring")  cmf::math::CVodeIntegrator::~CVodeIntegrator "virtual ~CVodeIntegrator() ";

%feature("docstring")  cmf::math::CVodeIntegrator::count "int count()
const

returns the number of state variables ";

%feature("docstring")  cmf::math::CVodeIntegrator::state "real
state(int position) const

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::CVodeIntegrator::state "void
state(int position, real newState)

Simplifies the assessment of state variables. ";


// File: a00010.xml
%feature("docstring") cmf::upslope::connections::Darcy "

Calculates the lateral flow using the gravitational potential gradient
only.

(see Formula in C++ documentation)  where  (see Formula in C++
documentation)  is the gravitational potential difference

(see Formula in C++ documentation)  is the distance from Cell 1 to
Cell 2

(see Formula in C++ documentation)  is the transmissivity of cell C,
calculated by SoilType::Transmissivity

(see Formula in C++ documentation)  is the width of the connection of
the cells

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
conc(cmf::math::Time t, const cmf::water::solute &solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";

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


// File: a00011.xml
%feature("docstring") cmf::math::Date "

An absolute time, not for calculation. Date and Time are
interchangable.

C++ includes: Time.h ";

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


// File: a00534.xml
%feature("docstring") std::deque "

STL class. ";


// File: a00537.xml
%feature("docstring") std::domain_error "

STL class. ";


// File: a00012.xml
%feature("docstring") cmf::water::DricheletBoundary "

Drichelet (constant head) boundary condition.

This boundary condition can be used either as a pure sink boundary
condition or as a conditional source / sink boundary condition. The
constant head of the boundary condition is interpreted and handled by
the connections of the boundary condition. Not head aware connections,
should not be used, since they are ignoring the constant head.

C++ includes: boundary_condition.h ";

%feature("docstring")  cmf::water::DricheletBoundary::get_potential "real get_potential() const

Returns the water potential of the node in m waterhead The base class
water storage always returns the height of the location. ";

%feature("docstring")  cmf::water::DricheletBoundary::set_potential "void set_potential(real new_potential) ";

%feature("docstring")  cmf::water::DricheletBoundary::set_conc "virtual void set_conc(const cmf::water::solute &solute, double value)
";

%feature("docstring")  cmf::water::DricheletBoundary::conc "virtual
real conc(cmf::math::Time t, const cmf::water::solute &solute) const

Returns the water quality of the flux_node, if it is not overridden
this is the mix of the incoming fluxes. ";

%feature("docstring")  cmf::water::DricheletBoundary::is_empty "bool
is_empty() const ";

%feature("docstring")  cmf::water::DricheletBoundary::RecalcFluxes "bool RecalcFluxes(cmf::math::Time t) const ";

%feature("docstring")
cmf::water::DricheletBoundary::DricheletBoundary "DricheletBoundary(const cmf::project &_p, real potential,
cmf::geometry::point Location=cmf::geometry::point()) ";

%feature("docstring")  cmf::water::DricheletBoundary::get_position "cmf::geometry::point get_position() const

Pure virtual function. Should return the position of the locatable. ";

%feature("docstring")  cmf::water::DricheletBoundary::set_position "virtual void set_position(cmf::geometry::point p)

Sets the position of the node in space. ";

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
state changes require an update of the fluxes. ";

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
Single fluxes can be excluded from the calculation.

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the water_balance
(e.g. to prevent closed circuits) ";

%feature("docstring")  cmf::water::DricheletBoundary::get_distance_to
"double get_distance_to(const Locatable &cmp)

Returns the distance between two locatable objects. ";

%feature("docstring")  cmf::water::DricheletBoundary::get_direction_to
"cmf::geometry::point get_direction_to(const Locatable &cmp)

Returns a vector with length=1 pointing in the direction of another
Locatable. ";


// File: a00538.xml
%feature("docstring") std::tr1::enable_shared_from_this "";


// File: a00539.xml
%feature("docstring") std::exception "

STL class. ";


// File: a00013.xml
%feature("docstring") cmf::math::ExplicitEuler_fixed "

An explicit Euler integrator, with a fixed time step.

C++ includes: ExplicitEuler_fixed.h ";

/*  Assessment of state variables for integration  */

%feature("docstring")  cmf::math::ExplicitEuler_fixed::AddState "virtual void AddState(cmf::math::StateVariable &statevar) ";

/*  model time  */

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

/*  Integrate  */

%feature("docstring")  cmf::math::ExplicitEuler_fixed::Integrate "int
Integrate(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::IntegrateUntil
"void IntegrateUntil(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::IntegrateUntil
"void IntegrateUntil(cmf::math::Time MaxTime, cmf::math::Time
TimeStep)

Integrates the vector of state variables until MaxTime. ";

/*  The state variables to integrate  */

/*  Accuracy parameters  */

%feature("docstring")
cmf::math::ExplicitEuler_fixed::AddStatesFromOwner "void
AddStatesFromOwner(cmf::math::StateVariableOwner &stateOwner)

Adds the state variables of a StateVariableOwner to the state
variables of the solver. ";

%feature("docstring")
cmf::math::ExplicitEuler_fixed::ExplicitEuler_fixed "ExplicitEuler_fixed(const StateVariableVector &states, real
epsilon=1e-9, cmf::math::Time tStepMin=Time::Seconds(10))

Constructs a new ExplicitEuler_fixed from a pointer to a vector of
state variables.

The Integrator becomes the owner of states

Parameters:
-----------

states:  Statevariables of the system

epsilon:  (ignored) relative error tolerance per time step
(default=1e-9)

tStepMin:  (ignored) minimum time step (default=10s) ";

%feature("docstring")
cmf::math::ExplicitEuler_fixed::ExplicitEuler_fixed "ExplicitEuler_fixed(StateVariableOwner &states, real epsilon=1e-9,
cmf::math::Time tStepMin=Time::Seconds(10)) ";

%feature("docstring")
cmf::math::ExplicitEuler_fixed::ExplicitEuler_fixed "ExplicitEuler_fixed(real epsilon=1e-9, cmf::math::Time
tStepMin=Time::Seconds(10))

Constructs a new ExplicitEuler_fixed.

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

Integrates the vector of state variables.

Parameters:
-----------

MaxTime:  (ignored) To stop the model (if running in a model
framework) at time steps of value exchange e.g. full hours, the next
value exchange time can be given

TimeStep:  Takes the proposed time step ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::count "int
count() const

returns the number of state variables ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::state "real
state(int position) const

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::state "void
state(int position, real newState)

Simplifies the assessment of state variables. ";


// File: a00540.xml
%feature("docstring") std::ios_base::failure "

STL class. ";


// File: a00014.xml
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
conc(cmf::math::Time t, const cmf::water::solute &solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";

%feature("docstring")  cmf::water::flux_connection::get_tracer_filter
"real get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")  cmf::water::flux_connection::set_tracer_filter
"void set_tracer_filter(real value) ";

%feature("docstring")  cmf::water::flux_connection::to_string "std::string to_string() const ";

%feature("docstring")  cmf::water::flux_connection::short_string "std::string short_string() const ";

%feature("docstring")  cmf::water::flux_connection::flux_connection "flux_connection(flux_node::ptr left, flux_node::ptr right, std::string
_type)

Creates a flux connection between the flux_node left and right.

Parameters:
-----------

left:   flux_node on the one side of the connection

right:   flux_node on the other side of the connection

_type:  Type of the flux connection ";

%feature("docstring")  cmf::water::flux_connection::~flux_connection "virtual ~flux_connection() ";


// File: a00015.xml
%feature("docstring") cmf::water::flux_node "

Base class for everything that can be connected by fluxes. Flux nodes
can be WaterStorages, flux end points, sinks, sources and bridges to
other model domains (e.g. Ponded water to river system). The base
class can be used where a simple routing, potentially with mixing, is
needed.

C++ includes: flux_node.h ";

%feature("docstring")  cmf::water::flux_node::get_position "cmf::geometry::point get_position() const

Pure virtual function. Should return the position of the locatable. ";

%feature("docstring")  cmf::water::flux_node::set_position "virtual
void set_position(cmf::geometry::point p)

Sets the position of the node in space. ";

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
state changes require an update of the fluxes. ";

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
Single fluxes can be excluded from the calculation.

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the water_balance
(e.g. to prevent closed circuits) ";

%feature("docstring")  cmf::water::flux_node::conc "virtual real
conc(cmf::math::Time t, const cmf::water::solute &solute) const

Returns the water quality of the flux_node, if it is not overridden
this is the mix of the incoming fluxes. ";

%feature("docstring")  cmf::water::flux_node::get_potential "virtual
real get_potential() const

Returns the water potential of the node in m waterhead The base class
water storage always returns the height of the location. ";

%feature("docstring")  cmf::water::flux_node::set_potential "virtual
void set_potential(real new_potential) ";

%feature("docstring")  cmf::water::flux_node::is_empty "virtual bool
is_empty() const ";

%feature("docstring")  cmf::water::flux_node::flux_node "flux_node(const cmf::project &_project, cmf::geometry::point
location=cmf::geometry::point()) ";

%feature("docstring")  cmf::water::flux_node::get_distance_to "double
get_distance_to(const Locatable &cmp)

Returns the distance between two locatable objects. ";

%feature("docstring")  cmf::water::flux_node::get_direction_to "cmf::geometry::point get_direction_to(const Locatable &cmp)

Returns a vector with length=1 pointing in the direction of another
Locatable. ";


// File: a00477.xml
%feature("docstring") std::fstream "

STL class. ";


// File: a00546.xml
%feature("docstring") cmf::upslope::ET::HargreaveET "";

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
conc(cmf::math::Time t, const cmf::water::solute &solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";

%feature("docstring")
cmf::upslope::ET::HargreaveET::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::ET::HargreaveET::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")  cmf::upslope::ET::HargreaveET::to_string "std::string to_string() const ";

%feature("docstring")  cmf::upslope::ET::HargreaveET::short_string "std::string short_string() const ";


// File: a00016.xml
%feature("docstring") cmf::upslope::connections::HBVlateral "

Lateral flow using a simple linear storage, can be directed to another
cell (HBV D like) or to a reach (HBV like).

(see Formula in C++ documentation)

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

%feature("docstring")  cmf::upslope::connections::HBVlateral::conc "real conc(cmf::math::Time t, const cmf::water::solute &solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";

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


// File: a00017.xml
%feature("docstring") cmf::upslope::connections::HBVparameters "

This class holds the properties for all HBV like connections
Implements the RetentionCurve interface is only partly, therefore it
is not suitable for other connection types.

(see Formula in C++ documentation)

C++ includes: HBVflow.h ";

%feature("docstring")
cmf::upslope::connections::HBVparameters::HBVparameters "HBVparameters(double _k0=1, double _k1=0.25, double _k2=0.005, double
_perc=0.05, double _fc=0.3, double _beta=4.0, double _uplim=.35,
double _lowlim=1.0, double _cfmax=2, double _sfcf=0.6, double
_cwh=0.1, double _cfr=0.05) ";

%feature("docstring")  cmf::upslope::connections::HBVparameters::K "virtual real K(real wetness, real depth) const

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

Returns the effective wetness, using a residual pF value (see Formula
in C++ documentation) . ";

%feature("docstring")
cmf::upslope::connections::HBVparameters::FillHeight "virtual real
FillHeight(real lowerDepth, real Area, real Volume) const

Returns the thickness of a soil column with a certain pore volume. ";

%feature("docstring")
cmf::upslope::connections::HBVparameters::Transmissivity "virtual
real Transmissivity(real upperDepth, real lowerDepth, real wetness)
const

Returns the transmissivity of a part of a soil column. ";

%feature("docstring")
cmf::upslope::connections::HBVparameters::Wetness_pF "real
Wetness_pF(real pF) const

returns the volumetric water content at a given pF value ";


// File: a00018.xml
%feature("docstring") cmf::upslope::connections::HBVpercolation "

Calculates the percolation as in the HBV model.

(see Formula in C++ documentation)

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
"real conc(cmf::math::Time t, const cmf::water::solute &solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";

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


// File: a00019.xml
%feature("docstring") cmf::river::IChannel "

Structure for the description of structural parameters of a reach
Abstract base class for different IChannel geometries.

C++ includes: ReachType.h ";

%feature("docstring")  cmf::river::IChannel::get_nManning "virtual
double get_nManning() const ";

%feature("docstring")  cmf::river::IChannel::set_nManning "virtual
void set_nManning(double val) ";

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

Calculates the flow rate from a given water volume in the reach (see
Formula in C++ documentation) .

Flow rate [m3/s]

Parameters:
-----------

A:  The area of the cross section [m2]

slope:  The slope of the reach [m/m] ";

%feature("docstring")  cmf::river::IChannel::IChannel "IChannel(double l) ";


// File: a00480.xml
%feature("docstring") std::ifstream "

STL class. ";


// File: a00020.xml
%feature("docstring") cmf::math::ImplicitEuler "

An implicit (backward) Euler integrator using fixpoint iteration.

C++ includes: FixpointImplicitEuler.h ";

/*  Internal data storages  */

/*  Assessment of state variables for integration  */

%feature("docstring")  cmf::math::ImplicitEuler::AddState "virtual
void AddState(cmf::math::StateVariable &statevar) ";

/*  model time  */

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

/*  Integrate  */

%feature("docstring")  cmf::math::ImplicitEuler::Integrate "int
Integrate(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::ImplicitEuler::IntegrateUntil "void
IntegrateUntil(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::ImplicitEuler::IntegrateUntil "void
IntegrateUntil(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables until MaxTime. ";

/*  The state variables to integrate  */

/*  Accuracy parameters  */

%feature("docstring")  cmf::math::ImplicitEuler::AddStatesFromOwner "void AddStatesFromOwner(cmf::math::StateVariableOwner &stateOwner)

Adds the state variables of a StateVariableOwner to the state
variables of the solver. ";

%feature("docstring")  cmf::math::ImplicitEuler::ImplicitEuler "ImplicitEuler(StateVariableOwner &states, real epsilon=1e-9,
cmf::math::Time tStepMin=Time::Seconds(10))

Constructs a new FixPointImplicitEuler from a pointer to a vector of
state variables.

The Integrator becomes the owner of states

Parameters:
-----------

states:  Statevariables of the system

epsilon:  relative error tolerance per time step (default=1e-9)

tStepMin:  minimum time step (default=10s) ";

%feature("docstring")  cmf::math::ImplicitEuler::ImplicitEuler "ImplicitEuler(real epsilon=1e-9, cmf::math::Time
tStepMin=Time::Seconds(10))

Constructs a new FixPointImplicitEuler.

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

Integrates the vector of state variables.

Parameters:
-----------

MaxTime:  To stop the model (if running in a model framework) at time
steps of value exchange e.g. full hours, the next value exchange time
can be given

TimeStep:  Takes the proposed timestep, and changes it into the
effictivly used timestep according to the local stiffness of the
problem and MaxTime ";

%feature("docstring")  cmf::math::ImplicitEuler::count "int count()
const

returns the number of state variables ";

%feature("docstring")  cmf::math::ImplicitEuler::state "real
state(int position) const

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::ImplicitEuler::state "void
state(int position, real newState)

Simplifies the assessment of state variables. ";


// File: a00021.xml
%feature("docstring") cmf::math::Integrator "

Base class for any kind of integrator.

Pure virtual functions: Integrate

Copy Please provide a custom copy constructorTodo Put the methods of
StateVariableVector here, and delete StateVariableVector

C++ includes: Integrator.h ";

/*  The state variables to integrate  */

/*  Accuracy parameters  */

/*  model time  */

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

/*  Assessment of state variables for integration  */

%feature("docstring")  cmf::math::Integrator::AddStatesFromOwner "virtual void AddStatesFromOwner(cmf::math::StateVariableOwner
&stateOwner)

Adds the state variables of a StateVariableOwner to the state
variables of the solver. ";

%feature("docstring")  cmf::math::Integrator::AddState "virtual void
AddState(cmf::math::StateVariable &statevar) ";

/*  Constructors and Destructors  */

%feature("docstring")  cmf::math::Integrator::Integrator "Integrator(const StateVariableVector &states, real epsilon=1e-9,
cmf::math::Time tStepMin=10.0/(3600.0 *24.0))

Constructs a new FixPointImplicitEuler from a pointer to a vector of
state variables.

The RKF Integrator becomes the owner of states

Parameters:
-----------

states:  Statevariables of the system

epsilon:  relative error tolerance per time step (default=1e-9)

tStepMin:  minimum time step (default=10s) ";

%feature("docstring")  cmf::math::Integrator::Integrator "Integrator(real epsilon=1e-9, cmf::math::Time
tStepMin=Time::Seconds(10))

Constructs a new Integrator with a new own state vector.

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

%feature("docstring")  cmf::math::Integrator::Integrate "virtual int
Integrate(cmf::math::Time MaxTime, cmf::math::Time TimeStep)=0

Integrates the vector of state variables.

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

%feature("docstring")  cmf::math::Integrator::count "int count()
const

returns the number of state variables ";

%feature("docstring")  cmf::math::Integrator::state "real state(int
position) const

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::Integrator::state "void state(int
position, real newState)

Simplifies the assessment of state variables. ";


// File: a00551.xml
%feature("docstring") std::invalid_argument "

STL class. ";


// File: a00485.xml
%feature("docstring") std::ios "

STL class. ";


// File: a00552.xml
%feature("docstring") std::ios_base "

STL class. ";


// File: a00490.xml
%feature("docstring") std::istream "

STL class. ";


// File: a00492.xml
%feature("docstring") std::istringstream "

STL class. ";


// File: a00507.xml
%feature("docstring") std::basic_string::iterator "

STL iterator class. ";


// File: a00553.xml
%feature("docstring") std::string::iterator "

STL iterator class. ";


// File: a00554.xml
%feature("docstring") std::wstring::iterator "

STL iterator class. ";


// File: a00535.xml
%feature("docstring") std::deque::iterator "

STL iterator class. ";


// File: a00555.xml
%feature("docstring") std::list::iterator "

STL iterator class. ";


// File: a00556.xml
%feature("docstring") std::map::iterator "

STL iterator class. ";


// File: a00557.xml
%feature("docstring") std::multimap::iterator "

STL iterator class. ";


// File: a00558.xml
%feature("docstring") std::set::iterator "

STL iterator class. ";


// File: a00559.xml
%feature("docstring") std::multiset::iterator "

STL iterator class. ";


// File: a00560.xml
%feature("docstring") std::vector::iterator "

STL iterator class. ";


// File: a00022.xml
%feature("docstring") cmf::river::IVolumeHeightFunction "

Volume height relations are functional objects, which return a height
and a crosssectional area of a volume for different geometric bodies.
This is the abstract base class, where the geometries derive from.

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


// File: a00023.xml
%feature("docstring")
cmf::upslope::connections::lateral_sub_surface_flux "

An abstract base class for lateral subsurface fluxes.

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
conc(cmf::math::Time t, const cmf::water::solute &solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";

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


// File: a00562.xml
%feature("docstring") std::length_error "

STL class. ";


// File: a00024.xml
%feature("docstring") cmf::water::linear_scale "

A linear scaling functor, with slope and displacement.

C++ includes: boundary_condition.h ";

%feature("docstring")  cmf::water::linear_scale::linear_scale "linear_scale(real _slope=1, real _displacement=0)

Creates a linear scale (by default it is a unity scale, $a=1; b=0$).
";


// File: a00025.xml
%feature("docstring") cmf::upslope::LinearRetention "

The linear retention curve provides a simple linear relationship
between storage and head.

Head function (head in m, calculated from upper side control volume)
(see Formula in C++ documentation)  Conductivity function (see Formula
in C++ documentation)

C++ includes: RetentionCurve.h ";

%feature("docstring")  cmf::upslope::LinearRetention::MatricPotential
"virtual real MatricPotential(real wetness) const

returns the wetness of the soil at given water content ";

%feature("docstring")  cmf::upslope::LinearRetention::Wetness "virtual real Wetness(real suction) const

Returns the wetness ( water content per pore volume) at a given head.
";

%feature("docstring")  cmf::upslope::LinearRetention::K "virtual real
K(real wetness, real depth) const

Returns the conductivity in m/day at a certain depth and water
content. ";

%feature("docstring")  cmf::upslope::LinearRetention::VoidVolume "virtual real VoidVolume(real upperDepth, real lowerDepth, real Area)
const

Returns the void volume of a soil column. ";

%feature("docstring")  cmf::upslope::LinearRetention::Transmissivity "virtual real Transmissivity(real upperDepth, real lowerDepth, real
wetness) const

Returns the transmissivity of a part of a soil column. ";

%feature("docstring")  cmf::upslope::LinearRetention::Porosity "virtual real Porosity(real depth) const

Returns the porosity at a certain depth. ";

%feature("docstring")  cmf::upslope::LinearRetention::FillHeight "virtual real FillHeight(real lowerDepth, real Area, real Volume) const

Returns the thickness of a soil column with a certain pore volume. ";

%feature("docstring")  cmf::upslope::LinearRetention::copy "LinearRetention* copy() const ";

%feature("docstring")  cmf::upslope::LinearRetention::LinearRetention
"LinearRetention(real _Ksat, real _Phi, real _thickness, real
_beta=1.0, real Ss=1e-4, real _residual_wetness=0.0, real
_ksat_decay=0.0, real _porosity_decay=0.0) ";

%feature("docstring")  cmf::upslope::LinearRetention::Wetness_eff "virtual real Wetness_eff(real wetness, real pF_r=4.2) const

Returns the effective wetness, using a residual pF value (see Formula
in C++ documentation) . ";

%feature("docstring")  cmf::upslope::LinearRetention::Wetness_pF "real Wetness_pF(real pF) const

returns the volumetric water content at a given pF value ";


// File: a00563.xml
%feature("docstring") std::list "

STL class. ";


// File: a00026.xml
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


// File: a00565.xml
%feature("docstring") cmf::geometry::Location "";

%feature("docstring")  cmf::geometry::Location::get_position "cmf::geometry::point get_position() const ";

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


// File: a00541.xml
%feature("docstring") std::logic_error "

STL class. ";


// File: a00027.xml
%feature("docstring") cmf::river::Manning "

Calculates the flux between two open water bodies, using Manning's
equation.

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
conc(cmf::math::Time t, const cmf::water::solute &solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";

%feature("docstring")  cmf::river::Manning::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")  cmf::river::Manning::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")  cmf::river::Manning::to_string "std::string
to_string() const ";

%feature("docstring")  cmf::river::Manning::short_string "std::string
short_string() const ";


// File: a00567.xml
%feature("docstring") cmf::river::Manning_Diffusive "";

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
conc(cmf::math::Time t, const cmf::water::solute &solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";

%feature("docstring")
cmf::river::Manning_Diffusive::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::river::Manning_Diffusive::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")  cmf::river::Manning_Diffusive::to_string "std::string to_string() const ";

%feature("docstring")  cmf::river::Manning_Diffusive::short_string "std::string short_string() const ";


// File: a00568.xml
%feature("docstring") cmf::river::Manning_Kinematic "";

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
conc(cmf::math::Time t, const cmf::water::solute &solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";

%feature("docstring")
cmf::river::Manning_Kinematic::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::river::Manning_Kinematic::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")  cmf::river::Manning_Kinematic::to_string "std::string to_string() const ";

%feature("docstring")  cmf::river::Manning_Kinematic::short_string "std::string short_string() const ";


// File: a00569.xml
%feature("docstring") std::map "

STL class. ";


// File: a00028.xml
%feature("docstring") cmf::upslope::connections::MatrixInfiltration "

Connects the surfacewater and the most upper layer.

If UpslopeCell::InfiltrationExcess and Cell is not saturated (see
Formula in C++ documentation)  else (see Formula in C++ documentation)

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
conc(cmf::math::Time t, const cmf::water::solute &solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";

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


// File: a00029.xml
%feature("docstring") cmf::river::MeanChannel "

A combination of two channel geometries.

C++ includes: ReachType.h ";

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

Calculates the flow rate from a given water volume in the reach (see
Formula in C++ documentation) .

Flow rate [m3/s]

Parameters:
-----------

A:  The area of the cross section [m2]

slope:  The slope of the reach [m/m] ";


// File: a00030.xml
%feature("docstring") cmf::atmosphere::Meteorology "

An abstract class, for objects generating Weather records at a
sepcific time.

C++ includes: Meteorology.h ";

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


// File: a00031.xml
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

import CMFlib as cmf                  latitude=51.2 # Latitude of
station in decimal degrees                  longitude=8.1 # Longitude
of station in decimal degrees (only needed for daily=false) timezone=1
# Timezone, pos. values mean east of GMT, negative west (Germany=1,
Pacific time=-8, only needed for daily=false) start=cmf.Time(1,1,2001)
# Creates all timeseries with this start time, one can change them
later                  step=cmf.day # s. start
name=\"Giessen\"           # A name for the station (optional)
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
2009, 2pm                  print 'Global Radiation: ',weather.Rs
# Daily mean Rs, since daily=true print 'Temperature:',weather.T
# Daily mean T, since nothing else in known

C++ includes: Meteorology.h ";

/*  Location and behaviour properties  */

%feature("docstring")  cmf::atmosphere::MeteoStation::get_position "virtual cmf::geometry::point get_position() const

Pure virtual function. Should return the position of the locatable. ";

/*  Timeseries of meteorological data  */

/*  Data access methods  */

%feature("docstring")  cmf::atmosphere::MeteoStation::get_data "cmf::atmosphere::Weather get_data(cmf::math::Time t, double height)
const

Returns the current Atmosphere state. Uses default values for missing
timeseries. ";

%feature("docstring")
cmf::atmosphere::MeteoStation::SetSunshineFraction "void
SetSunshineFraction(cmf::math::timeseries sunshine_duration)

Returns the global radiation at a given time step (see Formula in C++
documentation) ,
seehttp://www.fao.org/docrep/X0490E/x0490e07.htm#radiation (see
Formula in C++ documentation)  double
get_global_radiation(cmf::math::Time t,double height,double
sunshine_fraction) const;.

Calculates a timeseries of the sunshine fraction (to put into
Sunshine) from a timeseries of absolute sunshine duration, using the
potential sunshine duration in hours,
seehttp://www.fao.org/docrep/X0490E/x0490e07.htm#radiation (see
Formula in C++ documentation)  ";

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


// File: a00032.xml
%feature("docstring") cmf::atmosphere::MeteoStationList "

A list of meteorological stations.

Can find the nearest station for a position and calculate the
temperature lapse

C++ includes: Meteorology.h ";

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
"meteo_station_pointer add_station(std::string name, double
latitude=51, double longitude=8, double timezone=1, double
elevation=0, cmf::math::Time startTime=cmf::math::Time(1, 1, 2001),
cmf::math::Time timestep=cmf::math::day)

Creates a meteorological station and adds it to the list. ";

%feature("docstring")  cmf::atmosphere::MeteoStationList::add_station
"meteo_station_pointer add_station(std::string name,
cmf::geometry::point position, double latitude=51, double longitude=8,
double timezone=1, cmf::math::Time startTime=cmf::math::Time(1, 1,
2001), cmf::math::Time timestep=cmf::math::day)

Creates a meteorological station at a certain position and adds it to
the list.

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
at position.

The distance is calculated as (see Formula in C++ documentation)
Where (see Formula in C++ documentation)  is the station and (see
Formula in C++ documentation)  is the locatable A Meteorology using
the data of the nearest station to position

Parameters:
-----------

position:  The position (any locatable, like e.g. Cell possible) to
look for the station. The reference should be owned by the locatable

z_weight:  The weight of the height difference (see Formula in C++
documentation)  ";


// File: a00033.xml
%feature("docstring") cmf::atmosphere::MeteoStationReference "

A reference to a meteorological station. Returns the weather at a
given time for its place using MeteoStation::T_lapse.

C++ includes: Meteorology.h ";

%feature("docstring")
cmf::atmosphere::MeteoStationReference::get_station "meteo_station_pointer get_station() const

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
cmf::atmosphere::MeteoStationReference::MeteoStationReference "MeteoStationReference(meteo_station_pointer station, const
cmf::geometry::Locatable &location) ";

%feature("docstring")
cmf::atmosphere::MeteoStationReference::MeteoStationReference "MeteoStationReference(const MeteoStationReference &copy) ";

%feature("docstring")  cmf::atmosphere::MeteoStationReference::copy "MeteoStationReference* copy() const

Returns a copy of the meteorology object. Pure virtual function, needs
to be implemented. ";

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


// File: a00034.xml
%feature("docstring") cmf::math::MultiIntegrator "

The MultiIntegrator is a wrapper for a bunch integrators. The states
of the integrators should not have direct connections over integrator
boundaries.

C++ includes: MultiIntegrator.h ";

/*  Assessment of state variables for integration  */

%feature("docstring")  cmf::math::MultiIntegrator::AddState "virtual
void AddState(cmf::math::StateVariable &statevar) ";

/*  model time  */

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

/*  Integrate  */

%feature("docstring")  cmf::math::MultiIntegrator::Integrate "int
Integrate(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::MultiIntegrator::IntegrateUntil "void IntegrateUntil(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::MultiIntegrator::IntegrateUntil "void IntegrateUntil(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables until MaxTime. ";

/*  The state variables to integrate  */

/*  Accuracy parameters  */

%feature("docstring")  cmf::math::MultiIntegrator::Integrate "int
Integrate(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables.

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

Adds the state variables of a StateVariableOwner to the state
variables of the solver. ";

%feature("docstring")
cmf::math::MultiIntegrator::add_states_to_integrator "void
add_states_to_integrator(cmf::math::StateVariableOwner &stateOwner,
int integrator_position) ";

%feature("docstring")  cmf::math::MultiIntegrator::MultiIntegrator "MultiIntegrator(const cmf::math::Integrator &template_integrator, int
count) ";

%feature("docstring")  cmf::math::MultiIntegrator::count "int count()
const

returns the number of state variables ";

%feature("docstring")  cmf::math::MultiIntegrator::state "real
state(int position) const

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::MultiIntegrator::state "void
state(int position, real newState)

Simplifies the assessment of state variables. ";


// File: a00571.xml
%feature("docstring") std::multimap "

STL class. ";


// File: a00573.xml
%feature("docstring") std::multiset "

STL class. ";


// File: a00035.xml
%feature("docstring") cmf::upslope::NeighborIterator "

A class to iterate through the neighbors of a cell (const). Not needed
from the Python side, use the generator cell.neighbors instead.

C++ includes: Topology.h ";

%feature("docstring")
cmf::upslope::NeighborIterator::NeighborIterator "NeighborIterator(cmf::upslope::Topology &topo) ";

%feature("docstring")
cmf::upslope::NeighborIterator::NeighborIterator "NeighborIterator(cmf::upslope::Cell *cell) ";

%feature("docstring")  cmf::upslope::NeighborIterator::cell "Cell&
cell() ";

%feature("docstring")  cmf::upslope::NeighborIterator::flowwidth "double flowwidth() ";

%feature("docstring")  cmf::upslope::NeighborIterator::valid "bool
valid() const ";

%feature("docstring")  cmf::upslope::NeighborIterator::next "NeighborIterator& next()

Points the iterator to the next neighbor. ";


// File: a00036.xml
%feature("docstring") cmf::water::NeumannBoundary "

A Neumann boundary condition (constant flux boundary condition).

The flux is a timeseries, but can be used as a scalar. To scale the
timeseries to the specific conditions of this boundary condition the
linear_scale flux_scale can be used.

C++ includes: boundary_condition.h ";

%feature("docstring")  cmf::water::NeumannBoundary::conc "real
conc(cmf::math::Time t, const cmf::water::solute &solute) const

Returns the solute concentrations of the flux at a given time. ";

%feature("docstring")  cmf::water::NeumannBoundary::is_empty "bool
is_empty() const ";

%feature("docstring")  cmf::water::NeumannBoundary::RecalcFluxes "bool RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node. WaterStorage overrides this, since
state changes require an update of the fluxes. ";

%feature("docstring")  cmf::water::NeumannBoundary::connect_to "void
connect_to(cmf::water::flux_node::ptr target) ";

%feature("docstring")  cmf::water::NeumannBoundary::NeumannBoundary "NeumannBoundary(const cmf::project &_project, cmf::math::timeseries
_flux, cmf::water::SoluteTimeseries
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

%feature("docstring")  cmf::water::NeumannBoundary::NeumannBoundary "NeumannBoundary(const cmf::project &_project, cmf::geometry::point
loc=cmf::geometry::point()) ";

%feature("docstring")  cmf::water::NeumannBoundary::get_position "cmf::geometry::point get_position() const

Pure virtual function. Should return the position of the locatable. ";

%feature("docstring")  cmf::water::NeumannBoundary::set_position "virtual void set_position(cmf::geometry::point p)

Sets the position of the node in space. ";

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
Single fluxes can be excluded from the calculation.

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the water_balance
(e.g. to prevent closed circuits) ";

%feature("docstring")  cmf::water::NeumannBoundary::get_potential "virtual real get_potential() const

Returns the water potential of the node in m waterhead The base class
water storage always returns the height of the location. ";

%feature("docstring")  cmf::water::NeumannBoundary::set_potential "virtual void set_potential(real new_potential) ";

%feature("docstring")  cmf::water::NeumannBoundary::get_distance_to "double get_distance_to(const Locatable &cmp)

Returns the distance between two locatable objects. ";

%feature("docstring")  cmf::water::NeumannBoundary::get_direction_to "cmf::geometry::point get_direction_to(const Locatable &cmp)

Returns a vector with length=1 pointing in the direction of another
Locatable. ";


// File: a00037.xml
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

%feature("docstring")  cmf::water::NeumannBoundary_list::get "NeumannBoundary_ptr get(int index) const

Returns the Neumann boundary condition at position index. ";

%feature("docstring")  cmf::water::NeumannBoundary_list::get_fluxes "cmf::math::num_array get_fluxes(cmf::math::Time t=cmf::math::Time())
const

Returns the fluxes of the items as an array. ";

%feature("docstring")  cmf::water::NeumannBoundary_list::set_fluxes "void set_fluxes(cmf::math::num_array values) ";

%feature("docstring")  cmf::water::NeumannBoundary_list::append "void
append(NeumannBoundary_ptr nbc) ";

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

Returns the sum of the water balances of the nodes (see Formula in C++
documentation) .

Replaces slow Python code like: ";

%feature("docstring")  cmf::water::NeumannBoundary_list::water_balance
"cmf::math::num_array water_balance(cmf::math::Time t) const

Returns the water balance of each vector as a vector (see Formula in
C++ documentation) .

Replaces slow Python code like: ";


// File: a00038.xml
%feature("docstring") cmf::water::NeumannFlux "

This flux_connection is created, when connecting a Neumann boundary
condition with a state variable using Neumann::connect_to.

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
conc(cmf::math::Time t, const cmf::water::solute &solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";

%feature("docstring")  cmf::water::NeumannFlux::get_tracer_filter "real get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")  cmf::water::NeumannFlux::set_tracer_filter "void set_tracer_filter(real value) ";

%feature("docstring")  cmf::water::NeumannFlux::to_string "std::string to_string() const ";

%feature("docstring")  cmf::water::NeumannFlux::short_string "std::string short_string() const ";


// File: a00039.xml
%feature("docstring") cmf::water::node_list "

A collection of nodes for fast access of the waterbalance.

In setups with many storages and rather fast computations, the speed
of data access for output generation can take a high portion of the
total run time. To accelerate data access, one can use the node_list
object

C++ includes: collections.h ";

%feature("docstring")  cmf::water::node_list::size "int size() const

The number of nodes. ";

%feature("docstring")  cmf::water::node_list::begin "node_vector::iterator begin() ";

%feature("docstring")  cmf::water::node_list::end "node_vector::iterator end() ";

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

%feature("docstring")  cmf::water::node_list::AddStateVariables "void
AddStateVariables(cmf::math::StateVariableVector &vector)

Implements StateVariableOwner. ";

%feature("docstring")  cmf::water::node_list::append "void
append(flux_node::ptr node)

Adds a flux node to the list. ";

%feature("docstring")  cmf::water::node_list::set_potentials "int
set_potentials(const cmf::math::num_array &potentials)

Sets the potentials of the node_lists If node do not have changeabe
potentials, they are skipped.

The number of nodes with changed potential ";

%feature("docstring")  cmf::water::node_list::get_potentials "cmf::math::num_array get_potentials() ";

%feature("docstring")  cmf::water::node_list::global_water_balance "real global_water_balance(cmf::math::Time t) const

Returns the sum of the water balances of the nodes (see Formula in C++
documentation) .

Replaces slow Python code like: ";

%feature("docstring")  cmf::water::node_list::water_balance "cmf::math::num_array water_balance(cmf::math::Time t) const

Returns the water balance of each vector as a vector (see Formula in
C++ documentation) .

Replaces slow Python code like: ";

%feature("docstring")  cmf::water::node_list::conc "cmf::math::num_array conc(cmf::math::Time t, const cmf::water::solute
&solute) const

Returns an array holding the concentration of all the flux nodes for
the given solute. ";

%feature("docstring")  cmf::water::node_list::set_solute_source "int
set_solute_source(const cmf::water::solute &solute,
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


// File: a00040.xml
%feature("docstring") cmf::math::num_array "

A valarray kind of vector implementation with OpenMP capabilities.

C++ includes: num_array.h ";

/*  Constructors & Destructors  */

%feature("docstring")  cmf::math::num_array::num_array "num_array(int
count, real Value=0)

Creates a new vector with size count.

Parameters:
-----------

count:  size of new vector

Value:  Value of each element (default=0) ";

%feature("docstring")  cmf::math::num_array::num_array "num_array(size_t count, real Value=0) ";

%feature("docstring")  cmf::math::num_array::num_array "num_array()

Creates a vector of size 1 with value {0}. ";

%feature("docstring")  cmf::math::num_array::num_array "num_array(const num_array &Vector)

Copy constructor. ";

%feature("docstring")  cmf::math::num_array::num_array "num_array(double *data, int count) ";

%feature("docstring")  cmf::math::num_array::set_data_from_adress "void set_data_from_adress(size_t data_adress, size_t count) ";

%feature("docstring")  cmf::math::num_array::adress "size_t adress()
const ";

%feature("docstring")  cmf::math::num_array::num_array "num_array(const std::valarray< real > &Vector)

Convert from valarray<real>. ";

%feature("docstring")  cmf::math::num_array::~num_array "~num_array()

Destructor. ";

/*  Operators  */

/* Binary operators defined as free operators:

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

Return funct(this).

Parameters:
-----------

funct:  a function of signature real funct(real)

Example: Return the sinus of each element of vector x ";

/*  Summarizing functions  */

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
Euclidean distance, 0 Maximum of abs.

Parameters:
-----------

normtype:  An integer indicating the type of norm ";

%feature("docstring")  cmf::math::num_array::size "int size() const

Size of the vector. ";

%feature("docstring")  cmf::math::num_array::resize "void resize(int
count)

Changes the size of the vector. ";

%feature("docstring")  cmf::math::num_array::resize "void
resize(size_t count) ";


// File: a00495.xml
%feature("docstring") std::ofstream "

STL class. ";


// File: a00041.xml
%feature("docstring") cmf::upslope::connections::OHDISflow "

A connection similar to OHDIS-KWMSS (OHymos-based DIStributed model -
with Kinematic Wave Method for Surface and Subsurface runoff).

(see Formula in C++ documentation)

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

%feature("docstring")  cmf::upslope::connections::OHDISflow::conc "real conc(cmf::math::Time t, const cmf::water::solute &solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";

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


// File: a00042.xml
%feature("docstring") cmf::river::OpenWaterStorage "

An open water body. The potential is calculated from the stored water
using a water table function.

C++ includes: OpenWaterStorage.h ";

/*  Overrides of flux_node  */

%feature("docstring")  cmf::river::OpenWaterStorage::RecalcFluxes "virtual bool RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node. WaterStorage overrides this, since
state changes require an update of the fluxes. ";

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

Returns the gravitational potential (see Formula in C++ documentation)
. ";

%feature("docstring")  cmf::river::OpenWaterStorage::conc "real
conc(cmf::math::Time t, const cmf::water::solute &solute) const

Returns the current WaterQuality (concentration of all solutes). ";

%feature("docstring")  cmf::river::OpenWaterStorage::is_storage "virtual bool is_storage() const

true, if this is a waterstorage ";

%feature("docstring")  cmf::river::OpenWaterStorage::AddStateVariables
"void AddStateVariables(cmf::math::StateVariableVector &vector)

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";

%feature("docstring")  cmf::river::OpenWaterStorage::Solute "SoluteStorage& Solute(const cmf::water::solute &solute)

Returns the water quality of the water storage. ";

%feature("docstring")  cmf::river::OpenWaterStorage::Solute "const
SoluteStorage& Solute(const cmf::water::solute &solute) const ";

%feature("docstring")  cmf::river::OpenWaterStorage::conc "real
conc(const cmf::water::solute &solute) const

Returns the concentration of the given solute. ";

%feature("docstring")  cmf::river::OpenWaterStorage::conc "void
conc(const cmf::water::solute &solute, real NewConcetration)

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

%feature("docstring")  cmf::river::OpenWaterStorage::get_position "cmf::geometry::point get_position() const

Pure virtual function. Should return the position of the locatable. ";

%feature("docstring")  cmf::river::OpenWaterStorage::set_position "virtual void set_position(cmf::geometry::point p)

Sets the position of the node in space. ";

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
Single fluxes can be excluded from the calculation.

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the water_balance
(e.g. to prevent closed circuits) ";

%feature("docstring")  cmf::river::OpenWaterStorage::set_potential "virtual void set_potential(real new_potential) ";

%feature("docstring")  cmf::river::OpenWaterStorage::get_distance_to "double get_distance_to(const Locatable &cmp)

Returns the distance between two locatable objects. ";

%feature("docstring")  cmf::river::OpenWaterStorage::get_direction_to
"cmf::geometry::point get_direction_to(const Locatable &cmp)

Returns a vector with length=1 pointing in the direction of another
Locatable. ";


// File: a00498.xml
%feature("docstring") std::ostream "

STL class. ";


// File: a00500.xml
%feature("docstring") std::ostringstream "

STL class. ";


// File: a00566.xml
%feature("docstring") std::out_of_range "

STL class. ";


// File: a00575.xml
%feature("docstring") std::overflow_error "

STL class. ";


// File: a00547.xml
%feature("docstring") cmf::upslope::ET::PenmanEvaporation "";

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

%feature("docstring")  cmf::upslope::ET::PenmanEvaporation::conc "real conc(cmf::math::Time t, const cmf::water::solute &solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";

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


// File: a00043.xml
%feature("docstring") cmf::upslope::ET::PenmanMonteithET "

Calculates the potential evapotranspiration according to FAO(1998).

Gouverning equations: (see Formula in C++ documentation)

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
conc(cmf::math::Time t, const cmf::water::solute &solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";

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


// File: a00549.xml
%feature("docstring") cmf::river::PipeReach "";

%feature("docstring")  cmf::river::PipeReach::typecode "char
typecode() const ";

%feature("docstring")  cmf::river::PipeReach::get_channel_width "virtual double get_channel_width(double depth) const

(see Formula in C++ documentation)  ";

%feature("docstring")  cmf::river::PipeReach::get_wetted_perimeter "virtual double get_wetted_perimeter(double depth) const

(see Formula in C++ documentation)  ";

%feature("docstring")  cmf::river::PipeReach::get_depth "virtual
double get_depth(double area) const

(see Formula in C++ documentation)  ";

%feature("docstring")  cmf::river::PipeReach::get_flux_crossection "virtual double get_flux_crossection(double depth) const

(see Formula in C++ documentation)  ";

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

Calculates the flow rate from a given water volume in the reach (see
Formula in C++ documentation) .

Flow rate [m3/s]

Parameters:
-----------

A:  The area of the cross section [m2]

slope:  The slope of the reach [m/m] ";


// File: a00044.xml
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

Returns the euclidian distance to another point. (see Formula in C++
documentation) . ";

%feature("docstring")  cmf::geometry::point::distance3DTo "double
distance3DTo(point p) const ";

%feature("docstring")  cmf::geometry::point::distance_max "double
distance_max(point p) const

Returns the distance by the maximum orthogonal offset. ";

%feature("docstring")  cmf::geometry::point::azimuth "double
azimuth(point p) const

Returns the azimuth angle of the line (see Formula in C++
documentation)  to the Azimuth in degrees. ";

%feature("docstring")  cmf::geometry::point::angleToXAxis "double
angleToXAxis(point p) const

Returns the angle between the line (see Formula in C++ documentation)
to the x-Axis in degrees. ";


// File: a00576.xml
%feature("docstring") cmf::geometry::point_vector "";

%feature("docstring")  cmf::geometry::point_vector::point_vector "point_vector(int size) ";

%feature("docstring")  cmf::geometry::point_vector::get "point
get(int index) const ";

%feature("docstring")  cmf::geometry::point_vector::set "void set(int
index, cmf::geometry::point p) ";

%feature("docstring")  cmf::geometry::point_vector::size "int size()
const ";


// File: a00045.xml
%feature("docstring") cmf::math::PredictCorrectSimple "

A simple predictor - corrector solver (see Formula in C++
documentation) .

C++ includes: ExplicitEuler_fixed.h ";

/*  Assessment of state variables for integration  */

%feature("docstring")  cmf::math::PredictCorrectSimple::AddState "virtual void AddState(cmf::math::StateVariable &statevar) ";

/*  model time  */

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

/*  Integrate  */

%feature("docstring")  cmf::math::PredictCorrectSimple::Integrate "int Integrate(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::PredictCorrectSimple::IntegrateUntil
"void IntegrateUntil(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::PredictCorrectSimple::IntegrateUntil
"void IntegrateUntil(cmf::math::Time MaxTime, cmf::math::Time
TimeStep)

Integrates the vector of state variables until MaxTime. ";

/*  The state variables to integrate  */

/*  Accuracy parameters  */

%feature("docstring")
cmf::math::PredictCorrectSimple::AddStatesFromOwner "void
AddStatesFromOwner(cmf::math::StateVariableOwner &stateOwner)

Adds the state variables of a StateVariableOwner to the state
variables of the solver. ";

%feature("docstring")
cmf::math::PredictCorrectSimple::PredictCorrectSimple "PredictCorrectSimple(const StateVariableVector &states, real Alpha)

Constructs a new PredictCorrectSimple from a pointer to a vector of
state variables.

The Integrator becomes the owner of states

Parameters:
-----------

states:  Statevariables of the system

Alpha:  Weight factor (see Formula in C++ documentation)  to weight
(see Formula in C++ documentation)  and (see Formula in C++
documentation)  ";

%feature("docstring")
cmf::math::PredictCorrectSimple::PredictCorrectSimple "PredictCorrectSimple(StateVariableOwner &states, real Alpha)

Constructs a new PredictCorrectSimple from a pointer to a vector of
state variables.

The Integrator becomes the owner of states

Parameters:
-----------

states:  Statevariable owner of the system

Alpha:  Weight factor (see Formula in C++ documentation)  to weight
(see Formula in C++ documentation)  and (see Formula in C++
documentation)  ";

%feature("docstring")
cmf::math::PredictCorrectSimple::PredictCorrectSimple "PredictCorrectSimple(real Alpha=0.5)

Constructs a new PredictCorrectSimple.

Parameters:
-----------

Alpha:  Weight factor (see Formula in C++ documentation)  to weight
(see Formula in C++ documentation)  and (see Formula in C++
documentation)  ";

%feature("docstring")
cmf::math::PredictCorrectSimple::PredictCorrectSimple "PredictCorrectSimple(const Integrator &copy)

Copy constructor. ";

%feature("docstring")  cmf::math::PredictCorrectSimple::Copy "virtual
Integrator* Copy() const

Returns a new Integrator, based on this (without the state variables),
e.g. same type, epsilon, model time etc. ";

%feature("docstring")  cmf::math::PredictCorrectSimple::Integrate "int Integrate(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables.

Parameters:
-----------

MaxTime:  (ignored) To stop the model (if running in a model
framework) at time steps of value exchange e.g. full hours, the next
value exchange time can be given

TimeStep:  Takes the proposed time step ";

%feature("docstring")  cmf::math::PredictCorrectSimple::count "int
count() const

returns the number of state variables ";

%feature("docstring")  cmf::math::PredictCorrectSimple::state "real
state(int position) const

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::PredictCorrectSimple::state "void
state(int position, real newState)

Simplifies the assessment of state variables. ";


// File: a00577.xml
%feature("docstring") std::priority_queue "

STL class. ";


// File: a00046.xml
%feature("docstring") cmf::river::Prism "

Returns the height of a volume in a Prism with a defined base area.

C++ includes: ReachType.h ";

%feature("docstring")  cmf::river::Prism::Prism "Prism(double
base_area) ";

%feature("docstring")  cmf::river::Prism::copy "Prism* copy() const
";

%feature("docstring")  cmf::river::Prism::h "double h(double V) const

(see Formula in C++ documentation)  ";

%feature("docstring")  cmf::river::Prism::A "double A(double V) const

(see Formula in C++ documentation)  ";

%feature("docstring")  cmf::river::Prism::V "double V(double h) const
";


// File: a00047.xml
%feature("docstring") cmf::project "

The study area, holding all cells and outlets and streams.

C++ includes: project.h ";

%feature("docstring")  cmf::project::get_cells "const
upslope::cell_vector& get_cells() const ";

%feature("docstring")  cmf::project::get_cell "upslope::Cell&
get_cell(int index) ";

%feature("docstring")  cmf::project::size "int size() const ";

%feature("docstring")  cmf::project::project "project(std::string
solute_names=\"\")

Creates a new project. ";

%feature("docstring")  cmf::project::~project "~project() ";

%feature("docstring")  cmf::project::NewCell "cmf::upslope::Cell*
NewCell(double x, double y, double z, double Area)

Creates a new cell. ";

%feature("docstring")  cmf::project::NewCell "cmf::upslope::Cell*
NewCell(cmf::geometry::point p, double Area)

Creates a new cell. ";

%feature("docstring")  cmf::project::get_reach "cmf::river::Reach_ptr
get_reach(int index) ";

%feature("docstring")  cmf::project::reach_count "int reach_count()
const ";

%feature("docstring")  cmf::project::get_storages "cmf::water::node_list get_storages() ";

%feature("docstring")  cmf::project::NewReach "cmf::river::Reach_ptr
NewReach(cmf::river::Channel shape, bool diffusive=false)

Creates a new reach. ";


// File: a00578.xml
%feature("docstring") std::queue "

STL class. ";


// File: a00048.xml
%feature("docstring") cmf::atmosphere::RainCloud "

RainCloud is a specialization of NeumannBoundary . The only difference
is a reference to the owning cell and the unit conversion of the flux
timeseries from mm/day to m3/day.

C++ includes: Precipitation.h ";

%feature("docstring")  cmf::atmosphere::RainCloud::get_cell "const
cmf::upslope::Cell& get_cell() const ";

%feature("docstring")  cmf::atmosphere::RainCloud::conc "real
conc(cmf::math::Time t, const cmf::water::solute &solute) const

Returns the solute concentrations of the flux at a given time. ";

%feature("docstring")  cmf::atmosphere::RainCloud::is_empty "bool
is_empty() const ";

%feature("docstring")  cmf::atmosphere::RainCloud::RecalcFluxes "bool
RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node. WaterStorage overrides this, since
state changes require an update of the fluxes. ";

%feature("docstring")  cmf::atmosphere::RainCloud::connect_to "void
connect_to(cmf::water::flux_node::ptr target) ";

%feature("docstring")  cmf::atmosphere::RainCloud::get_position "cmf::geometry::point get_position() const

Pure virtual function. Should return the position of the locatable. ";

%feature("docstring")  cmf::atmosphere::RainCloud::set_position "virtual void set_position(cmf::geometry::point p)

Sets the position of the node in space. ";

%feature("docstring")  cmf::atmosphere::RainCloud::project "const
cmf::project& project() const

Returns the project, this node is part of. ";

%feature("docstring")  cmf::atmosphere::RainCloud::is_storage "virtual bool is_storage() const

true, if this is a waterstorage ";

%feature("docstring")  cmf::atmosphere::RainCloud::to_string "virtual
std::string to_string() const ";

%feature("docstring")  cmf::atmosphere::RainCloud::get_connections "cmf::water::connection_vector get_connections() const ";

%feature("docstring")  cmf::atmosphere::RainCloud::get_connection "cmf::water::flux_connection* get_connection(const
cmf::water::flux_node &target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::atmosphere::RainCloud::remove_connection "bool remove_connection(cmf::water::flux_node::ptr To)

Remove the connection. ";

%feature("docstring")  cmf::atmosphere::RainCloud::flux_to "real
flux_to(const cmf::water::flux_node &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from target into this\"). ";

%feature("docstring")  cmf::atmosphere::RainCloud::flux3d_to "cmf::geometry::point flux3d_to(const cmf::water::flux_node &target,
cmf::math::Time t) ";

%feature("docstring")  cmf::atmosphere::RainCloud::get_3d_flux "cmf::geometry::point get_3d_flux(cmf::math::Time t) ";

%feature("docstring")  cmf::atmosphere::RainCloud::water_balance "real water_balance(cmf::math::Time t, const flux_connection
*Without=0) const

Returns the sum of all fluxes (positive and negative) at time t.
Single fluxes can be excluded from the calculation.

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the water_balance
(e.g. to prevent closed circuits) ";

%feature("docstring")  cmf::atmosphere::RainCloud::get_potential "virtual real get_potential() const

Returns the water potential of the node in m waterhead The base class
water storage always returns the height of the location. ";

%feature("docstring")  cmf::atmosphere::RainCloud::set_potential "virtual void set_potential(real new_potential) ";

%feature("docstring")  cmf::atmosphere::RainCloud::get_distance_to "double get_distance_to(const Locatable &cmp)

Returns the distance between two locatable objects. ";

%feature("docstring")  cmf::atmosphere::RainCloud::get_direction_to "cmf::geometry::point get_direction_to(const Locatable &cmp)

Returns a vector with length=1 pointing in the direction of another
Locatable. ";


// File: a00543.xml
%feature("docstring") cmf::upslope::connections::Rainfall "";

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

%feature("docstring")  cmf::upslope::connections::Rainfall::conc "real conc(cmf::math::Time t, const cmf::water::solute &solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";

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


// File: a00579.xml
%feature("docstring") std::range_error "

STL class. ";


// File: a00049.xml
%feature("docstring") cmf::river::Reach "

A reach is a specialization of an open water storage.

The OpenWaterStorage attributes and methods are extended by
topological features, for the creation of a network of reaches.

C++ includes: reach.h ";

/*  Overrides of flux_node  */

%feature("docstring")  cmf::river::Reach::RecalcFluxes "virtual bool
RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node. WaterStorage overrides this, since
state changes require an update of the fluxes. ";

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
set_downstream(Reach_ptr new_downstream)

Connects the reach to another one downstream. ";

%feature("docstring")  cmf::river::Reach::get_downstream "Reach_ptr
get_downstream() const

Returns the reach downstream of this (or null if there is no reach
downstream). ";

%feature("docstring")  cmf::river::Reach::upstream_count "int
upstream_count() const

Returns the number of reaches upstream of this. ";

%feature("docstring")  cmf::river::Reach::get_upstream "Reach_ptr
get_upstream(int index) const

Returns a reach upstream of this. ";

%feature("docstring")  cmf::river::Reach::get_cells "cmf::upslope::cell_vector get_cells() const

Returns the cells connected with this. ";

%feature("docstring")  cmf::river::Reach::add_cell "void
add_cell(cmf::upslope::Cell *cell, bool soil_cut, real distance, real
width)

Connects the surfacewater of a cell with this reach.

Parameters:
-----------

cell:  The cell with the surface water to be connected with this reach

soil_cut:  True, if this reach might have inflow through the bank

distance:  The average distance from cell to this reach

width:  The flow width from cell to this reach ";

%feature("docstring")  cmf::river::Reach::cuts_soil_of "bool
cuts_soil_of(cmf::upslope::Cell *cell) const

True if bank flow is possible. ";

%feature("docstring")  cmf::river::Reach::get_distance_to "real
get_distance_to(cmf::upslope::Cell *cell) const

The average distance from cell to this reach. ";

%feature("docstring")  cmf::river::Reach::get_bank_length_with "real
get_bank_length_with(cmf::upslope::Cell *cell) const

The bank length in the cell. ";

%feature("docstring")  cmf::river::Reach::get_diffusive "bool
get_diffusive() const ";

%feature("docstring")  cmf::river::Reach::set_diffusive "void
set_diffusive(bool use_diffusive_wave)

Sets all Manning kind connections to either diffusive or kinematic. ";

%feature("docstring")  cmf::river::Reach::get_root "Reach_ptr
get_root()

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

Returns the gravitational potential (see Formula in C++ documentation)
. ";

%feature("docstring")  cmf::river::Reach::conc "real
conc(cmf::math::Time t, const cmf::water::solute &solute) const

Returns the current WaterQuality (concentration of all solutes). ";

%feature("docstring")  cmf::river::Reach::conc "real conc(const
cmf::water::solute &solute) const

Returns the concentration of the given solute. ";

%feature("docstring")  cmf::river::Reach::conc "void conc(const
cmf::water::solute &solute, real NewConcetration)

Sets a new concentration. ";

%feature("docstring")  cmf::river::Reach::is_storage "virtual bool
is_storage() const

true, if this is a waterstorage ";

%feature("docstring")  cmf::river::Reach::AddStateVariables "void
AddStateVariables(cmf::math::StateVariableVector &vector)

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";

%feature("docstring")  cmf::river::Reach::Solute "SoluteStorage&
Solute(const cmf::water::solute &solute)

Returns the water quality of the water storage. ";

%feature("docstring")  cmf::river::Reach::Solute "const
SoluteStorage& Solute(const cmf::water::solute &solute) const ";

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

%feature("docstring")  cmf::river::Reach::get_position "cmf::geometry::point get_position() const

Pure virtual function. Should return the position of the locatable. ";

%feature("docstring")  cmf::river::Reach::set_position "virtual void
set_position(cmf::geometry::point p)

Sets the position of the node in space. ";

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
Single fluxes can be excluded from the calculation.

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the water_balance
(e.g. to prevent closed circuits) ";

%feature("docstring")  cmf::river::Reach::set_potential "virtual void
set_potential(real new_potential) ";

%feature("docstring")  cmf::river::Reach::get_distance_to "double
get_distance_to(const Locatable &cmp)

Returns the distance between two locatable objects. ";

%feature("docstring")  cmf::river::Reach::get_direction_to "cmf::geometry::point get_direction_to(const Locatable &cmp)

Returns a vector with length=1 pointing in the direction of another
Locatable. ";


// File: a00050.xml
%feature("docstring") cmf::river::ReachIterator "

An iterator over every upstream reach from a start reach. Implements
both the Python and the C++ iterator interface Usage C++:.

Usage Python:

C++ includes: reach.h ";

%feature("docstring")  cmf::river::ReachIterator::next "Reach_ptr
next()

Returns the next reach in the upstream queue. ";

%feature("docstring")  cmf::river::ReachIterator::valid "bool valid()
const

Returns true, if reaches are left to iterate over. ";

%feature("docstring")  cmf::river::ReachIterator::ReachIterator "ReachIterator(Reach_ptr first)

Creates a ReachIterator from a first reach. ";

%feature("docstring")  cmf::river::ReachIterator::reach "Reach_ptr
reach() const

Returns the current reach. ";

%feature("docstring")  cmf::river::ReachIterator::position "double
position() const

Returns the distance to the root reach. ";


// File: a00051.xml
%feature("docstring") cmf::river::RectangularReach "

Describes a IChannel with a rectangular crosssection.

C++ includes: ReachType.h ";

%feature("docstring")  cmf::river::RectangularReach::typecode "char
typecode() const ";

%feature("docstring")  cmf::river::RectangularReach::get_channel_width
"virtual double get_channel_width(double depth) const

Returns the width of the stream at a given depth (see Formula in C++
documentation) . ";

%feature("docstring")
cmf::river::RectangularReach::get_wetted_perimeter "virtual double
get_wetted_perimeter(double depth) const

Returns the wetted perimeter at a given depth (see Formula in C++
documentation) . ";

%feature("docstring")  cmf::river::RectangularReach::get_depth "virtual double get_depth(double area) const

Returns the depth at a given crossection area (see Formula in C++
documentation) . ";

%feature("docstring")
cmf::river::RectangularReach::get_flux_crossection "virtual double
get_flux_crossection(double depth) const

Returns the crossection area at a given depth (see Formula in C++
documentation) . ";

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

Calculates the flow rate from a given water volume in the reach (see
Formula in C++ documentation) .

Flow rate [m3/s]

Parameters:
-----------

A:  The area of the cross section [m2]

slope:  The slope of the reach [m/m] ";


// File: a00580.xml


// File: a00052.xml
%feature("docstring") cmf::upslope::RetentionCurve "

Abstract base class for different types of retention curves.

This class, and its children uses wetness instead of volumetric water
content. The wetness of a soil is defined as water content per void
volume

C++ includes: RetentionCurve.h ";

%feature("docstring")  cmf::upslope::RetentionCurve::K "virtual real
K(real wetness, real depth) const =0

Returns the conductivity in m/day at a certain depth and water
content. ";

%feature("docstring")  cmf::upslope::RetentionCurve::Wetness_eff "virtual real Wetness_eff(real wetness, real pF_r=4.2) const

Returns the effective wetness, using a residual pF value (see Formula
in C++ documentation) . ";

%feature("docstring")  cmf::upslope::RetentionCurve::Porosity "virtual real Porosity(real depth) const =0

Returns the porosity at a certain depth. ";

%feature("docstring")  cmf::upslope::RetentionCurve::VoidVolume "virtual real VoidVolume(real upperDepth, real lowerDepth, real Area)
const

Returns the void volume of a soil column. ";

%feature("docstring")  cmf::upslope::RetentionCurve::FillHeight "virtual real FillHeight(real lowerDepth, real Area, real Volume) const

Returns the thickness of a soil column with a certain pore volume. ";

%feature("docstring")  cmf::upslope::RetentionCurve::Transmissivity "virtual real Transmissivity(real upperDepth, real lowerDepth, real
wetness) const

Returns the transmissivity of a part of a soil column. ";

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


// File: a00508.xml
%feature("docstring") std::basic_string::reverse_iterator "

STL iterator class. ";


// File: a00581.xml
%feature("docstring") std::wstring::reverse_iterator "

STL iterator class. ";


// File: a00564.xml
%feature("docstring") std::list::reverse_iterator "

STL iterator class. ";


// File: a00570.xml
%feature("docstring") std::map::reverse_iterator "

STL iterator class. ";


// File: a00572.xml
%feature("docstring") std::multimap::reverse_iterator "

STL iterator class. ";


// File: a00582.xml
%feature("docstring") std::string::reverse_iterator "

STL iterator class. ";


// File: a00574.xml
%feature("docstring") std::multiset::reverse_iterator "

STL iterator class. ";


// File: a00583.xml
%feature("docstring") std::set::reverse_iterator "

STL iterator class. ";


// File: a00536.xml
%feature("docstring") std::deque::reverse_iterator "

STL iterator class. ";


// File: a00584.xml
%feature("docstring") std::vector::reverse_iterator "

STL iterator class. ";


// File: a00053.xml
%feature("docstring") cmf::upslope::connections::Richards "

Calculates flow according to the Richards equation.

(see Formula in C++ documentation)  where  (see Formula in C++
documentation)  is the difference of the total water potentials of the
two soil layers

(see Formula in C++ documentation)  is the distance between the two
soil layers

(see Formula in C++ documentation)  is the geometric mean conductivity
(see SoilType::Kunsat)

(see Formula in C++ documentation)  is the crosssectional area of the
flux

(see Formula in C++ documentation)  is the matrix potential (see
SoilType::MatrixPotential)

(see Formula in C++ documentation)  is the height of a soil layer
above sea level

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

%feature("docstring")  cmf::upslope::connections::Richards::conc "real conc(cmf::math::Time t, const cmf::water::solute &solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";

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


// File: a00561.xml
%feature("docstring") cmf::upslope::connections::Richards_lateral "";

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
conc(cmf::math::Time t, const cmf::water::solute &solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";

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


// File: a00054.xml
%feature("docstring") cmf::math::RKFIntegrator "

Integrates a vector of cmf::math::StateVariable with the Runge-Kutta-
Fehlberg (RKF54) method.

C++ includes: RKFIntegrator.h ";

/*  Assessment of state variables for integration  */

%feature("docstring")  cmf::math::RKFIntegrator::AddState "virtual
void AddState(cmf::math::StateVariable &statevar) ";

/*  model time  */

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

/*  Integrate  */

%feature("docstring")  cmf::math::RKFIntegrator::Integrate "int
Integrate(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::RKFIntegrator::IntegrateUntil "void
IntegrateUntil(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::RKFIntegrator::IntegrateUntil "void
IntegrateUntil(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables until MaxTime. ";

/*  The state variables to integrate  */

/*  Accuracy parameters  */

%feature("docstring")  cmf::math::RKFIntegrator::AddStatesFromOwner "void AddStatesFromOwner(cmf::math::StateVariableOwner &stateOwner)

Adds the state variables of a StateVariableOwner to the state
variables of the solver. ";

%feature("docstring")  cmf::math::RKFIntegrator::RKFIntegrator "RKFIntegrator(StateVariableOwner &states, real epsilon=1e-9,
cmf::math::Time tStepMin=Time::Seconds(10))

Constructs a new RKFIntegrator from a pointer to a vector of state
variables.

The RKF Integrator becomes the owner of states

Parameters:
-----------

states:  Statevariables of the system

epsilon:  relative error tolerance per time step (default=1e-9)

tStepMin:  minimum time step (default=10s) ";

%feature("docstring")  cmf::math::RKFIntegrator::RKFIntegrator "RKFIntegrator(real epsilon=1e-9, cmf::math::Time
tStepMin=Time::Seconds(10))

Constructs a new RKFIntegrator.

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

Integrates the vector of state variables.

Parameters:
-----------

MaxTime:  To stop the model (if running in a model framework) at time
steps of value exchange e.g. full hours, the next value exchange time
can be given

TimeStep:  Takes the proposed timestep, and changes it into the
effictivly used timestep according to the local stiffness of the
problem and MaxTime ";

%feature("docstring")  cmf::math::RKFIntegrator::count "int count()
const

returns the number of state variables ";

%feature("docstring")  cmf::math::RKFIntegrator::state "real
state(int position) const

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::RKFIntegrator::state "void
state(int position, real newState)

Simplifies the assessment of state variables. ";


// File: a00542.xml
%feature("docstring") std::runtime_error "

STL class. ";


// File: a00586.xml
%feature("docstring") std::set "

STL class. ";


// File: a00055.xml
%feature("docstring") cmf::upslope::vegetation::ShuttleworthWallace "

Calculates the sum of soil evaporation and transpiration according to
Shuttleworth & Wallace 1985, as implemented in BROOK 90 (Federer
1990).

The difference to BROOK90 is, that the actual transpiration is not
calculated by plant resitance and potential gradient between plant and
soil, but by an piecewise linear function of the pF value (see Formula
in C++ documentation) : (see Formula in C++ documentation)

Calculation procedure, as in BROOK 90:

Evapotranspiration from the canopy: (see Formula in C++ documentation)

Evaporation from the ground: (see Formula in C++ documentation)

with  (see Formula in C++ documentation) , the slope of the sat. vap.
press. T function

(see Formula in C++ documentation) , the net radiation flux in the
ground

(see Formula in C++ documentation) , the net radiation flux in the
canopy

(see Formula in C++ documentation)  constants lambda, c_p_rho, gamma,
C_R

(see Formula in C++ documentation)  vapor pressure deficit at
effective source height, see function D0

(see Formula in C++ documentation)  Resistances for the vapor pressure
(see below)

C++ includes: ShuttleworthWallace.h ";

/*  Constants  */

/*  Private functions  */

%feature("docstring")
cmf::upslope::vegetation::ShuttleworthWallace::ET "real ET()

Actual evapotranspiration in mm/day. ";

%feature("docstring")
cmf::upslope::vegetation::ShuttleworthWallace::ShuttleworthWallace "ShuttleworthWallace(cmf::atmosphere::Weather w, real
soilwater_matrixpotential, const cmf::upslope::vegetation::Vegetation
&veg, bool CanopyStoresWater=false, real
measurement_height_above_canopy=2)

Calculates the transpiration and the soil evaporation from dry
surfaces.

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


// File: a00056.xml
%feature("docstring") cmf::upslope::ET::ShuttleworthWallaceET "

Calculates the actual transpiration and the soil evaporation from a
soil layer.

C++ includes: ET.h ";

%feature("docstring")
cmf::upslope::ET::ShuttleworthWallaceET::ShuttleworthWallaceET "ShuttleworthWallaceET(cmf::water::WaterStorage::ptr source,
cmf::water::flux_node::ptr ET_target, cmf::upslope::Cell &cell,
std::string Type=\"Shuttleworth Wallace get_evaporation\") ";

%feature("docstring")  cmf::upslope::ET::ShuttleworthWallaceET::q "real q(const flux_node &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::ET::ShuttleworthWallaceET::left_node "flux_node::ptr
left_node() const

Returns the left node of this connection. ";

%feature("docstring")
cmf::upslope::ET::ShuttleworthWallaceET::right_node "flux_node::ptr
right_node() const

returns the right node of this connection ";

%feature("docstring")
cmf::upslope::ET::ShuttleworthWallaceET::kill_me "bool kill_me()

Deregisters this connection from its nodes. Returns true if only one
reference is left. ";

%feature("docstring")
cmf::upslope::ET::ShuttleworthWallaceET::get_target "flux_node::ptr
get_target(const flux_node &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::ET::ShuttleworthWallaceET::get_target "flux_node::ptr
get_target(int index) const

With index 0, the left node is returned, with index 1 the right node
of the connection. ";

%feature("docstring")
cmf::upslope::ET::ShuttleworthWallaceET::exchange_target "void
exchange_target(flux_node::ptr oldtarget, flux_node::ptr newTarget) ";

%feature("docstring")  cmf::upslope::ET::ShuttleworthWallaceET::conc "real conc(cmf::math::Time t, const cmf::water::solute &solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";

%feature("docstring")
cmf::upslope::ET::ShuttleworthWallaceET::get_tracer_filter "real
get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::ET::ShuttleworthWallaceET::set_tracer_filter "void
set_tracer_filter(real value) ";

%feature("docstring")
cmf::upslope::ET::ShuttleworthWallaceET::to_string "std::string
to_string() const ";

%feature("docstring")
cmf::upslope::ET::ShuttleworthWallaceET::short_string "std::string
short_string() const ";


// File: a00544.xml
%feature("docstring") cmf::upslope::connections::SimpleTindexSnowMelt
"";

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
conc(cmf::math::Time t, const cmf::water::solute &solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";

%feature("docstring")
cmf::upslope::connections::SimpleTindexSnowMelt::get_tracer_filter "real get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")
cmf::upslope::connections::SimpleTindexSnowMelt::set_tracer_filter "void set_tracer_filter(real value) ";

%feature("docstring")
cmf::upslope::connections::SimpleTindexSnowMelt::to_string "std::string to_string() const ";

%feature("docstring")
cmf::upslope::connections::SimpleTindexSnowMelt::short_string "std::string short_string() const ";


// File: a00545.xml
%feature("docstring") cmf::upslope::connections::Snowfall "";

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

%feature("docstring")  cmf::upslope::connections::Snowfall::conc "real conc(cmf::math::Time t, const cmf::water::solute &solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";

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


// File: a00057.xml
%feature("docstring") cmf::upslope::SoilLayer "

A representation of a SoilLayer.

C++ includes: SoilLayer.h ";

/*  Overrides of flux_node  */

%feature("docstring")  cmf::upslope::SoilLayer::RecalcFluxes "virtual
bool RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node. WaterStorage overrides this, since
state changes require an update of the fluxes. ";

%feature("docstring")  cmf::upslope::SoilLayer::is_empty "virtual
bool is_empty() const ";

%feature("docstring")  cmf::upslope::SoilLayer::get_upper_boundary "virtual real get_upper_boundary() const

Returns the upper boundary of the water storage below ground in m. ";

%feature("docstring")  cmf::upslope::SoilLayer::get_lower_boundary "virtual real get_lower_boundary() const

Returns the lower boundary of the water storage below ground in m. ";

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

%feature("docstring")  cmf::upslope::SoilLayer::get_K "real get_K()
const

Returns the actual conductivity (see Formula in C++ documentation) .
";

%feature("docstring")  cmf::upslope::SoilLayer::get_Ksat "real
get_Ksat() const ";

%feature("docstring")  cmf::upslope::SoilLayer::get_wetness "virtual
real get_wetness() const

Returns the wetness of the soil (see Formula in C++ documentation) .
";

%feature("docstring")  cmf::upslope::SoilLayer::set_wetness "virtual
void set_wetness(real wetness) ";

%feature("docstring")  cmf::upslope::SoilLayer::get_matrix_potential "virtual real get_matrix_potential() const

Calls RetentionCurve::Matrixpotential. ";

%feature("docstring")
cmf::upslope::SoilLayer::get_gravitational_potential "real
get_gravitational_potential() const

Gravitational get_potential in m, reference height is sea level. If
the layer is saturated, it returns the saturated depth above sea
level, otherwise it returns the upperboundary of the layer (see
Formula in C++ documentation) . ";

%feature("docstring")  cmf::upslope::SoilLayer::get_porosity "real
get_porosity() const

Returns the mean porosity in the layer. ";

%feature("docstring")  cmf::upslope::SoilLayer::get_capacity "virtual
real get_capacity() const

Returns the capacity of the water storage in m3. ";

%feature("docstring")  cmf::upslope::SoilLayer::set_potential "virtual void set_potential(real waterhead)

Sets the potential of this soil water storage. ";

%feature("docstring")  cmf::upslope::SoilLayer::get_potential "virtual real get_potential() const

Returns the total potential in m (see Formula in C++ documentation) .
";

%feature("docstring")  cmf::upslope::SoilLayer::get_saturated_depth "virtual real get_saturated_depth() const

Returns the depth for saturation (see Formula in C++ documentation) .
";

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

%feature("docstring")  cmf::upslope::SoilLayer::is_storage "virtual
bool is_storage() const

true, if this is a waterstorage ";

%feature("docstring")  cmf::upslope::SoilLayer::AddStateVariables "void AddStateVariables(cmf::math::StateVariableVector &vector)

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";

%feature("docstring")  cmf::upslope::SoilLayer::Solute "SoluteStorage& Solute(const cmf::water::solute &solute)

Returns the water quality of the water storage. ";

%feature("docstring")  cmf::upslope::SoilLayer::Solute "const
SoluteStorage& Solute(const cmf::water::solute &solute) const ";

%feature("docstring")  cmf::upslope::SoilLayer::conc "real conc(const
cmf::water::solute &solute) const

Returns the concentration of the given solute. ";

%feature("docstring")  cmf::upslope::SoilLayer::conc "real
conc(cmf::math::Time t, const cmf::water::solute &solute) const

Returns the current WaterQuality (concentration of all solutes). ";

%feature("docstring")  cmf::upslope::SoilLayer::conc "void conc(const
cmf::water::solute &solute, real NewConcetration)

Sets a new concentration. ";

%feature("docstring")  cmf::upslope::SoilLayer::get_volume "virtual
real get_volume() const

Returns the volume of water in this storage in m3. ";

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

%feature("docstring")  cmf::upslope::SoilLayer::get_position "cmf::geometry::point get_position() const

Pure virtual function. Should return the position of the locatable. ";

%feature("docstring")  cmf::upslope::SoilLayer::set_position "virtual
void set_position(cmf::geometry::point p)

Sets the position of the node in space. ";

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
Single fluxes can be excluded from the calculation.

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the water_balance
(e.g. to prevent closed circuits) ";

%feature("docstring")  cmf::upslope::SoilLayer::get_distance_to "double get_distance_to(const Locatable &cmp)

Returns the distance between two locatable objects. ";

%feature("docstring")  cmf::upslope::SoilLayer::get_direction_to "cmf::geometry::point get_direction_to(const Locatable &cmp)

Returns a vector with length=1 pointing in the direction of another
Locatable. ";


// File: a00058.xml
%feature("docstring") cmf::water::solute "

A structure to identify a solute.

C++ includes: Solute.h ";

%feature("docstring")  cmf::water::solute::solute "solute(const
solute &copy) ";


// File: a00059.xml
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


// File: a00060.xml
%feature("docstring") cmf::water::SoluteStorage "

A class for the storage of any tracer. The state is the amount (mol,
kg etc. see cmf::water) of the tracer in the storage.

The derivative function is given by: (see Formula in C++
documentation)

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


// File: a00061.xml
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
conc(cmf::math::Time t, const cmf::water::solute &solute) const ";

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


// File: a00550.xml
%feature("docstring") cmf::math::SoluteWaterIntegrator "";

/*  The state variables to integrate  */

/*  Accuracy parameters  */

/*  model time  */

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

/*  Assessment of state variables for integration  */

%feature("docstring")  cmf::math::SoluteWaterIntegrator::AddState "virtual void AddState(cmf::math::StateVariable &statevar) ";

/*  Integrate  */

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

%feature("docstring")  cmf::math::SoluteWaterIntegrator::Integrate "int Integrate(cmf::math::Time MaxTime, cmf::math::Time TimeStep) ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::Copy "virtual cmf::math::SoluteWaterIntegrator* Copy() const ";

%feature("docstring")
cmf::math::SoluteWaterIntegrator::SoluteWaterIntegrator "SoluteWaterIntegrator(const cmf::math::Integrator &water_integrator,
const cmf::math::Integrator &solute_integrator) ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::Reset "virtual void Reset() ";

%feature("docstring")
cmf::math::SoluteWaterIntegrator::SoluteWaterIntegrator "SoluteWaterIntegrator(const cmf::math::Integrator &water_integrator,
const cmf::math::Integrator &solute_integrator,
cmf::math::StateVariableOwner &states) ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::count "int
count() const

returns the number of state variables ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::state "real
state(int position) const

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::SoluteWaterIntegrator::state "void
state(int position, real newState)

Simplifies the assessment of state variables. ";


// File: a00588.xml
%feature("docstring") std::stack "

STL class. ";


// File: a00062.xml
%feature("docstring") cmf::math::StateVariable "

Abstract class state variable.

Abstract class state variable

Simple exponential system class header implementing a state variable:

C++ includes: StateVariable.h ";

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


// File: a00063.xml
%feature("docstring") cmf::math::StateVariableOwner "

An abstract class, that owns one or more state variables, that can add
them to a vector of state variables in a certain order.

C++ includes: StateVariable.h ";

%feature("docstring")
cmf::math::StateVariableOwner::AddStateVariables "virtual void
AddStateVariables(cmf::math::StateVariableVector &vector)=0

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";

%feature("docstring")
cmf::math::StateVariableOwner::StateVariableOwner "StateVariableOwner() ";


// File: a00589.xml
%feature("docstring") cmf::math::StateVariableVector "";

%feature("docstring")  cmf::math::StateVariableVector::CopyStates "void CopyStates(num_array &destination) const

Copies the states to a numeric vector using use_OpenMP. ";

%feature("docstring")  cmf::math::StateVariableVector::CopyStates "void CopyStates(real *destination) const ";

%feature("docstring")  cmf::math::StateVariableVector::SetStates "void SetStates(const num_array &newStates)

Copies the new states to the actual states. ";

%feature("docstring")  cmf::math::StateVariableVector::SetStates "void SetStates(real *newStates) ";

%feature("docstring")  cmf::math::StateVariableVector::CopyDerivs "void CopyDerivs(Time time, num_array &destination, real factor=1)
const

Copies the derivatives at time step \"time\" to a numeric vector using
use_OpenMP.

Parameters:
-----------

time:   Time at which the derivatives should be calculated

destination:  Vector to be overwritten by the results

factor:  A factor that is multiplied to the derivate (e.g. unit
conversion or integration length) ";

%feature("docstring")  cmf::math::StateVariableVector::CopyDerivs "void CopyDerivs(Time time, real *destination, real factor=1) const

Copies the derivatives at time step \"time\" to an preallocated c
array.

Parameters:
-----------

time:   Time at which the derivatives should be calculated

destination:  Allocated c array

factor:  A factor that is multiplied to the derivate (e.g. unit
conversion or integration length) ";

%feature("docstring")  cmf::math::StateVariableVector::GetStates "num_array GetStates() const

Returns the states in a numeric vector using :CopyStates, but is
slower because of additional memory allocation. ";

%feature("docstring")  cmf::math::StateVariableVector::GetDerivs "num_array GetDerivs(Time time) const

Returns the derivatives at time step \"time\" in a numeric vector
using :CopyDerivs, but is slower because of additional memory
allocation. ";

%feature("docstring")
cmf::math::StateVariableVector::StateVariableVector "StateVariableVector() ";


// File: a00503.xml
%feature("docstring") std::string "

STL class. ";


// File: a00509.xml
%feature("docstring") std::stringstream "

STL class. ";


// File: a00064.xml
%feature("docstring") cmf::upslope::connections::SWATPercolation "

A tipping bucket percolation approach similar to the approach in SWAT.

(see Formula in C++ documentation)

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
conc(cmf::math::Time t, const cmf::water::solute &solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";

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


// File: a00065.xml
%feature("docstring") cmf::river::SWATReachType "

Structure for the description of structural parameters of a reach.

Uses the SWAT IChannel geometry (see SWAT Theoretical Documentation,
Version 2005 (ch. 7:1.1), Neitsch et al. 2005), in this class
referenced as SWATtheoDoc. Differences to the SWAT geometry: The flood
plain is not plain, but has a small slope=0.5%, but has an infinite
width

C++ includes: ReachType.h ";

%feature("docstring")  cmf::river::SWATReachType::typecode "char
typecode() const ";

%feature("docstring")  cmf::river::SWATReachType::get_channel_width "virtual double get_channel_width(double depth) const

Calculates the flow width from a given actual depth [m] using the
actual IChannel geometry (see Formula in C++ documentation) .

See:  SWATtheoDoc eq. 7:1.1.3 ";

%feature("docstring")  cmf::river::SWATReachType::get_wetted_perimeter
"virtual double get_wetted_perimeter(double depth) const

Calculates the wetted perimeter from a given actual depth [m] using
the actual IChannel geometry (see Formula in C++ documentation) .

See:  SWATtheoDoc eq. 7:1.1.5

Parameters:
-----------

depth:  Actual depth of the reach [m] ";

%feature("docstring")  cmf::river::SWATReachType::get_depth "virtual
double get_depth(double area) const

Calculates the actual depth of the reach using the IChannel geometry
(see Formula in C++ documentation) .

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
stored volume and l is the reach length (see Formula in C++
documentation) .

See:  SWATtheoDoc eq. 7:1.1.4

Wetted area of a river cross section [m2]

Parameters:
-----------

depth:  Depth of the reach [m] ";

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

Calculates the flow rate from a given water volume in the reach (see
Formula in C++ documentation) .

Flow rate [m3/s]

Parameters:
-----------

A:  The area of the cross section [m2]

slope:  The slope of the reach [m/m] ";


// File: a00066.xml
%feature("docstring") cmf::water::TechnicalFlux "

Produces a constant but changeable flux from a source to a target, if
enough water is present in the source.

(see Formula in C++ documentation)

C++ includes: boundary_condition.h ";

%feature("docstring")  cmf::water::TechnicalFlux::TechnicalFlux "TechnicalFlux(std::tr1::shared_ptr< cmf::water::WaterStorage >
&source, std::tr1::shared_ptr< cmf::water::flux_node > target, real
maximum_flux, real minimal_state=0, cmf::math::Time
flux_decrease_time=cmf::math::h)

Produces a constant but changeable flux from a source to a target, if
enough water is present in the source.

Parameters:
-----------

source:  The source of the water

target:  The target of the water

maximum_flux:  The requested flux (see Formula in C++ documentation)

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
conc(cmf::math::Time t, const cmf::water::solute &solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";

%feature("docstring")  cmf::water::TechnicalFlux::get_tracer_filter "real get_tracer_filter()

A value ranging from 0 to 1. ";

%feature("docstring")  cmf::water::TechnicalFlux::set_tracer_filter "void set_tracer_filter(real value) ";

%feature("docstring")  cmf::water::TechnicalFlux::to_string "std::string to_string() const ";

%feature("docstring")  cmf::water::TechnicalFlux::short_string "std::string short_string() const ";


// File: a00067.xml
%feature("docstring") cmf::math::Time "

A time class, used to pass around current modelling times.

Timespans and dates in cmf are used with a special object, called
Time. The advantage of an extra class has the advantage, that the user
does not has to rememember, which unit of time he or she uses or what
time unit is accepted by a specific function of the model. Arithmetic
and boolean operators are supported by Time. Internally the time
classes stores the time as integer milliseconds, therefore rounding
issues will only appear at very small time ranges. Absolute time (like
dates) are represented as milliseconds gone by from Dec, 31st 1899.
Microsoft Excel dates are represented as days from that time, using
floating point numbers, therefore it is very simple to convert Excel
time representations to cmf time.

Another object is Date, which is doesn't provide the operators, but
has a nice printed version and some special date functions, like day
of year (DOY) and provides access to the current hour of day and so
on, which only applyto dates and not to time spans. You can convert
Time to Date an vice versa. The printing is not culture aware and uses
the European representation. If you use the Python standard library
datetime, conversion between Python time and cmf time is
possibleCreating absolute time values (dates)

import CMFlib as cmf                 # Create the time: Jan, 5th 2001,
2:30 pm, 20s, 412 ms                 t=cmf.Time(5,1,2001,14,30,20,412)
print t # Prints: '36896d:14:30:20.412h'                 print
t.AsDate() # Prints: '05.01.2001 14:30:20.412'Creating time spans

In principle, there are three ways to create time spans. One is to use
one of the static functions, another is to multiply an existing time
span (like one of the build in constants) or to substrate two absolute
times.Available static functions, the default value is always 1

Milliseconds(): t=cmf.Time.Milliseconds(4100), create a timespan of
4.1 seconds

Seconds(): t=cmf. Time.Seconds(4.1), create a timespan of 4.1 seconds

Minutes(): t=cmf.Time.Minutes(138), create a timespan of 2.3 hours
(138 min)

Hours(): t=cmf. Time.Hours(2.3), create a timespan of 2.3 hours (138
min)

Days(): t=cmf. Time.Days(2.5), create a timespan of 60 hours

Years(): t=cmf. Time.Years(), create a timespan of 365 days Available
constants

cmf.sec * 4.1: 4.1 seconds

cmf.min * 138: 2.3 hours (138 min)

cmf.h * 2.3: 2.3 hours (138 min)

cmf.day * 2.5: 60 hours (2.5 days)

cmf.week: 7 days

cmf.month: 365/12 days (30.4167 days)

cmf.year: 365 days Available operators:

time + time = time, time - time = time

time * float = time ,time / float = time

time/time=float

&gt, &lt, ==, != Conversions

Converting to python datetime

import CMFlib as cmf                 pythontime = cmf.year.AsPython()
cmftime=cmf.AsCMFTime(pythontime)                 print
type(pythontime) # '<type 'datetime.datetime'>'                 print
type(cmftime)    # '<class 'CMFLib.Time'>'Converting to numbers

t.AsMilliseconds()

t.AsSeconds()

t.AsMinutes()

t.AsHours()

t.AsDays()

t.AsYears() Creating time ranges

import CMFLib as cmf                 start=cmf.Time(5,1,2001)
end=cmf.Time(6,1,2001)                 step=cmf.h * 6 for t in
cmf.timerange(start,end,step):                     print t.AsDate()
# Prints:                 # 05.01.2001 # 05.01.2001 06:00
# 05.01.2001 12:00 # 05.01.2001 18:00

C++ includes: Time.h ";

/*  Constructors  */

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


// File: a00068.xml
%feature("docstring") cmf::math::timeseries "

A timeseries is a list of values, equally distributed over time. To
create one, one have to provide as start date and a step size. The end
time is calculated from the number of values. Values queried for times
before the start time are returned as the first item, values after the
end time equal the last item. A timeseries with only one item reacts
like a scalar value.

Creating a time series

import CMFLib as cmf                 # Start date is the January 5th
2001 at 2:30 pm start=cmf.Time(5,1,2001,14,30)
# time step of the timeseries is 20 minutes
step=cmf.min*20                 # Type of interpolation between values
# 0 - Nearest neighbor,                 # 1 - Linear,
# 2 - Squared, # 3 - Cubic, etc.                 interpolation=1 #
Create timeseries
timeseries=cmf.timeseries(start,step,interpolation) # add data
timeseries.add(0.1) # Value at 2001/5/1 2:30pm is 0.1
timeseries.add(0.2) # Value at 2001/5/1 2:50pm is 0.2
timeseries.add(0.1) # Value at 2001/5/1 3:10pm is 0.1

With this technic it is simple to read files or databases to fill
timeseries.Using a timeseries

# Query every minute between 2:15 and 3:14 pm for t in
cmf.timerange(start,start+cmf.h,cmf.min): print
\"Time:\",t.AsDate(),\"Value:\", timeseries[t]                 # Query
a specific position of the timeseries                 print
timeseries[2]

C++ includes: timeseries.h ";

/*  Operators  */

/* Binary operators defined as free operators:

x = {+,-,*,/}

Defined for (x is one of the operators above):  timeseries =
timeseries x timeseries

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

%feature("docstring")  cmf::math::timeseries::interpolationpower "double interpolationpower() const

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

Constructor of a time series.

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


// File: a00590.xml


// File: a00069.xml
%feature("docstring")
cmf::upslope::connections::TopographicGradientDarcy "

Calculates the lateral flow using the topographic gradient.

(see Formula in C++ documentation)  where  (see Formula in C++
documentation)  is the topographic height difference

(see Formula in C++ documentation)  is the distance from Cell 1 to
Cell 2

(see Formula in C++ documentation)  is the transmissivity of cell C,
calculated by SoilType::Transmissivity

(see Formula in C++ documentation)  is the width of the connection of
the cells

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
conc(cmf::math::Time t, const cmf::water::solute &solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";

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


// File: a00070.xml
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


// File: a00071.xml
%feature("docstring") cmf::river::TriangularReach "

Structure for the description of reaches with a triangular cross
section.

Although double triangular cross section reach are rarely met, a
triangular reach does scale with its water load, and is therefore
preferable in case where nothing about IChannel geometry is known

C++ includes: ReachType.h ";

%feature("docstring")  cmf::river::TriangularReach::typecode "char
typecode() const ";

%feature("docstring")  cmf::river::TriangularReach::get_channel_width
"virtual double get_channel_width(double depth) const

Returns the width of the stream at a given depth (see Formula in C++
documentation) . ";

%feature("docstring")
cmf::river::TriangularReach::get_wetted_perimeter "virtual double
get_wetted_perimeter(double depth) const

Returns the wetted perimeter at a given depth (see Formula in C++
documentation) . ";

%feature("docstring")  cmf::river::TriangularReach::get_depth "virtual double get_depth(double area) const

Returns the depth at a given crossection area (see Formula in C++
documentation) . ";

%feature("docstring")
cmf::river::TriangularReach::get_flux_crossection "virtual double
get_flux_crossection(double depth) const

Returns the crossection area at a given depth (see Formula in C++
documentation) . ";

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

Calculates the flow rate from a given water volume in the reach (see
Formula in C++ documentation) .

Flow rate [m3/s]

Parameters:
-----------

A:  The area of the cross section [m2]

slope:  The slope of the reach [m/m] ";


// File: a00585.xml
%feature("docstring") std::underflow_error "

STL class. ";


// File: a00591.xml
%feature("docstring") std::valarray "

STL class. ";


// File: a00072.xml
%feature("docstring") cmf::upslope::VanGenuchtenMualem "

Provides the use of the Van Genuchten - Mualem retention curve (Van
Genuchten 1980).

Head - moisture realtionship: (see Formula in C++ documentation)
where:  (see Formula in C++ documentation)  is the conductivity in
(see Formula in C++ documentation)

(see Formula in C++ documentation)  is the wetness (Volume of soil
water per volume of pores)

(see Formula in C++ documentation)  is a shape parameter of the
retention curve

(see Formula in C++ documentation)  is inverse of the air entry
potential in (see Formula in C++ documentation)

(see Formula in C++ documentation)  is the matric potential in (see
Formula in C++ documentation)  at wetness W

C++ includes: RetentionCurve.h ";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::Wetness "virtual real Wetness(real suction) const

(see Formula in C++ documentation)  ";

%feature("docstring")
cmf::upslope::VanGenuchtenMualem::MatricPotential "virtual real
MatricPotential(real wetness) const

(see Formula in C++ documentation)  ";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::K "virtual
real K(real wetness, real depth) const

(see Formula in C++ documentation)  ";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::VoidVolume "virtual real VoidVolume(real upperDepth, real lowerDepth, real Area)
const

(see Formula in C++ documentation)  ";

%feature("docstring")
cmf::upslope::VanGenuchtenMualem::Transmissivity "virtual real
Transmissivity(real upperDepth, real lowerDepth, real wetness) const

(see Formula in C++ documentation)  ";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::Porosity "virtual real Porosity(real depth) const

(see Formula in C++ documentation)  ";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::FillHeight "virtual real FillHeight(real lowerDepth, real Area, real Volume) const

Returns the thickness of a soil column with a certain pore volume. ";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::copy "VanGenuchtenMualem* copy() const ";

%feature("docstring")
cmf::upslope::VanGenuchtenMualem::VanGenuchtenMualem "VanGenuchtenMualem() ";

%feature("docstring")
cmf::upslope::VanGenuchtenMualem::VanGenuchtenMualem "VanGenuchtenMualem(real _Ksat, real _phi, real _alpha, real _n, real
_m=-1)

Creates a van Genuchten-Mualem retention curve.

Parameters:
-----------

_Ksat:  Saturated conductivity in (see Formula in C++ documentation)

_phi:  Porosity in (see Formula in C++ documentation)

_alpha:  Van Genuchten (see Formula in C++ documentation)  in (see
Formula in C++ documentation)

_n:  Van Genuchten n

_m:  m parameter, if negative m is calculated as (see Formula in C++
documentation)  ";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::Wetness_eff "virtual real Wetness_eff(real wetness, real pF_r=4.2) const

Returns the effective wetness, using a residual pF value (see Formula
in C++ documentation) . ";

%feature("docstring")  cmf::upslope::VanGenuchtenMualem::Wetness_pF "real Wetness_pF(real pF) const

returns the volumetric water content at a given pF value ";


// File: a00592.xml
%feature("docstring") std::vector "

STL class. ";


// File: a00073.xml
%feature("docstring") cmf::upslope::vegetation::Vegetation "

Holds the vegetation parameters for the calculation of ET and
fractionating rainfall. Not every ET method uses all parameters.

C++ includes: StructVegetation.h ";

%feature("docstring")
cmf::upslope::vegetation::Vegetation::RootFraction "virtual double
RootFraction(double upperBoundary, double lowerBoundary) const ";

%feature("docstring")
cmf::upslope::vegetation::Vegetation::Vegetation "Vegetation(double
_LAI=2.88, double _Height=0.12, double _RootDepth=0.25, double
_StomatalResistance=100, double _albedo=0.23, double _CanopyClosure=1,
double _CanopyCapacityPerLAI=0.1, double _fraction_at_rootdepth=1.0)
";


// File: a00074.xml
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


// File: a00548.xml
%feature("docstring") cmf::water::waterbalance_connection "";

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

%feature("docstring")  cmf::water::waterbalance_connection::conc "real conc(cmf::math::Time t, const cmf::water::solute &solute)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";

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


// File: a00075.xml
%feature("docstring") cmf::water::WaterStorage "

A state variable for the storage of water.

A class for the storage of water. The state is the volume of water
stored in (see Formula in C++ documentation)  The derivative function
is given by: (see Formula in C++ documentation)

C++ includes: WaterStorage.h ";

/*  Overrides of flux_node  */

%feature("docstring")  cmf::water::WaterStorage::RecalcFluxes "virtual bool RecalcFluxes(cmf::math::Time t)

Pure flux_nodes do not influence fluxes, therefore no recalculation of
fluxes is required by flux_node. WaterStorage overrides this, since
state changes require an update of the fluxes. ";

%feature("docstring")  cmf::water::WaterStorage::is_empty "virtual
bool is_empty() const ";

%feature("docstring")  cmf::water::WaterStorage::is_storage "virtual
bool is_storage() const

true, if this is a waterstorage ";

%feature("docstring")  cmf::water::WaterStorage::AddStateVariables "void AddStateVariables(cmf::math::StateVariableVector &vector)

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";

%feature("docstring")  cmf::water::WaterStorage::WaterStorage "WaterStorage(const cmf::project &_project, double InitialState=0)

creates a water storage (abstract class) o

Parameters:
-----------

_project:  The project the waterstorage belongs to

InitialState:  Initial water content in m3 ";

%feature("docstring")  cmf::water::WaterStorage::Solute "SoluteStorage& Solute(const cmf::water::solute &solute)

Returns the water quality of the water storage. ";

%feature("docstring")  cmf::water::WaterStorage::Solute "const
SoluteStorage& Solute(const cmf::water::solute &solute) const ";

%feature("docstring")  cmf::water::WaterStorage::conc "real
conc(const cmf::water::solute &solute) const

Returns the concentration of the given solute. ";

%feature("docstring")  cmf::water::WaterStorage::conc "real
conc(cmf::math::Time t, const cmf::water::solute &solute) const

Returns the current WaterQuality (concentration of all solutes). ";

%feature("docstring")  cmf::water::WaterStorage::conc "void
conc(const cmf::water::solute &solute, real NewConcetration)

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

%feature("docstring")  cmf::water::WaterStorage::get_position "cmf::geometry::point get_position() const

Pure virtual function. Should return the position of the locatable. ";

%feature("docstring")  cmf::water::WaterStorage::set_position "virtual void set_position(cmf::geometry::point p)

Sets the position of the node in space. ";

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
Single fluxes can be excluded from the calculation.

Parameters:
-----------

t:  Time of the query

Without:  A flux_connection that is excluded from the water_balance
(e.g. to prevent closed circuits) ";

%feature("docstring")  cmf::water::WaterStorage::get_potential "virtual real get_potential() const

Returns the water potential of the node in m waterhead The base class
water storage always returns the height of the location. ";

%feature("docstring")  cmf::water::WaterStorage::set_potential "virtual void set_potential(real new_potential) ";

%feature("docstring")  cmf::water::WaterStorage::get_distance_to "double get_distance_to(const Locatable &cmp)

Returns the distance between two locatable objects. ";

%feature("docstring")  cmf::water::WaterStorage::get_direction_to "cmf::geometry::point get_direction_to(const Locatable &cmp)

Returns a vector with length=1 pointing in the direction of another
Locatable. ";


// File: a00076.xml
%feature("docstring") cmf::atmosphere::Weather "

A structure holding meteorological information, excluding
precipitation.

C++ includes: Meteorology.h ";

%feature("docstring")  cmf::atmosphere::Weather::Rn "double Rn(double
albedo, bool daily=false) const

Calculates the net radiation flux (see Formula in C++ documentation) .

(see Formula in C++ documentation)

Parameters:
-----------

albedo:  the albedo (see Formula in C++ documentation)  of the surface

daily:  If true, the net radiation for daily averages will be
calculated ";

%feature("docstring")  cmf::atmosphere::Weather::Weather "Weather()

Calculates the mean pressure for a specific height. ";

%feature("docstring")  cmf::atmosphere::Weather::to_string "std::string to_string() const ";


// File: a00587.xml


// File: a00478.xml
%feature("docstring") std::wfstream "

STL class. ";


// File: a00481.xml
%feature("docstring") std::wifstream "

STL class. ";


// File: a00486.xml
%feature("docstring") std::wios "

STL class. ";


// File: a00491.xml
%feature("docstring") std::wistream "

STL class. ";


// File: a00493.xml
%feature("docstring") std::wistringstream "

STL class. ";


// File: a00496.xml
%feature("docstring") std::wofstream "

STL class. ";


// File: a00499.xml
%feature("docstring") std::wostream "

STL class. ";


// File: a00501.xml
%feature("docstring") std::wostringstream "

STL class. ";


// File: a00504.xml
%feature("docstring") std::wstring "

STL class. ";


// File: a00510.xml
%feature("docstring") std::wstringstream "

STL class. ";


// File: a00119.xml


// File: a00120.xml
%feature("docstring")  cmf::atmosphere::vapour_pressure "double
cmf::atmosphere::vapour_pressure(double T) ";

%feature("docstring")  cmf::atmosphere::global_radiation "double
cmf::atmosphere::global_radiation(cmf::math::Time t, double height,
double sunshine_fraction, double longitude=8, double latitude=51, int
time_zone=1, bool daily=0) ";

%feature("docstring")  cmf::atmosphere::Pressure "double
cmf::atmosphere::Pressure(double height) ";


// File: a00121.xml


// File: a00122.xml
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


// File: a00123.xml
%feature("docstring")  cmf::river::make_river_gap "double
cmf::river::make_river_gap(Reach_ptr root_reach)

Ensures that rivers have a monotone downward flow direction. Reaches
with a bottom higher than any upstream reach are lowered to the
minimum height of any (possibly distant) upstream reach. ";


// File: a00124.xml
%feature("docstring")  cmf::upslope::connections::find_cell "cmf::upslope::Cell* cmf::upslope::find_cell(cells_ref,
cmf::geometry::point p, double max_dist=1e20) ";

%feature("docstring")  cmf::upslope::connections::get_boundary_cells "cmf::upslope::cell_vector
cmf::upslope::get_boundary_cells(cmf::upslope::cells_ref cells) ";

%feature("docstring")  cmf::upslope::connections::get_connections "cmf::water::connection_set cmf::upslope::get_connections(cells_ref
cells) ";

%feature("docstring")
cmf::upslope::connections::connect_cells_with_flux "void
cmf::upslope::connect_cells_with_flux(cells_ref cells, const
cmf::upslope::CellConnector &connect, int start_at_layer=0)

Connects all adjacent cells with a flux connection, implementing the
CellConnector protocol. ";

%feature("docstring")  cmf::upslope::connections::fill_sinks "int
cmf::upslope::fill_sinks(cells_ref cells, double min_difference=0.001)
";

%feature("docstring")  cmf::upslope::connections::area "double
cmf::upslope::area(cells_ref cells) ";

%feature("docstring")  cmf::upslope::connections::set_meteo_station "void cmf::upslope::set_meteo_station(cmf::upslope::cells_ref cells,
cmf::atmosphere::meteo_station_pointer meteo_station) ";

%feature("docstring")  cmf::upslope::connections::set_precipitation "void cmf::upslope::set_precipitation(cmf::upslope::cells_ref cells,
cmf::math::timeseries data_in_mm_day) ";

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


// File: a00125.xml
%feature("docstring")  cmf::upslope::connections::HBVinstall "void
cmf::upslope::connections::HBVinstall(HBVparameters parameters,
cmf::upslope::Cell &cell)

Installs an HBV like model setup at a cell Reference: Bergstroem et al
1995, in Singh (1995): Computer models of watershed hydrology.

Removes the layers of a cell and adds three layers with 1m thickness
and the HBVparameters as retention curve. Installs HBVpercolation
between the layers

Parameters:
-----------

parameters:  The parameters for the HBV like setup

cell:   Cell where HBV like percolation is to be calculated ";


// File: a00126.xml
%feature("docstring")  cmf::upslope::ET::PenmanMonteith "real
cmf::upslope::ET::PenmanMonteith(real Rn, real ra, real rs, real T,
real vap_press_deficit)

Returns the potential ET after Penman-Monteith using some
simplifications for a given Radiation balance, aerodynamic and surface
resistances, and a vapor pressure deficit.

(see Formula in C++ documentation)  where  (see Formula in C++
documentation)  is the evapotranspiration in (see Formula in C++
documentation)

(see Formula in C++ documentation)  is the slope of vapor pressure

(see Formula in C++ documentation)  is the radiation balance

(see Formula in C++ documentation)  is the surface resistance

(see Formula in C++ documentation)  is the aerodynamic resistance

(see Formula in C++ documentation)  is the psychrometer constant

(see Formula in C++ documentation)  is the vapor pressure deficit

Parameters:
-----------

Rn:  Radiation balance in (see Formula in C++ documentation)

ra:  Aerodynamic resistance in (see Formula in C++ documentation)

rs:  Surface resistance in (see Formula in C++ documentation) , is 0
for free water

T:  Actual Temperature in (see Formula in C++ documentation)

vap_press_deficit:  Deficit of vapor pressure (see Formula in C++
documentation)  ";

%feature("docstring")  cmf::upslope::ET::PenmanMonteith "real
cmf::upslope::ET::PenmanMonteith(cmf::atmosphere::Weather A, const
cmf::upslope::vegetation::Vegetation &veg, double h) ";

%feature("docstring")  cmf::upslope::ET::Tact "real
cmf::upslope::ET::Tact(real Tpot, const cmf::upslope::SoilLayer &sw,
const cmf::upslope::vegetation::Vegetation &veg) ";


// File: a00127.xml


// File: a00128.xml
%feature("docstring")  cmf::water::replace_node "void
cmf::water::replace_node(cmf::water::flux_node::ptr oldnode,
cmf::water::flux_node::ptr newnode) ";

%feature("docstring")  cmf::water::count_node_references "int
cmf::water::count_node_references(flux_node::ptr node) ";

%feature("docstring")  cmf::water::get_higher_node "flux_node::ptr
cmf::water::get_higher_node(flux_node::ptr node1, flux_node::ptr
node2) ";

%feature("docstring")  cmf::water::get_lower_node "flux_node::ptr
cmf::water::get_lower_node(flux_node::ptr node1, flux_node::ptr node2)
";


// File: a00129.xml


// File: a00077.xml


// File: a00078.xml


// File: a00079.xml


// File: a00080.xml


// File: a00081.xml


// File: a00082.xml


// File: a00083.xml


// File: a00084.xml


// File: a00085.xml


// File: a00086.xml


// File: a00087.xml


// File: a00088.xml


// File: a00089.xml


// File: a00090.xml


// File: a00091.xml


// File: a00092.xml


// File: a00093.xml


// File: a00094.xml


// File: a00095.xml


// File: a00096.xml


// File: a00097.xml


// File: a00098.xml


// File: a00099.xml


// File: a00100.xml


// File: a00101.xml


// File: a00102.xml


// File: a00103.xml
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

%feature("docstring")  boltzmann "real boltzmann(real x, real tau,
real x_half) ";

%feature("docstring")  sign "real sign(real x) ";

%feature("docstring")  square "real square(real x) ";


// File: a00104.xml


// File: a00105.xml


// File: a00106.xml


// File: a00107.xml


// File: a00108.xml


// File: a00109.xml


// File: a00110.xml


// File: a00111.xml


// File: a00112.xml


// File: a00113.xml


// File: a00114.xml


// File: a00115.xml


// File: a00116.xml


// File: a00117.xml


// File: a00118.xml


// File: todo.xml


// File: dir_d9a1b0d26f9822e8da29dc56defec648.xml


// File: dir_9c0f58e66a49bd0e7ed98f4227402432.xml


// File: dir_e67c5b7563c4f9fd924278918367cd31.xml


// File: dir_fb12ade6f26e1e8b1381214ccf62db5a.xml


// File: dir_275089585c7fc1b5fd5d7d42c69cb1da.xml


// File: dir_13c0ef65a9e347fc169fbd71e3e49474.xml


// File: dir_f870218932a9508c516ef6b35e7a9e95.xml


// File: dir_ae60f1072f58cf5ab99292df65da4727.xml


// File: dir_3ad767d151bc227b36e5233dd672f598.xml


// File: dir_9e07e95c99b62fe6c749a55ea8b632ba.xml


// File: dir_659879de4bebcd3acc17ace9e3e62610.xml


// File: dir_a9c029d02b457f8969019b089ce00baa.xml


// File: dir_63bf9441c04523c4f8b2cd24f88ffa74.xml


// File: dir_04eb38d39328fbb83ae7ceb217cd3674.xml


// File: dir_5345661babbdb40da3cb870fe70fc532.xml


// File: dir_d99dcaf41d53c2860c7b17e0dce0895e.xml

