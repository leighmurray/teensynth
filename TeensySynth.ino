#include "DisplayLib.h"
#include "SynthLib.h"

void setup ()
{
  synthSetup();
  displaySetup();
}

void loop ()
{
  synthLoop();
  displayLoop();
}
