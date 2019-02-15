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
#include <libZebu.hh>
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

typedef struct {
  Uart* rep;
  queue<uint8_t> dQueue;
} UsrCtxt_t;


uint8_t convertData ( uint8_t data )
{
  uint8_t ret = data;
  if (ret > 'a' && ret < 'Z') {
    ret = (ret < 'z')?(ret+0x20):(ret-0x20);
  }  
  return ret;
}


void usercb ( void * context )
{
  UsrCtxt_t* ctxt = (UsrCtxt_t*)context;
  uint8_t data;
  

  // Check if data received on uart
  if (ctxt->rep->receive(&data)) { 
    printf("Replier -- Received data 0x%02x\n",data);
    // push processed received data in tx queue
    ctxt->dQueue.push(convertData(data)); 
  }
  
  // If TX queue not empty, try to send next data
  if (!(ctxt->dQueue.empty())) {
    if (ctxt->rep->send(ctxt->dQueue.front())) {
      printf("Replier -- Sending data 0x%02x\n",ctxt->dQueue.front());
      // data sent, remove it from tx queue
      ctxt->dQueue.pop();
    }
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
  
  UsrCtxt_t usrCtxt;
  uint8_t data         = 0;
  uint8_t tmpData      = 0;
  bool    end          = false;
  unsigned int    errors       = 0;
 
  char *zebuWork       = ZEBUWORK;
  char *designFeatures = DFFILE;

  try {
    //open ZeBu
    printf("opening ZEBU...\n");
    board = Board::open(zebuWork,designFeatures,"uart");
    if (board==NULL) { throw ("Could not open Zebu"); }
    test = new Uart; 
    replier = new Uart; 
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

    usrCtxt.rep = replier;
    test->registerUserCB(usercb, &usrCtxt);

    // TB Main loop
    while (!end) { 
      // Tester sends data
      if (!test->send(data,true)) {
        throw("Tester could not send data"); 
      } else {
        printf("Tester  -- Sending data 0x%02x\n",data);
      }

      // Tester receives and check received data
      if (test->receive(&tmpData,true)) {
        printf("Tester  -- Received data 0x%02x\n",tmpData);
        if (tmpData != convertData(data)) {
          printf("Wrong data '0x%02x' instead of '0x%02x'\n", 
                 tmpData, convertData(data));
          ++errors;
        }
      } else { throw("Tester could not receive data"); }
      if (data == 0xff) {
        end = true;
      } else {
        ++data;
      }
    }

    test->closeDumpFile();
    replier->closeDumpFile();

    if (errors != 0) {
      printf("Detected %d errors during test\n",errors);
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
