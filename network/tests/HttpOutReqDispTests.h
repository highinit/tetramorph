#include <cxxtest/TestSuite.h>

#include "HttpOutReqDisp.h"

class TestRequester : public HttpOutRequestDisp::Requester {
	boost::function<void()> m_onWrongResp;
	boost::function<void()> m_onResp;
public:
	
	TestRequester(boost::function<void(int, int, const std::string&)> _onCall,
					boost::function<void(int)> _onFinished,
					int id,
					boost::function<void()> _onWrongResp,
					boost::function<void()> _onResp);
	
	virtual void onCallDone (int _callid, bool _success, const std::string &_resp);
	virtual void start();
};

typedef boost::shared_ptr<TestRequester> TestRequesterPtr;

class HttpOutReqDispTests : public CxxTest::TestSuite {
	HttpSrvPtr m_srv;
	HttpOutRequestDispPtr m_req_disp;
	boost::atomic<int> nresps;
public:
	void onResp();
	void onWrongResp();
	void onHttpRequest(HttpSrv::ConnectionPtr http_conn, HttpSrv::RequestPtr req);
	void onFinished();
	HttpOutReqDispTests();
};
