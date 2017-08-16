def histogramFilter(histName):
	if "params"      in histName: return False
	#if "QCD"       in histName: return False
	#if "TTbar"     in histName: return False
	#====SYSTEMATICS============
	#if "amp"       in histName: return False
	#if "shift"     in histName: return False
	#if "stretch"   in histName: return False
	#====SIGNAL================
	#if "Ms150"       in histName: return False
	#if "Ms200"       in histName: return False
	#if "Ms250"       in histName: return False
	#if "Ms300"       in histName: return False
	#if "Ms400"       in histName: return False
	else:
		print histName
		return True

def changeName(histName): return histName

def transformHist(hist) : return hist     #Nule --> No change

def getPDFDict(pdfNum)  :
	pdfStr = 'BkgCT10Var%i'%pdfNum
	temp = {pdfStr: (300, -3.0, 3.0)}
	return temp

def getUncert(Name)     :
	temp = {Name:(300, -3.0, 3.0)}
	return temp

def getHistWithoutFirstBin(hist):
	vals = hist.get_values()
	print hist.get_name(),"--->", vals
	newvals = vals[1:]
	binLowEdges=[]
	for i in range( 1,hist.get_nbins() ):
		binLowEdges.append( hist.get_x_low(i) )
	newHist = theta_auto.Histogram(binLowEdges[0],13000.,newvals,hist.get_uncertainties(),hist.get_name(),binLowEdges)
	return newHist

def MakeNullHisto(hist):
	if "3000" in hist.get_name():
		hist = hist.scale(0.0000001, hist.get_name() )
		print "--->After:",hist.get_values()
	return hist

def main():
#	INPUTFILE="theta_plots_sig_sb"
	INPUTFILE="theta_plots_sig15_sb"

	print "*"*30
	print "*"," "*4,INPUTFILE," "*(30-10-len(INPUTFILE)),"*"
	print "*"*30

	model = theta_auto.build_model_from_rootfile(INPUTFILE+".root",histogramFilter,transformHist,transformHist,False) #MC-uncertainties:False/True
	model.set_signal_processes('Ms*')
	print "_"*100 #print line
	model.add_lognormal_uncertainty("Normalization_ttbar", math.log(1.2), "TTbar")
	model.add_lognormal_uncertainty("Normalization_qcd", math.log(5.0), "QCD")
	model.add_lognormal_uncertainty("Normalization_ms100", math.log(1.2), "Ms100")
	model.add_lognormal_uncertainty("Normalization_ms150", math.log(1.2), "Ms150")
	model.add_lognormal_uncertainty("Normalization_ms200", math.log(1.2), "Ms200")
	model.add_lognormal_uncertainty("Normalization_ms250", math.log(1.2), "Ms250")
	model.add_lognormal_uncertainty("Normalization_ms300", math.log(1.2), "Ms300")
	model.add_lognormal_uncertainty("Normalization_ms400", math.log(1.2), "Ms400")
	model.add_lognormal_uncertainty("Normalization_ms500", math.log(1.2), "Ms500")
	#model.set_data_histogram("mggEBEB","DATA",True)
	
	theta_auto.model_summary(model,True,False,False)
	#==========================================
	limitOptions={
		'quantiles'             : [0.95],  #'accrate' to return the acceptance rate of the Markov Chain.
		'signal_process_groups' : None,
		'nuisance_constraint'   : None,
		'nuisance_prior_toys'   : None,
		'signal_prior'          : 'flat',
		'options'               : None,
		'parameter'             : 'beta_signal',
		'iterations'            : 50000, #Number of iterations to use in Markov Chain MC algo.
		'n_data'                : 100,   #Number of Markov Chains.
		'n_toy'                 : 1000,  #Number of toys for the expected Lim.Calculation.
		'run_theta'             : True,  #Run locally.
		'seed'                  : 0,
		#'smooth'                : True,
		#'hint_method'           : 'asimov-ll' #
		}
	#theta_auto.bayesian_quantiles(model, input, n, quantiles=[0.95], signal_process_groups=None, nuisance_constraint=None, nuisance_prior_toys=None, signal_prior='flat', options=None, parameter='beta_signal', iterations=10000, run_theta=True, seed=0, hint_method='asimov-ll')

	#===============================================================================
	#expected,observed = theta_auto.bayesian_limits(model,'expected','toys-asimov:0',**limitOptions)
	expected, observed = theta_auto.bayesian_limits(model, 'all', n_toy=500, n_data=10, iterations=50000)

	print "expected", expected
	print "observed", observed
	expected.write_txt("expected_" + INPUTFILE.replace("theta_plots_", "") + ".txt")
	observed.write_txt("observed_" + INPUTFILE.replace("theta_plots_", "") + ".txt")


	histogram_specs = {
		'beta_signal': (500, 0.0, 1.0),
		'shiftQCD': (500, -3.0, 3.0),
		'shiftTTbar': (500, -3.0, 3.0),
		'stretchQCD': (500, -3.0, 3.0),
		'stretchTTbar': (500, -3.0, 3.0),
		'Normalization_qcd': (500, -3.0, 3.0),
		'Normalization_ttbar': (500, -3.0, 3.0),
	}
	temp = theta_auto.bayesian_posteriors(model,'data',1,histogram_specs)
	theta_auto.write_histograms_to_rootfile(temp, INPUTFILE + "_posteriors.root")

	#mod_pred = theta_auto.bayesian_posterior_model_prediction(model,'data',1,signal_prior='flat')
	#theta_auto.write_histograms_to_rootfile(mod_pred,"PRED_"+INPUTFILE+".root")


	#histogram_specs2={'SMDiphoton':(1000,0.0,10.)}
	#PostInstance_PRED=theta_auto.bayesian_posterior_model_prediction(model,'data',1,histogram_specs2)
	#theta_auto.write_histograms_to_rootfile(PostInstance_PRED,"posteriors_PRED.root")

	# ayesianPostInstanceHist = theta_auto.bayesian_quantities(model,'data',10000,histogram_specs,None,None,[0.95],,,)
	#par_values={"beta_signal":0.15}
#	print theta_auto.evaluate_prediction(model,par_values,True,None)
	#HISTOS = model.get_parameters([])
	#print 'HISTOS', HISTOS
	#par_values={}
	#for p in model.get_parameters([]): par_values[p]=model.get_parameters([p])

	#theta_auto.write_histograms_to_rootfile(model.get_parameters([]),'Myfile.root')
	#==========================================

	#theta_auto.model_summary(model,True)

	#Func = theta_auto.HistogramFunction('cubiclinear')
	print "-->model.fill_histogram_zerobins(): ",model.fill_histogram_zerobins()
	print "-->get observables                : ",model.get_observables()
	print "-->model.has_data()               : ",model.has_data()
	print "-->get parameters                 : ",model.get_parameters([])
	#print "-->hist bins & range:",model.get_range_nbins('ADDMs3000')
	print "-------------ok till here------------------"

	################################################################################
	#signal_process_groups={"":[]}
	#fit = mle(model,'data',n=1,with_error=True,with_covariance=True,signal_process_groups=signal_process_groups)
	#parameter_values={}
	#for p in model.get_parameters([]):
	#    print " model.get_parameters-------->", p
	#    parameter_values[p] = fit[''][p][0][0]
	#PRED = evaluate_prediction(model,parameter_values,include_signal=False)
	#theta_auto.write_histograms_to_rootfile( PRED,"posteriors_BKD_"+INPUTFILE+".root" )
	################################################################################


	#************************************************************************
	pred = theta_auto.bayesian_posterior_model_prediction(model,'data',1, signal_process_groups=None,nuisance_constraint=None,nuisance_prior_toys=None,signal_prior='flat:[0,0.001]',iterations=50000)
	# This gives me the total background with uncertainties. (One for each signal, but the "signal_prior" is asking for the signal to be ignored.)

	theta_auto.write_histograms_to_rootfile(pred, INPUTFILE + "_prediction.root")

	#************************************************************************


	print "-------------ok till here------------------"


	##===============[ GET ALL NUISANCE PARAMETERS ]==================================================
#	temp = theta_auto.bayesian_posteriors( model,'data',1,getUncert('Normalization_') )
#	theta_auto.write_histograms_to_rootfile( temp,"posteriors_amp_"+INPUTFILE+".root" )
#	temp = theta_auto.bayesian_posteriors( model,'data',1,getUncert('shiftQCD') )
#	theta_auto.write_histograms_to_rootfile( temp,"posteriors_shift_"+INPUTFILE+".root" )
#	temp = theta_auto.bayesian_posteriors( model,'data',1,getUncert('stretch') )
#	theta_auto.write_histograms_to_rootfile( temp,"posteriors_stretch_"+INPUTFILE+".root" )
	##================================================================================================

	#model.signal_process_groups={
	#    'Ms3000': ['mggEBEB__Ms3000','mggEBEE__Ms3000'],
	#    'Ms3500': ['mggEBEB__Ms3500','mggEBEE__Ms3500'],
	#    'Ms4000': ['mggEBEB__Ms4000','mggEBEE__Ms4000'],
	#    'Ms4500': ['mggEBEB__Ms4500','mggEBEE__Ms4500'],
	#    'Ms5000': ['mggEBEB__Ms5000','mggEBEE__Ms5000'],
	#    'Ms5500': ['mggEBEB__Ms5500','mggEBEE__Ms5500'],
	#    'Ms6000': ['mggEBEB__Ms6000','mggEBEE__Ms6000']
	#    }
#	model = test_model.simple_counting(s = 2.0, b = 10.0, n_obs = 7, b_uncertainty = 3.0)
	signal_process_groups = {'': []}
#	fit = mle(model, 'data', n=1)
#	fit = mle(model, 'data', n=1, signal_process_groups=signal_process_groups)
	print "pre-mle"
	fit = mle(model, 'data', n=1, with_error=False, signal_process_groups=signal_process_groups)
	print "post-mle"
	parameter_values = {}
	for p in model.get_parameters([]):
		parameter_values[p] = fit[''][p][0][0]
	histos = evaluate_prediction(model, parameter_values, include_signal = False)
	write_histograms_to_rootfile(histos, INPUTFILE + '_mle.root')
	
	print "="*50
	print "MLE paramter values:"
	for key, value in parameter_values.items():
		print "{} = {}".format(key, value)
	print "="*50



# MAIN:
if __name__ == "__main__":
	main()
# /MAIN
