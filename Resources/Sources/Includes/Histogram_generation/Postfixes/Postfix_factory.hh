#ifndef POSTFIX_FACTORY_H
#define POSTFIX_FACTORY_H

#include "Postfix_base.hh"
#include "../../Ntuple_reader/Ntuple_reader.hh"

#include "Postfix_recipes/Layers_postfix.hh"
#include "Postfix_recipes/Ring12_postfix.hh"


class Postfix_factory
{
	private:
		static std::vector<std::unique_ptr<Postfix>> postfix_list;
	public:
		static std::unique_ptr<Postfix>& get_postfix(const std::string& type_p, Ntuple_reader*& ntuple_reader_p);
};


std::unique_ptr<Postfix>& Postfix_factory::get_postfix(const std::string& type_p, Ntuple_reader*& ntuple_reader_p)
{
	if(type_p == "Layers")
	{
		postfix_list.push_back(std::unique_ptr<Postfix>(new Layers_postfix(ntuple_reader_p)));
		return postfix_list.back();
	}
	if(type_p == "Ring12")
	{
		postfix_list.push_back(std::unique_ptr<Postfix>(new Ring12_postfix(ntuple_reader_p)));
		return postfix_list.back();
	}
	std::cerr << error_prompt << "Failed to generate postfix: " << type_p << ". (Invalid keyword?)" << std::endl;
	exit(-1);
	return postfix_list.back(); // never reached
}

std::vector<std::unique_ptr<Postfix>> Postfix_factory::postfix_list;

#endif