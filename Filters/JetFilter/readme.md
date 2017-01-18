# JetFilter
The JetFilter filters events based on their jet content. The user specifies the following:

* a jet collection (typically "selectedPatJetsCA12CHS")
* a pT cut (in GeV)
* an abs(eta) cut

If the two leading jets of the specified jet collection both pass these cuts, the event passes the filter.

## Example
An example of usage in a CMSSW configuration file:
```python
process.filter = cms.EDFilter("JetFilter",
	cut_pt=cms.double(175),
	cut_eta=cms.double(2.5),
	jetCollection=cms.InputTag("selectedPatJetsCA12CHS"),
)
```
