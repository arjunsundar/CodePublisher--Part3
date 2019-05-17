/////////////////////////////////////////////////////////////////////
//  IPublisher.h									               //
//  ver 1.0														   //
//  Language:      Visual C++ 2017                                 //
//  Platform:     HP intel core i7                                 //														  
//  Author:		  Arjun Kalyanasundaram	Syracuse University	       //
//				  akalyana@syr.edu								   //
/////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
* This package creates an interface to communicate with the CodePublisher.
*This is done by using the object factory. Through this package virtual functions are
*defined and then accesed by the client in the Shim
*  Required Files:
* =======================
* -
*  Maintainence History:
* =======================
*  ver 1.0 - 6 April 2019
*  - first release
*/

#pragma once
#include<vector>
#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

//---<interface>-----
//---<Class IPublisher is created>-----
class IPublisher {
public:
	virtual bool processCommandLineArgs(int argc, char ** argv) = 0;
	virtual bool extractFiles() = 0;
	virtual std::vector<std::string> publish() = 0;
	virtual ~IPublisher() {};

};

//---<Using Keyword Extern>-----
extern "C" {
	// Instantiates CodePublisher class 
	struct ObjectFactory {
		DLL_DECL IPublisher* createClient();
	};
}