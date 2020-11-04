//#include "DisplayManager.h"
#include "SynthLib.h"
//#include "Menu.h"
#include "DisplayLib.h"


//DisplayManager displayManager = DisplayManager();

void setup ()
{
  synthSetup();
  displaySetup();
  //menuSetup();
}

void loop ()
{
  synthLoop();
  displayLoop();
  //menuLoop();
}
