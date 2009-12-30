//
// C++ Implementation: debuglog
//
// Description: 
// 
//
// Author: ny,,, <ny.softwared@gmx.at>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "debuglog.hh"



DebugLog::DebugLog()
{
	out.open("log.txt");	
}

DebugLog& operator<<(DebugLog& log, string s)
{
	log.out<<s<<endl;
}

DebugLog::~DebugLog()
{
	out.close();
}


