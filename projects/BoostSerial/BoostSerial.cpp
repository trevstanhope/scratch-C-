#include <iostream>
#include "SimpleSerial.h"

using namespace std;
using namespace boost;

int main(int argc, char* argv[])
{

                SimpleSerial serial("/dev/ttyUSB0",115200);

                serial.writeString("Hello world\n");

                cout<<serial.readLine()<<endl;
}
