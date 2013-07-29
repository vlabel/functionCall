#ifndef GRAMMAR_H 
#define GRAMMAR_H 


#include "proto.h"
#include <boost/spirit/include/qi.hpp>

using namespace boost::spirit;
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <boost/spirit/include/karma.hpp>

template <typename Iterator> struct ParseGrammar: qi::grammar<Iterator, Message()> {
    qi::rule<Iterator>                            m_space;
    qi::rule<Iterator, Message()>                 m_message;
    qi::rule<Iterator, Request()>                 m_request;
    qi::rule<Iterator, Response()>                m_response;
    qi::rule<Iterator, uint32_t()>                m_code;
    qi::rule<Iterator, std::string()>             m_id;
    qi::rule<Iterator, std::vector<Parameter>()>  m_params;
    qi::rule<Iterator, Parameter() >              m_param;
    qi::rule<Iterator, std::vector<Parameter>()>  m_list;
    qi::rule<Iterator, uint64_t()>                m_number;
    qi::rule<Iterator, std::string()>             m_string;

    ParseGrammar(): qi::grammar<Iterator, Message()>(m_message) {
        m_space       = +ascii::space;

        m_message           = ( m_request| m_response) [qi::_val = qi::_1];

        m_request     = m_id              
				       [boost::phoenix::at_c<0>(qi::_val) = qi::_1]
                        >> m_space >> m_id      
				       [boost::phoenix::at_c<1>(qi::_val) = qi::_1]
                        >> -(m_space >> m_params
					   [boost::phoenix::at_c<2>(qi::_val) = qi::_1]);

        m_response    = m_code
				        [boost::phoenix::at_c<0>(qi::_val) = qi::_1]
						>> -(m_space >> m_params  
						[boost::phoenix::at_c<1>(qi::_val) = qi::_1]);

        m_code        = qi::hex      
				        [qi::_val = qi::_1];

        m_id          = +(ascii::char_("A-Za-z0-9_")         
						[qi::_val += qi::_1]);

        m_params      = (m_param 
						[boost::phoenix::push_back(qi::_val, qi::_1)]
                        % m_space) | qi::eps;

        m_param       = (m_list
                        | m_number
                        | m_string)          
				        [qi::_val = qi::_1];

        m_list        = '{' >> -m_space
                        >> (m_param   
						[boost::phoenix::push_back(qi::_val, qi::_1)]
                        % m_space | qi::eps)
                        >> -m_space >> '}';

        m_number      = ((qi::lit("0x") > qi::hex     
						[qi::_val = qi::_1])
                        | qi::long_long    
						[qi::_val = qi::_1]);

        m_string      = '"' >> *(
                            ('\\' >> ascii::char_              [qi::_val += qi::_1])
                            | (ascii::char_ - ascii::char_("\"\\")) [qi::_val += qi::_1]
                        ) >> '"';
    }
};

template<typename Iterator>
struct GenerateGrammar: karma::grammar<Iterator, Message()> {
    karma::rule<Iterator>                          m_space;
    karma::rule<Iterator, Message()>               m_message;
    karma::rule<Iterator, Request()>               m_request;
    karma::rule<Iterator, Response()>              m_response;
    karma::rule<Iterator, uint32_t()>              m_code;
    karma::rule<Iterator, std::string()>           m_id;
    karma::rule<Iterator, std::vector<Parameter>()>m_params;
    karma::rule<Iterator, Parameter() >            m_param;
    karma::rule<Iterator, std::vector<Parameter>()>m_list;
    karma::rule<Iterator, uint64_t()>              m_number;
    karma::rule<Iterator, std::string()>           m_string;
    karma::rule<Iterator, char()>                  m_char;

    GenerateGrammar(): karma::grammar<Iterator, Message()>(m_message) {
        m_space        = karma::lit(' ');
        m_message           =  m_request| m_response; 
        m_request     = '+' << m_id<< m_space << m_id << (m_space << m_params| karma::eps);
        m_response    = '=' << m_code<< (m_space << m_params| karma::eps);
        m_code        = karma::upper[karma::right_align(8, '0')[karma::hex]];
        m_id          = karma::string;
        m_params      = m_param% m_space;
        m_param       = m_list| m_number| m_string;
        m_list        = '{' << m_space << (m_param% m_space << m_space | karma::eps) << '}';
        m_number      = karma::long_long; 
        m_string      = '"' << *m_char << '"';
        m_char         = '\\' << karma::char_('\\') | '\\' << karma::char_('"') | karma::char_;
    }
};


#endif
