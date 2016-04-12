#include "Tree_initializer.hh"

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

void Tree_initializer::branch_check(TTree* tree_p, const std::string& branch_name_p)
{
	TBranch* branch = static_cast<TBranch*>(tree_p -> GetListOfBranches() -> FindObject(branch_name_p.c_str()));
	if(!branch)
	{
		std::cerr << error_prompt << "Missing branch: " << branch_name_p << std::endl;	
		tree_p -> SetBranchStatus(branch_name_p.c_str(), 0);
	}
	tree_p -> SetBranchStatus(branch_name_p.c_str(), 1);	
}

TBranch* Tree_initializer::check_get_branch(TTree* tree_p, const std::string& branch_name_p)
{
	branch_check(tree_p, branch_name_p);
	TBranch* branch = tree_p -> GetBranch(branch_name_p.c_str());
	return branch;
}

void Tree_initializer::prepare_to_run_on_event_tree()
{
	this -> clear_fields();
	this -> event_tree = (TTree*)(this -> input_file -> Get("eventTree"));
	this -> tree_check(this -> event_tree, "Error opening the eventTree.");
	this -> check_get_branch(event_tree, "event") -> SetAddress(&(this -> event_field));
}

void Tree_initializer::prepare_to_run_on_lumi_tree()
{
	this -> clear_fields();
	this -> lumi_tree = (TTree*)(this -> input_file -> Get("lumiTree"));
	this -> tree_check(this -> lumi_tree, "Error opening the lumiTree.");
	this -> check_get_branch(lumi_tree, "lumi") -> SetAddress(&(this -> lumi_field));
}

void Tree_initializer::prepare_to_run_on_run_tree()
{
	this -> clear_fields();
	this -> run_tree = (TTree*)(this -> input_file -> Get("runTree"));
	this -> tree_check(this -> run_tree, "Error opening the runTree.");
	this -> check_get_branch(run_tree, "run") -> SetAddress(&(this -> run_field));
}

void Tree_initializer::prepare_to_run_on_traj_tree()
{
	this -> clear_fields();
	this -> traj_tree = (TTree*)(this -> input_file -> Get("trajTree"));
	this -> tree_check(this -> traj_tree, "Error opening the trajTree.");
	this -> check_get_branch(traj_tree, "event")     -> SetAddress(&(this -> event_field));
	this -> check_get_branch(traj_tree, "module_on") -> SetAddress(&(this -> traj_field.mod_on));
	this -> check_get_branch(traj_tree, "traj")      -> SetAddress(&(this -> traj_field));
	this -> check_get_branch(traj_tree, "traj_alphabeta") -> SetAddress(&(this -> traj_field.alpha));
	this -> check_get_branch(traj_tree, "traj_dxy_cl") -> SetAddress(&(this -> traj_field.dx_cl));
	// this -> check_get_branch(traj_tree, "traj_occup") -> SetAddress(&(this -> traj_field.nclu_mod));
	// this -> check_get_branch(traj_tree, "traj_dxy_hit") -> SetAddress(&(this -> traj_field.dx_hit));
	this -> check_get_branch(traj_tree, "track")     -> SetAddress(&(this -> traj_field.trk));
	this -> check_get_branch(traj_tree, "track_eta") -> SetAddress(&(this -> traj_field.trk.eta));
	this -> module_field_ptr = &(this -> traj_field.mod_on);
}

void Tree_initializer::prepare_to_run_on_clust_tree()
{
	this -> clear_fields();
	this -> clust_tree = (TTree*)(this -> input_file -> Get("clustTree"));
	this -> tree_check(this -> clust_tree, "Error opening the clustTree.");
	this -> check_get_branch(clust_tree, "event")        -> SetAddress(&(this -> event_field));
	this -> check_get_branch(clust_tree, "module_on")    -> SetAddress(&(this -> cluster_field.mod_on));
	this -> check_get_branch(clust_tree, "clust_xy")     -> SetAddress(&(this -> cluster_field.x));
	this -> check_get_branch(clust_tree, "clust_sizeXY") -> SetAddress(&(this -> cluster_field.sizeX));
	this -> check_get_branch(clust_tree, "clust_i")      -> SetAddress(&(this -> cluster_field.i));
	this -> check_get_branch(clust_tree, "clust_edge")   -> SetAddress(&(this -> cluster_field.edge));
	this -> check_get_branch(clust_tree, "clust_badpix") -> SetAddress(&(this -> cluster_field.badpix));
	this -> check_get_branch(clust_tree, "clust_tworoc") -> SetAddress(&(this -> cluster_field.tworoc));
	this -> check_get_branch(clust_tree, "clust_size")   -> SetAddress(&(this -> cluster_field.size));
	this -> check_get_branch(clust_tree, "clust_charge") -> SetAddress(&(this -> cluster_field.charge));
	this -> check_get_branch(clust_tree, "clust_adc")    -> SetAddress(&(this -> cluster_field.adc));
	this -> check_get_branch(clust_tree, "clust_pix")    -> SetAddress(&(this -> cluster_field.pix));
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