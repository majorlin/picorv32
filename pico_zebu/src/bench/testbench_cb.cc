//----------------------------------------------------------------------
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
#include <libZebu.hh>
#include <sys/resource.h>
#include "XtorScheduler.hh"
#include <sys/time.h>
#include "TopScheduler.hh"
#include "svt_c_runtime_cfg.hh"
#include "svt_pthread_threading.hh"
#include "Uart.hh"

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
  if (ret > 'a' && ret < 'Z') {
    ret = (ret < 'z')?(ret+0x20):(ret-0x20);
  }  
  return ret;
}

typedef struct {
  uint8_t sentData;
  bool    dataExpected;
  unsigned int    error;
  bool    end;
} TbCtxt;

// TX Callback
bool txCB ( uint8_t* data, void* ctxt )
{
  bool send = false;
  TbCtxt* tbCtxt = (TbCtxt*)ctxt;
  if (!tbCtxt->dataExpected) {
    if (tbCtxt->sentData == 0xFF) {
      tbCtxt->end = true; // All data have been sent and received
    } else {
      *data = ++(tbCtxt->sentData);
      tbCtxt->dataExpected = true;
      send = true; // send next data
      printf("Tester  -- Sending data 0x%02x\n",*data);
    }
  }
  return send;
}

// RX Callback
void rxCB ( uint8_t data, bool valid, void* ctxt )
{
  TbCtxt* tbCtxt = (TbCtxt*)ctxt;

  printf("Tester  -- Received data 0x%02x %s\n",data,valid?"":"-- parity error detected !!! )");

  if (!tbCtxt->dataExpected) {
     printf("Received unexpected data '%02x'\n", data);
     ++(tbCtxt->error);
  } else {
     tbCtxt->dataExpected = false;
     if (!valid) {
       printf("Parity error detected on received data '%02x'\n", data);
       ++(tbCtxt->error);        
     } else if (data != convertData(tbCtxt->sentData)) {
       printf("Wrong data '0x%02x' instead of '0x%02x'\n", 
              data, convertData(tbCtxt->sentData));
       ++(tbCtxt->error);        
     }
  }
}


typedef struct {
  queue<uint8_t> dataQueue;
} RpCtxt;

// TX Callback
bool rpTxCB ( uint8_t* data, void* ctxt )
{
  bool send = false;
  RpCtxt* rpCtxt = (RpCtxt*)ctxt;
  
  if (!((rpCtxt->dataQueue).empty())) {
    *data =  convertData((rpCtxt->dataQueue).front());
    (rpCtxt->dataQueue).pop();
    send = true; // send next data
    printf("Replier -- Sending data 0x%02x\n",*data);
  }
  return send;
}

// RX Callback
void rpRxCB ( uint8_t data, bool valid, void* ctxt )
{ 
  RpCtxt* rpCtxt = (RpCtxt*)ctxt;
  printf("Replier -- Received data 0x%02x %s\n",data,valid?"":"-- parity error detected !!! )");
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
  Uart  *test    = NULL;
  Uart  *replier = NULL;
  Board *board   = NULL;
  bool ok;  
  TbCtxt tbenchCtxt;
  RpCtxt replierCtxt;

  char *zebuWork       = ZEBUWORK;
  char *designFeatures = DFFILE;

  tbenchCtxt.sentData     = 0;
  tbenchCtxt.dataExpected = false;
  tbenchCtxt.error        = 0;
  tbenchCtxt.end          = false;

  try {
    //open ZeBu
    printf("opening ZEBU...\n");

#ifdef SEM
    board = Board::openWithSimulationMode(zebuWork,designFeatures, "uart");
#else 
    board = Board::open(zebuWork,designFeatures, "uart");
#endif 

#ifndef ZEBU_VS_UAPI
    if (board==NULL) { throw ("Could not open Zebu"); }
    test = new Uart; 
    replier = new Uart; 
#else
    XtorScheduler * sched = XtorScheduler::get();
    	svt_c_runtime_cfg* runtime = new svt_c_runtime_cfg();
    	svt_c_threading *threading = new svt_pthread_threading();
    	runtime->set_threading_api(threading);
    	svt_c_runtime_cfg::set_default(runtime);
    	Uart::Register("Uart");
    	test = static_cast<Uart*>(Xtor::getNewXtor(board, "Uart",sched,NULL, runtime));
    	replier = static_cast<Uart*>(Xtor::getNewXtor(board, "Uart",sched,NULL, runtime));
#endif

    if (legacy_mode) {
      test->init(board,"uart_driver_0");
      replier->init(board,"uart_driver_1");
    } else {
      test->init(board,"testbench.uart_driver_0");
      replier->init(board,"testbench.uart_driver_1");
    }
    board->init(NULL);

    ok = test->setWidth(8);
    ok &= test->setParity(NoParity);
    ok &= test->setStopBit(TwoStopBit);
    ok &= test->setRatio(16);
    ok &= test->config(); 
    if (!ok ){ throw ("UART config failed");}

    ok = replier->setWidth(8);
    ok &= replier->setParity(NoParity);
    ok &= replier->setStopBit(TwoStopBit);
    ok &= replier->setRatio(16);
    ok &= replier->config(); 
    if (!ok ){ throw ("UART config failed");}

    test->dumpSetDisplayErrors(true);
    test->dumpSetFormat(DumpASCII);
    test->dumpSetDisplay(DumpSplit);
    test->dumpSetMaxLineWidth(40);
    test->openDumpFile("test_dump.log");
    
    replier->dumpSetDisplayErrors(true);
    replier->dumpSetFormat(DumpASCII);
    replier->dumpSetDisplay(DumpSplit);
    replier->dumpSetMaxLineWidth(40);
    replier->openDumpFile("test_dump.log");

    test->useZebuServiceLoop();
    test->setReceiveCB(rxCB,&tbenchCtxt);
    test->setSendCB(txCB,&tbenchCtxt);

    replier->useZebuServiceLoop();
    replier->setReceiveCB(rpRxCB,&replierCtxt);
    replier->setSendCB(rpTxCB,&replierCtxt);

    // TB Main loop
    while (!tbenchCtxt.end) { board->serviceLoop(); }

    test->closeDumpFile();
    replier->closeDumpFile();

    if (tbenchCtxt.error != 0) {
      printf("Detected %d errors during test\n",tbenchCtxt.error);
      ret = 1;
    }
  } catch (const char *err) {
    ret = 1; fprintf(stderr,"Testbench error: %s\n", err);
  }
  if (test != NULL)  { delete test; }
  if (board != NULL) { board->close((ret ==0)?"OK":"KO"); }
  printf("Test %s\n",(ret==0)?"OK":"KO");

  return ret;
}
