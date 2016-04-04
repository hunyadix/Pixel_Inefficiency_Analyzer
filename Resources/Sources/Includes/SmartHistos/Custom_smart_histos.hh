#ifndef CUSTOM_SMART_HISTOS_H
#define CUSTOM_SMART_HISTOS_H

#include "SmartHistos.hh"

#include "../Histogram_generation/Postfixes/Postfix_base.hh"
#include "../Histogram_generation/Fill_parameters/Fill_parameter_base.hh"
#include "../Histogram_generation/Cuts/SmartHisto_cut.hh"

#include <memory>
#include <functional>
#include <stdexcept>

/*
Postfix parameters:

const char* name;
std::function<size_t()> sel;
std::string pf;
std::string leg;
std::string colz;
*/

auto always_true_function = [] () { return true; }; 

class Custom_smart_histos : public SmartHistos
{
	using SmartHistos::AddNewPostfix;
	private:
		std::vector<std::string> tree_type_keyword_list;

		/////////////////////
		// Fill parameters //
		/////////////////////

		std::map<std::string, double> fill_param_name_to_fill_param_double_map;
		std::map<std::string, Fill_parameter*> fill_param_name_to_fill_param_object_map;
		std::map<std::string, std::vector<std::string>> tree_name_to_fill_param_name_collection_map;

		//////////
		// Cuts //
		//////////

		std::map<std::string, bool> cut_name_to_cut_boolean_map;
		std::map<std::string, Histogram_generation::Cut*> cut_name_to_cut_object_map;
		std::map<std::string, std::vector<std::string>> tree_name_to_cut_name_collection_map;

		/////////////
		// Utility //
		/////////////

		void unique_push(std::vector<std::string>& destination, const std::string element) const
		{
			if(std::find(destination.begin(), destination.end(), element) == destination.end())
			{
				destination.push_back(element);
			}
		};
		void conditional_unique_push(bool condition, std::vector<std::string>& destination, std::string element) const
		{
			if(condition)
			{
				unique_push(destination, element);
			}
		};
		template<typename Key_type, typename Value_type>
		Key_type find_key_in_map(std::map<Key_type, Value_type>& map_to_search, Value_type& value_to_find)
		{
			auto has_the_element = [&value_to_find] (const std::pair<Key_type, Value_type>& value_to_check) {return value_to_check.second == value_to_find;};
			auto map_it = std::find_if(map_to_search.begin(), map_to_search.end(), has_the_element);
			if(map_it == map_to_search.end())
			{
				std::cerr << error_prompt << "Error in find_key_in_map: value not found." << std::endl;
			}
			return map_it.first;
		}

	public:
		void AddHistoType(std::string type)
		{
			// std::cout << debug_prompt << "Histo type: " << type << std::endl;
			tree_type_keyword_list.push_back(type);
			SmartHistos::AddHistoType(type);
		};

		void AddNewPostfix(std::unique_ptr<Postfix>& pf_p)
		{
			SmartHistos::AddNewPostfix(pf_p -> name, pf_p -> sel, pf_p -> pf, pf_p -> leg, pf_p -> colz);
		};

		void AddNewFillParam(std::unique_ptr<Fill_parameter>& fill_parameter)
		{
			const std::string fill_param_name = fill_parameter -> name;
			if(fill_param_name_to_fill_param_object_map.count(fill_param_name) || fill_param_name_to_fill_param_double_map.count(fill_param_name))
			{
				std::cerr << error_prompt << "Fill parameter overwrite requested for fill parameter named " << fill_param_name << " in Custom_smart_histos::AddNewCut(). This operation is not supported." << std::endl;
				exit(-1);
			}
			fill_param_name_to_fill_param_object_map[fill_param_name] = fill_parameter.get();
			fill_param_name_to_fill_param_double_map.insert(std::make_pair(fill_param_name, 0.0));

			const double& fill_param_double_to_read = fill_param_name_to_fill_param_double_map.at(fill_param_name);
			std::function<double()> hand_updated_fill_parameter_function = [&fill_param_double_to_read]()
			{
				// std::cerr << debug_prompt << "Call to fill_parameter function, yields: " << fill_param_double_to_read << "." << std::endl;
				return fill_param_double_to_read;
			};
			SmartHistos::AddNewFillParam(fill_param_name, SmartHistos::FillParams(
			{
				.nbin = fill_parameter -> nbin,
				.bins = fill_parameter -> bins,
				.fill = hand_updated_fill_parameter_function,
				// .fill = fill_parameter -> fill,
				.axis_title = fill_parameter -> axis_title
			}));
		};

		void AddNewCut(std::unique_ptr<Histogram_generation::Cut>& cut_p)
		{
			const std::string cut_name(cut_p -> get_name());
			if(cut_name == "")
			{
				SmartHistos::AddNewCut(cut_name, always_true_function);
				return;
			}
			if(cut_name_to_cut_object_map.count(cut_name) || cut_name_to_cut_boolean_map.count(cut_name))
			{
				std::cerr << error_prompt << "Cut overwrite requested for cut named " << cut_name << " in Custom_smart_histos::AddNewCut(). This operation is not supported." << std::endl;
				exit(-1);
			}
			cut_name_to_cut_object_map[cut_name] = cut_p.get();
			cut_name_to_cut_boolean_map.insert(std::make_pair(cut_name, true));

			const bool& cut_bool_to_read = cut_name_to_cut_boolean_map.at(cut_name);
			std::function<bool()> hand_updated_cut_function = [&cut_bool_to_read]()
			{
				// std::cerr << debug_prompt << "Call to cut function, yields: " << cut_bool_to_read << "." << std::endl;
				return cut_bool_to_read;
			};
			SmartHistos::AddNewCut(cut_name, hand_updated_cut_function);
			std::cerr << debug_prompt << "Cut with name = \"" << cut_name << "\" added to the Custom_smart_histos' cuts." << std::endl;
		};

		void AddHistos(std::string tree_type, HistoParams hp, bool AddCutsToTitle = true)
		{
			// typedef struct HistoParams { std::string fill; std::vector<std::string> pfs; std::vector<std::string> cuts; std::string draw; std::string opt; std::vector<double> ranges; } HistoParams;
			std::vector<std::string> fill_parameter_names;
			std::string fill(hp.fill);
			size_t begin = 0;
			size_t end = fill.find("_vs_", begin);
			while(end != std::string::npos)
			{
			  fill_parameter_names.push_back(fill.substr(begin, end - begin));
			  fill.erase(end, 4);
			  begin = end;
			  end = fill.find("_vs_", begin);
			}
			fill_parameter_names.push_back(fill.substr(begin,fill.length() - begin));
			for(const auto& fill_parameter_name : fill_parameter_names )
			{
				// std::cerr << debug_prompt << "Fill parameter with name: " << fill_parameter_name << " added to the list to update for the tree called " << tree_type << "." << std::endl;
				unique_push(tree_name_to_fill_param_name_collection_map[tree_type], fill_parameter_name);
			}
			std::vector<std::string>& cut_names = hp.cuts;
			for(const auto& cut_name: cut_names)
			{
				unique_push(tree_name_to_cut_name_collection_map[tree_type], cut_name);
			}
			SmartHistos::AddHistos(tree_type, hp, AddCutsToTitle);
		}

		void UpdateFillParameter(const std::string& name)
		{
			// std::cerr << "updating fill parameter..." << std::endl;
			if(fill_param_name_to_fill_param_double_map.count(name) && fill_param_name_to_fill_param_object_map.count(name))
			{
				double& fill_parameter_to_update = fill_param_name_to_fill_param_double_map.at(name);
				Fill_parameter& fill_parameter_evaluation_object = *(fill_param_name_to_fill_param_object_map.at(name));
				fill_parameter_to_update = fill_parameter_evaluation_object();
				// std::cerr << debug_prompt << "Fill parameter with name: " << name << " updated to contain the value of: " << fill_parameter_to_update << "." << std::endl;
			}
		}

		void UpdateCut(const std::string& name)
		{
			// std::cerr << "updating cut..." << std::endl;
			if(cut_name_to_cut_boolean_map.count(name) && cut_name_to_cut_object_map.count(name))
			{
				bool& cut_to_update = cut_name_to_cut_boolean_map.at(name);
				// std::cerr << "updating..." << std::endl;
				Histogram_generation::Cut& cut_evaluation_object = *(cut_name_to_cut_object_map.at(name));
				// std::cerr << "updating..." << std::endl;
				cut_to_update = cut_evaluation_object();
				// std::cerr << "updating..." << std::endl;
			}
		}

		void UpdateRelevantFillParameters(const std::string& tree_type_keyword)
		{
			// std::cerr << debug_prompt << "Call to UpdateRelevantFillParameters()." << std::endl;
			for(const auto& fill_parameter_name: tree_name_to_fill_param_name_collection_map[tree_type_keyword])
			{
				// std::cerr << debug_prompt << "Updating: " << fill_parameter_name << "..." << std::endl;
				this -> UpdateFillParameter(fill_parameter_name);
				// std::cerr << debug_prompt << "Fill_parameter with name: " << fill_parameter_name << " updated." << std::endl;
				// std :: cin.get();
			}
		};
		
		void UpdateRelevantCuts(const std::string& tree_type_keyword)
		{
			// std::cerr << debug_prompt << "Call to UpdateRelevantCuts()." << std::endl;
			for(const auto& cut_name: tree_name_to_cut_name_collection_map[tree_type_keyword])
			{
				// std::cerr << debug_prompt << "Updating: " << cut_name << "..." << std::endl;
				this -> UpdateCut(cut_name);
				// std::cerr << debug_prompt << "Cut with name: " << cut_name << " updated." << std::endl;
			}
		};

		void UpdateRelevantFillParametersAndCuts(const std::string& tree_type_keyword)
		{
			UpdateRelevantFillParameters(tree_type_keyword);
			UpdateRelevantCuts(tree_type_keyword);
		}
};

#endif