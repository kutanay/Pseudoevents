#define makehistograms_cxx
#include "makehistograms.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include <sstream>
#include <iostream>
#include <string>

using namespace std;
// making a random test change to see stuff

void makehistograms::Loop(int id)
{
//   In a ROOT session, you can do:
//      root> .L makehistograms.C
//      root> makehistograms t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   
   int max =4500;

   bool fast_execute = false;// to make execution faster

   if(fast_execute)
   {
      max = 1;
      nentries=id;
   }

   int i, j; 
      
		string path = "", name10[max],name20[max],name30[max];

      string file = "/eos/user/t/takumar/hgcal/module_dist/CMSSW_13_3_0_pre5/src/histograms/distribution_"+to_string(id)+".root";

      TFile* f = new TFile(file.c_str(),"Recreate");

      TH1I *hist10[max], *hist20[max], *hist30[max];

      for(j=0;j<max;j++){
            name10[j] = "10_index"+to_string(j);//just to inititalize histograms
            hist10[j] = new TH1I(name10[j].c_str(),";Number of 10-bit hits;Frequency",432,0,432);
            name20[j] = "20_index"+to_string(j);//just to inititalize histograms
            hist20[j] = new TH1I(name20[j].c_str(),";Number of 20-bit hits;Frequency",432,0,432);
            name30[j] = "30_index"+to_string(j);//just to inititalize histograms
            hist30[j] = new TH1I(name30[j].c_str(),";Number of 30-bit hits;Frequency",432,0,432);
      }
      for (Long64_t jentry=nentries-50000; jentry<nentries;jentry++) 
      {
         Long64_t ientry = LoadTree(jentry);
         if (ientry < 0) break;
         nb = fChain->GetEntry(jentry);   nbytes += nb;
         // if (Cut(ientry) < 0) continue;

         for(j=0;j<max;j++){
            
            name10[j] ="module_10b_"+to_string((*layer)[j])+"_"+to_string((*type)[j])+"_"+to_string((*wafer_u)[j])+"_"+to_string((*wafer_v)[j]);
            name20[j] ="module_20b_"+to_string((*layer)[j])+"_"+to_string((*type)[j])+"_"+to_string((*wafer_u)[j])+"_"+to_string((*wafer_v)[j]);
            name30[j] ="module_30b_"+to_string((*layer)[j])+"_"+to_string((*type)[j])+"_"+to_string((*wafer_u)[j])+"_"+to_string((*wafer_v)[j]);

            hist10[j]->Fill((*nhits10_defined)[j]);
            hist20[j]->Fill((*nhits20_defined)[j]);
            hist30[j]->Fill((*nhits30_defined)[j]);
            
         }
         }
      for (j=0;j<max;j++)
      {  
         hist10[j]->SetName(name10[j].c_str());
         hist20[j]->SetName(name20[j].c_str());
         hist30[j]->SetName(name30[j].c_str());
         
         hist10[j]->Write();
         hist20[j]->Write();
         hist30[j]->Write();
         
      }
   f->Close();
}

