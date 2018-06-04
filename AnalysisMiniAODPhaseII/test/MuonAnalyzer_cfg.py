import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as opts

process = cms.Process("RecoMuon")

options = opts.VarParsing('analysis')

options.register ('inputDataset',
                  'PU200',
                  opts.VarParsing.multiplicity.singleton,
                  opts.VarParsing.varType.string,
                  'Input dataset')

options.parseArguments()

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('Demo')
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.MessageLogger.cerr.INFO = cms.untracked.PSet(limit = cms.untracked.int32(-1))


if 'PU200' in options.inputDataset :
    print '\n Will read /DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/PhaseIITDRSpring17MiniAOD-PU200BX8_91X_upgrade2023_realistic_v3-v6/MINIAODSIM\n'
    options.inputFiles = \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/14211B25-C077-E711-90D0-0025905D1D78.root', \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/1E5E5389-B477-E711-8518-0025905D1C56.root', \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/5823B0EF-EB78-E711-A5B1-1866DAEA6E20.root', \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/6671BA0A-9378-E711-8976-0025904C540C.root', \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/68954ACD-E778-E711-BFE8-0025905521D2.root', \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/70BEFF66-CD77-E711-B41D-B083FECFF2BE.root', \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/7ADEB521-0578-E711-B0B4-842B2B1810D3.root', \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/82597027-E577-E711-AC49-141877411D83.root', \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/825E4ACC-BB77-E711-91D3-0025904C66EA.root', \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/A0AA1AF5-9279-E711-9B35-782BCB5300A1.root', \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/BC4957D1-C577-E711-8473-0025905D1CB6.root', \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/D6BF7ECF-1E78-E711-A99D-B083FED429D5.root', \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/DAE448AC-A977-E711-8A39-0025904C66A2.root', \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/F0154A8B-F077-E711-95DC-141877410E71.root', \
    'file:/afs/cern.ch/user/p/piedra/work/store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/PU200BX8_91X_upgrade2023_realistic_v3-v6/110000/F08EEFEF-DB77-E711-9912-A4BADB1C5E26.root'

if 'noPU' in options.inputDataset :
    print '\n Will read /DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/PhaseIITDRSpring17MiniAOD-noPUBX8_91X_upgrade2023_realistic_v3-v3/MINIAODSIM\n'
    options.inputFiles = 'root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedMuons_Pt2to50_Dxy0to500-pythia8-gun/MINIAODSIM/noPUBX8_91X_upgrade2023_realistic_v3-v3/50000/FCEFF295-A27C-E711-92A6-0CC47AD98CEA.root'


if 'susy_pu' in options.inputDataset :
    print '\n Will read /DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/PhaseIITDRSpring17MiniAOD-PU200_91X_upgrade2023_realistic_v3-v2/MINIAODSIM \n'
    options.inputFiles ='root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/110000/1AD9250A-297F-E711-AD1B-48D539F38876.root','root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/110000/A2DEC450-707E-E711-BD0E-00259048AE50.root','root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/110000/BE2C32DF-097F-E711-B48B-0CC47A57CD56.root','root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/110000/D0162FF4-287F-E711-8039-0CC47A57CCF4.root','root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/110000/D2ACA348-557E-E711-AF2D-0CC47A4DEDFE.root','root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/70000/004CD079-877B-E711-AA24-00269E95B1BC.root',\
    'root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/70000/0628DE6A-517B-E711-AF5B-002590E7DE26.root',\
    'root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/70000/083AFF9C-0D7B-E711-9154-001E67E69E05.root',\
    'root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/70000/0C089C80-517B-E711-A8A2-008CFAF2222E.root',\
    'root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/70000/0CDD60CA-277B-E711-B0A9-6C3BE5B59150.root',\
    'root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/70000/0EA9129C-217B-E711-9675-0CC47A5FA3B9.root',
    'root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/70000/12C4CB84-E87A-E711-988E-0242AC1C0501.root',
    'root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/70000/1C3CC775-F77A-E711-8AF0-0CC47AD98CF2.root',
    'root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/70000/34650372-877B-E711-B277-A0369FC5B844.root',
    'root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/70000/3E2B439C-E97A-E711-B783-00266CFFC80C.root',
    'root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/70000/46DFDA8C-457B-E711-BD37-008CFA197480.root',
    'root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/70000/4A8F61D3-427A-E711-BA3D-008CFA14FA8C.root',
    'root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/70000/562EBE89-567B-E711-BC50-0025907B4EEE.root',
    'root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/70000/5AFDDB67-5F7B-E711-B319-3417EBE5062D.root',
    'root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/PU200_91X_upgrade2023_realistic_v3-v2/70000/5E94E2F5-167B-E711-8AEF-549F3525DB98.root'


if 'susy_0pu' in options.inputDataset :
    print '\n Will read /DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/PhaseIITDRSpring17MiniAOD_noPU_91X_upgrade2023_realistic_v3-v2/MINIAODSIM \n'
    options.inputFiles ='root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/50000/AA5A0677-FD7D-E711-AA69-48FD8EE73A8D.root', 'root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/00A47E5F-C47A-E711-87C2-0CC47AD98D6C.root','root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/149EF07A-C47A-E711-971B-00259074AE54.root','root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/1EF1CCD3-787A-E711-9BE0-1866DAEA7A40.root','root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/282CCB76-C47A-E711-969B-0242AC110009.root','root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/2E1DDD0D-C07A-E711-BB26-7845C4FC3C98.root','root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/3C0EA981-457A-E711-B93B-A0369F3102F6.root','root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/42B5A080-C47A-E711-8581-008CFAF5592A.root','root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/48FEC467-C47A-E711-9225-003048C8F3A2.root','root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/586E1B91-C47A-E711-A6AE-D4AE526A0419.root','root://cms-xrd-global.cern.ch//store/mc/PhaseIITDRSpring17MiniAOD/DisplacedSUSY_SmuonToMuNeutralino_M-200_CTau-1_TuneCUETP8M1_14TeV-pythia8/MINIAODSIM/noPU_91X_upgrade2023_realistic_v3-v2/70000/6ABB5C53-CB7A-E711-BBBC-0CC47ABB5178.root', 


process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(options.inputFiles))


#process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(3000))
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

process.TFileService=cms.Service('TFileService',
                                 fileName=cms.string('MyMuonPlots.root')
                                 )

process.muonAnalysis = cms.EDAnalyzer("ExampleMuonAnalyzer",
                                      MuonCollection = cms.InputTag('slimmedMuons'),
                                      packed = cms.InputTag("packedGenParticles"),
                                      pruned = cms.InputTag("prunedGenParticles"),
                                      vertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
                                      beamSpot = cms.InputTag("offlineBeamSpot")
                                      )

process.p = cms.Path(process.muonAnalysis)
