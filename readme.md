# Fatjet Analysis

## Prerequisites
Before installing, make sure that you've already (generated SSH keys on your machine)[https://help.github.com/articles/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent/] and (added them to your GitHub account)[https://help.github.com/articles/adding-a-new-ssh-key-to-your-github-account/].

You need to also install a CMSSW release. You can do this with
```
cmsrel CMSSW_xx_yy_zz_patchp
```

## Installation
Inside of the appropriate `CMSSW_xx_yy_zz_patchp/src` run
```
git clone git@github.com:elliot-hughes/fatjet_analysis.git .
```
Then,
```
cd Analyzers/FatjetAnalyzer/test
bash install.sh
```
