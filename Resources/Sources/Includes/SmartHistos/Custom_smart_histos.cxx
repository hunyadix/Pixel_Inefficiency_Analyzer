#include "Custom_smart_histos.hh"

#include "../Utility/debug_utility.hh"

auto always_true_function = [] () { return true; };

extern bool debug_customsmarthisto_inserting;
extern bool debug_customsmarthisto_updating;

void Custom_smart_histos::unique_push(std::vector<std::string>& destination, const std::string element) const
{
	if(std::find(destination.begin(), destination.end(), element) == destination.end())
	{
		destination.push_back(element);
	}
};

void Custom_smart_histos::conditional_unique_push(bool condition, std::vector<std::string>& destination, std::string element) const
{
	if(condition)
	{
		unique_push(destination, element);
	}
};

template<typename Key_type, typename Value_type>
Key_type Custom_smart_histos::find_key_in_map(std::map<Key_type, Value_type>& map_to_search, Value_type& value_to_find)
{
	auto has_the_element = [&value_to_find] (const std::pair<Key_type, Value_type>& value_to_check) {return value_to_check.second == value_to_find;};
	auto map_it = std::find_if(map_to_search.begin(), map_to_search.end(), has_the_element);
	if(map_it == map_to_search.end())
	{
		std::cerr << error_prompt << "Error in find_key_in_map: value not found." << std::endl;
	}
	return map_it.first;
}

////////////////////
// Public methods //
////////////////////

const bool& Custom_smart_histos::get_cut_boolean(const std::string& name)
{
	if(cut_name_to_cut_boolean_map.count(name))
	{
		if(cut_name_to_cut_boolean_map.at(name).second == false)
		{
			UpdateCut(name);
		}
		bool& cut_to_update = cut_name_to_cut_boolean_map.at(name).first;
		// Histogram_generation::Cut& cut_evaluation_object = *(cut_name_to_cut_object_map.at(name));
		// cut_to_update = cut_evaluation_object();
		return cut_to_update;
	}
	else
	{
		std::cerr << error_prompt << "Error finding cut: \"" << name << "\". Terminating..." << std::endl;
		exit(-1); 
	}
}


void Custom_smart_histos::AddHistoType(std::string type)
{
	conditional_debug_msg(debug_customsmarthisto_inserting, {"Histo_type: ", type});
	tree_type_keyword_list.push_back(type);
	SmartHistos::AddHistoType(type);
};

void Custom_smart_histos::AddNewFillParam(std::unique_ptr<Fill_parameter>& fill_parameter)
{
	const std::string fill_param_name = fill_parameter -> name;
	if(fill_param_name_to_fill_param_object_map.count(fill_param_name) || fill_param_name_to_fill_param_double_map.count(fill_param_name))
	{
		std::cerr << error_prompt << "Fill parameter overwrite requested for fill parameter named " << fill_param_name << " in Custom_smart_histos::AddNewCut(). This operation is not supported." << std::endl;
		exit(-1);
	}
	fill_param_name_to_fill_param_object_map[fill_param_name] = fill_parameter.get();
	fill_param_name_to_fill_param_double_map.insert(std::make_pair(fill_param_name, std::make_pair(0.0, false)));

	const double& fill_param_double_to_read = fill_param_name_to_fill_param_double_map.at(fill_param_name).first;
	std::function<double()> hand_updated_fill_parameter_function = [&fill_param_double_to_read]()
	{
		// conditional_debug_msg(debug_customsmarthisto_updating, {"Call to fill_parameter function, yields: ", std::to_string(fill_param_double_to_read), "."});
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

void Custom_smart_histos::AddNewPostfix(std::unique_ptr<Postfix>& postfix_p)
{
	const std::string postfix_name(postfix_p -> get_name());
	if(postfix_name_to_postfix_object_map.count(postfix_name) || postfix_name_to_postfix_size_t_map.count(postfix_name))
	{
		std::cerr << error_prompt << "Postifx overwrite requested for postfix named " << postfix_name << " in Custom_smart_histos::AddNewPostfix(). This operation is not supported." << std::endl;
		exit(-1);
	}
	postfix_name_to_postfix_object_map[postfix_name] = postfix_p.get();
	postfix_name_to_postfix_size_t_map.insert(std::make_pair(postfix_name, std::make_pair(0, false)));

	const size_t& postfix_size_t_to_read = postfix_name_to_postfix_size_t_map.at(postfix_name).first;
	std::function<size_t()> hand_updated_postfix_function = [&postfix_size_t_to_read]()
	{
		// conditional_debug_msg(debug_customsmarthisto_updating, {"Call to cut function, yields: ", std::to_string(cut_bool_to_read), "."});
		return postfix_size_t_to_read;
	};
	SmartHistos::AddNewPostfix(postfix_p -> name, hand_updated_postfix_function, postfix_p -> pf, postfix_p -> leg, postfix_p -> colz);
	std::cerr << debug_prompt << "Postfix with name = \"" << postfix_name << "\" added to the Custom_smart_histos' postfixes." << std::endl;
};

void Custom_smart_histos::AddNewCut(std::unique_ptr<Histogram_generation::Cut>& cut_p)
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
	cut_name_to_cut_boolean_map.insert(std::make_pair(cut_name, std::make_pair(true, false)));

	const bool& cut_bool_to_read = cut_name_to_cut_boolean_map.at(cut_name).first;
	std::function<bool()> hand_updated_cut_function = [&cut_bool_to_read]()
	{
		// conditional_debug_msg(debug_customsmarthisto_updating, {"Call to cut function, yields: ", std::to_string(cut_bool_to_read), "."});
		return cut_bool_to_read;
	};
	SmartHistos::AddNewCut(cut_name, hand_updated_cut_function);
	std::cerr << debug_prompt << "Cut with name = \"" << cut_name << "\" added to the Custom_smart_histos' cuts." << std::endl;
};

void Custom_smart_histos::AddHistos(std::string tree_type, HistoParams hp, bool AddCutsToTitle) // Default AddCutsToTitle: true
{
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
		conditional_debug_msg(debug_customsmarthisto_inserting, {"Fill parameter with name: ", fill_parameter_name, " added to the list to update for the tree called ", tree_type, "."});
		unique_push(tree_name_to_fill_param_name_collection_map[tree_type], fill_parameter_name);
	}
	std::vector<std::string>& postfix_names = hp.pfs;
	for(const auto& postfix_name: postfix_names)
	{
		unique_push(tree_name_to_postfix_name_collection_map[tree_type], postfix_name);
	}
	std::vector<std::string>& cut_names = hp.cuts;
	for(const auto& cut_name: cut_names)
	{
		unique_push(tree_name_to_cut_name_collection_map[tree_type], cut_name);
	}
	SmartHistos::AddHistos(tree_type, hp, AddCutsToTitle);
}

void Custom_smart_histos::SetFillParameterStatesUnupdated()
{
	for(auto& param_state: fill_param_name_to_fill_param_double_map)
	{
		param_state.second.second = false;
	}
}

void Custom_smart_histos::SetPostfixStatesUnupdated()
{
	for(auto& param_state: postfix_name_to_postfix_size_t_map)
	{
		param_state.second.second = false;
	}
}

void Custom_smart_histos::SetCutStatesUnupdated()
{
	for(auto& param_state: cut_name_to_cut_boolean_map)
	{
		param_state.second.second = false;
	}
}

void Custom_smart_histos::UpdateFillParameter(const std::string& name)
{
	// conditional_debug_msg(debug_customsmarthisto_updating, "Updating fill parameter...");
	if(fill_param_name_to_fill_param_double_map.count(name) && fill_param_name_to_fill_param_object_map.count(name))
	{
		double& fill_parameter_to_update = fill_param_name_to_fill_param_double_map.at(name).first;
		Fill_parameter& fill_parameter_evaluation_object = *(fill_param_name_to_fill_param_object_map.at(name));
		fill_parameter_to_update = fill_parameter_evaluation_object();
		fill_param_name_to_fill_param_double_map.at(name).second = true;
		// conditional_debug_msg(debug_customsmarthisto_updating, {"Fill parameter with name: ", name, std::to_string(fill_parameter_to_update), "."});
	}
	else
	{
		std::cerr << error_prompt << "Failed to update fill parameter: " << name << ". (Custom_smart_histos inner crash)" << std::endl;
	}
}

void Custom_smart_histos::UpdatePostfix(const std::string& name)
{
	// conditional_debug_msg(debug_customsmarthisto_updating, "updating postfix...");
	if(postfix_name_to_postfix_size_t_map.count(name) && postfix_name_to_postfix_object_map.count(name))
	{
		size_t& postfix_to_update = postfix_name_to_postfix_size_t_map.at(name).first;
		// conditional_debug_msg(debug_customsmarthisto_updating, "updating...");
		Postfix& postfix_evaluation_object = *(postfix_name_to_postfix_object_map.at(name));
		// conditional_debug_msg(debug_customsmarthisto_updating, "updating...");
		postfix_to_update = postfix_evaluation_object();
		postfix_name_to_postfix_size_t_map.at(name).second = true;
		// conditional_debug_msg(debug_customsmarthisto_updating, "updating...");
	}
	else
	{
		std::cerr << error_prompt << "Failed to update postfix: " << name << ". (Custom_smart_histos inner crash)" << std::endl;
	}
}

void Custom_smart_histos::UpdateCut(const std::string& name)
{
	// conditional_debug_msg(debug_customsmarthisto_updating, "updating cut...");
	if(cut_name_to_cut_boolean_map.count(name) && cut_name_to_cut_object_map.count(name))
	{
		bool& cut_to_update = cut_name_to_cut_boolean_map.at(name).first;
		// conditional_debug_msg(debug_customsmarthisto_updating, "updating...");
		Histogram_generation::Cut& cut_evaluation_object = *(cut_name_to_cut_object_map.at(name));
		// conditional_debug_msg(debug_customsmarthisto_updating, "updating...");
		cut_to_update = cut_evaluation_object();
		cut_name_to_cut_boolean_map.at(name).second = true;
		// conditional_debug_msg(debug_customsmarthisto_updating, "updating...");
	}
	else
	{
		if(name == "")
		{
			return;
		}
		std::cerr << error_prompt << "Failed to update cut: " << name << ". (Custom_smart_histos inner crash)" << std::endl;
	}
}

void Custom_smart_histos::UpdateRelevantFillParameters(const std::string& tree_type_keyword)
{
	// conditional_debug_msg(debug_customsmarthisto_updating, "Call to UpdateRelevantFillParameters().");
	for(const auto& fill_parameter_name: tree_name_to_fill_param_name_collection_map[tree_type_keyword])
	{
		// conditional_debug_msg(debug_customsmarthisto_updating, {"Updating: ", fill_parameter_name, "..."});
		this -> UpdateFillParameter(fill_parameter_name);
		// conditional_debug_msg(debug_customsmarthisto_updating, {"Fill_parameter with name: ", fill_parameter_name, " updated."});
		// std :: cin.get();
	}
};

void Custom_smart_histos::UpdateRelevantPostfixes(const std::string& tree_type_keyword)
{
	// conditional_debug_msg(debug_customsmarthisto_updating, "Call to UpdateRelevantPostfixes().");
	for(const auto& postifx_name: tree_name_to_postfix_name_collection_map[tree_type_keyword])
	{
		// conditional_debug_msg(debug_customsmarthisto_updating, {"Updating: ", postifx_name, "..."});
		this -> UpdatePostfix(postifx_name);
		// conditional_debug_msg(debug_customsmarthisto_updating, {"Postfix with name: ", postifx_name, " updated."});
		// std :: cin.get();
	}
};

void Custom_smart_histos::UpdateRelevantCuts(const std::string& tree_type_keyword)
{
	// conditional_debug_msg(debug_customsmarthisto_updating, "Call to UpdateRelevantCuts().");
	for(const auto& cut_name: tree_name_to_cut_name_collection_map[tree_type_keyword])
	{
		// conditional_debug_msg(debug_customsmarthisto_updating, {"Updating: ", cut_name, "..."});
		this -> UpdateCut(cut_name);
		// conditional_debug_msg(debug_customsmarthisto_updating, {"Cut with name: ", cut_name, " updated."});
	}
};

void Custom_smart_histos::UpdateRelevantFillParametersPostfixesCuts(const std::string& tree_type_keyword)
{
	SetFillParameterStatesUnupdated();
	SetPostfixStatesUnupdated();
	SetCutStatesUnupdated();
	UpdateRelevantFillParameters(tree_type_keyword);
	UpdateRelevantCuts(tree_type_keyword);
	UpdateRelevantPostfixes(tree_type_keyword);
}
