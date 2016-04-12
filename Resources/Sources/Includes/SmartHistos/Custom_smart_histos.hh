#ifndef CUSTOM_SMART_HISTOS_H
#define CUSTOM_SMART_HISTOS_H

#include "SmartHistos.hh"

#include "../Histogram_generation/Postfixes/Postfix_base.hh"
#include "../Histogram_generation/Fill_parameters/Fill_parameter_base.hh"
#include "../Histogram_generation/Cuts/SmartHisto_cut.hh"

#include <memory>
#include <algorithm>
#include <functional>
#include <stdexcept>

class Custom_smart_histos : public SmartHistos
{
	private:
		std::vector<std::string> tree_type_keyword_list;

		/////////////////////
		// Fill parameters //
		/////////////////////

		std::map<std::string, double> fill_param_name_to_fill_param_double_map;
		std::map<std::string, Fill_parameter*> fill_param_name_to_fill_param_object_map;
		std::map<std::string, std::vector<std::string>> tree_name_to_fill_param_name_collection_map;

		///////////////
		// Postfixes //
		///////////////

		std::map<std::string, size_t> postfix_name_to_postfix_size_t_map;
		std::map<std::string, Postfix*> postfix_name_to_postfix_object_map;
		std::map<std::string, std::vector<std::string>> tree_name_to_postfix_name_collection_map;

		//////////
		// Cuts //
		//////////

		std::map<std::string, bool> cut_name_to_cut_boolean_map;
		std::map<std::string, Histogram_generation::Cut*> cut_name_to_cut_object_map;
		std::map<std::string, std::vector<std::string>> tree_name_to_cut_name_collection_map;

		/////////////
		// Utility //
		/////////////

		void unique_push(std::vector<std::string>& destination, const std::string element) const;
		void conditional_unique_push(bool condition, std::vector<std::string>& destination, std::string element) const;
		template<typename Key_type, typename Value_type>
		Key_type find_key_in_map(std::map<Key_type, Value_type>& map_to_search, Value_type& value_to_find);

	public:

		void AddHistoType(std::string type);
		void AddNewFillParam(std::unique_ptr<Fill_parameter>& fill_parameter);
		void AddNewPostfix(std::unique_ptr<Postfix>& postfix_p);
		void AddNewCut(std::unique_ptr<Histogram_generation::Cut>& cut_p);
		void AddHistos(std::string tree_type, HistoParams hp, bool AddCutsToTitle = true);
		void UpdateFillParameter(const std::string& name);
		void UpdatePostfix(const std::string& name);
		void UpdateCut(const std::string& name);
		void UpdateRelevantFillParameters(const std::string& tree_type_keyword);
		void UpdateRelevantPostfixes(const std::string& tree_type_keyword);
		void UpdateRelevantCuts(const std::string& tree_type_keyword);
		void UpdateRelevantFillParametersPostfixesCuts(const std::string& tree_type_keyword);
};

#endif
