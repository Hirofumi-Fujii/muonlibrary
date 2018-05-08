// flux2p.cpp
//
// g++ -Wall flux2p.cpp flux2popt.cpp csvarray.cpp jokisch.cpp muflux.cpp hist2d.cpp ncpng.cpp -o flux2p
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include "flux2popt.h"
#include "csvarray.h"
#include "jokisch.h"
#include "hist2d.h"


static const int VERSION_NUMBER = 100;

int main (int argc, char* argv[])
{
	using namespace mylibrary;

	Flux2Popt opt;
	if ( !opt.set (argc, argv) )
	{
		opt.usage (std::cerr, argv[0]);
		return (-1);
	}
	if (!opt.m_flxfilename_given)
	{
		std::cerr << "ERROR: flux filename is missing" << std::endl;
		return (-1);
	}

	std::ifstream ifs (opt.m_flxfilename.c_str());
	if (!ifs)
	{
		std::cerr << "ERROR: flux file " << opt.m_flxfilename << " cannot be opened." << std::endl;
		return (-2);
	}

	std::ifstream ife (opt.m_flxerrfilename.c_str());
	if (!ife)
	{
		std::cerr << "ERROR: flux error file " << opt.m_flxerrfilename << " cannot be opened." << std::endl;
		return (-2);
	}

	std::ifstream cst;
	if (opt.m_cosfilename_given)
	{
		cst.open(opt.m_cosfilename.c_str());
		if (!cst)
		{
			std::cerr << "ERROR: cos file " << opt.m_cosfilename << " cannot be opened." << std::endl;
			return (-2);
		}
	}

	CSVArray flxcsv;
	flxcsv.Read (ifs);
	int nlines = (int)(flxcsv.m_array.size ());
	std::cout << "Total lines=" << nlines << std::endl;
	if (nlines < 1)
	{
		std::cerr << "ERROR: File[" << opt.m_flxfilename
			<< "] flux file is too short."
			<< std::endl;
		return (-2);
	}

	CSVArray flecsv;
	flecsv.Read (ife);
	int nlineserr = (int)(flecsv.m_array.size ());
	std::cout << "Total lines=" << nlineserr << std::endl;
	if (nlineserr < 1)
	{
		std::cerr << "ERROR: File[" << opt.m_flxerrfilename
			<< "] flux-error file is too short"
			<< std::endl;
		return (-2);
	}

	CSVArray cstcsv;
	if (opt.m_cosfilename_given)
	{
		cstcsv.Read (cst);
		int nlinescst = (int)(cstcsv.m_array.size ());
		std::cout << "Total lines=" << nlinescst << std::endl;
		if (nlinescst < 1)
		{
			std::cerr << "ERROR: cos-table file is too short"
				<< std::endl;
			return (-2);
		}
	}

	int histstart = -1;
	for (int i = 0; i < nlines; i++)
	{
//			std::cout << i << ' ' << flxcsv.CellString(i, 0) << std::endl;
			if (flxcsv.CellString(i, 0) == "Hist2D::")
				histstart = i;
	}
	if (histstart < 0)
	{
		std::cerr << "ERROR: Hist2D is not found."
			<< std::endl;
		return (-2);
	}

	std::cout << "Hist2D starts at " << histstart << std::endl;
	double xmin = flxcsv.CellDoubleValue (histstart + 2, 1);
	double xmax = flxcsv.CellDoubleValue (histstart + 2, 2);
	int nxbins = (int)(flxcsv.CellLongValue (histstart + 2, 3));
	std::cout << "xmin=" << xmin << " xmax=" << xmax << " nxbins=" << nxbins << std::endl;

	double ymin = flxcsv.CellDoubleValue (histstart + 3, 1);
	double ymax = flxcsv.CellDoubleValue (histstart + 3, 2);
	int nybins = (int)(flxcsv.CellLongValue (histstart + 3, 3));
	std::cout << "ymin=" << ymin << " ymax=" << ymax << " nybins=" << nybins << std::endl;


	int ixoffs = 3;
	int iyoffs = histstart + 7;

	double pcut0 = 0.0;
	if (opt.m_pcut0_given)
		pcut0 = opt.m_pcut0;

	double pcut = pcut0;
	muonlibrary::Jokisch jokisch;

	static const int num2dhist = 4;
	Hist2D hist2d[num2dhist] =
	{
		Hist2D ("Pcut dxdy", xmin, xmax, nxbins, ymin, ymax, nybins),
		Hist2D ("Pcut-err dxdy", xmin, xmax, nxbins, ymin, ymax, nybins),
		Hist2D ("Fluxdiff(0-obs)", xmin, xmax, nxbins, ymin, ymax, nybins),
		Hist2D ("Fluxdiff err", xmin, xmax, nxbins, ymin, ymax, nybins),
	};

	double pmax = opt.m_pmax;
	for (int ny = 0; ny < nybins; ny++)
	{
		double y = flxcsv.CellDoubleValue (ny + iyoffs, 0);
		for (int nx = 0; nx < nxbins; nx++)
		{
			double x = flxcsv.CellDoubleValue (iyoffs - 3, nx + ixoffs);
			double flx = flxcsv.CellDoubleValue (ny + iyoffs, nx + ixoffs);
			double fle = flecsv.CellDoubleValue (ny + iyoffs, nx + ixoffs);
			double cs;
			if (opt.m_cosfilename_given)
				cs = cstcsv.CellDoubleValue (ny + iyoffs, nx + ixoffs);
			else
				cs = y / sqrt (1.0 + (x * x) + (y * y));
			double csabs = fabs (cs);

			double perr;
			double fluxdiff = 0.0;
			if ((flx == 0.0) && (fle == 0.0))
			{
				pcut = 0.0;
				perr = 0.0;
			}
			else
			{
				double pcut1;
				double pcut2;
				{
					fluxdiff = jokisch.integral_flux (0.0, csabs) - flx;
					pcut = jokisch.flux2p (flx, csabs, pmax);
					pcut1 = jokisch.flux2p ((flx - fle), csabs, pmax);
					pcut2 = jokisch.flux2p ((flx + fle), csabs, pmax);
				}
				double perr1 = fabs (pcut1 - pcut);
				double perr2 = fabs (pcut2 - pcut);
				if (perr1 > perr2)
					perr = perr1;
				else
					perr = perr2;
			}
			hist2d[0].cumulate (x, y, pcut);
			hist2d[1].cumulate (x, y, perr);
			hist2d[2].cumulate (x, y, fluxdiff);
			hist2d[3].cumulate (x, y, fle);
		}
	}
	std::cout << "Done." << std::endl;

	for (int nh = 0; nh < num2dhist; nh++)
	{
		std::string outname;
		if (opt.m_outputname_given)
			outname = opt.m_outputname;
		else
			outname = "flux2p";
		std::stringstream ss;
		ss << outname << '-' << nh << ".csv";
		std::string ofcsvnam;
		ss >> ofcsvnam;
		std::ofstream ofcsv (ofcsvnam.c_str());
		if (ofcsv)
		{
			ofcsv << "\"";
			for (int i = 0; i < argc; i++)
				ofcsv << argv[i] << ' ';
			ofcsv << "\"";
			ofcsv << ",program,genpcut3,version," << VERSION_NUMBER;
//			if (opt.m_flux == Pcutopt::MIYAKE)
//				ofcsv << ",flux,Miyake";
//			else
				ofcsv << ",flux,Jokisch";
			ofcsv
				<< ",pcut0(GeV/c)," << pcut0
				<< ",pmax(GeV/c)," << pmax
				<< std::endl;
			ofcsv
				<< "Flux to p"
				<< std::endl;
			hist2d[nh].CSVdump (ofcsv);
		}
	}
	return 0;
}

