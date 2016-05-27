#ifndef NTUPLE_READER_SETUP_H
#define NTUPLE_READER_SETUP_H

#include "../Ntuple_reader/Ntuple_reader.hh"

#include "../Histogram_generation/Root_color_sets/root_color_sets.hh"

#include "../Histogram_reweighter/Histogram_reweighter.hh"

#include "../Histogram_generation/Postfixes/Postfix_factory.hh"
#include "../Histogram_generation/Fill_parameters/Fill_parameter_factory.hh"
#include "../Histogram_generation/Cuts/SmartHisto_cut_factory.hh"

// #define ADD_INSTLUMI_PLOTS
// #define ADD_PILEUP_PLOTS

struct Ntuple_reader_setup_options
{
	Ntuple_reader*&                        ntuple_reader;
	std::shared_ptr<Custom_smart_histos>&  histogram_requests;
	std::shared_ptr<Histogram_reweighter>& histogram_reweighter;
	const std::string&                     extra_cut;
	const std::string&                     reweighting_file_path;       
	const std::string&                     reweighting_histogram_name;  
	bool                                   is_mc;
};

int ntuple_reader_setup(Ntuple_reader_setup_options setup_options);

int ntuple_reader_setup(Ntuple_reader_setup_options setup_options)
{
	using Histogram_generation::Cut_factory;

	Ntuple_reader*&                        ntuple_reader              = setup_options.ntuple_reader;
	std::shared_ptr<Histogram_reweighter>& histogram_reweighter       = setup_options.histogram_reweighter;
	std::shared_ptr<Custom_smart_histos>&  histogram_requests         = setup_options.histogram_requests;
	const std::string&                     extra_cut                  = setup_options.extra_cut;
	const std::string&                     reweighting_file_path      = setup_options.reweighting_file_path;
	const std::string&                     reweighting_histogram_name = setup_options.reweighting_histogram_name;
	bool                                   is_mc                      = setup_options.is_mc;
	
	ntuple_reader -> set_histogram_requests(histogram_requests.get());
	histogram_requests -> AddHistoType("eventTree");
	histogram_requests -> AddHistoType("lumiTree");
	histogram_requests -> AddHistoType("runTree");
	histogram_requests -> AddHistoType("clustTree");
	histogram_requests -> AddHistoType("trajTree");
	
	histogram_requests -> AddNewPostfix(Postfix_factory::get_postfix("Validhit", ntuple_reader));
	histogram_requests -> AddNewPostfix(Postfix_factory::get_postfix("Layers", ntuple_reader));
	histogram_requests -> AddNewPostfix(Postfix_factory::get_postfix("Modules", ntuple_reader));
	histogram_requests -> AddNewPostfix(Postfix_factory::get_postfix("Ring12", ntuple_reader));
	histogram_requests -> AddNewPostfix(Postfix_factory::get_postfix("Eta", ntuple_reader));
	histogram_requests -> AddNewPostfix(Postfix_factory::get_postfix("Alpha", ntuple_reader));
	histogram_requests -> AddNewPostfix(Postfix_factory::get_postfix("Beta", ntuple_reader));
	histogram_requests -> AddNewPostfix(Postfix_factory::get_postfix("PassedEffcuts", ntuple_reader));
	histogram_requests -> AddNewPostfix(Postfix_factory::get_postfix("UnambiguousHit", ntuple_reader));

	histogram_requests -> AddSpecial({.name = "HitEfficiency", .name_plus_1d = "ValidHit", .axis = "Hit Efficiency", .axis_plus_1d = "Valid Hit"});

	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("Layers", ntuple_reader, is_mc));
	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("LayersDisks", ntuple_reader, is_mc));
	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("Ladders", ntuple_reader, is_mc));
	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("Modules", ntuple_reader, is_mc));
	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("FPixBlade", ntuple_reader, is_mc));
	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("FPixPanel", ntuple_reader, is_mc));
	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("ClustFPixBlade", ntuple_reader, is_mc));
	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("ClustFPixPanel", ntuple_reader, is_mc));
	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("TLadders", ntuple_reader, is_mc));
	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("TModules", ntuple_reader, is_mc));
	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("TrajROCx", ntuple_reader, is_mc));
	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("TrajROCy", ntuple_reader, is_mc));
	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("ClustROCx", ntuple_reader, is_mc));
	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("ClustROCy", ntuple_reader, is_mc));
	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("HitEfficiency", ntuple_reader, is_mc));
	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("Trkpt", ntuple_reader, is_mc));
	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("Nvtx", ntuple_reader, is_mc));
	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("ImpactAngleAlpha", ntuple_reader, is_mc));
	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("ImpactAngleBeta", ntuple_reader, is_mc));
	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("ImpactAngleGamma", ntuple_reader, is_mc));
	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("DXCL", ntuple_reader, is_mc));
	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("DYCL", ntuple_reader, is_mc));
	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("DRCL", ntuple_reader, is_mc));
	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("DRCLSecondCluster", ntuple_reader, is_mc));
	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("LocalPhi", ntuple_reader, is_mc));

	histogram_requests -> AddNewCut(Cut_factory::get_cut("zerobias", ntuple_reader)); 
	histogram_requests -> AddNewCut(Cut_factory::get_cut("nvtx", ntuple_reader)); 
	histogram_requests -> AddNewCut(Cut_factory::get_cut("effcut_all", ntuple_reader));
	histogram_requests -> AddNewCut(Cut_factory::get_cut("bpix", ntuple_reader));


	histogram_requests -> SetHistoWeights({});
	if(is_mc)
	{
		histogram_requests -> SetHistoWeights({histogram_reweighter -> get_reweighter(reweighting_file_path, reweighting_histogram_name, true)});
	}

	/*__________________________________________1D Histograms____________________________________________*/
	
	histogram_requests -> AddHistos("eventTree", {.fill="Nvtx",      .pfs={},       .cuts={"zerobias"}, .draw="HIST", .opt="", .ranges={0.0, 0.0}});
// Total Hits in Ladder ROCs
	histogram_requests -> AddHistos("clustTree", {.fill="Ladders",   .pfs={"Layers"}, .cuts={extra_cut,"zerobias", "nvtx"}, .draw="HIST", .opt="", .ranges={0.0, 0.0}});  
	histogram_requests -> AddHistos("clustTree", {.fill="ClustROCy", .pfs={"Layers"}, .cuts={extra_cut,"zerobias", "nvtx"}, .draw="HIST", .opt="", .ranges={0.0, 0.0}});  
// Total Hits in Module ROCs
	histogram_requests -> AddHistos("clustTree", {.fill="Modules",   .pfs={"Layers"}, .cuts={extra_cut,"zerobias", "nvtx"}, .draw="HIST", .opt="", .ranges={0.0, 0.0}});  
	histogram_requests -> AddHistos("clustTree", {.fill="ClustROCx", .pfs={"Layers"}, .cuts={extra_cut,"zerobias", "nvtx"}, .draw="HIST", .opt="", .ranges={0.0, 0.0}});  
	
	histogram_requests -> AddHistos("clustTree", {.fill="ClustFPixBlade",   .pfs={}, .cuts={extra_cut, "zerobias", "nvtx"}, .draw="HIST", .opt="", .ranges={0.0, 0.0}});  
	histogram_requests -> AddHistos("clustTree", {.fill="ClustFPixPanel",   .pfs={}, .cuts={extra_cut, "zerobias", "nvtx"}, .draw="HIST", .opt="", .ranges={0.0, 0.0}});  

	histogram_requests -> AddHistos("trajTree", {.fill="FPixBlade",    .pfs={}, .cuts={extra_cut,"zerobias", "nvtx"}, .draw="HIST", .opt="", .ranges={0.0, 0.0}});
	histogram_requests -> AddHistos("trajTree", {.fill="FPixPanel",    .pfs={}, .cuts={extra_cut,"zerobias", "nvtx"}, .draw="HIST", .opt="", .ranges={0.0, 0.0}});

// Ladder ROC efficiencies
	histogram_requests -> AddHistos("trajTree",  {.fill="HitEfficiency_vs_Ladders",  .pfs={"Layers"}, .cuts={extra_cut, "effcut_all"}, .draw="PE1", .opt="", .ranges={0.0, 0.0, 0.90, 1.05 }});  
	histogram_requests -> AddHistos("trajTree",  {.fill="HitEfficiency_vs_TrajROCy", .pfs={"Layers"}, .cuts={extra_cut, "effcut_all"}, .draw="PE1", .opt="", .ranges={0.0, 0.0, 0.90, 1.05 }});  
// Module ROC efficiencies
	histogram_requests -> AddHistos("trajTree",  {.fill="HitEfficiency_vs_Modules",  .pfs={"Layers"}, .cuts={extra_cut, "effcut_all"}, .draw="PE1", .opt="", .ranges={0.0, 0.0, 0.90, 1.05 }});  
	histogram_requests -> AddHistos("trajTree",  {.fill="HitEfficiency_vs_TrajROCx", .pfs={"Layers"}, .cuts={extra_cut, "effcut_all"}, .draw="PE1", .opt="", .ranges={0.0, 0.0, 0.90, 1.05 }});  
// Layer efficiencies
	histogram_requests -> AddHistos("trajTree", {.fill="TModules",     .pfs={}, .cuts={extra_cut,"zerobias", "nvtx"}, .draw="HIST", .opt="", .ranges={0.0, 0.0}});
	histogram_requests -> AddHistos("trajTree", {.fill="TLadders",     .pfs={}, .cuts={extra_cut,"zerobias", "nvtx"}, .draw="HIST", .opt="", .ranges={0.0, 0.0}});
	histogram_requests -> AddHistos("trajTree", {.fill="HitEfficiency_vs_Layers",     		.pfs={        }, .cuts={extra_cut, "effcut_all"}, .draw="HIST", .opt="", .ranges={0.0, 0.0}});  
	histogram_requests -> AddHistos("trajTree", {.fill="HitEfficiency_vs_Layers",     		.pfs={"Ring12"}, .cuts={extra_cut, "effcut_all"}, .draw="HIST", .opt="", .ranges={0.0, 0.0}});  
	histogram_requests -> AddHistos("trajTree", {.fill="HitEfficiency_vs_LayersDisks",     .pfs={        }, .cuts={extra_cut, "effcut_all"}, .draw="HIST", .opt="", .ranges={0.0, 0.0}});  

	histogram_requests -> AddHistos("trajTree", {.fill="Trkpt",     .pfs={}, .cuts={"zerobias", "nvtx"}, .draw="PE1", .opt="", .ranges={0.0, 0.0}});  
	histogram_requests -> AddHistos("trajTree", {.fill="Trkpt",     .pfs={"Layers"}, .cuts={"zerobias", "nvtx"}, .draw="PE1", .opt="", .ranges={0.0, 0.0}});  
	histogram_requests -> AddHistos("trajTree", {.fill="Trkpt",     .pfs={"Layers", "PassedEffcuts"}, .cuts={"zerobias", "nvtx"}, .draw="PE1", .opt="", .ranges={0.0, 0.0}});  
	histogram_requests -> AddHistos("trajTree", {.fill="Trkpt",     .pfs={"PassedEffcuts"}, .cuts={"zerobias", "nvtx"}, .draw="PE1", .opt="", .ranges={0.0, 0.0}});  
	
	histogram_requests -> AddHistos("trajTree",  {.fill="HitEfficiency_vs_ImpactAngleAlpha", .pfs={"Layers"}, .cuts={extra_cut, "effcut_all"}, .draw="PE1", .opt="", .ranges={0.0, 0.0, 0.87, 1.05}});
	histogram_requests -> AddHistos("trajTree",  {.fill="HitEfficiency_vs_ImpactAngleBeta",  .pfs={"Layers"}, .cuts={extra_cut, "effcut_all"}, .draw="PE1", .opt="", .ranges={0.0, 0.0, 0.87, 1.05}});
	histogram_requests -> AddHistos("trajTree",  {.fill="HitEfficiency_vs_ImpactAngleGamma", .pfs={"Layers"}, .cuts={extra_cut, "effcut_all"}, .draw="PE1", .opt="", .ranges={0.0, 0.0, 0.87, 1.05}});

	histogram_requests -> AddHistos("trajTree",  {.fill="LocalPhi", .pfs={}, .cuts={extra_cut, "effcut_all"}, .draw="PE1", .opt="", .ranges={0.0, 0.0, 0.0, 0.0}});
	histogram_requests -> AddHistos("trajTree",  {.fill="LocalPhi", .pfs={"Layers"}, .cuts={extra_cut, "effcut_all"}, .draw="PE1", .opt="", .ranges={0.0, 0.0, 0.0, 0.0}});
	histogram_requests -> AddHistos("trajTree",  {.fill="LocalPhi", .pfs={"Modules", "Layers"}, .cuts={extra_cut, "effcut_all"}, .draw="PE1", .opt="", .ranges={0.0, 0.0, 0.00, 0.00}});

	histogram_requests -> AddHistos("trajTree",  {.fill="HitEfficiency_vs_LocalPhi", .pfs={}, .cuts={extra_cut, "effcut_all"}, .draw="PE1", .opt="", .ranges={0.0, 0.0, 0.87, 1.05}});
	histogram_requests -> AddHistos("trajTree",  {.fill="HitEfficiency_vs_LocalPhi", .pfs={"Layers"}, .cuts={extra_cut, "effcut_all"}, .draw="PE1", .opt="", .ranges={0.0, 0.0, 0.87, 1.05}});
	histogram_requests -> AddHistos("trajTree",  {.fill="HitEfficiency_vs_LocalPhi", .pfs={"Modules", "Layers"}, .cuts={extra_cut, "effcut_all"}, .draw="PE1", .opt="", .ranges={0.0, 0.0, 0.87, 1.05}});

// DXCL, DYCL, DRCL, DRCLSecondCluster
	histogram_requests -> AddHistos("trajTree",  {.fill="DXCL",                      .pfs={},                              .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DYCL",                      .pfs={},                              .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DRCL",                      .pfs={},                              .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});

	histogram_requests -> AddHistos("trajTree",  {.fill="DXCL",                      .pfs={"Layers"},                        .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DYCL",                      .pfs={"Layers"},                        .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DRCL",                      .pfs={"Layers"},                        .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});

	histogram_requests -> AddHistos("trajTree",  {.fill="DXCL",                      .pfs={"Validhit"},                      .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DYCL",                      .pfs={"Validhit"},                      .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DRCL",                      .pfs={"Validhit"},                      .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});

	histogram_requests -> AddHistos("trajTree",  {.fill="DXCL",                      .pfs={"UnambiguousHit"},                .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DYCL",                      .pfs={"UnambiguousHit"},                .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DRCL",                      .pfs={"UnambiguousHit"},                .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});

	histogram_requests -> AddHistos("trajTree",  {.fill="DXCL",                      .pfs={"Layers", "UnambiguousHit"},      .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DYCL",                      .pfs={"Layers", "UnambiguousHit"},      .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DRCL",                      .pfs={"Layers", "UnambiguousHit"},      .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});

	histogram_requests -> AddHistos("trajTree",  {.fill="DXCL",                      .pfs={"Modules", "Layers", "UnambiguousHit"},      .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DYCL",                      .pfs={"Modules", "Layers", "UnambiguousHit"},      .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DRCL",                      .pfs={"Modules", "Layers", "UnambiguousHit"},      .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});

	histogram_requests -> AddHistos("trajTree",  {.fill="DXCL",                      .pfs={"Layers", "Validhit", "UnambiguousHit"},      .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DYCL",                      .pfs={"Layers", "Validhit", "UnambiguousHit"},      .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DRCL",                      .pfs={"Layers", "Validhit", "UnambiguousHit"},      .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});

	histogram_requests -> AddHistos("trajTree",  {.fill="DXCL",                      .pfs={"Eta", "Validhit"},               .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DYCL",                      .pfs={"Eta", "Validhit"},               .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DRCL",                      .pfs={"Eta", "Validhit"},               .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});

	histogram_requests -> AddHistos("trajTree",  {.fill="DXCL",                      .pfs={"Eta", "Layers", "Validhit"},     .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DYCL",                      .pfs={"Eta", "Layers", "Validhit"},     .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DRCL",                      .pfs={"Eta", "Layers", "Validhit"},     .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});

	histogram_requests -> AddHistos("trajTree",  {.fill="DXCL",                      .pfs={"Alpha", "Validhit"},             .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DYCL",                      .pfs={"Alpha", "Validhit"},             .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DRCL",                      .pfs={"Alpha", "Validhit"},             .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});

	histogram_requests -> AddHistos("trajTree",  {.fill="DXCL",                      .pfs={"Alpha", "Layers", "Validhit"},   .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DYCL",                      .pfs={"Alpha", "Layers", "Validhit"},   .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DRCL",                      .pfs={"Alpha", "Layers", "Validhit"},   .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});

	histogram_requests -> AddHistos("trajTree",  {.fill="DXCL",                      .pfs={"Beta", "Validhit"},              .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DYCL",                      .pfs={"Beta", "Validhit"},              .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DRCL",                      .pfs={"Beta", "Validhit"},              .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});

	histogram_requests -> AddHistos("trajTree",  {.fill="DXCL",                      .pfs={"Beta", "Layers", "Validhit"},    .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DYCL",                      .pfs={"Beta", "Layers", "Validhit"},    .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DRCL",                      .pfs={"Beta", "Layers", "Validhit"},    .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});

	histogram_requests -> AddHistos("trajTree",  {.fill="DXCL",                      .pfs={"Layers", "Validhit"},            .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DYCL",                      .pfs={"Layers", "Validhit"},            .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DRCL",                      .pfs={"Layers", "Validhit"},            .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});

	histogram_requests -> AddHistos("trajTree",  {.fill="DXCL",                      .pfs={"Modules", "Layers", "Validhit"}, .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DYCL",                      .pfs={"Modules", "Layers", "Validhit"}, .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DRCL",                      .pfs={"Modules", "Layers", "Validhit"}, .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});

	histogram_requests -> AddHistos("trajTree",  {.fill="DXCL",                      .pfs={"Validhit", "PassedEffcuts"},                      .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DYCL",                      .pfs={"Validhit", "PassedEffcuts"},                      .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.10, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DRCL",                      .pfs={"Validhit", "PassedEffcuts"},                      .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});

	histogram_requests -> AddHistos("trajTree",  {.fill="DXCL",                      .pfs={"Eta", "Validhit", "PassedEffcuts"},               .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DYCL",                      .pfs={"Eta", "Validhit", "PassedEffcuts"},               .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DRCL",                      .pfs={"Eta", "Validhit", "PassedEffcuts"},               .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});

	histogram_requests -> AddHistos("trajTree",  {.fill="DXCL",                      .pfs={"Eta", "Layers", "Validhit", "PassedEffcuts"},     .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DYCL",                      .pfs={"Eta", "Layers", "Validhit", "PassedEffcuts"},     .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DRCL",                      .pfs={"Eta", "Layers", "Validhit", "PassedEffcuts"},     .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});

	histogram_requests -> AddHistos("trajTree",  {.fill="DXCL",                      .pfs={"Alpha", "Validhit", "PassedEffcuts"},             .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DYCL",                      .pfs={"Alpha", "Validhit", "PassedEffcuts"},             .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DRCL",                      .pfs={"Alpha", "Validhit", "PassedEffcuts"},             .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});

	histogram_requests -> AddHistos("trajTree",  {.fill="DXCL",                      .pfs={"Alpha", "Layers", "Validhit", "PassedEffcuts"},   .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DYCL",                      .pfs={"Alpha", "Layers", "Validhit", "PassedEffcuts"},   .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DRCL",                      .pfs={"Alpha", "Layers", "Validhit", "PassedEffcuts"},   .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});

	histogram_requests -> AddHistos("trajTree",  {.fill="DXCL",                      .pfs={"Beta", "Validhit", "PassedEffcuts"},              .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DYCL",                      .pfs={"Beta", "Validhit", "PassedEffcuts"},              .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DRCL",                      .pfs={"Beta", "Validhit", "PassedEffcuts"},              .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});

	histogram_requests -> AddHistos("trajTree",  {.fill="DXCL",                      .pfs={"Beta", "Layers", "Validhit", "PassedEffcuts"},    .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DYCL",                      .pfs={"Beta", "Layers", "Validhit", "PassedEffcuts"},    .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DRCL",                      .pfs={"Beta", "Layers", "Validhit", "PassedEffcuts"},    .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});

	histogram_requests -> AddHistos("trajTree",  {.fill="DXCL",                      .pfs={"Layers", "Validhit", "PassedEffcuts"},            .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DYCL",                      .pfs={"Layers", "Validhit", "PassedEffcuts"},            .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DRCL",                      .pfs={"Layers", "Validhit", "PassedEffcuts"},            .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});

	histogram_requests -> AddHistos("trajTree",  {.fill="DXCL",                      .pfs={"Modules", "Layers", "Validhit", "PassedEffcuts"}, .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DYCL",                      .pfs={"Modules", "Layers", "Validhit", "PassedEffcuts"}, .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DRCL",                      .pfs={"Modules", "Layers", "Validhit", "PassedEffcuts"}, .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});

	histogram_requests -> AddHistos("trajTree",  {.fill="HitEfficiency_vs_DXCL",     .pfs={"Layers", "Validhit", "PassedEffcuts"},            .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="HitEfficiency_vs_DYCL",     .pfs={"Layers", "Validhit", "PassedEffcuts"},            .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="HitEfficiency_vs_DRCL",     .pfs={"Layers", "Validhit", "PassedEffcuts"},            .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="HitEfficiency_vs_DXCL",     .pfs={"Modules", "Validhit", "PassedEffcuts"},           .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="HitEfficiency_vs_DYCL",     .pfs={"Modules", "Validhit", "PassedEffcuts"},           .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="HitEfficiency_vs_DRCL",     .pfs={"Modules", "Validhit", "PassedEffcuts"},           .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="Norm", .ranges={0.0, 2000.0, 0.0, 0.0, 0.5, 0.6}});

	histogram_requests -> AddHistos("trajTree",  {.fill="DRCLSecondCluster",         .pfs={},                   .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="", .ranges={0.0, 20000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DRCLSecondCluster",         .pfs={"Layers"},             .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="", .ranges={0.0, 20000.0, 0.0, 0.0, 0.5, 0.6}});
	histogram_requests -> AddHistos("trajTree",  {.fill="DRCLSecondCluster",         .pfs={"Layers", "Validhit"}, .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="HIST", .opt="", .ranges={0.0, 20000.0, 0.0, 0.0, 0.5, 0.6}});

	/*__________________________________________2D Histograms____________________________________________*/

	// Cluster Occupancy in ROCs
	histogram_requests -> AddHistos("clustTree", {.fill="Ladders_vs_Modules",                      .pfs={"Layers"}, .cuts={extra_cut,"zerobias", "nvtx"}, .draw="COLZ", .opt="", .ranges={0.0, 0.0, 0.0, 0.0}});
	histogram_requests -> AddHistos("clustTree", {.fill="ClustROCy_vs_ClustROCx",                  .pfs={"Layers"}, .cuts={extra_cut,"zerobias", "nvtx"}, .draw="COLZ", .opt="", .ranges={0.0, 0.0, 0.0, 0.0}});
	histogram_requests -> AddHistos("clustTree", {.fill="ClustFPixPanel_vs_ClustFPixBlade",        .pfs={}, .cuts={extra_cut,"zerobias", "nvtx"}, .draw="COLZ", .opt="", .ranges={0.0, 0.0, 0.0, 0.0}});
	// Total Hits in ROCs
	histogram_requests -> AddHistos("trajTree",  {.fill="TLadders_vs_TModules",                    .pfs={"Layers"}, .cuts={extra_cut, "effcut_all"}, .draw="COLZ", .opt="", .ranges={0.0, 0.0, 0.0, 0.0}});
	histogram_requests -> AddHistos("trajTree",  {.fill="TrajROCy_vs_TrajROCx",                    .pfs={"Layers"}, .cuts={extra_cut, "effcut_all"}, .draw="COLZ", .opt="", .ranges={0.0, 0.0, 0.0, 0.0}});
	// Hit Efficiency in ROCs
	histogram_requests -> AddHistos("trajTree",  {.fill="HitEfficiency_vs_Ladders_vs_Modules",     .pfs={"Layers"}, .cuts={extra_cut, "effcut_all"}, .draw="COLZ", .opt="", .ranges={0.0, 0.0, 0.0, 0.0, 0.92, 1}});
	histogram_requests -> AddHistos("trajTree",  {.fill="HitEfficiency_vs_TrajROCy_vs_TrajROCx",   .pfs={"Layers"}, .cuts={extra_cut, "effcut_all"}, .draw="COLZ", .opt="", .ranges={0.0, 0.0, 0.0, 0.0, 0.92, 1}});

	histogram_requests -> AddHistos("trajTree",  {.fill="HitEfficiency_vs_FPixPanel_vs_FPixBlade", .pfs={}, .cuts={extra_cut, "effcut_all"}, .draw="COLZ", .opt="", .ranges={0.0, 0.0, 0.0, 0.0, 0.92, 1}});

	histogram_requests -> AddHistos("trajTree",  {.fill="HitEfficiency_vs_ImpactAngleBeta_vs_ImpactAngleAlpha", .pfs={"Layers"}, .cuts={extra_cut, "effcut_all"}, .draw="COLZ", .opt="", .ranges={0.0, 0.0, 0.0, 0.0, 0.92, 1.0}});
	histogram_requests -> AddHistos("trajTree",  {.fill="HitEfficiency_vs_ImpactAngleBeta_vs_ImpactAngleGamma", .pfs={"Layers"}, .cuts={extra_cut, "effcut_all"}, .draw="COLZ", .opt="", .ranges={0.0, 0.0, 0.0, 0.0, 0.92, 1.0}});
	histogram_requests -> AddHistos("trajTree",  {.fill="ImpactAngleBeta_vs_ImpactAngleAlpha", .pfs={"Layers"}, .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="COLZ", .opt="", .ranges={0.0, 0.0, 0.0, 0.0}});
	histogram_requests -> AddHistos("trajTree",  {.fill="ImpactAngleBeta_vs_ImpactAngleGamma", .pfs={"Layers"}, .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="COLZ", .opt="", .ranges={0.0, 0.0, 0.0, 0.0}});

	histogram_requests -> AddHistos("trajTree", {.fill="DYCL_vs_DXCL",    .pfs={"Layers", "Validhit"}, .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="COLZ", .opt="Norm", .ranges={0.0, 0.0}});
	histogram_requests -> AddHistos("trajTree", {.fill="DXCL_vs_Modules", .pfs={"Layers", "Validhit"}, .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="COLZ", .opt="Norm", .ranges={0.0, 0.0}});
	histogram_requests -> AddHistos("trajTree", {.fill="DYCL_vs_Modules", .pfs={"Layers", "Validhit"}, .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="COLZ", .opt="Norm", .ranges={0.0, 0.0}});
	histogram_requests -> AddHistos("trajTree", {.fill="DRCL_vs_Modules", .pfs={"Layers", "Validhit"}, .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="COLZ", .opt="Norm", .ranges={0.0, 0.0}});
	histogram_requests -> AddHistos("trajTree", {.fill="DXCL_vs_Ladders", .pfs={"Layers", "Validhit"}, .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="COLZ", .opt="Norm", .ranges={0.0, 0.0}});
	histogram_requests -> AddHistos("trajTree", {.fill="DYCL_vs_Ladders", .pfs={"Layers", "Validhit"}, .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="COLZ", .opt="Norm", .ranges={0.0, 0.0}});
	histogram_requests -> AddHistos("trajTree", {.fill="DRCL_vs_Ladders", .pfs={"Layers", "Validhit"}, .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="COLZ", .opt="Norm", .ranges={0.0, 0.0}});
	histogram_requests -> AddHistos("trajTree", {.fill="HitEfficiency_vs_DYCL_vs_DXCL",    .pfs={"Layers", "Validhit"}, .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="COLZ", .opt="Norm", .ranges={0.0, 0.0}});
	histogram_requests -> AddHistos("trajTree", {.fill="HitEfficiency_vs_DXCL_vs_Modules", .pfs={"Layers", "Validhit"}, .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="COLZ", .opt="Norm", .ranges={0.0, 0.0}});
	histogram_requests -> AddHistos("trajTree", {.fill="HitEfficiency_vs_DYCL_vs_Modules", .pfs={"Layers", "Validhit"}, .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="COLZ", .opt="Norm", .ranges={0.0, 0.0}});
	histogram_requests -> AddHistos("trajTree", {.fill="HitEfficiency_vs_DRCL_vs_Modules", .pfs={"Layers", "Validhit"}, .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="COLZ", .opt="Norm", .ranges={0.0, 0.0}});
	histogram_requests -> AddHistos("trajTree", {.fill="HitEfficiency_vs_DXCL_vs_Ladders", .pfs={"Layers", "Validhit"}, .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="COLZ", .opt="Norm", .ranges={0.0, 0.0}});
	histogram_requests -> AddHistos("trajTree", {.fill="HitEfficiency_vs_DYCL_vs_Ladders", .pfs={"Layers", "Validhit"}, .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="COLZ", .opt="Norm", .ranges={0.0, 0.0}});
	histogram_requests -> AddHistos("trajTree", {.fill="HitEfficiency_vs_DRCL_vs_Ladders", .pfs={"Layers", "Validhit"}, .cuts={extra_cut,"zerobias", "nvtx", "bpix"}, .draw="COLZ", .opt="Norm", .ranges={0.0, 0.0}});

#ifdef ADD_INSTLUMI_PLOTS
	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("Instlumi", ntuple_reader, is_mc));

	histogram_requests -> AddHistos("eventTree",{.fill="Instlumi",            .pfs={},         .cuts={"zerobias", "nvtx"}, .draw="HIST", .opt="", .ranges={0.0, 0.0}});
	histogram_requests -> AddHistos("trajTree", {.fill="Instlumi",            .pfs={"Layers"}, .cuts={extra_cut, "effcut_all"}, .draw="PE1", .opt="", .ranges={0.0, 0.0}});  
	histogram_requests -> AddHistos("trajTree", {.fill="Instlumi_vs_Ladders", .pfs={"Layers"}, .cuts={extra_cut,"zerobias", "nvtx"}, .draw="COLZ", .opt="", .ranges={0.0, 0.0, 0.0, 0.0}});
	histogram_requests -> AddHistos("trajTree", {.fill="Instlumi_vs_Modules", .pfs={"Layers"}, .cuts={extra_cut,"zerobias", "nvtx"}, .draw="COLZ", .opt="", .ranges={0.0, 0.0, 0.0, 0.0}});
	// histogram_requests -> AddHistos("eventTree", {.fill="Instlumi", .pfs={"Layers"}, .cuts={}, .draw="COLZ", .opt="", .ranges={0.0, 0.0}});  
	// histogram_requests -> AddHistos("trajTree",  {.fill="HitEfficiency_vs_Instlumi", .pfs={"Layers"}, .cuts={extra_cut, "effcut_all"}, .draw="COLZ", .opt="", .ranges={0.0, 0.0, 0.90, 1.05}});
	// histogram_requests -> AddHistos("clustTree", {.fill="Instlumi_vs_Ladders_vs_Modules", .pfs={"Layers"}, .cuts={extra_cut,"zerobias", "nvtx"}, .draw="COLZ", .opt="", .ranges={0.0, 0.0, 0.0, 0.0}});  
	// histogram_requests -> AddHistos("clustTree", {.fill="Instlumi_vs_ClustROCy_vs_ClustROCx", .pfs={"Layers"}, .cuts={extra_cut,"zerobias", "nvtx"}, .draw="COLZ", .opt="", .ranges={0.0, 0.0, 0.0, 0.0}});  
#endif


#ifdef ADD_PILEUP_PLOTS
	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("Pileup", ntuple_reader, is_mc));

	histogram_requests -> AddHistos("eventTree", {.fill="Pileup",    .pfs={},       .cuts={"zerobias", "nvtx"}, .draw="HIST", .opt="", .ranges={0.0, 0.0}});
	histogram_requests -> AddHistos("trajTree",  {.fill="HitEfficiency_vs_Instlumi", .pfs={"Layers", "Ring12"}, .cuts={extra_cut, "effcut_all"}, .draw="COLZ", .opt="", .ranges={0.0, 0.0, 0.90, 1.05}});
	histogram_requests -> AddHistos("trajTree",  {.fill="HitEfficiency_vs_Pileup",   .pfs={"Layers"},           .cuts={extra_cut, "effcut_all"}, .draw="COLZ", .opt="", .ranges={0.0, 0.0, 0.90, 1.05}});
#endif

	return 0;
}

#endif
