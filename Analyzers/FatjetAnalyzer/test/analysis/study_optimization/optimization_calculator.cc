void optimization_calculator() {
//	TFile *tf = TFile::Open("optimization_matrices.root");
	TFile *tf = TFile::Open("optimization_matrices_21tau750.root");
	
	vector<int> masses = {100, 150, 200, 250, 300, 400, 500};
	vector<int> sigs = {1, 2, 3, 4};
	vector<TH2*> avgs;
	
	for (int j = 0; j < sigs.size(); j ++) {
		for (int i = 0; i < masses.size(); i ++) {
			TString name = "sq" + to_string(masses[i]) + "to4j_s" + to_string(sigs[j]);
			TH2* sig = (TH2*) tf->Get(name);
			if (i == 0) avgs.push_back(sig);
			else avgs[j]->Add(sig);
		}
		avgs[j]->Scale(1/((double) masses.size()));
	}
	avgs[3]->Draw("colz");
	
	
	
//	vector<map<tuple<double, double>, double>> averages;
//	for (int j = 0; j < sigs.size(); j ++) {
//		map<tuple<double, double>, double> avgs;
//		for (int i = 0; i < masses.size(); i ++) {
//			TString name = "sq" + to_string(masses[i]) + "to4j_s" + to_string(sigs[j]);
//			TH2F* sig = (TH2F*) tf->Get(name);
//			
//			for (int k = 1; k < sig->GetXaxis()->GetNbins(); k ++) {
//				for (int l = 1; l < sig->GetYaxis()->GetNbins(); l ++) {
//					tuple<double, double> taus = make_tuple(sig->GetXaxis()->GetBinCenter(k), sig->GetYaxis()->GetBinCenter(l));
//					if (i == 0) avgs[taus] = 0;
//					avgs[taus] += sig->GetBinContent(k, l);
//				}
//			}
//		}
//		
//		for(auto const &kv : avgs) {
//			avgs[kv.first] = kv.second/masses.size();
//		}
//		averages.push_back(avgs);
//	}
//	
////	cout << averages[3][2525] << endl;
////	cout << averages[3][2626] << endl;
//	
//	double maxs = 0;
//	tuple<double, double> maxcode;
//	for(auto const &kv : averages[3]) {
//		if (kv.second > maxs) {
//			maxs = kv.second;
//			maxcode = kv.first;
//		}
//	}
//	cout << get<0>(maxcode) << "  " << get<1>(maxcode) << "  " << maxs << endl;
}
