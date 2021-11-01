

//#include "DisplayManager.h"
#include "SynthLib.h"
#include "Menu.h"
#include <TeensyThreads.h>
//#include "DisplayLib.h"
#include "UsbMidiHost.h"

//DisplayManager displayManager = DisplayManager();

void setup ()
{
  synthSetup();
  //displaySetup();
  menuSetup();
  threads.addThread(menuLoop);
  usbMidiHostSetup();
}

void loop ()
{
  handleMainEncoder();
  usbMidiHostLoop();
  synthLoop();
}
