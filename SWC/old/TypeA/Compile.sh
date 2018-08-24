
sudo g++ -c -fPIC Global.cpp
sudo g++ -c -fPIC interpreter.cpp
sudo g++ -c -fPIC Communication.cpp
sudo g++ -c -fPIC DataCapture.cpp
sudo g++ -c -fPIC action.cpp
sudo g++ -c -fPIC reply.cpp
sudo g++ -c -fPIC SWCServerMain.cpp
sudo g++ Global.o interpreter.o Communication.o DataCapture.o action.o reply.o SWCServerMain.o -o SWCServer -lpthread
