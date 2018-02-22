#!/bin/bash

echo "[--] Note: this script only works with >= CMSSW_9_*, so make sure you've cmsenved there first."

eval "$(edmPickEvents.py --output=jetht16b '/JetHT/Run2016B-03Feb2017_ver2-v2/MINIAOD' events.txt)"
eval "$(edmPickEvents.py --output=jetht16c '/JetHT/Run2016C-03Feb2017-v1/MINIAOD' events.txt)"
eval "$(edmPickEvents.py --output=jetht16d '/JetHT/Run2016D-03Feb2017-v1/MINIAOD' events.txt)"
eval "$(edmPickEvents.py --output=jetht16e '/JetHT/Run2016E-03Feb2017-v1/MINIAOD' events.txt)"
eval "$(edmPickEvents.py --output=jetht16f '/JetHT/Run2016F-03Feb2017-v1/MINIAOD' events.txt)"
eval "$(edmPickEvents.py --output=jetht16g '/JetHT/Run2016G-03Feb2017-v1/MINIAOD' events.txt)"
eval "$(edmPickEvents.py --output=jetht16ha '/JetHT/Run2016H-03Feb2017_ver2-v1/MINIAOD' events.txt)"
eval "$(edmPickEvents.py --output=jetht16hb '/JetHT/Run2016H-03Feb2017_ver3-v1/MINIAOD' events.txt)"

