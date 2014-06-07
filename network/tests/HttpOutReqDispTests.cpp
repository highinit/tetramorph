#include "HttpOutReqDispTests.h"

TestRequester::TestRequester(boost::function<void(int, int, const std::string&)> _onCall,
				boost::function<void(int)> _onFinished,
				int id,
				boost::function<void()> _onWrongResp):
	HttpOutRequestDisp::Requester(_onCall, _onFinished, id),
	m_onWrongResp(_onWrongResp) {

}

void TestRequester::onCallDone (int _callid, bool _success, const std::string &_resp) {
	std::cout << "TestRequester::onCallDone resp: " << _resp << std::endl;
	if (_resp != inttostr(m_id)) {
		std::cout << "wrong resp\n";
		m_onWrongResp();
	}

	finished();
}

void TestRequester::start() {
//	std::cout << "TestRequester::start\n";
	call(0, "http://localhost:1236/?param=" + inttostr(m_id));
}

void HttpOutReqDispTests::onFinished() {
	
}

void HttpOutReqDispTests::onHttpRequest(HttpSrv::ConnectionPtr http_conn, HttpSrv::RequestPtr req) {
	//std::cout << "HttpOutReqDispTests::onHttpRequest\n";
	hiaux::hashtable<std::string, std::string>::iterator it =
				req->values_GET.begin();
	
	std::string resp;
	while (it != req->values_GET.end()) {
		//std::cout << it->first << "/" << it->second << std::endl;
		resp += it->second;
		it++;
	}
	
	http_conn->sendResponse(resp);
	http_conn->close();
}

void HttpOutReqDispTests::onWrongResp() {
	TS_ASSERT(false);
}

HttpOutReqDispTests::HttpOutReqDispTests() {
	const int port = 1236;
	hThreadPoolPtr pool (new hThreadPool(100));
	TaskLauncherPtr launcher (new TaskLauncher(
					pool, 10, boost::bind(&HttpOutReqDispTests::onFinished, this)));
	m_srv.reset(new HttpSrv(launcher,
					HttpSrv::ResponseInfo("text/html; charset=utf-8",
										"highinit suggest server"),
					boost::bind(&HttpOutReqDispTests::onHttpRequest, this, _1, _2)));
					
	m_srv->start(port);
	pool->run();
	//pool->join();
	sleep(1);
	
	m_req_disp.reset(new HttpOutRequestDisp(launcher));
	
	for (int i = 0; i<100; i++) {
	
		TestRequesterPtr requester(new TestRequester(boost::bind(&HttpOutRequestDisp::onCall, m_req_disp.get(), _1, _2, _3),
													boost::bind(&HttpOutRequestDisp::onRequesterFinished, m_req_disp.get(), _1),
													i,
													boost::bind(&HttpOutReqDispTests::onWrongResp, this)));
		m_req_disp->addRequester(requester);
	}
	
	sleep(1);
	
	for (int i = 0; i<1000; i++) {
		if (i%100==0)
			sleep(1);
		m_req_disp->kick();
	}
}

