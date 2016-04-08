#ifndef FILL_PARAMETER_FACTORY_H
#define FILL_PARAMETER_FACTORY_H

#include "Fill_parameter_base.hh"
#include "../../Ntuple_reader/Ntuple_reader.hh"

#include "Fill_parameter_recipes/Ladders_fill_parameter.hh"
#include "Fill_parameter_recipes/Modules_fill_parameter.hh"
#include "Fill_parameter_recipes/Hit_efficiency_fill_parameter.hh"
#include "Fill_parameter_recipes/TrajROCx_fill_parameter.hh"
#include "Fill_parameter_recipes/TrajROCy_fill_parameter.hh"
#include "Fill_parameter_recipes/ClustROCx_fill_parameter.hh"
#include "Fill_parameter_recipes/ClustROCy_fill_parameter.hh"
#include "Fill_parameter_recipes/Nvtx_fill_parameter.hh"
#include "Fill_parameter_recipes/Instlumi_fill_parameter.hh"
#include "Fill_parameter_recipes/Pileup_fill_parameter.hh"
#include "Fill_parameter_recipes/Layers_fill_parameter.hh"
#include "Fill_parameter_recipes/ImpactAngleAlpha_fill_parameter.hh"
#include "Fill_parameter_recipes/ImpactAngleBeta_fill_parameter.hh"
#include "Fill_parameter_recipes/ImpactAngleGamma_fill_parameter.hh"
#include "Fill_parameter_recipes/DXCL_fill_parameter.hh"
#include "Fill_parameter_recipes/DYCL_fill_parameter.hh"
#include "Fill_parameter_recipes/DRCL_fill_parameter.hh"

class Fill_parameter_factory
{
	private:
		static std::vector<std::unique_ptr<Fill_parameter>> fill_parameter_list;
	public:
		static std::unique_ptr<Fill_parameter>& get_fill_parameter(const std::string& type_p, Ntuple_reader*& ntuple_reader_p, bool is_mc);
};


std::unique_ptr<Fill_parameter>& Fill_parameter_factory::get_fill_parameter(const std::string& type_p, Ntuple_reader*& ntuple_reader_p, bool is_mc)
{
	if(type_p == "Ladders")
	{
		fill_parameter_list.push_back(std::unique_ptr<Fill_parameter>(new Ladders_fill_parameter(ntuple_reader_p)));
		return fill_parameter_list.back();
	}
	if(type_p == "Modules")
	{
		fill_parameter_list.push_back(std::unique_ptr<Fill_parameter>(new Modules_fill_parameter(ntuple_reader_p)));
		return fill_parameter_list.back();
	}
	if(type_p == "TLadders")
	{
		fill_parameter_list.push_back(std::unique_ptr<Fill_parameter>(new Ladders_fill_parameter(ntuple_reader_p)));
		fill_parameter_list.back() -> set_name("TLadders");
		return fill_parameter_list.back();
	}
	if(type_p == "TModules")
	{
		fill_parameter_list.push_back(std::unique_ptr<Fill_parameter>(new Modules_fill_parameter(ntuple_reader_p)));
		fill_parameter_list.back() -> set_name("TModules");
		return fill_parameter_list.back();
	}
	if(type_p == "HitEfficiency")
	{
		fill_parameter_list.push_back(std::unique_ptr<Fill_parameter>(new Hit_efficiency_fill_parameter(ntuple_reader_p)));
		return fill_parameter_list.back();
	}
	if(type_p == "TrajROCx")
	{
		fill_parameter_list.push_back(std::unique_ptr<Fill_parameter>(new TrajROCx_fill_parameter(ntuple_reader_p)));
		return fill_parameter_list.back();
	}
	if(type_p == "TrajROCy")
	{
		fill_parameter_list.push_back(std::unique_ptr<Fill_parameter>(new TrajROCy_fill_parameter(ntuple_reader_p)));
		return fill_parameter_list.back();
	}
	if(type_p == "ClustROCx")
	{
		fill_parameter_list.push_back(std::unique_ptr<Fill_parameter>(new ClustROCx_fill_parameter(ntuple_reader_p)));
		return fill_parameter_list.back();
	}
	if(type_p == "ClustROCy")
	{
		fill_parameter_list.push_back(std::unique_ptr<Fill_parameter>(new ClustROCy_fill_parameter(ntuple_reader_p)));
		return fill_parameter_list.back();
	}
	if(type_p == "Nvtx")
	{
		fill_parameter_list.push_back(std::unique_ptr<Fill_parameter>(new Nvtx_fill_parameter(ntuple_reader_p)));
		return fill_parameter_list.back();
	}
	if(type_p == "Instlumi")
	{
		fill_parameter_list.push_back(std::unique_ptr<Fill_parameter>(new Instlumi_fill_parameter(ntuple_reader_p)));
		return fill_parameter_list.back();
	}
	if(type_p == "Pileup")
	{
		fill_parameter_list.push_back(std::unique_ptr<Fill_parameter>(new Pileup_fill_parameter(ntuple_reader_p)));
		return fill_parameter_list.back();
	}
	if(type_p == "Layers")
	{
		fill_parameter_list.push_back(std::unique_ptr<Fill_parameter>(new Layers_fill_parameter(ntuple_reader_p)));
		return fill_parameter_list.back();
	}
	if(type_p == "ImpactAngleAlpha")
	{
		fill_parameter_list.push_back(std::unique_ptr<Fill_parameter>(new ImpactAngleAlpha_fill_parameter(ntuple_reader_p)));
		return fill_parameter_list.back();
	}
	if(type_p == "ImpactAngleBeta")
	{
		fill_parameter_list.push_back(std::unique_ptr<Fill_parameter>(new ImpactAngleBeta_fill_parameter(ntuple_reader_p)));
		return fill_parameter_list.back();
	}
	if(type_p == "ImpactAngleGamma")
	{
		fill_parameter_list.push_back(std::unique_ptr<Fill_parameter>(new ImpactAngleGamma_fill_parameter(ntuple_reader_p)));
		return fill_parameter_list.back();
	}
	if(type_p == "DXCL")
	{
		fill_parameter_list.push_back(std::unique_ptr<Fill_parameter>(new DXCL_fill_parameter(ntuple_reader_p)));
		return fill_parameter_list.back();
	}
	if(type_p == "DYCL")
	{
		fill_parameter_list.push_back(std::unique_ptr<Fill_parameter>(new DYCL_fill_parameter(ntuple_reader_p)));
		return fill_parameter_list.back();
	}
	if(type_p == "DRCL")
	{
		fill_parameter_list.push_back(std::unique_ptr<Fill_parameter>(new DRCL_fill_parameter(ntuple_reader_p)));
		return fill_parameter_list.back();
	}
	std::cerr << error_prompt << "Failed to generate postfix: " << type_p << ". (Invalid keyword?)" << std::endl;
	exit(-1);
	return fill_parameter_list.back(); // never reached
}

std::vector<std::unique_ptr<Fill_parameter>> Fill_parameter_factory::fill_parameter_list;

#endif