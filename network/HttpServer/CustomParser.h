#ifndef _CUSTOM_PARSER_H_
#define _CUSTOM_PARSER_H_

#include "CustomRequest.h"
#include <string>

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include "hiaux/network/HttpServer/Request.h"
namespace hiaux {

class RequestParsingEx {
};

class CustomParser {
	
public:

	virtual void execute(const std::string &_d) = 0;
	virtual bool hasRequest() = 0;
	virtual CustomRequestPtr getRequest() = 0;
	
	virtual ~CustomParser();
};

typedef boost::shared_ptr<CustomParser> CustomParserPtr;

}

#endif
