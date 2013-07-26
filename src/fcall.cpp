#include "fcall.h"
template <typename T>
T FunctionCall::parse(const std::string &s)
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
                //throw ParseException();
            }
            return ret;
        } else {
            //throw ParseException();
        }
};


template <typename T>
std::string FunctionCall::generate(const Message &m) {
        GenerateGrammar<std::back_insert_iterator<std::string> > g;
        std::string res;
        bool r;
        r = karma::generate(std::back_insert_iterator<std::string>(res), g, m);
        if (r) {
            return res;
        } else {
           // throw GenerateException();
        }
    } 
}


