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
#include <ctype.h>


using namespace ZEBU;
using namespace ZEBU_IP;
using namespace UART;
using namespace std;

//#ifndef ZWORK
//#define ZWORK "../zebu.work"
//#endif
//
//#ifndef DFEATURES
//#define DFEATURES "./designFeatures"
//#endif

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
    //#if TEST_JMB==0
    //if(isalnum(data)){
    //    (rpCtxt->dataQueue).push('[');(rpCtxt->dataQueue).push('{');
    //(rpCtxt->dataQueue).push(data);
    //(rpCtxt->dataQueue).push(toupper(data));
    //(rpCtxt->dataQueue).push(tolower(data));
    //if (isdigit(data)) {
    //for (int i=(data-'0');i>=0;i--)  (rpCtxt->dataQueue).push(data);
    //}
    //(rpCtxt->dataQueue).push(']');         (rpCtxt->dataQueue).push('}');
    //}
    //#endif
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
  Board       *board = NULL;
  const unsigned   nbUartMax = 1;
  unsigned         nbUart = 0;
  Uart           * uarts[nbUartMax];
  const char     * uartInstNames[nbUartMax];
  bool ok;
  RpCtxt replierCtxt;
 
  char *zebuWork       = ZEBUWORK;
  char *designFeatures = DFFILE;

  try {
    //open ZeBu
    printf("opening ZEBU...\n");
    board = Board::open(zebuWork,designFeatures,"uart");
    if (board==NULL) { throw("Could not open Zebu");}

#ifdef USE_DRIVER_ITERATOR
    printf("Going through UART drivers...\n"); fflush(stdout);
    for (bool found = Uart::FirstDriver(board); found; found = Uart::NextDriver()) {
      switch (nbUart) {
      case 0:
        uarts[nbUart] = new UartTerm;
        break;
      case 1:
        uarts[nbUart] = new Uart;
        break;
      default:
        throw("Too many UART transactors  found");
      }
      if (nbUart < nbUartMax) {
        uartInstNames[nbUart] = Uart::GetInstanceName();
        printf("\nConnecting UART Xtor instance #%d '%s'\n", nbUart, Uart::GetInstanceName());
        uarts[nbUart]->init(board,uartInstNames[nbUart]);
      } else {
        throw("Too many UART drivers found !");
      }
      ++nbUart;
    } 
#else
    uarts[0] = new UartTerm( Conv_DOS_BSR ); // Conv_None , Conv_ASCII (DOS) : Conv_DOS_BSR
    if (legacy_mode) {
      uartInstNames[0] = "uart_driver_0";
    } else {
      uartInstNames[0] = "testbench.uart_driver_0";
    }
    uarts[0]->init(board,uartInstNames[0]);
    uarts[0]->setDebugLevel(2);uarts[0]->setName("UART_XTERMINAL_0");
/*
    uarts[1] = new Uart;
    if (legacy_mode) {
      uartInstNames[1] = "uart_driver_1";
    } else {
      uartInstNames[1] = "testbench.uart_driver_1";
    }

    uarts[1]->init(board,uartInstNames[1]);
    uarts[1]->setDebugLevel(2);uarts[1]->setName("UART_STD_1");
*/
    nbUart = 1;
#endif
    
    printf("\n\n  --  init board  -- \n"); fflush(stdout);
    board->init(NULL);

    for (unsigned int i = 0; (i < nbUart); ++i) {
      // Config UART interface
      ok =  uarts[i]->setWidth(8);
      ok &= uarts[i]->setParity(NoParity);
      ok &= uarts[i]->setStopBit(OneStopBit);
      ok &= uarts[i]->setStopBit(OneBitStop);
      ok &= uarts[i]->setRatio(106);
      ok &= uarts[i]->config(); 

      if (ok) {
        char logFname[1024];
        // Configure and open dump file
        uarts[i]->dumpSetDisplayErrors(true);
        uarts[i]->dumpSetFormat(DumpASCII);
        uarts[i]->dumpSetDisplay(DumpSplit);
        uarts[i]->dumpSetMaxLineWidth(80);
        sprintf(logFname,"%s_dump.log", uartInstNames[i]);
        uarts[i]->openDumpFile(logFname);
      }

      if(!ok) { throw ("Could not configure UART"); }
      uarts[i]->useZebuServiceLoop();
    }

    if (nbUart > 1) {
      uarts[1]->setReceiveCB(rpRxCB,&replierCtxt);
      uarts[1]->setSendCB(rpTxCB,&replierCtxt);
    }

    printf("\n\n  -- Starting testbench  -- \n"); fflush(stdout);
    // Testbench main loop
    while (uarts[0]->isAlive()) {board->serviceLoop();} 

    
    for (unsigned int i = 0; (i < nbUart); ++i) { uarts[i]->closeDumpFile(); }

  } catch (const char *err) {
    ret = 1; fprintf(stderr,"Testbench error: %s\n", err);
  }
  for (unsigned int i = 0; (i < nbUart); ++i) { delete uarts[i]; }
  if (board != NULL) { board->close((ret ==0)?"OK":"KO"); }
  printf("Test %s\n",(ret==0)?"OK":"KO");

  return ret;
}
