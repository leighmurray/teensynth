#include "USBHost_t36.h"

USBHost myusb;
USBHub hub1(myusb);
MIDIDevice midi1(myusb);

void usbMidiHostLoop()
{
  myusb.Task();
  midi1.read();
}

void OnNoteOn(byte channel, byte note, byte velocity)
{
  myNoteOn(channel, note, velocity);
}

void OnNoteOff(byte channel, byte note, byte velocity)
{
  myNoteOff(channel, note, velocity);
}

void OnControlChange(byte channel, byte control, byte value)
{
  myControlChange(channel, control, value);
}

void usbMidiHostSetup()
{
  myusb.begin();
  midi1.setHandleNoteOff(OnNoteOff);
  midi1.setHandleNoteOn(OnNoteOn);
  midi1.setHandleControlChange(OnControlChange);
}
