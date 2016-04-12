#ifndef ETA_POSTFIX_H
#define ETA_POSTFIX_H

//#include "../../SmartHistos/SmartHistos.hh"
#include "../Postfix_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

class Eta_postfix : public Postfix
{
	public:
		Eta_postfix(Ntuple_reader*& ntuple_reader_p);
		~Eta_postfix() 
		{
			if(debug)
			{
				std::cout << debug_prompt << "~Eta_postfix()" << std::endl; 
			} 
		};
};

Eta_postfix::Eta_postfix(Ntuple_reader*& ntuple_reader_p)
{
	this -> name = "Eta";
	this -> sel = [&ntuple_reader_p]()
	{
		float trk_eta = ntuple_reader_p -> get_traj_field_ptr() -> trk.eta;
		if(2.4 <= std::abs(trk_eta))
		{
			return static_cast<size_t>(-1);
		}
		return static_cast<size_t>(std::abs(trk_eta) / 0.4);
	};
	this -> pf = "Eta[0.0to2.0++0.4]to[0.4to2.4++0.4]";
	this -> leg = "|#eta| #subset [[0to2.0++0.4],[0.4to2.4++0.4]]";
	this -> colz = col6_rainbow_dark;
}

#endif