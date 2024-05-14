#define revamp_cxx
#include "revamp.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include <sstream>
#include <iostream>
#include <string>

using namespace std;

void revamp::Loop(int id)
{
//   In a ROOT session, you can do:
//      root> .L revamp.C
//      root> revamp t
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

   bool fast_execute = true;// to make execution faster

   if(fast_execute)
   {
      max = 1;
      nentries=id;
   }

   int i, j; 
      
		string path = "", name[max];

      TFile* file = new TFile("distribution.root","Recreate");
      TH1I *hist10[max];
      //TCanvas *c1[20];

      for(j=0;j<max;j++){
            name[j] = "index"+to_string(j);//just to inititalize histograms
            hist10[j] = new TH1I(name[j].c_str(),";Number of 10-bit hits;Frequency",432,0,432);
      }
      for (Long64_t jentry=0; jentry<nentries;jentry++) 
      {
         Long64_t ientry = LoadTree(jentry);
         if (ientry < 0) break;
         nb = fChain->GetEntry(jentry);   nbytes += nb;
         // if (Cut(ientry) < 0) continue;

         
          

         for(j=0;j<max;j++){
            //name = "index"+to_string(j);
            //hist[j] = new TH1I(name.c_str(),";Number of 10-bit hits;Frequency",432,0,432);
            //c1[j] = new TCanvas("","",1920,1080);
         
            //name[j]="module_"+to_string((*layer)[j])+"_"+to_string((*type)[j])+"_"+to_string((*wafer_u)[j])+"_"+to_string((*wafer_v)[j]);

            name[j] ="module_"+to_string((*layer)[j])+"_"+to_string((*type)[j])+"_"+to_string((*wafer_u)[j])+"_"+to_string((*wafer_v)[j]);

            hist10[j]->Fill((*nhits10_defined)[j]);
            
            
            
         }
         }
      for (j=0;j<max;j++)
      {  //gStyle->SetOptStat(2222222);
         //gStyle->SetOptFit(111);
         hist10[j]->SetName(name[j].c_str());
         //cout<<name[j]<< endl;
         hist10[j]->Write();
         //c1[j]->Print(path.c_str());
      }
   file->Close();
}
