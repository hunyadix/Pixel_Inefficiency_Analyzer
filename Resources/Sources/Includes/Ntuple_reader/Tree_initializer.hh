#ifndef DATA_FETCHER_H
#define DATA_FETCHER_H

#include "../Tree_structures/Event_tree_structure.hh"
#include "../Tree_structures/Luminosity_tree_structure.hh"
#include "../Tree_structures/Run_structure.hh"
#include "../Tree_structures/Cluster.hh"
#include "../Tree_structures/Traj_measurement.hh"
#include "../Input_output_interface/Input_output_interface.hh"
#include "../Console_actor/common_actors.hh"

#include "TTree.h"

extern bool debug;


class Tree_initializer : public Input_output_interface
{
	protected:
		TTree*           event_tree;
		TTree*           lumi_tree;
		TTree*           run_tree;
		TTree*           traj_tree;
		TTree*           clust_tree;
		EventData        event_field;
		LumiData         lumi_field;
		RunData          run_field;
		Cluster          cluster_field;
		TrajMeasurement  traj_field;
		ModuleData*      module_field_ptr = NULL;

		void tree_check(TTree* tree_p, std::string error_message_p);
		void prepare_to_run_on_event_tree();
		void prepare_to_run_on_lumi_tree();
		void prepare_to_run_on_run_tree();
		void prepare_to_run_on_traj_tree();
		void prepare_to_run_on_clust_tree();
		void clear_fields();
		//void init_trees();

	public:
		EventData*       get_event_field_ptr()  ;
		LumiData*        get_lumi_field_ptr()   ;
		RunData*         get_run_field_ptr()    ;
		ModuleData*      get_module_field_ptr()   ;
		Cluster*         get_cluster_field_ptr();
		TrajMeasurement* get_traj_field_ptr()   ;
		Tree_initializer();
		virtual ~Tree_initializer();

		EventData* get_event_field() const;


};

Tree_initializer::Tree_initializer()
{

}

Tree_initializer::~Tree_initializer()
{
	if(debug)
	{
		std::cout << debug_prompt << "~Tree_initializer()..." << std::endl;
	}
}

EventData*       Tree_initializer::get_event_field_ptr()   { return &(this -> event_field); }
LumiData*        Tree_initializer::get_lumi_field_ptr()    { return &(this -> lumi_field); }
RunData*         Tree_initializer::get_run_field_ptr()     { return &(this -> run_field); }
ModuleData*      Tree_initializer::get_module_field_ptr()  { return   this -> module_field_ptr; }
Cluster*         Tree_initializer::get_cluster_field_ptr() { return &(this -> cluster_field); }
TrajMeasurement* Tree_initializer::get_traj_field_ptr()    { return &(this -> traj_field); }

void Tree_initializer::tree_check(TTree* tree_p, std::string error_message_p)
{
	if(tree_p == NULL)
	{
		std::cerr << error_prompt << error_message_p << std::endl;
		exit(-1);
	}
}

void Tree_initializer::prepare_to_run_on_event_tree()
{
	this -> clear_fields();
	this -> event_tree = (TTree*)(this -> input_file -> Get("eventTree"));
	this -> tree_check(this -> event_tree, "Error opening the eventTree.");
	this -> event_tree -> GetBranch("event") -> SetAddress(&(this -> event_field));
}

void Tree_initializer::prepare_to_run_on_lumi_tree()
{
	this -> clear_fields();
	this -> lumi_tree = (TTree*)(this -> input_file -> Get("lumiTree"));
	this -> tree_check(this -> lumi_tree, "Error opening the lumiTree.");
	this -> lumi_tree -> GetBranch("lumi") -> SetAddress(&(this -> lumi_field));
}

void Tree_initializer::prepare_to_run_on_run_tree()
{
	this -> clear_fields();
	this -> run_tree = (TTree*)(this -> input_file -> Get("runTree"));
	this -> tree_check(this -> run_tree, "Error opening the runTree.");
	this -> run_tree -> GetBranch("run") -> SetAddress(&(this -> run_field));
}

void Tree_initializer::prepare_to_run_on_traj_tree()
{
	this -> clear_fields();
	this -> traj_tree = (TTree*)(this -> input_file -> Get("trajTree"));
	this -> tree_check(this -> traj_tree, "Error opening the trajTree.");
	this -> traj_tree -> GetBranch("event")     -> SetAddress(&(this -> event_field));
	this -> traj_tree -> GetBranch("module_on") -> SetAddress(&(this -> traj_field.mod_on));
	this -> traj_tree -> GetBranch("traj")      -> SetAddress(&(this -> traj_field));
	if(this -> traj_tree -> GetBranch("traj_alphabeta"))
	{
		this -> traj_tree -> GetBranch("traj_alphabeta") -> SetAddress(&(this -> traj_field.alpha));
	}
	// this -> traj_tree -> GetBranch("traj_alphabeta") -> SetAddress(&this -> traj_field.alpha);
	// this -> traj_tree -> GetBranch("traj_alphabeta") -> SetAddress(traj_field.alpha);
	// this -> traj_tree -> GetBranch("traj_alphabeta") -> SetAddress(&(traj_field.alphabeta));
	this -> traj_tree -> GetBranch("track")     -> SetAddress(&(this -> traj_field.trk));
	this -> module_field_ptr = &(this -> traj_field.mod_on);
}

void Tree_initializer::prepare_to_run_on_clust_tree()
{
	this -> clear_fields();
	this -> clust_tree = (TTree*)(this -> input_file -> Get("clustTree"));
	this -> tree_check(this -> clust_tree, "Error opening the clustTree.");
	this -> clust_tree -> GetBranch("event")        -> SetAddress(&(this -> event_field));
	this -> clust_tree -> GetBranch("module_on")    -> SetAddress(&(this -> cluster_field.mod_on));
	this -> clust_tree -> GetBranch("clust_xy")     -> SetAddress(&(this -> cluster_field.x));
	this -> clust_tree -> GetBranch("clust_sizeXY") -> SetAddress(&(this -> cluster_field.sizeX));
	this -> clust_tree -> GetBranch("clust_i")      -> SetAddress(&(this -> cluster_field.i));
	this -> clust_tree -> GetBranch("clust_edge")   -> SetAddress(&(this -> cluster_field.edge));
	this -> clust_tree -> GetBranch("clust_badpix") -> SetAddress(&(this -> cluster_field.badpix));
	this -> clust_tree -> GetBranch("clust_tworoc") -> SetAddress(&(this -> cluster_field.tworoc));
	this -> clust_tree -> GetBranch("clust_size")   -> SetAddress(&(this -> cluster_field.size));
	this -> clust_tree -> GetBranch("clust_charge") -> SetAddress(&(this -> cluster_field.charge));
	this -> clust_tree -> GetBranch("clust_adc")    -> SetAddress(&(this -> cluster_field.adc));
	this -> clust_tree -> GetBranch("clust_pix")    -> SetAddress(&(this -> cluster_field.pix));
	this -> module_field_ptr = &(this -> cluster_field.mod_on);
}

void Tree_initializer::clear_fields()
{
	EventData       clear_event_data;
	LumiData        clear_lumi_data;
	RunData         clear_run_data;
	Cluster         clear_cluster_data;
	TrajMeasurement clear_traj_data;
	this -> event_field       = clear_event_data;
	this -> lumi_field        = clear_lumi_data;
	this -> run_field         = clear_run_data;
	this -> module_field_ptr  = NULL;
	this -> cluster_field     = clear_cluster_data;
	this -> traj_field        = clear_traj_data;
}

#endif