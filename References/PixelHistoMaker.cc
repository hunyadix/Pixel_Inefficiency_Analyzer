/*
  Author:
    Janos Karancsi
  E-mail:
    janos.karancsi@cern.ch, karancsij@atomki.mta.hu
  
  Description:
    The program is used to creat histograms
    from the TimingStudy output files
  
  Version Info:
  VER   TimingStudy Ntuple version              Date created
 -2 :   v2928 SPLIT 0                           (used for 2010/2011 Data)
 -1 :   v3029 SPLIT 0                           (used for 2010/2011 Data)
  0 :   v3431 SPLIT 0                           (2012                - by Alberto/Janos/Joco/Silvia)
  1 :   v3533 SPLIT 0                           (2013 Apr24          - by Viktor)
  2 :   v3533 SPLIT 1 + alpha/beta              (2013 Apr28-May3     - by Viktor)
  3 :   v3734 SPLIT 1 + occupancy               (2013 May 13)
  4 :   v3735 SPLIT 1 + pileup reweighting      (2014 Feb 04)
  5 :   v3836 SPLIT 1 + resolution              (2014 Jun 16)
  
  Note:
    From VER 3 and up, you can use this new PixelHistoMaker version
    for the previous Ntuple versions, use the old file (PixelHistoMaker_old.cc)
*/

// Set on which trees you want to run on
// For now, only select 1, set others to 0
#define EVT_LOOP 1
#define TRAJ_LOOP 1
#define CLUST_LOOP 0
// Set this if you only want to run on each Nth file
// This is used to do quick tests of binning on low statistics
#define NTHFILE 10

// TimingStudy versions
#define VER 5
#if VER == -2 // v2928 backported
#define SPLIT 0
#define TREEREADER_VER 29
#define DATASTRUCT_VER 28
#elif VER == -1 // v3029 backported
#define SPLIT 0
#define TREEREADER_VER 30
#define DATASTRUCT_VER 29
#elif VER == 3
#define SPLIT 1
#define TREEREADER_VER 37
#define DATASTRUCT_VER 34
#elif VER == 4
#define SPLIT 1
#define TREEREADER_VER 37
#define DATASTRUCT_VER 35
#elif VER == 5
#define SPLIT 0
#define TREEREADER_VER 38
#define DATASTRUCT_VER 36
#endif

#define MAX_PU 100
#define PU_REWEIGHT_MC 1

//#define COMPLETE 0

#include "interface/TreeLooper.h"
#include "modifications.h"
#include "lowlumi_runs_2011.txt"
#include "run_203002.txt"

int main(int argc, char* argv[]) {
  // Get arguments from shell
  std::vector<std::string> filelist;
  std::string outputfile="";
  size_t opt_n = 0;
  // -n <number> option:
  // Set so the postfix that uses the v.pf_file_add
  // get overwritten by <number>-1
  // default is opt_n=0 - use with files added by editing this file
  // -o <output file> option:
  // Specify the output root filename
  // Rest of the arguments are treated as files added
  // Note:
  // If using postfixes with the v.pf_fila_add variable
  // each added file will increase this variable so when using *
  // add ""-s so instead of the shell TChain will parse the argument
  bool is_o = false, is_n = false, is_a = false;
  for(int i=1; i<argc; i++) {
    std::string arg = argv[i];
    // Check existence of an option
    if (arg[0]=='-'&&arg.size()==2) { 
      is_n = (arg[1]=='n'); 
      is_o = (arg[1]=='o'); 
      if (arg[1]=='a') is_a = 1;
    }
    // Check second argument after option
    else if (is_n) { std::stringstream ss; ss<<arg; ss>>opt_n; is_n=0; } 
    else if (is_o) { outputfile=arg; is_o=0; }
    else filelist.push_back(arg);
  }
  
  // Data structs holding variables read from TTrees
  RunData run; LumiData lumi; EventData e; TrajMeasurement t; Cluster c;
  
  // Variables that are derived from Data struct members
  Variables v;
  
  // TTree reader
  TreeReader tr(&run,&lumi,&e,&e,&t,&e,&c);
  
  // Histogram storage class
  SmartHistos sh;
  if (EVT_LOOP) sh.AddHistoType("evt");
  if (TRAJ_LOOP) sh.AddHistoType("traj");
  if (CLUST_LOOP) sh.AddHistoType("clust");

  // Colors
  // 400 kYellow  800 kOrange
  // 416 kGreen   820 kSpring
  // 432 kCyan    840 kTeal
  // 600 kBlue    860 kAzure
  // 616 kMagenta 880 kViolet
  // 632 kRed     900 kPink
  
  std::string col3_red_to_blue = "633,618,601,"; // red, purple, blue
  std::string col4_cyan_to_red = "434,601,618,633,"; // Cyan, blue, purple, red
  std::string col5_green_to_red = "418,434,601,618,633,"; // green, cyan, blue, purple, red
  std::string col5_red_to_green = "633,618,601,434,418,"; // red, , purple, blue, cyan, green
  std::string col6_rainbow_dark = "601,434,418,402,633,618,"; // blue, cyan, green, yellow, red, purple
  std::string col8 = "1,601,434,418,402,807,633,618,"; // above plus black and orange
  std::string col12 = "1,4,6,2,800,402,417,433,9,618,633,924,"; // black, blue, magenta, red, orange, darker yellow, darker green, darker cyan, blue-purple, dark purple, dark red
  std::string col12_rainbow = "402,416,433,600,617,632,802,813,833,863,883,892,"; // Go around first bright and then dark colors
  
  // Tree Looper class, that uses the TreeReader 
  // and Variables classes to obtain values to Data containers
  TreeLooper looper(&tr, &v, EVT_LOOP, TRAJ_LOOP, CLUST_LOOP); // Set these in the beginning of this file
  // Define Postfixes here:
  sh.AddNewPostfix("Layers",            [&v](){ return v.pf_layers;       }, "Lay[1to3]", "L[1to3]", col3_red_to_blue);
  sh.AddNewPostfix("Rings",             [&v](){ return v.pf_rings;        }, "Mod[1to4]", "Mod[1to4]", col4_cyan_to_red);
  sh.AddNewPostfix("Ring12/34",   [&v](){ return v.pf_rings>=2;     }, "Mod12;Mod34", "Mod 1,2;Mod 3,4", "633,601");
  sh.AddNewPostfix("Ladders",           [&v](){ return v.pf_ladders;      }, "Lad[1to22]", "Ladder [1to22]", col12+col12_rainbow);
  sh.AddNewPostfix("DisksInOut",        [&v](){ return v.pf_disksio;      }, "Disk1In;Disk1Out;Disk2In;Disk2Out", "Disk 1 - Inner 3 Plq;Disk 1 - Outer 4 Plq;Disk 2 - Inner 3 Plq;Disk 2 - Outer 4 Plq", col4_cyan_to_red);
  sh.AddNewPostfix("LayersDisksInOut",  [&v](){ return v.pf_lays_disksio; }, "Lay[1to3];Disk1In;Disk1Out;Disk2In;Disk2Out", "Layer [1to3];Disk 1 - Inner 3 Plq;Disk 1 - Outer 4 Plq;Disk 2 - Inner 3 Plq;Disk 2 - Outer 4 Plq", col8);
  sh.AddNewPostfix("LayersDisks",       [&v](){ return v.pf_lays_disks;   }, "Lay[1to3];Disk[1to2]", "Layer [1to3];Disk [1to2]", col5_red_to_green);
  sh.AddNewPostfix("HVGroupsScanned",   [&v](){ return v.pf_lays_disks;   }, "L[1to3]OneHVGrp;D[1to2]OneHVGrp", "Layer [1to3] (One HV group);Disk [1to2] (One HV group)", col5_red_to_green);
  sh.AddNewPostfix("Pileup4",           [&v](){ return (size_t)v.pileup>47? -1 : (size_t)(v.pileup/4); }, "pu[0to44++4]to[4to48++4]", "PU [[0to44++4],[4to48++4]]", col12_rainbow);
  sh.AddNewPostfix("Pileup2",           [&v](){ return (size_t)v.pileup>47? -1 : (size_t)(v.pileup/2); }, "pu[0to46++2]to[2to48++2]", "PU [[0to46++2],[2to48++2]]", col12_rainbow+col12_rainbow);
  sh.AddNewPostfix("InstLumi",          [&v](){ return v.pf_instlumi;     }, "[0to10]invnbHz", "[0to10] nb^{-1}s^{-1}", col12_rainbow);
  sh.AddNewPostfix("L1Rate",            [&v](){ return v.pf_l1rate;       }, "[0to100++10]kHz", "[0to100++10]kHz", col12_rainbow);
  
  sh.AddNewPostfix("NBx",          [&v](){ return (size_t)(v.nbx==NOVAL_F||v.nbx>=2500) ? -1 : v.nbx/50; }, "NBx[25to2475++50]", "[25to2475++50]#pm25b", col12+col12_rainbow+col12+col12_rainbow);
  sh.AddNewPostfix("LowNBx",       [&v](){ return (size_t)(v.nbx==NOVAL_F||v.nbx>=350) ? -1 : v.nbx/10;      }, "NBx[5to345++10]",   "[5to345++10]#pm5b", col12+col12_rainbow+col12+col12_rainbow);
  sh.AddNewPostfix("SelectedNBx",          [&v,&e]()
       { 
         size_t result=-1;
         if (v.main_filling_schemes_2015) result=0;
         else if (fabs(v.nbx-1813)<100) result=1;
         else if (fabs(v.nbx-1453)<70) result=2;
         else if (fabs(v.nbx-1021)<70) result=3;
         else if (e.run==259721) result=4;
         return result;
       }, "2232b;1813b;1453b;1021b;517b", "2232b;1813b;1453b;1021b;517b", "633,618,601,434,1");
  
  sh.AddNewPostfix("WBC",               [&v](){ return v.pf_wbc;          }, "WBC0;WBC12;WBC13;WBC12HV200", "WBC 0;WBC +12;WBC +13;WBC +12 - HV_{BPix} = 200 V", col4_cyan_to_red);
  sh.AddNewPostfix("DelayScanRuns",     [&e](){ return (e.run>=246919) + (e.run>=246920) + (e.run>=246923) + (e.run>=246926) + (e.run>=246930) + (e.run>=246936); }, 
       "246908;246919;246920;246923;246926;246930;246936", "246908;246919;246920;246923;246926;246930;246936", col8);
  sh.AddNewPostfix("BX>500",            [&e](){ return e.bx>500;          }, "Bx0to500;Bx500to3600",   "BX<500;BX>500", "601,633");
  
  sh.AddNewPostfix("FedErrorEvent",     [&e](){ return e.federrs_size>0; }, "NoFedErrorInEvt;FedErrorInEvt",   "No FED error in event;Fed error in event",   "418,633");
  sh.AddNewPostfix("FedErrorModule",    [&v](){ return v.federr_mod;     }, "NoFedError;Timeout;Overflow",   "No FED error on module;Timeout;Overflow", "418,633,807,");
  sh.AddNewPostfix("First4BX",          [&e,&v]()
       { return
           (e.bx==v.first_filled_bx-2) ? 0 :
           (e.bx==v.first_filled_bx-1) ? 1 :
           (e.bx==v.first_filled_bx)   ? 2 :
           (e.bx==v.first_filled_bx+1) ? 3 :
           (size_t)-1; 
       }, "BXm2;BXm1;BX0;BXp1", "First BX -2;First BX -1;First BX;First BX +1", "1,601,633,802");
  
  // Same category, but used widely to compare plots
  sh.AddNewPostfix("DcolScan",      [&v](){ 
         return v.pf_file_add;
       }, "Dcol[80,90,95,97,98,99,99.3,99.6,99.9,100]", "MC Dcol Eff = [80,90,95,97,98,99,99.3,99.6,99.9,100]%", "1-11");
  
  sh.AddNewPostfix("Years",         [&v](){ return v.pf_year;     },     "2011;2012;2015", "2011 Data @ 7 TeV;2012 Data @ 8 TeV;", "4,2,1");
  //sh.AddNewPostfix("Validation",    [&v](){ return v.pf_file_add; }, "Data;MC_Old;MC;MC_New", "Data;MC - No Dyn. Ineff.;MC - Current Version;MC - New Version", "1,2,4,3");
  //sh.AddNewPostfix("Validation",    [&v](){ return v.pf_file_add; }, "Data;MC_Old;MC", "Data;MC - No Dyn. Ineff.;MC - Current Version", "1,2,4");
  //sh.AddNewPostfix("Validation",    [&v](){ return v.pf_file_add; }, "Data;MC_New", "Data;MC - with FPix DynIneff", "1,2,4,3");
  //sh.AddNewPostfix("Validation",    [&v](){ return v.pf_file_add; }, "MC;MC_New", "MC - Current Version;MC - New Version", "4,3");
  sh.AddNewPostfix("Validation",    [&v](){ return v.pf_file_add; }, "Data;MC;MC_New", "Data;MC - No DynIneff;MC - with BPix+FPix DynIneff", "1,2,4,3");
  sh.AddNewPostfix("8and13TeV",     [&v](){ return v.pf_file_add; }, "Data_8TeV;MC_8TeV;MC_13TeV_50ns;MC_13TeV_50ns_PSI;MC_13TeV_25ns", "Data 8TeV;MC 8TeV;MC 13TeV (50ns);MC 13TeV (50ns) - PSI Sim.;MC 13TeV (25ns)", "1,2,4,618,3");
  sh.AddNewPostfix("Data",         [&e](){ if (e.run!=1) return (size_t)0; else return (size_t)-1; }, "Data", "", "1");
  //sh.AddNewPostfix("DatavsMC2015", [&v](){ return v.pf_file_add; }, "Data_50ns;MC_50ns;Data_25ns;MC_25ns", "Data - 50ns;MC - 50ns;Data - 25ns;MC - 25ns", "1,632,1,601");
  sh.AddNewPostfix("DatavsMC", [&v](){ return !v.is_data; }, "Data;MC", "Data;MC", "1,633");
  sh.AddNewPostfix("BunchSpacing", [&v](){ return v.pf_file_add>=2; }, "50ns;25ns", "50ns;25ns", "633,601");
  sh.AddNewPostfix("Special", [&v](){ return 0; }, "Data_8TeV;MC_8TeV;Data_50ns_50ns;MC_50ns_50ns;data;MC_25ns", "8 TeV Data - 50ns;8 TeV MC - 50ns;13 TeV Data - 50ns;13 TeV MC - 50ns;13 TeV Data - 25ns;13 TeV MC - 25ns", "1,14,602,433,633,910");
  sh.AddNewPostfix("data",         [](){ return 0; }, "data", "", "1");
  sh.AddNewPostfix("2016Pred", [&v](){ return v.pf_file_add; }, "Data_8TeV;Data;MC;MCPred", "Data - 2012;Data - 2015;MC - 2015 (1655b - average);MC - 2016 (2700b)", "1,418,601,633");
  sh.AddNewPostfix("DataMCComp",      [&v](){ return v.pf_file_add; }, "Data2015;MC;MC2015;MC2016", "Data - 2232 bunches;MC - 2015 Prediction;MC - 2015 Tune;MC - 2016 Prediction", "1,633,418,601");
  //sh.AddNewPostfix("8and13TeV",     [&v](){ return v.pf_file_add; }, "MC_13TeV_25ns", "MC 13TeV (25ns)", "3");
  //const char* main = "8and13TeV";
  //const char* main1 = "DatavsMC";
  //const char* main2 = "BunchSpacing";
  const char* main12 = "DataMCComp";
  
  // Define histo parameters and filling variable
  // X/Y/Z - axis parameters:
  sh.AddNewFillParam("NVertices",        { .nbin=MAX_PU, .bins={    0, MAX_PU}, .fill=[&e](){ return e.nvtx; }, .axis_title="N_{Vertices}"});
  sh.AddNewFillParam("NVerticesRwg",     { .nbin=MAX_PU, .bins={    0, MAX_PU}, .fill=[&e](){ return e.nvtx; }, .axis_title="Reweighted N_{Vertices}"});
  sh.AddNewFillParam("NPileup",          { .nbin=MAX_PU, .bins={    0, MAX_PU}, .fill=[&v](){ return v.pileup; }, .axis_title="N_{Pile-up}"});
  sh.AddNewFillParam("NPileupRwg",       { .nbin=MAX_PU, .bins={    0, MAX_PU}, .fill=[&v](){ return v.pileup; }, .axis_title="N_{Pile-up}"});
  sh.AddNewFillParam("NTracks",          { .nbin=100,  .bins={      0,   1000}, .fill=[&e](){ return e.ntracks;         }, .axis_title="N_{Tracks}"});
  sh.AddNewFillParam("NTracksLay1",      { .nbin=100,  .bins={      0,   1000}, .fill=[&e](){ return e.ntrackBPix[0];   }, .axis_title="N_{Tracks crossing Layer 1}"});
  sh.AddNewFillParam("NTracksDsk1",      { .nbin=100,  .bins={      0,   1000}, .fill=[&e](){ return e.ntrackFPix[0];   }, .axis_title="N_{Tracks crossing Disk 1}"});
#if DATASTRUCT_VER > 28
  sh.AddNewFillParam("NClu",             { .nbin= 80,  .bins={      0,   8000}, .fill=[&v](){ return v.nclu;            }, .axis_title="N_{Clusters}"});
  sh.AddNewFillParam("NPix",             { .nbin=125,  .bins={      0,  25000}, .fill=[&v](){ return v.npix;            }, .axis_title="N_{Pixels}"});
  sh.AddNewFillParam("NCluBPix",         { .nbin=100,  .bins={      0,   5000}, .fill=[&v](){ return v.nclu_bpix;       }, .axis_title="N_{Clusters} - BPix"});
  sh.AddNewFillParam("NPixBPix",         { .nbin=100,  .bins={      0,  20000}, .fill=[&v](){ return v.npix_bpix;       }, .axis_title="N_{Pixels} - BPix"});
  sh.AddNewFillParam("NCluFPix",         { .nbin=100,  .bins={      0,   2000}, .fill=[&e](){ return e.nclu[0];         }, .axis_title="N_{Clusters} - FPix"});
  sh.AddNewFillParam("NPixFPix",         { .nbin=100,  .bins={      0,   5000}, .fill=[&e](){ return e.npix[0];         }, .axis_title="N_{Pixels} - FPix"});
#endif
  sh.AddNewFillParam("LayersDisks",      { .nbin=   7, .bins={    0.5,    7.5}, .fill=[&v](){ return v.layers_disks;    }, .axis_title=""});
  sh.AddNewFillParam("LayersDisksInOut", { .nbin=   7, .bins={    0.5,    7.5}, .fill=[&v](){ return v.layers_disks;    }, .axis_title=""});
  sh.AddNewFillParam("DisksInOut",       { .nbin=   4, .bins={    0.5,    4.5}, .fill=[&v](){ return v.disks_inout;     }, .axis_title=""});
  sh.AddNewFillParam("Ladders",          { .nbin=  45, .bins={  -22.5,   22.5}, .fill=[&v](){ return v.ladder;          }, .axis_title="Ladders"});
  sh.AddNewFillParam("Modules",          { .nbin=   9, .bins={   -4.5,    4.5}, .fill=[&v](){ return v.module;          }, .axis_title="Modules"});
  sh.AddNewFillParam("Pileup",           { .nbin=  50, .bins={      0,    100}, .fill=[&v](){ return v.pileup;          }, .axis_title="Avg. Pile-up"});
  //sh.AddNewFillParam("InstLumi",         { .nbin=  80, .bins={      0,     20}, .fill=[&v](){ return v.instlumi;        }, .axis_title="Instantaneous luminosity (nb^{-1}s^{-1})"});
  sh.AddNewFillParam("InstLumi",         { .nbin= 100, .bins={      0,     20}, .fill=[&v](){ return v.instlumi;        }, .axis_title="Inst. Luminosity (#times10^{33}cm^{-2}s^{-1})"});
  sh.AddNewFillParam("InstLumi0p5",      { .nbin=  40, .bins={      0,     20}, .fill=[&v](){ return v.instlumi;        }, .axis_title="Inst. Luminosity (#times10^{33}cm^{-2}s^{-1})"});
  sh.AddNewFillParam("InstLumi1p0",      { .nbin=  21, .bins={   -0.5,   20.5}, .fill=[&v](){ return v.instlumi;        }, .axis_title="Inst. Luminosity (#times10^{33}cm^{-2}s^{-1})"});
  sh.AddNewFillParam("IntLumi",          { .nbin= 500, .bins={      0,    100}, .fill=[&v](){ return v.intlumi;         }, .axis_title="Integrated Luminosity (fb^{-1})"});
  sh.AddNewFillParam("IntLumiRunII",     { .nbin= 500, .bins={      0,    100}, .fill=[&v](){ return v.intlumi-27.6414; }, .axis_title="Integrated Luminosity - Run II (fb^{-1})"});
  sh.AddNewFillParam("NBx",              { .nbin= 100, .bins={      0,   2500}, .fill=[&v](){ return v.nbx;             }, .axis_title="N_{Bunches, CMS}"});
  sh.AddNewFillParam("L1Rate",           { .nbin=  13, .bins={     -5,    125}, .fill=[&e](){ return e.l1_rate/1000.;   }, .axis_title="L1-trigger rate (kHz)"});
  sh.AddNewFillParam("OnCluSize",        { .nbin=  26, .bins={   -0.5,   25.5}, .fill=[&t](){ return t.clu.size;        }, .axis_title="On-Trk Cluster Size (pixel)"});
  sh.AddNewFillParam("OnCluCharge",      { .nbin= 250, .bins={      0,    500}, .fill=[&v](){ return v.clu_charge;      }, .axis_title="On-Trk Cluster Charge (ke)"});
  sh.AddNewFillParam("OnCluChargeNorm",  { .nbin= 200, .bins={      0,    200}, .fill=[&t](){ return t.norm_charge;     }, .axis_title="Norm. On-Trk Clu. Charge (ke)"});
  sh.AddNewFillParam("CluSize",          { .nbin=  26, .bins={   -0.5,   25.5}, .fill=[&c](){ return c.size;            }, .axis_title="Cluster Size (pixel)"});
  sh.AddNewFillParam("CluCharge",        { .nbin=  25, .bins={      0,    500}, .fill=[&c](){ return c.charge;          }, .axis_title="Cluster Charge (ke)"});
  sh.AddNewFillParam("MCDColEff",        { .nbin=1001, .bins={-0.0005, 1.0005}, .fill=[&v](){ return v.dcol_eff;        }, .axis_title="DCol Efficiency"});
  sh.AddNewFillParam("BunchCrossing12",  { .nbin= 300, .bins={      0,   3600}, .fill=[&e](){ return e.bx;              }, .axis_title="Bunch-crossing"});
  sh.AddNewFillParam("BunchCrossing60",  { .nbin=  60, .bins={      0,   3600}, .fill=[&e](){ return e.bx;              }, .axis_title="Bunch-crossing"});
  sh.AddNewFillParam("NBxPerTrig",       { .nbin=  80, .bins={      0,   2000}, .fill=[&v](){ return v.nbx_per_trig;    }, .axis_title="Avg. N_{Bunch-crossing} / Trigger latency"});
  sh.AddNewFillParam("NColPerTrig",      { .nbin= 200, .bins={      0, 100000}, .fill=[&v](){ return v.ncol_per_trig;   }, .axis_title="Avg. N_{Collisions} / Trigger latency"});
  sh.AddNewFillParam("Runs",             { .nbin= 170, .bins={ 246000, 263000}, .fill=[&e](){ return e.run;             }, .axis_title="Runs"});
  sh.AddNewFillParam("TMuon",            { .nbin=  10, .bins={    -40,     60}, .fill=[&e](){ return e.tmuon;           }, .axis_title="t_{muon} @ Int. Point (ns)"});
  sh.AddNewFillParam("ROCLadders",       { .nbin=  88, .bins={    -22,     22}, .fill=[&v](){ return v.roc_ladder;      }, .axis_title="Ladders"});
  sh.AddNewFillParam("ROCModules",       { .nbin=  72, .bins={   -4.5,    4.5}, .fill=[&v](){ return v.roc_module;      }, .axis_title="Modules"});
  sh.AddNewFillParam("ROCBladesInner",   { .nbin=  72, .bins={    0.5,   12.5}, .fill=[&v](){ return v.roc_blade;       }, .axis_title="Blades (+X)"});
  sh.AddNewFillParam("ROCBladesOuter",   { .nbin=  72, .bins={  -12.5,   -0.5}, .fill=[&v](){ return v.roc_blade;       }, .axis_title="Blades (-X)"});
  sh.AddNewFillParam("ROCSideDiskPanel", { .nbin=  72, .bins={   -4.5,    4.5}, .fill=[&v](){ return v.roc_sdp;         }, .axis_title=""});
  sh.AddNewFillParam("TrkPt",            { .nbin=   9, .bins={0, 0.2, 0.4, 0.6, 0.8, 1, 1.5, 2, 5, 20}, .fill=[&t](){ return t.trk.pt; }, .axis_title="Track p_{T} (GeV/c)"});
  sh.AddNewFillParam("TrkNStrip",        { .nbin=  18, .bins={0, 2, 4, 6, 8, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 25, 35, 50}, .fill=[&t](){ return t.trk.strip; }, .axis_title="N_{strip hits}"});
  sh.AddNewFillParam("TrkD0",            { .nbin=  11, .bins={0, 0.001, 0.002, 0.005, 0.01, 0.02, 0.05, 0.1, 0.2, 0.5, 2, 20}, .fill=[&t](){ return fabs(t.trk.d0); }, .axis_title="Track Impact Parameter #Delta0 (cm)"});
  sh.AddNewFillParam("TrkDZ",            { .nbin=  10, .bins={0, 0.002, 0.005, 0.01, 0.02, 0.05, 0.1, 0.2, 0.5, 2, 20}, .fill=[&t](){ return fabs(t.trk.dz); }, .axis_title="Track Impact Parameter #DeltaZ (cm)"});
  sh.AddNewFillParam("NTimeoutEvt",      { .nbin=  51, .bins={   -0.5,   50.5}, .fill=[&v](){ return v.n_timeout;       }, .axis_title="N_{Time-out}/Event"});
  sh.AddNewFillParam("NOverflowEvt",     { .nbin=  51, .bins={   -0.5,   50.5}, .fill=[&v](){ return v.n_overflow;      }, .axis_title="N_{Overflow}/Event"});
  sh.AddNewFillParam("TimeoutMod",       { .nbin=   2, .bins={   -0.5,    1.5}, .fill=[&v](){ return v.federr_mod==1;   }, .axis_title="Time-out/Module"});
  sh.AddNewFillParam("OverflowMod",      { .nbin=   2, .bins={   -0.5,    1.5}, .fill=[&v](){ return v.federr_mod==2;   }, .axis_title="Overflow/Module"});
  
  // Special Y/Z axis parameters:
  sh.AddSpecial({ .name="DColEfficiency", .name_plus_1d="Validhit", .axis="DCol Efficiency", .axis_plus_1d="Valid Hit"});
  sh.AddSpecial({ .name="HitEfficiency",  .name_plus_1d="ValidHit", .axis="Hit Efficiency",  .axis_plus_1d="Valid Hit"});
  sh.AddNewFillParam("DColEfficiency",   { .nbin=   2, .bins={   -0.5,    1.5}, .fill=[&t](){ return t.validhit;        }, .axis_title="DCol Efficiency" });  
  sh.AddNewFillParam("HitEfficiency",    { .nbin=   2, .bins={   -0.5,    1.5}, .fill=[&t](){ return t.validhit;        }, .axis_title="Hit Efficiency" });

  // Define Cuts here:
  sh.AddNewCut("Nvtx",            [&v](){ return v.cut_nvtx;             });
  sh.AddNewCut("ZeroBias",        [&v](){ return v.cut_zb;               });
  sh.AddNewCut("Random",          [&e](){ return (e.trig>>2)&1;          });
  sh.AddNewCut("EffCuts",         [&v](){ return v.effcut_all;           });
  sh.AddNewCut("EffCutsNoNStrip", [&v](){ return v.effcut_nstrip;        });
  sh.AddNewCut("EffCutsNoPt",     [&v](){ return v.effcut_pt;            });
  sh.AddNewCut("EffCutsNoD0",     [&v](){ return v.effcut_d0;            });
  sh.AddNewCut("EffCutsNoDZ",     [&v](){ return v.effcut_dz;            });
  sh.AddNewCut("EffCutsNoFederr", [&v](){ return v.effcut_federr;        });
  sh.AddNewCut("BPix",            [&v](){ return v.is_bpix;              });
  sh.AddNewCut("FPix",            [&v](){ return v.is_fpix;              });
  sh.AddNewCut("FPixInner",       [&v](){ return v.is_fpix_inner;        });
  sh.AddNewCut("FPixOuter",       [&v](){ return v.is_fpix_outer;        });
  sh.AddNewCut("Lay1",            [&v](){ return v.is_l1;                });
  sh.AddNewCut("Data",            [&v](){ return v.is_data;              });
  sh.AddNewCut("MC",              [&v](){ return !v.is_data;             });
  sh.AddNewCut("Run203002",       [&v](){ return v.is_run203002;         });
  sh.AddNewCut("NStrip>10",       [&v](){ return v.nstrip;               });
  sh.AddNewCut("Pt>1GeV",         [&v](){ return v.pt;                   });
  sh.AddNewCut("MainFills",       [&v](){ return v.main_filling_schemes; });
  sh.AddNewCut("HighPU",          [&v](){ return v.cut_highpu;           });
  sh.AddNewCut("GlobalMuon",      [&e](){ return e.tmuon!=-9999;         });
  sh.AddNewCut("Lay3",            [&v](){ return v.pf_layers==2;         });
  sh.AddNewCut("ValidHit",        [&t](){ return t.clu.size>0;           });
  sh.AddNewCut("HasLay2ValidHit", [&t](){ return t.trk.validbpix[1]>0;   });
  sh.AddNewCut("ExclRing34",      [&t](){ if (t.mod_on.det==1) return true; else return (abs(t.mod_on.module)<3); });
  
  sh.AddNewCut("Run247992&&LS>182",   [&e](){ return e.run==247992&&e.ls>182; });
  
  // --------------------------------------------------------------------------
  //                           Vertex Reweighting
  
  sh.SetHistoWeights({});
  sh.AddHistos("evt", { .fill="NVertices",   .pfs={main12}, .cuts={"ZeroBias","Nvtx"}, .draw="", .opt="Norm", .ranges={} });
  sh.AddHistos("evt", { .fill="NPileup",     .pfs={main12}, .cuts={"ZeroBias","Nvtx"}, .draw="", .opt="Norm", .ranges={} });
  
#if PU_REWEIGHT_MC == 1
  bool norm = 0; // Normalize also to the number of events in Data
  TFile *f_data = TFile::Open("input/pileup_data_2232b_69mb.root");
  //TFile *f_data = TFile::Open("input/pileup_data_2232b_80mb.root");
  TH1D* Data = (TH1D*)f_data->Get("pileup");
  TFile *f_mc =TFile::Open("input/pileup_mc15pred_mc15tune_mc16pred.root");
  std::vector<TH1D*> MCs;
  MCs.push_back((TH1D*)f_mc->Get("mc15pred_pileup"));
  MCs.push_back((TH1D*)f_mc->Get("mc15tune_pileup"));
  MCs.push_back((TH1D*)f_mc->Get("mc16pred_pileup"));
  std::vector<std::vector<double> > pu_weights;
  for (TH1D* MC : MCs) {
    if (Data!=0&&MC!=0) {
      if (!norm) {
  if (Data->Integral()>0) Data->Scale(1/Data->Integral());
  if (MC->Integral()>0) MC->Scale(1/MC->Integral());
      }
      pu_weights.push_back(std::vector<double>());
      //std::cout<<"Pileup weights:\n";
      for (int npu=0; npu<=MAX_PU; ++npu) {
  pu_weights[pu_weights.size()-1].push_back(MC->GetBinContent(npu+1)>0 ? Data->GetBinContent(npu+1) / MC->GetBinContent(npu+1) : 0);
  //std::cout<<"Npu="<<npu<<" weight="<<pu_weights[pu_weights.size()-1][npu]<<std::endl;
      }
      
    }
  }
  f_mc->Close();
  f_data->Close();
  // Set weights for all Histos, except Background estimate, where MC is scaled to MC_Expected_IntLumi_invfb
  sh.SetHistoWeights({[&v,&e,&pu_weights](){
       if (v.is_data==1) return (double)1;
       if (v.pileup>MAX_PU) return (double)0;
       else return pu_weights[v.pf_file_add-1][(int)v.pileup]; // Implies 0: Data, 1-3: MC
           }});
#endif
  
  // ---------------------------------------------------------------------------------
  //                             Histogram Definitions
  //__________________________________________________________________________________
  //
  //                                  Event Plots

#if PU_REWEIGHT_MC == 1
  sh.AddHistos("evt", { .fill="NVerticesRwg", .pfs={main12}, .cuts={"ZeroBias","Nvtx"}, .draw="", .opt="Norm", .ranges={} });
  sh.AddHistos("evt", { .fill="NPileupRwg",   .pfs={main12}, .cuts={"ZeroBias","Nvtx"}, .draw="", .opt="Norm", .ranges={} });
  sh.AddHistos("evt", { .fill="NTracks",      .pfs={main12}, .cuts={"ZeroBias","Nvtx"}, .draw="", .opt="Norm", .ranges={} });
  sh.AddHistos("evt", { .fill="NTracksLay1",  .pfs={main12}, .cuts={"ZeroBias","Nvtx"}, .draw="", .opt="Norm", .ranges={} });
  sh.AddHistos("evt", { .fill="NTracksDsk1",  .pfs={main12}, .cuts={"ZeroBias","Nvtx"}, .draw="", .opt="Norm", .ranges={} });
#if DATASTRUCT_VER > 28
  sh.AddHistos("evt", { .fill="NClu",         .pfs={main12}, .cuts={"ZeroBias","Nvtx"}, .draw="", .opt="Norm", .ranges={} });
  sh.AddHistos("evt", { .fill="NPix",         .pfs={main12}, .cuts={"ZeroBias","Nvtx"}, .draw="", .opt="Norm", .ranges={} });
  sh.AddHistos("evt", { .fill="NCluBPix",     .pfs={main12}, .cuts={"ZeroBias","Nvtx"}, .draw="", .opt="Norm", .ranges={} });
  sh.AddHistos("evt", { .fill="NPixBPix",     .pfs={main12}, .cuts={"ZeroBias","Nvtx"}, .draw="", .opt="Norm", .ranges={} });
  sh.AddHistos("evt", { .fill="NCluFPix",     .pfs={main12}, .cuts={"ZeroBias","Nvtx"}, .draw="", .opt="Norm", .ranges={} });
  sh.AddHistos("evt", { .fill="NPixFPix",     .pfs={main12}, .cuts={"ZeroBias","Nvtx"}, .draw="", .opt="Norm", .ranges={} });
#endif
#endif
  
  // Federror stuff
  sh.AddHistos("evt", { .fill="AvgNTimeoutEvt_vs_InstLumi",          .pfs={main12}, .cuts={"ZeroBias","Nvtx"}, .draw="PE1", .opt="", .ranges={} });
  sh.AddHistos("evt", { .fill="AvgNTimeoutEvt_vs_Pileup",            .pfs={main12}, .cuts={"ZeroBias","Nvtx"}, .draw="PE1", .opt="", .ranges={} });
  sh.AddHistos("evt", { .fill="AvgNTimeoutEvt_vs_NBx",               .pfs={main12}, .cuts={"ZeroBias","Nvtx"}, .draw="PE1", .opt="", .ranges={} });
  sh.AddHistos("evt", { .fill="AvgNTimeoutEvt_vs_BunchCrossing60",   .pfs={main12}, .cuts={"ZeroBias","Nvtx"}, .draw="PE1", .opt="", .ranges={} });
  sh.AddHistos("evt", { .fill="AvgNOverflowEvt_vs_InstLumi",         .pfs={main12}, .cuts={"ZeroBias","Nvtx"}, .draw="PE1", .opt="", .ranges={} });
  sh.AddHistos("evt", { .fill="AvgNOverflowEvt_vs_Pileup",           .pfs={main12}, .cuts={"ZeroBias","Nvtx"}, .draw="PE1", .opt="", .ranges={} });
  sh.AddHistos("evt", { .fill="AvgNOverflowEvt_vs_NBx",              .pfs={main12}, .cuts={"ZeroBias","Nvtx"}, .draw="PE1", .opt="", .ranges={} });
  sh.AddHistos("evt", { .fill="AvgNOverflowEvt_vs_BunchCrossing60",  .pfs={main12}, .cuts={"ZeroBias","Nvtx"}, .draw="PE1", .opt="", .ranges={} });
  
  // Federror stuff (traj)
  //sh.AddHistos("traj", { .fill="HitEfficiency_vs_LayersDisks",        .pfs={"FedErrorEvent","Data"},  .cuts={"ZeroBias","EffCutsNoFederr"}, .draw="PE1", .opt="", .ranges={0,0, 0,0} });
  //sh.AddHistos("traj", { .fill="HitEfficiency_vs_LayersDisks",        .pfs={"FedErrorModule","Data"}, .cuts={"ZeroBias","EffCutsNoFederr"}, .draw="PE1", .opt="", .ranges={0,0, 0,0} });
  //sh.AddHistos("traj", { .fill="AvgTimeoutMod_vs_InstLumi",           .pfs={"LayersDisks","Data"},    .cuts={"ZeroBias","Nvtx"},            .draw="PE1", .opt="", .ranges={0,xmax_instl, 0,0} });
  //sh.AddHistos("traj", { .fill="AvgTimeoutMod_vs_Pileup",             .pfs={"LayersDisks","Data"},    .cuts={"ZeroBias","Nvtx"},            .draw="PE1", .opt="", .ranges={0,25,  0,0} });
  //sh.AddHistos("traj", { .fill="AvgTimeoutMod_vs_NBx",                .pfs={"LayersDisks","Data"},    .cuts={"ZeroBias","Nvtx"},            .draw="PE1", .opt="", .ranges={0,0,  0,0} });
  //sh.AddHistos("traj", { .fill="AvgTimeoutMod_vs_BunchCrossing60",    .pfs={"LayersDisks","Data"},    .cuts={"ZeroBias","Nvtx"},            .draw="PE1", .opt="", .ranges={0,0,  0,0} });
  //sh.AddHistos("traj", { .fill="AvgOverflowMod_vs_InstLumi",          .pfs={"LayersDisks","Data"},    .cuts={"ZeroBias","Nvtx"},            .draw="PE1", .opt="", .ranges={0,xmax_instl, 0,0} });
  //sh.AddHistos("traj", { .fill="AvgOverflowMod_vs_Pileup",            .pfs={"LayersDisks","Data"},    .cuts={"ZeroBias","Nvtx"},            .draw="PE1", .opt="", .ranges={0,25,  0,0} });
  //sh.AddHistos("traj", { .fill="AvgOverflowMod_vs_NBx",               .pfs={"LayersDisks","Data"},    .cuts={"ZeroBias","Nvtx"},            .draw="PE1", .opt="", .ranges={0,0,  0,0} });
  //sh.AddHistos("traj", { .fill="AvgOverflowMod_vs_BunchCrossing60",   .pfs={"LayersDisks","Data"},    .cuts={"ZeroBias","Nvtx"},            .draw="PE1", .opt="", .ranges={0,0,  0,0} }); 
  
  //__________________________________________________________________________________
  //
  //                     Cluster Plots (traj - on-track, clust - all)
  
  // Distributions
  sh.AddHistos("clust", { .fill="CluSize",         .pfs={main12,"LayersDisksInOut"}, .cuts={"ZeroBias"},                    .draw="", .opt="Norm", .ranges={0,0, 0,0} });
  sh.AddHistos("clust", { .fill="CluSize",         .pfs={main12,"Layers","Rings"},   .cuts={"ZeroBias","BPix"},             .draw="", .opt="Norm", .ranges={0,0, 0,0} });
  sh.AddHistos("clust", { .fill="CluCharge",       .pfs={main12,"LayersDisksInOut"}, .cuts={"ZeroBias"},                    .draw="", .opt="Norm", .ranges={0,100, 0,0} });
  sh.AddHistos("clust", { .fill="CluCharge",       .pfs={main12,"Layers","Rings"},   .cuts={"ZeroBias","BPix"},             .draw="", .opt="Norm", .ranges={0,200, 0,0} });
  sh.AddHistos("traj",  { .fill="OnCluSize",       .pfs={main12,"LayersDisksInOut"}, .cuts={"ZeroBias","Pt>1GeV","NStrip>10"},        .draw="", .opt="Norm", .ranges={1,20} });
  sh.AddHistos("traj",  { .fill="OnCluSize",       .pfs={main12,"Layers","Rings"},   .cuts={"ZeroBias","Pt>1GeV","NStrip>10","BPix"}, .draw="", .opt="Norm", .ranges={1,20} });
  sh.AddHistos("traj",  { .fill="OnCluCharge",     .pfs={main12,"LayersDisksInOut"}, .cuts={"ZeroBias","Pt>1GeV","NStrip>10"},        .draw="", .opt="Norm", .ranges={2,250} });
  sh.AddHistos("traj",  { .fill="OnCluCharge",     .pfs={main12,"Layers","Rings"},   .cuts={"ZeroBias","Pt>1GeV","NStrip>10","BPix"}, .draw="", .opt="Norm", .ranges={2,250} });
  sh.AddHistos("traj",  { .fill="OnCluChargeNorm", .pfs={main12,"LayersDisksInOut"}, .cuts={"ZeroBias","Pt>1GeV","NStrip>10"},        .draw="", .opt="Norm", .ranges={0,60} });
  sh.AddHistos("traj",  { .fill="OnCluChargeNorm", .pfs={main12,"Layers","Rings"},   .cuts={"ZeroBias","Pt>1GeV","NStrip>10","BPix"}, .draw="", .opt="Norm", .ranges={0,60} });
  
  // Occupancy
  sh.AddHistos("clust", { .fill="ROCLadders_vs_ROCModules",           .pfs={main12,"Layers"},  .cuts={"ZeroBias","BPix"},      .draw="COLZ", .opt="Log", .ranges={0,0, 0,0, 0,0} });
  sh.AddHistos("clust", { .fill="ROCBladesInner_vs_ROCSideDiskPanel", .pfs={main12},           .cuts={"ZeroBias","FPixInner"}, .draw="COLZ", .opt="Log", .ranges={0,0, 0,0, 0,0} });
  sh.AddHistos("clust", { .fill="ROCBladesOuter_vs_ROCSideDiskPanel", .pfs={main12},           .cuts={"ZeroBias","FPixOuter"}, .draw="COLZ", .opt="Log", .ranges={0,0, 0,0, 0,0} });
  //sh.AddHistos("traj",  { .fill="ROCLadders_vs_ROCModules",           .pfs={"Layers"},   .cuts={"ZeroBias","ValidHit","BPix"},      .draw="COLZ", .opt="Log", .ranges={0,0, 0,0, 0,0} });
  //sh.AddHistos("traj",  { .fill="ROCBladesInner_vs_ROCSideDiskPanel", .pfs={},           .cuts={"ZeroBias","ValidHit","FPixInner"}, .draw="COLZ", .opt="Log", .ranges={0,0, 0,0, 0,0} });
  //sh.AddHistos("traj",  { .fill="ROCBladesOuter_vs_ROCSideDiskPanel", .pfs={},           .cuts={"ZeroBias","ValidHit","FPixOuter"}, .draw="COLZ", .opt="Log", .ranges={0,0, 0,0, 0,0} });
  
  // Historical plots
  sh.AddHistos("clust", { .fill="AvgCluSize_vs_Runs",           .pfs={"LayersDisks","Data"}, .cuts={"ZeroBias"}, .draw="PE1", .opt="", .ranges={0,0, 0,0} });
  sh.AddHistos("clust", { .fill="AvgCluCharge_vs_Runs",         .pfs={"LayersDisks","Data"}, .cuts={"ZeroBias"}, .draw="PE1", .opt="", .ranges={0,0, 0,0} });
  sh.AddHistos("clust", { .fill="CluChargeMPV_vs_Runs",         .pfs={"LayersDisks","Data"}, .cuts={"ZeroBias"}, .draw="PE1", .opt="", .ranges={0,0, 0,0} });
  sh.AddHistos("clust", { .fill="AvgCluSize_vs_IntLumiRunII",   .pfs={"LayersDisks","Data"}, .cuts={"ZeroBias"}, .draw="PE1", .opt="", .ranges={0,0, 0,0} });
  sh.AddHistos("clust", { .fill="AvgCluCharge_vs_IntLumiRunII", .pfs={"LayersDisks","Data"}, .cuts={"ZeroBias"}, .draw="PE1", .opt="", .ranges={0,0, 0,0} });
  sh.AddHistos("clust", { .fill="CluChargeMPV_vs_IntLumiRunII", .pfs={"LayersDisks","Data"}, .cuts={"ZeroBias"}, .draw="PE1", .opt="", .ranges={0,0, 0,0} });
  
  sh.AddHistos("traj",  { .fill="AvgCluSize_vs_Runs",           .pfs={"LayersDisks","Data"}, .cuts={"ZeroBias","Pt>1GeV","NStrip>10"}, .draw="PE1", .opt="", .ranges={0,0, 0,0} });
  sh.AddHistos("traj",  { .fill="AvgCluCharge_vs_Runs",         .pfs={"LayersDisks","Data"}, .cuts={"ZeroBias","Pt>1GeV","NStrip>10"}, .draw="PE1", .opt="", .ranges={0,0, 0,0} });
  sh.AddHistos("traj",  { .fill="CluChargeMPV_vs_Runs",         .pfs={"LayersDisks","Data"}, .cuts={"ZeroBias","Pt>1GeV","NStrip>10"}, .draw="PE1", .opt="", .ranges={0,0, 0,0} });
  sh.AddHistos("traj",  { .fill="AvgCluSize_vs_IntLumiRunII",   .pfs={"LayersDisks","Data"}, .cuts={"ZeroBias","Pt>1GeV","NStrip>10"}, .draw="PE1", .opt="", .ranges={0,0, 0,0} });
  sh.AddHistos("traj",  { .fill="AvgCluCharge_vs_IntLumiRunII", .pfs={"LayersDisks","Data"}, .cuts={"ZeroBias","Pt>1GeV","NStrip>10"}, .draw="PE1", .opt="", .ranges={0,0, 0,0} });
  sh.AddHistos("traj",  { .fill="CluChargeMPV_vs_IntLumiRunII", .pfs={"LayersDisks","Data"}, .cuts={"ZeroBias","Pt>1GeV","NStrip>10"}, .draw="PE1", .opt="", .ranges={0,0, 0,0} });
  
  // Random events
  //sh.AddHistos("clust", { .fill="AvgCluCharge_vs_Runs",         .pfs={"First4BX","LayersDisks","Data"}, .cuts={"Random"}, .draw="PE1", .opt="", .ranges={0,0, 0,0} });
  //sh.AddHistos("clust", { .fill="AvgCluSize_vs_Runs",           .pfs={"First4BX","LayersDisks","Data"}, .cuts={"Random"}, .draw="PE1", .opt="", .ranges={0,0, 0,0} });
  //sh.AddHistos("traj",  { .fill="AvgCluCharge_vs_Runs",         .pfs={"First4BX","LayersDisks","Data"}, .cuts={"Random"}, .draw="PE1", .opt="", .ranges={0,0, 0,0} });
  //sh.AddHistos("traj",  { .fill="AvgCluSize_vs_Runs",           .pfs={"First4BX","LayersDisks","Data"}, .cuts={"Random"}, .draw="PE1", .opt="", .ranges={0,0, 0,0} });
  
  std::function<void()> add_special_histos = [&sh](){
    //sh.AddHistos("clust", { .fill="CluCharge",                  .pfs={"First4BX","LayersDisks","Runs","Data"}, .cuts={"Random"}, .draw="HIST", .opt="", .ranges={0,0, 0,5} });
    //sh.AddHistos("clust", { .fill="CluSize",                    .pfs={"First4BX","LayersDisks","Runs","Data"}, .cuts={"Random"}, .draw="HIST", .opt="", .ranges={0,0, 0,5} });
  };
  
  //__________________________________________________________________________________
  //
  //                             Hit Efficiency Plots
  
  // Pileup reweighting dependent plots
  // Layer/Disk Average Plots
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_LayersDisks",        .pfs={},                                       .cuts={"EffCuts"},                   .draw="PE1", .opt="", .ranges={0,0, 0.98,1} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_LayersDisks",        .pfs={main12},                                 .cuts={"ZeroBias","EffCuts"},        .draw="PE1", .opt="", .ranges={0,0, 0.98,1} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_LayersDisks",        .pfs={main12,"InstLumi"},                      .cuts={"ZeroBias","EffCuts"},        .draw="PE1", .opt="", .ranges={0,0, 0.98,1} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_LayersDisks",        .pfs={main12,"Ring12/34"},                     .cuts={"ZeroBias","EffCuts"},        .draw="PE1", .opt="", .ranges={0,0, 0.98,1} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_LayersDisks",        .pfs={main12,"InstLumi","Ring12/34"},          .cuts={"ZeroBias","EffCuts"},        .draw="PE1", .opt="", .ranges={0,0, 0.98,1} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_LayersDisksInOut",   .pfs={main12},                                 .cuts={"ZeroBias","EffCuts"},        .draw="PE1", .opt="", .ranges={0,0, 0.98,1} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_LayersDisksInOut",   .pfs={main12,"InstLumi"},                      .cuts={"ZeroBias","EffCuts"},        .draw="PE1", .opt="", .ranges={0,0, 0.98,1} });
  
  // Geometry Plots: Ladder, Ring, Module, ROC Efficiencies
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_Modules",            .pfs={main12,"Layers"},                        .cuts={"ZeroBias","EffCuts","BPix"}, .draw="PE1",  .opt="", .ranges={0,0, 0.98,1} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_Modules",            .pfs={main12,"InstLumi","Layers"},             .cuts={"ZeroBias","EffCuts","BPix"}, .draw="PE1",  .opt="", .ranges={0,0, 0.98,1} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_Ladders",            .pfs={main12,"Layers"},                        .cuts={"ZeroBias","EffCuts","BPix"}, .draw="PE1",  .opt="", .ranges={0,0, 0.98,1} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_Ladders",            .pfs={main12,"Layers","Ring12/34"},            .cuts={"ZeroBias","EffCuts","BPix"}, .draw="PE1",  .opt="", .ranges={0,0, 0.98,1} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_Ladders",            .pfs={main12,"InstLumi","Layers"},             .cuts={"ZeroBias","EffCuts","BPix"}, .draw="PE1",  .opt="", .ranges={0,0, 0.98,1} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_Ladders",            .pfs={main12,"InstLumi","Layers","Ring12/34"}, .cuts={"ZeroBias","EffCuts","BPix"}, .draw="PE1",  .opt="", .ranges={0,0, 0.98,1} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_Ladders_vs_Modules", .pfs={main12,"Layers"},                        .cuts={"ZeroBias","EffCuts","BPix"}, .draw="COLZ", .opt="", .ranges={0,0, 0,0, 0.97,1} });  
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_ROCLadders_vs_ROCModules",           .pfs={main12,"Layers"},        .cuts={"ZeroBias","EffCuts","BPix"},      .draw="COLZ", .opt="", .ranges={0,0, 0,0, 0,1} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_ROCBladesInner_vs_ROCSideDiskPanel", .pfs={main12},                 .cuts={"ZeroBias","EffCuts","FPixInner"}, .draw="COLZ", .opt="", .ranges={0,0, 0,0, 0,1} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_ROCBladesOuter_vs_ROCSideDiskPanel", .pfs={main12},                 .cuts={"ZeroBias","EffCuts","FPixOuter"}, .draw="COLZ", .opt="", .ranges={0,0, 0,0, 0,1} });
  
  // Efficiency N-1 plots
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_TrkNStrip",  .pfs={"LayersDisks",main12},  .cuts={"ZeroBias","EffCutsNoNStrip"}, .draw="PE1",  .opt="",     .ranges={0,0,   0.99,1} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_TrkPt",      .pfs={"LayersDisks",main12},  .cuts={"ZeroBias","EffCutsNoPt"},     .draw="PE1",  .opt="logX", .ranges={0,0,   0.99,1} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_TrkD0",      .pfs={"LayersDisks",main12},  .cuts={"ZeroBias","EffCutsNoD0"},     .draw="PE1",  .opt="logX", .ranges={0,0.1, 0.98,1} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_TrkDZ",      .pfs={"LayersDisks",main12},  .cuts={"ZeroBias","EffCutsNoDZ"},     .draw="PE1",  .opt="logX", .ranges={0,1,   0.99,1} });
  
  // Plots that doesn't need reweighting
  sh.SetHistoWeights({});
  //sh.SetHistoWeights({}); // Do not need to weight instlumi histo
  
  // InstLumi
  double xmax_instl = 15;
  double ymin_instl = 0.9;
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_Modules_vs_InstLumi0p5", .pfs={main12,"Layers"},                .cuts={"ZeroBias","EffCuts","BPix"},             .draw="COLZ", .opt="", .ranges={0,xmax_instl, -4,4, 0.95,1} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_Ladders_vs_InstLumi0p5", .pfs={main12,"Layers"},                .cuts={"ZeroBias","EffCuts","BPix"},             .draw="COLZ", .opt="", .ranges={0,xmax_instl,  0,0, 0.95,1} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_Ladders_vs_InstLumi0p5", .pfs={main12,"Layers","Rings"},        .cuts={"ZeroBias","EffCuts","BPix"},             .draw="COLZ", .opt="", .ranges={0,xmax_instl,  0,0, 0.95,1} });
  
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_InstLumi",    .pfs={main12,       "LayersDisks"},               .cuts={"ZeroBias","EffCuts","MainFills"},        .draw="PE1",  .opt="", .ranges={0,xmax_instl, ymin_instl,1, 0.4,0.5} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_InstLumi",    .pfs={main12,       "Layers","Ring12/34"},        .cuts={"ZeroBias","EffCuts","MainFills","BPix"}, .draw="PE1",  .opt="", .ranges={0,xmax_instl, ymin_instl,1, 0.4,0.5} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_InstLumi",    .pfs={"LayersDisks", main12},                     .cuts={"ZeroBias","EffCuts"},                    .draw="PE1",  .opt="", .ranges={0,xmax_instl, ymin_instl,1, 0.4,0.5} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_InstLumi",    .pfs={"LayersDisks","Pileup4","Data"},            .cuts={"ZeroBias","EffCuts"},                    .draw="PE1",  .opt="Keep", .ranges={0,xmax_instl, ymin_instl,1, 0.4,0.5} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_InstLumi",    .pfs={"LayersDisks","SelectedNBx","Data"},        .cuts={"ZeroBias","EffCuts"},                    .draw="PE1",  .opt="Keep", .ranges={0,xmax_instl, ymin_instl,1, 0.4,0.5} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_InstLumi",    .pfs={"Rings",      "Layers", main12},            .cuts={"ZeroBias","EffCuts","BPix"},             .draw="PE1",  .opt="", .ranges={0,xmax_instl, ymin_instl,1, 0.4,0.5} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_InstLumi",    .pfs={"Pileup4",    "LayersDisks","Data"},        .cuts={"ZeroBias","EffCuts"},                    .draw="PE1",  .opt="Keep", .ranges={0,xmax_instl, ymin_instl,1, 0.4,0.5} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_InstLumi",    .pfs={"SelectedNBx","LayersDisks","Data"},        .cuts={"ZeroBias","EffCuts"},                    .draw="PE1",  .opt="", .ranges={0,xmax_instl, ymin_instl,1, 0.4,0.5} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_InstLumi",    .pfs={"SelectedNBx","Layers","Ring12/34","Data"}, .cuts={"ZeroBias","EffCuts","BPix"},             .draw="PE1",  .opt="", .ranges={0,xmax_instl, ymin_instl,1, 0.4,0.5} });
  
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_InstLumi0p5", .pfs={main12,       "LayersDisks"},               .cuts={"ZeroBias","EffCuts","MainFills"},        .draw="PE1",  .opt="", .ranges={0,xmax_instl, ymin_instl,1, 0.4,0.5} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_InstLumi0p5", .pfs={main12,       "Layers","Ring12/34"},        .cuts={"ZeroBias","EffCuts","MainFills","BPix"}, .draw="PE1",  .opt="", .ranges={0,xmax_instl, ymin_instl,1, 0.4,0.5} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_InstLumi0p5", .pfs={"LayersDisks", main12},                     .cuts={"ZeroBias","EffCuts"},                    .draw="PE1",  .opt="", .ranges={0,xmax_instl, ymin_instl,1, 0.4,0.5} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_InstLumi0p5", .pfs={"LayersDisks","Pileup4","Data"},            .cuts={"ZeroBias","EffCuts"},                    .draw="PE1",  .opt="Keep", .ranges={0,xmax_instl, ymin_instl,1, 0.4,0.5} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_InstLumi0p5", .pfs={"LayersDisks","SelectedNBx","Data"},        .cuts={"ZeroBias","EffCuts"},                    .draw="PE1",  .opt="Keep", .ranges={0,xmax_instl, ymin_instl,1, 0.4,0.5} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_InstLumi0p5", .pfs={"Rings",      "Layers", main12},            .cuts={"ZeroBias","EffCuts","BPix"},             .draw="PE1",  .opt="", .ranges={0,xmax_instl, ymin_instl,1, 0.4,0.5} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_InstLumi0p5", .pfs={"Pileup4",    "LayersDisks","Data"},        .cuts={"ZeroBias","EffCuts"},                    .draw="PE1",  .opt="Keep", .ranges={0,xmax_instl, ymin_instl,1, 0.4,0.5} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_InstLumi0p5", .pfs={"SelectedNBx","LayersDisks","Data"},        .cuts={"ZeroBias","EffCuts"},                    .draw="PE1",  .opt="", .ranges={0,xmax_instl, ymin_instl,1, 0.4,0.5} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_InstLumi0p5", .pfs={"SelectedNBx","Layers","Ring12/34","Data"}, .cuts={"ZeroBias","EffCuts","BPix"},             .draw="PE1",  .opt="", .ranges={0,xmax_instl, ymin_instl,1, 0.4,0.5} });
  
  // Pileup
  double xmax_pu = 15;
  double ymin_pu = 0.9;
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_Pileup",      .pfs={main12,      "LayersDisks"},                         .cuts={"ZeroBias","EffCuts","MainFills"}, .draw="PE1",  .opt="", .ranges={0,xmax_pu, ymin_pu,1, 0.4,0.5} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_Pileup",      .pfs={main12,      "Layers","Ring12/34"},                  .cuts={"ZeroBias","EffCuts","MainFills","BPix"}, .draw="PE1",  .opt="", .ranges={0,xmax_pu, ymin_pu,1, 0.4,0.5} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_Pileup",      .pfs={"LayersDisks", main12},                              .cuts={"ZeroBias","EffCuts"},        .draw="PE1",  .opt="", .ranges={0,xmax_pu, ymin_pu,1, 0.4,0.5} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_Pileup",      .pfs={"LayersDisks","SelectedNBx","Data"},                 .cuts={"ZeroBias","EffCuts"},        .draw="PE1",  .opt="", .ranges={0,xmax_pu, ymin_pu,1, 0.4,0.5} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_Pileup",      .pfs={"Rings",     "Layers", main12},                      .cuts={"ZeroBias","EffCuts","BPix"}, .draw="PE1",  .opt="", .ranges={0,xmax_pu, ymin_pu,1, 0.4,0.5} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_Pileup",      .pfs={"SelectedNBx","LayersDisks","Data"},                 .cuts={"ZeroBias","EffCuts"},        .draw="PE1",  .opt="", .ranges={0,xmax_pu, ymin_pu,1, 0.4,0.5} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_Pileup",      .pfs={"SelectedNBx","BX>500","LayersDisks","Data"},        .cuts={"ZeroBias","EffCuts"},        .draw="PE1",  .opt="", .ranges={0,xmax_pu, ymin_pu,1, 0.4,0.5} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_Pileup",      .pfs={"SelectedNBx","Layers","Ring12/34","Data"},          .cuts={"ZeroBias","EffCuts","BPix"}, .draw="PE1",  .opt="", .ranges={0,xmax_pu, ymin_pu,1, 0.4,0.5} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_Pileup",      .pfs={"SelectedNBx","BX>500","Layers","Ring12/34","Data"}, .cuts={"ZeroBias","EffCuts"},  .draw="PE1",  .opt="", .ranges={0,xmax_pu, ymin_pu,1, 0.4,0.5} });
  
  // L1Rate
  //sh.AddHistos("traj", { .fill="HitEfficiency_vs_InstLumi1p0",             .pfs={"L1Rate",  "LayersDisks","Years"}, .cuts={"ZeroBias","EffCuts","Data"},            .draw="PE1",  .opt="",     .ranges={0,xmax_instl, 0.98,1, 0.4,0.5} });
  //sh.AddHistos("traj", { .fill="HitEfficiency_vs_InstLumi1p0_vs_L1Rate",   .pfs={           "LayersDisks","Years"}, .cuts={"ZeroBias","EffCuts","Data"},            .draw="COLZ", .opt="",     .ranges={0,0, 0,xmax_instl, 0.98,1, 0.4,0.5} });
  //sh.AddHistos("traj", { .fill="HitEfficiency_vs_L1Rate",                  .pfs={           "LayersDisks","Years"}, .cuts={"ZeroBias","EffCuts","Data"},            .draw="PE1",  .opt="",     .ranges={0,0, 0.98,1, 0.4,0.5} });
  //sh.AddHistos("traj", { .fill="HitEfficiency_vs_L1Rate",                  .pfs={"SelectedNBx",     "LayersDisks","Years"}, .cuts={"ZeroBias","EffCuts","Data"},            .draw="PE1",  .opt="", .ranges={0,0, 0.98,1, 0.4,0.5} });
  //sh.AddHistos("traj", { .fill="HitEfficiency_vs_L1Rate",                  .pfs={"InstLumi","LayersDisks","Years"}, .cuts={"ZeroBias","EffCuts","Data"},            .draw="PE1",  .opt="",     .ranges={0,0, 0.98,1, 0.4,0.5} });
  
  // BunchCrossing
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_BunchCrossing60",         .pfs={"L1Rate",  "LayersDisks","Data"}, .cuts={"ZeroBias","EffCuts","MainFills"}, .draw="PE1",  .opt="",     .ranges={0,0, 0.98,1, 0.4,0.4} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_BunchCrossing60",         .pfs={"Pileup4", "LayersDisks","Data"}, .cuts={"ZeroBias","EffCuts","MainFills"},            .draw="PE1",  .opt="", .ranges={0,0, 0.98,1, 0.4,0.4} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_BunchCrossing60",         .pfs={"InstLumi","LayersDisks","Data"}, .cuts={"ZeroBias","EffCuts","MainFills"}, .draw="PE1",  .opt="",     .ranges={0,0, 0.98,1, 0.4,0.4} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_BunchCrossing60",         .pfs={"SelectedNBx","LayersDisks","Data"}, .cuts={"ZeroBias","EffCuts"},            .draw="PE1",  .opt="Keep", .ranges={0,0, 0.98,1, 0.4,0.4} });
  sh.AddHistos("traj", { .fill="HitEfficiency_vs_BunchCrossing60",         .pfs={"SelectedNBx","Pileup4","LayersDisks","Data"}, .cuts={"ZeroBias","EffCuts"},            .draw="PE1",  .opt="", .ranges={0,0, 0.98,1, 0.4,0.4} });
  
  //sh.AddHistos("traj", { .fill="HitEfficiency_vs_BunchCrossing12",         .pfs={"L1Rate",  "LayersDisks","Years","NBx"}, .cuts={"ZeroBias","EffCuts","Data"}, .draw="PE1",  .opt="Keep", .ranges={0,0, 0.97,1} });
  //sh.AddHistos("traj", { .fill="HitEfficiency_vs_BunchCrossing12",         .pfs={"InstLumi","LayersDisks","Years","NBx"}, .cuts={"ZeroBias","EffCuts","Data"}, .draw="PE1",  .opt="Keep", .ranges={0,0, 0.97,1} });
  //sh.AddHistos("traj", { .fill="HitEfficiency_vs_BunchCrossing60",         .pfs={"L1Rate",  "LayersDisks","Years","NBx"}, .cuts={"ZeroBias","EffCuts","Data"}, .draw="PE1",  .opt="Keep", .ranges={0,0, 0.97,1} });
  //sh.AddHistos("traj", { .fill="HitEfficiency_vs_BunchCrossing60",         .pfs={"InstLumi","LayersDisks","Years","NBx"}, .cuts={"ZeroBias","EffCuts","Data"}, .draw="PE1",  .opt="Keep", .ranges={0,0, 0.97,1} });
  
  // DCol Scan
  //sh.AddHistos("traj", { .fill="HitEfficiency_vs_LayersDisks",        .pfs={"DcolScan"},                 .cuts={"ZeroBias","EffCuts","ExclRing34"},        .draw="", .opt="", .ranges={0,0, 0,0} });
  //sh.AddHistos("traj", { .fill="HitEfficiency_vs_Ladders",            .pfs={"Layers","Data"},            .cuts={"ZeroBias","EffCuts","BPix","ExclRing34"}, .draw="", .opt="", .ranges={0,0, 0,0} });
  //sh.AddHistos("traj", { .fill="HitEfficiency_vs_Modules",            .pfs={"Layers","Data"},            .cuts={"ZeroBias","EffCuts","BPix"}, .draw="",    .opt="", .ranges={0,0, 0,0} });
  //sh.AddHistos("traj", { .fill="HitEfficiency_vs_Ladders_vs_Modules", .pfs={"DcolScan","Layers"},        .cuts={"ZeroBias","EffCuts","BPix"},              .draw="", .opt="", .ranges={0,0, 0,0} });
  //sh.AddHistos("traj", { .fill="HitEfficiency_vs_InstLumi",           .pfs={"Layers","Data"},            .cuts={"ZeroBias","EffCuts","BPix","ExclRing34"}, .draw="", .opt="", .ranges={0,0, 0,0} });
  //sh.AddHistos("traj", { .fill="HitEfficiency_vs_InstLumi",           .pfs={"Layers","Rings","Data"},    .cuts={"ZeroBias","EffCuts","BPix"},              .draw="", .opt="", .ranges={0,0, 0,0} });
  
  // Dcol Efficiency
  //sh.AddHistos("traj", { .fill="AvgMCDColEff_vs_Pileup",                   .pfs={main12,"LayersDisks"},               .cuts={"ZeroBias","EffCuts","MC"},              .draw="P",    .opt="", .ranges={0,50, 0,1} });
  ////sh.AddHistos("traj", { .fill="AvgMCDColEff_vs_InstLumi",                 .pfs={"LayersDisks",main12},               .cuts={"ZeroBias","EffCuts","MC"},              .draw="P",    .opt="", .ranges={0,0, 0,1} });
  //sh.AddHistos("traj", { .fill="AvgMCDColEff_vs_InstLumi",                 .pfs={main12,"LayersDisks"},               .cuts={"ZeroBias","EffCuts","MC"},              .draw="P",    .opt="", .ranges={0,xmax_instl, 0,1} });
  //sh.AddHistos("traj", { .fill="HitEfficiency_vs_NBxPerTrig",              .pfs={"Years","LayersDisks"},            .cuts={"ZeroBias","EffCuts","Data"},            .draw="PE1",  .opt="", .ranges={0,0, 0.9,1} });
  //sh.AddHistos("traj", { .fill="HitEfficiency_vs_NColPerTrig",             .pfs={"Years","LayersDisks"},            .cuts={"ZeroBias","EffCuts","Data"},            .draw="PE1",  .opt="", .ranges={0,0, 0.9,1} });
  
  //sh.AddHistos("traj", { .fill="DColEfficiency_vs_InstLumi0p25",  .pfs={"Rings"},          .cuts={"ZeroBias","EffCuts","Lay1","Data"}, .draw="PE1",  .opt="", .ranges={0,xmax_instl,         0.8,1} });
  //sh.AddHistos("traj", { .fill="AvgMCDColEff_vs_InstLumi0p25",    .pfs={"Rings"},          .cuts={"ZeroBias","EffCuts","Lay1","Data"}, .draw="PE1",  .opt="", .ranges={0,xmax_instl,        0.85,1} });
  //sh.AddHistos("traj", { .fill="HitEfficiency_vs_Modules",    .pfs={"InstLumi0.5"},    .cuts={"ZeroBias","EffCuts","Lay1","Data"}, .draw="PE1",  .opt="", .ranges={0,0,    0.95,1} });
  ////sh.AddHistos("traj", { .fill="DColEfficiency_vs_Modules",   .pfs={"InstLumi0.5"},    .cuts={"ZeroBias","EffCuts","Lay1","Data"}, .draw="PE1",  .opt="", .ranges={0,0,    0.8, 1} });
  //sh.AddHistos("traj", { .fill="DColEfficiency_vs_Modules",   .pfs={"InstLumi0.5"},    .cuts={"ZeroBias","EffCuts","Lay1","Data"}, .draw="PE1",  .opt="", .ranges={0,0,    0.85, 1.05} });
  //sh.AddHistos("traj", { .fill="AvgMCDColEff_vs_Modules",     .pfs={"InstLumi0.5"},    .cuts={"ZeroBias","EffCuts","Lay1","Data"}, .draw="PE1",  .opt="", .ranges={0,0,    0.8, 1} });
  //sh.AddHistos("traj", { .fill="HitEfficiency_vs_Ladders",    .pfs={"InstLumi0.5"},    .cuts={"ZeroBias","EffCuts","Lay1","Data"}, .draw="PE1",  .opt="", .ranges={-10,10, 0.95,1} });
  //sh.AddHistos("traj", { .fill="AvgMCDColEff_vs_Ladders",     .pfs={"InstLumi0.5"},    .cuts={"ZeroBias","EffCuts","Lay1","Data"}, .draw="PE1",  .opt="", .ranges={-10,10, 0.8, 1} });
  //sh.AddHistos("traj", { .fill="HitEfficiency_vs_Ladders",    .pfs={"InstLumi0.5","Rings"},    .cuts={"ZeroBias","EffCuts","Lay1","Data"}, .draw="PE1",  .opt="", .ranges={-10,10, 0.95,1} });
  //sh.AddHistos("traj", { .fill="DColEfficiency_vs_Ladders",   .pfs={"InstLumi0.5","Rings"},    .cuts={"ZeroBias","EffCuts","Lay1","Data"}, .draw="PE1",  .opt="", .ranges={-10,10, 0.8, 1} });
  //sh.AddHistos("traj", { .fill="AvgMCDColEff_vs_Ladders",     .pfs={"InstLumi0.5","Rings"},    .cuts={"ZeroBias","EffCuts","Lay1","Data"}, .draw="PE1",  .opt="", .ranges={-10,10, 0.8, 1} });

  //__________________________________________________________________________________
  //
  //                             Cosmics Time Alignment
  
  //++  //sh.SetHistoWeights({});
  //++  sh.AddHistos("traj", { .fill="HitEfficiency_vs_Runs",       .pfs={},      .cuts={"GlobalMuon","Lay3"},            .draw="PE1", .opt="", .ranges={0,0, 0,1} });
  //++  sh.AddHistos("traj", { .fill="AvgOnCluChargeNorm_vs_Runs",  .pfs={},      .cuts={"GlobalMuon","Lay3","ValidHit"}, .draw="PE1", .opt="", .ranges={0,0, 0,60} });
  //++  sh.AddHistos("traj", { .fill="AvgOnCluCharge_vs_Runs",      .pfs={},      .cuts={"GlobalMuon","Lay3","ValidHit"}, .draw="PE1", .opt="", .ranges={0,0, 0,60} });
  //++  sh.AddHistos("traj", { .fill="AvgOnCluSize_vs_Runs",        .pfs={},      .cuts={"GlobalMuon","Lay3","ValidHit"}, .draw="PE1", .opt="", .ranges={0,0, 0,6} });
  //++  sh.AddHistos("traj", { .fill="HitEfficiency_vs_TMuon",      .pfs={"WBC"}, .cuts={"GlobalMuon","Lay3"},            .draw="PE1", .opt="", .ranges={0,0, 0,1} });
  //++  sh.AddHistos("traj", { .fill="AvgOnCluChargeNorm_vs_TMuon", .pfs={"WBC"}, .cuts={"GlobalMuon","Lay3","ValidHit"}, .draw="PE1", .opt="", .ranges={0,0, 0,60} });
  //++  sh.AddHistos("traj", { .fill="AvgOnCluCharge_vs_TMuon",     .pfs={"WBC"}, .cuts={"GlobalMuon","Lay3","ValidHit"}, .draw="PE1", .opt="", .ranges={0,0, 0,60} });
  //++  sh.AddHistos("traj", { .fill="AvgOnCluSize_vs_TMuon",       .pfs={"WBC"}, .cuts={"GlobalMuon","Lay3","ValidHit"}, .draw="PE1", .opt="", .ranges={0,0, 0,6} });
  //++  sh.AddHistos("traj", { .fill="TMuon",                       .pfs={"WBC"}, .cuts={"GlobalMuon","Lay3"},            .draw="",.opt="Norm", .ranges={0,0, 0,0} });
  //++  //"OnCluCharge""SelectedRuns"
  
  std::cout<<"-----------------------------------------------------------------\n";
  std::cout<<"Creating the following plots:\n"; sh.PrintNames();
  std::cout<<"-----------------------------------------------------------------\n";
  
  if (filelist.size()&&!is_a) {
    std::cout<<"Adding "<<filelist.size()<<" files from the shell arguments:\n";
    for (size_t i=0; i<filelist.size(); ++i) {
      std::cout<<filelist[i]<<std::endl;
      looper.AddFile(filelist[i]);
    }
  } else {
    // Add input files here
    //looper.AddFile("/data/gridout/jkarancs/TimingStudy/v3836_INC_SPL1_ns0_710pre9_DYToMuMu_13TeV_Spring14dr_RECO/Ntuple_1_1_CiR.root");
    
    // trajTree only (VER 4) - unfortunately directory wildcards doesn't work
    //looper.AddFile("/data/gridout/jkarancs/TimingStudy/v3735_INC_SPL1_ns11_5314_MB_2012C_22JanRR_RECO_201278/*.root");
    //looper.AddFile("/data/gridout/tvami/SiPixelDigitizer/DynIneff3/100/*/*.root");
    //looper.AddFile("/data/gridout/tvami/SiPixelDigitizer/DynIneff3/99_9/*/*.root");
    //looper.AddFile("/data/gridout/tvami/SiPixelDigitizer/DynIneff3/99_6/*/*.root");
    //looper.AddFile("/data/gridout/tvami/SiPixelDigitizer/DynIneff3/99_3/*/*.root");
    //looper.AddFile("/data/gridout/tvami/SiPixelDigitizer/DynIneff3/99/*/*.root");
    //looper.AddFile("/data/gridout/tvami/SiPixelDigitizer/DynIneff3/98/*/*.root");
    //looper.AddFile("/data/gridout/tvami/SiPixelDigitizer/DynIneff3/97/*/*.root");
    //looper.AddFile("/data/gridout/tvami/SiPixelDigitizer/DynIneff3/95/*/*.root");
    //looper.AddFile("/data/gridout/tvami/SiPixelDigitizer/DynIneff3/90/*/*.root");
    //looper.AddFile("/data/gridout/tvami/SiPixelDigitizer/DynIneff3/80/*/*.root");
    
    // Data + Dcol scan - with clustTree (VER 5) 
    //looper.AddFile("/data/gridout/mbartok/DynIneff/TimingStudy_clusttree_Run2012C-22Jan2013-v1_RECO/*.root");
    //looper.AddFile("/data/bartokm/DynamicIneff/eff_loss/clustercharge/CMSSW_7_1_0_pre6/src/DPGAnalysis/PixelTimingStudy/dcol100.root");
    //looper.AddFile("/data/bartokm/DynamicIneff/eff_loss/clustercharge/CMSSW_7_1_0_pre6/src/DPGAnalysis/PixelTimingStudy/dcol99_9.root");
    //looper.AddFile("/data/bartokm/DynamicIneff/eff_loss/clustercharge/CMSSW_7_1_0_pre6/src/DPGAnalysis/PixelTimingStudy/dcol99_6.root");
    //looper.AddFile("/data/bartokm/DynamicIneff/eff_loss/clustercharge/CMSSW_7_1_0_pre6/src/DPGAnalysis/PixelTimingStudy/dcol99_3.root");
    //looper.AddFile("/data/bartokm/DynamicIneff/eff_loss/clustercharge/CMSSW_7_1_0_pre6/src/DPGAnalysis/PixelTimingStudy/dcol99.root");
    //looper.AddFile("/data/bartokm/DynamicIneff/eff_loss/clustercharge/CMSSW_7_1_0_pre6/src/DPGAnalysis/PixelTimingStudy/dcol98.root");
    //looper.AddFile("/data/bartokm/DynamicIneff/eff_loss/clustercharge/CMSSW_7_1_0_pre6/src/DPGAnalysis/PixelTimingStudy/dcol97.root");
    //looper.AddFile("/data/bartokm/DynamicIneff/eff_loss/clustercharge/CMSSW_7_1_0_pre6/src/DPGAnalysis/PixelTimingStudy/dcol95.root");
    //looper.AddFile("/data/bartokm/DynamicIneff/eff_loss/clustercharge/CMSSW_7_1_0_pre6/src/DPGAnalysis/PixelTimingStudy/dcol90.root");
    //looper.AddFile("/data/bartokm/DynamicIneff/eff_loss/clustercharge/CMSSW_7_1_0_pre6/src/DPGAnalysis/PixelTimingStudy/dcol80.root");
    
    // DynIneff Validation
    // FPix DynIneff Validation
    if (VER == -1) { 
      // L1Rate test
      // 2011 data 7 TeV
      //std::vector<std::string> lowlumi = lowlumi_runs_2011(); for (size_t i=0; i<lowlumi.size(); ++i) looper.AddFile(lowlumi[i], i==0); // lowlumi fills
      looper.AddFile("/data/gridout/jkarancs/TimingStudy/2011/v2928_INC_SPL0_ns0_442p2_MB_2011A_08Nov2011_RECO/*.root");  // full 2011 data
      looper.AddFile("/data/gridout/jkarancs/TimingStudy/2011/v2928_INC_SPL0_ns0_442p2_MB_2011B_19Nov2011_RECO/*.root",0);
    } else if (VER == 4) {
      // Data
      //looper.AddFile("dummy_file",-1);
      // Run 201278
      //looper.AddFile("/data/gridout/jkarancs/TimingStudy/v3735_INC_SPL1_ns11_5314_MB_2012C_22JanRR_RECO_201278/*.root");
      looper.AddFile("/data/gridout/mbartok/DynIneff/TimingStudy_clusttree_Run2012C-22Jan2013-v1_RECO/*.root"); // same with clustTree
      // Run 203002
      //std::vector<std::string> v_str = run_203002(); for (size_t i=0; i<v_str.size(); ++i) looper.AddFile(v_str[i], i==0);
      // Full 2012
      //looper.AddFile("/data/gridout/jkarancs/TimingStudy/v3735_INC_SPL2_ns11_5315p1_MB_2012A_22JanRR_RECO_ZB_13may07JSON/*.root");
      //looper.AddFile("/data/gridout/jkarancs/TimingStudy/v3735_INC_SPL2_ns11_5315p1_MB_2012B_22JanRR_RECO_ZB_13may07JSON/*.root",0);
      //looper.AddFile("/data/gridout/jkarancs/TimingStudy/v3735_INC_SPL2_ns11_5315p1_MB_2012C_22JanRR_RECO_ZB_13may07JSON/*.root",0);
      //looper.AddFile("/data/gridout/jkarancs/TimingStudy/v3735_INC_SPL2_ns11_5315p1_MB_2012D_22JanRR_RECO_ZB_13may07JSON/*.root",0);
      
      // MCs
      //looper.AddFile("dummy_file",-1);
      //looper.AddFile("/data/gridout/mbartok/DynIneff/MC_pre6_final_new_ringfactor_validation_all_dyn_ineff_factors/*.root"); // current (710_pre8), no fpix, no clustTree
      //looper.AddFile("dummy_file",-1);
    } else if (VER==5) {
      // Data
      //+++ looper.AddFile("dummy_file",-1);
      
      // 8 TeV MC
      //looper.AddFile("/data/gridout/jkarancs/TimingStudy/v3836_INC_SPL1_ns0_710pre9_MB_8TeV_noDynIneff_RECO_resol/*.root"); // no DynIneff (MC_Old)
      //looper.AddFile("/data/gridout/mbartok_2/DynIneff/Timingstudy_no_dynineff_CMSSW_7_2_0_pre5_clusttree/*.root");         // no DynIneff with clustTree
      //+++ looper.AddFile("dummy_file",-1); //(MC - VER 4)
      //looper.AddFile("/data/gridout/jkarancs/TimingStudy/v3836_INC_SPL1_ns11_720pre1_MB_GENSIM_201278_flatPU_ViktorPU/*.root"); // with ViktorPU (MC_New)
      //looper.AddFile("/data/gridout/jkarancs/TimingStudy/v3836_INC_SPL1_ns11_720pre1_MB_GENSIM_201278_flatPU_ViktorPU/Marton_run/*.root",0); // with ViktorPU more stats
      //looper.AddFile("/data/gridout/mbartok/DynIneff/pu_viktor_validation_flatpu0-10/*.root",0);
      //looper.AddFile("/data/gridout/mbartok/DynIneff/pu_viktor_validation_noPU/*.root",0);
      //looper.AddFile("/data/gridout/mbartok_2/DynIneff/FPix/fpix_ineff_lay23_newring4factor/*.root"); // current (710_pre8) + fixed ring4 (lay2,3) + fpix ineff 8TeV 50ns
      
      // 13 TeV MC
      //looper.AddFile("/data/gridout/jkarancs/TimingStudy/v3836_COMP_SPL1_ns0_720pre5_MB_GENSIM_flatPU_13TeV_50ns/*.root"); // fpix+ring4+beamspot+13TeV/50ns (8TeV*1.18)
      ////looper.AddFile("/data/gridout/jkarancs/TimingStudy/v3836_COMP_SPL1_ns0_720pre5_MB_GENSIM_flatPU_13TeV_25ns/*.root"); // fpix+ring4+beamspot+13TeV/25ns (8TeV*1.18*2)
      //looper.AddFile("/data/gridout/mbartok/DynIneff/RUN2validation/MinBias_13TeV_BS25_GEN_SIM_RECO_ntuple_FlatPU_DynIneff/*.root"); // fpix+ring4+beamspot+13TeV/25ns (8TeV*1.18*1.82)
      //looper.AddFile("dummy_file",-1);
      //looper.AddFile("/data/gridout/mbartok_2/DynIneff/RUN2validation/MinBias_13TeV_BS50_GEN_SIM_RECO_ntuple_FlatPU_DynIneff/*.root"); // fpix+ring4+beamspot+13TeV/50ns (8TeV*1.1)
      //+++ looper.AddFile("/data/gridout/mbartok/DynIneff/RUN2validation/v3836_COMP_SPL1_ns0_720pre5_MB_GENSIM_flatPU_13TeV_50ns_PSI/*.root"); // Final 13TeV/50ns
      //+++ looper.AddFile("/data/gridout/jkarancs/TimingStudy/v3836_COMP_SPL1_ns0_720pre5_MB_GENSIM_flatPU_13TeV_25ns_PSI/*.root");            // Final 13TeV/25ns-PSI
      
      // 13 TeV 50ns PSI Sim
      // looper.AddFile("dummy_file",-1);
      // looper.AddFile("dummy_file",-1);
      // looper.AddFile("dummy_file",-1);
      // //looper.AddFile("dummy_file",-1);
      // looper.AddFile("/data/gridout/mbartok/DynIneff/RUN2validation/MB_GENSIM_flatPU_13TeV_50ns_PSI_2015_jan/*.root");
      // looper.AddFile("dummy_file",-1);
      
      // run 203002 MC
      //looper.AddFile("/data/gridout/jkarancs/TimingStudy/v3836_COMP_SPL2_ns0_712_RelValSingleNuE10_GENSIMRECO_203002PU/*.root");
      
      // Cosmics Time alignment test
      //looper.AddFile("/data/gridout/jkarancs/TimingStudy/2015/ExpressCosmics/crab_v3836_COMP_SPL1_ns0_732_Express/150214_164514/0000/*.root/TimingStudy");
      
      //looper.AddFile("/data/gridout/jkarancs/TimingStudy/TimingScan2015/ExpressPhysics/v3836_INC_SPL2_ns0_744p1_GR_R_74/150603_192816/0000/*.root");
      //looper.AddFile("/data/gridout/jkarancs/TimingStudy/TimingScan2015/batch_v3836_COMP_SPL2_ns0_744p1_GR_R_74_TimingScan_Express_246908_2/*.root"); // COMP - 908
      //looper.AddFile("/data/gridout/jkarancs/TimingStudy/TimingScan2015/ExpressPhysics/TimingScan2015_Express_01/150603_144545/0000/*.root"); // COMP
      
      // --------------------------- RUN II --------------------------------
      
      //--------------- SPEC DATA ------------------
      
      // HV Scan 18 - 2015 June 08
      //looper.AddFile("/data/gridout/jkarancs/TimingStudy/Scans/HV18_ZB1/*.root");
      
      //--------------- 2015 DATA ------------------
      //looper.AddFile("Dummy - 50ns Data",-1);
      looper.AddFile("Dummy - 25ns Data",-1);
      
      // 13 TeV Data - 3.8T - 50ns - 2015B     (INC SPL1 ns0 10thclu)
      //looper.AddFile("/data/gridout/mbartok/HitEff/Run2015B/MinimumBias/v3836_INC_SPL1_ns0_10thclu_746p6_74X_dataRun2_Prompt_v0_HitEff/150713_111608/0000/*.root");
      //looper.AddFile("/data/gridout/mbartok/HitEff/Run2015B/ZeroBias/v3837_INC_SPL1_ns0_10thclu_746p6_74X_dataRun2_Prompt_v0_HitEff_ZB_50ns_38T_full2015/150902_143559/0000/*.root");
      
      // 13 TeV Data - 3.8T - 25ns - 2015C     (INC SPL1 ns0 10thclu)
      //looper.AddFile("/data/gridout/mbartok/HitEff/Run2015C/ZeroBias/v3837_INC_SPL1_ns0_10thclu_746p6_74X_dataRun2_Prompt_v0_HitEff_ZB_25ns_38T/150902_120925/0000/*.root");
      
      // 13 TeV Data - 3.8T - 25ns - Full 2015 (INC SPL0 ns11 100thclu)
      //looper.AddFile("/data/gridout/jkarancs/TimingStudy/2015/v3836_INC_SPL0_ns11_100thclu_7415p1_74X_dataRun2_v4_Nov13SilverJSON/ZB_Run2015C_25ns/0000/*.root");
      //looper.AddFile("/data/gridout/jkarancs/TimingStudy/2015/v3836_INC_SPL0_ns11_100thclu_7415p1_74X_dataRun2_v4_Nov13SilverJSON/ZB_Run2015D_PRv3/0000/*.root",0);
      //looper.AddFile("/data/gridout/jkarancs/TimingStudy/2015/v3836_INC_SPL0_ns11_100thclu_7415p1_74X_dataRun2_v4_Nov13SilverJSON/ZB_Run2015D_PRv3/0001/*.root",0);
      //looper.AddFile("/data/gridout/jkarancs/TimingStudy/2015/v3836_INC_SPL0_ns11_100thclu_7415p1_74X_dataRun2_v4_Nov13SilverJSON/ZB_Run2015D_PRv3/0002/*.root",0);
      //looper.AddFile("/data/gridout/jkarancs/TimingStudy/2015/v3836_INC_SPL0_ns11_100thclu_7415p1_74X_dataRun2_v4_Nov13SilverJSON/ZB_Run2015D_PRv3/0003/*.root",0);
      //looper.AddFile("/data/gridout/jkarancs/TimingStudy/2015/v3836_INC_SPL0_ns11_100thclu_7415p1_74X_dataRun2_v4_Nov13SilverJSON/ZB_Run2015D_PRv4/0000/*.root",0);
      //looper.AddFile("/data/gridout/jkarancs/TimingStudy/2015/v3836_INC_SPL0_ns11_100thclu_7415p1_74X_dataRun2_v4_Nov13SilverJSON/ZB_Run2015D_PRv4/0001/*.root",0);
      //looper.AddFile("/data/gridout/jkarancs/TimingStudy/2015/v3836_INC_SPL0_ns11_100thclu_7415p1_74X_dataRun2_v4_Nov13SilverJSON/ZB_Run2015D_PRv4/0002/*.root",0);
      // Contains 2232b fills
      //looper.AddFile("/data/gridout/jkarancs/TimingStudy/2015/v3836_INC_SPL0_ns11_100thclu_7415p1_74X_dataRun2_v4_Nov13SilverJSON/ZB_Run2015D_PRv4/0002/*.root");
      
      //---------------- 2015 MC -------------------
      //looper.AddFile("Dummy - 50ns MC",-1);
      //looper.AddFile("Dummy - 25ns MC",-1);
      
      // DCol scan
      // looper.AddFile("/data/gridout/mbartok/DynIneff/RUN2_13TeV_50ns/DcolScan/08/MinBias_DcolScan/v3837_INC_SPL1_ns0_10thclu_750_75X_mcRun2_asymptotic_v1_DcolScan_08/150909_080454/0000/*.root");
      // looper.AddFile("/data/gridout/mbartok/DynIneff/RUN2_13TeV_50ns/DcolScan/09/MinBias_DcolScan/v3837_INC_SPL1_ns0_10thclu_750_75X_mcRun2_asymptotic_v1_DcolScan_09/150908_081340/0000/*.root");
      // looper.AddFile("/data/gridout/mbartok/DynIneff/RUN2_13TeV_50ns/DcolScan/095/MinBias_DcolScan/v3837_INC_SPL1_ns0_10thclu_750_75X_mcRun2_asymptotic_v1_DcolScan_095/150907_090057/0000/*.root");
      // looper.AddFile("/data/gridout/mbartok/DynIneff/RUN2_13TeV_50ns/DcolScan/097/MinBias_DcolScan/v3837_INC_SPL1_ns0_10thclu_750_75X_mcRun2_asymptotic_v1_DcolScan_097/150902_222906/0000/*.root");
      // looper.AddFile("/data/gridout/mbartok/DynIneff/RUN2_13TeV_50ns/DcolScan/098/MinBias_DcolScan/v3837_INC_SPL1_ns0_10thclu_750_75X_mcRun2_asymptotic_v1_DcolScan_098/150902_101845/0000/*.root");
      // looper.AddFile("/data/gridout/mbartok/DynIneff/RUN2_13TeV_50ns/DcolScan/099/MinBias_DcolScan/v3837_INC_SPL1_ns0_10thclu_750_75X_mcRun2_asymptotic_v1_DcolScan_099/150901_141509/0000/*.root");
      // looper.AddFile("/data/gridout/mbartok/DynIneff/RUN2_13TeV_50ns/DcolScan/0993/MinBias_DcolScan/v3837_INC_SPL1_ns0_10thclu_750_75X_mcRun2_asymptotic_v1_DcolScan_0993/150831_113523/0000/*.root");
      // looper.AddFile("/data/gridout/mbartok/DynIneff/RUN2_13TeV_50ns/DcolScan/0996/MinBias_DcolScan/v3837_INC_SPL1_ns0_10thclu_750_75X_mcRun2_asymptotic_v1_DcolScan_0996/150831_222112/0000/*.root");
      // looper.AddFile("/data/gridout/mbartok/DynIneff/RUN2_13TeV_50ns/DcolScan/0999/MinBias_DcolScan/v3837_INC_SPL1_ns0_10thclu_750_75X_mcRun2_asymptotic_v1_DcolScan_0999/150831_085351/0000/*.root");
      // looper.AddFile("/data/gridout/mbartok/DynIneff/RUN2_13TeV_50ns/DcolScan/1/MinBias_DcolScan/v3837_INC_SPL1_ns0_10thclu_750_75X_mcRun2_asymptotic_v1_DcolScan_1/150830_151425/0000/*.root");
      
      // 2015 Prediction - 50ns
      //looper.AddFile("/data/gridout/mbartok/DynIneff/RUN2_13TeV_50ns/HitEff/2014factors/MinBias_TuneCUETP8M1_13TeV-pythia8/v3837_INC_SPL1_ns0_10thclu_741p1_MCRUN2_74_V9_All_13TeV50nsMinBias/150830_162738/0000/Ntuple_*.root");
      
      // 2015 Prediction
      //looper.AddFile("/data/gridout/mbartok/DynIneff/RUN2_13TeV_25ns/HitEff/2014factors/MinBias_TuneCUETP8M1_13TeV-pythia8/v3837_INC_SPL1_ns0_10thclu_741p1_MCRUN2_74_V9_All_13TeV25nsMinBias/150831_092806/0000/*.root");
      looper.AddFile("/data/gridout/jkarancs/TimingStudy/2015/v3836_COMP_SPL1_ns0_720pre5_MB_GENSIM_flatPU_13TeV_25ns_PSI/*.root"); // Final 13TeV/25ns-PSI
      
      // 2015 Simualtion - 2232b
      looper.AddFile("/data/gridout/ahunyadi/dyn_ineff/MinBias_TuneCUETP8M1_13TeV-pythia8/dcolscan_dynIneff_fill_2015_CMSSW76X/160206_140309/0000/*.root");
      looper.AddFile("/data/gridout/ahunyadi/dyn_ineff/MinBias_TuneCUETP8M1_13TeV-pythia8/dcolscan_dynIneff_fill_2015_CMSSW76X/160206_140309/0001/*.root",0);
      looper.AddFile("/data/gridout/ahunyadi/dyn_ineff/MinBias_TuneCUETP8M1_13TeV-pythia8/dcolscan_dynIneff_fill_2015_CMSSW76X/160206_140309/0002/*.root",0);
      
      // 2016 Prediction
      looper.AddFile("/data/gridout/ahunyadi/dyn_ineff/MinBias_TuneCUETP8M1_13TeV-pythia8/dcolscan_dynIneff_fill_2016_CMSSW76X/160208_115541/0000/*.root");
      looper.AddFile("/data/gridout/ahunyadi/dyn_ineff/MinBias_TuneCUETP8M1_13TeV-pythia8/dcolscan_dynIneff_fill_2016_CMSSW76X/160208_115541/0001/*.root",0);
      looper.AddFile("/data/gridout/ahunyadi/dyn_ineff/MinBias_TuneCUETP8M1_13TeV-pythia8/dcolscan_dynIneff_fill_2016_CMSSW76X/160208_115541/0002/*.root",0);
      
    }
    // This comes last, it is needed for event weight, set to match pileup distribution of data that is used
    //looper.AddDataPileupHisto(0, "../../PileupHistogram_201278.root", "pileup", 501269);
    //looper.SetData(0, 353769); // 2nd arg (Nevt) is optional
    //looper.AddDataPileupHisto(0, "PileupHistogram_2012Data_FlatPUMC.root", "pileup", 51743715);
  }
  
  // Load/Add Histos from a file
  if (!is_a) {
    // Dcol Scan:
    // sh.Load("CluCharge_DcolScan_MConly.root");
    //sh.Add("New_*.root");
    //sh.Add("new.root");
    // DynIneff Validation:
    //sh.Add("PHM_out/Validation_Data_CurrMC.root");
    //sh.Add("PHM_out/Validation_OldMC_NewMC.root");
    //sh.Add("PHM_out/Validation_NewMC.root");
    //sh.Add("PHM_out/Validation_0PU_NewMC.root");
    //sh.Add("PHM_out/Validation_with_OldMC.root");
    //sh.Load("PHM_out/Validation_OldMC.root");
    //sh.Load("PHM_out/test.root");
    //sh.Add("PHM_out/Validation2_VER4.root");
    //sh.Add("PHM_out/Validation2_VER5.root");
    //sh.Add("PHM_out/Validation3_Data_2012*.root");
    //sh.Add("PHM_out/Validation3_VER4.root");
    //sh.Add("PHM_out/Validation3_VER5.root");
    //sh.Add("PHM_out/Validation4_Data.root");
    //sh.Add("PHM_out/Validation4_Data_2012*.root");
    //sh.Add("PHM_out/Data_vs_MC_trajonly_VER4_53X.root");
    //sh.Add("PHM_out/Data_vs_MC_trajonly_VER5.root");
    //sh.Add("PHM_out/Data_vs_MC_trajonly.root");
    // sh.Add("PHM_out/FPixValidation_2012Data.root");
    // //sh.Add("PHM_out/FPixValidation_201278.root");
    // sh.Add("PHM_out/FPixValidation_MCs.root");
    // sh.Load("PHM_out/FPixValidation_MC_Curr.root");
    // //sh.Load("PHM_out/FPixValidation_MC_NoIneff.root");
    //sh.Add("PHM_out/L1Rate_2011Data.root");
    //sh.Add("PHM_out/L1Rate_2012Data.root");
    //sh.Load("PHM_out/Bunches_4th.root");
    
    //sh.Load("PHM_out/13TeV_CurrMC_hack.root");
    //sh.Load("PHM_out/13TeV_NewMC_hack_4th.root");
    
    //sh.Add("PHM_out/PSI_Data_clu.root");
    ////sh.Add("PHM_out/PSI_Data.root");
    //sh.Load("PHM_out/13TeV_2012Data.root");
    //sh.Add("PHM_out/13TeV_CurrMC.root");
    //sh.Add("PHM_out/13TeV_NewMC_corr3.root");
    //sh.Load("PHM_out/PSI_MC25ns.root");
    //sh.Add("New.root");
    //sh.Load("PHM_out/Run2015D_part0002_vs_MC_25ns_4th.root");
    //sh.Add("PHM_out/Run2015D_part0_fullstat.root");
    //sh.Add("PHM_out/Run2015D_part1_1_fullstat.root");
    //sh.Add("PHM_out/Run2015D_part1_3_fullstat.root");
    //sh.Add("PHM_out/Run2015D_part1_2_fullstat.root");
    //sh.Add("PHM_out/Run2015D_part2_fullstat.root");
    //sh.Add("PHM_out/MC_25ns_fullstat.root");
    
    //LAST //sh.Load("PHM_out/Data_vs_MC_50_and_25ns_fullstat.root");
    //LAST //sh.Load("PHM_out/Run2015D_vs_MC_25ns_Comparison_fullstat.root");
    //LAST sh.Load("PHM_out/PSI_50ns_Added.root");
    //LAST //sh.Load("PHM_out/HitEff_13TeV_ZB_Run2015D_25ns_2ndrun_part1and2.root");
    //LAST 
    //LAST //sh.Load("HitEff_13TeV_ZB_Run2015D_25ns_2ndrun_259721.root");
    //LAST //sh.Add("PHM_out/*2ndrun.root");
    //LAST //sh.Add("PHM_out/2015_25ns_Data_merged.root");
    //LAST sh.Load("PHM_out/2015_25ns_Data_Part1.root");
    //LAST sh.Add("PHM_out/2015_25ns_Data_Part2.root");
    //LAST sh.Add("PHM_out/2015_25ns_Data_Part3.root");
    //LAST //sh.Add("PHM_out/2015_25ns_Data_with_FedErrors_part1.root");
    //LAST //sh.Add("PHM_out/2015_25ns_Data_with_FedErrors_part2.root");
    //LAST //sh.Add("PHM_out/2015_25ns_Data_with_FedErrors_part3.root");
    
    //sh.Add("PHM_out/DataMCComp_*.root");
    
  } else {
    std::cout<<"Adding histograms from "<<filelist.size()<<" files from the shell arguments:\n";
    for (size_t i=0; i<filelist.size(); ++i) {
      std::cout<<filelist[i]<<std::endl;
      sh.Add(filelist[i]);
    }
  }
  
  // Loop on trees and fill histograms
  if (!is_a) looper.LoopOnTrees(&sh, add_special_histos, opt_n);
  
  // Write histograms and canvases in an output file
  TFile* f_out = new TFile(outputfile.size()?outputfile.c_str():"PHM_out/test.root","recreate"); 
  sh.DrawPlots();
  sh.Write();
  f_out->Close();
  std::cout<<"Writing plots to file: "<<f_out->GetName()<<" done.\n";
  
  return 1;
}