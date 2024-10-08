#pragma once
#include "DataStructure.h"

/// @brief 映射 -> 字符串
bool map2str(const std::map<std::string, std::string>& src, std::string& dst);

/// @brief 字符串 -> 映射
bool str2map(const std::string& src, std::map<std::string, std::string>& dst);

/// @brief 序列化结构体
std::string area2str(const st_tf::Area& _area);

/// @brief 反序列化结构体
void str2area(const std::string& _data, st_tf::Area& _area);
