#include "system_connection.h"

void system_connection(){
#if defined __linux__
  struct utsname utsname; // объект для структуры типа utsname

  uname(&utsname); // передаем объект по ссылке

  // распечатаем на экране информацию об операционной системе
  std::cout << "OS name: " << utsname.sysname << std::endl;
  std::cout << "OS release: " << utsname.release << std::endl;
  std::cout << "OS version: " << utsname.version << std::endl;

#else
  const DWORD encodedVersion = ::GetVersion();
  const unsigned majorVersion = unsigned(LOBYTE(LOWORD(encodedVersion)));
  const unsigned minorVersion = unsigned(HIBYTE(LOWORD(encodedVersion)));
  std::cout << "Running under Windows " << majorVersion << "." << minorVersion << std::endl;
#endif
}
