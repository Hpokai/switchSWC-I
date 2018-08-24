#ifndef COMMUNICATION_H
#define COMMUNICATION_H

bool TerminalCommunicationOpen(int);
bool TerminalCommunicationClose(int);
bool SendMsg (int, char, char );
char* ReadMsg(int);

#endif
