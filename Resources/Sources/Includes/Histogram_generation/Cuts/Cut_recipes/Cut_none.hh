#ifndef CUT_NONE_H
#define CUT_NONE_H

#include "../SmartHisto_cut.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

namespace Histogram_generation
{

	class Cut_none : public Cut
	{
		public:
			Cut_none(Ntuple_reader*& ntuple_reader_p);
			~Cut_none()
			{
				if(debug)
				{
					std::cout << debug_prompt << "~Cut_none()" << std::endl;
				}
			};
	};

	Cut_none::Cut_none(Ntuple_reader*& ntuple_reader_p)
	{
		this -> cut_function = [&ntuple_reader_p]() 
		{
			return true;
		};
	}

} // Histogram_generation

#endif