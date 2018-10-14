#ifndef EMULATIONSTATION_ALL_GamestationUpgrade_H
#define EMULATIONSTATION_ALL_GamestationUpgrade_H

#include "GamestationSystem.h"

class GamestationUpgrade {

public:
    static GamestationUpgrade *getInstance();

    std::string getVersion();

    std::string getUpdateVersion();

    bool updateLastChangelogFile();

    std::string getChangelog();

    std::pair<std::string, int> updateSystem(BusyComponent *ui);

    std::string getUpdateChangelog();

    bool canUpdate();

private:
    GamestationSystem *system = GamestationSystem::getInstance();
    static GamestationUpgrade *instance;

};


#endif //EMULATIONSTATION_ALL_GamestationUpgrade_H
