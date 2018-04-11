#pragma once
#include "common.h"

class graphUtils
{
public:
    static string getCanonicalName(string s,
                          const map<string,string>&
                          variableName2variableName)
    {
        while(variableName2variableName.find(s) !=
              variableName2variableName.end())
        {
            auto it = variableName2variableName.find(s);
            if(s == it->second)
                break;
            s = it->second ;
        }
        return s;
    }
};
