//---------------------------------------------------------------------
//  COPYRIGHT (C) 2018 SYNOPSYS INC.
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
#include "UartMP.hh"
#include <ctype.h>
#include "svt_c_runtime_cfg.hh"
#include "svt_pthread_threading.hh"
#include "svt_zebu_platform.hh"


using namespace ZEBU;
using namespace ZEBU_IP;
using namespace ZEBU_MP;
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
  } else {
    (rpCtxt->dataQueue).push(0x0);
  }
}

uint8_t convertData2 ( uint8_t data )
{
  uint8_t ret = data;
  if (!((ret < 'A') || (ret > 'z'))) {
    ret = (ret+0x1);
  }  
  return ret;
}

bool user_CB ( uint8_t data, uint8_t &conv_data,void* ctxt )
{
  bool rsl = true;
  UartMP *uartterm = (UartTermMP*)ctxt;
  if (data == '\x03') {
    uartterm->printTermString("\nCtrl-C hit: 0x%02X\n",data);
  } 
  conv_data=data;
  return rsl;
}

//#################################
//  main
//#################################
int main (int argc, char *argv[]) {
  bool legacy_mode = LEGACY;
  int ret = 0;
  Board       *board = NULL;
  const unsigned   nbUartMax = 2;
  unsigned         nbUart = 0;
  UartMP           * uarts[nbUartMax];
  const char     * uartInstNames[nbUartMax];
  bool ok;
  RpCtxt replierCtxt;

  const char *zebuWork       = ZEBUWORK;
  const char *designFeatures = DFFILE;
  
  svt_report      *reporter  = new svt_report_default("Default SVT Reporter");
  svt_c_threading *threading = new svt_pthread_threading();
  
  //Run time configurations 
  svt_c_runtime_cfg* runtime = new svt_c_runtime_cfg();
  runtime->set_report_api(reporter);
  runtime->set_threading_api(threading);

  try {
    //open ZeBu
    printf("opening ZEBU...\n");
    board = Board::open(zebuWork,designFeatures);
    if (board==NULL) { throw("Could not open Zebu");}
    
    runtime->set_platform(new svt_zebu_platform(board,false, true));
    svt_c_runtime_cfg::set_default(runtime);


#ifdef USE_DRIVER_ITERATOR
    printf("Going through UART drivers...\n"); fflush(stdout);
    for (bool found = UartMP::FirstDriver(board); found; found = UartMP::NextDriver()) {
      switch (nbUart) {
      case 0:
         uarts[nbUart] = UartTermMP::get("uart_driver_0", runtime);
        break;
      case 1:
         uarts[nbUart] = UartMP::get("uart_driver_1", runtime);
        break;
      default:
        throw("Too many UART transactors  found");
      }
      if (nbUart < nbUartMax) {
        uartInstNames[nbUart] = UartMP::GetInstanceName();
        printf("\nConnecting UART Xtor instance #%d '%s'\n", nbUart, UartMP::GetInstanceName());
      } else {
        throw("Too many UART drivers found !");
      }
      ++nbUart;
    } 
#else
        
    if (legacy_mode) {
    std::cout << "Reached in legacy \n";
    uarts[0] = UartMP::get("uart_driver_0", runtime) ;
    uartInstNames[0] = "uart_driver_0" ;
    std::cout << "Exit -legacy \n";
    }
    else
    {
    std::cout << "Reached in Non-legacy \n";
    uarts[0] = UartMP::get("testbench.uart_driver_0", runtime) ;
    uartInstNames[0] = "testbench.uart_driver_0" ;
    std::cout << "Exit -Non - legacy \n";
    }

    uarts[0]->setLog("uartterm.log");
    uarts[0]->setDebugLevel(2);uarts[0]->setName("UART_XTERMINAL_0");


    if (legacy_mode) {
    std::cout << "Reached in legacy \n";
    uarts[1] = UartMP::get("uart_driver_1", runtime) ;
    uartInstNames[1] = "uart_driver_1" ;
    std::cout << "Exit -legacy \n";
    }

   else {
    std::cout << "Entererd in Non-legacy -entry\n";
    uarts[1] = UartMP::get("testbench.uart_driver_1", runtime) ;
    uartInstNames[1] = "testbench.uart_driver_1";
    std::cout << "Exit -- Non legacy \n";
  }
    uarts[1]->setDebugLevel(2);uarts[1]->setName("UART_STD_1");

    nbUart = 2; 
      
    
#endif
    
    printf("\n\n  --  init board  -- \n"); fflush(stdout);
    board->init(NULL);

    for (unsigned int i = 0; (i < nbUart); ++i) {
      // Config UART interface
      ok =  uarts[i]->setWidth(8);
      ok &= uarts[i]->setParity(NoParity);
      ok &= uarts[i]->setStopBit(TwoStopBit);
      ok &= uarts[i]->setStopBit(TwoBitStop);
      ok &= uarts[i]->setRatio(16);
      ok &= uarts[i]->config(); 

      if (ok) {
        char logFname[1024];
        // Configure and open dump file
        uarts[i]->dumpSetDisplayErrors(true);
        uarts[i]->dumpSetFormat(DumpASCII);
        uarts[i]->dumpSetDisplay(DumpSplit);
        uarts[i]->dumpSetMaxLineWidth(80);
        sprintf(logFname,"%s_dump.log", uartInstNames[i]);
        std::cout<<"logname is :"<<logFname<<"--"<<uartInstNames[i]<<endl;
        uarts[i]->openDumpFile(logFname);
      }

      if(!ok) { throw ("Could not configure UART"); }
      uarts[i]->useZebuServiceLoop();
    }

    if (nbUart > 1) {
      uarts[1]->setReceiveCB(rpRxCB,&replierCtxt);
      uarts[1]->setSendCB(rpTxCB,&replierCtxt);
    }
    uarts[0]->setInputCharCB(user_CB,uarts[0]);
    
    printf("\n\n  -- Starting testbench  -- \n"); fflush(stdout);
    // Testbench main loop
    while (uarts[0]->isAlive()) {board->serviceLoop();} 

    
    for (unsigned int i = 0; (i < nbUart); ++i) { uarts[i]->closeDumpFile(); }

  } catch (const char *err) {
    ret = 1; fprintf(stderr,"Testbench error: %s\n", err);
  }
  for (unsigned int i = 0; (i < nbUart); ++i) { delete uarts[i]; }
  
  runtime->get_platform()->end_of_simulation();

  printf("Test %s\n",(ret==0)?"OK":"KO");

  return ret;
}
