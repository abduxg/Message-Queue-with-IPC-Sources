all:
	g++ -o server src/server.cxx
	g++ -o client src/client.cxx
	mkdir out
	mv server out
	mv client out
