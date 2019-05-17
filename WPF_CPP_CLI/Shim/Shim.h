/////////////////////////////////////////////////////////////////////////////
// Shim.h  : defines a wrapper for IPublisher which works with .NET code   //
// ver 1.0                                                                 //
//                                                                         //
// Application    :  C++/CLI Shim   to translate C++ code to C#            //
// Platform       : Visual Studio 2017 Community Edition                   //
//                   HP intel core i7                                      //
// Author         :Arjun Kalyanasundaram, Syracuse University              //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
/*
*  Package Description:
* ======================
*  This package defines Shim class which can be used by .NET applications.
*  Here the Shim is used in WPF project to allow making calls from
*  WPF C# code to CodePublisher class in  native C++ code. The Shim is a mangaged (.NET) 
*  wrapper around IPublisher  interface and can make calls to native C++
*  classes and, in the same time, can be called by any .NET code (C# here).
*  In this package the function  is defined to send command line arguments to 
*  the CodePublisher
*  Public Interface:
* ===================
*  List <String^>^ SendCmd(List <String^>^ args);
*  Required Files:
* =================
*  Shim.h Shim.cpp IPublisher.h
*
*  Build Command:
* ================
*  msbuild Shim.vcxproj
*
*  Maintainence History:
* =======================
*  ver 1.0 -April 6th 2019
*    - first release
*/

#pragma once

#include <string>
#include "../../CodePublisher/IPublisher.h"
using namespace System::Collections::Generic;
using namespace System;
public ref class Shim {
public:
  Shim();
  ~Shim();
   List <String^>^ SendCmd(List <String^>^ args);    // function to send commands to CodePublisher

private:
  std::string sysStrToStdStr(String^ str);          // convert System::String to std::string    
  String^ stdStrToSysStr(const std::string& str);   //convert std::string to System::String
   IPublisher* client;								// this will point to IPublisher when instantiated through the ObjectFactory
};
