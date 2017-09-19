from decortication import dataset, eos
from ROOT import gROOT, gDirectory, TFile, TChain, TCanvas, TCut

gROOT.SetBatch()

tuple_eospath = "/store/user/tote/Sq200To4J/tuple_sq200to4j_0706_cutpt400eta25/170908_175449"

tf_out = TFile("conesize_plots.root", "recreate")

fnames = eos.listdir(tuple_eospath)
tchain = TChain("tuplizer/events")
for f in fnames: tchain.Add("root://cmseos.fnal.gov/" + tuple_eospath + "/" + f)

tcut = TCut("pre", "w[0]*(ak8_pf_ht[0]>900)") #	{"fjp_pre", "htak8>900&&Min$(pt)>400&&Max$(abs(eta))<2.0"},

tchain.Draw("ca12_pfp_m[0]*ca12_pfp_jmc[0]>>ca12_sq200to4j_m0(1200,0,1200)", tcut)
tchain.Draw("ak8_pfp_m[0]*ak8_pfp_jmc[0]>>ak8_sq200to4j_m0(1200,0,1200)", tcut)
tchain.Draw("(ca12_pfp_m[0]*ca12_pfp_jmc[0] + ca12_pfp_m[1]*ca12_pfp_jmc[1])/2>>ca12_sq200to4j_mavg(1200,0,1200)", tcut)
tchain.Draw("(ak8_pfp_m[0]*ak8_pfp_jmc[0] + ak8_pfp_m[1]*ak8_pfp_jmc[1])/2>>ak8_sq200to4j_mavg(1200,0,1200)", tcut)

h = gDirectory.Get("ca12_sq200to4j_m0")
tf_out.WriteTObject(h)
h = gDirectory.Get("ak8_sq200to4j_m0")
tf_out.WriteTObject(h)
h = gDirectory.Get("ca12_sq200to4j_mavg")
tf_out.WriteTObject(h)
h = gDirectory.Get("ak8_sq200to4j_mavg")
tf_out.WriteTObject(h)



#tuples = dataset.fetch_tuples(process="qcdmg", generation=["moriond17", "moriond17ext"])
#print len(tuples)
#files = [f for tup in tuples for f in tup.files]
#print len(files)

#tf_out = TFile("test.root", "recreate")

#tchain = TChain("tuplizer/events")
#for f in files: tchain.Add("root://cmseos.fnal.gov/" + f)
#tchain.Draw("(ca12_pfp_m[0] + ca12_pfp_m[1])/2>>ca12_qcdmg_mavg(1200,0,1200)", "w[0]*wpu[0]")
#h = gDirectory.Get("ca12_qcdmg_mavg")
#tf_out.WriteTObject(h)

#tc = TCanvas("test", "test")
#h.Draw()
#tc.SaveAs("test.pdf")
##input
