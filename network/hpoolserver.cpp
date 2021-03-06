#include "hiconfig.h"
#include "hpoolserver.h"

using namespace std;
/*
class PoolException : public std::exception
{
	std::string m_message;
	public:

	PoolException(std::string message) {
		m_message = message;
	}

	std::string what() {
		return m_message;
	}

	~PoolException() throw() {
		m_message.clear();
	}
};

hPoolServer::Connection::Connection(std::string _ip, int _port, int _sock,
			boost::function<void(int)> _onClose):
		create_ts(time(0)),
		ip(_ip),
		port(_port),
		m_sock(_sock),
		readmode(true),
		closing(false),
		m_onClose(_onClose) {
}

hPoolServer::Connection::~Connection() {
	if (!closing)
		close();
}

uint64_t hPoolServer::Connection::getCreateTs() {
	return create_ts;
}

void hPoolServer::Connection::close() {
//	std::cout << "pool connection closing\n";
	closing = true;
	m_onClose(m_sock);
}


hPoolServer::hPoolServer(TaskLauncherPtr launcher, 
					boost::function<void(ConnectionPtr)> _onRead,
					boost::function<void(ConnectionPtr)> _onWrite,
					boost::function<void(ConnectionPtr)> _onError) {
	m_launcher = launcher;
	m_onRead = _onRead;
	m_onWrite = _onWrite;
	m_onError = _onError;
	
	m_istopped = 0;
	m_events_watcher.reset(new EventWatcher(
			boost::bind(&hPoolServer::onRead, this, _1, _2),
			boost::bind(&hPoolServer::onWrite, this, _1, _2),
			boost::bind(&hPoolServer::onError, this, _1, _2),
			boost::bind(&hPoolServer::onAccept, this, _1, _2)));
}

hPoolServer::~hPoolServer() {
	stop();
}

void hPoolServer::onCloseConnection(int _sock_fd) {
	{
		hLockTicketPtr ticket = m_connections_lock.lock();
	
		hiaux::hashtable<int, ConnectionPtr>::iterator it = m_connections.find(_sock_fd);
		if (it != m_connections.end())
			m_connections.erase(it);
	}
	hLockTicketPtr _ticket = m_sockets_to_close_q_lock.lock();
	m_sockets_to_close_q.push(_sock_fd);
	
	//m_events_watcher->delSocket(_sock_fd, NULL);
}

void hPoolServer::onRead(int _sock_fd, void *_opaque_info) {

	hLockTicketPtr ticket = m_connections_lock.lock();
	hiaux::hashtable<int, ConnectionPtr>::iterator it = m_connections.find(_sock_fd);
	if (it != m_connections.end()) {
		ticket->unlock();
		m_onRead(it->second);
	}
}
*/
/*
TaskLauncher::TaskRet hPoolServer::handleReadTask(ConnectionPtr _conn) {
	m_handler(_conn);
	return TaskLauncher::NO_RELAUNCH;
}*/
/*
void hPoolServer::onWrite(int _sock_fd, void *_opaque_info) {

	hLockTicketPtr ticket = m_connections_lock.lock();
	hiaux::hashtable<int, ConnectionPtr>::iterator it = m_connections.find(_sock_fd);
	if (it != m_connections.end()) {
		ticket->unlock();
		m_onWrite(it->second);
	}
}

void hPoolServer::onError(int _sock_fd, void *_opaque_info) {

	hLockTicketPtr ticket = m_connections_lock.lock();
	hiaux::hashtable<int, ConnectionPtr>::iterator it = m_connections.find(_sock_fd);
	if (it != m_connections.end()) {
		ticket->unlock();
		m_onError(it->second);
	}
}

void hPoolServer::onSetWrite(int _sock) {
	
//	m_events_watcher->delSocket(sock_fd, NULL);
//	m_events_watcher->delSocket(sock_fd, NULL);
	
}

TaskLauncher::TaskRet hPoolServer::readThread() {
	
	while (m_isrunning) {
		//std::cout << "hPoolServer::readThread handleEvents\n";
		m_events_watcher->handleEvents();
		//std::cout << "hPoolServer::readThread handleEvents finished\n";
		
		{
			hLockTicketPtr ticket = m_sockets_to_close_q_lock.lock();
			while (!m_sockets_to_close_q.empty()) {
//				std::cout << "removing socket from m_events_watcher\n";
				int sock_fd = m_sockets_to_close_q.front();
				m_events_watcher->delSocket(sock_fd);
//				std::cout << "removed socket from m_events_watcher\n";
				::close(sock_fd);
				::shutdown(sock_fd, SHUT_RDWR);
				m_sockets_to_close_q.pop();
			}
		}
	}
	m_istopped.fetch_add(1);
	return TaskLauncher::NO_RELAUNCH;
}

hPoolServer::ConnectionPtr hPoolServer::getConnection(int _fd) {
	
	hLockTicketPtr ticket = m_connections_lock.lock();

	hiaux::hashtable<int, ConnectionPtr>::iterator it = m_connections.find(_fd);
	if (it != m_connections.end())
		return it->second;
//	std::cout << "hPoolServer::getConnection no such connection\n";
	return hPoolServer::ConnectionPtr();
}

void hPoolServer::onAccept(int _sock_fd, void *_opaque_info) {
	
	struct sockaddr_in cli_addr;
	size_t clilen = sizeof(cli_addr);
	int accepted_socket = accept(_sock_fd, 
			 (struct sockaddr *) &cli_addr, 
			 (socklen_t*)&clilen);

	if (accepted_socket < 0)
		return;
	
	//setSocketNonBlock(accepted_socket);
	
//	int set = 1;
//	setsockopt(accepted_socket, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int));
	
	ConnectionPtr connection(new Connection(inet_ntoa(cli_addr.sin_addr),
							cli_addr.sin_port,
							accepted_socket,
							boost::bind(&hPoolServer::onCloseConnection, this, _1)));
	
	{
		hLockTicketPtr ticket = m_connections_lock.lock();
		m_connections.insert(std::pair<int, ConnectionPtr>(connection->m_sock,
													connection) );
	}
			
	m_events_watcher->addSocketRead(connection->m_sock, NULL);
}

void hPoolServer::start(int port) {
	m_isrunning = true;
	m_listen_socket = startServer(port);
	
//	int set = 1;
//	setsockopt(m_listen_socket, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int));
	
	m_events_watcher->addSocketAccept(m_listen_socket, NULL);
	m_launcher->addTask(new boost::function<TaskLauncher::TaskRet()> (
			boost::bind(&hPoolServer::readThread, this)));
}

void hPoolServer::stop() {
    shutdown(m_listen_socket, SHUT_RDWR);
    m_isrunning = 0;
	
	while (m_istopped.load() == 0) {
	}
}

int hPoolServer::startServer(int port) {
	
	char bf[255];
	struct sockaddr_in serv_addr;

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
			throw new std::string("hsock_t::server: ERROR opening server socket");
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	int yes = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)  {
			std::cout << "hPoolServer::startServer setsockopt\n";
			exit(1);
	}

	if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes)) == -1)  {
			std::cout << "hPoolServer::startServer setsockopt\n";
			exit(1);
	}

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
	throw new std::string("hsock_t::server: Error server binding");

	listen(sockfd, POOLSERVER_SOCKET_CONN_QUEUE_SIZE);
	return sockfd;
}

int hPoolServer::startClient(const std::string &_ip, int portno) {
	
	int sockfd;
	struct sockaddr_in serv_addr;
	struct hostent *server = gethostbyname(_ip.c_str());

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
			throw new std::string("hsock_t::client: ERROR opening client socket");

	int yes = 1;
	if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes)) == -1)  {
	}

	memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);

	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)))
			throw new std::string("hsock_t::client: ERROR connecting");
	return sockfd;
}
*/