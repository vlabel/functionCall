#ifndef FCALC_H
#define FCALC_H

#include "grammars.h" 
#include <string>

template <typename T>
class FunctionCall {
  public:
T parse(const std::string &s)
{
        Message res;
        T ret;
        ParseGrammar<std::string::const_iterator> g;
        std::string::const_iterator it = s.begin();
        bool r = qi::parse(it, s.end(), g, res) && (it == s.end());
        if (r) {
            try {
                ret = boost::get<T>(res);
            }
            catch (boost::bad_get) {
            }
            return ret;
        } 
      	//throw smth 
}



std::string generate(const Message &m)
{
    GenerateGrammar<std::back_insert_iterator<std::string> > g;
    std::string res;
    bool r;
    r = karma::generate(std::back_insert_iterator<std::string>(res), g, m);
    if (r) {
         return res;
    } else {
         // throw GenerateException();
    }
		return res;
	} 
};




#endif /* fcalc */
