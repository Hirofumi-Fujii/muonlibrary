// jokisch.cpp
//
// 2018-05-02 derived from muFlux class
// 2018-05-01 flux2p () is added.
// 2017-08-04 Bug fixed in the equations
//
// Cosmic muon flux at sea level
//
// Physical Review D Volume 19 Number 5, March 1979, pp.1368-1372
// Cosmic-ray muon spectrum up to 1 TeV at 75-deg zenith angle
// H. Jokisch, K. Carstensen, W. D. Dau, H. J. Meyer, and O. C. Allkofer
//
#include <cmath>
#include "jokisch.h"

namespace muonlibrary
{
const double Jokisch::m_xx = 2.57;

const double Jokisch::m_xa = 451.0;
const double Jokisch::m_xb =  77.2;
const double Jokisch::m_xc =   9.2;
const double Jokisch::m_xd =  19.8;

const double Jokisch::m_xr = 35.0;
const double Jokisch::m_xs = 57.3;
const double Jokisch::m_xt = 11.4;
const double Jokisch::m_xu =  8.9;

double
Jokisch::integral_flux (double p, double cs)
{
	// returns momentum integrated muon flux in (1/m^2/sec/str)
	// input:
	//	p	lower limit momentum in GeV/c (upper limit is infinity)
	//	cs	cos(zenith-angle)

	double g = p * cs;
	double f = m_xr / (g + m_xs)
			* pow ((cs / (5.0 * g + m_xt)), (m_xx - 1.0))
			* (g + (m_xu * 3.0 * cs)) / (g + m_xu * (2.0 * cs + 1.0)); 
	return (f * 10000.0);	// cm^2 -> m^2
}

double
Jokisch::differential_flux (double p, double cs)
{
	// returns muon flux in (1/m^2/sec/str/(GeV/c)
	// input
	//	p	momentum in GeV/c
	//	cs	cos(zenith-angle)

	double g = p * cs;
	double f = m_xa / (g + m_xb)
			 * pow (cs / (5.0 * g + m_xc), m_xx)
			 * (g + m_xd * cs) / (g + m_xd);
	return (f * 10000.0);	// cm^2 -> m^2
}

Jokisch::Jokisch ()
{
}

Jokisch::~Jokisch ()
{
}

}	// muonlibrary
