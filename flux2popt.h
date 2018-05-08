// pcutopt.h

#ifndef FLUX2POPT_H_INCLUDED
#define FLUX2POPT_H_INCLUDED

#include <iostream>
#include <string>

namespace mylibrary
{

class Flux2Popt
{
public:
	enum
	{
		JOKISCH = 1, MIYAKE,
	};
	Flux2Popt ();
	virtual ~Flux2Popt ();
	void usage (std::ostream& os, const char* pname) const;
	bool set (int argc, char* argv[]);

public:
	std::string m_flxfilename;
	bool m_flxfilename_given;
	std::string m_flxerrfilename;
	bool m_flxerrfilename_given;
	std::string m_cosfilename;
	bool m_cosfilename_given;
	std::string m_outputname;
	bool m_outputname_given;
	double m_pcut0;
	bool m_pcut0_given;
	double m_pmax;
	bool m_pmax_given;
	int m_fluxmodel;
	bool m_fluxmodel_given;
};

}	// namespace mylibrary

#endif	// FLUX2POPT_H_INCLUDED
