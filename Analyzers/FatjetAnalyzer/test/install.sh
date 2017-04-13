set -e

eval `scramv1 runtime -sh`           # cmsenv

echo "[..] Installing."
mkdir -p $CMSSW_BASE/src/Deracination/Straphanger/test
cd $CMSSW_BASE/src/Deracination/Straphanger/test
echo "[..] Fetching python packages."
git clone git@github.com:elliot-hughes/truculence.git
git clone git@github.com:elliot-hughes/decortication.git
#mkdir ../python
#cd ../python
cd $CMSSW_BASE/python
ln -s $CMSSW_BASE/src/Deracination/test/decortication/decortication/ decortication
ln -s $CMSSW_BASE/src/Deracination/test/truculence/truculence/ truculence

cd $CMSSW_BASE/src
echo "[..] Compiling CMSSW area."
scram b -j8

echo "[..] Downloading JECs."
cd $CMSSW_BASE/src/Analyzers/FatjetAnalyzer/test/jec_data
cmsRun download_jec.py
