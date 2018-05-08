// muflux.h
//
// Base class for muon-flux

#ifndef MUFLUX_H_INCLUDED
#define MUFLUX_H_INCLUDED

namespace muonlibrary
{
class muFlux
{
public:
	muFlux ();
	virtual ~muFlux ();
	virtual double differential_flux (double pGeV, double cstheta) = 0;
	virtual double integral_flux (double pGeV, double cstheta) = 0;
	virtual double flux2p (double flux, double cstheta, double pmax);
	virtual double trx2p (double trx, double cstheta, double pmax);
};

}	// namespace muonlibrary

#endif	// MUFLUX_H_INCLUDED
