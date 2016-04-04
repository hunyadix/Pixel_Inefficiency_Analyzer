#ifndef CUT_NOSCAN_H
#define CUT_NOSCAN_H

#include "../SmartHisto_cut.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include "../Scan_points/scan_points.hh"

#include <string>
#include <functional>

extern bool debug;

namespace Histogram_generation
{

	class Cut_noscan : public Cut
	{
		public:
			Cut_noscan(Ntuple_reader*& ntuple_reader_p);
			~Cut_noscan()
			{
				if(debug)
				{
					std::cout << debug_prompt << "~Cut_noscan()" << std::endl;
				}
			};
	};

	Cut_noscan::Cut_noscan(Ntuple_reader*& ntuple_reader_p)
	{
		this -> cut_function = [&ntuple_reader_p]()
		{
			TrajMeasurement traj_field = *(ntuple_reader_p -> get_traj_field_ptr());
			EventData event_field      = *(ntuple_reader_p -> get_event_field_ptr());
			int pf_hv_scan = scans::hv_scan_no(event_field.run);
			int pf_delay_scan = scans::delay_scan_no(event_field.run);
			bool excl_hv = pf_hv_scan == -1;
			bool excl_delay = pf_delay_scan == -1 && 
				// One Module scanned (WBC scan)
				!(traj_field.mod_on.layer  ==  2 &&
				  traj_field.mod_on.ladder == -5 &&
				  traj_field.mod_on.module ==  2 &&
				 (event_field.run==161310 || event_field.run==161311));
			bool result = excl_hv && excl_delay;
			return result;
		};
	}

} // Histogram_generation

#endif