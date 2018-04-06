@page CmfTutUnits

[index](@ref tutorial) [next](@ref CmfTutSpaceTime)

# Units

To keep the conversions while using cmf at a minimal level, the use of
units is mostly consistent.

  - Length is always given @f$m@f$
  - Area is always given in @f$m^2@f$
  - Volume (of water, soil) is always given in @f$m^3@f$
  - Water fluxes are always given in @f$ \frac{m^{3}}{day} @f$, except
    for precipitation data. The flux is always daily, even if the flux
    occurs only for 5 min.
  - Precipitation timeseries (as driving data) is always given in
    @f$\frac{mm}{day}@f$, **regardless** the time resolution. If you
    have data measured every hour, the unit of the rainfall intensity
    **must** still be @f$\frac{mm}{day} = 288 \frac{mm}{5min}@f$
  - Solutes are only transported, hence the absolute unit of a tracer
    amount can be userdefined. Meaningful units include units of mass
    (kg,g,mg) or number of molecules (mol), as long as the value is
    stable during transport.
  - Concentration is the absolute unit of solute amount per volume. Eg.
    if the unit of solute amount is @f$g@f$ the unit of concentration
    is @f$\frac{g}{m^3}@f$, since the unit of volume is fixed.
  - Climate variables, mainly used for Evapotranspiration models have
    the following units:
  - Temperature is given in @f$°C@f$
  - Actual relative humidity is given in percent
  - Actual incoming shortwave global radiation is given in `@f$
    \frac{MJ}{m^{2}day} @f$`. Data in @f$ \frac{W}{m^{2}}@f$ has to be
    multiplied by 0.0864 to set it correctly.
  - Wind speed is given in @f$ \frac{m}{s}@f$

author: ina, version: 5 Thu May 8 18:34:58 2014
