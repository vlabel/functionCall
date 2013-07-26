#ifndef GRAMMAR_H 
#define GRAMMAR_H 


#include "proto.h"
#include <boost/spirit/include/qi.hpp>

using namespace boost::spirit;
//namespace qi = boost::spirit::qi;
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <boost/spirit/include/karma.hpp>

template <typename Iterator> struct ParseGrammar: qi::grammar<Iterator, Message()> {
    qi::rule<Iterator>                            _space;
    qi::rule<Iterator, Message()>                 _message;
    qi::rule<Iterator, Request()>                 _request;
    qi::rule<Iterator, Response()>                _response;
    qi::rule<Iterator, uint32_t()>                _code;
    qi::rule<Iterator, std::string()>             _id;
    qi::rule<Iterator, std::vector<Parameter>()>  _params;
    qi::rule<Iterator, Parameter() >              _param;
    qi::rule<Iterator, std::vector<Parameter>()>  _list;
    qi::rule<Iterator, uint64_t()>                _number;
    qi::rule<Iterator, std::string()>             _string;

    ParseGrammar(): qi::grammar<Iterator, Message()>(_message) {
        _space        = +ascii::space;

        _message            = ( _request | _response) [qi::_val = qi::_1];

        _request      = _id               
				       [boost::phoenix::at_c<0>(qi::_val) = qi::_1]
                        >> _space >> _id       
				       [boost::phoenix::at_c<1>(qi::_val) = qi::_1]
                        >> -(_space >> _params 
					   [boost::phoenix::at_c<2>(qi::_val) = qi::_1]);

        _response     = _code
				        [boost::phoenix::at_c<0>(qi::_val) = qi::_1]
						>> -(_space >> _params   
						[boost::phoenix::at_c<1>(qi::_val) = qi::_1]);

        _code         = qi::hex      
				        [qi::_val = qi::_1];

        _id           = +(ascii::char_("A-Za-z0-9_")         
						[qi::_val += qi::_1]);

        _params       = (_param  
						[boost::phoenix::push_back(qi::_val, qi::_1)]
                        % _space) | qi::eps;

        _param        = (_list
                        | _number
                        | _string)          
				        [qi::_val = qi::_1];

        _list         = '{' >> -_space
                        >> (_param    
						[boost::phoenix::push_back(qi::_val, qi::_1)]
                        % _space | qi::eps)
                        >> -_space >> '}';

        _number       = ((qi::lit("0x") > qi::hex     
						[qi::_val = qi::_1])
                        | qi::long_long    
						[qi::_val = qi::_1]);

        _string       = '"' >> *(
                            ('\\' >> ascii::char_              [qi::_val += qi::_1])
                            | (ascii::char_ - ascii::char_("\"\\")) [qi::_val += qi::_1]
                        ) >> '"';
    }
};

template<typename Iterator>
struct GenerateGrammar: karma::grammar<Iterator, Message()> {
    karma::rule<Iterator>                          _space;
    karma::rule<Iterator, Message()>            _message;
    karma::rule<Iterator, Request()>               _request;
    karma::rule<Iterator, Response()>              _response;
    karma::rule<Iterator, uint32_t()>          _code;
    karma::rule<Iterator, std::string()>           _id;
    karma::rule<Iterator, std::vector<Parameter>()> _params;
    karma::rule<Iterator, Parameter() >             _param;
    karma::rule<Iterator, std::vector<Parameter>()> _list;
    karma::rule<Iterator, uint64_t()>             _number;
    karma::rule<Iterator, std::string()>           _string;
    karma::rule<Iterator, char()>                  _char;

    GenerateGrammar(): karma::grammar<Iterator, Message()>(_message) {
        _space        = karma::lit(' ');
        _message            =  _request | _response; 
        _request      = '+' << _id << _space << _id << (_space << _params | karma::eps);
        _response     = '=' << _code << (_space << _params | karma::eps);
        _code         = karma::upper[karma::right_align(8, '0')[karma::hex]];
        _id           = karma::string;
        _params       = _param % _space;
        _param        = _list | _number | _string;
        _list         = '{' << _space << (_param % _space << _space | karma::eps) << '}';
        _number       = karma::long_long; 
        _string       = '"' << *_char << '"';
        _char         = '\\' << karma::char_('\\') | '\\' << karma::char_('"') | karma::char_;
    }
};


#endif
