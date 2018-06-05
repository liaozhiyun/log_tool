/*
 * log.h
 *
 *  Created on: 2016Äê5ÔÂ24ÈÕ
 *      Author: fandianyf
 */

#ifndef UTIL_LOG_H_
#define UTIL_LOG_H_

#include <string>
#include <iostream>
#include <boost/log/trivial.hpp>

class MyLog
{
public:
  MyLog();
  ~MyLog(void);
 
  static void Init(const std::string & dir);
  static void Log(const std::string & msg );
  static boost::log::sources::severity_logger<boost::log::trivial::severity_level > s_slg;
};




#endif /* UTIL_LOG_H_ */
