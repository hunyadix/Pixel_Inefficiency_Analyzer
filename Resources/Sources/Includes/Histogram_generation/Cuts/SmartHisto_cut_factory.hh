#ifndef CUT_FACTORY_H
#define CUT_FACTORY_H

#include "SmartHisto_cut.hh"
#include "../../Ntuple_reader/Ntuple_reader.hh"

#include "Cut_recipes/Cut_none.hh"
#include "Cut_recipes/Cut_d0.hh"
#include "Cut_recipes/Cut_dz.hh"
#include "Cut_recipes/Cut_federr.hh"
#include "Cut_recipes/Cut_goodmod.hh"
#include "Cut_recipes/Cut_goodroc.hh"
#include "Cut_recipes/Cut_hitsep.hh"
#include "Cut_recipes/Cut_hp.hh"
#include "Cut_recipes/Cut_lx_fid.hh"
#include "Cut_recipes/Cut_pixhit.hh"
#include "Cut_recipes/Cut_ly_fid.hh"
#include "Cut_recipes/Cut_zerobias.hh"
#include "Cut_recipes/Cut_noscan.hh"
#include "Cut_recipes/Cut_nstrip.hh"
#include "Cut_recipes/Cut_nvtx.hh"
#include "Cut_recipes/Cut_pt.hh"
#include "Cut_recipes/Cut_valmis.hh"
#include "Cut_recipes/Cut_bpix.hh"

extern bool debug;

namespace Histogram_generation
{
	class Cut_factory
	{
		private:
			static std::vector<std::unique_ptr<Cut>> cut_list;
			class Silent_deleter
			{
				public:
					void operator()(Cut* to_be_deleted_p)
					{
						bool debug_initial_state = debug;
						debug = false;
						delete to_be_deleted_p;
						debug = debug_initial_state;
					};
			};
			static std::unique_ptr<Cut, Silent_deleter> cut_none;
			static std::unique_ptr<Cut, Silent_deleter> cut_d0;
			static std::unique_ptr<Cut, Silent_deleter> cut_dz;
			static std::unique_ptr<Cut, Silent_deleter> cut_federr;
			static std::unique_ptr<Cut, Silent_deleter> cut_goodmod;
			static std::unique_ptr<Cut, Silent_deleter> cut_goodroc;
			static std::unique_ptr<Cut, Silent_deleter> cut_hitsep;
			static std::unique_ptr<Cut, Silent_deleter> cut_hp;
			static std::unique_ptr<Cut, Silent_deleter> cut_lx_fid;
			static std::unique_ptr<Cut, Silent_deleter> cut_ly_fid;
			static std::unique_ptr<Cut, Silent_deleter> cut_noscan;
			static std::unique_ptr<Cut, Silent_deleter> cut_nstrip;
			static std::unique_ptr<Cut, Silent_deleter> cut_nvtx;
			static std::unique_ptr<Cut, Silent_deleter> cut_zerobias;
			static std::unique_ptr<Cut, Silent_deleter> cut_pixhit;
			static std::unique_ptr<Cut, Silent_deleter> cut_pt;
			static std::unique_ptr<Cut, Silent_deleter> cut_valmis;
			static std::unique_ptr<Cut, Silent_deleter> cut_bpix;

		public:
			static std::unique_ptr<Cut>& get_cut(const std::string& type_p, Ntuple_reader*& ntuple_reader_p);
	};

	std::unique_ptr<Cut>& Cut_factory::get_cut(const std::string& type_p, Ntuple_reader*& ntuple_reader_p)
    {
		if(!cut_none)     {cut_none.reset(     new Cut_none(     ntuple_reader_p));}
		if(!cut_d0)       {cut_d0.reset(       new Cut_d0(       ntuple_reader_p));}
		if(!cut_dz)       {cut_dz.reset(       new Cut_dz(       ntuple_reader_p));}
		if(!cut_federr)   {cut_federr.reset(   new Cut_federr(   ntuple_reader_p));}
		if(!cut_goodmod)  {cut_goodmod.reset(  new Cut_goodmod(  ntuple_reader_p));}
		if(!cut_goodroc)  {cut_goodroc.reset(  new Cut_goodroc(  ntuple_reader_p));}
		if(!cut_hitsep)   {cut_hitsep.reset(   new Cut_hitsep(   ntuple_reader_p));}
		if(!cut_hp)       {cut_hp.reset(       new Cut_hp(       ntuple_reader_p));}
		if(!cut_lx_fid)   {cut_lx_fid.reset(   new Cut_lx_fid(   ntuple_reader_p));}
		if(!cut_ly_fid)   {cut_ly_fid.reset(   new Cut_ly_fid(   ntuple_reader_p));}
		if(!cut_noscan)   {cut_noscan.reset(   new Cut_noscan(   ntuple_reader_p));}
		if(!cut_nstrip)   {cut_nstrip.reset(   new Cut_nstrip(   ntuple_reader_p));}
		if(!cut_nvtx)     {cut_nvtx.reset(     new Cut_nvtx(     ntuple_reader_p));}
		if(!cut_pixhit)   {cut_pixhit.reset(   new Cut_pixhit(   ntuple_reader_p));}
		if(!cut_zerobias) {cut_zerobias.reset( new Cut_zerobias( ntuple_reader_p));}
		if(!cut_pt)       {cut_pt.reset(       new Cut_pt(       ntuple_reader_p));}
		if(!cut_valmis)   {cut_valmis.reset(   new Cut_valmis(   ntuple_reader_p));}
		if(!cut_bpix)   {cut_bpix.reset(       new Cut_bpix(     ntuple_reader_p));}

		if(type_p == "zerobias")
		{
			Cut* zerobias_base_cut = new Cut;
			zerobias_base_cut -> set_cut_function([] ()
			{
				if(!((*cut_zerobias)())) { return false; }
				return true;
			});
			zerobias_base_cut -> set_name("zerobias");
			cut_list.push_back(std::unique_ptr<Cut>(zerobias_base_cut));
			return cut_list.back();
		}
		
		if(type_p == "nvtx")
		{
			Cut* nvtx_base_cut = new Cut;
			nvtx_base_cut -> set_cut_function([] ()
			{
				if(!((*cut_nvtx)()))     { return false; }
				return true;
			});
			nvtx_base_cut -> set_name("nvtx");
			cut_list.push_back(std::unique_ptr<Cut>(nvtx_base_cut));
			return cut_list.back();
		}

		if(type_p == "bpix")
		{
			Cut* bpix_base_cut = new Cut;
			bpix_base_cut -> set_cut_function([] ()
			{
				if(!((*cut_bpix)()))     { return false; }
				return true;
			});
			bpix_base_cut -> set_name("bpix");
			cut_list.push_back(std::unique_ptr<Cut>(bpix_base_cut));
			return cut_list.back();
		}

		if(type_p == "effcut_all")
		{
			Cut* effcut_all = new Cut;
			effcut_all -> set_cut_function([] ()
			{
				if(!((*cut_nvtx)()))     { return false; }
				if(!((*cut_zerobias)())) { return false; }
				if(!((*cut_federr)()))   { return false; }
				if(!((*cut_hp)()))       { return false; }
				if(!((*cut_pt)()))       { return false; }
				if(!((*cut_nstrip)()))   { return false; }
				if(!((*cut_d0)()))       { return false; }
				if(!((*cut_dz)()))       { return false; }
				if(!((*cut_pixhit)()))   { return false; }
				if(!((*cut_noscan)()))   { return false; }
				if(!((*cut_goodmod)()))  { return false; }
				if(!((*cut_lx_fid)()))   { return false; }
				if(!((*cut_ly_fid)()))   { return false; }
				if(!((*cut_valmis)()))   { return false; }
				if(!((*cut_hitsep)()))   { return false; }
				return true;
			});
			effcut_all -> set_name("effcut_all");
			cut_list.push_back(std::unique_ptr<Cut>(effcut_all));
			return cut_list.back();
		}
		if(type_p == "")
		{
			// This part might be overridden in Custom_smart_histos.hh
			Cut* empty_cut = new Cut;
			empty_cut -> set_cut_function([] () {return true;});
			empty_cut -> set_name("");
			cut_list.push_back(std::unique_ptr<Cut>(empty_cut));
			return cut_list.back();
		}

		std::cerr << error_prompt << "Failed to generate cut: " << type_p << ". (Invalid keyword?)" << std::endl;
		exit(-1);
		return cut_list.back(); // never reached
	}

	std::vector<std::unique_ptr<Cut>> Cut_factory::cut_list;
	std::unique_ptr<Cut, Cut_factory::Silent_deleter> Cut_factory::cut_none(nullptr);
	std::unique_ptr<Cut, Cut_factory::Silent_deleter> Cut_factory::cut_d0(nullptr);
	std::unique_ptr<Cut, Cut_factory::Silent_deleter> Cut_factory::cut_dz(nullptr);
	std::unique_ptr<Cut, Cut_factory::Silent_deleter> Cut_factory::cut_federr(nullptr);
	std::unique_ptr<Cut, Cut_factory::Silent_deleter> Cut_factory::cut_zerobias(nullptr);
	std::unique_ptr<Cut, Cut_factory::Silent_deleter> Cut_factory::cut_goodmod(nullptr);
	std::unique_ptr<Cut, Cut_factory::Silent_deleter> Cut_factory::cut_goodroc(nullptr);
	std::unique_ptr<Cut, Cut_factory::Silent_deleter> Cut_factory::cut_hitsep(nullptr);
	std::unique_ptr<Cut, Cut_factory::Silent_deleter> Cut_factory::cut_hp(nullptr);
	std::unique_ptr<Cut, Cut_factory::Silent_deleter> Cut_factory::cut_lx_fid(nullptr);
	std::unique_ptr<Cut, Cut_factory::Silent_deleter> Cut_factory::cut_ly_fid(nullptr);
	std::unique_ptr<Cut, Cut_factory::Silent_deleter> Cut_factory::cut_noscan(nullptr);
	std::unique_ptr<Cut, Cut_factory::Silent_deleter> Cut_factory::cut_nstrip(nullptr);
	std::unique_ptr<Cut, Cut_factory::Silent_deleter> Cut_factory::cut_nvtx(nullptr);
	std::unique_ptr<Cut, Cut_factory::Silent_deleter> Cut_factory::cut_pixhit(nullptr);
	std::unique_ptr<Cut, Cut_factory::Silent_deleter> Cut_factory::cut_pt(nullptr);
	std::unique_ptr<Cut, Cut_factory::Silent_deleter> Cut_factory::cut_valmis(nullptr);
	std::unique_ptr<Cut, Cut_factory::Silent_deleter> Cut_factory::cut_bpix(nullptr);

} // Histogram_generation

#endif