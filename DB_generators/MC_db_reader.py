# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: --python_filename=DIGI_TO_RECO.py -s DIGI,L1,DIGI2RAW,RAW2DIGI,RECO --mc --conditions auto:run2_mc --pileup=NoPileUp --beamspot NominalCollision2015 --magField 38T_PostLS1 --customise SLHCUpgradeSimulations/Configuration/postLS1Customs.customisePostLS1 --process DigiToTimingstudy --eventcontent RECO -n 1000 --filein=/store/relval/CMSSW_7_5_0/RelValMinBias_13/GEN-SIM/75X_mcRun2_asymptotic_v1-v1/00000/3EF2051A-352A-E511-9B05-002590596498.root --fileout=DIGI_TO_RECO_1000_out.root --no_exec

max_events = 11
reco_output_file_name = "DIGI_TO_RECO_" + str(max_events) + "_out.root"
timing_study_output_file_name = "Ntuple.root"

import os.path
# filename = os.path.abspath(os.path.join(os.path.dirname("__file__"), os.pardir))

# parent_folder_name = os.path.abspath("__file__")
# parent_folder_name = os.path.split(os.path.dirname(parent_folder_name))[1]
# print '\n-> Parent folder name is: %s\n' % (parent_folder_name)

import FWCore.ParameterSet.Config as cms

process = cms.Process('DigiToTimingstudy')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mix_Flat_0_50_25ns_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(max_events)
)

# Input source
process.source = cms.Source("PoolSource",
    dropDescendantsOfDroppedBranches = cms.untracked.bool(False),
    fileNames = cms.untracked.vstring('/store/mc/RunIISummer15GS/MinBias_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v2/10000/004CC894-4877-E511-A11E-0025905C3DF8.root'),
    inputCommands = cms.untracked.vstring('keep *', 
        'drop *_genParticles_*_*', 
        'drop *_genParticlesForJets_*_*', 
        'drop *_kt4GenJets_*_*', 
        'drop *_kt6GenJets_*_*', 
        'drop *_iterativeCone5GenJets_*_*', 
        'drop *_ak4GenJets_*_*', 
        'drop *_ak7GenJets_*_*', 
        'drop *_ak8GenJets_*_*', 
        'drop *_ak4GenJetsNoNu_*_*', 
        'drop *_ak8GenJetsNoNu_*_*', 
        'drop *_genCandidatesForMET_*_*', 
        'drop *_genParticlesForMETAllVisible_*_*', 
        'drop *_genMetCalo_*_*', 
        'drop *_genMetCaloAndNonPrompt_*_*', 
        'drop *_genMetTrue_*_*', 
        'drop *_genMetIC5GenJs_*_*'),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('--python_filename=DIGI_TO_RECO.py nevts:' + str(max_events)),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.RECOoutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string(''),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    fileName = cms.untracked.string(reco_output_file_name),
    outputCommands = process.RECOEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

# Path and EndPath definitions
process.digitisation_step = cms.Path(process.pdigi)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOoutput_step = cms.EndPath(process.RECOoutput)

# customisation of the process.

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.postLS1Customs
from SLHCUpgradeSimulations.Configuration.postLS1Customs import customisePostLS1 

#call to customisation function customisePostLS1 imported from SLHCUpgradeSimulations.Configuration.postLS1Customs
process = customisePostLS1(process)

# End of customisation functions

#--------------- Added for TimingStudy ---------------

process.MessageLogger.cerr.FwkReport.reportEvery = 10

#-------------------------
#  MixingModule
#-------------------------

# GEN-SIM Input files with MinBias events
from DPGAnalysis.PixelTimingStudy.PoolSource_MinBias_2015_oct import *
process.mix.input.fileNames = fileNames

# Input pileup distribution for MixingModule
# Can use the same file in TimingStudy for pileup reweighting
# Note: the desired mc_input distribution has to be shifted by -1 wrt mcpileup
PileupHistoInput = cms.untracked.string(
    'file:PileupHistogram_201278_flatpileupMC.root' # Flat Pileup
    #'file:PileupHistogram_201278.root' # 201278 Pileup
)
PileupHistoName=cms.untracked.string('mc_input')


process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.QualityReader = cms.ESSource("PoolDBESSource",
    process.CondDBCommon.DBParameters,
    connect = cms.string('sqlite_file:dcol_scan_results.db'),
    toPut = cms.VPSet(cms.PSet(
        record = cms.string('SiPixelDynamicInefficiencyRcd'),
        tag = cms.string('SiPixelDynamicInefficiency_v1')
    ))
)

# Used to set which EventSetup provider should provide a particular data item
# in the case where multiple providers are capable of delivering the data.
# The first argument specifies the C++ class type of the prodiver.
# If the provider has been given a label, you must specify that label as the second argument.
# Additional 'vstring' arguments maybe used to specify exactly which EventSetup Records
# are being preferred and optionally which data items within that Record.
# E.g.,
#     #prefer all data in record 'OrangeRecord' from 'juicer'
#     ESPrefer("ESJuicerProd", OrangeRecord=cms.vstring())
# or
#     #prefer only "Orange" data in "OrangeRecord" from "juicer" 
#     ESPrefer("ESJuicerProd", OrangeRecord=cms.vstring("ExtraPulp"))
# or 
#     #prefer only "Orange" data with label "ExtraPulp" in "OrangeRecord" from "juicer" 
#     ESPrefer("ESJuicerProd", OrangeRecord=cms.vstring("Orange/ExtraPulp"))

# prefer all data in record "QualityReader" from "PoolDBESSource"
process.es_prefer_QualityReader = cms.ESPrefer("PoolDBESSource","QualityReader")

process.DynamicInefficiencyReader = cms.EDAnalyzer("SiPixelDynamicInefficiencyReader",
    printDebug = cms.untracked.bool(True)
  )
process.p = cms.Path(process.DynamicInefficiencyReader)

#-------------------------
#  Track Refitter
#-------------------------
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.Refitter = process.TrackRefitterP5.clone()
process.Refitter.src = 'generalTracks'
process.Refitter.TrajectoryInEvent = True

#-------------------------
#  TimingStudy
#-------------------------
process.TimingStudy = cms.EDAnalyzer("TimingStudy",
    trajectoryInput = cms.string('Refitter'),
    fileName = cms.string(timing_study_output_file_name),
    extrapolateFrom = cms.int32(2),
    extrapolateTo = cms.int32(1),
    keepOriginalMissingHit = cms.bool(False),
    usePixelCPE= cms.bool(True),
    minNStripHits = cms.int32(0),
    triggerNames=cms.vstring(
        "HLT_ZeroBias",
        "HLT_Physics",
        "HLT_Random",
        "HLT_PixelTracks_Multiplicity100",
        "HLT_PixelTracks_Multiplicity80",
        "HLT_PixelTracks_Multiplicity125",
        "HLT_PixelTracks_Multiplicity110",
        "HLT_PixelTracks_Multiplicity85",
        "HLT_PixelTracks_Multiplicity70",
        "HLT_PixelTracks_Multiplicity40",
        "HLT_L1Tech_BSC_HighMultiplicity",
        "HLT_JetE30_NoBPTX",
        "HLT_JetE30_NoBPTX_NoHalo",
        "HLT_JetE30_NoBPTX3BX_NoHalo",
        "HLT_JetE50_NoBPTX3BX_NoHalo",
        "HLT_60Jet10",
        "HLT_70Jet10",
        "HLT_70Jet13",
        "HLT_L1Tech_BSC_minBias",
        "HLT_MinBias"),
    dataPileupFile = cms.string("PileupHistogram_201278_flatpileupMC.root"),
    mcPileupFile   = cms.string("PileupHistogram_201278_flatpileupMC.root"),
    dataPileupHistoName = cms.string("pileup"),
    mcPileupHistoName = cms.string("mcpileup"),
    mcLumiScale = cms.double(200.0)
)

#-------------------------
#  Path/Schedule
#-------------------------
process.TimingStudy_step = cms.Path(process.Refitter*process.TimingStudy)

# Schedule definition
process.schedule = cms.Schedule(
process.digitisation_step,
process.L1simulation_step,
process.digi2raw_step,
process.raw2digi_step,
process.reconstruction_step,
process.TimingStudy_step,
process.endjob_step,
process.RECOoutput_step)
