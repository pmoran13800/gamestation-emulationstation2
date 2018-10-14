#include "GamestationConf.h"
#include <iostream>
#include <fstream>
#include <boost/regex.hpp>
#include "Log.h"
#include <boost/algorithm/string/predicate.hpp>

GamestationConf *GamestationConf::sInstance = NULL;
boost::regex validLine("^(?<key>[^;|#].*?)=(?<val>.*?)$");
boost::regex commentLine("^;(?<key>.*?)=(?<val>.*?)$");

std::string gamestationConfFile = "/gamestation/share/system/gamestation.conf";
std::string gamestationConfFileInit = "/gamestation/share_init/system/gamestation.conf";
std::string gamestationConfFileTmp = "/gamestation/share/system/gamestation.conf.tmp";

GamestationConf::GamestationConf(bool mainFile) {
    loadGamestationConf(mainFile);
}

GamestationConf::~GamestationConf() {
	if (sInstance && sInstance == this)
		delete sInstance;
}

GamestationConf *GamestationConf::getInstance() {
    if (sInstance == NULL)
        sInstance = new GamestationConf();

    return sInstance;
}

bool GamestationConf::loadGamestationConf(bool mainFile) {
    std::string line;
    std::string filePath = mainFile ? gamestationConfFile : gamestationConfFileInit;
    std::ifstream gamestationConf(filePath);
    if (gamestationConf && gamestationConf.is_open()) {
        while (std::getline(gamestationConf, line)) {
            boost::smatch lineInfo;
            if (boost::regex_match(line, lineInfo, validLine)) {
                confMap[std::string(lineInfo["key"])] = std::string(lineInfo["val"]);
            }
        }
        gamestationConf.close();
    } else {
        LOG(LogError) << "Unable to open " << filePath;
        return false;
    }
    return true;
}


bool GamestationConf::saveGamestationConf() {
    std::ifstream filein(gamestationConfFile); //File to read from
    if (!filein) {
        LOG(LogError) << "Unable to open for saving :  " << gamestationConfFile << "\n";
        return false;
    }
    /* Read all lines in a vector */
    std::vector<std::string> fileLines;
    std::string line;
    while (std::getline(filein, line)) {
        fileLines.push_back(line);
    }
    filein.close();


    /* Save new value if exists */
    for (std::map<std::string, std::string>::iterator it = confMap.begin(); it != confMap.end(); ++it) {
        std::string key = it->first;
        std::string val = it->second;
        bool lineFound = false;
        for (int i = 0; i < fileLines.size(); i++) {
            std::string currentLine = fileLines[i];

            if (boost::starts_with(currentLine, key+"=") || boost::starts_with(currentLine, ";"+key+"=")){
                fileLines[i] = key + "=" + val;
                lineFound = true;
            }
        }
        if(!lineFound){
            fileLines.push_back(key + "=" + val);
        }
    }
    std::ofstream fileout(gamestationConfFileTmp); //Temporary file
    if (!fileout) {
        LOG(LogError) << "Unable to open for saving :  " << gamestationConfFileTmp << "\n";
        return false;
    }
    for (int i = 0; i < fileLines.size(); i++) {
        fileout << fileLines[i] << "\n";
    }

    fileout.close();


    /* Copy back the tmp to gamestation.conf */
    std::ifstream  src(gamestationConfFileTmp, std::ios::binary);
    std::ofstream  dst(gamestationConfFile,   std::ios::binary);
    dst << src.rdbuf();

    remove(gamestationConfFileTmp.c_str());

    return true;
}

std::string GamestationConf::get(const std::string &name) {
    if (confMap.count(name)) {
        return confMap[name];
    }
    return "";
}
std::string GamestationConf::get(const std::string &name, const std::string &defaultValue) {
    if (confMap.count(name)) {
        return confMap[name];
    }
    return defaultValue;
}

bool GamestationConf::getBool(const std::string &name, bool defaultValue) {
    if (confMap.count(name)) {
        return confMap[name] == "1";
    }
    return defaultValue;
}

unsigned int GamestationConf::getUInt(const std::string &name, unsigned int defaultValue) {
    try {
        if (confMap.count(name)) {
            int value = std::stoi(confMap[name]);
            return value > 0 ? (unsigned int) value : 0;
        }
    } catch(std::invalid_argument&) {}

    return defaultValue;
}

void GamestationConf::set(const std::string &name, const std::string &value) {
    confMap[name] = value;
}

void GamestationConf::setBool(const std::string &name, bool value) {
    confMap[name] = value ? "1" : "0";
}

void GamestationConf::setUInt(const std::string &name, unsigned int value) {
    confMap[name] = std::to_string(value).c_str();
}

bool GamestationConf::isInList(const std::string &name, const std::string &value) {
    bool result = false;
    if (confMap.count(name)) {
        std::string s = confMap[name];
        std::string delimiter = ",";

        size_t pos = 0;
        std::string token;
        while (((pos = s.find(delimiter)) != std::string::npos) ) {
            token = s.substr(0, pos);
            if (token == value)
	            result = true;
            s.erase(0, pos + delimiter.length());
        }
	    if (s == value)
		    result = true;
    }
    return result;
}