# JetTuplizer
The JetTuplizer tuplizes jet collections. All jets that pass the tuplizer pT cut are tuplized.

## Branches
The tuples produced contain information about jets (AK4, AK8, and CA12), particle flow leptons and photons, generator-level quarks, and event information. The branches have names in the following format: `n_t_v` where `n` represents the object name (e.g., `ca12` for CA12 jets), `t` represents the object type (e.g., `pf` for particle flow), and `v` represents the branch variable (e.g., `pt` for the transverse momentum).
### Jet branches
The jet branches contain the following variables:

* `neef` - Neutral electromagnetic energy fraction
* `ceef` - Charged electromagnetic energy fraction
* `nhef` - Neutral hadron energy fraction
* `chef` - Charged hadron energy fraction
* `mef` - Muon energy fraction
* `nm` - Neutral particle multiplicity
* `cm` - Charged particle multiplicity
* `n` - Number of constituents, `nm+cm`
* `f` - Hadron flavor: `5` for *b*-hadron, `4` for *c*-hadron, `0` for light hadron
* `bd_csv` - Combined secondary vertex b-tag discriminator (MiniAOD jets only)
* `jetid_l` - Loose [https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetID](jetID flag): `0` means the jet did not pass, `1` means that it did.
* `jetid_t` - Tight [https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetID](jetID flag): `0` means the jet did not pass, `1` means that it did.
* [...]

### Lepton branches
The lepton branches are formed from particle flow (`pf`) leptons and photons. They have the following names:

* `le` - Electrons
* `lm` - Muons
* `lt` - Tauons
* `lp` - Photons

The branches have the following variables:

* [...]

### Quark branches
The quark branches are formed from generator-level quarks and squarks (useful for signal and ttbar MC samples). They contain the following variables:

* `pid` - Particle ID
* [...]

### Event branches
The event branches contain the following variables:

* `pt_hat` - pT-hat (QCD MC only)
* `run` - Run number
* `lumi` - Lumisection number
* `event` - Event number
* [...]

