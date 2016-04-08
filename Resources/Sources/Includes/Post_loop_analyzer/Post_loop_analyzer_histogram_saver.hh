#ifndef POST_LOOP_ANALYZER_HISTOGRAM_SAVER_H
#define POST_LOOP_ANALYZER_HISTOGRAM_SAVER_H

#include "Post_loop_analyzer_data_structure.hh"
#include "../Console_actor/common_actors.hh"

#include "TCanvas.h"
#include "TPad.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TF1.h"

#include <iomanip>
#include <sstream>
#include <memory>

extern bool debug;
extern bool debug_prompt_destructors;
extern bool debug_prompt_histogram_saving;

class Post_loop_analyzer_histogram_saver : 
virtual protected Post_loop_analyzer_data_structure
{
	protected:

template <typename T>
		void check_set_name_title(T plot_ptr, const std::string& name, const std::string& title);
		// Drawing helpers
		void set_canvas_style_basic(TCanvas* canvas_p);
		void set_graphics_and_draw(TVirtualPad* pad_p, TH2D* histogram_p);
		void set_graphics_and_draw(TVirtualPad* pad_p, TH1D* histogram_p);
		void create_complex_directory(TFile* output_file_p, const std::string& main_directory_name_p, const std::string& histograms_directory_name_p, const std::string& canvases_directory_name_p);
		void save_histograms_from_vector(const std::vector<TH1*>& histogram_vector_p);
		void save_histograms_from_vector_as_canvases(const std::vector<TH1*>& histogram_vector_p);
		void save_histogram_as_canvas(TH1D* histogram_p);
		void save_histogram_as_canvas(TH2D* histogram_p);
		void save_histogram_as_canvas(TF1*  histogram_p);
		// Canvas desings
		template<typename TH_type_1, typename TH_type_2>
		TCanvas* create_two_by_one_canvas(TH_type_1 histogram_1_p, TH_type_2 histogram_2, const std::string& canvas_name_p, const std::string& canvas_title_p);
		template<typename TH_type_1, typename TH_type_2>
		void save_as_two_by_one_canvas(TH_type_1 histogram_1_p, TH_type_2 histogram_2_p, TFile* output_file_p, const std::string& directory_name_p, const std::string& canvas_name_p, const std::string& canvas_title_p);
		template<typename TH_type_1, typename TH_type_2, typename TH_type_3, typename TH_type_4>
		TCanvas* create_two_by_two_canvas(TH_type_1 histogram_1_p, TH_type_2 histogram_2_p, TH_type_3 histogram_3_p, TH_type_4 histogram_4_p, const std::string& canvas_name_p, const std::string& canvas_title_p);
		template<typename TH_type_1, typename TH_type_2, typename TH_type_3, typename TH_type_4>
		void save_as_two_by_two_canvas(TH_type_1 histogram_1_p, TH_type_2 histogram_2_p, TH_type_3 histogram_3_p, TH_type_4 histogram_4_p, TFile* output_file_p, const std::string& directory_name_p, const std::string& canvas_name_p, const std::string& canvas_title_p);
		template<typename TH_type_1, typename TH_type_2, typename TH_type_3, typename TH_type_4, typename TH_type_5, typename TH_type_6>
		TCanvas* create_two_by_three_canvas(TH_type_1 histogram_1_p, TH_type_2 histogram_2_p, TH_type_3 histogram_3_p, TH_type_4 histogram_4_p, TH_type_5 histogram_5_p, TH_type_6 histogram_6_p, const std::string& canvas_name_p, const std::string& canvas_title_p);
		template<typename TH_type_1, typename TH_type_2, typename TH_type_3, typename TH_type_4, typename TH_type_5, typename TH_type_6>
		void save_as_two_by_three_canvas(TH_type_1 histogram_1_p, TH_type_2 histogram_2_p, TH_type_3 histogram_3_p, TH_type_4 histogram_4_p, TH_type_5 histogram_5_p, TH_type_6 histogram_6_p, TFile* output_file_p, const std::string& directory_name_p, const std::string& canvas_name_p, const std::string& canvas_title_p);
		template<typename TH_type_1, typename TH_type_2, typename TH_type_3, typename TH_type_4, typename TH_type_5, typename TH_type_6>
		TCanvas* create_three_by_two_canvas(TH_type_1 histogram_1_p, TH_type_2 histogram_2_p, TH_type_3 histogram_3_p, TH_type_4 histogram_4_p, TH_type_5 histogram_5_p, TH_type_6 histogram_6_p, const std::string& canvas_name_p, const std::string& canvas_title_p);
		template<typename TH_type_1, typename TH_type_2, typename TH_type_3, typename TH_type_4, typename TH_type_5, typename TH_type_6>
		void save_as_three_by_two_canvas(TH_type_1 histogram_1_p, TH_type_2 histogram_2_p, TH_type_3 histogram_3_p, TH_type_4 histogram_4_p, TH_type_5 histogram_5_p, TH_type_6 histogram_6_p, TFile* output_file_p, const std::string& directory_name_p, const std::string& canvas_name_p, const std::string& canvas_title_p);

		// Utility
		void autoset_maximum(TH2D* histogram_p);
		// Saving helpers
		void prepare_to_save_histograms();
		void prepare_to_save_canvases();
		void prepare_to_saving();
	public:
		Post_loop_analyzer_histogram_saver();
		~Post_loop_analyzer_histogram_saver();
		void save_histograms(const std::string& filename_p, const std::string& options_p = "");
};

Post_loop_analyzer_histogram_saver::Post_loop_analyzer_histogram_saver()
{

}

Post_loop_analyzer_histogram_saver::~Post_loop_analyzer_histogram_saver()
{
	if(debug_prompt_destructors)
	{
		std::cerr << debug_prompt << "~Post_loop_analyzer_histogram_saver()" << std::endl;
	}
}

/******************************************************************************************************/
/******************************************************************************************************/
/*                                                                                                    */
/*                                           Private Methods                                          */
/*                                                                                                    */
/******************************************************************************************************/
/******************************************************************************************************/

template <typename T>
void Post_loop_analyzer_histogram_saver::check_set_name_title(T plot_ptr, const std::string& name, const std::string& title)
{
	if(plot_ptr == nullptr)
	{
		std::cerr << error_prompt << "Setting name of \"" << name << "\" cannot be done without initializating the histogram first." << std::endl;
	}
	plot_ptr -> SetNameTitle(name.c_str(), title.c_str());
}

void Post_loop_analyzer_histogram_saver::set_canvas_style_basic(TCanvas* canvas_p)
{
	//std::cerr << debug_prompt << "Useless canvas style change...?" << std::endl;
}

void Post_loop_analyzer_histogram_saver::set_graphics_and_draw(TVirtualPad* pad_p, TH2D* histogram_p)
{
	pad_p -> cd();
	histogram_p -> Draw("COLZ");
	pad_p -> Update();
	TPaletteAxis *palette = (TPaletteAxis*)(histogram_p -> GetListOfFunctions() -> FindObject("palette"));
	if(palette)
	{
		palette -> SetY2NDC(0.67);
		palette -> SetX1NDC(0.91);
		palette -> SetX2NDC(0.935);
	}
	else
	{
		/**
		 * The relative cluster efficiency seems to have a bugged palette...
		 */
		std::cerr << error_prompt << "Palette seems to be missing for one of the plots..." << std::endl;
	}
	pad_p     -> SetRightMargin(0.105);
	pad_p -> Update();
}

void Post_loop_analyzer_histogram_saver::set_graphics_and_draw(TVirtualPad* pad_p, TH1D* histogram_p)
{
	pad_p -> cd();
	histogram_p -> Draw("LPE1");
	pad_p     -> SetRightMargin(0.105);
}

void Post_loop_analyzer_histogram_saver::create_complex_directory(TFile* output_file_p, const std::string& main_directory_name_p,
	                                                 const std::string& histograms_directory_name_p, const std::string& canvases_directory_name_p)
{
	if(! (output_file_p -> Get(main_directory_name_p.c_str())))
	{
		output_file_p -> mkdir(main_directory_name_p.c_str());
	}
	if(! (output_file_p -> Get(histograms_directory_name_p.c_str())))
	{
		output_file_p -> mkdir(histograms_directory_name_p.c_str());
	}
	if(! (output_file_p -> Get(canvases_directory_name_p.c_str())))
	{
		output_file_p -> mkdir(canvases_directory_name_p.c_str());
	}
}

void Post_loop_analyzer_histogram_saver::save_histograms_from_vector(const std::vector<TH1*>& histogram_vector_p)
{
	for(auto& histogram: histogram_vector_p)
	{
		histogram -> Write();
	}
}

/*void Post_loop_analyzer_histogram_saver::save_histograms_from_vector_as_canvases(const std::vector<TH1*>& histogram_vector_p)
{
	for(auto& histogram: histogram_vector_p)
	{
		this -> save_histogram_as_canvas(histogram);
	}
}*/

void Post_loop_analyzer_histogram_saver::save_histogram_as_canvas(TH1D* histogram_p)
{
	std::string canvas_name(histogram_p -> GetName());
	canvas_name += "_canvas";
	std::string canvas_title(histogram_p -> GetTitle());
	canvas_title += "_canvas";
	if(!(gDirectory -> Get(canvas_name.c_str())))
	{
		TCanvas* canvas = new TCanvas(canvas_name.c_str(), canvas_title.c_str(), 500, 500);
		canvas -> cd();
		histogram_p -> Draw("LPE1");
		canvas -> Write();
	}
}

void Post_loop_analyzer_histogram_saver::save_histogram_as_canvas(TF1* histogram_p)
{
	std::string canvas_name(histogram_p -> GetName());
	canvas_name += "_canvas";
	std::string canvas_title(histogram_p -> GetTitle());
	canvas_title += "_canvas";
	if(!(gDirectory -> Get(canvas_name.c_str())))
	{
		TCanvas* canvas = new TCanvas(canvas_name.c_str(), canvas_title.c_str(), 500, 500);
		canvas -> cd();
		histogram_p -> Draw("LPE1");
		canvas -> Write();
	}
}

void Post_loop_analyzer_histogram_saver::save_histogram_as_canvas(TH2D* histogram_p)
{
	std::string canvas_name(histogram_p -> GetName());
	canvas_name += "_canvas";
	std::string canvas_title(histogram_p -> GetTitle());
	canvas_title += "_canvas";
	if(!(gDirectory -> Get(canvas_name.c_str())))
	{
		TCanvas* canvas = new TCanvas(canvas_name.c_str(), canvas_title.c_str(), 500, 500);
		canvas -> cd();
		histogram_p -> Draw("COLZ");
		canvas -> Write();
	}
}

template<typename TH_type_1, typename TH_type_2>
TCanvas* Post_loop_analyzer_histogram_saver::create_two_by_one_canvas(TH_type_1 histogram_1_p, TH_type_2 histogram_2_p, const std::string& canvas_name_p, const std::string& canvas_title_p)
{
	TCanvas* canvas = new TCanvas(canvas_name_p.c_str(), canvas_title_p.c_str(), 1000, 500);
	canvas -> Divide(2, 1);
	gStyle -> SetPalette(1);
	this -> set_canvas_style_basic(canvas);
	TVirtualPad* pad_1 = canvas -> cd(1);
	this -> set_graphics_and_draw(pad_1, histogram_1_p);
	TVirtualPad* pad_2 = canvas -> cd(2);
	this -> set_graphics_and_draw(pad_2, histogram_2_p);
	canvas -> Update();
	return canvas;
	return nullptr;
}

template<typename TH_type_1, typename TH_type_2>
void Post_loop_analyzer_histogram_saver::save_as_two_by_one_canvas(TH_type_1 histogram_1_p, TH_type_2 histogram_2_p, 
	                                                  TFile* output_file_p, const std::string& directory_name_p, const std::string& canvas_name_p, const std::string& canvas_title_p)
{
	if(!histogram_1_p || !histogram_2_p)
	{
		std::cerr << error_prompt << "One or both of the histograms in function save_as_two_by_one_canvas() seem to be invalid or missing." << std::endl;
		return;
	}
	TCanvas* main_canvas = this -> create_two_by_one_canvas<TH_type_1, TH_type_2>(histogram_1_p, histogram_2_p, canvas_name_p, canvas_title_p);
	std::string main_directory_name(directory_name_p);
	std::string histograms_directory_name(directory_name_p);
	std::string canvases_directory_name(directory_name_p);
	histograms_directory_name += "/Histograms";
	canvases_directory_name += "/Canvases";
	this -> create_complex_directory(output_file_p, main_directory_name, histograms_directory_name, canvases_directory_name);
	output_file_p -> cd(main_directory_name.c_str());
	main_canvas -> Write();
	output_file_p -> cd(histograms_directory_name.c_str());
	std::vector<TH1*> histograms = { histogram_1_p, histogram_2_p };
	this -> save_histograms_from_vector(histograms);
	output_file_p -> cd(canvases_directory_name.c_str());
	this -> save_histogram_as_canvas(histogram_1_p);
	this -> save_histogram_as_canvas(histogram_2_p);
}


template<typename TH_type_1, typename TH_type_2, typename TH_type_3, typename TH_type_4>
TCanvas* Post_loop_analyzer_histogram_saver::create_two_by_two_canvas(TH_type_1 histogram_1_p, TH_type_2 histogram_2_p, TH_type_3 histogram_3_p, TH_type_4 histogram_4_p, 
	                                                     const std::string& canvas_name_p, const std::string& canvas_title_p)
{
	TCanvas* canvas = new TCanvas(canvas_name_p.c_str(), canvas_title_p.c_str(), 1000, 1000);
	canvas -> Divide(2, 2);
	gStyle -> SetPalette(1);
	this -> set_canvas_style_basic(canvas);
	TVirtualPad* pad_1 = canvas -> cd(1);
	this -> set_graphics_and_draw(pad_1, histogram_1_p);
	TVirtualPad* pad_2 = canvas -> cd(2);
	this -> set_graphics_and_draw(pad_2, histogram_2_p);
	TVirtualPad* pad_3 = canvas -> cd(3);
	this -> set_graphics_and_draw(pad_3, histogram_3_p);
	TVirtualPad* pad_4 = canvas -> cd(4);
	this -> set_graphics_and_draw(pad_4, histogram_4_p);
	canvas -> Update();
	return canvas;
}

template<typename TH_type_1, typename TH_type_2, typename TH_type_3, typename TH_type_4>
void Post_loop_analyzer_histogram_saver::save_as_two_by_two_canvas(TH_type_1 histogram_1_p, TH_type_2 histogram_2_p, TH_type_3 histogram_3_p, TH_type_4 histogram_4_p,
	                                                                  TFile* output_file_p, const std::string& directory_name_p, const std::string& canvas_name_p, const std::string& canvas_title_p)
{
	if(!histogram_1_p || !histogram_2_p || !histogram_3_p || !histogram_4_p)
	{
		std::cerr << error_prompt << "One or more of the histograms in function save_as_two_by_two_canvas() seem to be invalid or missing." << std::endl;
		return;
	}
	TCanvas* main_canvas = this -> create_two_by_two_canvas<TH_type_1, TH_type_2, TH_type_3, TH_type_4>(histogram_1_p, histogram_2_p, histogram_3_p, histogram_4_p, canvas_name_p, canvas_title_p);
	std::string main_directory_name(directory_name_p);
	std::string histograms_directory_name(directory_name_p);
	std::string canvases_directory_name(directory_name_p);
	histograms_directory_name += "/Histograms";
	canvases_directory_name += "/Canvases";
	this -> create_complex_directory(output_file_p, main_directory_name, histograms_directory_name, canvases_directory_name);
	output_file_p -> cd(main_directory_name.c_str());
	main_canvas -> Write();
	output_file_p -> cd(histograms_directory_name.c_str());
	std::vector<TH1*> histograms = { histogram_1_p, histogram_2_p, histogram_3_p, histogram_4_p };
	this -> save_histograms_from_vector(histograms);
	output_file_p -> cd(canvases_directory_name.c_str());
	this -> save_histogram_as_canvas(histogram_1_p);
	this -> save_histogram_as_canvas(histogram_2_p);
	this -> save_histogram_as_canvas(histogram_3_p);
	this -> save_histogram_as_canvas(histogram_4_p);
}

template<typename TH_type_1, typename TH_type_2, typename TH_type_3, typename TH_type_4, typename TH_type_5, typename TH_type_6>
TCanvas* Post_loop_analyzer_histogram_saver::create_two_by_three_canvas(TH_type_1 histogram_1_p, TH_type_2 histogram_2_p, TH_type_3 histogram_3_p, TH_type_4 histogram_4_p, TH_type_5 histogram_5_p, TH_type_6 histogram_6_p,
	                                                     const std::string& canvas_name_p, const std::string& canvas_title_p)
{
	TCanvas* canvas = new TCanvas(canvas_name_p.c_str(), canvas_title_p.c_str(), 750, 1000);
	canvas -> Divide(2, 3);
	gStyle -> SetPalette(1);
	this -> set_canvas_style_basic(canvas);
	TVirtualPad* pad_1 = canvas -> cd(1);
	this -> set_graphics_and_draw(pad_1, histogram_1_p);
	TVirtualPad* pad_2 = canvas -> cd(2);
	this -> set_graphics_and_draw(pad_2, histogram_2_p);
	TVirtualPad* pad_3 = canvas -> cd(3);
	this -> set_graphics_and_draw(pad_3, histogram_3_p);
	TVirtualPad* pad_4 = canvas -> cd(4);
	this -> set_graphics_and_draw(pad_4, histogram_4_p);
	TVirtualPad* pad_5 = canvas -> cd(5);
	this -> set_graphics_and_draw(pad_5, histogram_5_p);
	TVirtualPad* pad_6 = canvas -> cd(6);
	this -> set_graphics_and_draw(pad_6, histogram_6_p);
	canvas -> Update();
	return canvas;
}

template<typename TH_type_1, typename TH_type_2, typename TH_type_3, typename TH_type_4, typename TH_type_5, typename TH_type_6>
void Post_loop_analyzer_histogram_saver::save_as_two_by_three_canvas(TH_type_1 histogram_1_p, TH_type_2 histogram_2_p, TH_type_3 histogram_3_p, TH_type_4 histogram_4_p, TH_type_5 histogram_5_p, TH_type_6 histogram_6_p,
	                                                                  TFile* output_file_p, const std::string& directory_name_p, const std::string& canvas_name_p, const std::string& canvas_title_p)
{
	if(!histogram_1_p || !histogram_2_p || !histogram_3_p || !histogram_4_p || !histogram_5_p || !histogram_6_p)
	{
		std::cerr << error_prompt << "One or more of the histograms in function save_as_three_by_two_canvas() seem to be invalid or missing." << std::endl;
		return;
	}
	TCanvas* main_canvas = this -> create_two_by_three_canvas<TH_type_1, TH_type_2, TH_type_3, TH_type_4, TH_type_5, TH_type_6>(histogram_1_p, histogram_2_p, histogram_3_p, histogram_4_p, histogram_5_p, histogram_6_p, canvas_name_p, canvas_title_p);
	std::string main_directory_name(directory_name_p);
	std::string histograms_directory_name(directory_name_p);
	std::string canvases_directory_name(directory_name_p);
	histograms_directory_name += "/Histograms";
	canvases_directory_name += "/Canvases";
	this -> create_complex_directory(output_file_p, main_directory_name, histograms_directory_name, canvases_directory_name);
	output_file_p -> cd(main_directory_name.c_str());
	main_canvas -> Write();
	output_file_p -> cd(histograms_directory_name.c_str());
	std::vector<TH1*> histograms = { histogram_1_p, histogram_2_p, histogram_3_p, histogram_4_p, histogram_5_p, histogram_6_p };
	this -> save_histograms_from_vector(histograms);
	output_file_p -> cd(canvases_directory_name.c_str());
	this -> save_histogram_as_canvas(histogram_1_p);
	this -> save_histogram_as_canvas(histogram_2_p);
	this -> save_histogram_as_canvas(histogram_3_p);
	this -> save_histogram_as_canvas(histogram_4_p);
	this -> save_histogram_as_canvas(histogram_5_p);
	this -> save_histogram_as_canvas(histogram_6_p);
}

template<typename TH_type_1, typename TH_type_2, typename TH_type_3, typename TH_type_4, typename TH_type_5, typename TH_type_6>
TCanvas* Post_loop_analyzer_histogram_saver::create_three_by_two_canvas(TH_type_1 histogram_1_p, TH_type_2 histogram_2_p, TH_type_3 histogram_3_p, TH_type_4 histogram_4_p, TH_type_5 histogram_5_p, TH_type_6 histogram_6_p,
	                                                     const std::string& canvas_name_p, const std::string& canvas_title_p)
{
	TCanvas* canvas = new TCanvas(canvas_name_p.c_str(), canvas_title_p.c_str(), 1500, 1000);
	canvas -> Divide(3, 2);
	gStyle -> SetPalette(1);
	this -> set_canvas_style_basic(canvas);
	TVirtualPad* pad_1 = canvas -> cd(1);
	this -> set_graphics_and_draw(pad_1, histogram_1_p);
	TVirtualPad* pad_2 = canvas -> cd(2);
	this -> set_graphics_and_draw(pad_2, histogram_2_p);
	TVirtualPad* pad_3 = canvas -> cd(3);
	this -> set_graphics_and_draw(pad_3, histogram_3_p);
	TVirtualPad* pad_4 = canvas -> cd(4);
	this -> set_graphics_and_draw(pad_4, histogram_4_p);
	TVirtualPad* pad_5 = canvas -> cd(5);
	this -> set_graphics_and_draw(pad_5, histogram_5_p);
	TVirtualPad* pad_6 = canvas -> cd(6);
	this -> set_graphics_and_draw(pad_6, histogram_6_p);
	canvas -> Update();
	return canvas;
}

template<typename TH_type_1, typename TH_type_2, typename TH_type_3, typename TH_type_4, typename TH_type_5, typename TH_type_6>
void Post_loop_analyzer_histogram_saver::save_as_three_by_two_canvas(TH_type_1 histogram_1_p, TH_type_2 histogram_2_p, TH_type_3 histogram_3_p, TH_type_4 histogram_4_p, TH_type_5 histogram_5_p, TH_type_6 histogram_6_p,
	                                                                  TFile* output_file_p, const std::string& directory_name_p, const std::string& canvas_name_p, const std::string& canvas_title_p)
{
	if(!histogram_1_p || !histogram_2_p || !histogram_3_p || !histogram_4_p || !histogram_5_p || !histogram_6_p)
	{
		std::cerr << error_prompt << "One or more of the histograms in function save_as_three_by_two_canvas() seem to be invalid or missing." << std::endl;
		return;
	}
	TCanvas* main_canvas = this -> create_three_by_two_canvas<TH_type_1, TH_type_2, TH_type_3, TH_type_4, TH_type_5, TH_type_6>(histogram_1_p, histogram_2_p, histogram_3_p, histogram_4_p, histogram_5_p, histogram_6_p, canvas_name_p, canvas_title_p);
	std::string main_directory_name(directory_name_p);
	std::string histograms_directory_name(directory_name_p);
	std::string canvases_directory_name(directory_name_p);
	histograms_directory_name += "/Histograms";
	canvases_directory_name += "/Canvases";
	this -> create_complex_directory(output_file_p, main_directory_name, histograms_directory_name, canvases_directory_name);
	output_file_p -> cd(main_directory_name.c_str());
	main_canvas -> Write();
	output_file_p -> cd(histograms_directory_name.c_str());
	std::vector<TH1*> histograms = { histogram_1_p, histogram_2_p, histogram_3_p, histogram_4_p, histogram_5_p, histogram_6_p };
	this -> save_histograms_from_vector(histograms);
	output_file_p -> cd(canvases_directory_name.c_str());
	this -> save_histogram_as_canvas(histogram_1_p);
	this -> save_histogram_as_canvas(histogram_2_p);
	this -> save_histogram_as_canvas(histogram_3_p);
	this -> save_histogram_as_canvas(histogram_4_p);
	this -> save_histogram_as_canvas(histogram_5_p);
	this -> save_histogram_as_canvas(histogram_6_p);
}

void Post_loop_analyzer_histogram_saver::autoset_maximum(TH2D* histogram_p)
{
	const double padding_extra = 0.2;
	int max_bin;
	int max_bin_x;
	int max_bin_y;
	int max_bin_z;
	double maximum_value;
	max_bin = histogram_p -> GetMaximumBin();
	histogram_p -> GetBinXYZ(max_bin, max_bin_x, max_bin_y, max_bin_z);
	maximum_value = histogram_p -> GetBinContent(max_bin_x, max_bin_y);
	histogram_p -> GetZaxis() -> SetRangeUser(0.0, maximum_value * (1.0 + padding_extra));
}

void Post_loop_analyzer_histogram_saver::prepare_to_save_histograms()
{
	if(debug_prompt_histogram_saving)
	{
		std::cerr << debug_prompt << "Saving: step 1." << std::endl;
	}
	this -> check_set_name_title(hit_efficiency_on_modules_map_layer_1, "hit_efficiency_on_modules_layer_1", "Hit efficiency on modules layer 1");
	this -> check_set_name_title(total_hits_on_modules_map_layer_1, "total_hits_on_modules_layer_1", "Total hits on modules layer 1");
	this -> check_set_name_title(clusters_on_modules_map_layer_1, "clusters_on_modules_layer_1", "Clusters on modules layer 1");
	this -> check_set_name_title(hit_efficiency_on_modules_map_layer_2, "hit_efficiency_on_modules_layer_2", "Hit efficiency on modules layer 2");
	this -> check_set_name_title(total_hits_on_modules_map_layer_2, "total_hits_on_modules_layer_2", "Total hits on modules layer 2");
	this -> check_set_name_title(clusters_on_modules_map_layer_2, "clusters_on_modules_layer_2", "Clusters on modules layer 2");
	this -> check_set_name_title(hit_efficiency_on_modules_map_layer_3, "hit_efficiency_on_modules_layer_3", "Hit efficiency on modules layer 3");
	this -> check_set_name_title(total_hits_on_modules_map_layer_3, "total_hits_on_modules_layer_3", "Total hits on modules layer 3");
	this -> check_set_name_title(clusters_on_modules_map_layer_3, "clusters_on_modules_layer_3", "Clusters on modules layer 3");
	this -> hit_efficiency_on_modules_map_layer_1             -> GetYaxis() -> SetRangeUser(-10.5, 10.5);
	this -> total_hits_on_modules_map_layer_1                 -> GetYaxis() -> SetRangeUser(-10.5, 10.5);
	this -> clusters_on_modules_map_layer_1                   -> GetYaxis() -> SetRangeUser(-10.5, 10.5);
	this -> hit_efficiency_on_modules_map_layer_2             -> GetYaxis() -> SetRangeUser(-16.5, 16.5);
	this -> total_hits_on_modules_map_layer_2                 -> GetYaxis() -> SetRangeUser(-16.5, 16.5);
	this -> clusters_on_modules_map_layer_2                   -> GetYaxis() -> SetRangeUser(-16.5, 16.5);
	
	this -> check_set_name_title(hit_efficiency_on_ROCs_map_layer_1, "hit_efficiency_on_ROCs_layer_1", "Hit efficiency on ROCs layer 1");
	this -> check_set_name_title(total_hits_on_ROCs_map_layer_1, "total_hits_on_ROCs_layer_1", "Total hits on ROCs layer 1");
	this -> check_set_name_title(clusters_on_ROCs_map_layer_1, "clusters_on_ROCs_layer_1", "Clusters on ROCs layer 1");
	this -> check_set_name_title(hit_efficiency_on_ROCs_map_layer_2, "hit_efficiency_on_ROCs_layer_2", "Hit efficiency on ROCs layer 2");
	this -> check_set_name_title(total_hits_on_ROCs_map_layer_2, "total_hits_on_ROCs_layer_2", "Total hits on ROCs layer 2");
	this -> check_set_name_title(clusters_on_ROCs_map_layer_2, "clusters_on_ROCs_layer_2", "Clusters on ROCs layer 2");
	this -> check_set_name_title(hit_efficiency_on_ROCs_map_layer_3, "hit_efficiency_on_ROCs_layer_3", "Hit efficiency on ROCs layer 3");
	this -> check_set_name_title(total_hits_on_ROCs_map_layer_3, "total_hits_on_ROCs_layer_3", "Total hits on ROCs layer 3");
	this -> check_set_name_title(clusters_on_ROCs_map_layer_3, "clusters_on_ROCs_layer_3", "Clusters on ROCs layer 3");
	this -> hit_efficiency_on_ROCs_map_layer_1             -> GetYaxis() -> SetRangeUser(-10.5, 10.5);
	this -> total_hits_on_ROCs_map_layer_1                 -> GetYaxis() -> SetRangeUser(-10.5, 10.5);
	this -> clusters_on_ROCs_map_layer_1                   -> GetYaxis() -> SetRangeUser(-10.5, 10.5);
	this -> hit_efficiency_on_ROCs_map_layer_2             -> GetYaxis() -> SetRangeUser(-16.5, 16.5);
	this -> total_hits_on_ROCs_map_layer_2                 -> GetYaxis() -> SetRangeUser(-16.5, 16.5);
	this -> clusters_on_ROCs_map_layer_2                   -> GetYaxis() -> SetRangeUser(-16.5, 16.5);


	////////////////////////////////////////////
	// Hit Efficiency instlumi correspondence //
	////////////////////////////////////////////

	if(debug_prompt_histogram_saving)
	{
		std::cerr << debug_prompt << "Saving: step 2." << std::endl;
	}

	this -> check_set_name_title(hit_efficiency_instlumi_correspondence_layer_1, "hit_efficiency_instlumi_correspondence_layer_1",      "Hit efficiency vs Instlumi on layer 1;Instlumi;Eff");
	this -> check_set_name_title(hit_efficiency_instlumi_correspondence_layer_2, "hit_efficiency_instlumi_correspondence_layer_2",      "Hit efficiency vs Instlumi on layer 2;Instlumi;Eff");
	this -> check_set_name_title(hit_efficiency_instlumi_correspondence_layer_3, "hit_efficiency_instlumi_correspondence_layer_3",      "Hit efficiency vs Instlumi on layer 3;Instlumi;Eff");

	//////////////////////////
	// Impact angle analyis //
	//////////////////////////

	if(debug_prompt_histogram_saving)
	{
		std::cerr << debug_prompt << "Saving: step 3. 1/2" << std::endl;
	}

	this -> check_set_name_title(hit_efficiency_impact_alpha_correspondence_layer_1, "hit_efficiency_impact_alpha_correspondence_layer_1", "Hit efficiency vs alpha on layer 1;alpha;Eff");
	this -> check_set_name_title(hit_efficiency_impact_alpha_correspondence_layer_2, "hit_efficiency_impact_alpha_correspondence_layer_2", "Hit efficiency vs alpha on layer 2;alpha;Eff");
	this -> check_set_name_title(hit_efficiency_impact_alpha_correspondence_layer_3, "hit_efficiency_impact_alpha_correspondence_layer_3", "Hit efficiency vs alpha on layer 3;alpha;Eff");
	// this -> check_set_name_title(hit_efficiency_impact_gamma_correspondence_layer_1, "hit_efficiency_impact_gamma_correspondence_layer_1", "Hit efficiency vs gamma on layer 1;gamma;Eff");
	// this -> check_set_name_title(hit_efficiency_impact_gamma_correspondence_layer_2, "hit_efficiency_impact_gamma_correspondence_layer_2", "Hit efficiency vs gamma on layer 2;gamma;Eff");
	// this -> check_set_name_title(hit_efficiency_impact_gamma_correspondence_layer_3, "hit_efficiency_impact_gamma_correspondence_layer_3", "Hit efficiency vs gamma on layer 3;gamma;Eff");
	this -> check_set_name_title(hit_efficiency_impact_beta_correspondence_layer_1, "hit_efficiency_impact_beta_correspondence_layer_1", "Hit efficiency vs beta on layer 1;beta;Eff");
	this -> check_set_name_title(hit_efficiency_impact_beta_correspondence_layer_2, "hit_efficiency_impact_beta_correspondence_layer_2", "Hit efficiency vs beta on layer 2;beta;Eff");
	this -> check_set_name_title(hit_efficiency_impact_beta_correspondence_layer_3, "hit_efficiency_impact_beta_correspondence_layer_3", "Hit efficiency vs beta on layer 3;beta;Eff");

	hit_efficiency_impact_alpha_correspondence_layer_1 -> GetXaxis() -> SetRangeUser(70.0, 110.0);
	hit_efficiency_impact_alpha_correspondence_layer_1 -> GetYaxis() -> SetRangeUser(0.93, 1.00);
	hit_efficiency_impact_alpha_correspondence_layer_2 -> GetXaxis() -> SetRangeUser(70.0, 110.0);
	hit_efficiency_impact_alpha_correspondence_layer_2 -> GetYaxis() -> SetRangeUser(0.93, 1.00);
	hit_efficiency_impact_alpha_correspondence_layer_3 -> GetXaxis() -> SetRangeUser(70.0, 110.0);
	hit_efficiency_impact_alpha_correspondence_layer_3 -> GetYaxis() -> SetRangeUser(0.93, 1.00);
	hit_efficiency_impact_beta_correspondence_layer_1  -> GetXaxis() -> SetRangeUser(5.0,  175.0);
	hit_efficiency_impact_beta_correspondence_layer_1  -> GetYaxis() -> SetRangeUser(0.93, 1.00);
	hit_efficiency_impact_beta_correspondence_layer_2  -> GetXaxis() -> SetRangeUser(5.0,  175.0);
	hit_efficiency_impact_beta_correspondence_layer_2  -> GetYaxis() -> SetRangeUser(0.93, 1.00);
	hit_efficiency_impact_beta_correspondence_layer_3  -> GetXaxis() -> SetRangeUser(5.0,  175.0);
	hit_efficiency_impact_beta_correspondence_layer_3  -> GetYaxis() -> SetRangeUser(0.93, 1.00);

	this -> check_set_name_title(beta_vs_alpha_correlation_layer_1, "beta_vs_alpha_correspondence_layer_1", "Beta vs alpha on layer 1;alpha;beta");
	this -> check_set_name_title(beta_vs_alpha_correlation_layer_2, "beta_vs_alpha_correspondence_layer_2", "Beta vs alpha on layer 2;alpha;beta");
	this -> check_set_name_title(beta_vs_alpha_correlation_layer_3, "beta_vs_alpha_correspondence_layer_3", "Beta vs alpha on layer 3;alpha;beta");
	// this -> check_set_name_title(beta_vs_gamma_correlation_layer_1, "beta_vs_gamma_correspondence_layer_1", "Beta vs gamma on layer 1;gamma;beta");
	// this -> check_set_name_title(beta_vs_gamma_correlation_layer_2, "beta_vs_gamma_correspondence_layer_2", "Beta vs gamma on layer 2;gamma;beta");
	// this -> check_set_name_title(beta_vs_gamma_correlation_layer_3, "beta_vs_gamma_correspondence_layer_3", "Beta vs gamma on layer 3;gamma;beta");

	this -> check_set_name_title(hit_efficiency_vs_beta_vs_alpha_layer_1, "hit_efficiency_vs_beta_vs_alpha_layer_1", "Hit efficiency vs beta vs alpha on layer 1;alpha;beta");
	this -> check_set_name_title(hit_efficiency_vs_beta_vs_alpha_layer_2, "hit_efficiency_vs_beta_vs_alpha_layer_2", "Hit efficiency vs beta vs alpha on layer 2;alpha;beta");
	this -> check_set_name_title(hit_efficiency_vs_beta_vs_alpha_layer_3, "hit_efficiency_vs_beta_vs_alpha_layer_3", "Hit efficiency vs beta vs alpha on layer 3;alpha;beta");
	// this -> check_set_name_title(hit_efficiency_vs_beta_vs_gamma_layer_1, "hit_efficiency_vs_beta_vs_gamma_layer_1", "Hit efficiency vs beta vs gamma on layer 1;gamma;beta");
	// this -> check_set_name_title(hit_efficiency_vs_beta_vs_gamma_layer_2, "hit_efficiency_vs_beta_vs_gamma_layer_2", "Hit efficiency vs beta vs gamma on layer 2;gamma;beta");
	// this -> check_set_name_title(hit_efficiency_vs_beta_vs_gamma_layer_3, "hit_efficiency_vs_beta_vs_gamma_layer_3", "Hit efficiency vs beta vs gamma on layer 3;gamma;beta");

	this -> beta_vs_alpha_correlation_layer_1_rebinned       = new TH2D(*(this -> beta_vs_alpha_correlation_layer_1));
	this -> beta_vs_alpha_correlation_layer_2_rebinned       = new TH2D(*(this -> beta_vs_alpha_correlation_layer_2));
	this -> beta_vs_alpha_correlation_layer_3_rebinned       = new TH2D(*(this -> beta_vs_alpha_correlation_layer_3));
	this -> hit_efficiency_vs_beta_vs_alpha_layer_1_rebinned = new TH2D(*(this -> hit_efficiency_vs_beta_vs_alpha_layer_1));
	this -> hit_efficiency_vs_beta_vs_alpha_layer_2_rebinned = new TH2D(*(this -> hit_efficiency_vs_beta_vs_alpha_layer_2));
	this -> hit_efficiency_vs_beta_vs_alpha_layer_3_rebinned = new TH2D(*(this -> hit_efficiency_vs_beta_vs_alpha_layer_3));
	// this -> beta_vs_gamma_correlation_layer_1_rebinned       = new TH2D(*(this -> beta_vs_gamma_correlation_layer_1));
	// this -> beta_vs_gamma_correlation_layer_2_rebinned       = new TH2D(*(this -> beta_vs_gamma_correlation_layer_2));
	// this -> beta_vs_gamma_correlation_layer_3_rebinned       = new TH2D(*(this -> beta_vs_gamma_correlation_layer_3));
	this -> hit_efficiency_vs_beta_vs_gamma_layer_1_rebinned = new TH2D(*(this -> hit_efficiency_vs_beta_vs_gamma_layer_1));
	this -> hit_efficiency_vs_beta_vs_gamma_layer_2_rebinned = new TH2D(*(this -> hit_efficiency_vs_beta_vs_gamma_layer_2));
	this -> hit_efficiency_vs_beta_vs_gamma_layer_3_rebinned = new TH2D(*(this -> hit_efficiency_vs_beta_vs_gamma_layer_3));

	if(debug_prompt_histogram_saving)
	{
		std::cerr << debug_prompt << "Saving: step 3. 2/2" << std::endl;
	}

	this -> check_set_name_title(beta_vs_alpha_correlation_layer_1_rebinned, "beta_vs_alpha_correspondence_layer_1_rebinned", "Beta vs alpha on layer 1 rebinned;alpha;beta");
	this -> check_set_name_title(beta_vs_alpha_correlation_layer_2_rebinned, "beta_vs_alpha_correspondence_layer_2_rebinned", "Beta vs alpha on layer 2 rebinned;alpha;beta");
	this -> check_set_name_title(beta_vs_alpha_correlation_layer_3_rebinned, "beta_vs_alpha_correspondence_layer_3_rebinned", "Beta vs alpha on layer 3 rebinned;alpha;beta");
	this -> beta_vs_alpha_correlation_layer_1_rebinned       -> Rebin2D(5, 5);
	this -> beta_vs_alpha_correlation_layer_2_rebinned       -> Rebin2D(5, 5);
	this -> beta_vs_alpha_correlation_layer_3_rebinned       -> Rebin2D(5, 5);
	// this -> check_set_name_title(beta_vs_gamma_correlation_layer_1_rebinned, "beta_vs_gamma_correspondence_layer_1_rebinned", "Beta vs gamma on layer 1 rebinned;gamma;beta");
	// this -> check_set_name_title(beta_vs_gamma_correlation_layer_2_rebinned, "beta_vs_gamma_correspondence_layer_2_rebinned", "Beta vs gamma on layer 2 rebinned;gamma;beta");
	// this -> check_set_name_title(beta_vs_gamma_correlation_layer_3_rebinned, "beta_vs_gamma_correspondence_layer_3_rebinned", "Beta vs gamma on layer 3 rebinned;gamma;beta");
	// this -> beta_vs_gamma_correlation_layer_1_rebinned       -> Rebin2D(5, 5);
	// this -> beta_vs_gamma_correlation_layer_2_rebinned       -> Rebin2D(5, 5);
	// this -> beta_vs_gamma_correlation_layer_3_rebinned       -> Rebin2D(5, 5);

	this -> check_set_name_title(hit_efficiency_vs_beta_vs_alpha_layer_1_rebinned, "hit_efficiency_vs_beta_vs_alpha_layer_1_rebinned", "Beta vs alpha on layer 1 rebinned;alpha;beta");
	this -> check_set_name_title(hit_efficiency_vs_beta_vs_alpha_layer_2_rebinned, "hit_efficiency_vs_beta_vs_alpha_layer_2_rebinned", "Beta vs alpha on layer 2 rebinned;alpha;beta");
	this -> check_set_name_title(hit_efficiency_vs_beta_vs_alpha_layer_3_rebinned, "hit_efficiency_vs_beta_vs_alpha_layer_3_rebinned", "Beta vs alpha on layer 3 rebinned;alpha;beta");
	this -> hit_efficiency_vs_beta_vs_alpha_layer_1_rebinned -> Rebin2D(5, 5);
	this -> hit_efficiency_vs_beta_vs_alpha_layer_2_rebinned -> Rebin2D(5, 5);
	this -> hit_efficiency_vs_beta_vs_alpha_layer_3_rebinned -> Rebin2D(5, 5);
	// this -> check_set_name_title(hit_efficiency_vs_beta_vs_gamma_layer_1_rebinned, "hit_efficiency_vs_beta_vs_gamma_layer_1_rebinned", "Beta vs gamma on layer 1 rebinned;gamma;beta");
	// this -> check_set_name_title(hit_efficiency_vs_beta_vs_gamma_layer_2_rebinned, "hit_efficiency_vs_beta_vs_gamma_layer_2_rebinned", "Beta vs gamma on layer 2 rebinned;gamma;beta");
	// this -> check_set_name_title(hit_efficiency_vs_beta_vs_gamma_layer_3_rebinned, "hit_efficiency_vs_beta_vs_gamma_layer_3_rebinned", "Beta vs gamma on layer 3 rebinned;gamma;beta");
	// this -> hit_efficiency_vs_beta_vs_gamma_layer_1_rebinned -> Rebin2D(5, 5);
	// this -> hit_efficiency_vs_beta_vs_gamma_layer_2_rebinned -> Rebin2D(5, 5);
	// this -> hit_efficiency_vs_beta_vs_gamma_layer_3_rebinned -> Rebin2D(5, 5);

	this -> beta_vs_alpha_correlation_layer_1_rebinned        -> GetXaxis() -> SetRangeUser(70.0, 110.0);
	this -> beta_vs_alpha_correlation_layer_1_rebinned        -> GetYaxis() -> SetRangeUser(5.0,  175.0);
	this -> beta_vs_alpha_correlation_layer_2_rebinned        -> GetXaxis() -> SetRangeUser(70.0, 110.0);
	this -> beta_vs_alpha_correlation_layer_2_rebinned        -> GetYaxis() -> SetRangeUser(5.0,  175.0);
	this -> beta_vs_alpha_correlation_layer_3_rebinned        -> GetXaxis() -> SetRangeUser(70.0, 110.0);
	this -> beta_vs_alpha_correlation_layer_3_rebinned        -> GetYaxis() -> SetRangeUser(5.0,  175.0);
	this -> hit_efficiency_vs_beta_vs_alpha_layer_1_rebinned  -> GetXaxis() -> SetRangeUser(70.0, 110.0);
	this -> hit_efficiency_vs_beta_vs_alpha_layer_1_rebinned  -> GetYaxis() -> SetRangeUser(5.0,  175.0);
	this -> hit_efficiency_vs_beta_vs_alpha_layer_2_rebinned  -> GetXaxis() -> SetRangeUser(70.0, 110.0);
	this -> hit_efficiency_vs_beta_vs_alpha_layer_2_rebinned  -> GetYaxis() -> SetRangeUser(5.0,  175.0);
	this -> hit_efficiency_vs_beta_vs_alpha_layer_3_rebinned  -> GetXaxis() -> SetRangeUser(70.0, 110.0);
	this -> hit_efficiency_vs_beta_vs_alpha_layer_3_rebinned  -> GetYaxis() -> SetRangeUser(5.0,  175.0);
	// this -> beta_vs_gamma_correlation_layer_1_rebinned        -> GetXaxis() -> SetRangeUser(10.0,  170.0);
	// this -> beta_vs_gamma_correlation_layer_1_rebinned        -> GetYaxis() -> SetRangeUser(5.0,  175.0);
	// this -> beta_vs_gamma_correlation_layer_2_rebinned        -> GetXaxis() -> SetRangeUser(10.0,  170.0);
	// this -> beta_vs_gamma_correlation_layer_2_rebinned        -> GetYaxis() -> SetRangeUser(5.0,  175.0);
	// this -> beta_vs_gamma_correlation_layer_3_rebinned        -> GetXaxis() -> SetRangeUser(10.0,  170.0);
	// this -> beta_vs_gamma_correlation_layer_3_rebinned        -> GetYaxis() -> SetRangeUser(5.0,  175.0);
	this -> hit_efficiency_vs_beta_vs_gamma_layer_1_rebinned  -> GetXaxis() -> SetRangeUser(10.0,  170.0);
	this -> hit_efficiency_vs_beta_vs_gamma_layer_1_rebinned  -> GetYaxis() -> SetRangeUser(5.0,  175.0);
	this -> hit_efficiency_vs_beta_vs_gamma_layer_2_rebinned  -> GetXaxis() -> SetRangeUser(10.0,  170.0);
	this -> hit_efficiency_vs_beta_vs_gamma_layer_2_rebinned  -> GetYaxis() -> SetRangeUser(5.0,  175.0);
	this -> hit_efficiency_vs_beta_vs_gamma_layer_3_rebinned  -> GetXaxis() -> SetRangeUser(10.0,  170.0);
	this -> hit_efficiency_vs_beta_vs_gamma_layer_3_rebinned  -> GetYaxis() -> SetRangeUser(5.0,  175.0);

	/////////////////////////////////////////
	// Hit efficiency layer correspondence //
	/////////////////////////////////////////
	
	if(debug_prompt_histogram_saving)
	{
		std::cerr << debug_prompt << "Saving: step 4." << std::endl;
	}

	this -> hit_efficiency_layer_correspondence                 -> SetNameTitle("hit_efficiency_layer_correspondence", "Hit efficiency on layers;layer;efficiency");

	///////////////////////////////////////
	// Module and ladder correspondences //
	///////////////////////////////////////

	if(debug_prompt_histogram_saving)
	{
		std::cerr << debug_prompt << "Saving: step 5." << std::endl;
	}

	this -> hit_efficiency_ladder_number_correspondence_layer_1 -> SetNameTitle("hit_efficiency_ladder_number_correspondence_layer_1", "Hit efficiency vs Ladder-number on layer 1;Ladder;Eff");
	this -> hit_efficiency_ladder_number_correspondence_layer_2 -> SetNameTitle("hit_efficiency_ladder_number_correspondence_layer_2", "Hit efficiency vs Ladder-number on layer 2;Ladder;Eff");
	this -> hit_efficiency_ladder_number_correspondence_layer_3 -> SetNameTitle("hit_efficiency_ladder_number_correspondence_layer_3", "Hit efficiency vs Ladder-number on layer 3;Ladder;Eff");

	this -> hit_efficiency_module_number_correspondence_layer_1 -> SetNameTitle("hit_efficiency_module_number_correspondence_layer_1", "Hit efficiency vs Module-number on layer 1;Module;Eff");
	this -> hit_efficiency_module_number_correspondence_layer_2 -> SetNameTitle("hit_efficiency_module_number_correspondence_layer_2", "Hit efficiency vs Module-number on layer 2;Module;Eff");
	this -> hit_efficiency_module_number_correspondence_layer_3 -> SetNameTitle("hit_efficiency_module_number_correspondence_layer_3", "Hit efficiency vs Module-number on layer 3;Module;Eff");

	this -> hit_efficiency_ladder_number_correspondence_layer_1 -> GetYaxis() -> SetRangeUser(.97, 1.00);
	this -> hit_efficiency_module_number_correspondence_layer_1 -> GetYaxis() -> SetRangeUser(.93, 1.00);
	this -> hit_efficiency_instlumi_correspondence_layer_1      -> GetYaxis() -> SetRangeUser(.97, 1.00);
	this -> hit_efficiency_ladder_number_correspondence_layer_2 -> GetYaxis() -> SetRangeUser(.97, 1.00);
	this -> hit_efficiency_module_number_correspondence_layer_2 -> GetYaxis() -> SetRangeUser(.97, 1.00);
	this -> hit_efficiency_instlumi_correspondence_layer_2      -> GetYaxis() -> SetRangeUser(.97, 1.00);
	this -> hit_efficiency_ladder_number_correspondence_layer_3 -> GetYaxis() -> SetRangeUser(.97, 1.00);
	this -> hit_efficiency_module_number_correspondence_layer_3 -> GetYaxis() -> SetRangeUser(.97, 1.00);
	this -> hit_efficiency_instlumi_correspondence_layer_3      -> GetYaxis() -> SetRangeUser(.97, 1.00);
	this -> hit_efficiency_layer_correspondence                 -> GetYaxis() -> SetRangeUser(.97, 1.00);
	
	/////////////////////////
	// Analysis tier plots //
	/////////////////////////

	if(tier_1_analysis_done)
	{
		if(debug_prompt_histogram_saving)
		{
			std::cerr << debug_prompt << "Saving: step 6." << std::endl;
		}
		this -> relative_cluster_occupancy_on_modules_map_layer_1 -> SetNameTitle("relative_cluster_occupancy_on_modules_layer_1", "Rel. clust. occupancy on modules layer 1;Modules;Ladders");
		this -> relative_cluster_occupancy_on_modules_map_layer_2 -> SetNameTitle("relative_cluster_occupancy_on_modules_layer_2", "Rel. clust. occupancy on modules layer 2;Modules;Ladders");
		this -> relative_cluster_occupancy_on_modules_map_layer_3 -> SetNameTitle("relative_cluster_occupancy_on_modules_layer_3", "Rel. clust. occupancy on modules layer 3;Modules;Ladders");
		this -> relative_cluster_occupancy_on_ROCs_map_layer_1 -> SetNameTitle("relative_cluster_occupancy_on_ROCs_layer_1", "Rel. clust. occupancy on ROCs layer 1;Modules;Ladders");
		this -> relative_cluster_occupancy_on_ROCs_map_layer_2 -> SetNameTitle("relative_cluster_occupancy_on_ROCs_layer_2", "Rel. clust. occupancy on ROCs layer 2;Modules;Ladders");
		this -> relative_cluster_occupancy_on_ROCs_map_layer_3 -> SetNameTitle("relative_cluster_occupancy_on_ROCs_layer_3", "Rel. clust. occupancy on ROCs layer 3;Modules;Ladders");
		this -> autoset_maximum(this -> relative_cluster_occupancy_on_modules_map_layer_1);
		this -> autoset_maximum(this -> relative_cluster_occupancy_on_modules_map_layer_2);
		this -> autoset_maximum(this -> relative_cluster_occupancy_on_modules_map_layer_3);
		this -> relative_cluster_occupancy_on_modules_map_layer_1 -> GetYaxis() -> SetRangeUser(-10.5, 10.5);
		this -> relative_cluster_occupancy_on_modules_map_layer_2 -> GetYaxis() -> SetRangeUser(-16.5, 16.5);
		this -> autoset_maximum(this -> relative_cluster_occupancy_on_ROCs_map_layer_1);
		this -> autoset_maximum(this -> relative_cluster_occupancy_on_ROCs_map_layer_2);
		this -> autoset_maximum(this -> relative_cluster_occupancy_on_ROCs_map_layer_3);
		this -> relative_cluster_occupancy_on_ROCs_map_layer_1 -> GetYaxis() -> SetRangeUser(-10.5, 10.5);
		this -> relative_cluster_occupancy_on_ROCs_map_layer_2 -> GetYaxis() -> SetRangeUser(-16.5, 16.5);
	}

	if(tier_2_analysis_done)
	{
		if(debug_prompt_histogram_saving)
		{
			std::cerr << debug_prompt << "Saving: step 7." << std::endl;
		}
		this -> bad_ROCs_simple_efficiency_base_layer_1   -> GetYaxis() -> SetRangeUser(-10.5, 10.5);
		this -> bad_ROCs_weighted_efficiency_base_layer_1 -> GetYaxis() -> SetRangeUser(-10.5, 10.5);
		this -> bad_ROCs_cluster_occupancy_base_layer_1   -> GetYaxis() -> SetRangeUser(-10.5, 10.5);
		this -> bad_ROCs_relative_occupancy_base_layer_1  -> GetYaxis() -> SetRangeUser(-10.5, 10.5);
		this -> bad_ROCs_all_layer_1                      -> GetYaxis() -> SetRangeUser(-10.5, 10.5);
		this -> bad_ROCs_neighbour_hits_base_layer_1      -> GetYaxis() -> SetRangeUser(-10.5, 10.5);
		this -> bad_ROCs_simple_efficiency_base_layer_2   -> GetYaxis() -> SetRangeUser(-16.5, 16.5);
		this -> bad_ROCs_weighted_efficiency_base_layer_2 -> GetYaxis() -> SetRangeUser(-16.5, 16.5);
		this -> bad_ROCs_cluster_occupancy_base_layer_2   -> GetYaxis() -> SetRangeUser(-16.5, 16.5);
		this -> bad_ROCs_relative_occupancy_base_layer_2  -> GetYaxis() -> SetRangeUser(-16.5, 16.5);
		this -> bad_ROCs_all_layer_2                      -> GetYaxis() -> SetRangeUser(-16.5, 16.5);
		this -> bad_ROCs_neighbour_hits_base_layer_2      -> GetYaxis() -> SetRangeUser(-16.5, 16.5);
	}


	// this -> bad_ROCs_relative_occupancy_base_layer_1  -> GetZaxis() -> SetRangeUser(0.0, 1.0);
	// this -> bad_ROCs_relative_occupancy_base_layer_2  -> GetZaxis() -> SetRangeUser(0.0, 1.0);
	// this -> bad_ROCs_relative_occupancy_base_layer_3  -> GetZaxis() -> SetRangeUser(0.0, 1.0);
	// this -> bad_ROCs_all_layer_1                      -> GetZaxis() -> SetRangeUser(0.0, 1.0);
	// this -> bad_ROCs_all_layer_2                      -> GetZaxis() -> SetRangeUser(0.0, 1.0);
	// this -> bad_ROCs_all_layer_3                      -> GetZaxis() -> SetRangeUser(0.0, 1.0);

}

void Post_loop_analyzer_histogram_saver::prepare_to_save_canvases()
{
	/////////////////////////////////////////////////////////////////////
	// Changes made on non-on-the-fly-generated canvases before saving //
	/////////////////////////////////////////////////////////////////////
}

void Post_loop_analyzer_histogram_saver::prepare_to_saving()
{
	this -> prepare_to_save_histograms();
	this -> prepare_to_save_canvases();
}

/******************************************************************************************************/
/******************************************************************************************************/
/*                                                                                                    */
/*                                           Public Methods                                           */
/*                                                                                                    */
/******************************************************************************************************/
/******************************************************************************************************/


void Post_loop_analyzer_histogram_saver:: save_histograms(const std::string& filename_p, const std::string& options_p)
{
	this -> prepare_to_saving();

	TFile* output_file(new TFile(filename_p.c_str(), "RECREATE"));
	if(!(output_file -> IsOpen()))
	{
		std::cerr << error_prompt << "Error opening the output file: " << filename_p << "." << std::endl;
	}

	// Canvas: 3*2 név: Layer_3_ROC_Hit_efficiency_statistics
	// Plotok: - HitEfficiency vs Ladder, HitEfficiency vs Module, HitEfficiency vs Layer, HitEfficiency vs InstLumi, HitEfficiency vs Alpha, HitEfficiency vs Beta
	this -> save_as_two_by_three_canvas(this -> hit_efficiency_ladder_number_correspondence_layer_1, this -> hit_efficiency_module_number_correspondence_layer_1,
		                                this -> hit_efficiency_instlumi_correspondence_layer_1,      this -> hit_efficiency_layer_correspondence, 
		                                this -> hit_efficiency_impact_alpha_correspondence_layer_1, this -> hit_efficiency_impact_beta_correspondence_layer_1, 
		                                output_file, "Hit_efficiency_parametrization", "Layer_1_Hit_efficiency_parameter_correspondence", "Layer_1_ROC_Hit_efficiency_statistics");
	this -> save_as_two_by_three_canvas(this -> hit_efficiency_ladder_number_correspondence_layer_2, this -> hit_efficiency_module_number_correspondence_layer_2, 
		                                this -> hit_efficiency_instlumi_correspondence_layer_2,      this -> hit_efficiency_layer_correspondence, 
		                                this -> hit_efficiency_impact_alpha_correspondence_layer_2, this -> hit_efficiency_impact_beta_correspondence_layer_2, 
		                                output_file, "Hit_efficiency_parametrization", "Layer_2_Hit_efficiency_parameter_correspondence", "Layer_2_ROC_Hit_efficiency_statistics");
	this -> save_as_two_by_three_canvas(this -> hit_efficiency_ladder_number_correspondence_layer_3, this -> hit_efficiency_module_number_correspondence_layer_3,
		                                this -> hit_efficiency_instlumi_correspondence_layer_3,      this -> hit_efficiency_layer_correspondence, 
		                                this -> hit_efficiency_impact_alpha_correspondence_layer_3, this -> hit_efficiency_impact_beta_correspondence_layer_3, 
		                                output_file, "Hit_efficiency_parametrization", "Layer_3_Hit_efficiency_parameter_correspondence", "Layer_3_ROC_Hit_efficiency_statistics");

	//////////////////////////
	// Impact angle analyis //
	//////////////////////////

	output_file -> mkdir("Impact_angle_analysis");

	this -> save_as_two_by_one_canvas(beta_vs_alpha_correlation_layer_1_rebinned, hit_efficiency_vs_beta_vs_alpha_layer_1_rebinned,
		                              output_file, "Impact_angle_analysis", "Layer_1_impact_angle_statistics", "Layer_1_impact_angle_statistics");
	this -> save_as_two_by_one_canvas(beta_vs_alpha_correlation_layer_2_rebinned, hit_efficiency_vs_beta_vs_alpha_layer_2_rebinned,
		                              output_file, "Impact_angle_analysis", "Layer_2_impact_angle_statistics", "Layer_2_impact_angle_statistics");
	this -> save_as_two_by_one_canvas(beta_vs_alpha_correlation_layer_3_rebinned, hit_efficiency_vs_beta_vs_alpha_layer_3_rebinned,
		                              output_file, "Impact_angle_analysis", "Layer_3_impact_angle_statistics", "Layer_3_impact_angle_statistics");

	std::cerr << process_prompt << "Correlation of alpha and beta parameters on layer 1: " << beta_vs_alpha_correlation_layer_1 -> GetCorrelationFactor() << std::endl;
	std::cerr << process_prompt << "Correlation of alpha and beta parameters on layer 2: " << beta_vs_alpha_correlation_layer_2 -> GetCorrelationFactor() << std::endl;
	std::cerr << process_prompt << "Correlation of alpha and beta parameters on layer 3: " << beta_vs_alpha_correlation_layer_3 -> GetCorrelationFactor() << std::endl;

	std::cerr << process_prompt << "Covariance of alpha and beta parameters on layer 1: " << beta_vs_alpha_correlation_layer_1 -> GetCovariance() << std::endl;
	std::cerr << process_prompt << "Covariance of alpha and beta parameters on layer 2: " << beta_vs_alpha_correlation_layer_2 -> GetCovariance() << std::endl;
	std::cerr << process_prompt << "Covariance of alpha and beta parameters on layer 3: " << beta_vs_alpha_correlation_layer_3 -> GetCovariance() << std::endl;


	if(tier_1_analysis_done)
	{
		//////////////////////////////////////////////
		// Layer_1_Module_Hit_efficiency_statistics //
		//////////////////////////////////////////////
		
		// Plotok: Hit Efficiency, Hit Efficiency nevező, Klaszter Okkupancia, Relativ klaszter Okkupancia.
		output_file -> mkdir("Hit_efficiency_statistics");
		this -> save_as_two_by_two_canvas(this -> hit_efficiency_on_modules_map_layer_1, this -> total_hits_on_modules_map_layer_1,
			                              this -> clusters_on_modules_map_layer_1,       this -> relative_cluster_occupancy_on_modules_map_layer_1, 
			                              output_file, "Hit_efficiency_statistics/Module_bins", "Layer_1_module_Hit_efficiency_statistics", "Layer_1_module_Hit_efficiency_statistics");
		this -> save_as_two_by_two_canvas(this -> hit_efficiency_on_modules_map_layer_2, this -> total_hits_on_modules_map_layer_2,
			                              this -> clusters_on_modules_map_layer_2,       this -> relative_cluster_occupancy_on_modules_map_layer_2, 
			                              output_file, "Hit_efficiency_statistics/Module_bins", "Layer_2_module_Hit_efficiency_statistics", "Layer_2_module_Hit_efficiency_statistics");
		this -> save_as_two_by_two_canvas(this -> hit_efficiency_on_modules_map_layer_3, this -> total_hits_on_modules_map_layer_3,
			                              this -> clusters_on_modules_map_layer_3,       this -> relative_cluster_occupancy_on_modules_map_layer_3, 
			                              output_file, "Hit_efficiency_statistics/Module_bins", "Layer_3_module_Hit_efficiency_statistics", "Layer_3_module_Hit_efficiency_statistics");

		// Plotok: Hit Efficiency, Hit Efficiency nevező, Klaszter Okkupancia, Relativ klaszter Okkupancia.
		this -> save_as_two_by_two_canvas(this -> hit_efficiency_on_ROCs_map_layer_1, this -> total_hits_on_ROCs_map_layer_1,
			                              this -> clusters_on_ROCs_map_layer_1,       this -> relative_cluster_occupancy_on_ROCs_map_layer_1, 
			                              output_file, "Hit_efficiency_statistics/ROC_bins", "Layer_1_ROC_Hit_efficiency_statistics", "Layer_1_ROC_Hit_efficiency_statistics");
		this -> save_as_two_by_two_canvas(this -> hit_efficiency_on_ROCs_map_layer_2, this -> total_hits_on_ROCs_map_layer_2,
			                              this -> clusters_on_ROCs_map_layer_2,       this -> relative_cluster_occupancy_on_ROCs_map_layer_2, 
			                              output_file, "Hit_efficiency_statistics/ROC_bins", "Layer_2_ROC_Hit_efficiency_statistics", "Layer_2_ROC_Hit_efficiency_statistics");
		this -> save_as_two_by_two_canvas(this -> hit_efficiency_on_ROCs_map_layer_3, this -> total_hits_on_ROCs_map_layer_3,
			                              this -> clusters_on_ROCs_map_layer_3,       this -> relative_cluster_occupancy_on_ROCs_map_layer_3, 
			                              output_file, "Hit_efficiency_statistics/ROC_bins", "Layer_3_ROC_Hit_efficiency_statistics", "Layer_3_ROC_Hit_efficiency_statistics");
	}

	/////////////////////////
	// ROC filtering plots //
	/////////////////////////

	if(tier_2_analysis_done)
	{
		this -> save_as_two_by_one_canvas(this -> hit_efficiency_on_ROCs_map_layer_1, this -> bad_ROCs_simple_efficiency_base_layer_1,
			                              output_file, "ROC_exclusion", "Layer_1_simple_efficiency_filtering", "Layer_1_simple_efficiency_filtering");
		this -> save_as_two_by_one_canvas(this -> hit_efficiency_on_ROCs_map_layer_2, this -> bad_ROCs_simple_efficiency_base_layer_2,
			                              output_file, "ROC_exclusion", "Layer_2_simple_efficiency_filtering", "Layer_2_simple_efficiency_filtering");
		this -> save_as_two_by_one_canvas(this -> hit_efficiency_on_ROCs_map_layer_3, this -> bad_ROCs_simple_efficiency_base_layer_3,
			                              output_file, "ROC_exclusion", "Layer_3_simple_efficiency_filtering", "Layer_3_simple_efficiency_filtering");

		this -> save_as_two_by_one_canvas(this -> hit_efficiency_on_ROCs_map_layer_1, this -> bad_ROCs_weighted_efficiency_base_layer_1,
			                              output_file, "ROC_exclusion", "Layer_1_weighted_efficiency_filtering", "Layer_1_weighted_efficiency_filtering");
		this -> save_as_two_by_one_canvas(this -> hit_efficiency_on_ROCs_map_layer_2, this -> bad_ROCs_weighted_efficiency_base_layer_2,
			                              output_file, "ROC_exclusion", "Layer_2_weighted_efficiency_filtering", "Layer_2_weighted_efficiency_filtering");
		this -> save_as_two_by_one_canvas(this -> hit_efficiency_on_ROCs_map_layer_3, this -> bad_ROCs_weighted_efficiency_base_layer_3,
			                              output_file, "ROC_exclusion", "Layer_3_weighted_efficiency_filtering", "Layer_3_weighted_efficiency_filtering");

		this -> save_as_two_by_one_canvas(this -> relative_cluster_occupancy_on_ROCs_map_layer_1, this -> bad_ROCs_relative_occupancy_base_layer_1,
			                              output_file, "ROC_exclusion", "Layer_1_rel._occ._filtering", "Layer_1_rel._occ._filtering");
		this -> save_as_two_by_one_canvas(this -> relative_cluster_occupancy_on_ROCs_map_layer_2, this -> bad_ROCs_relative_occupancy_base_layer_2,
			                              output_file, "ROC_exclusion", "Layer_2_rel._occ._filtering", "Layer_2_rel._occ._filtering");
		this -> save_as_two_by_one_canvas(this -> relative_cluster_occupancy_on_ROCs_map_layer_3, this -> bad_ROCs_relative_occupancy_base_layer_3,
			                              output_file, "ROC_exclusion", "Layer_3_rel._occ._filtering", "Layer_3_rel._occ._filtering");

		this -> save_as_two_by_one_canvas(this -> total_hits_on_ROCs_map_layer_1, this -> bad_ROCs_neighbour_hits_base_layer_1,
			                              output_file, "ROC_exclusion", "Layer_1_hit_comparison_filtering", "Layer 1 hit comparison filtering");
		this -> save_as_two_by_one_canvas(this -> total_hits_on_ROCs_map_layer_2, this -> bad_ROCs_neighbour_hits_base_layer_2,
			                              output_file, "ROC_exclusion", "Layer_2_hit_comparison_filtering", "Layer 2 hit comparison filtering");
		this -> save_as_two_by_one_canvas(this -> total_hits_on_ROCs_map_layer_3, this -> bad_ROCs_neighbour_hits_base_layer_3,
			                              output_file, "ROC_exclusion", "Layer_3_hit_comparison_filtering", "Layer 3 hit comparison filtering");

		this -> save_as_two_by_one_canvas(this -> clusters_on_ROCs_map_layer_1, this -> bad_ROCs_cluster_occupancy_base_layer_1,
			                             output_file, "ROC_exclusion", "Layer_1 cluster-occupancy filtering", "Layer 1 cluster occupancy filtering");
		this -> save_as_two_by_one_canvas(this -> clusters_on_ROCs_map_layer_2, this -> bad_ROCs_cluster_occupancy_base_layer_2,
			                             output_file, "ROC_exclusion", "Layer_2 cluster-occupancy filtering", "Layer 2 cluster occupancy filtering");
		this -> save_as_two_by_one_canvas(this -> clusters_on_ROCs_map_layer_3, this -> bad_ROCs_cluster_occupancy_base_layer_3,
			                             output_file, "ROC_exclusion", "Layer_3 cluster-occupancy filtering", "Layer 3 cluster occupancy filtering");

		this -> save_as_two_by_two_canvas(this -> bad_ROCs_simple_efficiency_base_layer_1,  this -> bad_ROCs_cluster_occupancy_base_layer_1,
			                              this -> bad_ROCs_neighbour_hits_base_layer_1, this -> bad_ROCs_all_layer_1, 
			                              output_file, "ROC_exclusion", "Layer_1_Filters", "Layer_1_Filters");
		this -> save_as_two_by_two_canvas(this -> bad_ROCs_simple_efficiency_base_layer_2,  this -> bad_ROCs_cluster_occupancy_base_layer_2,
			                              this -> bad_ROCs_neighbour_hits_base_layer_2, this -> bad_ROCs_all_layer_2, 
			                                                           output_file, "ROC_exclusion", "Layer_2_Filters", "Layer_2_Filters");
		this -> save_as_two_by_two_canvas(this -> bad_ROCs_simple_efficiency_base_layer_3,  this -> bad_ROCs_cluster_occupancy_base_layer_3,
			                              this -> bad_ROCs_neighbour_hits_base_layer_3, this -> bad_ROCs_all_layer_3, 
			                              output_file, "ROC_exclusion", "Layer_3_Filters", "Layer_3_Filters");

		this -> save_histogram_as_canvas(this -> bad_ROC_filtering_3_distribution_layer_1);
		this -> save_histogram_as_canvas(this -> bad_ROC_filtering_3_distribution_layer_2);
		this -> save_histogram_as_canvas(this -> bad_ROC_filtering_3_distribution_layer_3);

		this -> save_histogram_as_canvas(this -> bad_ROC_filtering_3_fit_layer_1);
		this -> save_histogram_as_canvas(this -> bad_ROC_filtering_3_fit_layer_2);
		this -> save_histogram_as_canvas(this -> bad_ROC_filtering_3_fit_layer_3);

		this -> save_histogram_as_canvas(this -> bad_ROCs_cluster_occupancy_base_layer_1);
		this -> save_histogram_as_canvas(this -> bad_ROCs_cluster_occupancy_base_layer_2);
		this -> save_histogram_as_canvas(this -> bad_ROCs_cluster_occupancy_base_layer_3);
	}

	// this -> save_histogram_as_canvas(this -> bad_ROCs_neighbour_hits_base_layer_1);
	// this -> save_histogram_as_canvas(this -> bad_ROCs_neighbour_hits_base_layer_2);
	// this -> save_histogram_as_canvas(this -> bad_ROCs_neighbour_hits_base_layer_3);

	output_file -> Close();
	delete output_file;
}

#endif