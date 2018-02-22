#!/bin/bash

echo -e '.x ~/decortication/macros/common.cc\n jetht->Scan("run:lumi:event:mavg_p:phi[0]:eta[0]:pt[0]:m_p[0]:phi[1]:eta[1]:pt[1]:m_p[1]", get_cut_modified("fjp_sig","Max$(tau42)<0.4&&Max$(tau43)<0.6"), "colsize=10")' | root -l ~/anatuples/anatuple_cutpt300eta20_pre.root | tee events_raw.txt
