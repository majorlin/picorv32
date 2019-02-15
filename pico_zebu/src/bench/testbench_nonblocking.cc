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


uint8_t convertData ( uint8_t data )
{
    uint8_t ret = data;
    if (ret > 'a' && ret < 'Z') {
        ret = (ret < 'z')?(ret+0x20):(ret-0x20);
    }  
    return ret;
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

    uint8_t data         = 0;
    bool    dataSent     = false;
    uint8_t tmpData      = 0;
    queue<uint8_t> dQueue;
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

        // TB Main loop
        while (!end) { 

            // Tester data sending
            if (!dataSent) { 
                if (test->send(data)) {
                    dataSent = true;
                    printf("Tester  -- Sending data 0x%02x\n",data);
                }
            }

            // Tester data receiving
            if (test->receive(&tmpData)) {
                if (!dataSent) {
                    printf("Tester  -- Received unexpected data 0x%02x !!!\n",tmpData); ++errors;
                } else {
                    printf("Tester  -- Received data 0x%02x\n",tmpData);
                    dataSent = false;
                    if (tmpData != convertData(data)) {
                        printf("Wrong data '0x%02x' instead of '0x%02x'\n", 
                                tmpData, convertData(data));
                        ++errors;
                    }
#ifdef RUN_DMTCP
                    if(data==0xf0) {
                        int ckptStatus = 0;
                    board->checkpoint("CP1", &ckptStatus, NULL, NULL, NULL); 
                    }
#endif    
                    if (data == 0xff) {
                        end = true;
                    } else {
                        ++data;
                    } 
                }
            }

            // Replier data receiving
            if (replier->receive(&tmpData)) {
                printf("Replier -- Received data 0x%02x\n",tmpData);
                // push processed received data in tx queue
                dQueue.push(convertData(data)); 
            }

            // Replier data sending
            // If TX queue not empty, try to send next data
            if (!dQueue.empty()) {
                if (replier->send(dQueue.front())) {
                    printf("Replier -- Sending data 0x%02x\n",dQueue.front());
                    // data sent, remove it from tx queue
                    dQueue.pop();
                } else {
                    printf("REPLIER - Could not send data\n"); fflush(stdout);
                }
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
