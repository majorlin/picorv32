//---------------------------------------------------------------------  
// COPYRIGHT (C) 2018 SYNOPSYS INC.       
// This software and the associated documentation are confidential and   
// proprietary to Synopsys, Inc. Your use or disclosure of this software 
// is subject to the terms and conditions of a written license agreement 
// between you, or your company, and Synopsys, Inc. In the event of      
// publications, the following notice is applicable:                     
//                                                                       
// ALL RIGHTS RESERVED                                                   
//                                                                       
// The entire notice above must be reproduced on all authorized copies.  
//---------------------------------------------------------------------- 

#include <stdexcept>
#include <exception>
#include <queue>
#include <sys/time.h>
#include <libZebu.hh>
#include "Uart.hh"
#include <string.h>


using namespace ZEBU;
using namespace ZEBU_IP;
using namespace UART;
using namespace std;

#define xstr(s) str(s)
#define str(s) #s

//#ifndef ZWORK
//#define ZWORK "../zebu.work"
//#endif
//
//#ifndef DFEATURES
//#define DFEATURES "./designFeatures"
//#endif

#ifndef TCP_PORT
#define TCP_PORT    10000
#endif

#ifndef TCP_SERVER
#define TCP_SERVER "localhost"
#endif

#ifndef TCP_MODE
#define SERVER_MODE  true
#else
#define SERVER_MODE  ( strncmp( "client" , TCP_MODE ,6) != 0 )
#endif

uint8_t convertData ( uint8_t data )
{
  uint8_t ret = data;
  if (!((ret < 'A') || (ret > 'z'))) {
    ret = (ret < 'a')?(ret+0x20):(ret-0x20);
  }  
  return ret;
}


typedef struct {
  queue<uint8_t> dataQueue;
} RpCtxt;

// Replier TX Callback
bool rpTxCB ( uint8_t* data, void* ctxt )
{
  bool send = false;
  RpCtxt* rpCtxt = (RpCtxt*)ctxt;
  
  if (!((rpCtxt->dataQueue).empty())) {
    *data =  convertData((rpCtxt->dataQueue).front());
    (rpCtxt->dataQueue).pop();
    send = true; // send next data
  }
  return send;
}

// Replier RX Callback
void rpRxCB ( uint8_t data, bool valid, void* ctxt )
{ 
  RpCtxt* rpCtxt = (RpCtxt*)ctxt;
  if (valid) {
    (rpCtxt->dataQueue).push(data);
  } else {
    (rpCtxt->dataQueue).push(0x0);
  }
}


//#################################
//  main
//#################################
int main (int argc, char *argv[]) {
  bool legacy_mode = LEGACY;
  int ret = 0;
  Board       *board   = NULL;
  UartServer*  test    = NULL;
  Uart*        replier = NULL;
  bool ok;
  RpCtxt replierCtxt;
  uint16_t tcpPort     = TCP_PORT;
  char*    tcpServer   = TCP_SERVER;
  bool     serverMode  = SERVER_MODE;
 
  char *zebuWork       = ZEBUWORK;
  char *designFeatures = DFFILE;

  try {
    //open ZeBu
    printf("opening ZEBU...\n");
    board = Board::open(zebuWork,designFeatures,"uart");
    if (board==NULL) { throw("Could not open Zebu");}

    printf("Going through UART drivers...\n"); fflush(stdout);
    
    test    = new UartServer;
    replier = new Uart;

    if (legacy_mode) {
      test->init(board,"uart_driver_0");
      replier->init(board,"uart_driver_1");
    } else {
      test->init(board,"testbench.uart_driver_0");
      replier->init(board,"testbench.uart_driver_1");
    }
    printf("\n\n  --  init board  -- \n"); fflush(stdout);
    board->init(NULL);

    ok =  test->setWidth(8);
    ok &= test->setParity(NoParity);
    ok &= test->setStopBit(TwoStopBit);
    ok &= test->setRatio(16);
    ok &= test->config(); 
    if(!ok) { throw ("Could not configure UART"); }
    
    ok =  replier->setWidth(8);
    ok &= replier->setParity(NoParity);
    ok &= replier->setStopBit(TwoStopBit);
    ok &= replier->setRatio(16);
    ok &= replier->config(); 
    if(!ok) { throw ("Could not configure UART"); }

    test->dumpSetDisplayErrors(true);
    test->dumpSetFormat(DumpASCII);
    test->dumpSetDisplay(DumpSplit);
    test->dumpSetMaxLineWidth(40);
    test->openDumpFile("test_dump.log");
    
    replier->dumpSetDisplayErrors(true);
    replier->dumpSetFormat(DumpASCII);
    replier->dumpSetDisplay(DumpSplit);
    replier->dumpSetMaxLineWidth(40);
    replier->openDumpFile("replier_dump.log");

    test->useZebuServiceLoop();

    replier->useZebuServiceLoop();
    replier->setReceiveCB(rpRxCB,&replierCtxt);
    replier->setSendCB(rpTxCB,&replierCtxt);

    if (serverMode) {
      char hostName[1024];
      printf("\n\n  -- Start TCP server  -- \n"); 
      if (!test->startServer(tcpPort)) {throw ("Could not start UART Server."); }
      if (gethostname(hostName, 1024) == 0) {
        printf("\nTCP server is up, TCP client may now be connected to %s:%u\n\n",hostName,tcpPort);
      }
      while (!test->isConnected()) {}
      printf("\nTCP client is now connected\n\n");
    } else {
      printf("\n\n  -- Starting TCP connection  -- \n"); 
      if (!test->startClient( tcpServer, tcpPort)) {throw ("Could not start UART Server."); }
    }

    printf("\n\n  -- Starting testbench  -- \n"); fflush(stdout);
    while (test->isConnected()) {board->serviceLoop();}

    test->closeDumpFile();
    replier->closeDumpFile();

  } catch (const char *err) {
    ret = 1; fprintf(stderr,"Testbench error: %s\n", err);
  }

  if (replier != NULL) { delete replier; replier = NULL;}
  if (test != NULL)    { delete test; test = NULL;}
  if (board != NULL) { board->close((ret ==0)?"OK":"KO"); }
  printf("Test %s\n",(ret==0)?"OK":"KO");

  return ret;
}
