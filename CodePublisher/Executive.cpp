/////////////////////////////////////////////////////////////////////
//  Executive.cpp												   //
//  ver 1.0														   //
//  Language:      Visual C++ 2017                                 //
//  Platform:     HP intel core i7                                 //														   //
//  Author:		  Arjun Kalyanasundaram	Syracuse University	       //
//				  akalyana@syr.edu								   //
/////////////////////////////////////////////////////////////////////

#define IN_DLL
#include<iostream>
#include "Executive.h"
#include "../Logger/Logger.h"

using namespace Utilities;
using namespace LoggingC;
using namespace FileSystem;

ProcessCmdLine::Usage customUsage();

CodePublisher::CodePublisher()
{

}
// -----< demonstrating requirement 1 >----------------------------------------------
void demoreq1()
{
	std::cout << ("----------<Demonstrating requirement 1>--------------\n");
	std::cout<<("\n");
	std::cout<<("Using  Visual Studio 2017 and its C++ Windows Console Projects \n");
}

// --------------------< demonstrating requirement 2 >----------------------------------------------

void demoreq2()
{
	std::cout << ("\n");
	std::cout << ("----------<Demonstrating requirement 2>--------------\n");
	std::cout << ("using  Windows Presentation Foundation (WPF) and C# to implement the Client GUI \n");
	std::cout << ("Code Fragments:\n");
	std::cout << ("private string WDirectory { get; set; }\n");
	
}
// -----< demonstrating  requirement 3 >----------------------------------------------
void demoReq3() {
	std::cout << ("\n");
	std::cout << ("-----<Demonstrating requirement 3>--------\n");
	std::cout << ("Provide an interface for the Code Publisher developed in Project #2, it declares methods to access all of the Publisher's facilities,\n"); 
	std::cout << ("and provides an object factory that returns a pointer to the interface. \n");
	std::cout << ("Code Fragments:\n");
	std::cout << (" struct ObjectFactory {\n");
	std::cout << ("DLL_DECL IPublisher* createClient();\n");
	std::cout << ("};\n");		
}

// -----< demonstrating requirements 4>---------------------------------------
void demoreq4() {
	std::cout << ("\n");
	std::cout << ("-----<Demonstrating requirement 4>-------\n");
	std::cout << ("Built the Publisher as a dynamic link library and exposed  it to the interface and object factory.\n");
	
}

// -----< demonstrating requirement 5 >------------------------------

void demoReq5() {
	std::cout << ("\n");
	std::cout << ("----------<Demonstrating requirement 5>--------------\n");
	std::cout << ("provided a C++\CLI translater that delegates calls from the Client GUI into the publisher's methods, as declared in its interface.\n");
}


// -----< demonstrating requirement 6 >-------------------------------------

void demoReq6() {
	std::cout << ("\n");
	std::cout << ("----------<Demonstrating requirement 6>--------------\n");
	std::cout << ("provided a Client package that implements a WPF GUI with two views, one for navigation, and one to display converted files, as described in the purpose section.\n");

}
// -----< demonstrating requirement 7 >-------------------------------------

void demoReq7()
{
	std::cout << ("\n");
	std::cout << ("----------<Demonstrating requirement 7>--------------\n");
	std::cout << ("An automated test includes a path to the  project directory on its command line, invokes the Code Publisher, through its interface, to convert all the project files matching a pattern that accepts *.h and *.cpp files, and then it opens the Client GUI's Display view.\n");
	std::cout << ("The automated test demonstrates all the requirements ");

}

// -----< Creating aclient from object factory using interface>-------------------------------------
IPublisher* ObjectFactory::createClient()
{
	return new CodePublisher;

}

// -----< process cmd to get info >------------------------------------
bool CodePublisher::processCommandLineArgs(int argc, char ** argv)
{
	pcl_ = new ProcessCmdLine(argc, argv);
	pcl_->usage(customUsage());
	if (pcl_->parseError())
	{
		pcl_->usage();
		std::cout << "\n\n";
		return false;
	}
	dirIn_ = pcl_->path();
	return true;
}

// -----< return input directory - from PCL >----------------------------
const std::string & CodePublisher::workingDirectory() const
{
	return dirIn_;
}

// -----< set output directory >-----------------------------------------
void CodePublisher::outputDirectory(const std::string & dir)
{
	dirOut_ = dir;
	cconv_.outputDir(dir);
}

// -----< return output directory >--------------------------------------
const std::string & CodePublisher::outputDirectory() const
{
	return dirOut_;
}

// -----< extract files - after processing cmd >-------------------------
bool CodePublisher::extractFiles()
{
	DirExplorerN de(pcl_->path());

	for (auto patt : pcl_->patterns())
	{
		de.addPattern(patt);
	}

	if (pcl_->hasOption("s"))
	{
		de.recurse();
	}

	bool res = de.search() && de.match_regexes(pcl_->regexes());
	de.showStats();

	files_ = de.filesList();

	return res;
}

// -----< publish - files extracted from directory explorer>---------------
// -----<Returns a vector of converted files>---------------
std::vector<std::string> CodePublisher::publish()
{
	demoreq1();
	demoreq2();
	demoReq3();
	demoreq4();
	demoReq5();
	demoReq6();
	demoReq7();
	std::vector<std::string> ak = cconv_.convert(files_);   
	return ak;                
}

// -----< publish - single file >-------------------------------------------
void CodePublisher::publish(const std::string & file)
{
	display_.display(cconv_.convert(file));
}

// -----< publish - must provide list >-------------------------------------


void CodePublisher::publish(const std::vector<std::string>& files)
{
	display_.display(cconv_.convert(files));
}

// -----< gets display mode as set by PCL >---------------------------------


Utilities::DisplayMode CodePublisher::displayMode() const
{
	return pcl_->displayMode();
}


// -----< command line usage >----------------------------------------------
ProcessCmdLine::Usage customUsage()
{
	std::string usage;
	usage += "\n  Command Line: path [/option]* [/pattern]* [/regex]*";
	usage += "\n    path is relative or absolute path where processing begins";
	usage += "\n    [/option]* are one or more options of the form:";
	usage += "\n      /s     - walk directory recursively";
	usage += "\n      /demo  - run in demonstration mode (cannot coexist with /debug)";
	usage += "\n      /debug - run in debug mode (cannot coexist with /demo)";
	//usage += "\n      /h - hide empty directories";
	//usage += "\n      /a - on stopping, show all files in current directory";
	usage += "\n    [pattern]* are one or more pattern strings of the form:";
	usage += "\n      *.h *.cpp *.cs *.txt or *.*";
	usage += "\n    [regex] regular expression(s), i.e. [A-B](.*)";
	usage += "\n";
	return usage;
}

// -----< initialize loggers in proper display mode >------------------------

void initializeLoggers(DisplayMode dm) {
	if (dm == DisplayMode::Debug) {
		LoggerDebug::attach(&std::cout);
		LoggerDebug::start();
	}

	else if (dm == DisplayMode::Demo) {
		LoggerDemo::attach(&std::cout);
		LoggerDemo::start();
	}
}

#ifdef USE_CP

#include <iostream>

// -----< NORMAL USE MAIN > ------------------------------------------
int main(int argc, char ** argv) {
	CodePublisher cp;
	if (!cp.processCommandLineArgs(argc, argv)) {
		std::cout << "\n  Failed to process command line arguments. Terminating\n\n";
		return 1;
	}

	initializeLoggers(cp.displayMode());

	if (!cp.extractFiles()) {
		stopLoggers();
		std::cout << "\n  Found no files to process. Terminating.\n\n";
		return 0;
	}

	cp.publish();
	stopLoggers();
	return 0;
}

#endif