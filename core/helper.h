#ifndef HELPER_H
#define HELPER_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <map>
#include <glog/logging.h>
#include "timer.h"


namespace porcupine
{
/**
* @brief Characters set
*/
extern const std::string gComplexChars;

/**
* @brief Default config file name
*/
extern const std::string gDefaultConfigFile;

/**
* @brief record the start time
*/
extern TimeInSec gStartTime;

/**
* do some initial work
*/
void InitPorcupine();

/* split source into num parts by sep */
std::vector<std::string> Split(std::string source, std::string sep = " ", int num = INT_MAX);

/* trim character from source's left */
std::string Ltrim(std::string source, char character = ' ');

/* trim character from source's right */
std::string Rtrim(std::string source, char character = ' ');

/* trim character from left and right */
std::string Trim(std::string source, char character = ' ');

/* Generate random string */
std::string GetRandomString(int32_t length);

/* Transfer int to string */
std::string IntToString(int64_t x);

/* Get hostname */
std::string GetHostName();

template <typename T>
T GetParameter(std::map<std::string, std::string>& params, const std::string& key, T value);

/*
* @brief polynomial multiplication 
* @param p1 p1[i] is the exponent, e.g. x^(p1[i])
* @return same format with p1
*/

std::vector<double> PloyNormialMultiplication(const std::vector<double> p1, const std::vector<double> p2);

}
#endif
