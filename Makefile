all:
	g++ -o server server.cxx
	g++ -o client client.cxx
	mkdir out
	mv server out
	mv client out
