#ifndef POSTFIX_FACTORY_H
#define POSTFIX_FACTORY_H

#include "Postfix_base.hh"
#include "../../Ntuple_reader/Ntuple_reader.hh"

#include "Postfix_recipes/Validhit_postfix.hh"
#include "Postfix_recipes/Layers_postfix.hh"
#include "Postfix_recipes/Modules_postfix.hh"
#include "Postfix_recipes/Ring12_postfix.hh"
#include "Postfix_recipes/Eta_postfix.hh"
#include "Postfix_recipes/Alpha_postfix.hh"
#include "Postfix_recipes/Beta_postfix.hh"


class Postfix_factory
{
	private:
		static std::vector<std::unique_ptr<Postfix>> postfix_list;
	public:
		static std::unique_ptr<Postfix>& get_postfix(const std::string& type_p, Ntuple_reader*& ntuple_reader_p);
};


std::unique_ptr<Postfix>& Postfix_factory::get_postfix(const std::string& type_p, Ntuple_reader*& ntuple_reader_p)
{
	if(type_p == "Validhit")
	{
		postfix_list.push_back(std::unique_ptr<Postfix>(new Validhit_postfix(ntuple_reader_p)));
		return postfix_list.back();
	}
	if(type_p == "Layers")
	{
		postfix_list.push_back(std::unique_ptr<Postfix>(new Layers_postfix(ntuple_reader_p)));
		return postfix_list.back();
	}
	if(type_p == "Modules")
	{
		postfix_list.push_back(std::unique_ptr<Postfix>(new Modules_postfix(ntuple_reader_p)));
		return postfix_list.back();
	}
	if(type_p == "Ring12")
	{
		postfix_list.push_back(std::unique_ptr<Postfix>(new Ring12_postfix(ntuple_reader_p)));
		return postfix_list.back();
	}
	if(type_p == "Eta")
	{
		postfix_list.push_back(std::unique_ptr<Postfix>(new Eta_postfix(ntuple_reader_p)));
		return postfix_list.back();
	}
	if(type_p == "Alpha")
	{
		postfix_list.push_back(std::unique_ptr<Postfix>(new Alpha_postfix(ntuple_reader_p)));
		return postfix_list.back();
	}
	if(type_p == "Beta")
	{
		postfix_list.push_back(std::unique_ptr<Postfix>(new Beta_postfix(ntuple_reader_p)));
		return postfix_list.back();
	}
	std::cerr << error_prompt << "Failed to generate postfix: " << type_p << ". (Invalid keyword?)" << std::endl;
	exit(-1);
	return postfix_list.back(); // never reached
}

std::vector<std::unique_ptr<Postfix>> Postfix_factory::postfix_list;

#endif