/* 
 * File:   NetworkThread.cpp
 * Author: matthieu
 * 
 * Created on 6 février 2015, 11:40
 */

#include "NetworkThread.h"
#include "gamestation/GamestationSystem.h"
#include "gamestation/GamestationUpgrade.h"
#include "GamestationConf.h"
#include "Locale.h"

NetworkThread::NetworkThread(Window* window) : mWindow(window){
    mFirstRun = true;
    mRunning = true;
    mThreadHandle = new boost::thread(boost::bind(&NetworkThread::run, this));
}

NetworkThread::~NetworkThread() {
    mThreadHandle->join();
}

void NetworkThread::run(){
    while(mRunning){
        if(mFirstRun){
            boost::this_thread::sleep(boost::posix_time::seconds(15));
            mFirstRun = false;
        } else {
            boost::this_thread::sleep(boost::posix_time::hours(1));
        }

        if (GamestationUpgrade::getInstance()->canUpdate()) {
            if(GamestationConf::getInstance()->get("updates.enabled") == "1") {
                std::string changelog = GamestationUpgrade::getInstance()->getUpdateChangelog();

                while (mWindow->isShowingPopup()) {
                    boost::this_thread::sleep(boost::posix_time::seconds(5));
                }

                if (changelog != "") {
                    std::string message = changelog;
                    std::string updateVersion = GamestationUpgrade::getInstance()->getUpdateVersion();
                    mWindow->displayScrollMessage(_("AN UPDATE IS AVAILABLE FOR YOUR GAMESTATION"),
                                                _("UPDATE VERSION:") + " " + updateVersion + "\n" +
                                                _("UPDATE CHANGELOG:") + "\n" + message);
                } else {
                    mWindow->displayMessage(_("AN UPDATE IS AVAILABLE FOR YOUR GAMESTATION"));
                }
            }
            mRunning = false;
        }
    }
}

