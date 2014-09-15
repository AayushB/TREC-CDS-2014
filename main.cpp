
/** Note comment/uncomment the lines below *^* for switching between Aayush Computer and Mahurti
 */
/**
 * Statement of Purpose:
 *
 * Aayush Bhandari
 * aayushb@mail.sfsu.edu
 * Xcode Version 4.6.2 (4H1003)
 * February 11th, 2013
 * Copyright (c) 2014 Aayush Bhandari. All rights reserved.
 *
 * Description of the Program's Input and Output:
 *
 * Description of How to Use the Program:
 *
 * Assumptions (such as the type of data expected):
 *
 * Brief Description of the Major Classes and Structs:
 */


#include "Query.h"
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <vector>
#include "StopWords.h"

/*-----------------------------------------------------------------------------------
 Function Declarations
 -----------------------------------------------------------------------------------*/


void extractMetaMapping(vector<Query>& tokenizedData);
void extractNegatedTerms(vector<string> & negatedPhrases);
void printWithStructure(string stringValue, double weight);
void printWithoutStructure(string stringValue, double weight);
void printMesh(string meshResult);

bool isASymbol(char value);
string MESH(string value, double weight);
string removeCase(string &rawQuery);
int numberOfWords(string value);



/*-----------------------------------------------------------------------------------
 Main Program
 -----------------------------------------------------------------------------------*/


int main(int argc, const char * argv[])
{
    //Tokenize phrases from meta map
    vector<Query> tokenizedData;
    extractMetaMapping(tokenizedData);
    
    //Tokenize negated terms
    vector<string> negatedPhrases;
    extractNegatedTerms(negatedPhrases);
    

    
    cout << "#filreq(" << endl;
    
    
    cout << "#syn(" << endl; // begin syn
    for (int i=0; i<tokenizedData.size(); i++) // Go through all the tokenized Meta Mappings
    {
        for (int j=0; j<tokenizedData.at(i).getTermVector().size(); j++) // Go through individual concepts in each Meta Mappings
        {
            

            Concept currConcept=tokenizedData.at(i).getTermVector().at(j).concept;
            string currSelectedPhrase= tokenizedData.at(i).getTermVector().at(j).selectedPhrase;
            
            
            //-----------------------------------------------------------------------------------------------------------
            //                                            Skip List / Negated terms/ vitals
            //-----------------------------------------------------------------------------------------------------------
        
            //cat sample.txt.out |grep "Negated  Concept"|cut -f2-10 -d:| tr ',' '\n'| sed -e 's/^[ \t]*//'| cut -f2 -d:
        
            // cout << "." << currSelectedPhrase << "*";
        
            bool negate=false;
            for (int k=0; k<negatedPhrases.size(); k++)
            {
                if(negatedPhrases.at(k)+" " ==currSelectedPhrase) // skipe negated words
                {
                negate=true;
                }
            }
        
            if (currSelectedPhrase=="30S " || currSelectedPhrase=="20S " || currSelectedPhrase=="suffered " || currSelectedPhrase=="heart rate "  || currSelectedPhrase=="*Temperature " || currSelectedPhrase=="Pulse Rate " || currSelectedPhrase=="Respiratory rate " || currSelectedPhrase=="Body Temperature " || currSelectedPhrase=="Blood Pressure " || currSelectedPhrase=="Treatment " || currSelectedPhrase=="Test " || negate)
            {
                continue; // skips the word, includes vitals and negations
            }
            
            //-----------------------------------------------------------------------------------------------------------
            
            //-----------------------------------------------------------------------------------------------------------
            //                                            Case: Filter Required
            //-----------------------------------------------------------------------------------------------------------
            else if (currConcept== SOSY || currConcept==DSYN  || currConcept==BPOC
                     ||currConcept==BLOR || currConcept==HOPS || currConcept==AAPP || currConcept==OTHR ||
                     currConcept==PHSU || currConcept==ORCH || currConcept==INCH|| currConcept==TISU || currConcept==CELL
                     || currConcept==MOBD ||currConcept==PATF || currConcept==FNDG ||currConcept==PROG || currConcept==CLNA)
            {
                printWithStructure(currSelectedPhrase, 0);
            }

        }
    }
    cout << ")" << endl; // end syn
    
    
    
    
    cout << "#weight(" << endl; // sorround everything with weight
    
    
    for (int i=0; i<tokenizedData.size(); i++) // Go through all the tokenized Meta Mappings
    {
        for (int j=0; j<tokenizedData.at(i).getTermVector().size(); j++) // Go through individual concepts in each Meta Mappings
        {
            Concept currConcept=tokenizedData.at(i).getTermVector().at(j).concept;
            string currSelectedPhrase= tokenizedData.at(i).getTermVector().at(j).selectedPhrase;
            string currExpansion=tokenizedData.at(i).getTermVector().at(j).expansion;
            double currWeight=tokenizedData.at(i).getTermVector().at(j).weight;
            
            
            
            //-----------------------------------------------------------------------------------------------------------
            //                                            Skip List / Negated terms/vitals
            //-----------------------------------------------------------------------------------------------------------
            
            //cat sample.txt.out |grep "Negated  Concept"|cut -f2-10 -d:| tr ',' '\n'| sed -e 's/^[ \t]*//'| cut -f2 -d:
            
           // cout << "." << currSelectedPhrase << "*";
            
            bool negate=false;
            for (int k=0; k<negatedPhrases.size(); k++)
            {
                if(negatedPhrases.at(k)+" " ==currSelectedPhrase) // skipe negated words
                {
                    negate=true;
                }
            }

            if (currSelectedPhrase=="30S " || currSelectedPhrase=="20S " || currSelectedPhrase=="suffered " || currSelectedPhrase=="heart rate "  || currSelectedPhrase=="*Temperature " || currSelectedPhrase=="Pulse Rate " || currSelectedPhrase=="Respiratory rate " || currSelectedPhrase=="Body Temperature " || currSelectedPhrase=="Blood Pressure " || currSelectedPhrase=="Treatment " || currSelectedPhrase=="Test " || negate)
            {
                continue; // skips the word
            }
            
            //-----------------------------------------------------------------------------------------------------------
            
            
            
            //-----------------------------------------------------------------------------------------------------------
            //                                            Case 1: Sign or Symptoms
            //-----------------------------------------------------------------------------------------------------------
            else if (currConcept== SOSY)
            {
                string meshResult=MESH(tokenizedData.at(i).getTermVector().at(j).selectedPhrase, 3);//include MESH for SOSY
                printWithStructure(currSelectedPhrase, currWeight);
                printWithStructure(currExpansion, currWeight);
                printMesh(meshResult);
            }
            
            //-----------------------------------------------------------------------------------------------------------
            //                                     Case 2: Include both Phrase and Expansion
            //-----------------------------------------------------------------------------------------------------------
            
            else if (currConcept== ANAB || currConcept==BDSU || currConcept==CELC ||currConcept==CELL
                      || currConcept== PATF || currConcept==MNOB || currConcept==TOPP)
            {
                printWithStructure(currSelectedPhrase, currWeight);
                printWithStructure(currExpansion, currWeight/2);
            }
            
            //-----------------------------------------------------------------------------------------------------------
            //                                     Case 3: Include only Phrase
            //-----------------------------------------------------------------------------------------------------------
            
            else if (currWeight>0)
            {
                printWithStructure(currSelectedPhrase, currWeight);
            }

            
            //-----------------------------------------------------------------------------------------------------------
            //                                            Case 4: Exclude the remaning
            //-----------------------------------------------------------------------------------------------------------
            
            else
            {
                //don't print
            }
        }
        
        
    }
    
    cout << ")\n)"; // close the weight and filreq
    
    return 0;
}


/*-----------------------------------------------------------------------------------
 Function Definitions
 -----------------------------------------------------------------------------------*/
void extractNegatedTerms(vector<string> & negatedPhrases)
{
    /// tokenizing negated terms
    ifstream file1;
    //*^*
    //file1.open("/Users/Aayush/Desktop/negation.txt");
     file1.open("negation.tmp");
    
    if (file1.fail())
    {
        cout << "negation file could not be opened";
    }
    
    string str ="";
    
    while ( getline(file1, str))
    {
        negatedPhrases.push_back(str);
    }
    
    file1.close();
    
    // tokenizng negated terms done
}



void printMesh(string meshResult)
{
    if(meshResult!="")
    {
        cout << removeCase(meshResult);
    }
}



void printWithStructure(string stringValue, double weight)
{
    string cleanString=stringValue;
    cleanString.erase(remove_if(cleanString.begin(), cleanString.end(), isASymbol), cleanString.end());
    int totalWords=numberOfWords(cleanString);

    if(totalWords==1) // does not structure
    {
       /*
        if(weight!=0)
            cout << fixed << setprecision(1)<< weight << " ";
        cout << removeCase(cleanString) << ".abstract" << endl;
        if(weight!=0)
            cout << fixed << setprecision(1)<< weight << " ";
        cout << removeCase(cleanString) << ".body" << endl;
        */
        printWithoutStructure(stringValue, weight);
    }
    
    else if(totalWords > 1 && totalWords < 5) // only structures more than 1 word, and less than 4 words
    {
        if(weight!=0)
        {
            cout << fixed << setprecision(1)<< weight << " ";
            cout << "#uw5(" << removeCase(cleanString) << ").abstract" << endl;
            cout << fixed << setprecision(1)<< weight << " ";
            cout << "#uw5(" << removeCase(cleanString) << ").body" << endl;
        }
        else
        {
            cout << "#1(" << removeCase(cleanString) << ").abstract" << endl;
            cout << "#1(" << removeCase(cleanString) << ").body" << endl;
        }
    }
    else
    {
        printWithoutStructure(stringValue, weight);
    }
}

void printWithoutStructure(string stringValue, double weight)
{
    string cleanString=stringValue;
    //remove all the symbols from the string
    cleanString.erase(remove_if(cleanString.begin(), cleanString.end(), isASymbol), cleanString.end());
    //remove all the cases from the string
    cleanString=removeCase(cleanString);
    
    
    //-----------------------------------------------------------------
    //                  Tokenizing the string to remove stopWords
    //-----------------------------------------------------------------
    vector<string> tokenizedCleanString;
    istringstream iss(cleanString);
    do
    {
        string sub;
        iss >> sub;
        tokenizedCleanString.push_back(sub);
    } while (iss);
    
    //-----------------------------------------------------------------
    //                 Printing without Stop Words
    //-----------------------------------------------------------------
    
    for(int i=0 ; i<tokenizedCleanString.size();i++)
    {
        bool isAStopWord=false;
        for (int j=0; j<stopwords.size();j++)
        {
            
            
            if (removeCase(tokenizedCleanString.at(i))==stopwords.at(j) || tokenizedCleanString.at(i)=="")
                // "" is sometimes tokenized, hence treated as a stopword
            {
                isAStopWord=true;
            }
        }
        if(isAStopWord==false)
        {
            if(weight!=0)
                cout << std::fixed << std::setprecision(1)<< weight << " ";
            cout << tokenizedCleanString.at(i) << ".abstract" << endl;
            if(weight!=0)
                cout << std::fixed << std::setprecision(1)<< weight << " ";
            cout << tokenizedCleanString.at(i) << ".body" << endl;
        }
    }
}


void extractMetaMapping(vector<Query>& tokenizedData)  // Very Sensitive- Do not edit except the highlighted edit group
{
    ifstream file1;
    //*^*
    //file1.open("/Users/Aayush/Desktop/results.txt");
     file1.open("meta.tmp");
    
    if (file1.fail())
    {
        cout << "meta file could not be opened";
    }
    
    string str;
    string str2;
    string selectedPhrase="";
    string concept="";
    string expansion="";
    int mode=0;
    bool looped=false;
    
    Query* tempQuery;
    Term tempTerm;
    
    while ( getline(file1, str))
    {
        if(looped)
        {
            getline(file1, str);
        }
        
        long foundANo = removeCase(str).find(" no "); // line that ignores phrases with a "no"
        
        tempQuery= new Query();
        str.erase(remove_if(str.begin(), str.end(), isASymbol), str.end());
        tempQuery->setPhrase(str);
        
        while( str2!="@" && file1 >> str2)
        {
            if(str2=="(") // recognizes the beginning of an expansion
            {
                mode=1;
            }
            if (str2=="[") // recognizes end of expansion and beginning of concept
            {
                mode=2;
            }
            if (str2=="]")  // recognizes end of concept
            {
                mode=0;
                selectedPhrase.erase(remove_if(selectedPhrase.begin(),selectedPhrase.end(), isASymbol), selectedPhrase.end());
                if(selectedPhrase!="" && selectedPhrase.at(0)==' ')
                {
                    selectedPhrase.replace(0,1,"");
                }
                concept.erase(remove_if(concept.begin(),concept.end(), isASymbol), concept.end());
                expansion.erase(remove_if(expansion.begin(),expansion.end(), isASymbol), expansion.end());
                
                
                //Fixing spacing in parsing
                //---------------------------
                if(expansion!="" && expansion.at(0)==' ')
                {
                    expansion.replace(0,1,"");
                }
                if(expansion!="" && expansion.at(expansion.length()-1)==' ')
                {
                    expansion.replace(expansion.length()-2,1,"");
                }
                //-------------------------------
                
                
                /*
                 * Edit this for adding more semantic Group
                 *------------------------------------------
                 *------------------------------------------
                 */
                
                if(concept==" Amino Acid Peptide or Protein ")
                {
                    tempTerm.concept=AAPP;
                    tempTerm.weight=5;
                }
                else if(concept==" Acquired Abnormality ")
                {
                    tempTerm.concept=ACAB;
                    tempTerm.weight=5;
                }
                else if(concept==" Activity ")
                {
                    tempTerm.concept=ACTY;
                    tempTerm.weight=0;
                }
                else if(concept==" Age Group ")
                {
                    tempTerm.concept=AGGP;
                    tempTerm.weight=0;
                }
                else if(concept==" Amino Acid Sequence ")
                {
                    tempTerm.concept=AMAS;
                    tempTerm.weight=5;
                }
                else if(concept==" Amphibian ")
                {
                    tempTerm.concept=AMPH;
                    tempTerm.weight=5;
                }
                else if(concept==" Anatomical Abnormality ")
                {
                    tempTerm.concept=ANAB;
                    tempTerm.weight=5;
                }
                else if(concept==" Animal ")
                {
                    tempTerm.concept=ANIM;
                    tempTerm.weight=5;
                }
                else if(concept==" Antibiotic ")
                {
                    tempTerm.concept=ANTB;
                    tempTerm.weight=5;
                }
                else if(concept==" Archaeon ")
                {
                    tempTerm.concept=ARCH;
                    tempTerm.weight=5;
                }
                else if(concept==" Biologically Active Substance ")
                {
                    tempTerm.concept=BACS;
                    tempTerm.weight=5;
                }
                else if(concept==" Bacterium ")
                {
                    tempTerm.concept=BACT;
                    tempTerm.weight=5;
                }
                else if(concept==" Body Substance ")
                {
                    tempTerm.concept=BDSU;
                    tempTerm.weight=5;
                }
                else if(concept==" Body System ")
                {
                    tempTerm.concept=BDSY;
                    tempTerm.weight=5;
                }
                else if(concept==" Behavior ")
                {
                    tempTerm.concept=BHVR;
                    tempTerm.weight=5;
                }
                else if(concept==" Biologic Function ")
                {
                    tempTerm.concept=BIOF;
                    tempTerm.weight=5;
                }
                else if(concept==" Bird ")
                {
                    tempTerm.concept=BIRD;
                    tempTerm.weight=5;
                }
                else if(concept==" Body Location or Region ") // done
                {
                    tempTerm.concept=BLOR;
                    tempTerm.weight=15;
                }
                else if(concept==" Biomedical Occupation or Discipline ")
                {
                    tempTerm.concept=BMOD;
                    tempTerm.weight=5;
                }
                else if(concept==" Body Part Organ or Organ Component ")  //done
                {
                    tempTerm.concept=BPOC;
                    tempTerm.weight=15;
                }
                else if(concept==" Body Space or Junction ")
                {
                    tempTerm.concept=BSOJ;
                    tempTerm.weight=7;
                }
                else if(concept==" Carbohydrate ")
                {
                    tempTerm.concept=CARB;
                    tempTerm.weight=5;
                }
                else if(concept==" Cell Component ")
                {
                    tempTerm.concept=CELC;
                    tempTerm.weight=5;
                }
                else if(concept==" Cell ")
                {
                    tempTerm.concept=CELL;
                    tempTerm.weight=5;
                }
                else if(concept==" Congenital Abnormality ")
                {
                    tempTerm.concept=CGAB;
                    tempTerm.weight=5;
                }
                else if(concept==" Chemical ")
                {
                    tempTerm.concept=CHEM;
                    tempTerm.weight=5;
                }
                else if(concept==" Chemical Viewed Functionally ")
                {
                    tempTerm.concept=CHVF;
                    tempTerm.weight=5;
                }
                else if(concept==" Chemical Viewed Structurally ")
                {
                    tempTerm.concept=CHVS;
                    tempTerm.weight=7;
                }
                else if(concept==" Classification ")
                {
                    tempTerm.concept=CLAS;
                    tempTerm.weight=0;
                }
                else if(concept==" Clinical Attribute ") // chest x -ray
                {
                    tempTerm.concept=CLNA;
                    tempTerm.weight=5.5;
                }
                else if(concept==" Clinical Drug ")
                {
                    tempTerm.concept=CLND;
                    tempTerm.weight=5;
                }
                else if(concept==" Conceptual Entity ")
                {
                    tempTerm.concept=CNCE;
                    tempTerm.weight=0;
                }
                else if(concept==" Cell or Molecular Dysfunction ")
                {
                    tempTerm.concept=COMD;
                    tempTerm.weight=5;
                }
                else if(concept==" Carbohydrate Sequence ")
                {
                    tempTerm.concept=CRBS;
                    tempTerm.weight=5;
                }
                else if(concept==" Diagnostic Procedure ")  //done
                {
                    tempTerm.concept=DIAP;
                    tempTerm.weight=3;
                }
                else if(concept==" Daily or Recreational Activity ")
                {
                    tempTerm.concept=DORA;
                    tempTerm.weight=2;
                }
                else if(concept==" Drug Delivery Device ")
                {
                    tempTerm.concept=DRDD;
                    tempTerm.weight=0;
                }
                else if(concept==" Disease or Syndrome ")  //done
                {
                    tempTerm.concept=DSYN;
                    tempTerm.weight=7;
                }
                else if(concept==" Educational Activity ")
                {
                    tempTerm.concept=EDAC;
                    tempTerm.weight=2;
                }
                else if(concept==" Environmental Effect of Humans ")
                {
                    tempTerm.concept=EEHU;
                    tempTerm.weight=0;
                }
                else if(concept==" Eicosanoid ")
                {
                    tempTerm.concept=EICO;
                    tempTerm.weight=5;
                }
                else if(concept==" Element Ion or Isotope ")
                {
                    tempTerm.concept=ELII;
                    tempTerm.weight=5;
                }
                else if(concept==" Experimental Model of Disease ")
                {
                    tempTerm.concept=EMOD;
                    tempTerm.weight=5;
                }
                else if(concept==" Embryonic Structure ")
                {
                    tempTerm.concept=EMST;
                    tempTerm.weight=5;
                }
                else if(concept==" Entity ")
                {
                    tempTerm.concept=ENTY;
                    tempTerm.weight=5;
                }
                else if(concept==" Enzyme ")
                {
                    tempTerm.concept=ENZY;
                    tempTerm.weight=5;
                }
                else if(concept==" Eukaryote ")
                {
                    tempTerm.concept=EUKA;
                    tempTerm.weight=5;
                }
                else if(concept==" Event ")
                {
                    tempTerm.concept=EVNT;
                    tempTerm.weight=0;
                }
                else if(concept==" Family Group ")
                {
                    tempTerm.concept=FAMG;
                    tempTerm.weight=0;
                }
                else if(concept==" Fully Formed Anatomical Structure ")
                {
                    tempTerm.concept=FFAS;
                    tempTerm.weight=5;
                }
                else if(concept==" Fish ")
                {
                    tempTerm.concept=FISH;
                    tempTerm.weight=5;
                }
                else if(concept==" Finding ")  //done
                {
                    tempTerm.concept=FNDG;
                    tempTerm.weight=10;
                }
                else if(concept==" Fungus ")
                {
                    tempTerm.concept=FNGS;
                    tempTerm.weight=5;
                }
                else if(concept==" Food ")
                {
                    tempTerm.concept=FOOD;
                    tempTerm.weight=5;
                }
                else if(concept==" Functional Concept ")   //done
                {
                    tempTerm.concept=FTCN;
                    tempTerm.weight=6;
                }
                else if(concept==" Genetic Function ")
                {
                    tempTerm.concept=GENF;
                    tempTerm.weight=3;
                }
                else if(concept==" Geographic Area ")
                {
                    tempTerm.concept=GEOA;
                    tempTerm.weight=0;
                }
                else if(concept==" Gene or Genome ")
                {
                    tempTerm.concept=GNGM;
                    tempTerm.weight=0;
                }
                else if(concept==" Governmental or Regulatory Activity ")
                {
                    tempTerm.concept=GORA;
                    tempTerm.weight=0;
                }
                else if(concept==" Group Attribute ")
                {
                    tempTerm.concept=GRPA;
                    tempTerm.weight=0;
                }
                else if(concept==" Group ")
                {
                    tempTerm.concept=GRUP;
                    tempTerm.weight=0;
                }
                else if(concept==" Human-caused Phenomenon or Process ")
                {
                    tempTerm.concept=HCPP;
                    tempTerm.weight=3;
                }
                else if(concept==" Health Care Related Organization ")  //done
                {
                    tempTerm.concept=HCRO;
                    tempTerm.weight=7;
                }
                else if(concept==" Health Care Activity ")
                {
                    tempTerm.concept=HLCA;
                    tempTerm.weight=4;
                }
                else if(concept==" Hazardous or Poisonous Substance ")
                {
                    tempTerm.concept=HOPS;
                    tempTerm.weight=7;
                }
                else if(concept==" Hormone ")
                {
                    tempTerm.concept=HORM;
                    tempTerm.weight=5;
                }
                else if(concept==" Human ")
                {
                    tempTerm.concept=HUMN;
                    tempTerm.weight=0;
                }
                else if(concept==" Idea or Concept ")
                {
                    tempTerm.concept=IDCN;
                    tempTerm.weight=0;
                }
                else if(concept==" Immunologic Factor ")
                {
                    tempTerm.concept=IMFT;
                    tempTerm.weight=5;
                }
                else if(concept==" Individual Behavior ")
                {
                    tempTerm.concept=INBE;
                    tempTerm.weight=4;
                }
                else if(concept==" Inorganic Chemical ")
                {
                    tempTerm.concept=INCH;
                    tempTerm.weight=5;
                }
                else if(concept==" Injury or Poisoning ")
                {
                    tempTerm.concept=INPO;
                    tempTerm.weight=5;
                }
                else if(concept==" Intellectual Product ")
                {
                    tempTerm.concept=INPR;
                    tempTerm.weight=3;
                }
                else if(concept==" Indicator Reagent or Diagnostic Aid ")
                {
                    tempTerm.concept=IRDA;
                    tempTerm.weight=4;
                }
                else if(concept==" Language ")
                {
                    tempTerm.concept=LANG;
                    tempTerm.weight=0;
                }
                else if(concept==" Laboratory Procedure ")
                {
                    tempTerm.concept=LBPR;
                    tempTerm.weight=4;
                }
                else if(concept==" Laboratory or Test Result ")
                {
                    tempTerm.concept=LBTR;
                    tempTerm.weight=4;
                }
                else if(concept==" Lipid ")
                {
                    tempTerm.concept=LIPD;
                    tempTerm.weight=5;
                }
                else if(concept==" Mammal ")
                {
                    tempTerm.concept=MAMM;
                    tempTerm.weight=5;
                }
                else if(concept==" Molecular Biology Research Technique ")
                {
                    tempTerm.concept=MBRT;
                    tempTerm.weight=0;
                }
                else if(concept==" Machine Activity ")
                {
                    tempTerm.concept=MCHA;
                    tempTerm.weight=0;
                }
                else if(concept==" Medical Device ")
                {
                    tempTerm.concept=MEDD;
                    tempTerm.weight=0;
                }
                else if(concept==" Mental Process ")
                {
                    tempTerm.concept=MENP;
                    tempTerm.weight=3;
                }
                else if(concept==" Manufactured Object ")
                {
                    tempTerm.concept=MNOB;
                    tempTerm.weight=3;
                }
                else if(concept==" Mental or Behavioral Dysfunction ")
                {
                    tempTerm.concept=MOBD;
                    tempTerm.weight=5;
                }
                else if(concept==" Molecular Function ")
                {
                    tempTerm.concept=MOFT;
                    tempTerm.weight=3;
                }
                else if(concept==" Neoplastic Process ")
                {
                    tempTerm.concept=NEOP;
                    tempTerm.weight=6;
                }
                else if(concept==" Nucleic Acid Nucleoside or Nucleotide ")
                {
                    tempTerm.concept=NNON;
                    tempTerm.weight=6;
                }
                else if(concept==" Natural Phenomenon or Process ")
                {
                    tempTerm.concept=NPOP;
                    tempTerm.weight=0;
                }
                else if(concept==" Neuroreactive Substance or Biogenic Amine ")
                {
                    tempTerm.concept=NSBA;
                    tempTerm.weight=5;
                }
                else if(concept==" Nucleotide Sequence ")
                {
                    tempTerm.concept=NUSQ;
                    tempTerm.weight=5;
                }
                else if(concept==" Occupational Activity ")
                {
                    tempTerm.concept=OCAC;
                    tempTerm.weight=0;
                }
                else if(concept==" Occupation or Discipline ")
                {
                    tempTerm.concept=OCDI;
                    tempTerm.weight=0;
                }
                else if(concept==" Organophosphorus Compound ")
                {
                    tempTerm.concept=OPCO;
                    tempTerm.weight=5;
                }
                else if(concept==" Organic Chemical ")
                {
                    tempTerm.concept=ORCH;
                    tempTerm.weight=5;
                }
                else if(concept==" Organism Attribute ")
                {
                    tempTerm.concept=ORGA;
                    tempTerm.weight=3;
                }
                else if(concept==" Organism Function ")
                {
                    tempTerm.concept=ORGF;
                    tempTerm.weight=3;
                }
                else if(concept==" Organism ")
                {
                    tempTerm.concept=ORGM;
                    tempTerm.weight=5;
                }
                else if(concept==" Organization ")
                {
                    tempTerm.concept=ORGT;
                    tempTerm.weight=0;
                }
                else if(concept==" Organ or Tissue Function ")
                {
                    tempTerm.concept=ORTF;
                    tempTerm.weight=5;
                }
                else if(concept==" Pathologic Function ")
                {
                    tempTerm.concept=PATF;
                    tempTerm.weight=5;
                }
                else if(concept==" Physical Object ")
                {
                    tempTerm.concept=PHOB;
                    tempTerm.weight=4;
                }
                else if(concept==" Phenomenon or Process ")
                {
                    tempTerm.concept=PHPR;
                    tempTerm.weight=5;
                }
                else if(concept==" Physiologic Function ")
                {
                    tempTerm.concept=PHSF;
                    tempTerm.weight=5;
                }
                else if(concept==" Pharmacologic Substance ")
                {
                    tempTerm.concept=PHSU;
                    tempTerm.weight=5;
                }
                else if(concept==" Plant ")
                {
                    tempTerm.concept=PLNT;
                    tempTerm.weight=5;
                }
                else if(concept==" Patient or Disabled Group ")
                {
                    tempTerm.concept=PODG;
                    tempTerm.weight=0;
                }
                else if(concept==" Population Group ")
                {
                    tempTerm.concept=POPG;
                    tempTerm.weight=0;
                }
                else if(concept==" Professional or Occupational Group ")
                {
                    tempTerm.concept=PROG;
                    tempTerm.weight=5;
                }
                else if(concept==" Professional Society ")
                {
                    tempTerm.concept=PROS;
                    tempTerm.weight=0;
                }
                else if(concept==" Qualitative Concept ")  //done
                {
                    tempTerm.concept=QLCO;
                    tempTerm.weight=4;
                }
                else if(concept==" Quantitative Concept ")
                {
                    tempTerm.concept=QNCO;
                    tempTerm.weight=0;
                }
                else if(concept==" Receptor ")
                {
                    tempTerm.concept=RCPT;
                    tempTerm.weight=5;
                }
                else if(concept==" Reptile ")
                {
                    tempTerm.concept=REPT;
                    tempTerm.weight=5;
                }
                else if(concept==" Research Activity ")
                {
                    tempTerm.concept=RESA;
                    tempTerm.weight=1;
                }
                else if(concept==" Research Device ")
                {
                    tempTerm.concept=RESD;
                    tempTerm.weight=1;
                }
                else if(concept==" Regulation or Law ")
                {
                    tempTerm.concept=RNLW;
                    tempTerm.weight=0;
                }
                else if(concept==" Substance ")
                {
                    tempTerm.concept=SBST;
                    tempTerm.weight=5;
                }
                else if(concept==" Self-help or Relief Organization ")
                {
                    tempTerm.concept=SHRO;
                    tempTerm.weight=5;
                }
                else if(concept==" Social Behavior ")
                {
                    tempTerm.concept=SOCB;
                    tempTerm.weight=2.5;
                }
                else if(concept==" Sign or Symptom ")  //done
                {
                    tempTerm.concept=SOSY;
                    tempTerm.weight=10;
                }
                else if(concept==" Spatial Concept ")  //done
                {
                    tempTerm.concept=SPCO;
                    tempTerm.weight=0;
                }
                else if(concept==" Steroid ")
                {
                    tempTerm.concept=STRD;
                    tempTerm.weight=5;
                }
                else if(concept==" Tissue ")
                {
                    tempTerm.concept=TISU;
                    tempTerm.weight=5;
                }
                else if(concept==" Temporal Concept ")
                {
                    tempTerm.concept=TMCO;
                    tempTerm.weight=0;
                }
                else if(concept==" Therapeutic or Preventive Procedure ")  //done-semi
                {
                    tempTerm.concept=TOPP;
                    tempTerm.weight=7.5;
                }
                else if(concept==" Virus ")
                {
                    tempTerm.concept=VIRS;
                    tempTerm.weight=5;
                }
                else if(concept==" Vitamin ")
                {
                    tempTerm.concept=VITA;
                    tempTerm.weight=5;
                }
                else if(concept==" Vertebrate ")
                {
                    tempTerm.concept=VTBT;
                    tempTerm.weight=5;
                }
                else
                {
                    tempTerm.concept=OTHR;
                    tempTerm.weight=5;
                }
                //--------------------------------------------
                //--------------------------------------------
                //--------------------------------------------
                
                
                tempTerm.expansion=expansion;
                tempTerm.selectedPhrase=selectedPhrase;
                
                tempQuery->addTerm(tempTerm);
                
                //cout << selectedPhrase  << " " << concept << endl;
                selectedPhrase="";
                concept="";
                expansion="";
                str2="";
                
                
                
            }
            
            //Seperating phrase, concepts and expansion with spaces
            if(mode==0)
            {
                selectedPhrase+=str2+" ";
            }
            
            if (mode==1)
            {
                expansion+=str2+" ";
            }
            
            if (mode==2)
            {
                concept+=str2+" ";
            }
        }
        
        
        selectedPhrase="";
        concept="";
        expansion="";
        str2="";
        mode=0;
        looped=true;
        if (foundANo==std::string::npos) // line that ignores phrases with a "no"
        {
        tokenizedData.push_back(*tempQuery);
        }
    }
    
    file1.close();
}



int numberOfWords(string value)
{
    
    /*
     * Precondition:
     * None
     *
	 * Purpose:
	 * The function accepts a strings as an input and returns the numbers of words in it
     * by counting the total number of spaces
	 *
	 * Postcondition:
     * The total number of words in the given string is returned
	 *
	 */
    
    int numWords=0;
    for (int i=0; i<value.length(); i++)
    {
        if(value.at(i)==' ')
        {
            numWords++;
        }
    }
    
    return numWords;
}

string removeCase(string &rawQuery)
{
    
    /*
     * Precondition:
     * None
     *
	 * Purpose:
	 * The function accepts a raw query as reference and changes each character in the string
     * into a lowered case version.
	 *
	 * Postcondition:
     * The rawQuery is changed in the main program and turned into all lowered case.
	 *
	 */
    for (int i=0; i<rawQuery.length(); i++)
    {
        string temp(1,tolower(rawQuery.at(i))); // converting char into lowerd case string
        rawQuery.replace(i,1,temp);
    }
    
    if(rawQuery!="" && rawQuery.at(rawQuery.length()-1)==' ')
    {
        rawQuery.replace(rawQuery.length()-1,1,"");
    }
    
    return rawQuery;
}


string MESH(string value, double weight)
{
    //This is to convert weight into string
    std::ostringstream strs;
    strs << fixed << setprecision(1) << weight;
    std::string weightString = strs.str();
    //*^*
    string command="./Mesh.sh " + value + " >output.tmp";
    //string command="/Users/Aayush/Desktop/Research/Project2/scripts/MESH/Mesh.sh " + value + " >output.tmp";
    system(command.c_str());
    ifstream file2;
    file2.open("output.tmp");
    if(file2.fail())
    {
        cout << "mesh file fail open";
    }
    
    string str;
    string result;
    while(getline(file2, str))
    {
        if(numberOfWords(str)<1) // If the string contains only one word then no need to add any structure
        {
            result+=weightString + " "+str+".abstract\n";
            result+=weightString + " "+str+".body\n";
        }
        else  // we add an unordered structure
        {
        result+=weightString + " #uw5("+str+").abstract\n";
        result+=weightString + " #uw5("+str+").body\n";
        }
    }
    file2.close();
    system("rm output.tmp");
    return result;
}


bool isASymbol(char value)
{
    /*
     * Precondition:
     * None
     *
	 * Purpose:
	 * The function accepts a char and evaluates if its one of the 26 english alphabets, single quote, numbers
     * or a space.
	 *
	 * Postcondition:
     * A true value is returned for english alphabets, single quote or spaces. A false is returned otherwise.
	 *
	 */
    
    int x= value;
    
    if(x==47)//will include slash
    {
        return false;
    }
    if((x>=65 && x <=90)|| (x >=97 && x<=122) || (x>=48 && x<=57) || x==32 || x==39)
        // using the ASCII value for comparison for english alphabets, numbers apostrophe and space, hyphens
    {
        return false;
    }
    return true;
}


