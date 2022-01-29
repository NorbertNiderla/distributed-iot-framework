#pragma once
#include <string>

class NeedToExitException : public std::exception{
    public:
        const char * what () const throw (){
            return "planned exit";
        }
};