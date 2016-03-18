CXX=g++
CXXFLAGS=-std=c++11

all: server reporters readers admin

server:
	${CXX} ${CXXFLAGS} Server_TCP.cpp -o server.out

reporters:
	${CXX} ${CXXFLAGS} reporter.cpp news.h -o reporter1.out
	${CXX} ${CXXFLAGS} reporter.cpp news.h -o reporter2.out

readers:
	${CXX} ${CXXFLAGS} reader.cpp news.h -o reader1.out
	${CXX} ${CXXFLAGS} reader.cpp news.h -o reader2.out
	${CXX} ${CXXFLAGS} reader.cpp news.h -o reader3.out

admin:
	${CXX} ${CXXFLAGS} administrator.cpp news.h -o admin.out

clean: clean_reporters clean_readers clean_admin clean_server

clean_server:
	rm server.out

clean_reporters:
	rm reporter1.out reporter2.out

clean_readers:
	rm reader1.out reader2.out reader3.out

clean_admin:
	rm admin.out

