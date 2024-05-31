#ifndef GAMESPY_H
#define GAMESPY_H

void gspy_StartGame(char *name);
void gspy_EndGame();
int gspy_Init(void);
void gspy_DoFrame();

int gspy_ParseReq(char *buffer,SOCKADDR_IN *addr);

int gspy_DoEcho(SOCKADDR_IN *addr,char *msg);
int gspy_DoBasic(SOCKADDR_IN *addr);
int gspy_DoStatus(SOCKADDR_IN *addr);
int gspy_DoRules(SOCKADDR_IN *addr);
int gspy_DoPlayers(SOCKADDR_IN *addr);
int gspy_DoGameInfo(SOCKADDR_IN *addr);
int gspy_DoHeartbeat(SOCKADDR_IN *addr);






#endif