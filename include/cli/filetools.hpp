#pragma once

#include <map>
#include <fstream>
#include <string>

typedef std::map<std::string, std::string> configmap;

configmap file_to_map(std::string filepath);
void map_to_file(configmap map, std::string filepath);
