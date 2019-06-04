# Fatjet Analysis

## Prerequisites
Before installing, make sure that you've already [generated SSH keys on your machine](https://help.github.com/articles/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent/) and [added them to your GitHub account](https://help.github.com/articles/adding-a-new-ssh-key-to-your-github-account/).

You also need to install a CMSSW release. You can do this with the following:
```
cmsrel CMSSW_xx_yy_zz_patchp
```

## Installation
Inside of the appropriate `CMSSW_xx_yy_zz_patchp/src` clone this repository.

Then,
```
cd Analyzers/FatjetAnalyzer/test
bash install.sh
```

At this point, you should be able to run the basic code. For more advanced functionality, refer to the applicable directions on the [repository wiki](https://github.com/elliot-hughes/fatjet_analysis/wiki).
