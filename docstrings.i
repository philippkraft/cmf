
// File: index.xml

// File: a00549.xml
%feature("docstring") std::allocator "

STL class. ";


// File: a00550.xml
%feature("docstring") std::auto_ptr "

STL class. ";


// File: a00551.xml
%feature("docstring") std::bad_alloc "

STL class. ";


// File: a00552.xml
%feature("docstring") std::bad_cast "

STL class. ";


// File: a00553.xml
%feature("docstring") std::bad_exception "

STL class. ";


// File: a00554.xml
%feature("docstring") std::bad_typeid "

STL class. ";


// File: a00555.xml
%feature("docstring") std::basic_fstream "

STL class. ";


// File: a00558.xml
%feature("docstring") std::basic_ifstream "

STL class. ";


// File: a00561.xml
%feature("docstring") std::basic_ios "

STL class. ";


// File: a00566.xml
%feature("docstring") std::basic_iostream "

STL class. ";


// File: a00562.xml
%feature("docstring") std::basic_istream "

STL class. ";


// File: a00568.xml
%feature("docstring") std::basic_istringstream "

STL class. ";


// File: a00573.xml
%feature("docstring") std::basic_ofstream "

STL class. ";


// File: a00563.xml
%feature("docstring") std::basic_ostream "

STL class. ";


// File: a00576.xml
%feature("docstring") std::basic_ostringstream "

STL class. ";


// File: a00581.xml
%feature("docstring") std::basic_string "

STL class. ";


// File: a00584.xml
%feature("docstring") std::basic_string::const_iterator "

STL iterator class. ";


// File: a00585.xml
%feature("docstring") std::basic_string::const_reverse_iterator "

STL iterator class. ";


// File: a00586.xml
%feature("docstring") std::basic_string::iterator "

STL iterator class. ";


// File: a00587.xml
%feature("docstring") std::basic_string::reverse_iterator "

STL iterator class. ";


// File: a00567.xml
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

/*  Internal data storages  */

/*  Gear functions: The following functions evaluate the Gear formulas
of specified order, loading the results into compareStates.  */

/*  Assessment of state variables for integration  */

%feature("docstring")  cmf::math::BDF2::States "cmf::math::StateVariableVector& States()

Returns the vector of StateVariable pointers. ";

%feature("docstring")  cmf::math::BDF2::States "const
cmf::math::StateVariableVector& States() const ";

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

/*  Integrate  */

%feature("docstring")  cmf::math::BDF2::Integrate "int
Integrate(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::BDF2::IntegrateUntil "void
IntegrateUntil(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::BDF2::IntegrateUntil "void
IntegrateUntil(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables until MaxTime. ";

%feature("docstring")  cmf::math::BDF2::__call__ "void
__call__(cmf::math::Time until)

Calls IntegrateUntil. ";

/*  The state variables to integrate  */

/*  Accuracy parameters  */

%feature("docstring")  cmf::math::BDF2::AddStatesFromOwner "void
AddStatesFromOwner(cmf::math::StateVariableOwner &stateOwner)

Adds the state variables of a StateVariableOwner to the state
variables of the solver. ";

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
cmf::math::Time tStepMin=Time::Seconds(10))

Constructs a new Gears_var_Step.

Parameters:
-----------

epsilon:  relative error tolerance per time step (default=1e-9)

tStepMin:  minimum time step (default=10s) ";

%feature("docstring")  cmf::math::BDF2::BDF2 "BDF2(const Integrator
&templ) ";

%feature("docstring")  cmf::math::BDF2::Copy "virtual Integrator*
Copy() const

Returns a new Integrator, based on this (without the state variables),
e.g. same type, epsilon, model time etc. ";

%feature("docstring")  cmf::math::BDF2::Integrate "int
Integrate(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables.

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

%feature("docstring")  cmf::math::BDF2::count "int count() const

returns the number of state variables ";

%feature("docstring")  cmf::math::BDF2::state "const real& state(int
position) const

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::BDF2::state "void state(int
position, real newState)

Simplifies the assessment of state variables. ";


// File: a00590.xml
%feature("docstring") std::bitset "

STL class. ";


// File: a00002.xml
%feature("docstring") cmf::geometry::BoundingBox "

Holds the corner coordinates of a bounding box.

C++ includes: geometry.h ";

%feature("docstring")  cmf::geometry::BoundingBox::Center "point
Center() const ";

%feature("docstring")  cmf::geometry::BoundingBox::Includes "bool
Includes(point p) const

Returns true if the point p is inside the bounding box. ";

%feature("docstring")  cmf::geometry::BoundingBox::isempty "bool
isempty() const

Returns true, if the bounding box is empty. ";

%feature("docstring")  cmf::geometry::BoundingBox::makeempty "void
makeempty()

Makes the bounding box empty. ";

%feature("docstring")  cmf::geometry::BoundingBox::extend "void
extend(point p)

Extends the BoundingBox to include the given point. ";

%feature("docstring")  cmf::geometry::BoundingBox::extend "void
extend(const BoundingBox &bbox)

Extends the bounding box to contain the given Boundingbox although. ";

%feature("docstring")  cmf::geometry::BoundingBox::AsWKT "std::string
AsWKT() const ";

%feature("docstring")  cmf::geometry::BoundingBox::BoundingBox "BoundingBox()

Creates an empty bounding box. ";

%feature("docstring")  cmf::geometry::BoundingBox::BoundingBox "BoundingBox(double X1, double Y1, double X2, double Y2)

Creates a bounding box between to coordinates. ";

%feature("docstring")  cmf::geometry::BoundingBox::BoundingBox "BoundingBox(point p1, point p2)

Creates a bounding box between to coordinates. ";

%feature("docstring")  cmf::geometry::BoundingBox::BoundingBox "BoundingBox(const BoundingBox &copy)

Copies the bounding box. ";

%feature("docstring")  cmf::geometry::BoundingBox::BoundingBox "BoundingBox(double *bbox) ";

%feature("docstring")  cmf::geometry::BoundingBox::Vertices "Points
Vertices() const ";


// File: a00003.xml
%feature("docstring") cmf::upslope::BrooksCoreyRetentionCurve "

Provides the use of the Brooks-Corey retention curve
\\\\begin{eqnarray*} K(W) &=& K_{sat} W^{2+3b} \\\\\\\\ \\\\Psi(W) &=&
\\\\Psi_X \\\\left(\\\\frac{W}{W_X}\\\\right)^{-b} \\\\\\\\ W &=&
{\\\\left( \\\\frac{\\\\Psi_X}{\\\\Psi}\\\\right)
}^{\\\\frac{1}{b}}\\\\ W_X \\\\end{eqnarray*} where:  $K$ is the
conductivity in $\\\\frac m {day}$

$W$ is the wetness (Volume of soil water per volume of pores)

$b$ is the shape of the retention curve (usually between 4 (sand) and
14 (clay))

$\\\\Psi(W)$ is the matric potential in $m H_2O$ at wetness W

$\\\\Psi_X$ is a matric potential at a known wetness in $m H_2O$

$\\\\W_X$ is the wetness with a known matric potential for dynamic
changes with depth, exponential decays of porosity and saturated
conductivity are used The decay function is: $ v(d)=v(0) (1+a)^{-d} $,
where v is the value ( $ K_{sat},\\\\Phi$), d is the depth in m and a
is the fractional decay per m. E.g. 0.1 means the value has in 1 m
depth 90% of the value at the surface.

C++ includes: RetentionCurve.h ";

%feature("docstring")  cmf::upslope::BrooksCoreyRetentionCurve::K "virtual real K(real wetness, real depth) const

Returns the conductivity in m/day at a certain depth and water
content. ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::SetKsat "real SetKsat(real
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

ksat:  Saturated conductivity $\\\\frac{m}{day}$

porosity:   $\\\\frac {m^3 Pores}{m^3 Soil}$

_b:  Shape of the retention curve (if you do not know how to
parameterize this, take a look at the other constructor)

theta_x:   $\\\\theta_X$ Water content at a specific suction pressure

psi_x:  Suction pressure for $\\\\theta_X$ in m water column, use the
conversion functions pF_to_waterhead, pressure_to_waterhead to convert
pressure in to waterhead height (default pF=2.5)

ksat_decay:  Relative decay of conductivity with depth, e.g. 0.1 means
conductivity gets 10% smaller per meter

porosity_decay:  Relative decay of porosity with depth, e.g. 0.1 means
conductivity gets 10% smaller per meter ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::BrooksCoreyRetentionCurve "BrooksCoreyRetentionCurve(real ksat, real porosity, real theta1, real
theta2, real psi_1=pF_to_waterhead(2.5), real
psi_2=pF_to_waterhead(4.2))

Creates a soiltype from two known points of the retention curve (e.g.
fieldcapacity and wilting point).

Parameters:
-----------

ksat:  Saturated conductivity $\\\\frac{m}{day}$

porosity:   $\\\\frac {m^3 Pores}{m^3 Soil}$

theta1:   $\\\\theta_1$ First water content at a specific suction
pressure (e.g. fieldcapacity)

theta2:   $\\\\theta_2$ Second water content at a specific suction
pressure (e.g. wiltingpoint)

psi_1:   $ \\\\Psi_1$ Suction pressure for $\\\\theta_1$ in m water
column, use the conversion functions pF_to_waterhead,
pressure_to_waterhead to convert pressure in to waterhead height
(default pF=2.5)

psi_2:   $ \\\\Psi_2$ Suction pressure for $\\\\theta_2$ in m water
column, use the conversion functions pF_to_waterhead,
pressure_to_waterhead to convert pressure in to waterhead height
(default pF=4.2)

The paramter b (shape of the retention curve) is calculated by: \\\\[
b =
\\\\frac{\\\\log\\\\left(\\\\frac{\\\\Psi_1}{\\\\Psi_2}\\\\right)}{\\\\log\\\\left(\\\\frac{\\\\theta_2}{\\\\theta_1}\\\\right)}
\\\\] ";

%feature("docstring")  cmf::upslope::BrooksCoreyRetentionCurve::copy "virtual BrooksCoreyRetentionCurve* copy() const ";

%feature("docstring")
cmf::upslope::BrooksCoreyRetentionCurve::Wetness_pF "real
Wetness_pF(real pF) const

returns the volumetric water content at a given pF value ";


// File: a00591.xml
%feature("docstring") cmf::upslope::connections::CanopyOverflow "";

%feature("docstring")
cmf::upslope::connections::CanopyOverflow::CanopyOverflow "CanopyOverflow(cmf::water::WaterStorage &Canopy, cmf::water::FluxNode
&target, cmf::upslope::Cell &cell) ";

%feature("docstring")  cmf::upslope::connections::CanopyOverflow::q "real q(const FluxNode &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::CanopyOverflow::Target "FluxNode&
Target(const FluxNode &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::CanopyOverflow::Target "const FluxNode&
Target(const FluxNode &inquirer) const

Returns the other end of a connection than the asking end (const). ";

%feature("docstring")  cmf::upslope::connections::CanopyOverflow::conc
"WaterQuality conc(const FluxNode &inquirer, cmf::math::Time t)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";


// File: a00004.xml
%feature("docstring") cmf::upslope::ET::CanopyStorageEvaporation "

Calculates the evaporation from a canopy storage.

C++ includes: ET.h ";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::CanopyStorageEvaporation "CanopyStorageEvaporation(cmf::water::FluxNode &CanopyStorage,
cmf::water::FluxNode &ET_target, const cmf::atmosphere::Meteorology
&meteo, cmf::upslope::Cell &cell) ";

%feature("docstring")  cmf::upslope::ET::CanopyStorageEvaporation::q "real q(const FluxNode &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::Target "FluxNode&
Target(const FluxNode &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::Target "const FluxNode&
Target(const FluxNode &inquirer) const

Returns the other end of a connection than the asking end (const). ";

%feature("docstring")
cmf::upslope::ET::CanopyStorageEvaporation::conc "WaterQuality
conc(const FluxNode &inquirer, cmf::math::Time t)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";


// File: a00005.xml
%feature("docstring") cmf::Catchment "

The catchment of a reach.

C++ includes: Basin.h ";

%feature("docstring")  cmf::Catchment::Reach "cmf::river::reach&
Reach()

The reach of the catchment. ";

%feature("docstring")  cmf::Catchment::Reach "const
cmf::river::reach& Reach() const ";

%feature("docstring")  cmf::Catchment::AddCell "void
AddCell(cmf::upslope::UpslopeCell *cell)

Returns the total catchment size of this reach including the
catchments of contributing streams. For the contributing area of this
reach only use Upslope().Area() Adds a cell to the catchment. ";

%feature("docstring")  cmf::Catchment::UpdateReachQlat "void
UpdateReachQlat()

Pushes the integrated ponded water to the reach, and resets the ponded
water integration. ";

%feature("docstring")  cmf::Catchment::IntegrateQlat "void
IntegrateQlat(const cmf::math::Time &t, const cmf::math::Time &dt)

Integrates the ponded water for the given time step length.

Parameters:
-----------

t:  Current model time

dt:  Current calculation step length ";

%feature("docstring")  cmf::Catchment::Boundary "cmf::geometry::Ring
Boundary() const

Returns the geometry of the Basin boundary. ";

%feature("docstring")  cmf::Catchment::Catchment "Catchment(cmf::river::reach *_reach)

Creates a catchment without upslope area (to be added later). ";

%feature("docstring")  cmf::Catchment::Catchment "Catchment(cmf::river::reach *_reach, cmf::upslope::UpslopeVector
&upslope)

Creates a catchment from a reach and a contributing upslope area (best
created with the constructor of StudyArea). ";

%feature("docstring")  cmf::Catchment::Catchment "Catchment(const
cmf::Catchment &tocopy) ";

%feature("docstring")  cmf::Catchment::Catchment "Catchment() ";


// File: a00592.xml
%feature("docstring") cmf::upslope::Cell "";

/*  Location  */

%feature("docstring")  cmf::upslope::Cell::Center "cmf::geometry::point Center() const

Returns the location of the cell. ";

%feature("docstring")  cmf::upslope::Cell::Geometry "cmf::geometry::Polygon Geometry() const

Returns the shape of the cell. ";

%feature("docstring")  cmf::upslope::Cell::Geometry "void
Geometry(cmf::geometry::Polygon val)

Sets the shape of the cell. ";

%feature("docstring")  cmf::upslope::Cell::Area "double Area() const

Returns the area of the cell. ";

%feature("docstring")  cmf::upslope::Cell::RecalcSatDepth "void
RecalcSatDepth()

Marks the saturated depth as unvalid. ";

%feature("docstring")  cmf::upslope::Cell::SaturatedDepth "real
SaturatedDepth() ";

/*  Flux nodes of the cell  */

%feature("docstring")  cmf::upslope::Cell::ET "cmf::water::FluxNode&
ET()

Returns the end point of all evapotranspiration of this cell. ";

%feature("docstring")  cmf::upslope::Cell::Canopy "cmf::water::FluxNode& Canopy()

Returns the canopy of this cell. ";

%feature("docstring")  cmf::upslope::Cell::Snow "cmf::water::FluxNode& Snow()

Returns the snow of this cell. ";

%feature("docstring")  cmf::upslope::Cell::SnowWater "cmf::water::FluxNode& SnowWater()

Returns the snow water of this cell. ";

%feature("docstring")  cmf::upslope::Cell::SurfaceWater "cmf::water::FluxNode& SurfaceWater()

returns the surface water of this cell ";

%feature("docstring")  cmf::upslope::Cell::SetCanopy "void
SetCanopy(const cmf::water::FluxNode &newObject)

Exchanges the type of canopy storage. ";

%feature("docstring")  cmf::upslope::Cell::SetSnow "void
SetSnow(const cmf::water::FluxNode &newObject) ";

%feature("docstring")  cmf::upslope::Cell::SetSnowWater "void
SetSnowWater(const cmf::water::FluxNode &newObject) ";

%feature("docstring")  cmf::upslope::Cell::SetSurfaceWater "void
SetSurfaceWater(const cmf::water::FluxNode &newObject) ";

/*  Topology  */

/*  Layers  */

%feature("docstring")  cmf::upslope::Cell::LayerCount "int
LayerCount() const ";

%feature("docstring")  cmf::upslope::Cell::Layer "cmf::upslope::SoilWaterStorage& Layer(int ndx) ";

%feature("docstring")  cmf::upslope::Cell::Layer "const
cmf::upslope::SoilWaterStorage& Layer(int ndx) const ";

%feature("docstring")  cmf::upslope::Cell::AddLayer "void
AddLayer(cmf::upslope::SoilWaterStorage *layer)

Registers a layer at the cell. This function is used by the ctor's of
the layers and should never be used in other code. ";

%feature("docstring")  cmf::upslope::Cell::~Cell "virtual ~Cell() ";

%feature("docstring")  cmf::upslope::Cell::Cell "Cell(double x,
double y, double z, double area) ";


// File: a00006.xml
%feature("docstring") cmf::upslope::CellBuilder "

Abstract base class for Functor's creating cells for a specified
location. Specializations are holding appropriate maps for external
data.

C++ includes: UpslopeFromRaster.h ";

%feature("docstring")  cmf::upslope::CellBuilder::CreateCell "cmf::upslope::UpslopeCell* CreateCell(cmf::geometry::point p, double
Area) const ";

%feature("docstring")  cmf::upslope::CellBuilder::CreateCell "cmf::upslope::UpslopeCell* CreateCell(double x, double y, double z,
double Area) const ";


// File: a00596.xml
%feature("docstring") std::complex "

STL class. ";


// File: a00597.xml
%feature("docstring") cmf::upslope::ET::constantETpot "";

%feature("docstring")  cmf::upslope::ET::constantETpot::GetETpot "real GetETpot(cmf::math::Time t) ";

%feature("docstring")  cmf::upslope::ET::constantETpot::constantETpot
"constantETpot(cmf::upslope::SoilWaterStorage &source,
cmf::water::FluxNode &ET_target, double constantETpot_value) ";

%feature("docstring")  cmf::upslope::ET::constantETpot::GetETpot "real GetETpot(cmf::math::Time t) const =0 ";

%feature("docstring")  cmf::upslope::ET::constantETpot::q "real
q(const FluxNode &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::ET::constantETpot::Target "FluxNode& Target(const FluxNode &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::ET::constantETpot::Target "const
FluxNode& Target(const FluxNode &inquirer) const

Returns the other end of a connection than the asking end (const). ";

%feature("docstring")  cmf::upslope::ET::constantETpot::conc "WaterQuality conc(const FluxNode &inquirer, cmf::math::Time t)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";


// File: a00007.xml
%feature("docstring") cmf::upslope::vegetation::constantFluxVegetation
"

A very simple implementation of IVegetation. All fluxes are constant
Throughfall is set to 800 mm/year

Evapotranspiration is set to 200 mm/year.

C++ includes: Vegetation.h ";

/*  Root distribution  */

%feature("docstring")
cmf::upslope::vegetation::constantFluxVegetation::RootDepth "double
RootDepth() const

The depth of the rooted zone. ";

%feature("docstring")
cmf::upslope::vegetation::constantFluxVegetation::RootDepth "void
RootDepth(double val)

Sets the depth of the rooted zone. ";

%feature("docstring")
cmf::upslope::vegetation::constantFluxVegetation::RootDistribution "virtual double RootDistribution(double upperBoundary, double
lowerBoundary) const

Returns the relative amount of roots [0..1] in the given soil layer
This function assumes a uniform distribution of roots. ";

%feature("docstring")
cmf::upslope::vegetation::constantFluxVegetation::RootDistribution_Michealis_Menten
"virtual double RootDistribution_Michealis_Menten(double
upperBoundary, double lowerBoundary) const ";

%feature("docstring")
cmf::upslope::vegetation::constantFluxVegetation::GetETpot "virtual
double GetETpot(cmf::math::Time timestep) const

Returns the Evapotranspiration rate at a given time step [mm/day]. ";

%feature("docstring")
cmf::upslope::vegetation::constantFluxVegetation::GetThroughfall "virtual cmf::water::WaterFlux GetThroughfall(cmf::math::Time timestep)
const

Returns the Rate of precipitation entering the soil (precipitation -
interception) [mm/day]. ";

%feature("docstring")
cmf::upslope::vegetation::constantFluxVegetation::GetTemperature "virtual double GetTemperature(cmf::math::Time timestep) const

Returns the air temperature at the timestep. ";

%feature("docstring")
cmf::upslope::vegetation::constantFluxVegetation::constantFluxVegetation
"constantFluxVegetation(double throughfall=800.0/365.0, double
etpot=200.0/365.0, double Temperature=10)

Creates the constantFluxVegetation.

Parameters:
-----------

throughfall:  Throughfall rate in mm/day

etpot:  ETpot in mm/day

Temperature:  Air Temperature in deg C ";

%feature("docstring")
cmf::upslope::vegetation::constantFluxVegetation::constantFluxVegetation
"constantFluxVegetation(const constantFluxVegetation &forcopy) ";

%feature("docstring")
cmf::upslope::vegetation::constantFluxVegetation::copy "constantFluxVegetation* copy() const

Pure virtual method, to copy the current vegetation. ";


// File: a00008.xml
%feature("docstring") cmf::upslope::maps::ContinuousRasterMap "

Distributes a scalar value (int, float or double) with a raster
dataset. The simplest way to create a continuous raster map is to load
a raster in the ESRI ASCII format.

C++ includes: maps.h ";

%feature("docstring")
cmf::upslope::maps::ContinuousRasterMap::ContinuousRasterMap "ContinuousRasterMap(const cmf::geometry::Raster< _Ty > &raster)

Creates a new continuous map.

Parameters:
-----------

raster:  Pointer to an existing cmf::geometry::Raster ";

%feature("docstring")
cmf::upslope::maps::ContinuousRasterMap::ContinuousRasterMap "ContinuousRasterMap(std::string ASCFileName, _Ty factor=1, _Ty
nodata=-9999)

Creates a new continuous map.

Parameters:
-----------

ASCFileName:  A file name of an ESRI ASC raster dataset

factor:  Scales the raster dataset (e.g. unit conversion)

nodata:  Value returned, if the raster has no data (if Has(x,y)=false)
default: -9999 ";

%feature("docstring")  cmf::upslope::maps::ContinuousRasterMap::Get "_Ty Get(cmf::geometry::point p)

Returns a soil type by calling the protected virtual function get. ";

%feature("docstring")  cmf::upslope::maps::ContinuousRasterMap::Get "_Ty Get(double x, double y)

Returns a soil type by calling the protected virtual function get. ";

%feature("docstring")  cmf::upslope::maps::ContinuousRasterMap::Has "bool Has(double x, double y) ";

%feature("docstring")  cmf::upslope::maps::ContinuousRasterMap::Has "bool Has(cmf::geometry::point p) ";

%feature("docstring")
cmf::upslope::maps::ContinuousRasterMap::__call__ "_Ty
__call__(double x, double y) ";


// File: a00009.xml
%feature("docstring") cmf::upslope::connections::DarcyConnection "

Calculates the lateral flow using the gravitational potential only.

\\\\[ q_{lat} = \\\\frac{\\\\Delta \\\\Psi_G}{\\\\|C_1-C_2\\\\|}
\\\\frac12 (T(C_1)+T(C_2)) w \\\\] where  $ \\\\Delta \\\\Psi_G $ is
the gravitational potential difference

$ \\\\|C_1-C_2\\\\| $ is the distance from Cell 1 to Cell 2

$ T(C) $ is the transmissivity of cell C, calculated by
SoilType::Transmissivity

$ w $ is the width of the connection of the cells

C++ includes: FluxConnections.h ";

%feature("docstring")
cmf::upslope::connections::DarcyConnection::DarcyConnection "DarcyConnection(cmf::upslope::SoilWaterStorage &left,
cmf::upslope::SoilWaterStorage &right, real FlowArea, real Distance)
";

%feature("docstring")  cmf::upslope::connections::DarcyConnection::q "real q(const FluxNode &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::DarcyConnection::Target "FluxNode&
Target(const FluxNode &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::DarcyConnection::Target "const FluxNode&
Target(const FluxNode &inquirer) const

Returns the other end of a connection than the asking end (const). ";

%feature("docstring")
cmf::upslope::connections::DarcyConnection::conc "WaterQuality
conc(const FluxNode &inquirer, cmf::math::Time t)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";


// File: a00010.xml
%feature("docstring") cmf::upslope::vegetation::DatadrivenVegetation "

A Vegetation class with measured or previously calculated Throughfall
and ETpot.

C++ includes: DatadrivenVegetation.h ";

/*  Root distribution  */

%feature("docstring")
cmf::upslope::vegetation::DatadrivenVegetation::RootDepth "double
RootDepth() const

The depth of the rooted zone. ";

%feature("docstring")
cmf::upslope::vegetation::DatadrivenVegetation::RootDepth "void
RootDepth(double val)

Sets the depth of the rooted zone. ";

%feature("docstring")
cmf::upslope::vegetation::DatadrivenVegetation::RootDistribution "virtual double RootDistribution(double upperBoundary, double
lowerBoundary) const

Returns the relative amount of roots [0..1] in the given soil layer
This function assumes a uniform distribution of roots. ";

%feature("docstring")
cmf::upslope::vegetation::DatadrivenVegetation::RootDistribution_Michealis_Menten
"virtual double RootDistribution_Michealis_Menten(double
upperBoundary, double lowerBoundary) const ";

%feature("docstring")
cmf::upslope::vegetation::DatadrivenVegetation::GetThroughfall "virtual cmf::water::WaterFlux GetThroughfall(cmf::math::Time timestep)
";

%feature("docstring")
cmf::upslope::vegetation::DatadrivenVegetation::GetETpot "virtual
double GetETpot(cmf::math::Time timestep) ";

%feature("docstring")
cmf::upslope::vegetation::DatadrivenVegetation::DatadrivenVegetation "DatadrivenVegetation()

Standard constructor. ";

%feature("docstring")
cmf::upslope::vegetation::DatadrivenVegetation::DatadrivenVegetation "DatadrivenVegetation(cmf::math::Time begin, cmf::math::Time step)

Initializes Throughfall and ETpot and ThroughfalConcentration
timeseries with the given start and step time. ";

%feature("docstring")
cmf::upslope::vegetation::DatadrivenVegetation::DatadrivenVegetation "DatadrivenVegetation(cmf::math::timeseries throughfall,
cmf::math::timeseries etpot)

Uses the given timeseries for Throughfall and ETpot and initializes
the ThroughfalConcentration timeseries with the begin and step time of
Throughfall. ";

%feature("docstring")
cmf::upslope::vegetation::DatadrivenVegetation::DatadrivenVegetation "DatadrivenVegetation(const DatadrivenVegetation &copy)

Copy constructor. ";

%feature("docstring")
cmf::upslope::vegetation::DatadrivenVegetation::copy "DatadrivenVegetation* copy() ";

%feature("docstring")
cmf::upslope::vegetation::DatadrivenVegetation::GetETpot "virtual
double GetETpot(cmf::math::Time timestep) const =0

Returns the Evapotranspiration rate at a given time step [mm/day]. ";

%feature("docstring")
cmf::upslope::vegetation::DatadrivenVegetation::GetThroughfall "virtual cmf::water::WaterFlux GetThroughfall(cmf::math::Time timestep)
const =0

Returns the Rate of precipitation entering the soil (precipitation -
interception) [mm/day]. ";

%feature("docstring")
cmf::upslope::vegetation::DatadrivenVegetation::GetTemperature "virtual double GetTemperature(cmf::math::Time timestep) const =0

Returns the air temperature at the timestep. ";

%feature("docstring")
cmf::upslope::vegetation::DatadrivenVegetation::copy "virtual
IVegetation* copy() const =0

Pure virtual method, to copy the current vegetation. ";


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

%feature("docstring")  cmf::math::Date::DOY "int DOY()

Returns the day of year. ";

%feature("docstring")  cmf::math::Date::ToString "std::string
ToString()

Returns a string representing the date. ";


// File: a00598.xml
%feature("docstring") std::deque "

STL class. ";


// File: a00599.xml
%feature("docstring") std::deque::const_iterator "

STL iterator class. ";


// File: a00600.xml
%feature("docstring") std::deque::const_reverse_iterator "

STL iterator class. ";


// File: a00601.xml
%feature("docstring") std::deque::iterator "

STL iterator class. ";


// File: a00602.xml
%feature("docstring") std::deque::reverse_iterator "

STL iterator class. ";


// File: a00012.xml
%feature("docstring") cmf::upslope::maps::DiscreteRasterMap "

Distributes a discrete value or object as a given in an integer raster
dataset according to a lookup table. To use this, one have to provide
the dstribution raster and add id object pairs to the map prior to
use.

C++ includes: maps.h ";

%feature("docstring")  cmf::upslope::maps::DiscreteRasterMap::Add "void Add(int index, _Ty value)

Adds a value to the lookup table. ";

%feature("docstring")
cmf::upslope::maps::DiscreteRasterMap::DiscreteRasterMap "DiscreteRasterMap(geometry::Raster< int > *raster, _Ty
NoDataValue=_Ty())

Creates a new map of discrete values, for example soil types.

Parameters:
-----------

raster:  Pointer to an existing cmf::geometry::Raster<int>

NoDataValue:  Value returned, if the raster has no data
(Has(x,y)=false) ";

%feature("docstring")
cmf::upslope::maps::DiscreteRasterMap::DiscreteRasterMap "DiscreteRasterMap(std::string RasterFileName, _Ty NoDataValue=_Ty())

Creates a new map of discrete values, for example soil types.

Parameters:
-----------

RasterFileName:  File name of an ESRI ASC-raster, containing the
handles

NoDataValue:  Value returned if the handle is raster.NoData() or the
value is not found in the lookup table ";

%feature("docstring")  cmf::upslope::maps::DiscreteRasterMap::Get "_Ty Get(cmf::geometry::point p)

Returns a soil type by calling the protected virtual function get. ";

%feature("docstring")  cmf::upslope::maps::DiscreteRasterMap::Get "_Ty Get(double x, double y)

Returns a soil type by calling the protected virtual function get. ";

%feature("docstring")  cmf::upslope::maps::DiscreteRasterMap::Has "bool Has(double x, double y) ";

%feature("docstring")  cmf::upslope::maps::DiscreteRasterMap::Has "bool Has(cmf::geometry::point p) ";

%feature("docstring")  cmf::upslope::maps::DiscreteRasterMap::__call__
"_Ty __call__(double x, double y) ";


// File: a00603.xml
%feature("docstring") std::domain_error "

STL class. ";


// File: a00427.xml
%feature("docstring") cmf::upslope::ET::ET "";

%feature("docstring")  cmf::upslope::ET::ET::GetETpot "real
GetETpot(cmf::math::Time t) const =0 ";

%feature("docstring")  cmf::upslope::ET::ET::ET "ET(cmf::upslope::SoilWaterStorage &source, cmf::water::FluxNode
&ET_target, Vegetation veg) ";

%feature("docstring")  cmf::upslope::ET::ET::q "real q(const FluxNode
&inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::ET::ET::Target "FluxNode&
Target(const FluxNode &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::ET::ET::Target "const FluxNode&
Target(const FluxNode &inquirer) const

Returns the other end of a connection than the asking end (const). ";

%feature("docstring")  cmf::upslope::ET::ET::conc "WaterQuality
conc(const FluxNode &inquirer, cmf::math::Time t)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";


// File: a00604.xml
%feature("docstring") std::exception "

STL class. ";


// File: a00013.xml
%feature("docstring") cmf::math::ExplicitEuler_fixed "

An explicit Euler integrator, with a fixed time step.

C++ includes: ExplicitEuler_fixed.h ";

/*  Assessment of state variables for integration  */

%feature("docstring")  cmf::math::ExplicitEuler_fixed::States "cmf::math::StateVariableVector& States()

Returns the vector of StateVariable pointers. ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::States "const
cmf::math::StateVariableVector& States() const ";

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

/*  Integrate  */

%feature("docstring")  cmf::math::ExplicitEuler_fixed::Integrate "int
Integrate(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::IntegrateUntil
"void IntegrateUntil(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::IntegrateUntil
"void IntegrateUntil(cmf::math::Time MaxTime, cmf::math::Time
TimeStep)

Integrates the vector of state variables until MaxTime. ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::__call__ "void
__call__(cmf::math::Time until)

Calls IntegrateUntil. ";

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

%feature("docstring")  cmf::math::ExplicitEuler_fixed::state "const
real& state(int position) const

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::ExplicitEuler_fixed::state "void
state(int position, real newState)

Simplifies the assessment of state variables. ";


// File: a00014.xml
%feature("docstring") cmf::math::ExplicitEuler_variable "

An explicit Euler integrator with variable step size.

C++ includes: ExplicitEuler_variable.h ";

/*  Assessment of state variables for integration  */

%feature("docstring")  cmf::math::ExplicitEuler_variable::States "cmf::math::StateVariableVector& States()

Returns the vector of StateVariable pointers. ";

%feature("docstring")  cmf::math::ExplicitEuler_variable::States "const cmf::math::StateVariableVector& States() const ";

/*  model time  */

%feature("docstring")  cmf::math::ExplicitEuler_variable::ModelTime "cmf::math::Time ModelTime() const

Returns the current model time. ";

%feature("docstring")  cmf::math::ExplicitEuler_variable::ModelTime "void ModelTime(cmf::math::Time val)

Sets the current model time. ";

%feature("docstring")  cmf::math::ExplicitEuler_variable::TimeStep "cmf::math::Time TimeStep() const

Returns the last time step. ";

%feature("docstring")  cmf::math::ExplicitEuler_variable::NextTimeStep
"cmf::math::Time NextTimeStep() const

Returns the next time step width. ";

%feature("docstring")  cmf::math::ExplicitEuler_variable::NextTimeStep
"void NextTimeStep(cmf::math::Time val)

Sets the next time step width. ";

%feature("docstring")  cmf::math::ExplicitEuler_variable::MinTimestep
"const cmf::math::Time MinTimestep() const

Returns The minimal allowed time step length. ";

%feature("docstring")  cmf::math::ExplicitEuler_variable::Iterations "int Iterations() const ";

%feature("docstring")
cmf::math::ExplicitEuler_variable::ResetIterations "void
ResetIterations() ";

/*  Integrate  */

%feature("docstring")  cmf::math::ExplicitEuler_variable::Integrate "int Integrate(cmf::math::Time MaxTime) ";

%feature("docstring")
cmf::math::ExplicitEuler_variable::IntegrateUntil "void
IntegrateUntil(cmf::math::Time MaxTime) ";

%feature("docstring")
cmf::math::ExplicitEuler_variable::IntegrateUntil "void
IntegrateUntil(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables until MaxTime. ";

%feature("docstring")  cmf::math::ExplicitEuler_variable::__call__ "void __call__(cmf::math::Time until)

Calls IntegrateUntil. ";

/*  The state variables to integrate  */

/*  Accuracy parameters  */

%feature("docstring")
cmf::math::ExplicitEuler_variable::AddStatesFromOwner "void
AddStatesFromOwner(cmf::math::StateVariableOwner &stateOwner)

Adds the state variables of a StateVariableOwner to the state
variables of the solver. ";

%feature("docstring")
cmf::math::ExplicitEuler_variable::ExplicitEuler_variable "ExplicitEuler_variable(const StateVariableVector &states, real
epsilon=1e-9, cmf::math::Time tStepMin=Time::Seconds(10))

Constructs a new ExplicitEuler_variable from a pointer to a vector of
state variables.

The Integrator becomes the owner of states

Parameters:
-----------

states:  Statevariables of the system

epsilon:  relative error tolerance per time step (default=1e-9)

tStepMin:  minimum time step (default=10s) ";

%feature("docstring")
cmf::math::ExplicitEuler_variable::ExplicitEuler_variable "ExplicitEuler_variable(real epsilon=1e-9, cmf::math::Time
tStepMin=Time::Seconds(10))

Constructs a new ExplicitEuler_variable.

Parameters:
-----------

epsilon:  relative error tolerance per time step (default=1e-9)

tStepMin:  minimum time step (default=10s) ";

%feature("docstring")
cmf::math::ExplicitEuler_variable::ExplicitEuler_variable "ExplicitEuler_variable(const Integrator &copy)

Copy constructor. ";

%feature("docstring")
cmf::math::ExplicitEuler_variable::~ExplicitEuler_variable "virtual
~ExplicitEuler_variable() ";

%feature("docstring")  cmf::math::ExplicitEuler_variable::Copy "virtual Integrator* Copy() const

Returns a new Integrator, based on this (without the state variables),
e.g. same type, epsilon, model time etc. ";

%feature("docstring")  cmf::math::ExplicitEuler_variable::Integrate "int Integrate(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables.

Parameters:
-----------

MaxTime:  To stop the model (if running in a model framework) at time
steps of value exchange e.g. full hours, the next value exchange time
can be given

TimeStep:  Takes the proposed time step, and changes it into the
effectively used time step according to the local stiffness of the
problem and MaxTime ";

%feature("docstring")  cmf::math::ExplicitEuler_variable::count "int
count() const

returns the number of state variables ";

%feature("docstring")  cmf::math::ExplicitEuler_variable::state "const real& state(int position) const

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::ExplicitEuler_variable::state "void
state(int position, real newState)

Simplifies the assessment of state variables. ";


// File: a00015.xml
%feature("docstring") cmf::water::reaction::ExternalFunctionReaction "

Not yet implemented: Will take a pointer to an external function to
calculate the reaction.

C++ includes: Reaction.h ";

%feature("docstring")
cmf::water::reaction::ExternalFunctionReaction::ReactiveFlux "virtual
double ReactiveFlux(const WaterQuality &Environment)=0

Returns the reactive flux as $\\\\frac{d[X]}{dt}=f([A],[B]...)
\\\\left[\\\\frac{mol}{m^3\\\\ day}\\\\right]$. ";


// File: a00016.xml
%feature("docstring") cmf::upslope::FlexibleSizeLayer "

A soil water storage above a FlexibleSizeSaturatedLayer, can only be
created by FlexibleSaturatedLayer.

C++ includes: SoilWaterStorage.h ";

/*  Overrides of FluxNode  */

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::copy "virtual
WaterStorage* copy() const ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::RecalcFluxes "virtual bool RecalcFluxes(cmf::math::Time t)

Pure FluxNodes do not influence fluxes, therefore no recalculation of
fluxes is required by fluxnode. WaterStorage overrides this, since
state changes require an update of the fluxes. ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::Empty "virtual bool Empty() ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::LowerBoundary
"virtual real LowerBoundary() ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::UpperBoundary
"virtual double UpperBoundary() const

Returns the upper boundary of the water storage below ground in m. ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::LowerBoundary
"virtual double LowerBoundary() const

Returns the lower boundary of the water storage below ground in m. ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::Thickness "double Thickness() const ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::Soil "virtual
const RetentionCurve& Soil() const

Returns the soil properties of the water storage. ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::Soil "virtual
RetentionCurve& Soil() ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::theta "virtual real theta() const

Returns the actual volumetric water content of the water storage. ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::theta "virtual void theta(real Value) ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::K "double K()
const

Returns the actual conductivity $\\\\frac{m}{day}$. ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::Wetness "virtual double Wetness() const

Returns the wetness of the soil $ \\\\frac{V_{H_2O}}{V_{pores}} $. ";

%feature("docstring")
cmf::upslope::FlexibleSizeLayer::MatrixPotential "virtual real
MatrixPotential() const

Calls RetentionCurve::Matrixpotential. ";

%feature("docstring")
cmf::upslope::FlexibleSizeLayer::GravitationalPotential "real
GravitationalPotential() const

Gravitational Potential in m, reference height is sea level \\\\[
\\\\Psi_G=h \\\\]. ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::Capacity "virtual real Capacity()

Returns the capacity of the water storage in m3. ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::SetPotential "void SetPotential(real waterhead)

Sets the potential of this soil water storage. ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::Potential "virtual real Potential() const

Returns the total potential in m \\\\[ \\\\Psi = \\\\Psi_M + \\\\Psi_G
\\\\]. ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::Potential "virtual real Potential()

Returns the water potential of the node in m waterhead The base class
water storage always returns the height of the location. ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::copy "SoilWaterStorage* copy() ";

%feature("docstring")
cmf::upslope::FlexibleSizeLayer::AddStateVariables "void
AddStateVariables(cmf::math::StateVariableVector &vector)

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::Solute "SoluteStorage& Solute(const cmf::water::Solute &solute)

Returns the water quality of the water storage. ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::Solute "const
SoluteStorage& Solute(const cmf::water::Solute &solute) const ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::conc "real
conc(const cmf::water::Solute &solute) const

Returns the concentration of the given solute. ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::conc "WaterQuality conc(cmf::math::Time t)

Returns the current WaterQuality (concentration of all solutes). ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::conc "void
conc(const cmf::water::Solute &solute, real NewConcetration)

Sets a new concentration. ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::water "real
water() const

Returns the volume of water in this storage in m3. ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::water "void
water(real newwatercontent)

Sets the volume of water in this storage in m3. ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::ToString "virtual std::string ToString() const ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::Derivate "virtual real Derivate(const cmf::math::Time &time)

Returns the derivate of the state variable at time time. ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::State "const
real& State() const

Returns the current state of the variable. ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::State "void
State(const real &newState)

Gives access to the state variable. ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::StateIsChanged
"bool StateIsChanged()

Returns if the state was currently updated. ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::ConnectionTo "cmf::water::FluxConnection& ConnectionTo(const FluxNode &target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::FluxTo "real
FluxTo(const FluxNode &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from target into this\"). ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::Waterbalance "real Waterbalance(cmf::math::Time t, const FluxConnection *except=0)

Returns the sum of all fluxes (positive and negative) at time t.
Single fluxes can be excluded from the calculation.

Parameters:
-----------

t:  Time of the query

except:  A FluxConnection that is excluded from the Waterbalance (e.g.
to prevent closed circuits) ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::sum_inflow "cmf::water::WaterFlux sum_inflow(cmf::math::Time t, const
FluxConnection *except=0)

Returns the sum of the incoming (positive) fluxes and the mix of their
concentrations. ";

%feature("docstring")  cmf::upslope::FlexibleSizeLayer::sum_outflow "cmf::water::WaterFlux sum_outflow(cmf::math::Time t, const
FluxConnection *except=0)

Returns the sum of the incoming (positive) fluxes and their
concentrations. ";


// File: a00017.xml
%feature("docstring") cmf::upslope::FlexibleSizeSaturatedZone "

A soil water storage with a flexible (non constant) upper boundary,
according to its fill state. This soil water storage is always
saturated.

C++ includes: SoilWaterStorage.h ";

/*  Overrides of FluxNode  */

%feature("docstring")  cmf::upslope::FlexibleSizeSaturatedZone::copy "virtual WaterStorage* copy() const ";

%feature("docstring")
cmf::upslope::FlexibleSizeSaturatedZone::RecalcFluxes "virtual bool
RecalcFluxes(cmf::math::Time t)

Pure FluxNodes do not influence fluxes, therefore no recalculation of
fluxes is required by fluxnode. WaterStorage overrides this, since
state changes require an update of the fluxes. ";

%feature("docstring")  cmf::upslope::FlexibleSizeSaturatedZone::Empty
"virtual bool Empty() ";

%feature("docstring")
cmf::upslope::FlexibleSizeSaturatedZone::UpperBoundary "virtual real
UpperBoundary()

Returns the current upper boundary of this layer (function of state).
";

%feature("docstring")
cmf::upslope::FlexibleSizeSaturatedZone::FlexibleSizeSaturatedZone "FlexibleSizeSaturatedZone(Cell &cell, real maximum_upperboundary, real
lowerboundary, const RetentionCurve &r_curve)

Creates two soilwaterstorages and registers them with the owning cell.
The boundary between the two layers is flexible and changes with
saturation. The lower layer is always saturated, the upper never. ";

%feature("docstring")
cmf::upslope::FlexibleSizeSaturatedZone::UpperBoundary "virtual
double UpperBoundary() const

Returns the upper boundary of the water storage below ground in m. ";

%feature("docstring")
cmf::upslope::FlexibleSizeSaturatedZone::LowerBoundary "virtual
double LowerBoundary() const

Returns the lower boundary of the water storage below ground in m. ";

%feature("docstring")
cmf::upslope::FlexibleSizeSaturatedZone::Thickness "double
Thickness() const ";

%feature("docstring")  cmf::upslope::FlexibleSizeSaturatedZone::Soil "virtual const RetentionCurve& Soil() const

Returns the soil properties of the water storage. ";

%feature("docstring")  cmf::upslope::FlexibleSizeSaturatedZone::Soil "virtual RetentionCurve& Soil() ";

%feature("docstring")  cmf::upslope::FlexibleSizeSaturatedZone::theta
"virtual real theta() const

Returns the actual volumetric water content of the water storage. ";

%feature("docstring")  cmf::upslope::FlexibleSizeSaturatedZone::theta
"virtual void theta(real Value) ";

%feature("docstring")  cmf::upslope::FlexibleSizeSaturatedZone::K "double K() const

Returns the actual conductivity $\\\\frac{m}{day}$. ";

%feature("docstring")
cmf::upslope::FlexibleSizeSaturatedZone::Wetness "virtual double
Wetness() const

Returns the wetness of the soil $ \\\\frac{V_{H_2O}}{V_{pores}} $. ";

%feature("docstring")
cmf::upslope::FlexibleSizeSaturatedZone::MatrixPotential "virtual
real MatrixPotential() const

Calls RetentionCurve::Matrixpotential. ";

%feature("docstring")
cmf::upslope::FlexibleSizeSaturatedZone::GravitationalPotential "real
GravitationalPotential() const

Gravitational Potential in m, reference height is sea level \\\\[
\\\\Psi_G=h \\\\]. ";

%feature("docstring")
cmf::upslope::FlexibleSizeSaturatedZone::Capacity "virtual real
Capacity()

Returns the capacity of the water storage in m3. ";

%feature("docstring")
cmf::upslope::FlexibleSizeSaturatedZone::SetPotential "void
SetPotential(real waterhead)

Sets the potential of this soil water storage. ";

%feature("docstring")
cmf::upslope::FlexibleSizeSaturatedZone::Potential "virtual real
Potential() const

Returns the total potential in m \\\\[ \\\\Psi = \\\\Psi_M + \\\\Psi_G
\\\\]. ";

%feature("docstring")
cmf::upslope::FlexibleSizeSaturatedZone::Potential "virtual real
Potential()

Returns the water potential of the node in m waterhead The base class
water storage always returns the height of the location. ";

%feature("docstring")  cmf::upslope::FlexibleSizeSaturatedZone::copy "SoilWaterStorage* copy() ";

%feature("docstring")
cmf::upslope::FlexibleSizeSaturatedZone::AddStateVariables "void
AddStateVariables(cmf::math::StateVariableVector &vector)

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";

%feature("docstring")  cmf::upslope::FlexibleSizeSaturatedZone::Solute
"SoluteStorage& Solute(const cmf::water::Solute &solute)

Returns the water quality of the water storage. ";

%feature("docstring")  cmf::upslope::FlexibleSizeSaturatedZone::Solute
"const SoluteStorage& Solute(const cmf::water::Solute &solute) const
";

%feature("docstring")  cmf::upslope::FlexibleSizeSaturatedZone::conc "real conc(const cmf::water::Solute &solute) const

Returns the concentration of the given solute. ";

%feature("docstring")  cmf::upslope::FlexibleSizeSaturatedZone::conc "WaterQuality conc(cmf::math::Time t)

Returns the current WaterQuality (concentration of all solutes). ";

%feature("docstring")  cmf::upslope::FlexibleSizeSaturatedZone::conc "void conc(const cmf::water::Solute &solute, real NewConcetration)

Sets a new concentration. ";

%feature("docstring")  cmf::upslope::FlexibleSizeSaturatedZone::water
"real water() const

Returns the volume of water in this storage in m3. ";

%feature("docstring")  cmf::upslope::FlexibleSizeSaturatedZone::water
"void water(real newwatercontent)

Sets the volume of water in this storage in m3. ";

%feature("docstring")
cmf::upslope::FlexibleSizeSaturatedZone::ToString "virtual
std::string ToString() const ";

%feature("docstring")
cmf::upslope::FlexibleSizeSaturatedZone::Derivate "virtual real
Derivate(const cmf::math::Time &time)

Returns the derivate of the state variable at time time. ";

%feature("docstring")  cmf::upslope::FlexibleSizeSaturatedZone::State
"const real& State() const

Returns the current state of the variable. ";

%feature("docstring")  cmf::upslope::FlexibleSizeSaturatedZone::State
"void State(const real &newState)

Gives access to the state variable. ";

%feature("docstring")
cmf::upslope::FlexibleSizeSaturatedZone::StateIsChanged "bool
StateIsChanged()

Returns if the state was currently updated. ";

%feature("docstring")
cmf::upslope::FlexibleSizeSaturatedZone::ConnectionTo "cmf::water::FluxConnection& ConnectionTo(const FluxNode &target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::upslope::FlexibleSizeSaturatedZone::FluxTo
"real FluxTo(const FluxNode &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from target into this\"). ";

%feature("docstring")
cmf::upslope::FlexibleSizeSaturatedZone::Waterbalance "real
Waterbalance(cmf::math::Time t, const FluxConnection *except=0)

Returns the sum of all fluxes (positive and negative) at time t.
Single fluxes can be excluded from the calculation.

Parameters:
-----------

t:  Time of the query

except:  A FluxConnection that is excluded from the Waterbalance (e.g.
to prevent closed circuits) ";

%feature("docstring")
cmf::upslope::FlexibleSizeSaturatedZone::sum_inflow "cmf::water::WaterFlux sum_inflow(cmf::math::Time t, const
FluxConnection *except=0)

Returns the sum of the incoming (positive) fluxes and the mix of their
concentrations. ";

%feature("docstring")
cmf::upslope::FlexibleSizeSaturatedZone::sum_outflow "cmf::water::WaterFlux sum_outflow(cmf::math::Time t, const
FluxConnection *except=0)

Returns the sum of the incoming (positive) fluxes and their
concentrations. ";


// File: a00608.xml
%feature("docstring") FluxConnection "";


// File: a00018.xml
%feature("docstring") cmf::water::FluxConnection "

Represents a connection between FluxNodes, where water fluxes occur.

C++ includes: FluxConnection.h ";

%feature("docstring")  cmf::water::FluxConnection::Target "FluxNode&
Target(const FluxNode &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::water::FluxConnection::Target "const
FluxNode& Target(const FluxNode &inquirer) const

Returns the other end of a connection than the asking end (const). ";

%feature("docstring")  cmf::water::FluxConnection::q "real q(const
FluxNode &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::water::FluxConnection::conc "WaterQuality
conc(const FluxNode &inquirer, cmf::math::Time t)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";

%feature("docstring")  cmf::water::FluxConnection::FluxConnection "FluxConnection(FluxNode &left, FluxNode &right)

Creates a flux connection between the FLuxNode left and right.

Parameters:
-----------

left:   FluxNode on the one side of the connection

right:   FluxNode on the other side of the connection ";

%feature("docstring")  cmf::water::FluxConnection::~FluxConnection "virtual ~FluxConnection() ";


// File: a00019.xml
%feature("docstring") cmf::water::FluxNode "

Base class for everything that can be connected by fluxes. Flux nodes
can be WaterStorages, flux end points, sinks, sources and bridges to
other model domains (e.g. Ponded water to river system). The base
class can be used where a simple routing, potentially with mixing, is
needed.

C++ includes: FluxConnection.h ";

%feature("docstring")  cmf::water::FluxNode::~FluxNode "virtual
~FluxNode()

The destructor deletes all connections. ";

%feature("docstring")  cmf::water::FluxNode::RecalcFluxes "virtual
bool RecalcFluxes(cmf::math::Time t)

Pure FluxNodes do not influence fluxes, therefore no recalculation of
fluxes is required by fluxnode. WaterStorage overrides this, since
state changes require an update of the fluxes. ";

%feature("docstring")  cmf::water::FluxNode::ConnectionTo "cmf::water::FluxConnection& ConnectionTo(const FluxNode &target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::water::FluxNode::FluxTo "real
FluxTo(const FluxNode &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from target into this\"). ";

%feature("docstring")  cmf::water::FluxNode::Waterbalance "real
Waterbalance(cmf::math::Time t, const FluxConnection *except=0)

Returns the sum of all fluxes (positive and negative) at time t.
Single fluxes can be excluded from the calculation.

Parameters:
-----------

t:  Time of the query

except:  A FluxConnection that is excluded from the Waterbalance (e.g.
to prevent closed circuits) ";

%feature("docstring")  cmf::water::FluxNode::sum_inflow "cmf::water::WaterFlux sum_inflow(cmf::math::Time t, const
FluxConnection *except=0)

Returns the sum of the incoming (positive) fluxes and the mix of their
concentrations. ";

%feature("docstring")  cmf::water::FluxNode::conc "virtual
cmf::water::WaterQuality conc(cmf::math::Time t)

Returns the water quality of the FluxNode, if it is not overridden
this is the mix of the incoming fluxes. ";

%feature("docstring")  cmf::water::FluxNode::sum_outflow "cmf::water::WaterFlux sum_outflow(cmf::math::Time t, const
FluxConnection *except=0)

Returns the sum of the incoming (positive) fluxes and their
concentrations. ";

%feature("docstring")  cmf::water::FluxNode::Potential "virtual real
Potential()

Returns the water potential of the node in m waterhead The base class
water storage always returns the height of the location. ";

%feature("docstring")  cmf::water::FluxNode::Empty "virtual bool
Empty() ";

%feature("docstring")  cmf::water::FluxNode::copy "virtual FluxNode*
copy() const ";


// File: a00556.xml
%feature("docstring") std::fstream "

STL class. ";


// File: a00020.xml
%feature("docstring") cmf::math::Gears_Fixpoint "

An up to 4 step Gears-Method with fixpoint iteration, needs bug
fixing.

Derived from Roussel C. and Roussel M. (2003) \"Generic Object-
Oriented Differential Equation Integrators\", C/C++ User Journal, Nov.
2003,http://www.ddj.com/cpp/184401724?pgno=8

C++ includes: Gears_Fixpoint.h ";

/*  Internal data storages  */

/*  Time step management  */

%feature("docstring")  cmf::math::Gears_Fixpoint::Time_reduce_stage "int Time_reduce_stage() const

Returns int Stage oof time reduction. ";

/*  Gear functions: The following functions evaluate the Gear formulas
of specified order, loading the results into compareStates.  */

/*  Assessment of state variables for integration  */

%feature("docstring")  cmf::math::Gears_Fixpoint::States "cmf::math::StateVariableVector& States()

Returns the vector of StateVariable pointers. ";

%feature("docstring")  cmf::math::Gears_Fixpoint::States "const
cmf::math::StateVariableVector& States() const ";

/*  model time  */

%feature("docstring")  cmf::math::Gears_Fixpoint::ModelTime "cmf::math::Time ModelTime() const

Returns the current model time. ";

%feature("docstring")  cmf::math::Gears_Fixpoint::ModelTime "void
ModelTime(cmf::math::Time val)

Sets the current model time. ";

%feature("docstring")  cmf::math::Gears_Fixpoint::TimeStep "cmf::math::Time TimeStep() const

Returns the last time step. ";

%feature("docstring")  cmf::math::Gears_Fixpoint::NextTimeStep "cmf::math::Time NextTimeStep() const

Returns the next time step width. ";

%feature("docstring")  cmf::math::Gears_Fixpoint::NextTimeStep "void
NextTimeStep(cmf::math::Time val)

Sets the next time step width. ";

%feature("docstring")  cmf::math::Gears_Fixpoint::MinTimestep "const
cmf::math::Time MinTimestep() const

Returns The minimal allowed time step length. ";

%feature("docstring")  cmf::math::Gears_Fixpoint::Iterations "int
Iterations() const ";

%feature("docstring")  cmf::math::Gears_Fixpoint::ResetIterations "void ResetIterations() ";

/*  Integrate  */

%feature("docstring")  cmf::math::Gears_Fixpoint::Integrate "int
Integrate(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::Gears_Fixpoint::IntegrateUntil "void IntegrateUntil(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::Gears_Fixpoint::IntegrateUntil "void IntegrateUntil(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables until MaxTime. ";

%feature("docstring")  cmf::math::Gears_Fixpoint::__call__ "void
__call__(cmf::math::Time until)

Calls IntegrateUntil. ";

/*  The state variables to integrate  */

/*  Accuracy parameters  */

%feature("docstring")  cmf::math::Gears_Fixpoint::AddStatesFromOwner "void AddStatesFromOwner(cmf::math::StateVariableOwner &stateOwner)

Adds the state variables of a StateVariableOwner to the state
variables of the solver. ";

%feature("docstring")  cmf::math::Gears_Fixpoint::Gears_Fixpoint "Gears_Fixpoint(const StateVariableVector &states, real epsilon=1e-9,
cmf::math::Time tStepMin=Time::Seconds(10), int maxOrder=4)

Constructs a new Gears_Fixpoint from a pointer to a vector of state
variables.

The Integrator becomes the owner of states

Parameters:
-----------

states:  Statevariables of the system

epsilon:  relative error tolerance per time step (default=1e-9)

tStepMin:  minimum time step (default=10s)

maxOrder:  Maximum order of method (default=4) ";

%feature("docstring")  cmf::math::Gears_Fixpoint::Gears_Fixpoint "Gears_Fixpoint(real epsilon=1e-9, cmf::math::Time
tStepMin=Time::Seconds(10), int maxOrder=4)

Constructs a new Gears_Fixpoint.

Parameters:
-----------

epsilon:  relative error tolerance per time step (default=1e-9)

tStepMin:  minimum time step (default=10s)

maxOrder:  Maximum order of method (default=4) ";

%feature("docstring")  cmf::math::Gears_Fixpoint::Gears_Fixpoint "Gears_Fixpoint(const Integrator &)

Copy constructor. ";

%feature("docstring")  cmf::math::Gears_Fixpoint::Copy "virtual
Integrator* Copy() const

Returns a new Integrator, based on this (without the state variables),
e.g. same type, epsilon, model time etc. ";

%feature("docstring")  cmf::math::Gears_Fixpoint::~Gears_Fixpoint "virtual ~Gears_Fixpoint() ";

%feature("docstring")  cmf::math::Gears_Fixpoint::Integrate "int
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

%feature("docstring")  cmf::math::Gears_Fixpoint::count "int count()
const

returns the number of state variables ";

%feature("docstring")  cmf::math::Gears_Fixpoint::state "const real&
state(int position) const

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::Gears_Fixpoint::state "void
state(int position, real newState)

Simplifies the assessment of state variables. ";


// File: a00613.xml
%feature("docstring") cmf::river::georeach "";

/*  Topological relation to other reaches  */

%feature("docstring")  cmf::river::georeach::Upstreams "cmf::river::reachvector& Upstreams()

Returns A collection of Reaches, directly contributing to this. ";

%feature("docstring")  cmf::river::georeach::Upstreams "const
cmf::river::reachvector& Upstreams() const

Returns A collection of Reaches, directly contributing to this. ";

%feature("docstring")  cmf::river::georeach::DownStream "cmf::river::reach* DownStream()

Returns cmf::river::reach* (DownStream). ";

/*  Overrides of FluxNode  */

%feature("docstring")  cmf::river::georeach::RecalcFluxes "virtual
bool RecalcFluxes(cmf::math::Time t)

Pure FluxNodes do not influence fluxes, therefore no recalculation of
fluxes is required by fluxnode. WaterStorage overrides this, since
state changes require an update of the fluxes. ";

%feature("docstring")  cmf::river::georeach::Empty "virtual bool
Empty() ";

%feature("docstring")  cmf::river::georeach::copy "virtual
WaterStorage* copy() const ";

%feature("docstring")  cmf::river::georeach::AddPart "void
AddPart(double x, double y, double z)

Adds a part o the reach. ";

%feature("docstring")  cmf::river::georeach::Line "const
cmf::geometry::Line& Line() const

Returns the vertices of the reach. ";

%feature("docstring")  cmf::river::georeach::georeach "georeach(cmf::geometry::point start, double hMin, ReachType
*typeOfReach=0)

Creates a root reach.

Parameters:
-----------

start:  Position of the first reach vertex

hMin:  Lowest height of the reach

typeOfReach:  the channel geometry ";

%feature("docstring")  cmf::river::georeach::georeach "georeach(reach
*downstream, cmf::geometry::point start, double hMin, ReachType
*typeOfReach=0)

Create a reach with a downstream connection.

Parameters:
-----------

downstream:  The downstream connected reach

start:  Position of the first vertex of the reach

hMin:  the height of the outlet reach

typeOfReach:  the channel geometry ";

%feature("docstring")  cmf::river::georeach::~georeach "virtual
~georeach() ";

%feature("docstring")  cmf::river::georeach::Type "const
cmf::river::ReachType& Type() const

Properties of the reach.

A const reference to the type of the reach ";

%feature("docstring")  cmf::river::georeach::Length "double Length()
const ";

%feature("docstring")  cmf::river::georeach::AddPart "void
AddPart(double val, double hMax) ";

%feature("docstring")  cmf::river::georeach::Slope "double Slope()
const

Returns the avg. slope of the reach. ";

%feature("docstring")  cmf::river::georeach::Volume "real Volume() ";

%feature("docstring")  cmf::river::georeach::GetQout "cmf::water::WaterFlux GetQout() ";

%feature("docstring")  cmf::river::georeach::Q_out "const
cmf::water::WaterFlux& Q_out(const cmf::math::Time &time) ";

%feature("docstring")  cmf::river::georeach::Q_in "const
cmf::water::WaterFlux& Q_in(const cmf::math::Time &time) ";

%feature("docstring")  cmf::river::georeach::Derivate "real
Derivate(const cmf::math::Time &time)

Returns the derivate of the state variable at time time. ";

%feature("docstring")  cmf::river::georeach::InsertReaches "void
InsertReaches(reachvector &target)

Inserts this and all upstream reaches into target. ";

%feature("docstring")  cmf::river::georeach::GetAllReaches "reachvector GetAllReaches()

returns a reachvector with all reaches (not only the root reaches) ";

%feature("docstring")  cmf::river::georeach::AddStateVariables "void
AddStateVariables(cmf::math::StateVariableVector &vector)

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";

%feature("docstring")  cmf::river::georeach::Solute "SoluteStorage&
Solute(const cmf::water::Solute &solute)

Returns the water quality of the water storage. ";

%feature("docstring")  cmf::river::georeach::Solute "const
SoluteStorage& Solute(const cmf::water::Solute &solute) const ";

%feature("docstring")  cmf::river::georeach::conc "real conc(const
cmf::water::Solute &solute) const

Returns the concentration of the given solute. ";

%feature("docstring")  cmf::river::georeach::conc "WaterQuality
conc(cmf::math::Time t)

Returns the current WaterQuality (concentration of all solutes). ";

%feature("docstring")  cmf::river::georeach::conc "void conc(const
cmf::water::Solute &solute, real NewConcetration)

Sets a new concentration. ";

%feature("docstring")  cmf::river::georeach::water "real water()
const

Returns the volume of water in this storage in m3. ";

%feature("docstring")  cmf::river::georeach::water "void water(real
newwatercontent)

Sets the volume of water in this storage in m3. ";

%feature("docstring")  cmf::river::georeach::ToString "virtual
std::string ToString() const ";

%feature("docstring")  cmf::river::georeach::State "const real&
State() const

Returns the current state of the variable. ";

%feature("docstring")  cmf::river::georeach::State "void State(const
real &newState)

Gives access to the state variable. ";

%feature("docstring")  cmf::river::georeach::StateIsChanged "bool
StateIsChanged()

Returns if the state was currently updated. ";

%feature("docstring")  cmf::river::georeach::ConnectionTo "cmf::water::FluxConnection& ConnectionTo(const FluxNode &target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::river::georeach::FluxTo "real
FluxTo(const FluxNode &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from target into this\"). ";

%feature("docstring")  cmf::river::georeach::Waterbalance "real
Waterbalance(cmf::math::Time t, const FluxConnection *except=0)

Returns the sum of all fluxes (positive and negative) at time t.
Single fluxes can be excluded from the calculation.

Parameters:
-----------

t:  Time of the query

except:  A FluxConnection that is excluded from the Waterbalance (e.g.
to prevent closed circuits) ";

%feature("docstring")  cmf::river::georeach::sum_inflow "cmf::water::WaterFlux sum_inflow(cmf::math::Time t, const
FluxConnection *except=0)

Returns the sum of the incoming (positive) fluxes and the mix of their
concentrations. ";

%feature("docstring")  cmf::river::georeach::sum_outflow "cmf::water::WaterFlux sum_outflow(cmf::math::Time t, const
FluxConnection *except=0)

Returns the sum of the incoming (positive) fluxes and their
concentrations. ";

%feature("docstring")  cmf::river::georeach::Potential "virtual real
Potential()

Returns the water potential of the node in m waterhead The base class
water storage always returns the height of the location. ";


// File: a00021.xml
%feature("docstring") cmf::upslope::vegetation::HargreaveVegetation "

A Vegetation class with real Temperature and Precipitation data, ETpot
by Hargreave method.

The step size of the time series has to be constant Values between
data points are linear interpolated ETpot=Hargreaves(T [deg
C],Latitude [deg])

Pthroughfall = 0.8 Pbulk

C++ includes: HargreaveVegetation.h ";

/*  Time dependent water flux information  */

%feature("docstring")
cmf::upslope::vegetation::HargreaveVegetation::GetThroughfall "virtual cmf::water::WaterFlux GetThroughfall(cmf::math::Time timestep)
const

Returns the Rate of precipitation entering the soil (precipitation -
interception) [mm/day], using CMFclassic approach. ";

%feature("docstring")
cmf::upslope::vegetation::HargreaveVegetation::GetETpot "virtual
double GetETpot(cmf::math::Time timestep) const

Returns the Evapotranspiration rate at a given time step [mm/day],
using hargreave equation. ";

%feature("docstring")
cmf::upslope::vegetation::HargreaveVegetation::GetTemperature "double
GetTemperature(cmf::math::Time timestep) const

Returns the air temperature at the timestep. ";

/*  Constructive methods  */

%feature("docstring")
cmf::upslope::vegetation::HargreaveVegetation::HargreaveVegetation "HargreaveVegetation(cmf::atmosphere::Meteorology *meteorology, double
refCropFactor=1, double rootdepth=0.5)

Creates a Vegetation with the Hargreave ETpot method.

Parameters:
-----------

meteorology:  The meteorological timeseries

refCropFactor:  Factor to scale ETpot to the landuse type. Factor of
one means reference crop (short grass)

rootdepth:  Depth of the rooted zone ";

%feature("docstring")
cmf::upslope::vegetation::HargreaveVegetation::copy "HargreaveVegetation* copy() const

Copies the vegetation. ";

/*  Root distribution  */

%feature("docstring")
cmf::upslope::vegetation::HargreaveVegetation::RootDepth "double
RootDepth() const

The depth of the rooted zone. ";

%feature("docstring")
cmf::upslope::vegetation::HargreaveVegetation::RootDepth "void
RootDepth(double val)

Sets the depth of the rooted zone. ";

%feature("docstring")
cmf::upslope::vegetation::HargreaveVegetation::RootDistribution "virtual double RootDistribution(double upperBoundary, double
lowerBoundary) const

Returns the relative amount of roots [0..1] in the given soil layer
This function assumes a uniform distribution of roots. ";

%feature("docstring")
cmf::upslope::vegetation::HargreaveVegetation::RootDistribution_Michealis_Menten
"virtual double RootDistribution_Michealis_Menten(double
upperBoundary, double lowerBoundary) const ";


// File: a00366.xml
%feature("docstring") cmf::upslope::HasCellReference "";

%feature("docstring")
cmf::upslope::HasCellReference::HasCellReference "HasCellReference(Cell &_cell) ";


// File: a00612.xml
%feature("docstring") cmf::upslope::snow::hbv_snow "";

%feature("docstring")  cmf::upslope::snow::hbv_snow::RecalcFluxes "virtual bool RecalcFluxes(cmf::math::Time t)

Pure FluxNodes do not influence fluxes, therefore no recalculation of
fluxes is required by fluxnode. WaterStorage overrides this, since
state changes require an update of the fluxes. ";

%feature("docstring")  cmf::upslope::snow::hbv_snow::ConnectionTo "cmf::water::FluxConnection& ConnectionTo(const FluxNode &target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::upslope::snow::hbv_snow::FluxTo "real
FluxTo(const FluxNode &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from target into this\"). ";

%feature("docstring")  cmf::upslope::snow::hbv_snow::Waterbalance "real Waterbalance(cmf::math::Time t, const FluxConnection *except=0)

Returns the sum of all fluxes (positive and negative) at time t.
Single fluxes can be excluded from the calculation.

Parameters:
-----------

t:  Time of the query

except:  A FluxConnection that is excluded from the Waterbalance (e.g.
to prevent closed circuits) ";

%feature("docstring")  cmf::upslope::snow::hbv_snow::sum_inflow "cmf::water::WaterFlux sum_inflow(cmf::math::Time t, const
FluxConnection *except=0)

Returns the sum of the incoming (positive) fluxes and the mix of their
concentrations. ";

%feature("docstring")  cmf::upslope::snow::hbv_snow::conc "virtual
cmf::water::WaterQuality conc(cmf::math::Time t)

Returns the water quality of the FluxNode, if it is not overridden
this is the mix of the incoming fluxes. ";

%feature("docstring")  cmf::upslope::snow::hbv_snow::sum_outflow "cmf::water::WaterFlux sum_outflow(cmf::math::Time t, const
FluxConnection *except=0)

Returns the sum of the incoming (positive) fluxes and their
concentrations. ";

%feature("docstring")  cmf::upslope::snow::hbv_snow::Potential "virtual real Potential()

Returns the water potential of the node in m waterhead The base class
water storage always returns the height of the location. ";

%feature("docstring")  cmf::upslope::snow::hbv_snow::Empty "virtual
bool Empty() ";

%feature("docstring")  cmf::upslope::snow::hbv_snow::copy "virtual
FluxNode* copy() const ";

%feature("docstring")  cmf::upslope::snow::hbv_snow::AddStateVariables
"virtual void AddStateVariables(cmf::math::StateVariableVector
&vector)=0

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";


// File: a00023.xml
%feature("docstring") cmf::geometry::Histogram "

Holds the histogram for a raster.

C++ includes: Raster.h ";

%feature("docstring")  cmf::geometry::Histogram::min "double min()
const

Returns the left side of the first histogram bar. ";

%feature("docstring")  cmf::geometry::Histogram::max "double max()
const

Returns the right side of the last histogram bar. ";

%feature("docstring")  cmf::geometry::Histogram::barwidth "double
barwidth() const

Returns the width of the histogram bars. ";

%feature("docstring")  cmf::geometry::Histogram::size "int size()
const

Returns the number of the historam bars. ";

%feature("docstring")  cmf::geometry::Histogram::sum "int sum(double
until=1e308) const

Returns the number of entries with a value <= until, default is the
total number of entries. ";

%feature("docstring")  cmf::geometry::Histogram::frequency "int
frequency(double val) const

Returns the frequency of values near val. ";

%feature("docstring")  cmf::geometry::Histogram::relfrequency "double
relfrequency(double val) const

Returns the relative frequency of values near val. ";

%feature("docstring")  cmf::geometry::Histogram::frequency "int
frequency(int pos) const

Returns the frequency of bar number pos. ";

%feature("docstring")  cmf::geometry::Histogram::relfrequency "double
relfrequency(int pos) const

Returns the relative frequency of bar number pos. ";

%feature("docstring")  cmf::geometry::Histogram::pos "int pos(double
val) const

Returns the bar number, that contains value val. ";

%feature("docstring")  cmf::geometry::Histogram::barcenter "double
barcenter(double pos) const

Returns the center of the bar at position pos. ";

%feature("docstring")  cmf::geometry::Histogram::quantile "double
quantile(double Quantile=0.5) const

Returns the inexact quantile. The result gets better with higher
numbers of bars Assumes the values of each bar uniformly distributed.
";

%feature("docstring")  cmf::geometry::Histogram::CountValue "void
CountValue(double val)

Counts a value in the histogram. ";

%feature("docstring")  cmf::geometry::Histogram::Histogram "Histogram(double _min, double _max, double _width)

Creates a histogram with bars ranging from _min to _max with a width
of width. ";

%feature("docstring")  cmf::geometry::Histogram::Histogram "Histogram(double _min, double _max, size_t _count)

Creates a histogram with count bars ranging from _min to _max. ";


// File: a00559.xml
%feature("docstring") std::ifstream "

STL class. ";


// File: a00024.xml
%feature("docstring") cmf::upslope::maps::IMap "

An interface, that provides a Get(x,y) function, that returns the
value or object at the given location (x,y) in planar coordinates, and
a Has(x,y) function the returns true, if an object exists at this
location The coordination system has to be the same as used for the
UpslopeCells, since no conversion between coordinate systems is
provided In Python the name resolves to IMap_* where * is one of the
following: Int, Float, Double, SoilType, SoilProfile, Vegetation,
Meteorology. The same convention applies to the following derived
classes.

C++ includes: maps.h ";

%feature("docstring")  cmf::upslope::maps::IMap::Get "_Ty
Get(cmf::geometry::point p)

Returns a soil type by calling the protected virtual function get. ";

%feature("docstring")  cmf::upslope::maps::IMap::Get "_Ty Get(double
x, double y)

Returns a soil type by calling the protected virtual function get. ";

%feature("docstring")  cmf::upslope::maps::IMap::Has "bool Has(double
x, double y) ";

%feature("docstring")  cmf::upslope::maps::IMap::Has "bool
Has(cmf::geometry::point p) ";

%feature("docstring")  cmf::upslope::maps::IMap::__call__ "_Ty
__call__(double x, double y) ";


// File: a00025.xml
%feature("docstring") cmf::math::ImplicitEuler "

An implicit (backward) Euler integrator using fixpoint iteration.

C++ includes: FixpointImplicitEuler.h ";

/*  Internal data storages  */

/*  Assessment of state variables for integration  */

%feature("docstring")  cmf::math::ImplicitEuler::States "cmf::math::StateVariableVector& States()

Returns the vector of StateVariable pointers. ";

%feature("docstring")  cmf::math::ImplicitEuler::States "const
cmf::math::StateVariableVector& States() const ";

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

/*  Integrate  */

%feature("docstring")  cmf::math::ImplicitEuler::Integrate "int
Integrate(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::ImplicitEuler::IntegrateUntil "void
IntegrateUntil(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::ImplicitEuler::IntegrateUntil "void
IntegrateUntil(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables until MaxTime. ";

%feature("docstring")  cmf::math::ImplicitEuler::__call__ "void
__call__(cmf::math::Time until)

Calls IntegrateUntil. ";

/*  The state variables to integrate  */

/*  Accuracy parameters  */

%feature("docstring")  cmf::math::ImplicitEuler::AddStatesFromOwner "void AddStatesFromOwner(cmf::math::StateVariableOwner &stateOwner)

Adds the state variables of a StateVariableOwner to the state
variables of the solver. ";

%feature("docstring")  cmf::math::ImplicitEuler::ImplicitEuler "ImplicitEuler(const StateVariableVector &states, real epsilon=1e-9,
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

%feature("docstring")  cmf::math::ImplicitEuler::state "const real&
state(int position) const

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::ImplicitEuler::state "void
state(int position, real newState)

Simplifies the assessment of state variables. ";


// File: a00026.xml
%feature("docstring") cmf::upslope::connections::Infiltration "

Connects the surfacewater and the unsaturated zone.

If UpslopeCell::InfiltrationExcess and Cell is not saturated \\\\[ K_I
= \\\\min\\\\left(\\\\frac{\\\\rho_{wg} \\\\Delta
z-\\\\Psi_M}{\\\\Delta z \\\\rho_{wg}}
K\\\\left(\\\\theta\\\\right),\\\\sum q_{surface\\\\ in}\\\\right)
\\\\] else \\\\[ K_I = \\\\sum q_{surface\\\\ in} \\\\]

C++ includes: FluxConnections.h ";

%feature("docstring")
cmf::upslope::connections::Infiltration::Infiltration "Infiltration(cmf::upslope::SoilWaterStorage &soilwater, FluxNode
&surfacewater) ";

%feature("docstring")  cmf::upslope::connections::Infiltration::q "real q(const FluxNode &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::connections::Infiltration::Target
"FluxNode& Target(const FluxNode &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::connections::Infiltration::Target
"const FluxNode& Target(const FluxNode &inquirer) const

Returns the other end of a connection than the asking end (const). ";

%feature("docstring")  cmf::upslope::connections::Infiltration::conc "WaterQuality conc(const FluxNode &inquirer, cmf::math::Time t)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";


// File: a00027.xml
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

/*  Assessment of state variables for integration  */

%feature("docstring")  cmf::math::Integrator::States "cmf::math::StateVariableVector& States()

Returns the vector of StateVariable pointers. ";

%feature("docstring")  cmf::math::Integrator::States "const
cmf::math::StateVariableVector& States() const ";

%feature("docstring")  cmf::math::Integrator::AddStatesFromOwner "virtual void AddStatesFromOwner(cmf::math::StateVariableOwner
&stateOwner)

Adds the state variables of a StateVariableOwner to the state
variables of the solver. ";

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

%feature("docstring")  cmf::math::Integrator::__call__ "void
__call__(cmf::math::Time until)

Calls IntegrateUntil. ";

%feature("docstring")  cmf::math::Integrator::count "int count()
const

returns the number of state variables ";

%feature("docstring")  cmf::math::Integrator::state "const real&
state(int position) const

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::Integrator::state "void state(int
position, real newState)

Simplifies the assessment of state variables. ";


// File: a00614.xml
%feature("docstring") std::invalid_argument "

STL class. ";


// File: a00564.xml
%feature("docstring") std::ios "

STL class. ";


// File: a00615.xml
%feature("docstring") std::ios_base "

STL class. ";


// File: a00605.xml
%feature("docstring") std::ios_base::failure "

STL class. ";


// File: a00569.xml
%feature("docstring") std::istream "

STL class. ";


// File: a00571.xml
%feature("docstring") std::istringstream "

STL class. ";


// File: a00028.xml
%feature("docstring") cmf::upslope::vegetation::IVegetation "

The abstract base class for all kinds of vegetations. Derivate classes
must override the GetETpot and GetThroughfall functions.

C++ includes: Vegetation.h ";

/*  Root distribution  */

%feature("docstring")
cmf::upslope::vegetation::IVegetation::RootDepth "double RootDepth()
const

The depth of the rooted zone. ";

%feature("docstring")
cmf::upslope::vegetation::IVegetation::RootDepth "void
RootDepth(double val)

Sets the depth of the rooted zone. ";

%feature("docstring")
cmf::upslope::vegetation::IVegetation::RootDistribution "virtual
double RootDistribution(double upperBoundary, double lowerBoundary)
const

Returns the relative amount of roots [0..1] in the given soil layer
This function assumes a uniform distribution of roots. ";

%feature("docstring")
cmf::upslope::vegetation::IVegetation::RootDistribution_Michealis_Menten
"virtual double RootDistribution_Michealis_Menten(double
upperBoundary, double lowerBoundary) const ";

%feature("docstring")  cmf::upslope::vegetation::IVegetation::GetETpot
"virtual double GetETpot(cmf::math::Time timestep) const =0

Returns the Evapotranspiration rate at a given time step [mm/day]. ";

%feature("docstring")
cmf::upslope::vegetation::IVegetation::GetThroughfall "virtual
cmf::water::WaterFlux GetThroughfall(cmf::math::Time timestep) const
=0

Returns the Rate of precipitation entering the soil (precipitation -
interception) [mm/day]. ";

%feature("docstring")
cmf::upslope::vegetation::IVegetation::GetTemperature "virtual double
GetTemperature(cmf::math::Time timestep) const =0

Returns the air temperature at the timestep. ";

%feature("docstring")  cmf::upslope::vegetation::IVegetation::copy "virtual IVegetation* copy() const =0

Pure virtual method, to copy the current vegetation. ";


// File: a00029.xml
%feature("docstring") cmf::math::Jacobian "

Calculates the jacobian of a system (a vector of state variables).

Code is inspired by RADAU5 method from Hairer E,Wanner G (1991)
Solving Ordinary Differential Equations II, Springer-Verlag

C++ includes: Jacobian.h ";

%feature("docstring")  cmf::math::Jacobian::Jacobian "Jacobian() ";

%feature("docstring")  cmf::math::Jacobian::Calculate "void
Calculate(StateVariableVector &vector, const cmf::math::Time
&timestep)

Calculates the jacobian for a given system at a given time step. ";

%feature("docstring")  cmf::math::Jacobian::ToFile "void
ToFile(std::string filename)

Saves the matrix in an undocumented file. Cols are tab seperated, rows
are ended with endl. ";

%feature("docstring")  cmf::math::Jacobian::~Jacobian "~Jacobian() ";


// File: a00593.xml
%feature("docstring") cmf::upslope::layered::LayeredCellBuilder "";

%feature("docstring")
cmf::upslope::layered::LayeredCellBuilder::LayeredCellBuilder "LayeredCellBuilder(cmf::upslope::maps::IMap<
cmf::upslope::vegetation::IVegetation * > *vegetationMap,
cmf::upslope::maps::IMap< cmf::upslope::SoilProfile > *soilMap,
cmf::upslope::maps::IMap< float > *soilDepthMap=0) ";

%feature("docstring")
cmf::upslope::layered::LayeredCellBuilder::CreateCell "cmf::upslope::UpslopeCell* CreateCell(cmf::geometry::point p, double
Area) const ";

%feature("docstring")
cmf::upslope::layered::LayeredCellBuilder::CreateCell "cmf::upslope::UpslopeCell* CreateCell(double x, double y, double z,
double Area) const ";


// File: a00616.xml
%feature("docstring") std::length_error "

STL class. ";


// File: a00617.xml
%feature("docstring") cmf::geometry::Line "";

%feature("docstring")  cmf::geometry::Line::Center "point Center()
const

returns the point in the middle of the line ";

%feature("docstring")  cmf::geometry::Line::PointOnLine "point
PointOnLine(double d) const

returns the point in the distance d from the start point on the line
";

%feature("docstring")  cmf::geometry::Line::Length "double Length()
const ";

%feature("docstring")  cmf::geometry::Line::AsWKT "std::string
AsWKT() const

returns the Well-Known-Text of aline with the vertices of this vector
";

%feature("docstring")  cmf::geometry::Line::AddPoint "void
AddPoint(point p)

Adds a point to the shape. ";

%feature("docstring")  cmf::geometry::Line::AddPoint "void
AddPoint(double x, double y, double z=0) ";

%feature("docstring")  cmf::geometry::Line::RemovePoint "void
RemovePoint(int pos) ";

%feature("docstring")  cmf::geometry::Line::size "int size() const ";

%feature("docstring")  cmf::geometry::Line::Vertex "point Vertex(int
i) const ";

%feature("docstring")  cmf::geometry::Line::Vertices "Points
Vertices() const ";

%feature("docstring")  cmf::geometry::Line::MaxStraight "Line
MaxStraight() const

Returns the longest straight line between two vertices of the shape.
";

%feature("docstring")  cmf::geometry::Line::boundingbox "BoundingBox
boundingbox() ";


// File: a00030.xml
%feature("docstring") cmf::water::reaction::LinearReaction "

A simple reaction with a constant linear change of the concentration.
If the constant range is $ 1\\\\frac{mol}{m^3\\\\ day}$ and the
rainfall deposition is 0, the concentration in each state equals the
time the water stayed in the system in days.

C++ includes: Reaction.h ";


// File: a00619.xml
%feature("docstring") std::list "

STL class. ";


// File: a00620.xml
%feature("docstring") std::list::const_iterator "

STL iterator class. ";


// File: a00621.xml
%feature("docstring") std::list::const_reverse_iterator "

STL iterator class. ";


// File: a00622.xml
%feature("docstring") std::list::iterator "

STL iterator class. ";


// File: a00623.xml
%feature("docstring") std::list::reverse_iterator "

STL iterator class. ";


// File: a00606.xml
%feature("docstring") std::logic_error "

STL class. ";


// File: a00625.xml
%feature("docstring") std::map "

STL class. ";


// File: a00626.xml
%feature("docstring") std::map::const_iterator "

STL iterator class. ";


// File: a00627.xml
%feature("docstring") std::map::const_reverse_iterator "

STL iterator class. ";


// File: a00628.xml
%feature("docstring") std::map::iterator "

STL iterator class. ";


// File: a00629.xml
%feature("docstring") std::map::reverse_iterator "

STL iterator class. ";


// File: a00031.xml
%feature("docstring") cmf::atmosphere::Meteorology "

In order to calculate ETpot with cmf a big amount of meteorological
data is needed, more data than usually available. The Meteorology
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
Meteorology class in the API documentationCreating a meteorological
station

import CMFlib as cmf                  latitude=51.2 # Latitude of
station in decimal degrees                  longitude=8.1 # Longitude
of station in decimal degrees (only needed for daily=false) timezone=1
# Timezone, pos. values mean east of GMT, negative west (Germany=1,
Pacific time=-8, only needed for daily=false) start=cmf.Time(1,1,2001)
# Creates all timeseries with this start time, one can change them
later                  step=cmf.day # s. start
name=\"Giessen\"           # A name for the station (optional)
meteo=cmf.Meteorology(latitude,longitude,timezone,start,step,name)

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

weather=meteo.GetData(cmf.Time(3,2,2001,14)) # Weather at Feb. 3rd,
2001, 2pm                  print 'Global Radiation: ',weather.Rs
# Daily mean Rs, since daily=true print 'Temperature:',weather.T
# Daily mean T, since nothing else in known

C++ includes: Meteorology.h ";

/*  Location and behaviour properties  */

/*  Additional properties  */

/*  Timeseries of meteorological data  */

%feature("docstring")  cmf::atmosphere::Meteorology::GetDeposition "cmf::water::WaterFlux GetDeposition(cmf::math::Time t)

Returns the rainfall with its current concentration. The concentration
is derived from Deposition. ";

/*  Data access methods  */

%feature("docstring")  cmf::atmosphere::Meteorology::GetData "Weather
GetData(cmf::math::Time t, double height) const

Returns the current Atmosphere state. Uses default values for missing
timeseries. ";

%feature("docstring")  cmf::atmosphere::Meteorology::GetRs "double
GetRs(cmf::math::Time t, double height, double sunshine_fraction)
const

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
a.s.l. in }m \\\\\\\\ \\\\end{eqnarray*}. ";

%feature("docstring")  cmf::atmosphere::Meteorology::GetRn "double
GetRn(double albedo, double Rs, double Tmax, double Tmin, double T,
double e_a, double sunshine_fraction) const ";

%feature("docstring")  cmf::atmosphere::Meteorology::GetRn "double
GetRn(double albedo, Weather w)

Returns the net radiation flux at a site,
seehttp://www.fao.org/docrep/X0490E/x0490e07.htm#radiation
\\\\begin{eqnarray*} R_{ns} &=& (1-\\\\alpha)R_s \\\\mbox{ Net
shortwave radiation in } \\\\frac{MJ}{m^2 day} \\\\\\\\ && \\\\alpha =
\\\\mbox{ Albedo} \\\\\\\\ \\\\sigma &=& 4.903\\\\ 10^{-9}
\\\\frac{MJ}{m^2 K^4 day} \\\\mbox{ Stefan-Boltzmann constant}\\\\\\\\
\\\\sigma T^4 &=&
\\\\sigma\\\\left((T_{max}+273.16)^4+(T_{min}+273.16)^4\\\\right)
\\\\mbox{ if daily} \\\\\\\\ \\\\sigma T^4 &=& \\\\sigma
\\\\left(T+273.16\\\\right)^4 \\\\mbox{ if hourly} \\\\\\\\
R_{nl}&=&\\\\sigma(T) \\\\left(0.34-0.14\\\\sqrt{e_a}\\\\right)
\\\\left(0.1+0.9\\\\frac{n}{N}\\\\right) \\\\\\\\ R_n&=& R_{ns}-R_{nl}
\\\\\\\\ \\\\end{eqnarray*}.

Parameters:
-----------

albedo:  Albedo of the surface $\\\\alpha$

w:  A Weather structure provided from GetData ";

%feature("docstring")
cmf::atmosphere::Meteorology::SetSunshineFraction "void
SetSunshineFraction(cmf::math::timeseries sunshine_duration) const

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
n\\\\mbox{Absolute sunshine duration} \\\\end{eqnarray*}. ";

%feature("docstring")  cmf::atmosphere::Meteorology::Pressure "static
double Pressure(double height)

Calculates the mean pressure for a specific height. ";

/*  I/O  */

%feature("docstring")  cmf::atmosphere::Meteorology::Save "void
Save(const std::string &filename)

Save the Meteorology data to an ASCII File with fixed format(
uses cmf::math::timeseries::Save ). ";

%feature("docstring")  cmf::atmosphere::Meteorology::Meteorology "Meteorology(const std::string &filename)

Creates a Meterology from a File in fixed format (uses
cmf::math::timeseries::timeseries(std::istream&) ). ";

%feature("docstring")  cmf::atmosphere::Meteorology::Meteorology "Meteorology(double latitude=51, double longitude=8, double timezone=1,
double elevation=0, cmf::math::Time startTime=cmf::math::Time(1, 1,
2001), cmf::math::Time timestep=cmf::math::day, std::string name=\"\")

Constructor of the Atmosphere object.

Parameters:
-----------

latitude:  Latitude of the study area (for solar radiation)

longitude:  Longitude of the study area (for solar time)

timezone:  Time zone of the study area (e.g Germany +1,U.S. Pacific
time -8

elevation:  Height of the meterological station above sea level [m]

startTime:  Date of the beginning of the climatic data (may be changed
for each time series later)

timestep:  Frequency of climatic data (may be changed for each time
series later)

name:  Name of the station ";


// File: a00032.xml
%feature("docstring") cmf::water::reaction::MichaelisMentenReaction "

A Michaelis Menten reaction without inhibitors.

C++ includes: Reaction.h ";

%feature("docstring")
cmf::water::reaction::MichaelisMentenReaction::ReactiveFlux "virtual
double ReactiveFlux(const WaterQuality &Environment)

Reactive flux: $ \\\\frac{d[X]}{dt}=\\\\frac{v_{max}\\\\ [X]}{K_m+[X]}
$. ";


// File: a00630.xml
%feature("docstring") std::multimap "

STL class. ";


// File: a00631.xml
%feature("docstring") std::multimap::const_iterator "

STL iterator class. ";


// File: a00632.xml
%feature("docstring") std::multimap::const_reverse_iterator "

STL iterator class. ";


// File: a00633.xml
%feature("docstring") std::multimap::iterator "

STL iterator class. ";


// File: a00634.xml
%feature("docstring") std::multimap::reverse_iterator "

STL iterator class. ";


// File: a00618.xml
%feature("docstring") cmf::geometry::MultiPoint "";

%feature("docstring")  cmf::geometry::MultiPoint::AddPoint "void
AddPoint(point p)

Adds a point to the shape. ";

%feature("docstring")  cmf::geometry::MultiPoint::AddPoint "void
AddPoint(double x, double y, double z=0) ";

%feature("docstring")  cmf::geometry::MultiPoint::RemovePoint "void
RemovePoint(int pos) ";

%feature("docstring")  cmf::geometry::MultiPoint::size "int size()
const ";

%feature("docstring")  cmf::geometry::MultiPoint::Vertex "point
Vertex(int i) const ";

%feature("docstring")  cmf::geometry::MultiPoint::Vertices "Points
Vertices() const ";

%feature("docstring")  cmf::geometry::MultiPoint::Center "point
Center() const ";

%feature("docstring")  cmf::geometry::MultiPoint::MaxStraight "Line
MaxStraight() const

Returns the longest straight line between two vertices of the shape.
";

%feature("docstring")  cmf::geometry::MultiPoint::AsWKT "std::string
AsWKT() const ";

%feature("docstring")  cmf::geometry::MultiPoint::boundingbox "BoundingBox boundingbox() ";

%feature("docstring")  cmf::geometry::MultiPoint::MultiPoint "MultiPoint() ";

%feature("docstring")  cmf::geometry::MultiPoint::MultiPoint "MultiPoint(const MultiPoint &copy) ";

%feature("docstring")  cmf::geometry::MultiPoint::MultiPoint "MultiPoint(const Points &copy) ";


// File: a00636.xml
%feature("docstring") std::multiset "

STL class. ";


// File: a00637.xml
%feature("docstring") std::multiset::const_iterator "

STL iterator class. ";


// File: a00638.xml
%feature("docstring") std::multiset::const_reverse_iterator "

STL iterator class. ";


// File: a00639.xml
%feature("docstring") std::multiset::iterator "

STL iterator class. ";


// File: a00640.xml
%feature("docstring") std::multiset::reverse_iterator "

STL iterator class. ";


// File: a00033.xml
%feature("docstring") cmf::upslope::maps::NearestNeighborMap "

Registers objects with locations and returns the nearest neighbor to
the queried location. Distance is measured as: $ d =
\\\\sqrt{(x1-x2)^2+(y1-y2)^2} + w_z|z1-z2| $ with wz is the weight
factor for height differences, zWeight. A high wz may be used for
contured object distribution, where $ \\\\sqrt{(x1-x2)^2+(y1-y2)^2} $
is small compared to $ w_z|z1-z2| $.

C++ includes: maps.h ";

%feature("docstring")  cmf::upslope::maps::NearestNeighborMap::Add "void Add(cmf::geometry::point p, _Ty value)

Adds a value at a location.

Parameters:
-----------

p:  location of value

value:  Value ";

%feature("docstring")  cmf::upslope::maps::NearestNeighborMap::Add "void Add(double x, double y, _Ty value)

Adds a value at a location.

Parameters:
-----------

x:  X-Value of location

y:  Y-Value of location

value:  Value ";

%feature("docstring")  cmf::upslope::maps::NearestNeighborMap::size "int size()

Returns the number of point value pairs in this map. ";

%feature("docstring")
cmf::upslope::maps::NearestNeighborMap::Position "cmf::geometry::point& Position(int index)

Returns the position of a value. ";

%feature("docstring")  cmf::upslope::maps::NearestNeighborMap::Value "_Ty Value(int index)

Returns one of the values. ";

%feature("docstring")
cmf::upslope::maps::NearestNeighborMap::NearestNeighborMap "NearestNeighborMap(double z_Weight=0) ";

%feature("docstring")  cmf::upslope::maps::NearestNeighborMap::Get "_Ty Get(cmf::geometry::point p)

Returns a soil type by calling the protected virtual function get. ";

%feature("docstring")  cmf::upslope::maps::NearestNeighborMap::Get "_Ty Get(double x, double y)

Returns a soil type by calling the protected virtual function get. ";

%feature("docstring")  cmf::upslope::maps::NearestNeighborMap::Has "bool Has(double x, double y) ";

%feature("docstring")  cmf::upslope::maps::NearestNeighborMap::Has "bool Has(cmf::geometry::point p) ";

%feature("docstring")
cmf::upslope::maps::NearestNeighborMap::__call__ "_Ty __call__(double
x, double y) ";


// File: a00034.xml
%feature("docstring") cmf::upslope::Neighbor "

Contains a reference to a cell and the width of the flow between
neighboring cells.

C++ includes: UpslopeCell.h ";

%feature("docstring")  cmf::upslope::Neighbor::Neighbor "Neighbor(UpslopeCell *cell, double flowwidth) ";

%feature("docstring")  cmf::upslope::Neighbor::Neighbor "Neighbor(const Neighbor &copy) ";


// File: a00035.xml
%feature("docstring") cmf::math::NevilleInterpolator "

Interpolates between known values (of type numVector ) at given points
in time. For a polynom interpolation, temporary variables for the y
values are needed. These variables (vectors) will be allocated
beforehand in the constructor. If various interpolations for the same
number of points and the same value space are done, costly new
allocation of memory will be avoided.

C++ includes: neville_interpolator.h ";

%feature("docstring")
cmf::math::NevilleInterpolator::NevilleInterpolator "NevilleInterpolator(int NumberOfPoints, int VectorSize)

Reserves temporary space for the temporary vectors. ";

%feature("docstring")  cmf::math::NevilleInterpolator::Interpolate "void Interpolate(const numVector *y[], const Time x[], Time t, int n,
numVector &_result)

Interpolates the missing values at position t from the n values y[] at
n positions x[] and copies them to _result No memory allocation needed
(despite a bit in the stack) if the sizes of the vectors and the
number of points are equal to the last call. ";

%feature("docstring")  cmf::math::NevilleInterpolator::Interpolate "numVector Interpolate(const numVector *y[], const Time x[], Time t,
int n)

Interpolates the missing values at position t from the n values y[] at
n positions x[] and returns them. ";


// File: a00036.xml
%feature("docstring") cmf::math::numVector "

A valarray kind of vector implementation with OpenMP capabilities.

C++ includes: numVector.h ";

/*  Constructors & Destructors  */

%feature("docstring")  cmf::math::numVector::numVector "numVector(int
count, real Value=0)

Creates a new vector with size count.

Parameters:
-----------

count:  size of new vector

Value:  Value of each element (default=0) ";

%feature("docstring")  cmf::math::numVector::numVector "numVector(size_t count, real Value=0) ";

%feature("docstring")  cmf::math::numVector::numVector "numVector()

Creates a vector of size 1 with value {0}. ";

%feature("docstring")  cmf::math::numVector::numVector "numVector(const numVector &Vector)

Copy constructor. ";

%feature("docstring")  cmf::math::numVector::numVector "numVector(const std::valarray< real > &Vector)

Convert from valarray<real>. ";

%feature("docstring")  cmf::math::numVector::~numVector "~numVector()

Destructor. ";

/*  Operators  */

/* Binary operators defined as free operators:

x = {+,-,*,/}

Defined for (x is one of the operators above): vector = vector x
vector

vector = real x vector

vector = vector x real

*/

%feature("docstring")  cmf::math::numVector::power "numVector
power(const numVector &) const

Return each element to the power of each element of vector. ";

%feature("docstring")  cmf::math::numVector::power "numVector
power(real exponent) const

Return each element to the power \"exponent\". ";

%feature("docstring")  cmf::math::numVector::apply "numVector
apply(real funct(real)) const

Return funct(this).

Parameters:
-----------

funct:  a function of signature real funct(real)

Example: Return the sinus of each element of vector x ";

/*  Summarizing functions  */

%feature("docstring")  cmf::math::numVector::dot "real dot(const
numVector &) const

Return dot product of this and vector. ";

%feature("docstring")  cmf::math::numVector::sum "real sum() const

Return sum of vector. ";

%feature("docstring")  cmf::math::numVector::max "real max() const

Return max of vector. ";

%feature("docstring")  cmf::math::numVector::min "real min() const

Return min of vector. ";

%feature("docstring")  cmf::math::numVector::norm "real norm(int
normtype=0) const

Returns a norm of the vector, implemented norms: 1 - sum of abs, 2 -
Euclidean distance, 0 Maximum of abs.

Parameters:
-----------

normtype:  An integer indicating the type of norm ";

%feature("docstring")  cmf::math::numVector::size "int size() const

Size of the vector. ";

%feature("docstring")  cmf::math::numVector::resize "void resize(int
count)

Changes the size of the vector. ";

%feature("docstring")  cmf::math::numVector::resize "void
resize(size_t count) ";


// File: a00574.xml
%feature("docstring") std::ofstream "

STL class. ";


// File: a00577.xml
%feature("docstring") std::ostream "

STL class. ";


// File: a00579.xml
%feature("docstring") std::ostringstream "

STL class. ";


// File: a00624.xml
%feature("docstring") std::out_of_range "

STL class. ";


// File: a00037.xml
%feature("docstring") cmf::upslope::connections::OutOfSoilConnection "

Calculates the flow between a soil layer and any FluxNode using
Richards equation. It uses the same formula as RichardsConnection but
the conductivity is determined by the soil side only.

C++ includes: FluxConnections.h ";

%feature("docstring")
cmf::upslope::connections::OutOfSoilConnection::q "real q(const
FluxNode &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::OutOfSoilConnection::Target "FluxNode&
Target(const FluxNode &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::OutOfSoilConnection::Target "const
FluxNode& Target(const FluxNode &inquirer) const

Returns the other end of a connection than the asking end (const). ";

%feature("docstring")
cmf::upslope::connections::OutOfSoilConnection::conc "WaterQuality
conc(const FluxNode &inquirer, cmf::math::Time t)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";


// File: a00641.xml
%feature("docstring") std::overflow_error "

STL class. ";


// File: a00038.xml
%feature("docstring") cmf::upslope::ET::PenmanMonteithET "

Calculates the potential evapotranspiration according to FAO(1998)
\\\\begin{eqnarray*} \\\\lambda ET &=& \\\\frac{\\\\Delta\\\\left(R_n
- G\\\\right)+\\\\rho_a c_p \\\\frac{e_s - e_a}{r_a}}{\\\\Delta +
\\\\gamma\\\\left(1+\\\\frac{r_s}{r_a}\\\\right)} \\\\mbox{ FAO 1998,
Eq. 3} \\\\\\\\ \\\\mbox{With:} \\\\\\\\ \\\\Delta &=& 4098
\\\\frac{0.6108 e^{17.27 T}}{(T+237.3)^2} \\\\frac{kPa}{^\\\\circ C}
\\\\mbox{ (FAO 1998, Eq. 13): Slope of vapor pressure } \\\\\\\\ T &=&
\\\\mbox{Actual Temperature in } ^\\\\circ C \\\\\\\\ R_n &=&
\\\\mbox{net Radiation (see Atmosphere) in } \\\\frac{MJ}{m^2day}
\\\\\\\\ G &=& 0 \\\\ \\\\frac{MJ}{m^2day} \\\\mbox{ if daily average
(FAO 1998, Eq. 42)} \\\\\\\\ && 0.1 R_n \\\\ \\\\mbox{ if day time
(FAO 1998, Eq. 45)} \\\\\\\\ && 0.5 R_n \\\\ \\\\mbox{ if night time
(FAO 1998, Eq. 46)} \\\\\\\\ \\\\gamma &=& \\\\frac{c_p P}{\\\\epsilon
\\\\lambda} \\\\mbox{ (FAO 1998,Eq. 8): Psychrometric constant }
\\\\frac{kPa}{^\\\\circ C} \\\\\\\\ c_p &=& 0.001013
\\\\frac{MJ}{kg\\\\ ^\\\\circ C}\\\\mbox{ specific heat at constant
pressure } \\\\\\\\ P &=& 101.3
\\\\left(\\\\frac{293-0.0065z}{293}\\\\right)^{5.26} \\\\mbox{ (FAO
1998,Eq. 7): Mean pressure kPa as a function of elevation above sea
level in m} \\\\\\\\ \\\\epsilon &=& 0.622 \\\\mbox{ ratio molecular
weight of water vapour/dry air} \\\\\\\\ \\\\lambda &=& 2.45
\\\\frac{MJ}{kg} \\\\mbox{ (FAO 1998,Eq. 8): latent heat of
vaporization} \\\\\\\\ R &=& 0.287 \\\\frac{kJ}{kg\\\\ k}\\\\mbox{
Specific gas constant } \\\\\\\\ \\\\rho_a &=&
\\\\frac{P}{1.01(T+273)R} \\\\mbox{ (FAO 1998,Box. 6): Mean air
density at constant pressure} \\\\\\\\ e_s &=& \\\\mbox{ Saturated
vapor pressure (see Atmosphere) in } kPa \\\\\\\\ e_a &=& \\\\mbox{
Actual vapor pressure (see Atmosphere) in } kPa \\\\\\\\ r_a &=&
\\\\frac{\\\\ln\\\\left(\\\\frac{2-d}{z_{om}}\\\\right)\\\\ln\\\\left(\\\\frac{2-d}{z_{oh}}\\\\right)}{k^2
u_2} \\\\mbox{ (FAO 1998, Eq. 4/Box 4): Aerodynamic resitance in }
\\\\frac s m \\\\\\\\ && d=\\\\frac 2 3 h,z_{om}=0.123 h,z_{oh}=0.1
z_{om}, k=0.41 \\\\\\\\ h &=& \\\\mbox{ Vegetation height in }m
\\\\\\\\ u_2 &=& \\\\mbox{ Windspeed in 2m above ground (see
Atmosphere) } \\\\frac m s \\\\\\\\ r_s &=&
\\\\frac{r_l}{LAI_{Active}} \\\\mbox{ (FAO 1998, Eq. 5/Box 5): bulk
surface resistance} \\\\frac s m \\\\\\\\ && r_l=100 \\\\frac s m,
LAI_{Active}=0.5 LAI \\\\end{eqnarray*}.

C++ includes: ET.h ";

%feature("docstring")  cmf::upslope::ET::PenmanMonteithET::GetETpot "real GetETpot(cmf::math::Time t) const ";

%feature("docstring")  cmf::upslope::ET::PenmanMonteithET::r_a "real
r_a(cmf::atmosphere::Weather A) const ";

%feature("docstring")  cmf::upslope::ET::PenmanMonteithET::r_s "real
r_s() const ";

%feature("docstring")
cmf::upslope::ET::PenmanMonteithET::PenmanMonteithET "PenmanMonteithET(cmf::upslope::SoilWaterStorage &source,
cmf::water::FluxNode &ET_target, cmf::upslope::vegetation::Vegetation
&VegetationAttributes, cmf::atmosphere::Meteorology &meteo) ";

%feature("docstring")  cmf::upslope::ET::PenmanMonteithET::q "real
q(const FluxNode &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::upslope::ET::PenmanMonteithET::Target "FluxNode& Target(const FluxNode &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::upslope::ET::PenmanMonteithET::Target "const FluxNode& Target(const FluxNode &inquirer) const

Returns the other end of a connection than the asking end (const). ";

%feature("docstring")  cmf::upslope::ET::PenmanMonteithET::conc "WaterQuality conc(const FluxNode &inquirer, cmf::math::Time t)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";


// File: a00039.xml
%feature("docstring")
cmf::upslope::vegetation::PenmanMonteithVegetation "

An implementation of IVegetation using the Penman-Monteith
transpiration model, according to: FAO (1998): Crop evapotranspiration
- Guidelines for computing crop water requirements - FAO Irrigation
and drainage paper 56 ISBN 92-5-104219-5,
URL:http://www.fao.org/docrep/X0490E/x0490e00.HTM.

C++ includes: PenmanMonteithVegetation.h ";

/*  Root distribution  */

%feature("docstring")
cmf::upslope::vegetation::PenmanMonteithVegetation::RootDepth "double
RootDepth() const

The depth of the rooted zone. ";

%feature("docstring")
cmf::upslope::vegetation::PenmanMonteithVegetation::RootDepth "void
RootDepth(double val)

Sets the depth of the rooted zone. ";

%feature("docstring")
cmf::upslope::vegetation::PenmanMonteithVegetation::RootDistribution "virtual double RootDistribution(double upperBoundary, double
lowerBoundary) const

Returns the relative amount of roots [0..1] in the given soil layer
This function assumes a uniform distribution of roots. ";

%feature("docstring")
cmf::upslope::vegetation::PenmanMonteithVegetation::RootDistribution_Michealis_Menten
"virtual double RootDistribution_Michealis_Menten(double
upperBoundary, double lowerBoundary) const ";

%feature("docstring")
cmf::upslope::vegetation::PenmanMonteithVegetation::PenmanMonteithVegetation
"PenmanMonteithVegetation(cmf::atmosphere::Meteorology *atm, double
rootdepth=1., double vegetationheight=0.12, double _LAI=2.88, double
Albedo=0.23, double StomatalResistance=100.0)

Constructs a Penman-Monteith-Vegetation. Prerequisite: An
cmf::atmosphere::Atmosphere.

Parameters:
-----------

atm:  Reference to atmospheric data

rootdepth:  Root depth

vegetationheight:  Height of the vegetation above ground in m

_LAI:  Leave area index in $ \\\\frac{m^2 \\\\mbox{leaves}}{m^2
\\\\mbox{ground}} $

Albedo:  Albedo of the vegetation

StomatalResistance:  Stomatal resistance in $ \\\\frac s m$ ";

%feature("docstring")
cmf::upslope::vegetation::PenmanMonteithVegetation::copy "PenmanMonteithVegetation* copy() const

Pure virtual method, to copy the current vegetation. ";

%feature("docstring")
cmf::upslope::vegetation::PenmanMonteithVegetation::GetAtmosphereData
"cmf::atmosphere::Weather GetAtmosphereData(cmf::math::Time t) ";

%feature("docstring")
cmf::upslope::vegetation::PenmanMonteithVegetation::GetAtmosphereData
"cmf::atmosphere::Weather GetAtmosphereData(cmf::math::Time t) const
";

%feature("docstring")
cmf::upslope::vegetation::PenmanMonteithVegetation::GetThroughfall "cmf::water::WaterFlux GetThroughfall(cmf::math::Time timestep) const

Returns the Rate of precipitation entering the soil (precipitation -
interception) [mm/day]. ";

%feature("docstring")
cmf::upslope::vegetation::PenmanMonteithVegetation::GetETpot "double
GetETpot(cmf::math::Time timestep) const

Calculates the potential evapotranspiration according to FAO(1998)
\\\\begin{eqnarray*} \\\\lambda ET &=& \\\\frac{\\\\Delta\\\\left(R_n
- G\\\\right)+\\\\rho_a c_p \\\\frac{e_s - e_a}{r_a}}{\\\\Delta +
\\\\gamma\\\\left(1+\\\\frac{r_s}{r_a}\\\\right)} \\\\mbox{ FAO 1998,
Eq. 3} \\\\\\\\ \\\\mbox{With:} \\\\\\\\ \\\\Delta &=& 4098
\\\\frac{0.6108 e^{17.27 T}}{(T+237.3)^2} \\\\frac{kPa}{^\\\\circ C}
\\\\mbox{ (FAO 1998, Eq. 13): Slope of vapor pressure } \\\\\\\\ T &=&
\\\\mbox{Actual Temperature in } ^\\\\circ C \\\\\\\\ R_n &=&
\\\\mbox{net Radiation (see Atmosphere) in } \\\\frac{MJ}{m^2day}
\\\\\\\\ G &=& 0 \\\\ \\\\frac{MJ}{m^2day} \\\\mbox{ if daily average
(FAO 1998, Eq. 42)} \\\\\\\\ && 0.1 R_n \\\\ \\\\mbox{ if day time
(FAO 1998, Eq. 45)} \\\\\\\\ && 0.5 R_n \\\\ \\\\mbox{ if night time
(FAO 1998, Eq. 46)} \\\\\\\\ \\\\gamma &=& \\\\frac{c_p P}{\\\\epsilon
\\\\lambda} \\\\mbox{ (FAO 1998,Eq. 8): Psychrometric constant }
\\\\frac{kPa}{^\\\\circ C} \\\\\\\\ c_p &=& 0.001013
\\\\frac{MJ}{kg\\\\ ^\\\\circ C}\\\\mbox{ specific heat at constant
pressure } \\\\\\\\ P &=& 101.3
\\\\left(\\\\frac{293-0.0065z}{293}\\\\right)^{5.26} \\\\mbox{ (FAO
1998,Eq. 7): Mean pressure kPa as a function of elevation above sea
level in m} \\\\\\\\ \\\\epsilon &=& 0.622 \\\\mbox{ ratio molecular
weight of water vapour/dry air} \\\\\\\\ \\\\lambda &=& 2.45
\\\\frac{MJ}{kg} \\\\mbox{ (FAO 1998,Eq. 8): latent heat of
vaporization} \\\\\\\\ R &=& 0.287 \\\\frac{kJ}{kg\\\\ K}\\\\mbox{
Specific gas constant } \\\\\\\\ \\\\rho_a &=&
\\\\frac{P}{1.01(T+273)R} \\\\mbox{ (FAO 1998,Box. 6): Mean air
density at constant pressure} \\\\\\\\ e_s &=& \\\\mbox{ Saturated
vapor pressure (see Atmosphere) in } kPa \\\\\\\\ e_a &=& \\\\mbox{
Actual vapor pressure (see Atmosphere) in } kPa \\\\\\\\ r_a &=&
\\\\frac{\\\\ln\\\\left(\\\\frac{2-d}{z_{om}}\\\\right)\\\\ln\\\\left(\\\\frac{2-d}{z_{oh}}\\\\right)}{k^2
u_2} \\\\mbox{ (FAO 1998, Eq. 4/Box 4): Aerodynamic resitance in }
\\\\frac s m \\\\\\\\ && d=\\\\frac 2 3 h,z_{om}=0.123 h,z_{oh}=0.1
z_{om}, k=0.41 \\\\\\\\ h &=& \\\\mbox{ Vegetation height in }m
\\\\\\\\ u_2 &=& \\\\mbox{ Windspeed in 2m above ground (see
Atmosphere) } \\\\frac m s \\\\\\\\ r_s &=&
\\\\frac{r_l}{LAI_{Active}} \\\\mbox{ (FAO 1998, Eq. 5/Box 5): bulk
surface resistance} \\\\frac s m \\\\\\\\ && r_l=100 \\\\frac s m,
LAI_{Active}=0.5 LAI \\\\end{eqnarray*}. ";

%feature("docstring")
cmf::upslope::vegetation::PenmanMonteithVegetation::GetTemperature "double GetTemperature(cmf::math::Time timestep) const

Returns the actual Temperature in deg C from the Meteorology. ";


// File: a00040.xml
%feature("docstring")
cmf::upslope::vegetation::PeriodicRainfallVegetation "

A periodic rainfall implementation of IVegetation.

C++ includes: Vegetation.h ";

/*  Root distribution  */

%feature("docstring")
cmf::upslope::vegetation::PeriodicRainfallVegetation::RootDepth "double RootDepth() const

The depth of the rooted zone. ";

%feature("docstring")
cmf::upslope::vegetation::PeriodicRainfallVegetation::RootDepth "void
RootDepth(double val)

Sets the depth of the rooted zone. ";

%feature("docstring")
cmf::upslope::vegetation::PeriodicRainfallVegetation::RootDistribution
"virtual double RootDistribution(double upperBoundary, double
lowerBoundary) const

Returns the relative amount of roots [0..1] in the given soil layer
This function assumes a uniform distribution of roots. ";

%feature("docstring")
cmf::upslope::vegetation::PeriodicRainfallVegetation::RootDistribution_Michealis_Menten
"virtual double RootDistribution_Michealis_Menten(double
upperBoundary, double lowerBoundary) const ";

%feature("docstring")
cmf::upslope::vegetation::PeriodicRainfallVegetation::copy "PeriodicRainfallVegetation* copy() const

Pure virtual method, to copy the current vegetation. ";

%feature("docstring")
cmf::upslope::vegetation::PeriodicRainfallVegetation::GetThroughfall "virtual cmf::water::WaterFlux GetThroughfall(cmf::math::Time timestep)
const

Returns the Rate of precipitation entering the soil (precipitation -
interception) [mm/day]. ";

%feature("docstring")
cmf::upslope::vegetation::PeriodicRainfallVegetation::GetETpot "virtual double GetETpot(cmf::math::Time timestep) const

Returns the Evapotranspiration rate at a given time step [mm/day]. ";

%feature("docstring")
cmf::upslope::vegetation::PeriodicRainfallVegetation::GetTemperature "virtual double GetTemperature(cmf::math::Time timestep) const

Returns the air temperature at the timestep. ";

%feature("docstring")
cmf::upslope::vegetation::PeriodicRainfallVegetation::PeriodicRainfallVegetation
"PeriodicRainfallVegetation(cmf::water::WaterFlux AvgThroughfall,
double AvgEtPot, double freq, double duration, double Temperature=10)

Creates a new Periodic Rainfall vegetation.

Parameters:
-----------

AvgThroughfall:  Average throughfall in mm/d

AvgEtPot:  Potential ET in mm/d

freq:  Length of period in d

duration:  Length of single rainfall event in d

Temperature:  Constant air temperature in deg C ";

%feature("docstring")
cmf::upslope::vegetation::PeriodicRainfallVegetation::PeriodicRainfallVegetation
"PeriodicRainfallVegetation(const PeriodicRainfallVegetation
&forcopy) ";


// File: a00041.xml
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
$\\\\sqrt{(this.x-p.x)^2+(this.y-p.y)^2}$. ";

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

%feature("docstring")  cmf::geometry::point::AsWKT "std::string
AsWKT() const ";

%feature("docstring")  cmf::geometry::point::Vertices "Points
Vertices() const ";

%feature("docstring")  cmf::geometry::point::Vertex "size_t Vertex()
const ";


// File: a00642.xml
%feature("docstring") cmf::geometry::Polygon "";

%feature("docstring")  cmf::geometry::Polygon::Area "double Area()
const ";

%feature("docstring")  cmf::geometry::Polygon::Center "cmf::geometry::point Center() const ";

%feature("docstring")  cmf::geometry::Polygon::size "size_t size()
const ";

%feature("docstring")  cmf::geometry::Polygon::AddPart "cmf::geometry::Ring& AddPart(const cmf::geometry::Ring &part) ";

%feature("docstring")  cmf::geometry::Polygon::AddPart "cmf::geometry::Ring& AddPart() ";

%feature("docstring")  cmf::geometry::Polygon::boundingbox "cmf::geometry::BoundingBox boundingbox() ";

%feature("docstring")  cmf::geometry::Polygon::Polygon "Polygon() ";

%feature("docstring")  cmf::geometry::Polygon::Polygon "Polygon(const
Polygon &copy) ";

%feature("docstring")  cmf::geometry::Polygon::Polygon "Polygon(const
cmf::geometry::Ring &copy) ";

%feature("docstring")  cmf::geometry::Polygon::Polygon "Polygon(const
cmf::geometry::BoundingBox &copy) ";

%feature("docstring")  cmf::geometry::Polygon::AsWKT "std::string
AsWKT() const ";

%feature("docstring")  cmf::geometry::Polygon::Includes "bool
Includes(cmf::geometry::point p) const ";

%feature("docstring")  cmf::geometry::Polygon::Includes "bool
Includes(cmf::geometry::point p)

returns true if p lies in the polygon defined by the points, and
creates the bounding box if needed

Algorithm by W. Randolph
Franklin,http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
";

%feature("docstring")  cmf::geometry::Polygon::Includes "bool
Includes(const cmf::geometry::shape &shp) const

Returns true if all vertices of mp are lying in the ring. ";

%feature("docstring")  cmf::geometry::Polygon::Includes "bool
Includes(const cmf::geometry::shape &shp) ";

%feature("docstring")  cmf::geometry::Polygon::Vertices "Points
Vertices() const ";


// File: a00042.xml
%feature("docstring") cmf::upslope::maps::PolygonMap "

Should return objects or values that are registered with a polygon, if
the queried point is in the polygon. This is not tested yet, nor
exists a really simple example. Happy Hacking!

C++ includes: maps.h ";

%feature("docstring")  cmf::upslope::maps::PolygonMap::PolygonMap "PolygonMap(_Ty nodata=_Ty())

Creates a new map based on polygon/value pairs.

Parameters:
-----------

nodata:  Value returned, if no polygon is found at the given location
";

%feature("docstring")  cmf::upslope::maps::PolygonMap::Add "void
Add(const cmf::geometry::Ring &polygon, _Ty value)

Adds a ring with a value Makes a copy of polygon and stores it in the
list of values. ";

%feature("docstring")  cmf::upslope::maps::PolygonMap::size "int
size()

Returns the number of point value pairs in this map. ";

%feature("docstring")  cmf::upslope::maps::PolygonMap::Position "cmf::geometry::Ring& Position(int index)

Returns the polygon of a value. ";

%feature("docstring")  cmf::upslope::maps::PolygonMap::Value "_Ty
Value(int index)

Returns one of the values. ";

%feature("docstring")  cmf::upslope::maps::PolygonMap::Get "_Ty
Get(cmf::geometry::point p)

Returns a soil type by calling the protected virtual function get. ";

%feature("docstring")  cmf::upslope::maps::PolygonMap::Get "_Ty
Get(double x, double y)

Returns a soil type by calling the protected virtual function get. ";

%feature("docstring")  cmf::upslope::maps::PolygonMap::Has "bool
Has(double x, double y) ";

%feature("docstring")  cmf::upslope::maps::PolygonMap::Has "bool
Has(cmf::geometry::point p) ";

%feature("docstring")  cmf::upslope::maps::PolygonMap::__call__ "_Ty
__call__(double x, double y) ";


// File: a00643.xml
%feature("docstring") cmf::geometry::PolyLine "";

%feature("docstring")  cmf::geometry::PolyLine::Length "double
Length() const ";

%feature("docstring")  cmf::geometry::PolyLine::Center "point
Center() const ";

%feature("docstring")  cmf::geometry::PolyLine::size "size_t size()
const ";

%feature("docstring")  cmf::geometry::PolyLine::AddPart "cmf::geometry::Line& AddPart(const cmf::geometry::Line &part) ";

%feature("docstring")  cmf::geometry::PolyLine::AddPart "cmf::geometry::Line& AddPart() ";

%feature("docstring")  cmf::geometry::PolyLine::boundingbox "cmf::geometry::BoundingBox boundingbox() ";

%feature("docstring")  cmf::geometry::PolyLine::straight_parts "cmf::geometry::PolyLine straight_parts() const ";

%feature("docstring")  cmf::geometry::PolyLine::PolyLine "PolyLine()
";

%feature("docstring")  cmf::geometry::PolyLine::PolyLine "PolyLine(const PolyLine &copy) ";

%feature("docstring")  cmf::geometry::PolyLine::PolyLine "PolyLine(const Line &copy) ";

%feature("docstring")  cmf::geometry::PolyLine::Vertices "cmf::geometry::Points Vertices() const ";

%feature("docstring")  cmf::geometry::PolyLine::AsWKT "std::string
AsWKT() const ";


// File: a00043.xml
%feature("docstring") cmf::atmosphere::Precipitation "

A connector that puts the rainfall to a FluxNode.

C++ includes: Precipitation.h ";

%feature("docstring")  cmf::atmosphere::Precipitation::Precipitation "Precipitation(PrecipitationData &rainfall, FluxNode &target, double
targetArea=1.0) ";

%feature("docstring")  cmf::atmosphere::Precipitation::q "real
q(const FluxNode &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::atmosphere::Precipitation::Target "FluxNode& Target(const FluxNode &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::atmosphere::Precipitation::Target "const
FluxNode& Target(const FluxNode &inquirer) const

Returns the other end of a connection than the asking end (const). ";

%feature("docstring")  cmf::atmosphere::Precipitation::conc "WaterQuality conc(const FluxNode &inquirer, cmf::math::Time t)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";


// File: a00044.xml
%feature("docstring") cmf::atmosphere::PrecipitationData "

Holds the precipitation and concentration timeseries.

C++ includes: Precipitation.h ";

%feature("docstring")
cmf::atmosphere::PrecipitationData::RecalcFluxes "virtual bool
RecalcFluxes(t)

Returns true, meaning: always use the actual data. ";

%feature("docstring")  cmf::atmosphere::PrecipitationData::conc "virtual cmf::water::WaterQuality conc(cmf::math::Time t)

Returns the concentration of the rainfall at time t. ";

%feature("docstring")
cmf::atmosphere::PrecipitationData::PrecipitationData "PrecipitationData(cmf::math::timeseries data)

Conversion constructor, taking a timeseries. ";

%feature("docstring")
cmf::atmosphere::PrecipitationData::RecalcFluxes "virtual bool
RecalcFluxes(cmf::math::Time t)

Pure FluxNodes do not influence fluxes, therefore no recalculation of
fluxes is required by fluxnode. WaterStorage overrides this, since
state changes require an update of the fluxes. ";

%feature("docstring")
cmf::atmosphere::PrecipitationData::ConnectionTo "cmf::water::FluxConnection& ConnectionTo(const FluxNode &target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::atmosphere::PrecipitationData::FluxTo "real FluxTo(const FluxNode &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from target into this\"). ";

%feature("docstring")
cmf::atmosphere::PrecipitationData::Waterbalance "real
Waterbalance(cmf::math::Time t, const FluxConnection *except=0)

Returns the sum of all fluxes (positive and negative) at time t.
Single fluxes can be excluded from the calculation.

Parameters:
-----------

t:  Time of the query

except:  A FluxConnection that is excluded from the Waterbalance (e.g.
to prevent closed circuits) ";

%feature("docstring")  cmf::atmosphere::PrecipitationData::sum_inflow
"cmf::water::WaterFlux sum_inflow(cmf::math::Time t, const
FluxConnection *except=0)

Returns the sum of the incoming (positive) fluxes and the mix of their
concentrations. ";

%feature("docstring")  cmf::atmosphere::PrecipitationData::sum_outflow
"cmf::water::WaterFlux sum_outflow(cmf::math::Time t, const
FluxConnection *except=0)

Returns the sum of the incoming (positive) fluxes and their
concentrations. ";

%feature("docstring")  cmf::atmosphere::PrecipitationData::Potential "virtual real Potential()

Returns the water potential of the node in m waterhead The base class
water storage always returns the height of the location. ";

%feature("docstring")  cmf::atmosphere::PrecipitationData::Empty "virtual bool Empty() ";

%feature("docstring")  cmf::atmosphere::PrecipitationData::copy "virtual FluxNode* copy() const ";


// File: a00644.xml
%feature("docstring") std::priority_queue "

STL class. ";


// File: a00645.xml
%feature("docstring") std::queue "

STL class. ";


// File: a00609.xml
%feature("docstring") cmf::atmosphere::Rainfall "";

%feature("docstring")  cmf::atmosphere::Rainfall::Precipitation "Precipitation(PrecipitationData &rainfall, FluxNode &target,
cmf::geometry::point Location, const cmf::atmosphere::Meteorology
&meteo, double targetArea=1.0) ";

%feature("docstring")  cmf::atmosphere::Rainfall::q "real q(const
FluxNode &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::atmosphere::Rainfall::Target "FluxNode&
Target(const FluxNode &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::atmosphere::Rainfall::Target "const
FluxNode& Target(const FluxNode &inquirer) const

Returns the other end of a connection than the asking end (const). ";

%feature("docstring")  cmf::atmosphere::Rainfall::conc "WaterQuality
conc(const FluxNode &inquirer, cmf::math::Time t)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";


// File: a00646.xml
%feature("docstring") std::range_error "

STL class. ";


// File: a00045.xml
%feature("docstring") cmf::geometry::Raster "

Represents a raster dataset.

C++ includes: Raster.h ";

/*  Focal functions  */

%feature("docstring")  cmf::geometry::Raster::focal_min "Raster<rasterType> focal_min(int n=3)

Creates a raster, which contains for each cell the minimum of the
surrounding n x n window. ";

%feature("docstring")  cmf::geometry::Raster::focal_max "Raster<rasterType> focal_max(int n=3)

Creates a raster, which contains for each cell the maximum of the
surrounding n x n window. ";

%feature("docstring")  cmf::geometry::Raster::focal_mean "Raster<rasterType> focal_mean(int n=3)

Creates a raster, which contains for each cell the mean of the
surrounding n x n window. ";

%feature("docstring")  cmf::geometry::Raster::focal_stdev "Raster<rasterType> focal_stdev(int n=3)

Creates a raster, which contains for each cell the standard deviation
of the surrounding n x n window. ";

%feature("docstring")  cmf::geometry::Raster::focal_majority "Raster<rasterType> focal_majority(int n=3)

Creates a raster, which contains for each cell the majority value of
the surrounding n x n window (usually only used for integer raster).
";

%feature("docstring")  cmf::geometry::Raster::focal_mean_difference "Raster<rasterType> focal_mean_difference(int n=3)

Creates a raster, which contains for each cell the difference between
the actual value and the mean of the surrounding n x n window (usually
only used for integer raster) This function can be used to identify
very important points (VIP) for triangulation. ";

/*  Metadata  */

%feature("docstring")  cmf::geometry::Raster::Xllcorner "double
Xllcorner() const

x-Coordinate for the Lower Left corner (in cellsize units) ";

%feature("docstring")  cmf::geometry::Raster::Yllcorner "double
Yllcorner() const

y-Coordinate for the Lower Left corner (in cellsize units) ";

%feature("docstring")  cmf::geometry::Raster::XCellsize "double
XCellsize() const

Cellsize of the raster. ";

%feature("docstring")  cmf::geometry::Raster::YCellsize "double
YCellsize() const

Cellsize of the raster. ";

%feature("docstring")  cmf::geometry::Raster::ColumnCount "int
ColumnCount() const

Number of columns in the raster. ";

%feature("docstring")  cmf::geometry::Raster::RowCount "int
RowCount() const

Number of rows in the raster. ";

%feature("docstring")  cmf::geometry::Raster::Width "double Width()
const

Extent W-E in cellsize units. ";

%feature("docstring")  cmf::geometry::Raster::Height "double Height()
const

Extent N-S in cellsize units. ";

%feature("docstring")  cmf::geometry::Raster::NoData "rasterType
NoData() const

Returns rasterType (NoData). ";

%feature("docstring")  cmf::geometry::Raster::GetBounds "BoundingBox
GetBounds() const

Returns the bounding box of the raster. ";

/*  Data access  */

%feature("docstring")  cmf::geometry::Raster::IdentifyXY "rasterType
IdentifyXY(double x, double y) const

Returns the value of the raster at the given position. ";

%feature("docstring")  cmf::geometry::Raster::IdentifyRowCol "rasterType IdentifyRowCol(int row, int col) const

Returns the value of the raster at the given cell. ";

%feature("docstring")  cmf::geometry::Raster::IdentifyColRow "rasterType IdentifyColRow(int col, int row) const

Returns the value of the raster at the given cell. ";

%feature("docstring")  cmf::geometry::Raster::HasData "bool
HasData(double x, double y) const

Returns true if there is data at the given position. ";

%feature("docstring")  cmf::geometry::Raster::HasData "bool
HasData(int col, int row) const

Returns true if there is data at the given position. ";

%feature("docstring")  cmf::geometry::Raster::SetData "void
SetData(double x, double y, rasterType val)

Sets a value at the given position.

Parameters:
-----------

x:  X-coordinate

y:  Y-coordinate

val:  Value to set at (x,y) ";

%feature("docstring")  cmf::geometry::Raster::SetData "void
SetData(int col, int row, rasterType val)

Sets a value at the given position.

Parameters:
-----------

col:  Column of the raster

row:  Row of the raster

val:  Value to set at (col,row) ";

/*  Analysis  */

%feature("docstring")  cmf::geometry::Raster::statistics "RasterStatistics statistics()

Creates statistics for the raster. ";

%feature("docstring")  cmf::geometry::Raster::histogram "Histogram
histogram(size_t bins=100) ";

/*  Operators  */

/*  Constructors & IO-Methods  */

%feature("docstring")  cmf::geometry::Raster::Raster "Raster(int
ncols, int nrows, double xllcorner, double yllcorner, double
xcellsize, double ycellsize, rasterType nodata, rasterType
initialValue=0)

Creates an empty Raster dataset. ";

%feature("docstring")  cmf::geometry::Raster::Raster "Raster(const
Raster< rasterType > &R)

Copy constructor. ";

%feature("docstring")  cmf::geometry::Raster::Raster "Raster(const
Raster< rasterType > &R, rasterType FixedValue)

Copy constructor, creates an empty raster dataset with the same
spatial properties like the input raster. ";

%feature("docstring")  cmf::geometry::Raster::Raster "Raster(const
std::string &FileName)

Builds a new Rasterdataset and passes the ownership of the dataset to
the Raster. No external reference to the dataset should be used. Loads
an ESRI ASCII-raster data set. ";

%feature("docstring")  cmf::geometry::Raster::Raster "Raster(std::istream &ASCFile)

Loads an ESRI ASCII-raster data set. ";

%feature("docstring")  cmf::geometry::Raster::WriteToASCFile "void
WriteToASCFile(std::ostream &ASCFile)

Writes the raster to a stream in ESRI-ASC format. ";

%feature("docstring")  cmf::geometry::Raster::WriteToASCFile "void
WriteToASCFile(std::string filename)

Writes the raster to a filename. ";

%feature("docstring")  cmf::geometry::Raster::WriteToBinary "void
WriteToBinary(std::string filename) const

Writes the data to the file with the given file name and the header to
a filename with the extension .hdr.

If the filename has the extension .flt and the raster is a float
raster the saved file can be read by ArcGIS ";

/*  Conversion functions  */

%feature("docstring")  cmf::geometry::Raster::ToInt "Raster<int>
ToInt() const

Converts the raster to a raster of int. ";

%feature("docstring")  cmf::geometry::Raster::ToFloat "Raster<float>
ToFloat()

Converts the raster to a raster of float (32bit). ";

%feature("docstring")  cmf::geometry::Raster::ToDouble "Raster<double> ToDouble()

Converts the raster to a raster of float (64bit). ";

%feature("docstring")  cmf::geometry::Raster::GetXPosition "double
GetXPosition(int col)

Returns the real world x position of a column. ";

%feature("docstring")  cmf::geometry::Raster::GetYPosition "double
GetYPosition(int row)

Returns the real world y position of a row. ";

%feature("docstring")  cmf::geometry::Raster::__len__ "int __len__()
";

%feature("docstring")  cmf::geometry::Raster::__getitem__ "rasterType
__getitem__(int index) ";


// File: a00022.xml


// File: a00647.xml


// File: a00046.xml
%feature("docstring") cmf::geometry::RasterStatistics "

Holds the statistics for a raster.

C++ includes: Raster.h ";

%feature("docstring")
cmf::geometry::RasterStatistics::RasterStatistics "RasterStatistics()

Count of cells with data. ";


// File: a00047.xml
%feature("docstring") cmf::water::reaction::RateReaction "

A simple reaction with a constant reaction rate.

C++ includes: Reaction.h ";


// File: a00048.xml
%feature("docstring") cmf::river::reach "

An alternative reach calculation approach (will get the only one)
\\\\begin{eqnarray*} \\\\frac{dV}{dt} &=& \\\\sum_{i=1}^{U}(Q_{in,i})
+ Q_{lat} - Q_{Manning}(V) \\\\end{eqnarray*}.

C++ includes: reach.h ";

/*  Topological relation to other reaches  */

%feature("docstring")  cmf::river::reach::Upstreams "cmf::river::reachvector& Upstreams()

Returns A collection of Reaches, directly contributing to this. ";

%feature("docstring")  cmf::river::reach::Upstreams "const
cmf::river::reachvector& Upstreams() const

Returns A collection of Reaches, directly contributing to this. ";

%feature("docstring")  cmf::river::reach::DownStream "cmf::river::reach* DownStream()

Returns cmf::river::reach* (DownStream). ";

/*  Overrides of FluxNode  */

%feature("docstring")  cmf::river::reach::RecalcFluxes "virtual bool
RecalcFluxes(cmf::math::Time t)

Pure FluxNodes do not influence fluxes, therefore no recalculation of
fluxes is required by fluxnode. WaterStorage overrides this, since
state changes require an update of the fluxes. ";

%feature("docstring")  cmf::river::reach::Empty "virtual bool Empty()
";

%feature("docstring")  cmf::river::reach::copy "virtual WaterStorage*
copy() const ";

%feature("docstring")  cmf::river::reach::Type "const
cmf::river::ReachType& Type() const

Properties of the reach.

A const reference to the type of the reach ";

%feature("docstring")  cmf::river::reach::Length "double Length()
const ";

%feature("docstring")  cmf::river::reach::AddPart "void
AddPart(double val, double hMax) ";

%feature("docstring")  cmf::river::reach::Slope "double Slope() const

Returns the avg. slope of the reach. ";

%feature("docstring")  cmf::river::reach::Volume "real Volume() ";

%feature("docstring")  cmf::river::reach::GetQout "cmf::water::WaterFlux GetQout() ";

%feature("docstring")  cmf::river::reach::Q_out "const
cmf::water::WaterFlux& Q_out(const cmf::math::Time &time) ";

%feature("docstring")  cmf::river::reach::Q_in "const
cmf::water::WaterFlux& Q_in(const cmf::math::Time &time) ";

%feature("docstring")  cmf::river::reach::Derivate "real
Derivate(const cmf::math::Time &time)

Returns the derivate of the state variable at time time. ";

%feature("docstring")  cmf::river::reach::InsertReaches "void
InsertReaches(reachvector &target)

Inserts this and all upstream reaches into target. ";

%feature("docstring")  cmf::river::reach::GetAllReaches "reachvector
GetAllReaches()

returns a reachvector with all reaches (not only the root reaches) ";

%feature("docstring")  cmf::river::reach::reach "reach(double hMin,
double length=0, cmf::river::ReachType *typeOfReach=0) ";

%feature("docstring")  cmf::river::reach::reach "reach(cmf::river::reach *downstream, double hMin, double length=0,
cmf::river::ReachType *typeOfReach=0) ";

%feature("docstring")  cmf::river::reach::AddStateVariables "void
AddStateVariables(cmf::math::StateVariableVector &vector)

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";

%feature("docstring")  cmf::river::reach::Solute "SoluteStorage&
Solute(const cmf::water::Solute &solute)

Returns the water quality of the water storage. ";

%feature("docstring")  cmf::river::reach::Solute "const
SoluteStorage& Solute(const cmf::water::Solute &solute) const ";

%feature("docstring")  cmf::river::reach::conc "real conc(const
cmf::water::Solute &solute) const

Returns the concentration of the given solute. ";

%feature("docstring")  cmf::river::reach::conc "WaterQuality
conc(cmf::math::Time t)

Returns the current WaterQuality (concentration of all solutes). ";

%feature("docstring")  cmf::river::reach::conc "void conc(const
cmf::water::Solute &solute, real NewConcetration)

Sets a new concentration. ";

%feature("docstring")  cmf::river::reach::water "real water() const

Returns the volume of water in this storage in m3. ";

%feature("docstring")  cmf::river::reach::water "void water(real
newwatercontent)

Sets the volume of water in this storage in m3. ";

%feature("docstring")  cmf::river::reach::ToString "virtual
std::string ToString() const ";

%feature("docstring")  cmf::river::reach::State "const real& State()
const

Returns the current state of the variable. ";

%feature("docstring")  cmf::river::reach::State "void State(const
real &newState)

Gives access to the state variable. ";

%feature("docstring")  cmf::river::reach::StateIsChanged "bool
StateIsChanged()

Returns if the state was currently updated. ";

%feature("docstring")  cmf::river::reach::ConnectionTo "cmf::water::FluxConnection& ConnectionTo(const FluxNode &target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::river::reach::FluxTo "real FluxTo(const
FluxNode &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from target into this\"). ";

%feature("docstring")  cmf::river::reach::Waterbalance "real
Waterbalance(cmf::math::Time t, const FluxConnection *except=0)

Returns the sum of all fluxes (positive and negative) at time t.
Single fluxes can be excluded from the calculation.

Parameters:
-----------

t:  Time of the query

except:  A FluxConnection that is excluded from the Waterbalance (e.g.
to prevent closed circuits) ";

%feature("docstring")  cmf::river::reach::sum_inflow "cmf::water::WaterFlux sum_inflow(cmf::math::Time t, const
FluxConnection *except=0)

Returns the sum of the incoming (positive) fluxes and the mix of their
concentrations. ";

%feature("docstring")  cmf::river::reach::sum_outflow "cmf::water::WaterFlux sum_outflow(cmf::math::Time t, const
FluxConnection *except=0)

Returns the sum of the incoming (positive) fluxes and their
concentrations. ";

%feature("docstring")  cmf::river::reach::Potential "virtual real
Potential()

Returns the water potential of the node in m waterhead The base class
water storage always returns the height of the location. ";


// File: a00049.xml
%feature("docstring") cmf::river::ReachType "

Structure for the description of structural parameters of a reach
Abstract base class for different channel geometries.

C++ includes: ReachType.h ";

%feature("docstring")  cmf::river::ReachType::Width "virtual double
Width(double depth) const =0

Calculates the flow width from a given actual depth [m] using the
actual channel geometry. ";

%feature("docstring")  cmf::river::ReachType::Perimeter "virtual
double Perimeter(double depth) const =0

Calculates the wetted perimeter from a given actual depth [m] using
the actual channel geometry. ";

%feature("docstring")  cmf::river::ReachType::Depth "virtual double
Depth(double area) const =0

Calculates the actual depth of the reach using the channel geometry.

Depth of the reach [m]

Parameters:
-----------

area:  Wetted area of a river cross section [m2], can be obtained by
V/l, where V is the stored volume and l is the reach length ";

%feature("docstring")  cmf::river::ReachType::Area "virtual double
Area(double depth) const =0

Calculates the wetted area from a given depth using the channel
geometry. In most cases use Area=V/l, where V is the stored volume and
l is the reach length.

Wetted area of a river cross section [m2]

Parameters:
-----------

depth:  Depth of the reach [m] ";

%feature("docstring")  cmf::river::ReachType::qManning "double
qManning(double A, double slope) const

Calculates the flow rate from a given water volume in the reach
\\\\begin{eqnarray*} q_{Manning}&=& A R^{\\\\frac 23}
\\\\sqrt{\\\\frac {\\\\Delta_z} n} \\\\\\\\ A &=& \\\\frac V l
\\\\mbox{ Area of the wetted crossection, Volume per length}\\\\\\\\
R&=&\\\\frac A P(d) \\\\\\\\ P(d)&=& \\\\mbox{ Perimeter of the wetted
crossection, a function of reach depth} \\\\\\\\ d(V)&=& \\\\mbox{
Depth of the reach} \\\\\\\\ \\\\Delta_z&=& \\\\frac{z_{max} -
z_{min}}{l} \\\\mbox{ Slope of the reach} \\\\\\\\ \\\\end{eqnarray*}.

Flow rate [m3/s]

Parameters:
-----------

A:  The area of the cross section [m2]

slope:  The slope of the reach [m/m] ";

%feature("docstring")  cmf::river::ReachType::ReachType "ReachType()
";


// File: a00050.xml
%feature("docstring") cmf::water::reaction::Reaction "

An abstract type to use chemical (or other) reactions of solutes.
Override the ReactiveFlux function or use one of the child classes, if
appropriate (Have a look at the ExternalFunctionReaction, before you
start recompiling cmf).

C++ includes: Reaction.h ";

%feature("docstring")  cmf::water::reaction::Reaction::ReactiveFlux "virtual double ReactiveFlux(const WaterQuality &Environment)=0

Returns the reactive flux as $\\\\frac{d[X]}{dt}=f([A],[B]...)
\\\\left[\\\\frac{mol}{m^3\\\\ day}\\\\right]$. ";

%feature("docstring")  cmf::water::reaction::Reaction::Reaction "Reaction(const cmf::water::Solute &solute) ";


// File: a00648.xml
%feature("docstring") cmf::river::RectangularReach "";

%feature("docstring")  cmf::river::RectangularReach::Width "virtual
double Width(double depth) const

Returns the width of the stream at a given depth \\\\begin{eqnarray*}
w &=& const \\\\end{eqnarray*}. ";

%feature("docstring")  cmf::river::RectangularReach::Perimeter "virtual double Perimeter(double depth) const

Returns the wetted perimeter at a given depth \\\\begin{eqnarray*} P
&=& 2 d + w \\\\end{eqnarray*}. ";

%feature("docstring")  cmf::river::RectangularReach::Depth "virtual
double Depth(double area) const

Returns the depth at a given crossection area \\\\begin{eqnarray*} d
&=& \\\\frac A w \\\\end{eqnarray*}. ";

%feature("docstring")  cmf::river::RectangularReach::Area "virtual
double Area(double depth) const

Returns the crossection area at a given depth \\\\begin{eqnarray*} A
&=& d w \\\\end{eqnarray*}. ";

%feature("docstring")  cmf::river::RectangularReach::TriangularReach "TriangularReach(double width)

Creates a new rectangular reach type. ";

%feature("docstring")  cmf::river::RectangularReach::qManning "double
qManning(double A, double slope) const

Calculates the flow rate from a given water volume in the reach
\\\\begin{eqnarray*} q_{Manning}&=& A R^{\\\\frac 23}
\\\\sqrt{\\\\frac {\\\\Delta_z} n} \\\\\\\\ A &=& \\\\frac V l
\\\\mbox{ Area of the wetted crossection, Volume per length}\\\\\\\\
R&=&\\\\frac A P(d) \\\\\\\\ P(d)&=& \\\\mbox{ Perimeter of the wetted
crossection, a function of reach depth} \\\\\\\\ d(V)&=& \\\\mbox{
Depth of the reach} \\\\\\\\ \\\\Delta_z&=& \\\\frac{z_{max} -
z_{min}}{l} \\\\mbox{ Slope of the reach} \\\\\\\\ \\\\end{eqnarray*}.

Flow rate [m3/s]

Parameters:
-----------

A:  The area of the cross section [m2]

slope:  The slope of the reach [m/m] ";


// File: a00051.xml
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

%feature("docstring")  cmf::upslope::RetentionCurve::Porosity "virtual real Porosity(real depth) const =0

Returns the porosity at a certain depth. ";

%feature("docstring")  cmf::upslope::RetentionCurve::VoidVolume "virtual real VoidVolume(real upperDepth, real lowerDepth, real Area)
const =0

Returns the void volume of a soil column. ";

%feature("docstring")  cmf::upslope::RetentionCurve::FillHeight "virtual real FillHeight(real lowerDepth, real Area, real Volume) const
=0

Returns the thickness of a soil column with a certain pore volume. ";

%feature("docstring")  cmf::upslope::RetentionCurve::Transmissivity "virtual real Transmissivity(real upperDepth, real lowerDepth, real
theta) const =0

Returns the transmissivity of a part of a soil column. ";

%feature("docstring")  cmf::upslope::RetentionCurve::Wetness "virtual
real Wetness(real suction) const =0

returns the wetness (volumetric water content per pore space) at a
given suction pressure ";

%feature("docstring")  cmf::upslope::RetentionCurve::Wetness_pF "real
Wetness_pF(real pF) const

returns the volumetric water content at a given pF value ";

%feature("docstring")  cmf::upslope::RetentionCurve::MatricPotential "virtual real MatricPotential(real wetness) const =0

returns the wetness of the soil at given water content ";

%feature("docstring")  cmf::upslope::RetentionCurve::copy "virtual
RetentionCurve* copy() const =0 ";


// File: a00052.xml
%feature("docstring") cmf::upslope::connections::RichardsConnection "

Calculates flow according to the Richards equation.

\\\\begin{eqnarray*} q_{Richards} &=& \\\\frac{\\\\Delta\\\\Psi_tot}{d
\\\\rho_{wg}} K(\\\\theta) A \\\\\\\\ \\\\Psi_tot =
\\\\Psi_{M}(\\\\theta) + \\\\frac{h}{\\\\rho_{wg}} \\\\end{eqnarray*}
where  $ \\\\Delta\\\\Psi_tot $ is the difference of the total water
potentials of the two soil layers

$ d $ is the distance between the two soil layers

$ \\\\rho_{wg}=9810 \\\\frac{Pa}{m} $ is the constant density of water
times gravitational acceleration

$ K(\\\\theta)$ is the actual conductivity (see SoilType::Kunsat)

$ A $ is the crosssectional area of the flux

$ \\\\Psi_M(\\\\theta) $ is the matrix potential (see
SoilType::MatrixPotential)

$ h $ is the height of a soil layer above see level

C++ includes: FluxConnections.h ";

%feature("docstring")
cmf::upslope::connections::RichardsConnection::q "real q(const
FluxNode &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::RichardsConnection::Target "FluxNode&
Target(const FluxNode &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::RichardsConnection::Target "const
FluxNode& Target(const FluxNode &inquirer) const

Returns the other end of a connection than the asking end (const). ";

%feature("docstring")
cmf::upslope::connections::RichardsConnection::conc "WaterQuality
conc(const FluxNode &inquirer, cmf::math::Time t)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";


// File: a00635.xml
%feature("docstring") cmf::geometry::Ring "";

%feature("docstring")  cmf::geometry::Ring::AsWKT "std::string
AsWKT() const

returns the Well-Known-Text of a polygon with the vertices of this
ring ";

%feature("docstring")  cmf::geometry::Ring::Area "double Area() const

returns the Area of a polygon defined by the points ";

%feature("docstring")  cmf::geometry::Ring::Center "point Center()
const

returns the centroid of a polygon defined by the points ";

%feature("docstring")  cmf::geometry::Ring::Includes "bool
Includes(point p) const

returns true if p lies in the polygon defined by the points

Algorithm by W. Randolph
Franklin,http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
";

%feature("docstring")  cmf::geometry::Ring::Includes "bool
Includes(point p)

returns true if p lies in the polygon defined by the points, and
creates the bounding box if needed

Algorithm by W. Randolph
Franklin,http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
";

%feature("docstring")  cmf::geometry::Ring::Ring::Includes "bool
Ring::Includes(const shape &shp) const

Returns true if all vertices of mp are lying in the ring. ";

%feature("docstring")  cmf::geometry::Ring::Ring::Includes "bool
Ring::Includes(const shape &shp) ";

%feature("docstring")  cmf::geometry::Ring::SharedBoundary "Line
SharedBoundary(const Ring &with, double tolerance=0.1) const

Returns the shared boundary of two rings. ";

%feature("docstring")  cmf::geometry::Ring::Ring "Ring() ";

%feature("docstring")  cmf::geometry::Ring::Ring "Ring(const
cmf::geometry::Ring &copy) ";

%feature("docstring")  cmf::geometry::Ring::Ring "Ring(const
cmf::geometry::BoundingBox &bbox) ";

%feature("docstring")  cmf::geometry::Ring::Ring "Ring(const
cmf::geometry::Line &copy) ";

%feature("docstring")  cmf::geometry::Ring::AddPoint "void
AddPoint(point p)

Adds a point to the shape. ";

%feature("docstring")  cmf::geometry::Ring::AddPoint "void
AddPoint(double x, double y, double z=0) ";

%feature("docstring")  cmf::geometry::Ring::RemovePoint "void
RemovePoint(int pos) ";

%feature("docstring")  cmf::geometry::Ring::size "int size() const ";

%feature("docstring")  cmf::geometry::Ring::Vertex "point Vertex(int
i) const ";

%feature("docstring")  cmf::geometry::Ring::Vertices "Points
Vertices() const ";

%feature("docstring")  cmf::geometry::Ring::MaxStraight "Line
MaxStraight() const

Returns the longest straight line between two vertices of the shape.
";

%feature("docstring")  cmf::geometry::Ring::boundingbox "BoundingBox
boundingbox() ";


// File: a00053.xml
%feature("docstring") cmf::math::RKFIntegrator "

Integrates a vector of cmf::math::StateVariable with the Runge-Kutta-
Fehlberg (RKF54) method.

C++ includes: RKFIntegrator.h ";

/*  Assessment of state variables for integration  */

%feature("docstring")  cmf::math::RKFIntegrator::States "cmf::math::StateVariableVector& States()

Returns the vector of StateVariable pointers. ";

%feature("docstring")  cmf::math::RKFIntegrator::States "const
cmf::math::StateVariableVector& States() const ";

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

/*  Integrate  */

%feature("docstring")  cmf::math::RKFIntegrator::Integrate "int
Integrate(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::RKFIntegrator::IntegrateUntil "void
IntegrateUntil(cmf::math::Time MaxTime) ";

%feature("docstring")  cmf::math::RKFIntegrator::IntegrateUntil "void
IntegrateUntil(cmf::math::Time MaxTime, cmf::math::Time TimeStep)

Integrates the vector of state variables until MaxTime. ";

%feature("docstring")  cmf::math::RKFIntegrator::__call__ "void
__call__(cmf::math::Time until)

Calls IntegrateUntil. ";

/*  The state variables to integrate  */

/*  Accuracy parameters  */

%feature("docstring")  cmf::math::RKFIntegrator::AddStatesFromOwner "void AddStatesFromOwner(cmf::math::StateVariableOwner &stateOwner)

Adds the state variables of a StateVariableOwner to the state
variables of the solver. ";

%feature("docstring")  cmf::math::RKFIntegrator::RKFIntegrator "RKFIntegrator(const StateVariableVector &states, real epsilon=1e-9,
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

%feature("docstring")  cmf::math::RKFIntegrator::state "const real&
state(int position) const

Simplifies the assessment of state variables. ";

%feature("docstring")  cmf::math::RKFIntegrator::state "void
state(int position, real newState)

Simplifies the assessment of state variables. ";


// File: a00611.xml
%feature("docstring") cmf::water::RouteWaterBalanceConnection "";

%feature("docstring")
cmf::water::RouteWaterBalanceConnection::RouteWaterBalanceConnection "RouteWaterBalanceConnection(FluxNode &source, FluxNode &target) ";

%feature("docstring")  cmf::water::RouteWaterBalanceConnection::q "real q(const FluxNode &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::water::RouteWaterBalanceConnection::Target
"FluxNode& Target(const FluxNode &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::water::RouteWaterBalanceConnection::Target
"const FluxNode& Target(const FluxNode &inquirer) const

Returns the other end of a connection than the asking end (const). ";

%feature("docstring")  cmf::water::RouteWaterBalanceConnection::conc "WaterQuality conc(const FluxNode &inquirer, cmf::math::Time t)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";


// File: a00607.xml
%feature("docstring") std::runtime_error "

STL class. ";


// File: a00650.xml
%feature("docstring") std::set "

STL class. ";


// File: a00651.xml
%feature("docstring") std::set::const_iterator "

STL iterator class. ";


// File: a00652.xml
%feature("docstring") std::set::const_reverse_iterator "

STL iterator class. ";


// File: a00653.xml
%feature("docstring") std::set::iterator "

STL iterator class. ";


// File: a00654.xml
%feature("docstring") std::set::reverse_iterator "

STL iterator class. ";


// File: a00655.xml
%feature("docstring") cmf::geometry::shape "";

%feature("docstring")  cmf::geometry::shape::Vertices "virtual Points
Vertices() const =0 ";

%feature("docstring")  cmf::geometry::shape::Center "virtual point
Center() const =0 ";

%feature("docstring")  cmf::geometry::shape::AsWKT "virtual
std::string AsWKT() const =0

Returns the OGC well known text representation of the shape as defined
in OpenGIS Implementation Specification for Geographic information -
Simple feature access - Part 1: Common
architecturehttp://www.opengeospatial.org/standards/is. ";


// File: a00054.xml
%feature("docstring") cmf::upslope::vegetation::ShuttleworthWallace "

Calculates the sum of soil evaporation and transpiration according to
Shuttleworth & Wallace 1985, as implemented in BROOK 90 (Federer
1990).

The difference to BROOK90 is, that the actual transpiration is not
calculated by plant resitance and potential gradient between plant and
soil, but by an piecewise linear function of the pF value $ pF =
\\\\log_{10}\\\\left(-\\\\Psi [hPa]\\\\right) $: \\\\[
\\\\frac{T_{act}}{T_{pot}} = \\\\left\\\\{\\\\begin{array}{cl} 1 &
\\\\mbox{if $pF \\\\le 3.35$} \\\\\\\\ \\\\frac{pF - 4.2}{3.35 - 4.2}
& \\\\mbox{if $pF \\\\in [3.35 .. 4.2] $} \\\\\\\\ 0 & \\\\mbox{if $pF
\\\\ge 4.2$} \\\\end{array}\\\\right. \\\\]

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

Aerodynamic resistances $r_{ac},r_{as},r_{aa}$  $ r_{ac} = 100 n
\\\\frac{\\\\sqrt{\\\\frac {w_{leaf}}{u_h}}}{1-\\\\exp(-\\\\frac n 2)}
$

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


// File: a00656.xml


// File: a00055.xml
%feature("docstring") cmf::upslope::ET::ShuttleworthWallaceET "

Calculates the actual transpiration and the soil evaporation from a
soil layer.

C++ includes: ET.h ";

%feature("docstring")
cmf::upslope::ET::ShuttleworthWallaceET::ShuttleworthWallaceET "ShuttleworthWallaceET(cmf::upslope::SoilWaterStorage &source,
cmf::water::FluxNode &ET_target, cmf::atmosphere::Meteorology &meteo)
";


// File: a00056.xml
%feature("docstring") cmf::upslope::maps::SingleValueMap "

A SingleValueMap is the simplest possible implementation of the IMap
interface. the Get(x,y) function returns always the same value/object
no matter what location is given.

C++ includes: maps.h ";

%feature("docstring")
cmf::upslope::maps::SingleValueMap::SingleValueMap "SingleValueMap(_Ty value) ";

%feature("docstring")  cmf::upslope::maps::SingleValueMap::Get "_Ty
Get(cmf::geometry::point p)

Returns a soil type by calling the protected virtual function get. ";

%feature("docstring")  cmf::upslope::maps::SingleValueMap::Get "_Ty
Get(double x, double y)

Returns a soil type by calling the protected virtual function get. ";

%feature("docstring")  cmf::upslope::maps::SingleValueMap::Has "bool
Has(double x, double y) ";

%feature("docstring")  cmf::upslope::maps::SingleValueMap::Has "bool
Has(cmf::geometry::point p) ";

%feature("docstring")  cmf::upslope::maps::SingleValueMap::__call__ "_Ty __call__(double x, double y) ";


// File: a00610.xml
%feature("docstring") cmf::atmosphere::Snowfall "";

%feature("docstring")  cmf::atmosphere::Snowfall::Precipitation "Precipitation(PrecipitationData &rainfall, FluxNode &target,
cmf::geometry::point Location, const cmf::atmosphere::Meteorology
&meteo, double targetArea=1.0) ";

%feature("docstring")  cmf::atmosphere::Snowfall::q "real q(const
FluxNode &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")  cmf::atmosphere::Snowfall::Target "FluxNode&
Target(const FluxNode &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")  cmf::atmosphere::Snowfall::Target "const
FluxNode& Target(const FluxNode &inquirer) const

Returns the other end of a connection than the asking end (const). ";

%feature("docstring")  cmf::atmosphere::Snowfall::conc "WaterQuality
conc(const FluxNode &inquirer, cmf::math::Time t)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";


// File: a00057.xml
%feature("docstring") cmf::upslope::SoilProfile "

Holds a soil profile, with one SoilType fro each layer and the
vertical boundaries of the layers.

C++ includes: SoilProfile.h ";

%feature("docstring")  cmf::upslope::SoilProfile::AddLayer "void
AddLayer(double Ksat, double Phi, double Lambda, double FieldCapacity,
double WiltingPoint, double PLE, double LowerBoundary)

Adds a Layer to the soil profile.

Parameters:
-----------

Ksat:  Water conductivity of the soil at saturation $
\\\\left[\\\\frac{m}{day}\\\\right] $

Phi:  Fraction of pore volume per soil volume $ \\\\left[\\\\frac{m^3
Pores}{m^3 Soil}\\\\right] $

Lambda:  Lambda parameter for unsaturated flow []

FieldCapacity:  Water content at pF=1.5 $ \\\\left[\\\\frac{m^3
H_2O}{m^3 Soil}\\\\right] $

WiltingPoint:  Water content at pF=4.2 $ \\\\left[\\\\frac{m^3
H_2O}{m^3 Soil}\\\\right] $

PLE:  Power Law Exponent to calculate the saturated Transmissivity []

LowerBoundary:  Lower boundary of the layer $ [m] $ ";

%feature("docstring")  cmf::upslope::SoilProfile::AddLayer "void
AddLayer(SoilType soil, double LowerBoundary) ";

%feature("docstring")  cmf::upslope::SoilProfile::size "size_t size()
";

%feature("docstring")  cmf::upslope::SoilProfile::GetSoilProperties "SoilType& GetSoilProperties(size_t index) ";

%feature("docstring")  cmf::upslope::SoilProfile::GetLowerBoundary "double GetLowerBoundary(size_t index) ";

%feature("docstring")  cmf::upslope::SoilProfile::GetUpperBoundary "double GetUpperBoundary(size_t index) ";

%feature("docstring")  cmf::upslope::SoilProfile::GetThickness "double GetThickness(size_t index) ";

%feature("docstring")  cmf::upslope::SoilProfile::Average "SoilType
Average() ";


// File: a00058.xml
%feature("docstring") cmf::upslope::SoilType "

Soil description for CMF.

C++ includes: soil.h ";

/*  Description of Pores  */

%feature("docstring")  cmf::upslope::SoilType::PoreVolume "real
PoreVolume(real soildepth, real area) const

The total pore volume, exponential drain to include?

[m3]

Parameters:
-----------

soildepth:  Depth of porose soil [m]

area:  [m2] ";

/*  Parameters and methods for water flow  */

%feature("docstring")  cmf::upslope::SoilType::Kunsat "real
Kunsat(real theta) const

Returns the unsaturated conductivity of a soil, using Brooks-Corey
\\\\[ K_{unsat}(\\\\theta) = K_{sat} \\\\left(\\\\frac{\\\\theta -
\\\\theta_r}{\\\\theta_s - \\\\theta_r}\\\\right)^{3+2\\\\lambda^{-1}}
\\\\left[\\\\frac{m}{day}\\\\right] \\\\].

Parameters:
-----------

theta:   $ \\\\theta $ actual water content in $\\\\frac{m^3 H_2O}{m^3
\\\\mbox{Soil}}$

Might get extended through different Kunsat approaches. The approach
will be chosen by a static variable of soil ";

%feature("docstring")  cmf::upslope::SoilType::Kunsat2 "real
Kunsat2(real theta) const

Van Genuchten/Mualem $ \\\\theta - K_{sat} $ relationship \\\\[
K_{unsat} = K_{sat} \\\\sqrt{W}\\\\left(1-\\\\left(1-W^{\\\\frac 1
m}\\\\right)^m\\\\right)^2 \\\\]. ";

%feature("docstring")  cmf::upslope::SoilType::Transmissivity "real
Transmissivity(real z_sat, real z_soil) const

Calculates the transmissivity \\\\[ T(z_{sat},z_{soil}) = K_{sat}
z_{soil}\\\\left(1-\\\\frac{z_{sat}}{z_{soil}}\\\\right)^{PLE}
\\\\left[\\\\frac{m^2}{day}\\\\right] \\\\] Transmissivity of a layer
with upper and lower boundary $z_{upper},z_{lower}$ \\\\[
T_{layer}(z_{upper},z_{lower}) =
T\\\\left(\\\\max\\\\left(z_{upper},z_{sat}\\\\right),z_{soil}\\\\right)
- T\\\\left(z_{lower},z_{soil}\\\\right) \\\\].

Transmissivity

Parameters:
-----------

z_sat:   $ z_{sat} $ depth in m below ground of the saturated zone

z_soil:   $ z_{soil} $ total soil depth

Taken from Wigmosta et al 1999, but changed. Original eq.: $
T(z_{sat},z_{soil}) = K_{sat}
\\\\frac{z_{soil}}{PLE}\\\\left(1-\\\\frac{z_{sat}}{z_{soil}}\\\\right)^{PLE}
$ ";

%feature("docstring")  cmf::upslope::SoilType::Transmissivity "real
Transmissivity(real K, real z_upper, real z_lower, real z_soil) const

Calculates the transmissivity for a layer that is not necessarily
saturated \\\\[ T(z_{upper},z_{lower},z_{soil}) = K
\\\\left(z_{soil}\\\\left(1-\\\\frac{z_{upper}}{z_{soil}}\\\\right)^{PLE}
-
z_{soil}\\\\left(1-\\\\frac{z_{upper}}{z_{soil}}\\\\right)^{PLE}\\\\right)
\\\\].

Parameters:
-----------

K:  Conductivity (e.g. Ksat or Kunsat(theta)

z_upper:  Upper depth of the boundary (m)

z_lower:  Lower depth of the boundary (m)

z_soil:  Soil depth ";

/*  BROOK90 Parameters  */

%feature("docstring")  cmf::upslope::SoilType::Wetness "real
Wetness(real theta) const

Returns the wetness of a soil \\\\[ W(\\\\theta) =\\\\frac{\\\\theta -
\\\\theta_r}{\\\\theta_s - \\\\theta_r} \\\\]. ";

%feature("docstring")  cmf::upslope::SoilType::Wetness_inf "real
Wetness_inf() const

Wetness at dry end of near-saturation range for soil layer $
W_i=\\\\max\\\\left(0.92+\\\\frac {0.01} {\\\\sqrt{\\\\lambda}},
\\\\frac \\\\lambda {1+\\\\lambda}\\\\right) $. ";

%feature("docstring")  cmf::upslope::SoilType::b_exp "real b_exp()
const

Calculates the b exponent of the matric potential equation from two
known points on the $ \\\\theta(\\\\Psi) $ function
\\\\begin{eqnarray*} b &=&
\\\\frac{\\\\log\\\\left(\\\\frac{\\\\Psi_f}{\\\\Psi_w}\\\\right)}{\\\\log\\\\left(\\\\frac{\\\\theta_f}{\\\\theta_w}\\\\right)}
\\\\\\\\ \\\\Psi_f,\\\\Psi_w &=& \\\\mbox{ Matric potential at
fieldcapacity / wilting point } 10^{2.5}/10^{4.2} hPa \\\\\\\\
\\\\theta_f,\\\\theta_w &=& \\\\mbox{ Water content at fieldcapacity /
wilting point } \\\\end{eqnarray*}. ";

%feature("docstring")  cmf::upslope::SoilType::MatrixPotential "real
MatrixPotential(real theta) const

Matrix potential $\\\\Psi$ of the soil at a water content $\\\\theta$
in Pa (always negative) \\\\begin{eqnarray*} \\\\mbox{if }
W(\\\\theta)<W_i : \\\\\\\\ \\\\Psi &=& \\\\Psi_f\\\\left(\\\\frac
{W(\\\\theta)} {W(\\\\theta_f)} \\\\right)^{-b} \\\\\\\\ \\\\mbox{if }
W(\\\\theta) \\\\ge W_i : \\\\\\\\ \\\\Psi &=&-m (W - n)(1-W) \\\\\\\\
&& m = -\\\\frac{\\\\Psi_i}{(1-W_i)^2}-\\\\lambda \\\\frac {\\\\Psi_i}
{W_i (1-W_i)} \\\\\\\\ && n = 2 W_i - 1+ \\\\lambda \\\\frac
{\\\\Psi_i}{m} W_i \\\\\\\\ && \\\\Psi_i = \\\\Psi_f
\\\\left(\\\\frac{W_i}{W(\\\\theta_f)}\\\\right)^{2b+3} \\\\\\\\
\\\\end{eqnarray*}  $ W(\\\\theta) $ is defined in Wetness()

$ W_i=\\\\max\\\\left(0.92+\\\\frac {0.01} {\\\\sqrt{\\\\lambda}},
\\\\frac \\\\lambda {1+\\\\lambda}\\\\right) $ is defined in
Wetness_inf()

$\\\\Psi_f=- 10^{2.5} hPa = - 10^{4.5} Pa = -32000 Pa $.

Todo Precalculation of $ W_i,m,n,\\\\Psi_i,\\\\Psi_f$ for better
perfomance ";

/*  Constructors  */

%feature("docstring")  cmf::upslope::SoilType::SoilType "SoilType()

Creates a solid rock soil. ";

%feature("docstring")  cmf::upslope::SoilType::SoilType "SoilType(real ksat, real phi, real lambda, real fieldCap, real
wiltPoint, real ple, real stonefraction=0.0)

Complete Constructor.

Parameters:
-----------

ksat:  Water conductivity of the soil at saturation [m/day]

phi:  Fraction of pore volume per soil volume []

lambda:  Lambda parameter for unsaturated flow []

fieldCap:  Water content at pF=1.5 $ \\\\left[\\\\frac{m^3 H_2O}{m^3
Soil}\\\\right] $

wiltPoint:  Volume of Water/(Phi*Soilvolume) at pF=4.2 []

ple:  Power Law Exponent to calculate the saturated Transmissivity

stonefraction:  Fraction of stones in the soil [0..1] ";


// File: a00059.xml
%feature("docstring") cmf::upslope::SoilWaterStorage "

A representation of a Layer.

C++ includes: SoilWaterStorage.h ";

/*  Overrides of FluxNode  */

%feature("docstring")  cmf::upslope::SoilWaterStorage::RecalcFluxes "virtual bool RecalcFluxes(cmf::math::Time t)

Pure FluxNodes do not influence fluxes, therefore no recalculation of
fluxes is required by fluxnode. WaterStorage overrides this, since
state changes require an update of the fluxes. ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::Empty "virtual
bool Empty() ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::copy "virtual
WaterStorage* copy() const ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::UpperBoundary "virtual double UpperBoundary() const

Returns the upper boundary of the water storage below ground in m. ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::LowerBoundary "virtual double LowerBoundary() const

Returns the lower boundary of the water storage below ground in m. ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::Thickness "double Thickness() const ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::Soil "virtual
const RetentionCurve& Soil() const

Returns the soil properties of the water storage. ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::Soil "virtual
RetentionCurve& Soil() ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::theta "virtual
real theta() const

Returns the actual volumetric water content of the water storage. ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::theta "virtual
void theta(real Value) ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::K "double K()
const

Returns the actual conductivity $\\\\frac{m}{day}$. ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::Wetness "virtual double Wetness() const

Returns the wetness of the soil $ \\\\frac{V_{H_2O}}{V_{pores}} $. ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::MatrixPotential
"virtual real MatrixPotential() const

Calls RetentionCurve::Matrixpotential. ";

%feature("docstring")
cmf::upslope::SoilWaterStorage::GravitationalPotential "real
GravitationalPotential() const

Gravitational Potential in m, reference height is sea level \\\\[
\\\\Psi_G=h \\\\]. ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::Capacity "virtual real Capacity()

Returns the capacity of the water storage in m3. ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::SetPotential "void SetPotential(real waterhead)

Sets the potential of this soil water storage. ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::Potential "virtual real Potential() const

Returns the total potential in m \\\\[ \\\\Psi = \\\\Psi_M + \\\\Psi_G
\\\\]. ";

%feature("docstring")
cmf::upslope::SoilWaterStorage::SoilWaterStorage "SoilWaterStorage(cmf::upslope::Cell &_cell, real lowerboundary, const
RetentionCurve &r_curve, real saturateddepth=-10) ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::copy "SoilWaterStorage* copy() ";

%feature("docstring")
cmf::upslope::SoilWaterStorage::AddStateVariables "void
AddStateVariables(cmf::math::StateVariableVector &vector)

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::Solute "SoluteStorage& Solute(const cmf::water::Solute &solute)

Returns the water quality of the water storage. ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::Solute "const
SoluteStorage& Solute(const cmf::water::Solute &solute) const ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::conc "real
conc(const cmf::water::Solute &solute) const

Returns the concentration of the given solute. ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::conc "WaterQuality conc(cmf::math::Time t)

Returns the current WaterQuality (concentration of all solutes). ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::conc "void
conc(const cmf::water::Solute &solute, real NewConcetration)

Sets a new concentration. ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::water "real
water() const

Returns the volume of water in this storage in m3. ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::water "void
water(real newwatercontent)

Sets the volume of water in this storage in m3. ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::ToString "virtual std::string ToString() const ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::Derivate "virtual real Derivate(const cmf::math::Time &time)

Returns the derivate of the state variable at time time. ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::State "const
real& State() const

Returns the current state of the variable. ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::State "void
State(const real &newState)

Gives access to the state variable. ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::StateIsChanged
"bool StateIsChanged()

Returns if the state was currently updated. ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::ConnectionTo "cmf::water::FluxConnection& ConnectionTo(const FluxNode &target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::FluxTo "real
FluxTo(const FluxNode &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from target into this\"). ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::Waterbalance "real Waterbalance(cmf::math::Time t, const FluxConnection *except=0)

Returns the sum of all fluxes (positive and negative) at time t.
Single fluxes can be excluded from the calculation.

Parameters:
-----------

t:  Time of the query

except:  A FluxConnection that is excluded from the Waterbalance (e.g.
to prevent closed circuits) ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::sum_inflow "cmf::water::WaterFlux sum_inflow(cmf::math::Time t, const
FluxConnection *except=0)

Returns the sum of the incoming (positive) fluxes and the mix of their
concentrations. ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::sum_outflow "cmf::water::WaterFlux sum_outflow(cmf::math::Time t, const
FluxConnection *except=0)

Returns the sum of the incoming (positive) fluxes and their
concentrations. ";

%feature("docstring")  cmf::upslope::SoilWaterStorage::Potential "virtual real Potential()

Returns the water potential of the node in m waterhead The base class
water storage always returns the height of the location. ";


// File: a00060.xml
%feature("docstring") cmf::upslope::SolidRockCell "

A class for debugging the routing model, equals the ponded flow and
the inflow, equals solid rock.

C++ includes: SolidRockCell.h ";

/*  Spatial Properties.  */

/* Properties of UpslopeCell that are describing the spatial
distribution of the Cells

*/

%feature("docstring")  cmf::upslope::SolidRockCell::Area "double
Area() const

Area of UpslopeCell in m2. ";

%feature("docstring")  cmf::upslope::SolidRockCell::Center "cmf::geometry::point& Center()

Center of the Cell. ";

%feature("docstring")  cmf::upslope::SolidRockCell::Center "cmf::geometry::point Center() const ";

%feature("docstring")  cmf::upslope::SolidRockCell::Height "double
Height() const

Mean height of Cell a.s.l. ";

%feature("docstring")  cmf::upslope::SolidRockCell::GetSlopeTo "double GetSlopeTo(cmf::upslope::UpslopeCell &other) const

Calculates the topographic slope to another cell in m/m. ";

/*  Neighborhood.  */

/* Methods to handle and describe the topological relation to other
Cells

*/

%feature("docstring")  cmf::upslope::SolidRockCell::NeighborCount "int NeighborCount() const ";

%feature("docstring")  cmf::upslope::SolidRockCell::GetNeighbor "Neighbor& GetNeighbor(int index)

The Neighborhood of a cell, this acces is quite slow, in C++ use the
NeighborIterator for cycling.

Parameters:
-----------

index:  Index of the neighbor ";

%feature("docstring")  cmf::upslope::SolidRockCell::GetNeighbor "NeighborIterator GetNeighbor()

Returns an NeighborIterator for fast cycling (only C++ API). ";

%feature("docstring")  cmf::upslope::SolidRockCell::GetNeighbor "NeighborIterator GetNeighbor(const UpslopeCell &cell) ";

%feature("docstring")  cmf::upslope::SolidRockCell::AddNeighbor "bool
AddNeighbor(UpslopeCell &cell, double flowwidth)

Adds a Neighbor cell to the neighborhood.

True if cell was added, false if cell was already defiened as neighbor

Parameters:
-----------

cell:  Neighboring cell

flowwidth:  Lenght of intersection [m] ";

%feature("docstring")  cmf::upslope::SolidRockCell::IsNeighbor "bool
IsNeighbor(const UpslopeCell &cell)

Returns true if the given cell is a neighbor of this. ";

%feature("docstring")  cmf::upslope::SolidRockCell::RemoveNeighbor "void RemoveNeighbor(int index)

Removes a neighbor at an specified index. ";

%feature("docstring")  cmf::upslope::SolidRockCell::RemoveNeighbor "bool RemoveNeighbor(const UpslopeCell &cell)

Removes a specific cell. ";

%feature("docstring")  cmf::upslope::SolidRockCell::JoinCell "void
JoinCell(UpslopeCell &cell)

Creates a union from the given cell and this. Changes area and
neighborhood relations, any other properties stay the same. Don't
forget to remove the joined cell from any collection and delete it! ";

%feature("docstring")  cmf::upslope::SolidRockCell::MainOutlet "Neighbor MainOutlet(bool ForceCalculation)

Gets the downslope cell with steepest slope (terrain), calculated at
first call of the function. ";

%feature("docstring")  cmf::upslope::SolidRockCell::MainOutlet "Neighbor MainOutlet() const ";

%feature("docstring")
cmf::upslope::SolidRockCell::GetNeighborhoodType "cmf::upslope::UpslopeCell::NeighborhoodType GetNeighborhoodType()

Returns the type of neighborhood. ";

%feature("docstring")
cmf::upslope::SolidRockCell::SortNeighborhoodClockwise "void
SortNeighborhoodClockwise()

Sorts the neighbors in clockwise direction. ";

/*  Interface to the soil water storages  */

%feature("docstring")  cmf::upslope::SolidRockCell::InFlow "virtual
cmf::water::WaterFlux InFlow(const cmf::math::Time &time)

Returns the incoming water (e.g. throughfall, snow melting rate).

Parameters:
-----------

time:  Time step of the model ";

%feature("docstring")  cmf::upslope::SolidRockCell::ETpot "double
ETpot(const cmf::math::Time &time)

Returns the potential evapotranspiration (from the vegetation) in
m3/day.

Parameters:
-----------

time:  Time step of the model ";

/*  Flow Accumulation  */

%feature("docstring")  cmf::upslope::SolidRockCell::CatchmentSize "double CatchmentSize() const

Public Get-Method of the catchment size. ";

%feature("docstring")  cmf::upslope::SolidRockCell::AddToCatchmentSize
"void AddToCatchmentSize(const cmf::upslope::UpslopeCell &upc)

Adds a cell to the catchment, not for external use. ";

/*  References to Map-based properties (like Vegetation and Soil)  */

%feature("docstring")  cmf::upslope::SolidRockCell::Vegetation "const
cmf::upslope::vegetation::IVegetation& Vegetation() const

Vegetation of the UpslopeCell ";

%feature("docstring")  cmf::upslope::SolidRockCell::Vegetation "void
Vegetation(const cmf::upslope::vegetation::IVegetation &vegetation)

Sets the vegetation of the cell. The vegetation is copied to the cell,
therefore each cell owns an extra copy of vegetation.

Parameters:
-----------

vegetation:  Vegetation that should be referenced by UpslopeCell

Vegetation does not mean the Vegetation type, like forest, arable land
etc., but the special vegetation patch at the location of the
UpslopeCell. The IVegetation object can (and should) know about its
vegetation type, but for the UpslopeCell the Vegetation-object acts as
the upper boundary of the model. For coupling with biogeochemical plot
models derivations of the IVegetation-Interface can be used. ";

%feature("docstring")  cmf::upslope::SolidRockCell::Snowpack "const
cmf::upslope::snow::Snowpack& Snowpack() const ";

%feature("docstring")  cmf::upslope::SolidRockCell::Snowpack "cmf::upslope::snow::Snowpack& Snowpack() ";

%feature("docstring")  cmf::upslope::SolidRockCell::Snowpack "void
Snowpack(const cmf::upslope::snow::Snowpack &snowpack) ";

/*  Metadata  */

%feature("docstring")  cmf::upslope::SolidRockCell::AddStateVariables
"void AddStateVariables(cmf::math::StateVariableVector &vector)

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";

%feature("docstring")  cmf::upslope::SolidRockCell::AverageSaturation
"double AverageSaturation(double integrationDepth=-1)

Pure virtual function, returns the average Saturation [0..1] of the
soil.

average Saturation [0..1] in m3 H2O m-3 pores

Parameters:
-----------

integrationDepth:  Depth of the zone to include into the calculation,
negative Values will use the overall depth of the soil ";

%feature("docstring")  cmf::upslope::SolidRockCell::SaturatedDepth "double SaturatedDepth()

Pure virtual function, returns the current saturated depth [m]. ";

%feature("docstring")  cmf::upslope::SolidRockCell::SetSaturation "void SetSaturation(double newSat)

Pure virtual function, sets the average saturation. ";

%feature("docstring")  cmf::upslope::SolidRockCell::ETact "cmf::water::WaterFlux ETact(const cmf::math::Time &Time)

Pure virtual function, returns the actual evapotranspiration [m3/d].
";

%feature("docstring")  cmf::upslope::SolidRockCell::GetPondedFlow "cmf::water::WaterFlux GetPondedFlow(cmf::math::Time timestep)

Returns the flow of ponded water [m3/s].

Parameters:
-----------

time:  ( cmf::math::Time) ";

%feature("docstring")  cmf::upslope::SolidRockCell::SetSaturatedDepth
"void SetSaturatedDepth(double depth)

Sets the saturated depth. ";

%feature("docstring")  cmf::upslope::SolidRockCell::SolidRockCell "SolidRockCell(double x, double y, double z, double Area,
cmf::upslope::vegetation::IVegetation *Veg) ";

%feature("docstring")  cmf::upslope::SolidRockCell::ToString "std::string ToString() const

Converts a cell to a string. ";


// File: a00061.xml
%feature("docstring") cmf::water::Solute "

A structure to identify a solute.

C++ includes: Solute.h ";

%feature("docstring")  cmf::water::Solute::Solute "Solute(const
Solute &copy) ";


// File: a00062.xml
%feature("docstring") cmf::water::Solutes "

Manages the solutes of the whole model. The static member All is the
only instance. Before you are creating any cmf object (like
UpslopeCell, Reach or IVegetation or Meteorology), make sure you have
added all solutes you are interest in to Solutes::All Generally you
are free to add any kind of Solute to the model, but each additional
will slow down calculations remarkably, and if you intend to use one
or more ExternalFunctionReaction special requirements apply.

C++ includes: Solute.h ";

%feature("docstring")  cmf::water::Solutes::add "const Solute&
add(const std::string &name, const std::string &unit=\"\", double
uptake=1)

Creates a solute in Solute::All.

Parameters:
-----------

name:  the name of the solute, e.g. 'Tracer', 'NO3'

unit:  Used unit for amount of solute e.g. 'mol','mmol','g','mg' etc.

uptake:  Fraction of the available concentration, that is taken up by
vegetation ";

%feature("docstring")  cmf::water::Solutes::size "size_t size() ";

%feature("docstring")  cmf::water::Solutes::SetInUse "void SetInUse()

If the Solutes of the model are marked as \"InUse\", no more solutes
may be added. They get marked as \"InUse\" as soon as an object
depending on the number of solutes is created (e.g. an UpslopeCell,
Reach, IVegetation or Meteorology). ";

%feature("docstring")  cmf::water::Solutes::begin "iterator begin()
";

%feature("docstring")  cmf::water::Solutes::end "iterator end() ";

%feature("docstring")  cmf::water::Solutes::begin "const_iterator
begin() const ";

%feature("docstring")  cmf::water::Solutes::end "const_iterator end()
const ";


// File: a00063.xml
%feature("docstring") cmf::water::SoluteStorage "

A class for the storage of any tracer. The state is the amount (mol,
kg etc. see cmf::water) of the tracer in the storage
\\\\begin{eqnarray*} \\\\frac{dX}{dt}&=&\\\\sum_{f=1}^{F}\\\\left( q_f
[X]_f\\\\right) +
\\\\sum_{r=1}^R\\\\left(f_r\\\\left([A],...,[Z]\\\\right)\\\\
V\\\\right) \\\\left[\\\\frac{mol}{day}\\\\right]\\\\\\\\ F&=&
\\\\mbox{Number of fluxes in water storage} \\\\\\\\ q_f&=&
\\\\mbox{Water flux in } \\\\frac{m^3}{day} \\\\\\\\
\\\\left[X\\\\right]_f &=& \\\\mbox{Concentration of solute X in flux
}q_f \\\\mbox{ in } \\\\frac{mol}{m^3} \\\\\\\\ R&=& \\\\mbox{Number
of reactions defined for this solute storage} \\\\\\\\
f_r\\\\left([A],...,[Z]\\\\right)&=& \\\\mbox{Reactive flux of }[X]
\\\\mbox{ in environment } [A],...,[Z] \\\\left[\\\\frac{mol}{m^3\\\\
day}\\\\right] \\\\\\\\ V &=& \\\\mbox{Volume of water in water
storage }\\\\left[m^3\\\\right] \\\\end{eqnarray*}.

C++ includes: SoluteStorage.h ";

%feature("docstring")  cmf::water::SoluteStorage::conc "real conc()
const

Returns the concentration of the solute. ";

%feature("docstring")  cmf::water::SoluteStorage::Derivate "virtual
real Derivate(const cmf::math::Time &time)

Returns the derivate of the state variable at time time. ";

%feature("docstring")  cmf::water::SoluteStorage::State "const real&
State() const

Returns the current state of the variable. ";

%feature("docstring")  cmf::water::SoluteStorage::State "void
State(const real &newState)

Gives access to the state variable. ";

%feature("docstring")  cmf::water::SoluteStorage::StateIsChanged "bool StateIsChanged()

Returns if the state was currently updated. ";


// File: a00064.xml
%feature("docstring") cmf::water::SoluteTimeseries "

A map of concentration time series for solutes.

C++ includes: Solute.h ";

%feature("docstring")  cmf::water::SoluteTimeseries::begin "iterator
begin() ";

%feature("docstring")  cmf::water::SoluteTimeseries::end "iterator
end() ";

%feature("docstring")  cmf::water::SoluteTimeseries::begin "const_iterator begin() const ";

%feature("docstring")  cmf::water::SoluteTimeseries::end "const_iterator end() const ";

%feature("docstring")  cmf::water::SoluteTimeseries::conc "WaterQuality conc(cmf::math::Time t) ";

%feature("docstring")  cmf::water::SoluteTimeseries::size "size_t
size() const

Returns the number of solutes in the solution. ";

%feature("docstring")  cmf::water::SoluteTimeseries::SoluteTimeseries
"SoluteTimeseries() ";

%feature("docstring")  cmf::water::SoluteTimeseries::SoluteTimeseries
"SoluteTimeseries(cmf::math::Time begin, cmf::math::Time step) ";

%feature("docstring")  cmf::water::SoluteTimeseries::SoluteTimeseries
"SoluteTimeseries(const cmf::water::SoluteTimeseries &sts) ";


// File: a00657.xml
%feature("docstring") std::stack "

STL class. ";


// File: a00065.xml
%feature("docstring") cmf::math::StateVariable "

Abstract class state variable

Simple exponential system class header implementing a state variable:

C++ includes: StateVariable.h ";

%feature("docstring")  cmf::math::StateVariable::Derivate "virtual
real Derivate(const cmf::math::Time &time)=0

Returns the derivate of the state variable at time time. ";

%feature("docstring")  cmf::math::StateVariable::State "const real&
State() const

Returns the current state of the variable. ";

%feature("docstring")  cmf::math::StateVariable::State "void
State(const real &newState)

Gives access to the state variable. ";

%feature("docstring")  cmf::math::StateVariable::StateIsChanged "bool
StateIsChanged()

Returns if the state was currently updated. ";

%feature("docstring")  cmf::math::StateVariable::StateVariable "StateVariable(real InitialState=0)

ctor ";


// File: a00066.xml
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


// File: a00067.xml
%feature("docstring") cmf::math::StateVariableVector "

A vector of state variables, can be solved by RKFIntegrator.

C++ includes: StateVariable.h ";

%feature("docstring")  cmf::math::StateVariableVector::CopyStates "void CopyStates(numVector &destination) const

Copies the states to a numeric vector using OpenMP. ";

%feature("docstring")  cmf::math::StateVariableVector::SetStates "void SetStates(const numVector &newStates)

Copies the new states to the actual states. ";

%feature("docstring")  cmf::math::StateVariableVector::CopyDerivs "void CopyDerivs(Time time, numVector &destination, real factor=1)
const

Copies the derivatives at time step \"time\" to a numeric vector using
OpenMP.

Parameters:
-----------

time:   Time at which the derivatives should be calculated

destination:  Vector to be overwritten by the results

factor:  A factor that is multiplied to the derivate (e.g. unit
conversion or integration length) ";

%feature("docstring")  cmf::math::StateVariableVector::GetStates "numVector GetStates() const

Returns the states in a numeric vector using :CopyStates, but is
slower because of additional memory allocation. ";

%feature("docstring")  cmf::math::StateVariableVector::GetDerivs "numVector GetDerivs(Time time) const

Returns the derivatives at time step \"time\" in a numeric vector
using :CopyDerivs, but is slower because of additional memory
allocation. ";


// File: a00582.xml
%feature("docstring") std::string "

STL class. ";


// File: a00658.xml
%feature("docstring") std::string::const_iterator "

STL iterator class. ";


// File: a00659.xml
%feature("docstring") std::string::const_reverse_iterator "

STL iterator class. ";


// File: a00660.xml
%feature("docstring") std::string::iterator "

STL iterator class. ";


// File: a00661.xml
%feature("docstring") std::string::reverse_iterator "

STL iterator class. ";


// File: a00588.xml
%feature("docstring") std::stringstream "

STL class. ";


// File: a00068.xml
%feature("docstring") cmf::StudyArea "

The whole study area, consisting of several catchments.

A study area consists of one or more subbasins (called Catchment). A
catchment consists of a reach and the contributing upslope area. The
upslope area is divided into cells of any shape with defined
connections to calculate subsurface flow. The cells are the central
concept of cmf.

In general you have the choice between layered cells, where you can
assign different soil properties in different layers or the vertical
homogenous cell, with constant soil properties throughout the profile.
While layered cells provide a more realistic description of soil
properties and hydrological processes, since they can provide a finer
vertical discretization of the soil, they demand much more calculation
power.

C++ includes: Basin.h ";

%feature("docstring")  cmf::StudyArea::StudyArea "StudyArea(cmf::upslope::UpslopeVector &upslopes, double
CatchmentAreaThreshold, cmf::upslope::UpslopeCell *pourPoint=0, double
maxReachLength=0)

Creates a study area from a vector of connected upslope cells
(automatic reach & catchment creation).

Parameters:
-----------

upslopes:  The DEM as a collection of UpslopeCells (
cmf::upslope::UpslopeVector *)

CatchmentAreaThreshold:  If the CatchmentSize of a cell is larger than
this value, a new Reach/Subbasin is created

pourPoint:  Defines a cell where the routing network should start. If
NULL (or None in Python), every possible reach is build

maxReachLength:  If the length of a reach is larger than this, a new
Reach/Subbasin is created (0 if there is no length boundary) ";

%feature("docstring")  cmf::StudyArea::StudyArea "StudyArea()

Creates an empty StudyArea (no catchments). ";

%feature("docstring")  cmf::StudyArea::FindCatchment "cmf::Catchment&
FindCatchment(cmf::river::reach &reach)

Finds the catchment of a reach (slow). ";

%feature("docstring")  cmf::StudyArea::FindCatchment "const
cmf::Catchment& FindCatchment(const cmf::river::reach &reach) const ";

%feature("docstring")  cmf::StudyArea::Cells "cmf::upslope::UpslopeVector Cells()

Creates a vector of all upslope cells. ";


// File: a00069.xml
%feature("docstring") cmf::StudyAreaSolver "

A solver for a complete study area. Consists of an Integrator for the
surface water system and some SubsurfaceSolver.

C++ includes: StudyAreaSolver.h ";

%feature("docstring")  cmf::StudyAreaSolver::StudyArea "cmf::StudyArea& StudyArea()

Provides access to the study area of this solver. ";

%feature("docstring")  cmf::StudyAreaSolver::StudyAreaSolver "StudyAreaSolver(cmf::StudyArea &studyarea, double
TargetSystemArea=0.0)

Creates a StudyAreaSolver.

Parameters:
-----------

studyarea:  The study area to be solved

TargetSystemArea:  The area in m2, each SubsurfaceSystem should have.
Any value smaller than the smallest catchment (e.g. 0) leads to one
system per catchment, any value greater than the biggest catchment
(e.g. 1e308) leads to one SubsurfaceSolver for the whole study area.
Values in between will pack some SubsurfaceSolver together. Your
mileage may vary. ";

%feature("docstring")  cmf::StudyAreaSolver::~StudyAreaSolver "~StudyAreaSolver() ";

%feature("docstring")  cmf::StudyAreaSolver::Run "int
Run(cmf::math::Time Until, cmf::math::Time ReachTimeStep)

Runs the whole system to an synchronization time.

Parameters:
-----------

Until:  Syncronization time of the system

ReachTimeStep:  Time step of the surface water system (usually < 10
min) ";

%feature("docstring")  cmf::StudyAreaSolver::ModelTime "cmf::math::Time ModelTime() const

Returns the current model time. ";

%feature("docstring")  cmf::StudyAreaSolver::ModelTime "void
ModelTime(cmf::math::Time val)

Sets the current model time. ";

%feature("docstring")  cmf::StudyAreaSolver::ChangeReachIntegrator "void ChangeReachIntegrator(const cmf::math::Integrator
&IntegratorTemplate)

Changes the integrator for the surface water system to an integrator
of the given type. ";

%feature("docstring")
cmf::StudyAreaSolver::ChangeSubsurfaceIntegrators "void
ChangeSubsurfaceIntegrators(const cmf::math::Integrator
&IntegratorTemplate)

Changes the integrators for the subsurface systems to an integrator of
the given type. ";

%feature("docstring")  cmf::StudyAreaSolver::UnionSubsurfaceSolvers "cmf::SubsurfaceSolver& UnionSubsurfaceSolvers(int
SubsurfacesolverIndex1, int SubsurfacesolverIndex2)

Pushes the catchments of subsurface solver at index 2 to the
subsurface solver at index 1 and removes subsurface solver 2 and
returns subsurface solver at index 1. ";

%feature("docstring")  cmf::StudyAreaSolver::UnionSubsurfaceSolvers "cmf::SubsurfaceSolver& UnionSubsurfaceSolvers(const
cmf::SubsurfaceSolver &Solver1, const cmf::SubsurfaceSolver &Solver2)

Pushes the catchments of subsurface solver 2 to the subsurface solver
1 and removes subsurface solver 2 and returns subsurface solver 1. ";


// File: a00070.xml
%feature("docstring") cmf::SubsurfaceSolver "

Combines an cmf::math::Integrator with some Catchments. These
Catchments are one subsurface system. All Neighborhood relations of
the cells inside the system are switched on and all relations outside
this system are switched off (cmf::Upslope::Neighbor::Active flag)
automatically. Note: Adding a catchment to a SubsurfaceSolver effects
the Catchment!

C++ includes: StudyAreaSolver.h ";

%feature("docstring")  cmf::SubsurfaceSolver::SubsurfaceSolver "SubsurfaceSolver(const cmf::math::Integrator &Integrator)

Constructor for specified integrator (catchments have to be added). ";

%feature("docstring")  cmf::SubsurfaceSolver::SubsurfaceSolver "SubsurfaceSolver()

Std. constructor (empty catchment list, BDF2 integrator). ";

%feature("docstring")  cmf::SubsurfaceSolver::SubsurfaceSolver "SubsurfaceSolver(Catchment &catchment)

Solver for a single catchment (may be extended), BDF2 integrator. ";

%feature("docstring")  cmf::SubsurfaceSolver::~SubsurfaceSolver "~SubsurfaceSolver() ";

%feature("docstring")  cmf::SubsurfaceSolver::Integrator "cmf::math::Integrator& Integrator()

Access to the integrator. ";

%feature("docstring")  cmf::SubsurfaceSolver::Integrator "const
cmf::math::Integrator& Integrator() const ";

%feature("docstring")  cmf::SubsurfaceSolver::Add "void Add(Catchment
&catchment)

Adds the catchment to the solver and updates the neighborhood
relations of the complete upslope area. ";

%feature("docstring")  cmf::SubsurfaceSolver::ChangeIntegrator "void
ChangeIntegrator(const cmf::math::Integrator &IntegratorTemplate)

Changes the integrator to another type. ";

%feature("docstring")  cmf::SubsurfaceSolver::ModelTime "cmf::math::Time ModelTime() const

Returns the model time of the Integrator. ";

%feature("docstring")  cmf::SubsurfaceSolver::ModelTime "void
ModelTime(cmf::math::Time newTime)

Sets the model time of the Integrator. ";

%feature("docstring")  cmf::SubsurfaceSolver::Cells "const
cmf::upslope::UpslopeVector Cells() const ";

%feature("docstring")  cmf::SubsurfaceSolver::Run "int
Run(cmf::math::Time Until)

Runs the subsurface system and updates the lateral in flow to the
reaches. ";

%feature("docstring")  cmf::SubsurfaceSolver::min_dt "cmf::math::Time
min_dt() ";

%feature("docstring")  cmf::SubsurfaceSolver::size "size_t size()
const

returns the number of catchments of this integrator ";

%feature("docstring")  cmf::SubsurfaceSolver::__getitem__ "cmf::Catchment& __getitem__(int i) ";


// File: a00071.xml
%feature("docstring") cmf::river::SWATReachType "

Structure for the description of structural parameters of a reach.

Uses the SWAT channel geometry (see SWAT Theoretical Documentation,
Version 2005 (ch. 7:1.1), Neitsch et al. 2005), in this class
referenced as SWATtheoDoc. Differences to the SWAT geometry: The flood
plain is not plain, but has a small slope=0.5%, but has an infinite
width

C++ includes: ReachType.h ";

%feature("docstring")  cmf::river::SWATReachType::Width "virtual
double Width(double depth) const

Calculates the flow width from a given actual depth [m] using the
actual channel geometry \\\\begin{eqnarray*} \\\\mbox{If } d\\\\le
d_{channel} && \\\\\\\\ w &=& w_{bottom} + 2 \\\\Delta_{bank} d
\\\\\\\\ \\\\mbox{else, } && \\\\mbox{if the river floods the flood
plain} \\\\\\\\ w &=& w_{bank} + 2 \\\\Delta_{Floodplain}
(d-d_{channel} \\\\\\\\ \\\\end{eqnarray*}.

See:  SWATtheoDoc eq. 7:1.1.3 ";

%feature("docstring")  cmf::river::SWATReachType::Perimeter "virtual
double Perimeter(double depth) const

Calculates the wetted perimeter from a given actual depth [m] using
the actual channel geometry \\\\begin{eqnarray*} \\\\mbox{If } d\\\\le
d_{channel} && \\\\\\\\ P &=& w_{bottom} + 2 \\\\sqrt{1+
{\\\\Delta_{bank}}^2} d \\\\\\\\ \\\\mbox{else, } && \\\\mbox{if the
river floods the flood plain} \\\\\\\\ P &=& P(d_{channel} + 2
\\\\sqrt{1+ {\\\\Delta_{flood\\\\ plain}}^2} (d-d_{channel}) \\\\\\\\
\\\\end{eqnarray*}.

See:  SWATtheoDoc eq. 7:1.1.5

Parameters:
-----------

depth:  Actual depth of the reach [m] ";

%feature("docstring")  cmf::river::SWATReachType::Depth "virtual
double Depth(double area) const

Calculates the actual depth of the reach using the channel geometry
\\\\begin{eqnarray*} d &=& \\\\sqrt{\\\\frac{A}{\\\\Delta_{bank}} +
\\\\frac{{w_{bottom}}^2}{4 {\\\\Delta_{bank}}^2}} -
\\\\frac{w_{bottom}}{2 \\\\Delta_{bank}} \\\\\\\\ \\\\mbox{If }
d>d_{channel} &&\\\\\\\\
d&=&d_{channel}+\\\\sqrt{\\\\frac{A-A(d_{channel})}{\\\\Delta_{flood\\\\
plain}} + \\\\frac{{w(d_{channel})}^2}{4 {\\\\Delta_{flood\\\\
plain}}^2}} - \\\\frac{w(d_{channel})}{2 \\\\Delta_{flood\\\\ plain}}
\\\\\\\\ \\\\end{eqnarray*}.

See:  SWATtheoDoc eq. 7:1.2.4

Depth of the reach [m]

Parameters:
-----------

area:  Wetted area of a river cross section [m2], can be obtained by
V/l, where V is the stored volume and l is the reach length ";

%feature("docstring")  cmf::river::SWATReachType::Area "virtual
double Area(double depth) const

Calculates the wetted area from a given depth using the channel
geometry. In most cases use Area=V/l, where V is the stored volume and
l is the reach length \\\\begin{eqnarray*} \\\\mbox{If } d>d_{channel}
&&\\\\\\\\ A &=& \\\\left(w_{bottom} + \\\\Delta_{bank} d\\\\right) d
\\\\\\\\ \\\\mbox{else, } && \\\\mbox{if the river floods the flood
plain} \\\\\\\\ A &=& A(d_{channel}) + \\\\left(w(d_{channel} +
\\\\Delta_{flood\\\\ plain} \\\\left(d-d_{channel}\\\\right)\\\\right)
(d-d_{channel}) \\\\\\\\ \\\\end{eqnarray*}.

See:  SWATtheoDoc eq. 7:1.1.4

Wetted area of a river cross section [m2]

Parameters:
-----------

depth:  Depth of the reach [m] ";

%feature("docstring")  cmf::river::SWATReachType::SWATReachType "SWATReachType()

Creates a new reach structure with standard values (small natural
river) BottomWidth = 3m, ChannelDepth = 0.5m, BankSlope = 2, nManning
= 0.0035, FloodPlainSlope = 200. ";

%feature("docstring")  cmf::river::SWATReachType::SWATReachType "SWATReachType(double BankWidth, double Depth)

Creates a new reach structure from a give width and depth.

Parameters:
-----------

BankWidth:  Width of the reach from bank to bank [m]

Depth:  Depth of the reach [m] ";

%feature("docstring")  cmf::river::SWATReachType::qManning "double
qManning(double A, double slope) const

Calculates the flow rate from a given water volume in the reach
\\\\begin{eqnarray*} q_{Manning}&=& A R^{\\\\frac 23}
\\\\sqrt{\\\\frac {\\\\Delta_z} n} \\\\\\\\ A &=& \\\\frac V l
\\\\mbox{ Area of the wetted crossection, Volume per length}\\\\\\\\
R&=&\\\\frac A P(d) \\\\\\\\ P(d)&=& \\\\mbox{ Perimeter of the wetted
crossection, a function of reach depth} \\\\\\\\ d(V)&=& \\\\mbox{
Depth of the reach} \\\\\\\\ \\\\Delta_z&=& \\\\frac{z_{max} -
z_{min}}{l} \\\\mbox{ Slope of the reach} \\\\\\\\ \\\\end{eqnarray*}.

Flow rate [m3/s]

Parameters:
-----------

A:  The area of the cross section [m2]

slope:  The slope of the reach [m/m] ";


// File: a00072.xml
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

%feature("docstring")  cmf::math::Time::ToString "std::string
ToString(char seperator=':') ";

/*  Arithmetic operators  */

/*  Boolean Operators  */

%feature("docstring")  cmf::math::Time::NextDay "Time NextDay() const

Returns the beginning of the next day. ";

%feature("docstring")  cmf::math::Time::NextFullHour "Time
NextFullHour() const

Returns the next full hour. ";


// File: a00073.xml
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
timeseries=cmf.timeseries(start,step,interpolation) # Add data
timeseries.Add(0.1) # Value at 2001/5/1 2:30pm is 0.1
timeseries.Add(0.2) # Value at 2001/5/1 2:50pm is 0.2
timeseries.Add(0.1) # Value at 2001/5/1 3:10pm is 0.1

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

%feature("docstring")  cmf::math::timeseries::power "timeseries&
power(double)

raises the timeseries to a power ";

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

/*  I/O  */

/* Creates a timeseries with a bigger timestep, containing the average

*/

%feature("docstring")  cmf::math::timeseries::Save "void
Save(std::ostream &file)

Save the Meteorology data to an ASCII File with fixed format. ";

%feature("docstring")  cmf::math::timeseries::timeseries "timeseries(std::istream &file)

Creates a Meterology from a File in fixed format. ";

%feature("docstring")  cmf::math::timeseries::end "cmf::math::Time
end() const

Last date of measurements. ";

%feature("docstring")  cmf::math::timeseries::Add "void Add(double
Value)

Appends a measurement. ";

%feature("docstring")  cmf::math::timeseries::isempty "bool isempty()
const

returns true if no values are added to the timeseries ";

%feature("docstring")  cmf::math::timeseries::clear "void clear() ";

%feature("docstring")  cmf::math::timeseries::size "int size() const
";

%feature("docstring")  cmf::math::timeseries::timeseries "timeseries(cmf::math::Time _begin, cmf::math::Time _step, int
_interpolationmethod=0)

Constructor of a time series.

Parameters:
-----------

_begin:  First date of measurement

_step:   Time between measurements

_interpolationmethod:  Method for the interpolation (0 - Nearest
neighbor, 1- linear, 2 - cubic spline (not implemented yet) ";

%feature("docstring")  cmf::math::timeseries::timeseries "timeseries(cmf::math::Time _begin, cmf::math::Time _step, int size,
int _interpolationmethod) ";

%feature("docstring")  cmf::math::timeseries::timeseries "timeseries() ";

%feature("docstring")  cmf::math::timeseries::timeseries "timeseries(const cmf::math::timeseries &ts) ";


// File: a00074.xml
%feature("docstring") cmf::upslope::TotalFlux "

A simple structure holding a water flux for throughfall, ETact and
ponded flux.

C++ includes: UpslopeVector.h ";

%feature("docstring")  cmf::upslope::TotalFlux::TotalFlux "TotalFlux() ";


// File: a00075.xml
%feature("docstring") cmf::river::TriangularReach "

Structure for the description of reaches with a triangular cross
section.

Although real triangular cross section reach are rarely met, a
triangular reach does scale with its water load, and is therefore
preferable in case where nothing about channel geometry is known

C++ includes: ReachType.h ";

%feature("docstring")  cmf::river::TriangularReach::Width "virtual
double Width(double depth) const

Returns the width of the stream at a given depth \\\\begin{eqnarray*}
w &=& 2 \\\\Delta\\\\ d \\\\end{eqnarray*}. ";

%feature("docstring")  cmf::river::TriangularReach::Perimeter "virtual double Perimeter(double depth) const

Returns the wetted perimeter at a given depth \\\\begin{eqnarray*} P
&=& 2 d \\\\sqrt{1+\\\\Delta^2} \\\\end{eqnarray*}. ";

%feature("docstring")  cmf::river::TriangularReach::Depth "virtual
double Depth(double area) const

Returns the depth at a given crossection area \\\\begin{eqnarray*} d
&=& \\\\sqrt{\\\\frac{A}{\\\\Delta}} \\\\end{eqnarray*}. ";

%feature("docstring")  cmf::river::TriangularReach::Area "virtual
double Area(double depth) const

Returns the crossection area at a given depth \\\\begin{eqnarray*} A
&=& d^2 \\\\Delta \\\\end{eqnarray*}. ";

%feature("docstring")  cmf::river::TriangularReach::TriangularReach "TriangularReach(double bankSlope=2)

Creates a new triangular reach type. ";

%feature("docstring")  cmf::river::TriangularReach::qManning "double
qManning(double A, double slope) const

Calculates the flow rate from a given water volume in the reach
\\\\begin{eqnarray*} q_{Manning}&=& A R^{\\\\frac 23}
\\\\sqrt{\\\\frac {\\\\Delta_z} n} \\\\\\\\ A &=& \\\\frac V l
\\\\mbox{ Area of the wetted crossection, Volume per length}\\\\\\\\
R&=&\\\\frac A P(d) \\\\\\\\ P(d)&=& \\\\mbox{ Perimeter of the wetted
crossection, a function of reach depth} \\\\\\\\ d(V)&=& \\\\mbox{
Depth of the reach} \\\\\\\\ \\\\Delta_z&=& \\\\frac{z_{max} -
z_{min}}{l} \\\\mbox{ Slope of the reach} \\\\\\\\ \\\\end{eqnarray*}.

Flow rate [m3/s]

Parameters:
-----------

A:  The area of the cross section [m2]

slope:  The slope of the reach [m/m] ";


// File: a00649.xml
%feature("docstring") std::underflow_error "

STL class. ";


// File: a00076.xml
%feature("docstring") cmf::upslope::connections::UnsatSatConnection "

Calculates the flux from the unsaturated zone to the saturated zone by
vertical outflow from the unsaturated zone and water exchange by water
table changes.

\\\\begin{eqnarray*} K_u &=& K(\\\\theta) A \\\\\\\\ Ex_w &=&
\\\\frac{dz_{sat}}{dt\\\\ \\\\Delta Z_{layer}} V_{layer} \\\\\\\\
\\\\frac{dz_{sat}}{dt} &=& \\\\frac{ \\\\sum q_{Saturated}}{ A
\\\\Phi} \\\\\\\\ \\\\end{eqnarray*} where:  $ A $ is the area of the
cell

$ K(\\\\theta) $ is the unsaturated conductivity (see SoilType::Kunsat
)

$ \\\\Delta Z $ is the thickness of a layer

$ V $ is the volume of water in a layer

$ layer $ is the unsaturated zone if $ \\\\frac{dz_{sat}}{dt} $ is
positive, otherwise layer is the saturated zone

$ \\\\sum q $ is the water balance of the saturated zone

$ \\\\Phi $ is the porosity

C++ includes: FluxConnections.h ";

%feature("docstring")
cmf::upslope::connections::UnsatSatConnection::UnsatSatConnection "UnsatSatConnection(cmf::upslope::FlexibleSizeLayer &unsat,
cmf::upslope::FlexibleSizeSaturatedZone &sat)

Creates a connection between unsaturated and saturated zone. ";

%feature("docstring")
cmf::upslope::connections::UnsatSatConnection::q "real q(const
FluxNode &inquirer, cmf::math::Time t)

Returns the current flux through a connection. Negative signs mean out
of the inquirer, positive are inflows to the inquirer. ";

%feature("docstring")
cmf::upslope::connections::UnsatSatConnection::Target "FluxNode&
Target(const FluxNode &inquirer)

Returns the other end of a connection than the asking end. ";

%feature("docstring")
cmf::upslope::connections::UnsatSatConnection::Target "const
FluxNode& Target(const FluxNode &inquirer) const

Returns the other end of a connection than the asking end (const). ";

%feature("docstring")
cmf::upslope::connections::UnsatSatConnection::conc "WaterQuality
conc(const FluxNode &inquirer, cmf::math::Time t)

Returns the concentration of the flux. If not overridden, it returns
the concentration of the source of the flux (direction depending). ";


// File: a00077.xml
%feature("docstring") cmf::upslope::UpslopeCell "

Abstract class Upslope cell, encapsulation for subsurface flows.

This class holds the position of the cell in space, subsurface
neighborhood relations, main outlet for surface flows and provides a
connection to a vegetation object, for water exchange with the
atmosphere

Required and proposed overrides in Derivates  Required:
AddStateVariables - Adds the Statevariables of this upslope cell to a
vector of state variables

Required: GetPondedFlow - Returns the flow rate of ponded water

Required: AverageSaturation - Returns the average Saturation of the
soil in the rooting zone

Required: SetSaturation - Set an initial saturation of the cell

Required: SaturatedDepth - Returns the depth of the water table

Proposed: Derived* AsDerived(cell*) - A standalone function that
perfoms a dynamic cast of a UpslopeCell pointer (e.g. an element of
UpslopeVector) to the child class, this is needed for the Python
interface If the cell is not of type Derived, the function returns a
null- pointer (None in Python)

C++ includes: UpslopeCell.h ";

/*  Interface to the soil water storages  */

%feature("docstring")  cmf::upslope::UpslopeCell::AverageSaturation "virtual double AverageSaturation(double integrationDepth=-1)=0

Pure virtual function, returns the average Saturation [0..1] of the
soil.

average Saturation [0..1] in m3 H2O m-3 pores

Parameters:
-----------

integrationDepth:  Depth of the zone to include into the calculation,
negative Values will use the overall depth of the soil ";

%feature("docstring")  cmf::upslope::UpslopeCell::SaturatedDepth "virtual double SaturatedDepth()=0

Pure virtual function, returns the current saturated depth [m]. ";

%feature("docstring")  cmf::upslope::UpslopeCell::SetSaturation "virtual void SetSaturation(double newSat)=0

Pure virtual function, sets the average saturation. ";

%feature("docstring")  cmf::upslope::UpslopeCell::SetSaturatedDepth "virtual void SetSaturatedDepth(double depth)

Sets the saturated depth. ";

%feature("docstring")  cmf::upslope::UpslopeCell::ETact "virtual
cmf::water::WaterFlux ETact(const cmf::math::Time &time)=0

Pure virtual function, returns the actual evapotranspiration [m3/d].
";

%feature("docstring")  cmf::upslope::UpslopeCell::InFlow "virtual
cmf::water::WaterFlux InFlow(const cmf::math::Time &time)

Returns the incoming water (e.g. throughfall, snow melting rate).

Parameters:
-----------

time:  Time step of the model ";

%feature("docstring")  cmf::upslope::UpslopeCell::GetPondedFlow "virtual cmf::water::WaterFlux GetPondedFlow(cmf::math::Time time)=0

Returns the flow of ponded water [m3/s].

Parameters:
-----------

time:  ( cmf::math::Time) ";

%feature("docstring")  cmf::upslope::UpslopeCell::ETpot "double
ETpot(const cmf::math::Time &time)

Returns the potential evapotranspiration (from the vegetation) in
m3/day.

Parameters:
-----------

time:  Time step of the model ";

/*  Metadata  */

/*  References to Map-based properties (like Vegetation and Soil)  */

%feature("docstring")  cmf::upslope::UpslopeCell::Vegetation "const
cmf::upslope::vegetation::IVegetation& Vegetation() const

Vegetation of the UpslopeCell ";

%feature("docstring")  cmf::upslope::UpslopeCell::Vegetation "void
Vegetation(const cmf::upslope::vegetation::IVegetation &vegetation)

Sets the vegetation of the cell. The vegetation is copied to the cell,
therefore each cell owns an extra copy of vegetation.

Parameters:
-----------

vegetation:  Vegetation that should be referenced by UpslopeCell

Vegetation does not mean the Vegetation type, like forest, arable land
etc., but the special vegetation patch at the location of the
UpslopeCell. The IVegetation object can (and should) know about its
vegetation type, but for the UpslopeCell the Vegetation-object acts as
the upper boundary of the model. For coupling with biogeochemical plot
models derivations of the IVegetation-Interface can be used. ";

%feature("docstring")  cmf::upslope::UpslopeCell::Snowpack "const
cmf::upslope::snow::Snowpack& Snowpack() const ";

%feature("docstring")  cmf::upslope::UpslopeCell::Snowpack "cmf::upslope::snow::Snowpack& Snowpack() ";

%feature("docstring")  cmf::upslope::UpslopeCell::Snowpack "void
Snowpack(const cmf::upslope::snow::Snowpack &snowpack) ";

/*  Spatial Properties.  */

/* Properties of UpslopeCell that are describing the spatial
distribution of the Cells

*/

%feature("docstring")  cmf::upslope::UpslopeCell::Area "double Area()
const

Area of UpslopeCell in m2. ";

%feature("docstring")  cmf::upslope::UpslopeCell::Center "cmf::geometry::point& Center()

Center of the Cell. ";

%feature("docstring")  cmf::upslope::UpslopeCell::Center "cmf::geometry::point Center() const ";

%feature("docstring")  cmf::upslope::UpslopeCell::Height "double
Height() const

Mean height of Cell a.s.l. ";

%feature("docstring")  cmf::upslope::UpslopeCell::GetSlopeTo "double
GetSlopeTo(cmf::upslope::UpslopeCell &other) const

Calculates the topographic slope to another cell in m/m. ";

/*  Neighborhood.  */

/* Methods to handle and describe the topological relation to other
Cells

*/

%feature("docstring")  cmf::upslope::UpslopeCell::NeighborCount "int
NeighborCount() const ";

%feature("docstring")  cmf::upslope::UpslopeCell::GetNeighbor "Neighbor& GetNeighbor(int index)

The Neighborhood of a cell, this acces is quite slow, in C++ use the
NeighborIterator for cycling.

Parameters:
-----------

index:  Index of the neighbor ";

%feature("docstring")  cmf::upslope::UpslopeCell::AddNeighbor "bool
AddNeighbor(UpslopeCell &cell, double flowwidth)

Adds a Neighbor cell to the neighborhood.

True if cell was added, false if cell was already defiened as neighbor

Parameters:
-----------

cell:  Neighboring cell

flowwidth:  Lenght of intersection [m] ";

%feature("docstring")  cmf::upslope::UpslopeCell::IsNeighbor "bool
IsNeighbor(const UpslopeCell &cell)

Returns true if the given cell is a neighbor of this. ";

%feature("docstring")  cmf::upslope::UpslopeCell::RemoveNeighbor "void RemoveNeighbor(int index)

Removes a neighbor at an specified index. ";

%feature("docstring")  cmf::upslope::UpslopeCell::RemoveNeighbor "bool RemoveNeighbor(const UpslopeCell &cell)

Removes a specific cell. ";

%feature("docstring")  cmf::upslope::UpslopeCell::JoinCell "void
JoinCell(UpslopeCell &cell)

Creates a union from the given cell and this. Changes area and
neighborhood relations, any other properties stay the same. Don't
forget to remove the joined cell from any collection and delete it! ";

%feature("docstring")  cmf::upslope::UpslopeCell::GetNeighbor "NeighborIterator GetNeighbor()

Returns an NeighborIterator for fast cycling (only C++ API). ";

%feature("docstring")  cmf::upslope::UpslopeCell::GetNeighbor "NeighborIterator GetNeighbor(const UpslopeCell &cell) ";

%feature("docstring")  cmf::upslope::UpslopeCell::MainOutlet "Neighbor MainOutlet(bool ForceCalculation)

Gets the downslope cell with steepest slope (terrain), calculated at
first call of the function. ";

%feature("docstring")  cmf::upslope::UpslopeCell::MainOutlet "Neighbor MainOutlet() const ";

%feature("docstring")  cmf::upslope::UpslopeCell::GetNeighborhoodType
"cmf::upslope::UpslopeCell::NeighborhoodType GetNeighborhoodType()

Returns the type of neighborhood. ";

%feature("docstring")
cmf::upslope::UpslopeCell::SortNeighborhoodClockwise "void
SortNeighborhoodClockwise()

Sorts the neighbors in clockwise direction. ";

/*  Flow Accumulation  */

%feature("docstring")  cmf::upslope::UpslopeCell::CatchmentSize "double CatchmentSize() const

Public Get-Method of the catchment size. ";

%feature("docstring")  cmf::upslope::UpslopeCell::AddToCatchmentSize "void AddToCatchmentSize(const cmf::upslope::UpslopeCell &upc)

Adds a cell to the catchment, not for external use. ";

/*  Constructors.  */

%feature("docstring")  cmf::upslope::UpslopeCell::UpslopeCell "UpslopeCell(double x, double y, double z, double Area, const
cmf::upslope::vegetation::IVegetation *vegetation)

Constructor of an upslope cell.

Parameters:
-----------

x:  X-Coordinate of cell [m]

y:  Y-Coordinate of cell [m]

z:  Mean height of cell a.s.l. [m]

Area:  Area of the cell [m2]

vegetation:  Pointer to the vegetation of the cell (
cmf::upslope::vegetation::IVegetation), the instance is copied. ";

%feature("docstring")  cmf::upslope::UpslopeCell::UpslopeCell "UpslopeCell()

Initializes the location with (0 0 0) and the Area with 1. ";

%feature("docstring")  cmf::upslope::UpslopeCell::~UpslopeCell "virtual ~UpslopeCell() ";

%feature("docstring")  cmf::upslope::UpslopeCell::ToString "std::string ToString() const

Converts a cell to a string. ";

%feature("docstring")  cmf::upslope::UpslopeCell::AddStateVariables "virtual void AddStateVariables(cmf::math::StateVariableVector
&vector)=0

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";


// File: a00078.xml
%feature("docstring") cmf::upslope::UpslopeCell::NeighborIterator "

Fast iterator for the neighborhood of a cell (only for the C++ API,
use GetNeighbor instead).

C++ includes: UpslopeCell.h ";

%feature("docstring")
cmf::upslope::UpslopeCell::NeighborIterator::valid "bool valid()

Returns false, if the all neighbors are cycled. ";

%feature("docstring")
cmf::upslope::UpslopeCell::NeighborIterator::NeighborIterator "NeighborIterator(std::vector< Neighbor >::iterator Begin, std::vector<
Neighbor >::iterator End)

Creates an iterator for a range. ";

%feature("docstring")
cmf::upslope::UpslopeCell::NeighborIterator::NeighborIterator "NeighborIterator(const NeighborIterator &copy)

Copies an iterator. ";


// File: a00079.xml
%feature("docstring") cmf::upslope::UpslopeResultRaster "

A raster with fast access to results of UpslopeCells.

C++ includes: UpslopeResultRaster.h ";

/*  Metadata  */

%feature("docstring")  cmf::upslope::UpslopeResultRaster::Xllcorner "double Xllcorner() const

x-Coordinate for the Lower Left corner (in cellsize units) ";

%feature("docstring")  cmf::upslope::UpslopeResultRaster::Yllcorner "double Yllcorner() const

y-Coordinate for the Lower Left corner (in cellsize units) ";

%feature("docstring")  cmf::upslope::UpslopeResultRaster::XCellsize "double XCellsize() const

Cellsize of the raster. ";

%feature("docstring")  cmf::upslope::UpslopeResultRaster::YCellsize "double YCellsize() const

Cellsize of the raster. ";

%feature("docstring")  cmf::upslope::UpslopeResultRaster::ColumnCount
"int ColumnCount() const

Number of columns in the raster. ";

%feature("docstring")  cmf::upslope::UpslopeResultRaster::RowCount "int RowCount() const

Number of rows in the raster. ";

%feature("docstring")  cmf::upslope::UpslopeResultRaster::Width "double Width() const

Extent W-E in cellsize units. ";

%feature("docstring")  cmf::upslope::UpslopeResultRaster::Height "double Height() const

Extent N-S in cellsize units. ";

%feature("docstring")  cmf::upslope::UpslopeResultRaster::NoData "float  NoData() const

Returns rasterType (NoData). ";

%feature("docstring")  cmf::upslope::UpslopeResultRaster::GetBounds "BoundingBox GetBounds() const

Returns the bounding box of the raster. ";

/*  Data access  */

%feature("docstring")  cmf::upslope::UpslopeResultRaster::IdentifyXY "float  IdentifyXY(double x, double y) const

Returns the value of the raster at the given position. ";

%feature("docstring")
cmf::upslope::UpslopeResultRaster::IdentifyRowCol "float
IdentifyRowCol(int row, int col) const

Returns the value of the raster at the given cell. ";

%feature("docstring")
cmf::upslope::UpslopeResultRaster::IdentifyColRow "float
IdentifyColRow(int col, int row) const

Returns the value of the raster at the given cell. ";

%feature("docstring")  cmf::upslope::UpslopeResultRaster::HasData "bool HasData(double x, double y) const

Returns true if there is data at the given position. ";

%feature("docstring")  cmf::upslope::UpslopeResultRaster::HasData "bool HasData(int col, int row) const

Returns true if there is data at the given position. ";

%feature("docstring")  cmf::upslope::UpslopeResultRaster::SetData "void SetData(double x, double y, floatval)

Sets a value at the given position.

Parameters:
-----------

x:  X-coordinate

y:  Y-coordinate

val:  Value to set at (x,y) ";

%feature("docstring")  cmf::upslope::UpslopeResultRaster::SetData "void SetData(int col, int row, floatval)

Sets a value at the given position.

Parameters:
-----------

col:  Column of the raster

row:  Row of the raster

val:  Value to set at (col,row) ";

/*  Analysis  */

%feature("docstring")  cmf::upslope::UpslopeResultRaster::statistics "RasterStatistics statistics()

Creates statistics for the raster. ";

%feature("docstring")  cmf::upslope::UpslopeResultRaster::histogram "Histogram histogram(size_t bins=100) ";

/*  Operators  */

/*  Constructors & IO-Methods  */

%feature("docstring")
cmf::upslope::UpslopeResultRaster::WriteToASCFile "void
WriteToASCFile(std::ostream &ASCFile)

Writes the raster to a stream in ESRI-ASC format. ";

%feature("docstring")
cmf::upslope::UpslopeResultRaster::WriteToASCFile "void
WriteToASCFile(std::string filename)

Writes the raster to a filename. ";

%feature("docstring")
cmf::upslope::UpslopeResultRaster::WriteToBinary "void
WriteToBinary(std::string filename) const

Writes the data to the file with the given file name and the header to
a filename with the extension .hdr.

If the filename has the extension .flt and the raster is a float
raster the saved file can be read by ArcGIS ";

/*  Conversion functions  */

%feature("docstring")  cmf::upslope::UpslopeResultRaster::ToInt "Raster<int> ToInt() const

Converts the raster to a raster of int. ";

%feature("docstring")  cmf::upslope::UpslopeResultRaster::ToFloat "Raster<float> ToFloat()

Converts the raster to a raster of float (32bit). ";

%feature("docstring")  cmf::upslope::UpslopeResultRaster::ToDouble "Raster<double> ToDouble()

Converts the raster to a raster of float (64bit). ";

/*  Focal functions  */

%feature("docstring")  cmf::upslope::UpslopeResultRaster::focal_min "Raster<float > focal_min(int n=3)

Creates a raster, which contains for each cell the minimum of the
surrounding n x n window. ";

%feature("docstring")  cmf::upslope::UpslopeResultRaster::focal_max "Raster<float > focal_max(int n=3)

Creates a raster, which contains for each cell the maximum of the
surrounding n x n window. ";

%feature("docstring")  cmf::upslope::UpslopeResultRaster::focal_mean "Raster<float > focal_mean(int n=3)

Creates a raster, which contains for each cell the mean of the
surrounding n x n window. ";

%feature("docstring")  cmf::upslope::UpslopeResultRaster::focal_stdev
"Raster<float > focal_stdev(int n=3)

Creates a raster, which contains for each cell the standard deviation
of the surrounding n x n window. ";

%feature("docstring")
cmf::upslope::UpslopeResultRaster::focal_majority "Raster<float >
focal_majority(int n=3)

Creates a raster, which contains for each cell the majority value of
the surrounding n x n window (usually only used for integer raster).
";

%feature("docstring")
cmf::upslope::UpslopeResultRaster::focal_mean_difference "Raster<float > focal_mean_difference(int n=3)

Creates a raster, which contains for each cell the difference between
the actual value and the mean of the surrounding n x n window (usually
only used for integer raster) This function can be used to identify
very important points (VIP) for triangulation. ";

%feature("docstring")
cmf::upslope::UpslopeResultRaster::UpslopeResultRaster "UpslopeResultRaster(const cmf::geometry::Raster< float >
&TemplateRaster)

Creates a UpslopeResultRaster from a template raster. ";

%feature("docstring")
cmf::upslope::UpslopeResultRaster::SetSaturation "void
SetSaturation(const UpslopeVector &upslopes, double
integrationdepth=-1)

Populates the raster with the average saturation of each cell in
upslopes. ";

%feature("docstring")
cmf::upslope::UpslopeResultRaster::SetSaturationDepth "void
SetSaturationDepth(const UpslopeVector &upslopes)

Populates the raster with the saturated depth of each cell in
upslopes. ";

%feature("docstring")
cmf::upslope::UpslopeResultRaster::SetPondedFlow "void
SetPondedFlow(const UpslopeVector &upslopes, cmf::math::Time timestep)

Populates the raster with the ponded flow of each cell in upslopes at
the given time step. ";

%feature("docstring")  cmf::upslope::UpslopeResultRaster::GetXPosition
"double GetXPosition(int col)

Returns the real world x position of a column. ";

%feature("docstring")  cmf::upslope::UpslopeResultRaster::GetYPosition
"double GetYPosition(int row)

Returns the real world y position of a row. ";

%feature("docstring")  cmf::upslope::UpslopeResultRaster::__len__ "int __len__() ";

%feature("docstring")  cmf::upslope::UpslopeResultRaster::__getitem__
"float  __getitem__(int index) ";


// File: a00080.xml
%feature("docstring") cmf::upslope::UpslopeVector "

Vector of cmf::upslope::UpslopeCell. This class inherits from the STL
vector container.

C++ includes: UpslopeVector.h ";

/*  Constructors  */

%feature("docstring")  cmf::upslope::UpslopeVector::UpslopeVector "UpslopeVector(int count)

Creates a new UpslopeVector with count Capacity.

Parameters:
-----------

count:  Initial capacity ";

%feature("docstring")  cmf::upslope::UpslopeVector::UpslopeVector "UpslopeVector() ";

%feature("docstring")  cmf::upslope::UpslopeVector::UpslopeVector "UpslopeVector(const UpslopeVector &vector) ";

/*  Catchment-Utilities  */

%feature("docstring")
cmf::upslope::UpslopeVector::CalcFlowAccumulation "void
CalcFlowAccumulation()

Sorts the upslope cells in ascending height order, sets the MainOutlet
cell and calculates the contributing area for surface flow. ";

%feature("docstring")  cmf::upslope::UpslopeVector::sort "void sort()

Sorts the upslope cell after ascending heights. ";

%feature("docstring")  cmf::upslope::UpslopeVector::FlowAccCalculated
"bool FlowAccCalculated() const

Returns bool A flag indicating if the UpslopeVector is sorted. ";

%feature("docstring")  cmf::upslope::UpslopeVector::Area "double
Area() const

Returns the sum of the area of all upslope cell. ";

%feature("docstring")  cmf::upslope::UpslopeVector::Boundary "UpslopeVector Boundary() const

Returns a new UplsopeVector containing all outer cells. ";

%feature("docstring")
cmf::upslope::UpslopeVector::DeactivateExternalNeighborRelations "void DeactivateExternalNeighborRelations()

Deactivates all neighborhood relations outside of this vector. ";

%feature("docstring")
cmf::upslope::UpslopeVector::UpdateNeighborhoodRelations "void
UpdateNeighborhoodRelations()

Deactivates all neighborhood relations outside of this vector and
reactivates all neighborhood relations inside of this vector (this is
called by cmf::SubsurfaceSolver::Add()). ";

%feature("docstring")  cmf::upslope::UpslopeVector::SetVegetation "void SetVegetation(cmf::upslope::maps::IMap<
cmf::upslope::vegetation::IVegetation * > &vegetationmap)

Replaces the current vegetation objects of the cells by a new one,
taken from the given map. ";

/*  Overrides of std::vector  */

%feature("docstring")  cmf::upslope::UpslopeVector::push_back "void
push_back(UpslopeCell *const &_Val)

calls vector::push_back and sets m_sorted = false ";

%feature("docstring")  cmf::upslope::UpslopeVector::erase "iterator
erase(iterator _Where)

calls vector::erase(iterator _Where) and sets m_sorted = false ";

%feature("docstring")  cmf::upslope::UpslopeVector::erase "iterator
erase(iterator _First, iterator _Last)

calls vector::erase(iterator _First,iterator _Last) and sets m_sorted
= false ";

%feature("docstring")  cmf::upslope::UpslopeVector::insert "void
insert(iterator _Where, iterator _First, iterator _Last)

calls vector::insert(iterator _Where, _Iter _First, _Iter _Last) and
sets m_sorted = false ";

%feature("docstring")  cmf::upslope::UpslopeVector::find "UpslopeCell* find(cmf::geometry::point p, double maxdist=1e200)

Finds the nearest UpslopeCell to the point p. ";

%feature("docstring")  cmf::upslope::UpslopeVector::find "const
UpslopeCell* find(cmf::geometry::point p, double maxdist=1e200) const
";

%feature("docstring")  cmf::upslope::UpslopeVector::find "UpslopeCell* find(double x, double y, double maxdist=1e200)

Finds the nearest UpslopeCell to the point (x,y). ";

%feature("docstring")  cmf::upslope::UpslopeVector::find "const
UpslopeCell* find(double x, double y, double maxdist=1e200) const ";

%feature("docstring")  cmf::upslope::UpslopeVector::find "int
find(const cmf::upslope::UpslopeCell *) const

Finds the cell, returns this->size() if the cell is not in this. ";

/*  Implements StateVariableOwner  */

%feature("docstring")  cmf::upslope::UpslopeVector::AddStateVariables
"void AddStateVariables(cmf::math::StateVariableVector &states)

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";

/*  total flows  */

%feature("docstring")  cmf::upslope::UpslopeVector::Fluxes "TotalFlux
Fluxes(const cmf::math::Time &t)

Returns the total input into the collection of UpslopeCells. ";

%feature("docstring")  cmf::upslope::UpslopeVector::FillSinks "int
FillSinks(double minCellDifference=0.001) ";


// File: a00662.xml
%feature("docstring") std::valarray "

STL class. ";


// File: a00663.xml
%feature("docstring") std::vector "

STL class. ";


// File: a00664.xml
%feature("docstring") std::vector::const_iterator "

STL iterator class. ";


// File: a00665.xml
%feature("docstring") std::vector::const_reverse_iterator "

STL iterator class. ";


// File: a00666.xml
%feature("docstring") std::vector::iterator "

STL iterator class. ";


// File: a00667.xml
%feature("docstring") std::vector::reverse_iterator "

STL iterator class. ";


// File: a00668.xml
%feature("docstring") cmf::upslope::vegetation::Vegetation "";

%feature("docstring")
cmf::upslope::vegetation::Vegetation::RootFraction "virtual double
RootFraction(double upperBoundary, double lowerBoundary) const ";

%feature("docstring")
cmf::upslope::vegetation::Vegetation::Vegetation "Vegetation(double
_LAI=12, double _Height=0.12, double _RootDepth=0.25, double
_StomatalResistance=100, double _albedo=0.23, double _CanopyClosure=1,
double _CanopyCapacityPerLAI=0.01) ";


// File: a00594.xml
%feature("docstring")
cmf::upslope::verticalHomogeneous::VerticalHomogeneousCellBuilder "";

%feature("docstring")
cmf::upslope::verticalHomogeneous::VerticalHomogeneousCellBuilder::VerticalHomogeneousCellBuilder
"VerticalHomogeneousCellBuilder(cmf::upslope::maps::IMap<
cmf::upslope::vegetation::IVegetation * > *vegetationMap,
cmf::upslope::maps::IMap< cmf::upslope::SoilType > *soilMap,
cmf::upslope::maps::IMap< float > *soilDepthMap=0) ";

%feature("docstring")
cmf::upslope::verticalHomogeneous::VerticalHomogeneousCellBuilder::CreateCell
"cmf::upslope::UpslopeCell* CreateCell(cmf::geometry::point p, double
Area) const ";

%feature("docstring")
cmf::upslope::verticalHomogeneous::VerticalHomogeneousCellBuilder::CreateCell
"cmf::upslope::UpslopeCell* CreateCell(double x, double y, double z,
double Area) const ";


// File: a00595.xml
%feature("docstring")
cmf::upslope::verticalHomogeneous::VerticalHomogeneousCellSurfaceWaterBuilder
"";

%feature("docstring")
cmf::upslope::verticalHomogeneous::VerticalHomogeneousCellSurfaceWaterBuilder::VerticalHomogeneousCellSurfaceWaterBuilder
"VerticalHomogeneousCellSurfaceWaterBuilder(cmf::upslope::maps::IMap<
cmf::upslope::vegetation::IVegetation * > *vegetationMap,
cmf::upslope::maps::IMap< cmf::upslope::SoilType > *soilMap,
cmf::upslope::maps::IMap< float > *soilDepthMap=0) ";

%feature("docstring")
cmf::upslope::verticalHomogeneous::VerticalHomogeneousCellSurfaceWaterBuilder::CreateCell
"cmf::upslope::UpslopeCell* CreateCell(cmf::geometry::point p, double
Area) const ";

%feature("docstring")
cmf::upslope::verticalHomogeneous::VerticalHomogeneousCellSurfaceWaterBuilder::CreateCell
"cmf::upslope::UpslopeCell* CreateCell(double x, double y, double z,
double Area) const ";


// File: a00081.xml
%feature("docstring") cmf::water::WaterFlux "

Water fluxes consist of the flux in $\\\\frac{m^3}{day}$ and a map of
concentrations (ConcentrationMap).

C++ includes: WaterFlux.h ";

%feature("docstring")  cmf::water::WaterFlux::Update "WaterFlux&
Update(real waterflux, const cmf::water::WaterStorage &origin, bool
uptake=false) ";

%feature("docstring")  cmf::water::WaterFlux::WaterFlux "WaterFlux()

Standard constructor. ";

%feature("docstring")  cmf::water::WaterFlux::WaterFlux "WaterFlux(const WaterFlux &forcopy)

Copy constructor. ";

%feature("docstring")  cmf::water::WaterFlux::WaterFlux "WaterFlux(real flux)

Creates a clear water flux. ";

%feature("docstring")  cmf::water::WaterFlux::WaterFlux "WaterFlux(real flux, const cmf::water::WaterStorage &origin, bool
uptake=false) ";

%feature("docstring")  cmf::water::WaterFlux::Add "WaterFlux&
Add(real flux, const cmf::water::WaterStorage &origin)

Adds water from the origin to the flux (mixing) Both fluxes may be
negative, but mixed signs will throw an error \\\\begin{eqnarray*}
q&=& \\\\mbox{Flux in }\\\\frac{m^3}{day} \\\\\\\\ c&=&
\\\\mbox{Vector of concentrations in } \\\\frac{mol}{m^3} \\\\\\\\
c_{this}&=&\\\\frac{c_{this}q_{this}+c_{other}q_{other}}{q_{this}+q_{other}}
\\\\\\\\ q_{this}&=&q_{this}+q_{other} \\\\\\\\ \\\\end{eqnarray*}. ";

%feature("docstring")  cmf::water::WaterFlux::Add "WaterFlux&
Add(real flux, const cmf::water::WaterQuality &concentrations)

Adds water of a certain water quality to the flux (mixing) Both fluxes
may be negative, but mixed signs will throw an error
\\\\begin{eqnarray*} q&=& \\\\mbox{Flux in }\\\\frac{m^3}{day}
\\\\\\\\ c&=& \\\\mbox{Vector of concentrations in }
\\\\frac{mol}{m^3} \\\\\\\\
c_{this}&=&\\\\frac{c_{this}q_{this}+c_{other}q_{other}}{q_{this}+q_{other}}
\\\\\\\\ q_{this}&=&q_{this}+q_{other} \\\\\\\\ \\\\end{eqnarray*}. ";

%feature("docstring")  cmf::water::WaterFlux::Abs "WaterFlux& Abs()

Changes the sign of the flux to positive. ";

%feature("docstring")  cmf::water::WaterFlux::matterflux "real
matterflux(const cmf::water::Solute &solute)

Returns the water flux of the given solute in $\\\\frac{mol}{day}$,
depending on the unit of the concentration (see cmf::water )
\\\\begin{eqnarray*} Q_{Solute} &=&
q_{H_2O}\\\\left[\\\\frac{m^3}{day}\\\\right] c_{Solute}
\\\\left[\\\\frac{mol}{m^3}\\\\right] \\\\end{eqnarray*}. ";

%feature("docstring")  cmf::water::WaterFlux::clear "void clear()

Sets all concentrations and the flux to 0. ";


// File: a00082.xml
%feature("docstring") cmf::water::WaterQuality "

Map of concentrations for different solutes.

C++ includes: Solute.h ";

%feature("docstring")  cmf::water::WaterQuality::begin "iterator
begin() ";

%feature("docstring")  cmf::water::WaterQuality::end "iterator end()
";

%feature("docstring")  cmf::water::WaterQuality::begin "const_iterator begin() const ";

%feature("docstring")  cmf::water::WaterQuality::end "const_iterator
end() const ";

%feature("docstring")  cmf::water::WaterQuality::size "size_t size()
const

Returns the number of solutes. ";

%feature("docstring")  cmf::water::WaterQuality::tostring "std::string tostring() const ";

%feature("docstring")  cmf::water::WaterQuality::clear "void clear()

Clears the water. ";

%feature("docstring")  cmf::water::WaterQuality::WaterQuality "WaterQuality()

Standard constructor. ";

%feature("docstring")  cmf::water::WaterQuality::WaterQuality "WaterQuality(const cmf::water::WaterQuality &wq) ";


// File: a00083.xml
%feature("docstring") cmf::water::WaterStorage "

A state variable for the storage of water A class for the storage of
any tracer. The state is the amount (mol, kg etc. see cmf::water) of
the tracer in the storage \\\\begin{eqnarray*}
\\\\frac{dV}{dt}&=&\\\\sum_{f=1}^{F} q_f \\\\\\\\ F&=& \\\\mbox{Number
of fluxes in water storage} \\\\\\\\ q_f&=& \\\\mbox{Water flux in }
\\\\frac{m^3}{day} \\\\\\\\ \\\\end{eqnarray*} The vector fluxes is
used used by concentration to get the amount of water mixing.

C++ includes: WaterStorage.h ";

/*  Overrides of FluxNode  */

%feature("docstring")  cmf::water::WaterStorage::RecalcFluxes "virtual bool RecalcFluxes(cmf::math::Time t)

Pure FluxNodes do not influence fluxes, therefore no recalculation of
fluxes is required by fluxnode. WaterStorage overrides this, since
state changes require an update of the fluxes. ";

%feature("docstring")  cmf::water::WaterStorage::Empty "virtual bool
Empty() ";

%feature("docstring")  cmf::water::WaterStorage::copy "virtual
WaterStorage* copy() const ";

%feature("docstring")  cmf::water::WaterStorage::AddStateVariables "void AddStateVariables(cmf::math::StateVariableVector &vector)

Add the state variables, owned by an object derived from
StateVariableOwner, to the given vector. ";

%feature("docstring")  cmf::water::WaterStorage::WaterStorage "WaterStorage(double InitialState=0)

creates a water storage (abstract class)

Parameters:
-----------

InitialState:  Initial water content in m3 ";

%feature("docstring")  cmf::water::WaterStorage::WaterStorage "WaterStorage(const WaterStorage &forcopy) ";

%feature("docstring")  cmf::water::WaterStorage::Solute "SoluteStorage& Solute(const cmf::water::Solute &solute)

Returns the water quality of the water storage. ";

%feature("docstring")  cmf::water::WaterStorage::Solute "const
SoluteStorage& Solute(const cmf::water::Solute &solute) const ";

%feature("docstring")  cmf::water::WaterStorage::conc "real
conc(const cmf::water::Solute &solute) const

Returns the concentration of the given solute. ";

%feature("docstring")  cmf::water::WaterStorage::conc "WaterQuality
conc(cmf::math::Time t)

Returns the current WaterQuality (concentration of all solutes). ";

%feature("docstring")  cmf::water::WaterStorage::conc "void
conc(const cmf::water::Solute &solute, real NewConcetration)

Sets a new concentration. ";

%feature("docstring")  cmf::water::WaterStorage::water "real water()
const

Returns the volume of water in this storage in m3. ";

%feature("docstring")  cmf::water::WaterStorage::water "void
water(real newwatercontent)

Sets the volume of water in this storage in m3. ";

%feature("docstring")  cmf::water::WaterStorage::ToString "virtual
std::string ToString() const ";

%feature("docstring")  cmf::water::WaterStorage::Derivate "virtual
real Derivate(const cmf::math::Time &time)

Returns the derivate of the state variable at time time. ";

%feature("docstring")  cmf::water::WaterStorage::State "const real&
State() const

Returns the current state of the variable. ";

%feature("docstring")  cmf::water::WaterStorage::State "void
State(const real &newState)

Gives access to the state variable. ";

%feature("docstring")  cmf::water::WaterStorage::StateIsChanged "bool
StateIsChanged()

Returns if the state was currently updated. ";

%feature("docstring")  cmf::water::WaterStorage::ConnectionTo "cmf::water::FluxConnection& ConnectionTo(const FluxNode &target)

Returns the connection between this and target. ";

%feature("docstring")  cmf::water::WaterStorage::FluxTo "real
FluxTo(const FluxNode &target, cmf::math::Time t)

Returns the actual flux between this and target (positive sign means
\"from target into this\"). ";

%feature("docstring")  cmf::water::WaterStorage::Waterbalance "real
Waterbalance(cmf::math::Time t, const FluxConnection *except=0)

Returns the sum of all fluxes (positive and negative) at time t.
Single fluxes can be excluded from the calculation.

Parameters:
-----------

t:  Time of the query

except:  A FluxConnection that is excluded from the Waterbalance (e.g.
to prevent closed circuits) ";

%feature("docstring")  cmf::water::WaterStorage::sum_inflow "cmf::water::WaterFlux sum_inflow(cmf::math::Time t, const
FluxConnection *except=0)

Returns the sum of the incoming (positive) fluxes and the mix of their
concentrations. ";

%feature("docstring")  cmf::water::WaterStorage::sum_outflow "cmf::water::WaterFlux sum_outflow(cmf::math::Time t, const
FluxConnection *except=0)

Returns the sum of the incoming (positive) fluxes and their
concentrations. ";

%feature("docstring")  cmf::water::WaterStorage::Potential "virtual
real Potential()

Returns the water potential of the node in m waterhead The base class
water storage always returns the height of the location. ";


// File: a00084.xml
%feature("docstring") cmf::atmosphere::Weather "

A structure to return all parts of the incoming radiation, all values
in $\\\\frac{MJ}{m^2 day}$.

C++ includes: Meteorology.h ";

%feature("docstring")  cmf::atmosphere::Weather::Rn "double Rn(double
albedo, bool daily=false)

Calculates the net radiation flux $R_n \\\\left[\\\\frac{MJ}{m^2
day}\\\\right]$.

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


// File: a00557.xml
%feature("docstring") std::wfstream "

STL class. ";


// File: a00560.xml
%feature("docstring") std::wifstream "

STL class. ";


// File: a00565.xml
%feature("docstring") std::wios "

STL class. ";


// File: a00570.xml
%feature("docstring") std::wistream "

STL class. ";


// File: a00572.xml
%feature("docstring") std::wistringstream "

STL class. ";


// File: a00575.xml
%feature("docstring") std::wofstream "

STL class. ";


// File: a00578.xml
%feature("docstring") std::wostream "

STL class. ";


// File: a00580.xml
%feature("docstring") std::wostringstream "

STL class. ";


// File: a00583.xml
%feature("docstring") std::wstring "

STL class. ";


// File: a00669.xml
%feature("docstring") std::wstring::const_iterator "

STL iterator class. ";


// File: a00670.xml
%feature("docstring") std::wstring::const_reverse_iterator "

STL iterator class. ";


// File: a00671.xml
%feature("docstring") std::wstring::iterator "

STL iterator class. ";


// File: a00672.xml
%feature("docstring") std::wstring::reverse_iterator "

STL iterator class. ";


// File: a00589.xml
%feature("docstring") std::wstringstream "

STL class. ";


// File: a00136.xml


// File: a00137.xml


// File: a00138.xml


// File: a00139.xml


// File: a00140.xml


// File: a00141.xml
%feature("docstring")  cmf::upslope::connections::CreateIrregularCells
"cmf::upslope::UpslopeVector*
cmf::upslope::CreateIrregularCells(const cmf::geometry::Polygons
&polygons, const cmf::geometry::Points &centers, const
cmf::upslope::CellBuilder &cellbuilder, double slither_tolerance=0.1)
";

%feature("docstring")  cmf::upslope::connections::CreateSquaredCells "cmf::upslope::UpslopeVector* cmf::upslope::CreateSquaredCells(const
cmf::geometry::Raster< double > &DEM, const cmf::upslope::CellBuilder
&cellbuilder)

Creates a squared cell network from a raster dataset.

Before you can create meshes from rastered dem's you have to know: How
to create vegetation objects

How to distribute objects and values in space with the objects of the
maps namespace

Meshed networks of cells are generally hard to set up. But if you can
limit your needs to use regular squared cells, the process of building
the network might become quite simple using factories. Until now there
are two factories to build UpslopeCells from a raster dataset, one for
layered cells and one for vertical homogeneous cells. Most of the work
is the same for both kinds of factories. First the
VerticalHomogeneousCellFactory is explained, and then only the
differing features of the layered one are shown.

The following objects are needed to create the mesh: A Raster
representing a DEM. The simplest way to create such an raster is to
load an ESRI ASCII raster file

A map (namespace maps) of vegetation (IVegetation) objects.

A map of SoilTypes or SoilProfiles

A map of soil depth (float)[m] (optional for layered cells)

If you like to create verticalHomogeneous cells, you will need a
VerticalHomogeneousCellFactory , for layered cells you will need a
LayeredCellFactory . The created cell mesh

Parameters:
-----------

DEM:  a DoubleRaster (Raster<double>) containing the digital elevation
model

cellbuilder:  A CellBuilder, creates cells at given locations ";

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

%feature("docstring")  cmf::upslope::connections::UpslopeCellIsLower "bool cmf::upslope::UpslopeCellIsLower(UpslopeCell *upc1, UpslopeCell
*upc2) ";

%feature("docstring")  cmf::upslope::connections::deleteUpslope "void
cmf::upslope::deleteUpslope(UpslopeCell *upc) ";


// File: a00142.xml


// File: a00143.xml
%feature("docstring")  cmf::upslope::ET::PenmanMonteith "real
cmf::upslope::ET::PenmanMonteith(real Rn, real ra, real rs, real T,
real vap_press_deficit)

Returns the potential Evaporation after Penman-Monteith using some
simplifications for a given Radiation balance, aerodynamic and surface
resistances, and a vapor pressure deficit.

\\\\[ ET &=& \\\\frac{\\\\Delta R_n}{\\\\lambda \\\\Delta + \\\\gamma
+ \\\\gamma \\\\frac{r_s}{r_a}} + \\\\frac{c_p\\\\rho_a}{\\\\Delta +
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


// File: a00144.xml


// File: a00145.xml


// File: a00146.xml


// File: a00147.xml


// File: a00148.xml


// File: a00149.xml


// File: a00150.xml


// File: a00151.xml


// File: a00085.xml


// File: a00086.xml


// File: a00087.xml


// File: a00088.xml


// File: a00089.xml
%feature("docstring")  printResult "void printResult(std::ostream
&stream, cmf::river::Reach &reach, cmf::math::Time time, char
delimiter='\\\\t')

Prints states and fluxes of a reach and its subreaches and their
subbasins to a stream. ";


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
%feature("docstring")  minimum "real minimum(real a, real b)

Returns the minimum of two values. ";

%feature("docstring")  maximum "real maximum(real a, real b) ";

%feature("docstring")  minmax "real minmax(real x, real min, real
max) ";

%feature("docstring")  piecewise_linear "real piecewise_linear(real
x, real xmin, real xmax, real ymin=0, real ymax=1) ";


// File: a00115.xml


// File: a00116.xml


// File: a00117.xml


// File: a00118.xml


// File: a00119.xml


// File: a00120.xml


// File: a00121.xml


// File: a00122.xml


// File: a00123.xml


// File: a00124.xml


// File: a00125.xml


// File: a00126.xml


// File: a00127.xml


// File: a00128.xml


// File: a00129.xml


// File: a00130.xml


// File: a00131.xml


// File: a00132.xml


// File: a00133.xml


// File: a00134.xml


// File: a00135.xml


// File: todo.xml


// File: dir_c1ea5421cc65d94b94e40c62a9b5a1dd.xml


// File: dir_6894f7e083c2acd5418ccb3ff9f63a93.xml


// File: dir_ae9974ebd7051295e79d2ed4449e54bc.xml


// File: dir_038de44c667e837ac7bd0b408387318b.xml


// File: dir_604b8536ae4240420803795590a868a7.xml


// File: dir_81549b682468b23bdbc7df6c0b32b8f5.xml


// File: dir_c0896370e153f001321c91693a4cbc22.xml


// File: dir_0b605bb2de6e1c694a2540ec3ac67506.xml


// File: dir_b83afa0b74726f74995a386148265b1e.xml


// File: dir_8d7ec48f781d1fac3a54fee02c978e9f.xml


// File: dir_50af7c761aef9dc579d8e1abd9fe623f.xml


// File: dir_0da5447fd7d47a67fe2452d4bdbe2da9.xml


// File: dir_11bc2ed6aa39a494ef0bce8cb36cb582.xml


// File: dir_9eff83046f00f9e6cc4048faf2ac1c38.xml


// File: dir_45ea423ffc5cafa91296868978211296.xml


// File: dir_e95cedbddb14e57b939e6a50e26231be.xml


// File: dir_ba45a2dbe8dd3f378776f2648a94f5f5.xml


// File: dir_1265e4f635bae04c16c8d7056d02c669.xml


// File: dir_3dad22b3fe88b15a4f51db2444a052f5.xml


// File: dir_340f115698215a189081ce0e243457b9.xml


// File: dir_e7d645e420bf6d0d95a90959c6d04a44.xml


// File: main.xml

