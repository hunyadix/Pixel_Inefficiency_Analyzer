#ifndef HISTOGRAM_REWEIGHTER_H
#define HISTOGRAM_REWEIGHTER_H

class Histogram_reweighter
{
	/////////////////
	// Data fields //
	/////////////////
	
	private:

		int MAX_PU                             = 100;
		Ntuple_reader* ntuple_reader           = nullptr;
		std::vector<double> pu_weights;

	///////////////
	// Interface //
	///////////////

	public:

		Histogram_reweighter(Ntuple_reader* ntuple_reader_p);
		~Histogram_reweighter();

		std::function<double()> get_reweighter(std::string reweighting_file_path, std::string reweighting_histogram_name, bool normalize_mc);

	/////////////
	// Utility //
	/////////////

	private:
		void normalize_histogram(TH1D* histogram);
		void read_pu_weights_from_file(std::string reweighting_file_path, std::string reweighting_histogram_name, bool normalize_mc);

};

Histogram_reweighter::Histogram_reweighter(Ntuple_reader* ntuple_reader_p) : ntuple_reader(ntuple_reader_p) {}

Histogram_reweighter::~Histogram_reweighter() {}

std::function<double()>  Histogram_reweighter::get_reweighter(std::string reweighting_file_path, std::string reweighting_histogram_name, bool normalize_mc)
{
	read_pu_weights_from_file(reweighting_file_path, reweighting_histogram_name, normalize_mc);
	std::cerr << debug_prompt << "Histogram_reweighter: reading pileup weights from reweighting histogram file succeded!" << std::endl;
	// Set weights for all Histos, except Background estimate, where MC is scaled to MC_Expected_IntLumi_invfb
	return [this]()
	{
		Ntuple_reader*& ntuple_reader   = this -> ntuple_reader;
		std::vector<double>& pu_weights = this -> pu_weights;
		// std::cerr << "Pileup weights vector size: " << pu_weights.size() << std::endl;
		int& MAX_PU                     = this -> MAX_PU;
		// std::cerr << "MAX_PU: " << MAX_PU << std::endl;
		bool is_data                    = ntuple_reader -> get_event_field_ptr()->run != 1;
		// std::cerr << "Data: " << is_data << std::endl;
		float& pileup                   = ntuple_reader -> get_event_field_ptr()->pileup;
		// std::cerr << "Pileup: " << pileup << std::endl;
		if(is_data)
		{
			return static_cast<double>(1.0);
		}
		else
		{
			if(pileup < MAX_PU)
			{
				int intpileup = static_cast<int>(pileup);
				// if(intpileup < 0 || pu_weights.size() < static_cast<unsigned int>(intpileup))
				// {
				// 	std::cerr << error_prompt << "in Histogram_reweighter::_reweighter_lambda: invalid pileup: " << intpileup << "." << std :: endl;
				// 	return static_cast<double>(0.0);
				// }
				return pu_weights[intpileup]; // Implies 0-1: Data, Rest: MC
			}
			else
			{
				std::cerr << error_prompt << "Error int Histogram_reweighter::Histogram_get_reweighter(): Maximum pileup exceeded." << std::endl;
				return static_cast<double>(0.0);
			}
		}
	};
}

void Histogram_reweighter::read_pu_weights_from_file(std::string reweighting_file_path, std::string reweighting_histogram_name, bool normalize_mc)
{
	TFile *f_data        = TFile::Open(reweighting_file_path.c_str());
	if(!(f_data -> IsOpen()))
	{
		std::cerr << error_prompt << "In Histogram_reweighter:: failed to open the file containing the reweighting info." << std::endl;
	}
	TH1D* mc_histogram   = new TH1D("mc_histogram_pileup", "", 100, 0, 100);
	TH1D* data_histogram = (TH1D*)f_data->Get(reweighting_histogram_name.c_str());
	if(!data_histogram)
	{
		std::cerr << error_prompt << "In Histogram_reweighter:: failed to fetch data histogram." << std::endl;
	}
	
	for (int pu = 0; pu <= 50; ++pu)
	{
		mc_histogram -> Fill(pu);
	}
	
	if(data_histogram != nullptr && mc_histogram != nullptr) 
	{
		if (!normalize_mc) 
		{
			normalize_histogram(data_histogram);
			normalize_histogram(mc_histogram);
		}

		for(int npu = 0; npu < MAX_PU; ++npu)
		{
			if(mc_histogram -> GetBinContent(npu + 1) > 0)
			{
				pu_weights.push_back(data_histogram -> GetBinContent(npu + 1) / mc_histogram -> GetBinContent(npu + 1));
			}
			else
			{
				pu_weights.push_back(0.0);
			}
		}
	}
	f_data->Close();
}

void Histogram_reweighter::normalize_histogram(TH1D* histogram)
{
	double histogram_integral = histogram -> Integral();
	if(histogram_integral != 0)
	{
		histogram -> Scale(1.0 / histogram_integral);
	}
	else
	{
		std::cerr << error_prompt << "Error int Histogram_reweighter::normalize_histogram(): trying to normalize histogram with integral of 0." << std::endl;
	}
}

#endif // HISTOGRAM_REWEIGHTER_H
