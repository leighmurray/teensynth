

//#include "DisplayManager.h"
#include "SynthLib.h"
#include "Menu.h"
#include <TeensyThreads.h>
//#include "DisplayLib.h"


//DisplayManager displayManager = DisplayManager();

void setup ()
{
  synthSetup();
  //displaySetup();
  menuSetup();
  threads.addThread(menuLoop);
}

void loop ()
{
  synthLoop();
}
