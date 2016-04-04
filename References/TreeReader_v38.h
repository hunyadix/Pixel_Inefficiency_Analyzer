#define VERSION1 38

class TreeReader {
 
 public:
  TreeReader(RunData* _run, LumiData* _lumi, EventData* _evt, 
	     EventData* _traj_evt, TrajMeasurement* _traj, 
	     EventData* _clu_evt, Cluster* _clu) { 
    run_=_run; lumi_=_lumi; evt_=_evt;
    traj_evt_=_traj_evt; traj_=_traj;
    clu_evt_=_clu_evt; clu_=_clu;
    tg_traj_evt_ = 1; tg_traj_traj_ = 1; tg_traj_mod_on_ = 1; tg_traj_clu_ = 1; tg_traj_trk_ = 1; tg_traj_mod_ = 0;
    tg_clu_evt_ = 1; tg_clu_mod_on_ = 1; tg_clu_mod_ = 0; tg_clu_clu_ = 1;
  }
  ~TreeReader() { }

 private:
  TFile f_;

  // Toggle reading branches on-off (only for trajTree)
  int tg_traj_evt_;
  int tg_traj_traj_; 
  int tg_traj_mod_on_; 
  int tg_traj_clu_; 
  int tg_traj_trk_; 
  int tg_traj_mod_;
  int tg_clu_evt_;
  int tg_clu_mod_on_; 
  int tg_clu_mod_;
  int tg_clu_clu_;
  
  // Trees
  TTree* runTree_;
  TTree* lumiTree_;
  TTree* eventTree_;
  TTree* trajTree_;
#if CLUST_LOOP > 0
  TTree* clustTree_;
#endif
  
  // Data structures
  RunData*         run_;
  LumiData*        lumi_;
  EventData*       evt_;
  EventData*       traj_evt_;
  TrajMeasurement* traj_;
  EventData*       clu_evt_;
  Cluster*         clu_;
  //TrackData*       trk_;
  //Digi*            digi_;
  
  // Number of entries
  long long int nrun_;
  long long int nls_;
  long long int nevt_;
  long long int nhit_;
  long long int nclu_;

  int debug_;
#if CLUST_LOOP > 0
  void read_(TFile &f, RunData& _run, LumiData& _lumi, EventData& _evt, 
	     EventData& _traj_evt, TrajMeasurement& _traj, 
	     EventData& _clu_evt, Cluster& _clu, std::string subdir="") { 
#else
  void read_(TFile &f, RunData& _run, LumiData& _lumi, EventData& _evt, 
	     EventData& _traj_evt, TrajMeasurement& _traj, std::string subdir="") { 
#endif
    debug_=0;
    if (debug_) std::cout<<"start ok"<<std::endl;
    // runTree
    runTree_ = (TTree*)f.Get((subdir+"runTree").c_str());
    nrun_ = (long long int)runTree_->GetEntries();
    TBranch* b_r_run    = runTree_->GetBranch("run");
    b_r_run    ->SetAddress(&_run);
    // lumiTree
    lumiTree_ = (TTree*)f.Get((subdir+"lumiTree").c_str());
    nls_ = (long long int)lumiTree_->GetEntries();
    TBranch* b_l_lumi    = lumiTree_->GetBranch("lumi");
    b_l_lumi    ->SetAddress(&_lumi);
    // eventTree
    eventTree_ = (TTree*)f.Get((subdir+"eventTree").c_str());
    nevt_ = (long long int)eventTree_->GetEntries();
    TBranch* b_e_evt    = eventTree_->GetBranch("event");
    b_e_evt     ->SetAddress(&_evt);
    // trajTree
    trajTree_ = (TTree*)f.Get((subdir+"trajTree").c_str());
    nhit_ = (long long int)trajTree_->GetEntries();
    if (debug_) std::cout<<"trees1 ok"<<std::endl;
#ifndef SPLIT
    TBranch* b_t_evt     = trajTree_->GetBranch("event");
    TBranch* b_t_traj    = trajTree_->GetBranch("traj");
    TBranch* b_t_mod_on  = trajTree_->GetBranch("module_on");
    TBranch* b_t_clu     = trajTree_->GetBranch("clust");
    TBranch* b_t_clu     = trajTree_->GetBranch("clust_pix");
    TBranch* b_t_trk     = trajTree_->GetBranch("track");
    b_t_evt     ->SetAddress(&_traj_evt);
    b_t_traj    ->SetAddress(&_traj);
    b_t_mod_on  ->SetAddress(&_traj.mod_on);
    b_t_clu     ->SetAddress(&_traj.clu);
    b_t_clu     ->SetAddress(&_traj.clu.pix);
    b_t_trk     ->SetAddress(&_traj.trk);
    trajTree_   ->SetBranchStatus("event",     tg_traj_evt_);
    trajTree_   ->SetBranchStatus("traj",      tg_traj_traj_);
    trajTree_   ->SetBranchStatus("module_on", tg_traj_mod_on_);
    trajTree_   ->SetBranchStatus("clust",     tg_traj_clu_);
    trajTree_   ->SetBranchStatus("clust_pix", tg_traj_clu_);
    trajTree_   ->SetBranchStatus("track",     tg_traj_trk_);
    nclu_ = 0;
#ifdef COMPLETE
    if (debug_) std::cout<<"trees2 ok"<<std::endl;
    TBranch* b_t_mod     = trajTree_->GetBranch("module");
    b_t_mod     ->SetAddress(&_traj.mod);
    trajTree_   ->SetBranchStatus("module",    tg_traj_mod_);
    // clustTree
    clustTree_ = (TTree*)f.Get((subdir+"clustTree").c_str());
    nclu_  = (long long int)clustTree_->GetEntries();
    TBranch* b_c_evt     = clustTree_->GetBranch("event");
    TBranch* b_c_mod     = clustTree_->GetBranch("module");
    TBranch* b_c_mod_on  = clustTree_->GetBranch("module_on");
    TBranch* b_c_clu     = clustTree_->GetBranch("clust");
    TBranch* b_c_clu_pix = clustTree_->GetBranch("clust_pix");
    b_c_evt     ->SetAddress(&_clu_evt);
    b_c_mod     ->SetAddress(&_clu.mod);
    b_c_mod_on  ->SetAddress(&_clu.mod_on);
    b_c_clu     ->SetAddress(&_clu);
    b_c_clu_pix ->SetAddress(&_clu.pix);
    clustTree_  ->SetBranchStatus("event",     tg_clu_evt_);
    clustTree_  ->SetBranchStatus("module",    tg_clu_mod_);
    clustTree_  ->SetBranchStatus("module_on", tg_clu_mod_on_);
    clustTree_  ->SetBranchStatus("clust",     tg_clu_clu_);
    clustTree_  ->SetBranchStatus("clust_pix", tg_clu_clu_);
    if (debug_) std::cout<<"trees3 (COMP) ok"<<std::endl;
#endif
#else
    /************************************ Split mode - GetBranch ********************************/
    // Non-splitted branches
    TBranch* traj_event                = trajTree_->GetBranch("event");
    TBranch* traj_module_on            = trajTree_->GetBranch("module_on");
    
    // traj
    // Non-splitted branch
    TBranch* traj_traj                 = trajTree_->GetBranch("traj");
    // Paired branches
#if SPLIT > 0
    TBranch* traj_traj_occup           = trajTree_->GetBranch("traj_occup");
    TBranch* traj_traj_alphabeta       = trajTree_->GetBranch("traj_alphabeta");
    TBranch* traj_dxy_cl               = trajTree_->GetBranch("traj_dxy_cl");
    TBranch* traj_dxy_hit              = trajTree_->GetBranch("traj_dxy_hit");
#endif
    // Split-mode branches
    if (debug_) std::cout<<"split1 ok"<<std::endl;
    TBranch* traj_traj_norm_charge     = trajTree_->GetBranch("traj_norm_charge");
#ifdef COMPLETE
    TBranch* traj_traj_lz              = trajTree_->GetBranch("traj_lz");
    TBranch* traj_traj_glx             = trajTree_->GetBranch("traj_glx");
    TBranch* traj_traj_gly             = trajTree_->GetBranch("traj_gly");
    TBranch* traj_traj_glz             = trajTree_->GetBranch("traj_glz");
    TBranch* traj_traj_lxmatch         = trajTree_->GetBranch("traj_lxmatch");
    TBranch* traj_traj_lymatch         = trajTree_->GetBranch("traj_lymatch");
    TBranch* traj_traj_i               = trajTree_->GetBranch("traj_i");
    TBranch* traj_traj_onedge          = trajTree_->GetBranch("traj_onedge");
    TBranch* traj_traj_inactive        = trajTree_->GetBranch("traj_inactive");
    TBranch* traj_traj_badhit          = trajTree_->GetBranch("traj_badhit");
    //   TBranch* traj_traj_telescope       = trajTree_->GetBranch("traj_telescope");
    //   TBranch* traj_traj_telescope_valid = trajTree_->GetBranch("traj_telescope_valid");
    //   TBranch* traj_traj_dmodule         = trajTree_->GetBranch("traj_dmodule");
    //   TBranch* traj_traj_dladder         = trajTree_->GetBranch("traj_dladder");
    //   TBranch* traj_traj_glmatch         = trajTree_->GetBranch("traj_glmatch");
    //   TBranch* traj_traj_lx_err          = trajTree_->GetBranch("traj_lx_err");
    //   TBranch* traj_traj_ly_err          = trajTree_->GetBranch("traj_ly_err");
    //   TBranch* traj_traj_lz_err          = trajTree_->GetBranch("traj_lz_err");
    //   TBranch* traj_traj_lxymatch        = trajTree_->GetBranch("traj_lxymatch");
    //   TBranch* traj_traj_res_hit         = trajTree_->GetBranch("traj_res_hit");
    //   TBranch* traj_traj_sig_hit         = trajTree_->GetBranch("traj_sig_hit");
    //   TBranch* traj_traj_d_cl            = trajTree_->GetBranch("traj_d_cl");
    if (debug_) std::cout<<"split1 comp ok"<<std::endl;
#endif
    
    // clust
    // Paired branches
#if SPLIT > 1
    TBranch* traj_clust_xy             = trajTree_->GetBranch("clust_xy");
    TBranch* traj_clust_sizeXY         = trajTree_->GetBranch("clust_sizeXY");
#endif
    // Split-mode branches
#ifdef COMPLETE
    //   TBranch* traj_clust_i              = trajTree_->GetBranch("clust_i");
    TBranch* traj_clust_edge           = trajTree_->GetBranch("clust_edge");
    TBranch* traj_clust_badpix         = trajTree_->GetBranch("clust_badpix");
    TBranch* traj_clust_tworoc         = trajTree_->GetBranch("clust_tworoc");
#endif
    TBranch* traj_clust_size           = trajTree_->GetBranch("clust_size");
    TBranch* traj_clust_adc            = trajTree_->GetBranch("clust_adc");
#if SPLIT > 1
    TBranch* traj_clust_charge         = trajTree_->GetBranch("clust_charge");
    TBranch* traj_clust_pix            = trajTree_->GetBranch("clust_pix");
#endif    
    if (debug_) std::cout<<"split2 ok"<<std::endl;

    // track
    // Non-splitted branch
    TBranch* traj_track                = trajTree_->GetBranch("track");
    // Paired branches
    TBranch* traj_track_ndofchi2       = trajTree_->GetBranch("track_ndofchi2");
    // Split-mode branches
    TBranch* traj_track_eta            = trajTree_->GetBranch("track_eta");
    TBranch* traj_track_phi            = trajTree_->GetBranch("track_phi");
#ifdef COMPLETE
    TBranch* traj_track_theta          = trajTree_->GetBranch("track_theta");
    TBranch* traj_track_p              = trajTree_->GetBranch("track_p");
    TBranch* traj_track_algo           = trajTree_->GetBranch("track_algo");
    TBranch* traj_track_i              = trajTree_->GetBranch("track_i");
    TBranch* traj_track_pix            = trajTree_->GetBranch("track_pix");
    TBranch* traj_track_pixhit         = trajTree_->GetBranch("track_pixhit");
    TBranch* traj_track_validpixhit    = trajTree_->GetBranch("track_validpixhit");
    TBranch* traj_track_fpix           = trajTree_->GetBranch("track_fpix");
    TBranch* traj_track_bpix           = trajTree_->GetBranch("track_bpix");
    TBranch* traj_track_highPurity     = trajTree_->GetBranch("track_highPurity");
    //   TBranch* traj_track_fromVtx        = trajTree_->GetBranch("track_fromVtx");
#endif
    if (debug_) std::cout<<"split3 ok"<<std::endl;
    
#if CLUST_LOOP > 0
    // clustTree
    clustTree_ = (TTree*)f.Get((subdir+"clustTree").c_str());
    nclu_  = (long long int)clustTree_->GetEntries();
    // Non-splitted branches
    TBranch* clust_event               = clustTree_->GetBranch("event");
    TBranch* clust_module_on           = clustTree_->GetBranch("module_on");
    
    // clust
    // Paired branches
    TBranch* clust_clust_xy            = clustTree_->GetBranch("clust_xy");
    TBranch* clust_clust_sizeXY        = clustTree_->GetBranch("clust_sizeXY");
    // Split-mode branches
    TBranch* clust_clust_i             = clustTree_->GetBranch("clust_i");
    TBranch* clust_clust_edge          = clustTree_->GetBranch("clust_edge");
    TBranch* clust_clust_badpix        = clustTree_->GetBranch("clust_badpix");
    TBranch* clust_clust_tworoc        = clustTree_->GetBranch("clust_tworoc");
    TBranch* clust_clust_size          = clustTree_->GetBranch("clust_size");
    TBranch* clust_clust_charge        = clustTree_->GetBranch("clust_charge");
    TBranch* clust_clust_adc           = clustTree_->GetBranch("clust_adc");
    TBranch* clust_clust_pix           = clustTree_->GetBranch("clust_pix");
    if (debug_) std::cout<<"split4 clu ok"<<std::endl;
#endif
    
    /************************************ SetAddress ******************************************/
    // Non-splitted branches
    traj_event                 -> SetAddress(&_traj_evt);
    traj_module_on             -> SetAddress(&_traj.mod_on);
    
    // traj
    // Non-splitted branch
    traj_traj                  -> SetAddress(&_traj);
    // Paired branches
#if SPLIT > 0
    traj_traj_occup            -> SetAddress(&_traj.nclu_mod);
    traj_traj_alphabeta        -> SetAddress(&_traj.alpha);
    traj_dxy_cl                -> SetAddress(&_traj.dx_cl);
    traj_dxy_hit               -> SetAddress(&_traj.dx_hit);
#endif
    // Split-mode branches
    traj_traj_norm_charge     -> SetAddress(&_traj.norm_charge);
    if (debug_) std::cout<<"split5 ok"<<std::endl;
#ifdef COMPLETE
    traj_traj_lz              -> SetAddress(&_traj.lz);
    traj_traj_glx             -> SetAddress(&_traj.glx);
    traj_traj_gly             -> SetAddress(&_traj.gly);
    traj_traj_glz             -> SetAddress(&_traj.glz);
    traj_traj_lxmatch         -> SetAddress(&_traj.lxmatch);
    traj_traj_lymatch         -> SetAddress(&_traj.lymatch);
    traj_traj_i               -> SetAddress(&_traj.i);
    traj_traj_onedge          -> SetAddress(&_traj.onedge);
    traj_traj_inactive        -> SetAddress(&_traj.inactive);
    traj_traj_badhit          -> SetAddress(&_traj.badhit);
    //   traj_traj_telescope       -> SetAddress(&_traj.telescope);
    //   traj_traj_telescope_valid -> SetAddress(&_traj.telescope_valid);
    //   traj_traj_dmodule         -> SetAddress(&_traj.dmodule);
    //   traj_traj_dladder         -> SetAddress(&_traj.dladder);
    //   traj_traj_glmatch         -> SetAddress(&_traj.glmatch);
    //   traj_traj_lx_err          -> SetAddress(&_traj.lx_err);
    //   traj_traj_ly_err          -> SetAddress(&_traj.ly_err);
    //   traj_traj_lz_err          -> SetAddress(&_traj.lz_err);
    //   traj_traj_lxymatch        -> SetAddress(&_traj.lxymatch);
    //   traj_traj_res_hit         -> SetAddress(&_traj.res_hit);
    //   traj_traj_sig_hit         -> SetAddress(&_traj.sig_hit);
    //   traj_traj_d_cl            -> SetAddress(&_traj.d_cl);
#endif
    if (debug_) std::cout<<"split5 ok"<<std::endl;
    
    // clust
    // Paired branches
#if SPLIT > 1
    traj_clust_xy             -> SetAddress(&_traj.clu.x);
    traj_clust_sizeXY         -> SetAddress(&_traj.clu.sizeX);
#endif
    // Split-mode branches
#ifdef COMPLETE
    //   traj_clust_i              -> SetAddress(&_traj.clu.i);
    traj_clust_edge           -> SetAddress(&_traj.clu.edge);
    traj_clust_badpix         -> SetAddress(&_traj.clu.badpix);
    traj_clust_tworoc         -> SetAddress(&_traj.clu.tworoc);
#endif
    traj_clust_size           -> SetAddress(&_traj.clu.size);
    traj_clust_adc            -> SetAddress(&_traj.clu.adc);
#if SPLIT > 1
    traj_clust_charge         -> SetAddress(&_traj.clu.charge);
    traj_clust_pix            -> SetAddress(&_traj.clu.pix);
#endif
    
    if (debug_) std::cout<<"split6 ok"<<std::endl;
    // track
    // Non-splitted branch
    traj_track                -> SetAddress(&_traj.trk);
    // Paired branches
    traj_track_ndofchi2       -> SetAddress(&_traj.trk.ndof);
    // Split-mode branches
    traj_track_eta            -> SetAddress(&_traj.trk.eta);
    traj_track_phi            -> SetAddress(&_traj.trk.phi);
#ifdef COMPLETE
    traj_track_theta          -> SetAddress(&_traj.trk.theta);
    traj_track_p              -> SetAddress(&_traj.trk.p);
    traj_track_algo           -> SetAddress(&_traj.trk.algo);
    traj_track_i              -> SetAddress(&_traj.trk.i);
    traj_track_pix            -> SetAddress(&_traj.trk.pix);
    traj_track_pixhit         -> SetAddress(&_traj.trk.pixhit);
    traj_track_validpixhit    -> SetAddress(&_traj.trk.validpixhit);
    traj_track_fpix           -> SetAddress(&_traj.trk.fpix);
    traj_track_bpix           -> SetAddress(&_traj.trk.bpix);
    traj_track_highPurity     -> SetAddress(&_traj.trk.highPurity);
    //   traj_track_fromVtx        -> SetAddress(&_traj.trk.fromVtx);
#endif
    
    if (debug_) std::cout<<"split7 ok"<<std::endl;
#if CLUST_LOOP > 0
    // Non-splitted branches
    clust_event               -> SetAddress(&_clu_evt);
    clust_module_on           -> SetAddress(&_clu.mod_on);
    
    // clust
    // Paired branches
    clust_clust_xy            -> SetAddress(&_clu.x);
    clust_clust_sizeXY        -> SetAddress(&_clu.sizeX);
    // Split-mode branches
    clust_clust_i             -> SetAddress(&_clu.i);
    clust_clust_edge          -> SetAddress(&_clu.edge);
    clust_clust_badpix        -> SetAddress(&_clu.badpix);
    clust_clust_tworoc        -> SetAddress(&_clu.tworoc);
    clust_clust_size          -> SetAddress(&_clu.size);
    clust_clust_charge        -> SetAddress(&_clu.charge);
    clust_clust_adc           -> SetAddress(&_clu.adc);
    clust_clust_pix           -> SetAddress(&_clu.pix);
#endif
    if (debug_) std::cout<<"split8 ok"<<std::endl;

    /******************* SetBranchStatus - Uncomment to disable certain branches  *******************/
    
    /*     trajTree_->SetBranchStatus("event",0); */
    /*     trajTree_->SetBranchStatus("module_on",0); */

    /*     trajTree_->SetBranchStatus("traj",0); */
#if SPLIT > 0
    // trajTree_->SetBranchStatus("traj_occup",0);
    // trajTree_->SetBranchStatus("traj_alphabeta",0);
    // trajTree_->SetBranchStatus("traj_dxy_cl",0); 
    // trajTree_->SetBranchStatus("traj_dxy_hit",0); // DO NOT Uncomment!!! somehow screws up hit_near variable
#endif
    /*     trajTree_->SetBranchStatus("traj_norm_charge",0); */
#ifdef COMPLETE
    trajTree_->SetBranchStatus("traj_lz",0);
    trajTree_->SetBranchStatus("traj_glx",0);
    trajTree_->SetBranchStatus("traj_gly",0);
    trajTree_->SetBranchStatus("traj_glz",0);
    trajTree_->SetBranchStatus("traj_lxmatch",0);
    trajTree_->SetBranchStatus("traj_lymatch",0);
    trajTree_->SetBranchStatus("traj_i",0);
    trajTree_->SetBranchStatus("traj_onedge",0);
    trajTree_->SetBranchStatus("traj_inactive",0);
    trajTree_->SetBranchStatus("traj_badhit",0);
    /*     trajTree_->SetBranchStatus("traj_telescope",0); */
    /*     trajTree_->SetBranchStatus("traj_telescope_valid",0); */
    /*     trajTree_->SetBranchStatus("traj_dmodule",0); */
    /*     trajTree_->SetBranchStatus("traj_dladder",0); */
    /*     trajTree_->SetBranchStatus("traj_glmatch",0); */
    /*     trajTree_->SetBranchStatus("traj_lx_err",0); */
    /*     trajTree_->SetBranchStatus("traj_ly_err",0); */
    /*     trajTree_->SetBranchStatus("traj_lz_err",0); */
    /*     trajTree_->SetBranchStatus("traj_lxymatch",0); */
    /*     trajTree_->SetBranchStatus("traj_res_hit",0); */
    /*     trajTree_->SetBranchStatus("traj_sig_hit",0); */
    /*     trajTree_->SetBranchStatus("traj_d_cl",0); */
#endif
    if (debug_) std::cout<<"set1 ok"<<std::endl;
    
#if SPLIT > 1
    // trajTree_->SetBranchStatus("clust_xy",0);
    /*     trajTree_->SetBranchStatus("clust_sizeXY",0); */
#endif
#ifdef COMPLETE
    // trajTree_->SetBranchStatus("clust_i",0);
    trajTree_->SetBranchStatus("clust_edge",0);
    trajTree_->SetBranchStatus("clust_badpix",0);
    trajTree_->SetBranchStatus("clust_tworoc",0);
#endif
    /*     trajTree_->SetBranchStatus("clust_size",0); */
    /*     trajTree_->SetBranchStatus("clust_adc",0); */
#if SPLIT > 1
    //trajTree_->SetBranchStatus("clust_charge",0);
    trajTree_->SetBranchStatus("clust_pix",0);
#endif
    if (debug_) std::cout<<"set2 ok"<<std::endl;
    
    /*     trajTree_->SetBranchStatus("track",0); */
    trajTree_->SetBranchStatus("track_ndofchi2",0);
    // trajTree_->SetBranchStatus("track_eta",0);
    // trajTree_->SetBranchStatus("track_phi",0);
#ifdef COMPLETE
    trajTree_->SetBranchStatus("track_theta",0);
    trajTree_->SetBranchStatus("track_p",0);
    trajTree_->SetBranchStatus("track_algo",0);
    trajTree_->SetBranchStatus("track_i",0);
    trajTree_->SetBranchStatus("track_pix",0);
    trajTree_->SetBranchStatus("track_pixhit",0);
    trajTree_->SetBranchStatus("track_validpixhit",0);
    trajTree_->SetBranchStatus("track_fpix",0);
    trajTree_->SetBranchStatus("track_bpix",0);
    trajTree_->SetBranchStatus("track_highPurity",0);
    /*     trajTree_->SetBranchStatus("track_fromVtx",0); */
#endif
    if (debug_) std::cout<<"set3 ok"<<std::endl;
    
#if CLUST_LOOP > 0
    // clustTree_->SetBranchStatus("event",0);
    // clustTree_->SetBranchStatus("module_on",0);
    
    // // clustTree_->SetBranchStatus("clust_xy",0);
    // clustTree_->SetBranchStatus("clust_sizeXY",0);
    // clustTree_->SetBranchStatus("clust_i",0);
    // clustTree_->SetBranchStatus("clust_edge",0);
    // clustTree_->SetBranchStatus("clust_badpix",0);
    // clustTree_->SetBranchStatus("clust_tworoc",0);
    // // clustTree_->SetBranchStatus("clust_size",0);
    // //clustTree_->SetBranchStatus("clust_charge",0);
    // // clustTree_->SetBranchStatus("clust_adc",0);
    // clustTree_->SetBranchStatus("clust_pix",0);
#endif
    if (debug_) std::cout<<"set4 ok"<<std::endl;
#endif
    debug_=0;
    severity=0;
  }
  
  void recover_missing_cluster_() {
#ifndef SPLIT
    if (traj_.missing==1&&sqrt(traj_.dx_cl[0]*traj_.dx_cl[0]+traj_.dy_cl[0]*traj_.dy_cl[0])<0.05) {
      traj_.missing=0;
      traj_.validhit=1;
    }
#else
#if VERSION1 > 28
    if (traj_->missing&&traj_->clust_near) { traj_->missing=0; traj_->validhit=1; }
#else
    if (traj_->missing==1&&sqrt(traj_->dx_cl[0]*traj_->dx_cl[0]+traj_->dy_cl[0]*traj_->dy_cl[0])<0.05) {
      traj_->missing=0;
      traj_->validhit=1;
    }
#endif 
#endif
  }
  
 public:
  // Problems
  int severity;

  //Accessors
  // Entries
  long long int nrun() { return nrun_; }
  long long int nls()  { return nls_; }
  long long int nevt() { return nevt_; }
  long long int nhit() { return nhit_; }
  long long int nclu() { return nclu_; }
  // Read Entries
  void run_read(long long int i)  { runTree_->GetEntry(i); }
  void lumi_read(long long int i) { lumiTree_->GetEntry(i); }
  void evt_read(long long int i)  { eventTree_->GetEntry(i); }
  Int_t traj_read(long long int i) { Int_t read = trajTree_->GetEntry(i); recover_missing_cluster_(); return read; }
#if CLUST_LOOP > 0
  Int_t clu_read(long long int i) { return clustTree_->GetEntry(i); }
#endif
  // Memory adress of entry
  const RunData&         run()      { return *run_; }
  const LumiData&        lumi()     { return *lumi_; }
  const EventData&       evt()      { return *evt_; }
  const EventData&       traj_evt() { return *traj_evt_; }
  const TrajMeasurement& traj()     { return *traj_; }
#if CLUST_LOOP > 0
  const EventData&       clu_evt()  { return *clu_evt_; }
  const Cluster&         clu()      { return *clu_; }
#endif
  // Modifiers
#if CLUST_LOOP > 0
  void readtrees(TFile &f, std::string subdir="") { read_(f, *run_, *lumi_, *evt_, *traj_evt_, *traj_, *clu_evt_, *clu_, subdir); }
#else
  void readtrees(TFile &f, std::string subdir="") { read_(f, *run_, *lumi_, *evt_, *traj_evt_, *traj_, subdir); }
#endif
  
};