CXX=g++-5
CXXFLAGS=-std=c++11
all: client test abc xyz

client: client.cpp email.h
	${CXX} ${CXXFLAGS} client.cpp -o client.out

test: testing.cpp email.h
	${CXX} ${CXXFLAGS} testing.cpp -o test.out

abc: serverABC.cpp email.h
	${CXX} ${CXXFLAGS} serverABC.cpp -o abc.out

xyz: serverXYZ.cpp email.h
	${CXX} ${CXXFLAGS} serverXYZ.cpp -o xyz.out

clean:
	rm client.out abc.out xyz.out test.out
