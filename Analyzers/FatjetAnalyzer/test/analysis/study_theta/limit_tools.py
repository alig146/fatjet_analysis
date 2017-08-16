from ROOT import TGraph, TGraphErrors, TGraphAsymmErrors

cross_sections = {
	100: [1521.11, 0.154038],		# value, percent uncertainty
	125: [574.981, 0.149895],
	150: [249.409, 0.147477],
	175: [121.416, 0.146341],
	200: [64.5085, 0.144098],
	225: [36.3818, 0.142189],
	250: [21.5949, 0.140595],
	275: [13.3231, 0.142549],
	300: [8.51615, 0.139223],
	325: [5.60471, 0.138144],
	350: [3.78661, 0.136877],
	375: [2.61162, 0.138477],
	400: [1.83537, 0.136985],
	425: [1.31169, 0.135013],
	450: [0.948333, 0.134559],
	475: [0.697075, 0.133926],
	500: [0.51848, 0.133797],
	525: [0.390303, 0.133443],
	550: [0.296128, 0.132687],
	575: [0.226118, 0.132741],
	600: [0.174599, 0.132074],
}

def parse_limit_data(filename):
	data = {
		"x": [],
		"y": [],
		"sigma1_plus": [],
		"sigma1_minus": [],
		"sigma2_plus": [],
		"sigma2_minus": [],
	}
	
	with open(filename) as f:
		for line in f:
			if line.strip()[0] == "#": continue
			pieces = line.strip().split()
			if len(pieces) >= 3:
				x = float(pieces[0])
				y = float(pieces[1])*cross_sections[int(x)][0]
				data["x"].append(x)
				data["y"].append(y)
			if len(pieces) != 6: continue
			data["sigma1_minus"].append(float(pieces[4])*cross_sections[int(x)][0])
			data["sigma1_plus"].append(float(pieces[5])*cross_sections[int(x)][0])
			data["sigma2_minus"].append(float(pieces[2])*cross_sections[int(x)][0])
			data["sigma2_plus"].append(float(pieces[3])*cross_sections[int(x)][0])
	return data


def make_limit_graphs(filename):
	name = filename.split("_")[0]
	tgraphs = {
		"graph": TGraph(),
		"sigma1": TGraphAsymmErrors(),
		"sigma2": TGraphAsymmErrors(),
	}
	for key, tg in tgraphs.items():
		tg.SetName("{}_{}".format(name, key))
	
	data = parse_limit_data(filename)
	xs = data["x"]
	for i, x in enumerate(xs):
		y = data["y"][i]
		for key, tg in tgraphs.items(): tg.SetPoint(i, x, y)
		if data["sigma1_plus"]:
			sigma1_plus = data["sigma1_plus"][i]
			sigma1_minus = data["sigma1_minus"][i]
			sigma2_plus = data["sigma2_plus"][i]
			sigma2_minus = data["sigma2_minus"][i]
	#		print y, sigma1_minus, sigma1_plus
	#		print y-sigma1_minus, sigma1_plus-y
			tgraphs["sigma1"].SetPointError(i, 0.0, 0.0, y-sigma1_minus, sigma1_plus-y)
			tgraphs["sigma2"].SetPointError(i, 0.0, 0.0, y-sigma2_minus, sigma2_plus-y)
	return tgraphs


def make_cross_graphs():
	tgraphs = {
		"cross": TGraph(),
		"cross_sigma1": TGraphErrors(),
	}
	tgraphs["cross"].SetName("cross")
	tgraphs["cross_sigma1"].SetName("cross_sigma1")
	xs = sorted(cross_sections.keys())
	for i, x in enumerate(xs):
		y = cross_sections[x][0]
		tgraphs["cross"].SetPoint(i, float(x), y)
		tgraphs["cross_sigma1"].SetPoint(i, float(x), y)
		tgraphs["cross_sigma1"].SetPointError(i, 0.0, y*cross_sections[x][1])
	return tgraphs
	



#	out = TFile("theta.root","recreate")
