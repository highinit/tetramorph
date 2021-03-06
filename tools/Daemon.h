#ifndef _DAEMON_H_
#define _DAEMON_H_

#include "hiconfig.h"
/*
#include "hiaux/network/HttpServer/HttpServer.h"
#include "hiaux/network/HttpOutReqDisp.h"
#include "hiaux/threads/threadpool.h"
#include "hiaux/threads/tasklauncher.h"
*/
//#include "hiaux/structs/hashtable.h"
#include "hiaux/loadconf/loadconf.h"

#include <vector>
#include <map>
#include <string>

#include <iostream>
#include <fstream>

#include <fcntl.h>
#include <sys/resource.h>
#include <signal.h>

#include <errno.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <boost/program_options.hpp>

class Daemon {

	void parseConfig(const std::string &_config_file);
	
	int checkLockFile(const std::string &_filename);
	void daemonize(const std::string &_pidfile, const std::string &_logfile);
	
	void startWatcher();
	void startWorker();
	void loadConfig(const std::string &_config_file);
	
	std::string m_chdir;
	
protected:
	
	typedef boost::program_options::variables_map variables_map;
	variables_map m_cmd_vm;
	
	enum Command {
		START,
		STOP,
		RESTART,
		RELOAD
	};
	
	int m_lockfile_fd;
	
	std::vector<std::string> m_required_params;
	std::vector<std::string> m_optional_params;

	ConfigParams m_config;
	
	boost::program_options::options_description m_cmd_opts;

	std::string m_config_path;
	bool interactive;

	void fallDown(std::string _s);
	void onFinished();

	void setDefaultSignalHandlers();

	virtual void addCmdArguments();
	virtual void setParamsList(std::vector<std::string> &_required_params, std::vector<std::string> &_optional_params) = 0;
	virtual void doStart() = 0;
	
	int doStop();
	void setChDir(const std::string &_dir);
	
public:
	
	Daemon();
	virtual ~Daemon();
	
	void start(const std::string &_config_name, int argc, char** argv);
};

typedef boost::shared_ptr<Daemon> DaemonPtr;

#endif
