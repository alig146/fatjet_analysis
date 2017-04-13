eval `scramv1 runtime -sh`           # cmsenv

#if [ "$CMSSW_BASE" == "" ]; then
#	echo "[!!] Installation aborted! Please make sure to run 'cmsenv' before running this installation script."
#	exit 1
#fi

echo "install"
mkdir -p $CMSSW_BASE/src/Deracination/Straphanger/test
cd $CMSSW_BASE/src/Deracination/Straphanger/test
git clone git@github.com:elliot-hughes/truculence.git
git clone git@github.com:elliot-hughes/decortication.git
mkdir ../python
cd ../python
ln -s ../test/decortication/decortication/ decortication
ln -s ../test/truculence/truculence/ truculence
cd $CMSSW_BASE/src
scram b
