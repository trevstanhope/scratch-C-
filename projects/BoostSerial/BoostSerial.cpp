#include <iostream>
#include "SimpleSerial.h"

using namespace std;
using namespace boost;

int main(int argc, char** argv)
{
  string port = argv[1]; // the device path
  int baud = atoi(argv[2]); // parse string to integer
  try {
    SimpleSerial serial(port, baud);
    serial.writeString("Hello world\n");
    cout << serial.readLine() << endl;
  }
  catch(boost::system::system_error& e) {
    cout << "Error: " << e.what() << endl;
    return 1;
  }
}
