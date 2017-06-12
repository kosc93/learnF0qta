/*
 * SplitString.h
 *
 *  Created on: 12 June 2017
 *      Author: patrick
 */

#ifndef SPLITSTRING_H_
#define SPLITSTRING_H_

#include <string>
#include <vector>

void split( std::vector<std::string> & theStringVector,  /* Altered/returned value */
       const  std::string  & theString,
       const  std::string  & theDelimiter);

#endif /* SPLITSTRING_H_ */
