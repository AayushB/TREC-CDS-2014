//
//  Query.cpp
//  Query
//
//  Created by Aayush Bhandari on 6/3/14.
//  Copyright (c) 2014 Aayush Bhandari. All rights reserved.
//

#include "Query.h"


Query::Query()
{
    phrase="";
}

void Query::setPhrase(string value)
{
    phrase=value;
}

void Query::addTerm(Term value)
{
    term.push_back(value);
}

string Query::getPhrase()
{
    return phrase;
}

vector<Term> Query::getTermVector()
{
    return term;
}