
#CCFLAGS+= -I../include

default:	all
all:    
	cxxtest/bin/cxxtestgen --error-printer -o threadpool_main.cpp threadpool_tests.h & /usr/local/bin/scons
clean:
	/usr/local/bin/scons -c
install:
	/usr/local/bin/scons install

#LDFLAGS+= /usr/local/lib/libbson.a
#LDFLAGS+= /usr/local/lib/libmongoc.a

#all: 
#	g++	main.cpp db_interactions.cpp -o varqs_updater	$^ $(LDFLAGS)

#clean:
#	rm -rf *.o varqs_updater

#LDLIBS=-libmongoc.a



