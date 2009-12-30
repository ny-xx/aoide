//
// C++ Interface: debuglog
//
// Description: 
//
//
// Author: ny,,, <ny.softwared@gmx.at>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DEBUGLOG_H
#define DEBUGLOG_H

#include <fstream> 
#include <string>

using namespace std;

class DebugLog
{
public:
	DebugLog();
    ~DebugLog();
	
	ofstream out;
private:
	
};

DebugLog& operator<<(DebugLog& log, string s);

#endif
