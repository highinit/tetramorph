#include "hiconfig.h"
#include "hiaux/crypt/sha1.h"
#include "hiaux/strings/string_utils.h"
#include "hiaux/crypt/md5.h"

#include "curl/curl.h"

class HttpApiClient {
	std::string m_url;
	std::string m_api_userid;
	std::string m_api_key;
	
	bool m_is_auth_info_set;
	
	CURL *m_curl;

public:
	HttpApiClient(const std::string &_url,
					const std::string &_userid,
					const std::string &_key);
					
	HttpApiClient(const std::string &_url);
	
	~HttpApiClient();
	
	void buildRequestUrl(const std::string &_method,
						const hiaux::hashtable<std::string, std::string> &_get_params,
						std::string &_requrl) const;
	
	void call(const std::string &_method,
				const hiaux::hashtable<std::string, std::string> &_get_params,
				std::string &_resp) const;
};
