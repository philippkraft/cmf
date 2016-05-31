#include "waterstress.h"
#include "ET.h"

cmf::upslope::ET::ContentStress* cmf::upslope::ET::ContentStress::copy() const
{
	return new cmf::upslope::ET::ContentStress(*this);
}

cmf::upslope::ET::ContentStress::ContentStress(real _theta_d, real _theta_w)
	: theta_d(_theta_d), theta_w(_theta_w)
{

}
real cmf::upslope::ET::ContentStress::Tact(const stressedET* connection,real Tpot) const
{
	cmf::upslope::SoilLayer::ptr sl = connection->get_layer();
	real
		wetness = sl->get_wetness(),
		w_wp = theta_w * sl->get_porosity(),
		w_d = theta_d * sl->get_porosity();
		
	if (theta_w < 0) {
		w_wp = sl->get_soil().Wetness_pF(4.2);
	}
	if (theta_d < 0) {
		w_d = 0.5 * (sl->get_soil().Wetness_pF(1.8) + w_wp);
	}
	real
		area = sl->cell.get_area(),
		rootfraction=sl->get_rootfraction();

	return Tpot * rootfraction * piecewise_linear(wetness,w_wp,w_d) * area * 1e-3;
}

cmf::upslope::ET::SuctionStress* cmf::upslope::ET::SuctionStress::copy() const
{
	return new SuctionStress(*this);
}

cmf::upslope::ET::SuctionStress::SuctionStress( const SuctionStress& other ) 
	: P0(other.P0),P1(other.P1),P2(other.P2),P3(other.P3)
{

}

cmf::upslope::ET::SuctionStress::SuctionStress( real Pot0,real Pot1,real Pot2,real Pot3 ) 
	: P0(Pot0),P1(Pot1),P2(Pot2),P3(Pot3)
{

}


real cmf::upslope::ET::SuctionStress::Tact(const stressedET* connection,real Tpot) const
{
	cmf::upslope::SoilLayer::ptr sl = connection->get_layer();
	real 
		suction = sl->get_matrix_potential(),
		area = sl->cell.get_area(),
		rootfraction=sl->get_rootfraction();

	return fT(suction) * Tpot * rootfraction * area * 1e-3;
}

real cmf::upslope::ET::SuctionStress::fT( real potential ) const
{
	if			(potential>P0) {
		return 0.0;
	} else if	(potential>P1) {
		return 1.0 - (P1-potential)/(P1-P2);
	} else if	(potential>P2) {
		return 1.0;
	} else if	(potential>P3)  {
		return (P3-potential)/(P3-P2);
	} else						{
		return 0.0;
	}
}


cmf::upslope::ET::VolumeStress::VolumeStress( real nostress_volume,real wiltpoint_volume )
	: V1(nostress_volume),V0(wiltpoint_volume)
{

}

cmf::upslope::ET::VolumeStress::VolumeStress( const VolumeStress& other )
	: V1(other.V1),V0(other.V0)
{

}

cmf::upslope::ET::VolumeStress* cmf::upslope::ET::VolumeStress::copy() const
{
	return new VolumeStress(*this);
}

real cmf::upslope::ET::VolumeStress::Tact( const stressedET* connection,real Tpot ) const
{
	cmf::upslope::SoilLayer::ptr sl = connection->get_layer();
	real 
		vol = sl->get_volume(),
		area = sl->cell.get_area(),
		rootfraction=sl->get_rootfraction();
	return Tpot * rootfraction * piecewise_linear(vol,V0,V1) * area * 1e-3;

}
