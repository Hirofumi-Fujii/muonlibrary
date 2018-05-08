// jokisch.h
//
// 2018-05-02 inherited from muFlux class.
// 2018-05-01 flux2p () is added.
//

#ifndef JOKISCH_H_INCLUDED
#define JOKISCH_H_INCLUDED

#include "muflux.h"

namespace muonlibrary
{

class Jokisch : public muFlux
{
public:
	Jokisch ();
	~Jokisch ();
	double integral_flux (double p, double cs);
	double differential_flux (double p, double cs);

protected:
	static const double m_xx;

	static const double m_xa;
	static const double m_xb;
	static const double m_xc;
	static const double m_xd;

	static const double m_xr;
	static const double m_xs;
	static const double m_xt;
	static const double m_xu;

};

}	// namespace muonlibrary

#endif	// JOKISCH_H_INCLUDED
