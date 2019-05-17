/////////////////////////////////////////////////////////////////////////////
// Shim.cpp       : provides definitions and test stub for Shim.h          //
// ver 1.0                                                                 //
//                                                                         //
// Application    :  C++/CLI Shim                                          //
// Platform       : Visual Studio 2017 Community Edition                   //
//                 HP intel Core i7                                        //
// Author         : Arjun Kalyanasundaram, Syracuse University             //
/////////////////////////////////////////////////////////////////////////////

#include "Shim.h"

// -----< Shim constructor - instantiates IPublisher >------------------
Shim::Shim() {
  Console::Write("\n  Shim created");
  ObjectFactory factory;
  // uses factory to get instance of CppClient
  client = factory.createClient();
  Console::Write("\n  Shim instantiated CppClient as IPublisher*");
}


//// -----< Shim destructor - deletes IPublisher>------------------------
Shim::~Shim() {
  delete client;
  Console::Write("\n  Shim destroyed\n\n");
}

// -----< Function that sends commands and returns converted list of files>------------------
List <String^>^ Shim::SendCmd(List<String^>^ args)
{
	int argc = args->Count;
	char ** argv = new char*[argc];
	for (int i = 0; i < argc; i++)
	{
		std::string arg = sysStrToStdStr(args[i]);        //converting system string to standard string
		System::Console::WriteLine(args[i]);
		argv[i] = new char[args[i]->Length];
		strcpy_s(argv[i], args[i]->Length + 1, arg.c_str());
	}
	client->processCommandLineArgs(argc, argv);    //cient sends commands argc and argv to CodePublisher
	client->extractFiles();
	std::vector<std::string> vec = client->publish();               
	delete[] argv;
	int size = vec.size();
	List<String^>^con_list = gcnew List<String^> (size);
	String^ adder;
	std::string normal;
	for (auto const& strn : vec) 
	{
		adder = stdStrToSysStr(strn);
		con_list->Add(adder);
	}
	return con_list;    // returns the converted files in the form of  a list
}


//----< convert std::string to System.String >-------------------------
String^ Shim::stdStrToSysStr(const std::string& str) {
  return gcnew String(str.c_str());
}

//----< convert System.String to std::string >-------------------------
std::string Shim::sysStrToStdStr(String^ str) {
  std::string temp;
  for (int i = 0; i < str->Length; ++i)
    temp += char(str[i]);
  return temp;
}


// -----< Shim Test Stub >---------------------------------------------

#ifdef SHIM_TEST1
int main(List<System::String^> ^args)
{

	Shim localShim;  // C++ style construction
					 // sleep for 100ms to allow better formatted output
	System::Threading::Thread::Sleep(100);
	Console::Write("\n\n  Sending Arguments to Sendcmd:");
	Console::Write("\n =========================================\n");
	localShim.SendCmd(args);
	Console::Write("\n\n  Finished demonstration.");
	Console::Write("\n  Shim dtor is automatically called after 'main' goes out of scope.\n");

	return 0;
} // NOTE: Shim destructor will automatically be called here just like in native C++

#endif // SHIM_TEST1

