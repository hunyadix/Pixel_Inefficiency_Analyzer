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

#include "TObjArray.h"

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

		void     tree_check(TTree* tree_p, std::string error_message_p);
		void     branch_check(TTree* tree_p, const std::string& branch_name_p);
		TBranch* check_get_branch(TTree* tree_p, const std::string& branch_name_p);
		TTree*   prepare_to_run_on_event_tree();
		TTree*   prepare_to_run_on_lumi_tree();
		TTree*   prepare_to_run_on_run_tree();
		TTree*   prepare_to_run_on_traj_tree();
		TTree*   prepare_to_run_on_clust_tree();
		void     clear_fields();
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

#endif