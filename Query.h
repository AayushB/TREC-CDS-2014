//
//  Concept.h
//  Query
//
//  Created by Aayush Bhandari on 6/3/14.
//  Copyright (c) 2014 Aayush Bhandari. All rights reserved.
//

#ifndef Query_Query_h
#define Query_Query_h
#include <iostream>
#include <string>
#include <vector>
using namespace std;

enum Concept{AAPP,ACAB,ACTY,AGGP,AMAS,AMPH,ANAB,ANIM,ANST,ANTB,ARCH,BACS,BACT,BDSU,BDSY,BHVR,BIOF,BIRD,BLOR,BMOD,BODM,BPOC,BSOJ,CARB,CELC,CELF,CELL,CGAB,CHEM,CHVF,CHVS,CLAS,CLNA,CLND,CNCE,COMD,CRBS,DIAP,DORA,DRDD,DSYN,EDAC,EEHU,EICO,ELII,EMOD,EMST,ENTY,ENZY,EUKA,EVNT,FAMG,FFAS,FISH,FNDG,FNGS,FOOD,FTCN,GENF,GEOA,GNGM,GORA,GRPA,GRUP,HCPP,HCRO,HLCA,HOPS,HORM,HUMN,IDCN,IMFT,INBE,INCH,INPO,INPR,IRDA,LANG,LBPR,LBTR,LIPD,MAMM,MBRT,MCHA,MEDD,MENP,MNOB,MOBD,MOFT,MOSQ,NEOP,NNON,NPOP,NSBA,NUSQ,OCAC,OCDI,OPCO,ORCH,ORGA,ORGF,ORGM,ORGT,ORTF,PATF,PHOB,PHPR,PHSF,PHSU,PLNT,PODG,POPG,PROG,PROS,QLCO,QNCO,RCPT,REPT,RESA,RESD,RNLW,SBST,SHRO,SOCB,SOSY,SPCO,STRD,TISU,TMCO,TOPP,VIRS,VITA,VTBT,OTHR
    
};
struct Term
{
    Concept concept;
    string expansion;
    string selectedPhrase;
    double weight;
};

class Query
{
public:
    Query();
    void setPhrase(string value);
    void addTerm(Term value);
    string getPhrase();
    vector<Term> getTermVector();
    
private:
    string phrase;
    vector<Term> term;
};




#endif
