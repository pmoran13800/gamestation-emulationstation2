//
// Created by matthieu on 12/09/15.
//

#ifndef EMULATIONSTATION_ALL_GAMESTATIONCONF_H
#define EMULATIONSTATION_ALL_GAMESTATIONCONF_H


#include <string>
#include <map>

class GamestationConf {

public:
    GamestationConf(bool mainFile = true);

    ~GamestationConf();

    bool loadGamestationConf(bool mainFile = true);

    bool saveGamestationConf();

    std::string get(const std::string &name);
    std::string get(const std::string &name, const std::string &defaultValue);
    bool getBool(const std::string& name, bool defaultValue = false);
    unsigned int getUInt(const std::string& name, unsigned int defaultValue = 0);

    void set(const std::string &name, const std::string &value);
    void setBool(const std::string &name, bool value);
    void setUInt(const std::string &name, unsigned int value);

    bool isInList(const std::string &name, const std::string &value);

    static GamestationConf *sInstance;

    static GamestationConf *getInstance();
private:
    std::map<std::string, std::string> confMap;

};


#endif //EMULATIONSTATION_ALL_GAMESTATIONCONF_H
