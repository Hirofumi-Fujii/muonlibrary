// muflux.cpp
//
// Base class for muon-flux.
//

#include <cmath>
#include "muflux.h"

namespace muonlibrary
{

muFlux::muFlux ()
{
}

muFlux::~muFlux ()
{
}

double
muFlux::flux2p (double flux, double cstheta, double pmax)
{
	if (flux <= 0.0)
		return pmax;

	double p0 = 0.0;
	double f0 = integral_flux (p0, cstheta);
	if (flux > f0)
		return 0.0;

	// serch for the momentum p2
	double p1 = p0;
	double p2 = 100.0;	// initial value (100.0 GeV/c)
	double f2 = integral_flux (p2, cstheta);

	while (f2 > flux)
	{
		if (p2 > pmax)
			return pmax;
		p1 = p2;
		p2 = p2 * 2.0;
		f2 = integral_flux (p2, cstheta);
	}

	// narrow the range keeping the condition (flux(p1) > flux > flux(p2)),
	double p = (p1 + p2) * 0.5;
	double f = integral_flux (p, cstheta);

	while ( fabs ((f - flux) / flux) > 0.001)
	{
		if (f > flux)
			p1 = p;
		else
			p2 = p;
		p = (p1 + p2) * 0.5;
		f = integral_flux (p, cstheta);
	}
	return p;
}

double
muFlux::trx2p (double trx, double cstheta, double pmax)
{
	if (trx >= 1.0)
		return 0.0;
	if (trx <= 0.0)
		return pmax;

	double flux0 = integral_flux (0.0, cstheta);
	return flux2p (flux0 * trx, cstheta, pmax);
}

}	// muonlibrary
