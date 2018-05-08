// flux2popt.cpp

#include <iostream>
#include <sstream>
#include "flux2popt.h"

namespace mylibrary
{

Flux2Popt::Flux2Popt()
{
	m_flxfilename = "";
	m_flxfilename_given = false;
	m_flxerrfilename = "";
	m_flxerrfilename_given = false;
	m_cosfilename = "";
	m_cosfilename_given = false;
	m_outputname = "";
	m_outputname_given = false;
	m_pcut0 = 0.0;
	m_pcut0_given = false;
	m_pmax = 1000.0;
	m_pmax_given = false;
	m_fluxmodel = Flux2Popt::JOKISCH;
	m_fluxmodel_given = false;
}

Flux2Popt::~Flux2Popt()
{
}

void
Flux2Popt::usage (std::ostream& os, const char* pname) const
{
	os << "Usage: "
		<< pname << " [options] fluxfilename fluxerrfilename\n"
		<< " options:\n"
		<< " -c[os] filename sets the cos-file name.\n"
		<< " -o[ut] outname  sets the output name.\n"
		<< " -pcut0 value    sets the detector cutoff momentum in GeV/c.\n"
		<< " -pmax value     sets the maxmum momentum in GeV/c.\n"
		<< " -jokisch        selects Jokisch-flux.\n"
		<< " -miyake         selects Miyake-flux.\n"
		<< std::endl;
}

bool
Flux2Popt::set (int argc, char* argv[])
{
	if (argc < 1)
		return false;

	bool result = true;
	int ip = 1;	// arguments start from index = 1
	while (ip < argc)
	{
		if (*argv[ip] == '-')
		{
			std::string word (argv[ip++]);
			if ((word == "-pcut") || (word == "-pcut0") || (word == "-pmax"))
			{
				if (ip >= argc)
				{
					std::cerr << "ERROR "
						<< word
						<< " missing value."
						<< std::endl;
					result = false;
					break;
				}
				double dv(0.0);
				std::stringstream ss (argv[ip++]);
				if (!(ss >> dv))
				{
					std::cerr << "ERROR "
						<< word
						<< " requires numeric value."
						<< std::endl;
					result = false;
					break;
				}
				if ((word == "-pcut") || (word == "-pcut0"))
				{
					m_pcut0 = dv;
					m_pcut0_given = true;
				}
				else if (word == "-pmax")
				{
					m_pmax = dv;
					m_pmax_given = true;
				}
			}
			else if ((word == "-out") || (word == "-o"))
			{
				if (ip >= argc)
				{
					std::cerr << "ERROR "
						<< word
						<< " missing output name."
						<< std::endl;
					result = false;
					break;
				}
				m_outputname = std::string(argv[ip++]);
				m_outputname_given = true;
			}
			else if ((word == "-cos") || (word == "-c"))
			{
				if (ip >= argc)
				{
					std::cerr << "ERROR "
						<< word
						<< " missing cos-file name."
						<< std::endl;
					result = false;
					break;
				}
				m_cosfilename = std::string(argv[ip++]);
				m_cosfilename_given = true;
			}
			else if (word == "-jokisch")
			{
				m_fluxmodel = JOKISCH;
				m_fluxmodel_given = true;
			}
			else if (word == "-miyake")
			{
				m_fluxmodel = MIYAKE;
				m_fluxmodel_given = true;
			}
			else
			{
				std::cerr << "ERROR "
					<< argv[ip] << " -- Unknown option."
					<< std::endl;
				result = false;
				break;
			}
		}
		else
		{
			if (!m_flxfilename_given)
			{
				m_flxfilename = std::string (argv[ip++]);
				m_flxfilename_given = true;
			}
			else if (!m_flxerrfilename_given)
			{
				m_flxerrfilename = std::string (argv[ip++]);
				m_flxerrfilename_given = true;
			}
			else
			{
				std::cerr << "ERROR"
					<< "trx-, trxerr- and rng-filenames have been already given."
					<< std::endl;
				result = false;
				break;
			}
		}
	}
	if ((!m_flxfilename_given) || (!m_flxerrfilename_given))
		result = false;
	return result;
}

}	// mylibrary
