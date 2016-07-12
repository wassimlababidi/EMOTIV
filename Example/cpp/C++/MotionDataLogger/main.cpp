/****************************************************************************
**
** Copyright 2015 by Emotiv. All rights reserved
** Example - MotionDataLogger
** This example demonstrates how to extract live Motion data using the EmoEngineTM
** in C++. Data is read from the headset and sent to an output file for
** later analysis
**
****************************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdlib>
#include <stdexcept>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#endif
#if __linux__ || __APPLE__
    #include <unistd.h>
#endif

#include "IEmoStateDLL.h"
#include "Iedk.h"
#include "IedkErrorCode.h"

IEE_MotionDataChannel_t targetChannelList[] = {
        IMD_COUNTER,      
        IMD_GYROX, 
        IMD_GYROY, 
        IMD_GYROZ, 
        IMD_ACCX,  
        IMD_ACCY,  
        IMD_ACCZ,  
        IMD_MAGX,  
        IMD_MAGY,  
        IMD_MAGZ,  
        IMD_TIMESTAMP
	};

const char header[] = "COUNTER, GYROX, GYROY, GYROZ, ACCX, ACCY, ACCZ, MAGX, "
	"MAGY, MAGZ, TIMESTAMP";

#if __linux__ || __APPLE__
int _kbhit(void);
#endif

int main(int argc, char** argv) {

	EmoEngineEventHandle eEvent			= IEE_EmoEngineEventCreate();
	EmoStateHandle eState				= IEE_EmoStateCreate();
	unsigned int userID					= 0;	
	float secs							= 1;
	unsigned int datarate				= 0;
	bool readytocollect					= false;
	int state							= 0;
    std::string filename;

	try {

		if (argc != 2) {
#if __linux__
            filename="/tmp/MotionDataLog.txt";
            std::cout << "Write log to file " << filename
                      << std::endl;
#else
#ifdef __APPLE__
            std::string home_path;
            const char* home = getenv("HOME");
            home_path.assign(home);
            home_path.append("/Desktop/MotionDataLog.csv");
            filename = home_path;
            std::cout << "Write log to file " << filename
                      << std::endl;
#else
            throw std::runtime_error("Please supply the log file name.\n"
                                     "Usage: EmoStateLogger [log_file_name].");
#endif
#endif
        } else {
            filename = argv[1];
        }

        std::cout << "==================================================================="
                  << std::endl;
        std::cout << "Example to show how to log Motion Data from EmoDriver."
                  << std::endl;
        std::cout << "==================================================================="
                  << std::endl;

        
		if (IEE_EngineConnect() != EDK_OK) 
			throw std::runtime_error("Emotiv Driver start up failed.");
		
        std::cout << "Start receiving IEEG Data! "
                  << "Press any key to stop logging...\n"
                  << std::endl;

        std::ofstream ofs(filename.c_str(),std::ios::trunc);
		ofs << header << std::endl;
		
		DataHandle hMotionData = IEE_MotionDataCreate();
		IEE_MotionDataSetBufferSizeInSec(secs);

		std::cout << "Buffer size in secs:" << secs << std::endl;
		
		while (!_kbhit()) {

			state = IEE_EngineGetNextEvent(eEvent);
			if (state == EDK_OK) {

				IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
				IEE_EmoEngineEventGetUserId(eEvent, &userID);

				// Log the EmoState if it has been updated
				if (eventType == IEE_UserAdded) {
					std::cout << "User added";
					readytocollect = true;
				}
			}

			if (readytocollect) {
						
                IEE_MotionDataUpdateHandle(0, hMotionData);

                unsigned int nSamplesTaken=0;
                IEE_MotionDataGetNumberOfSample(hMotionData, &nSamplesTaken);

                std::cout << "Updated " << nSamplesTaken << std::endl;

                if (nSamplesTaken != 0) {

                    double* data = new double[nSamplesTaken];
                    for (int sampleIdx=0 ; sampleIdx<(int)nSamplesTaken ; ++ sampleIdx) {
                        for (int i = 0 ;
                             i<sizeof(targetChannelList)/sizeof(IEE_MotionDataChannel_t) ;
                             i++) {

                            IEE_MotionDataGet(hMotionData, targetChannelList[i],
                                        data, nSamplesTaken);
                            ofs << data[sampleIdx] << ",";
                        }
                        ofs << std::endl;
                    }
                    delete[] data;
                }

			}

#ifdef _WIN32
            Sleep(1);
#endif
#if __linux__ || __APPLE__
            usleep(10000);
#endif
		}

		ofs.close();
		IEE_MotionDataFree(hMotionData);
		

	}
    catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		std::cout << "Press any key to exit..." << std::endl;
		getchar();
	}

	IEE_EngineDisconnect();
	IEE_EmoStateFree(eState);
	IEE_EmoEngineEventFree(eEvent);

	return 0;
}

#ifdef __linux__
int _kbhit(void)
{
    struct timeval tv;
    fd_set read_fd;

    tv.tv_sec=0;
    tv.tv_usec=0;

    FD_ZERO(&read_fd);
    FD_SET(0,&read_fd);

    if(select(1, &read_fd,NULL, NULL, &tv) == -1)
    return 0;

    if(FD_ISSET(0,&read_fd))
        return 1;

    return 0;
}
#endif
#ifdef __APPLE__
int _kbhit (void)
{
    struct timeval tv;
    fd_set rdfs;

    tv.tv_sec = 0;
    tv.tv_usec = 0;

    FD_ZERO(&rdfs);
    FD_SET (STDIN_FILENO, &rdfs);

    select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &rdfs);
}
#endif
