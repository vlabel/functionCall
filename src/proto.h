#ifndef PROTO_H	 
#define PROTO_H 

#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>

#include <string>
#include <iostream>

#include <boost/spirit/include/classic.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include "stdint.h"
using namespace boost::spirit;


typedef boost::make_recursive_variant<std::vector<boost::recursive_variant_>,
                                		uint64_t, std::string>::type Parameter;

struct Package {
    std::vector<Parameter> m_params;
};


struct Request: public Package {
    std::string m_function;
    ~Request() {}
};

BOOST_FUSION_ADAPT_STRUCT(
    Request,
    (std::string, m_function)
    (std::vector<Parameter>, m_params)
)

struct Response: public Package {
    uint32_t m_code;
    ~Response() {}
};


BOOST_FUSION_ADAPT_STRUCT(
    Response,
    (uint32_t, m_code)
    (std::vector<Parameter>, m_params)
)

typedef boost::variant<Request, Response> Message;

#endif /*PROTO_H*/
