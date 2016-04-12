#ifndef PASSEDEFFCUTS_POSTFIX_H
#define PASSEDEFFCUTS_POSTFIX_H

//#include "../../SmartHistos/SmartHistos.hh"
#include "../Postfix_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include "../../../Histogram_generation/Cuts/SmartHisto_cut_factory.hh"

#include <string>
#include <functional>

extern bool debug;

class PassedEffcuts_postfix : public Postfix
{
	private:
		Histogram_generation::Cut* effcut_all;
	public:
		PassedEffcuts_postfix(Ntuple_reader*& ntuple_reader_p);
		~PassedEffcuts_postfix() 
		{
			if(debug)
			{
				std::cout << debug_prompt << "~PassedEffcuts_postfix()" << std::endl; 
			} 
		};
};

PassedEffcuts_postfix::PassedEffcuts_postfix(Ntuple_reader*& ntuple_reader_p)
{
	this -> name       = "PassedEffcuts";
	this -> effcut_all = Histogram_generation::Cut_factory::get_cut("effcut_all", ntuple_reader_p).get();
	this -> sel        = [&ntuple_reader_p, this]()
	{
		if((*(this -> effcut_all))())
		{
			return 0;
		}
		return 1;
	};
	this -> pf = "PassedEffcuts;CaughtByEffcuts";
	this -> leg = "PassedEffcuts;CaughtByEffcuts";
	this -> colz = col3_red_to_blue;
}

#endif
