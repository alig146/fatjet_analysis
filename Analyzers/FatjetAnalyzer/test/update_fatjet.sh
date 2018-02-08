#!/bin/bash

cd $CMSSW_BASE/src &&
git pull origin master &&
cd $CMSSW_BASE/src/Deracination/Straphanger/test/truculence &&
git pull origin master &&
cd $CMSSW_BASE/src/Deracination/Straphanger/test/decortication && 
git pull origin	master

