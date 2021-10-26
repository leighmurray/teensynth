
//      ******************************************************************
//      *                                                                *
//      *   Example shows how to prompt the user for numbers and values  *
//      *                                                                *
//      *            S. Reifel & Co.                8/8/2019             *
//      *                                                                *
//      ******************************************************************

//
// DOCUMENTATION:
//
// Documentation for the "Teensy User Interface" library can be found at:
//    https://github.com/Stan-Reifel/TeensyUserInterface



// ***********************************************************************

#define ENCODER_OPTIMIZE_INTERRUPTS
#include <TeensyUserInterface.h>
#include <font_Arial.h>
#include <font_ArialBold.h>
#include "SynthLib.h"
#include <Encoder.h>

void commandGetAnInteger(void);
void commandGetAFloat(void);
void oscillatorOneChoices(void);
void oscillatorTwoChoices(void);
void ampADSRChoices(void);
void mixerChoices(void);
void enableSelfDestructCallback(void);
//
// create the user interface object
//
TeensyUserInterface ui;
Encoder mainEncoder(4, 5);
int encoderCC = CCmixer1;

// ---------------------------------------------------------------------------------
//                                 Setup the hardware
// ---------------------------------------------------------------------------------

void setControlChange(byte controlChange, byte value);

void menuSetup() 
{
  //
  // initialize the user interface, including setting the screen orientation and font
  //
  ui.begin(LCD_ORIENTATION_LANDSCAPE_4PIN_LEFT, Arial_9_Bold);

  //
  // use a grayscale color palette
  //
  ui.setColorPaletteGray();
}


// ---------------------------------------------------------------------------------
//           Define the menus and top level loop, place menus after setup()
// ---------------------------------------------------------------------------------

//
// Notes on building the menu table:
//
// This example uses many "Command" button, exactly like we have used in the
// earlier examples.  Here we introduce a new menu item, the "Toggle" button.
//
// A MENU_ITEM_TYPE_TOGGLE is used somewhat like a Radio Button in a dialog 
// box.  It allows the user to select one of a fixed number of choices (such as  
// On / Off,   or   Red / Green / Blue).  Each time the user presses this button, 
// it alternates the selection (i.e. toggles between On and Off, or rotates between 
// Red, Green and Blue).  The third field in this entry points to a callback 
// function that alternates the value.
//
// The "Self Destruct" button in this example uses the Toggle feature.


//
// for each menu, create a forward declaration with "extern"
//
extern MENU_ITEM mainMenu[];


//
// the main menu
//
MENU_ITEM mainMenu[] = {
  {MENU_ITEM_TYPE_MAIN_MENU_HEADER,  "TeenSynth",                                 MENU_COLUMNS_2,              mainMenu},
  {MENU_ITEM_TYPE_COMMAND,           "Oscillator 1",                              oscillatorOneChoices,        NULL},
  {MENU_ITEM_TYPE_COMMAND,           "Oscillator 2",                              oscillatorTwoChoices,        NULL},
  {MENU_ITEM_TYPE_COMMAND,           "ADSR",                                      ampADSRChoices,              NULL},
  {MENU_ITEM_TYPE_COMMAND,           "Mixer",                                     mixerChoices,                NULL},
  {MENU_ITEM_TYPE_TOGGLE,            "Self destruct",                             enableSelfDestructCallback,  NULL},
  {MENU_ITEM_TYPE_END_OF_MENU,       "",                                          NULL,                        NULL}
};

int previousEncoderValue = 0;

void handleMainEncoder() {
  long currentEncoderValue = mainEncoder.read();

  if (previousEncoderValue != currentEncoderValue) {
    int modulus = currentEncoderValue % 4;
    if (modulus == 0)
    {
      setControlChange(encoderCC, currentEncoderValue*4);
    }
  }
  previousEncoderValue = currentEncoderValue;
  
}

//
// display the menu, then execute commands selected by the user
//
void menuLoop() 
{  
  ui.displayAndExecuteMenu(mainMenu);
}

void setControlChange(byte controlChange, byte value){
  encoderCC = controlChange;
  myControlChange(1, controlChange, value);
}


// ---------------------------------------------------------------------------------
//                            Commands executed from the menu
// ---------------------------------------------------------------------------------

//
// menu command that demonstrates how to prompt the user for an integer
//
static int xOffsetValue = 50;

void commandGetAnInteger(void)
{
  char sBuffer[25];
  
  //
  // draw the title bar and clear the screen
  //
  ui.drawTitleBar("Prompt User for an Integer");
  ui.clearDisplaySpace();
  
  //
  // set the size and initial value of the number box
  //
  const int numberBoxWidth = 200;
  const int numberBoxAndButtonsHeight = 35;

  //
  // define a Number Box so the user can select a numeric value, specify the initial value, 
  // max and min values, and step up/down amount
  //
  NUMBER_BOX my_NumberBox;
  my_NumberBox.labelText     = "Set X offset";
  my_NumberBox.value         = xOffsetValue;
  my_NumberBox.minimumValue  = -200;
  my_NumberBox.maximumValue  = 200;
  my_NumberBox.stepAmount    = 2;
  my_NumberBox.centerX       = ui.displaySpaceCenterX;
  my_NumberBox.centerY       = ui.displaySpaceCenterY - 20;
  my_NumberBox.width         = numberBoxWidth;
  my_NumberBox.height        = numberBoxAndButtonsHeight;
  ui.drawNumberBox(my_NumberBox);


  //
  // define and display "OK" and "Cancel" buttons
  //
  BUTTON okButton        = {"OK",      ui.displaySpaceCenterX-70, ui.displaySpaceBottomY-35,  120 , numberBoxAndButtonsHeight};
  ui.drawButton(okButton);

  BUTTON cancelButton    = {"Cancel",  ui.displaySpaceCenterX+70, ui.displaySpaceBottomY-35,  120 , numberBoxAndButtonsHeight};
  ui.drawButton(cancelButton);


  //
  // process touch events
  //
  while(true)
  {
    ui.getTouchEvents();

    //
    // process touch events on the Number Box
    //
    ui.checkForNumberBoxTouched(my_NumberBox);
 
    //
    // check for touch events on the "OK" button
    //
    if (ui.checkForButtonClicked(okButton))
    {
      //
      // user OK pressed, get the value from the Number Box and display it
      //
      xOffsetValue = my_NumberBox.value;
      sprintf(sBuffer, "X Offset = %d", xOffsetValue);

      ui.clearDisplaySpace();      
      ui.lcdSetCursorXY(ui.displaySpaceCenterX, ui.displaySpaceCenterY-10);
      ui.lcdPrintCentered(sBuffer);
      delay(1500);
      return;
    }

    //
    // check for touch events on the "Cancel" button
    //
    if (ui.checkForButtonClicked(cancelButton))
      return;
  }
}



//
// menu command that demonstrates how to prompt the user for an float
//
static float xScalerValue = 0.57;

void commandGetAFloat(void)
{
  char sBuffer[30];
  
  //
  // draw the title bar and clear the screen
  //
  ui.drawTitleBar("Prompt User for an float");
  ui.clearDisplaySpace();
  
  //
  // set the size and initial value of the number box
  //
  const int numberBoxWidth = 200;
  const int numberBoxAndButtonsHeight = 35;

  //
  // define a Number Box for Floats so the user can select a numeric value, specify 
  // the initial value, max and min values, and step up/down amount
  //
  NUMBER_BOX_FLOAT my_NumberBox;
  my_NumberBox.labelText           = "Set X scaler";
  my_NumberBox.value                = xScalerValue;
  my_NumberBox.minimumValue         = 0.0;
  my_NumberBox.maximumValue         = 1.0;
  my_NumberBox.stepAmount           = 0.01;
  my_NumberBox.digitsRightOfDecimal = 2;
  my_NumberBox.centerX              = ui.displaySpaceCenterX;
  my_NumberBox.centerY              = ui.displaySpaceCenterY - 20;;
  my_NumberBox.width                = numberBoxWidth;
  my_NumberBox.height               = numberBoxAndButtonsHeight;
  ui.drawNumberBox(my_NumberBox);


  //
  // define and display "OK" and "Cancel" buttons
  //
  BUTTON okButton        = {"OK",      ui.displaySpaceCenterX-70, ui.displaySpaceBottomY-35,  120 , numberBoxAndButtonsHeight};
  ui.drawButton(okButton);

  BUTTON cancelButton    = {"Cancel",  ui.displaySpaceCenterX+70, ui.displaySpaceBottomY-35,  120 , numberBoxAndButtonsHeight};
  ui.drawButton(cancelButton);


  //
  // process touch events
  //
  while(true)
  {
    ui.getTouchEvents();

    //
    // process touch events on the Number Box
    //
    ui.checkForNumberBoxTouched(my_NumberBox);
 
    //
    // check for touch events on the "OK" button
    //
    if (ui.checkForButtonClicked(okButton))
    {
      //
      // user OK pressed, get the value from the Number Box and display it
      //
      xScalerValue = my_NumberBox.value;
      sprintf(sBuffer, "X Offset = %4.2f", xScalerValue);

      ui.clearDisplaySpace();      
      ui.lcdSetCursorXY(ui.displaySpaceCenterX, ui.displaySpaceCenterY-10);
      ui.lcdPrintCentered(sBuffer);
      delay(1500);
      return;
    }

    //
    // check for touch events on the "Cancel" button
    //
    if (ui.checkForButtonClicked(cancelButton))
      return;
  }
}



//
// menu command that demonstrates how to prompt the user to select one of n things
//
static int oscillatorOneSelection = 1;
static int oscillatorTwoSelection = 1;

void oscillatorOneChoices(void)
{  
  ui.drawTitleBarWithBackButton("Oscillator One");
  ui.clearDisplaySpace();

  //
  // define and display 3 selection boxes, one with 2 choice, one with 3 choice 
  // and one with 4
  //

  SELECTION_BOX oscillatorOneBox;
  oscillatorOneBox.labelText = "Oscillator 1";
  oscillatorOneBox.value = oscillatorOneSelection;
  oscillatorOneBox.choice0Text = "Sine";
  oscillatorOneBox.choice1Text = "Triangle";
  oscillatorOneBox.choice2Text = "Saw";
  oscillatorOneBox.choice3Text = "Pulse";
  oscillatorOneBox.centerX = ui.displaySpaceCenterX;
  oscillatorOneBox.centerY = 79;
  oscillatorOneBox.width = 250;
  oscillatorOneBox.height = 33;
  ui.drawSelectionBox(oscillatorOneBox);

  //
  // process touch events
  //
  while(true)
  {
    ui.getTouchEvents();

    //
    // process touch events in the selection boxes
    //
    if (ui.checkForSelectionBoxTouched(oscillatorOneBox)) {
      setControlChange(CCosc1,oscillatorOneBox.value);
    }

    //
    // when the "Back" button is pressed, save all the values selected
    // by the user
    //
    if (ui.checkForBackButtonClicked())
    {
      oscillatorOneSelection = oscillatorOneBox.value;
      return;
    }
  }
}

void oscillatorTwoChoices(void)
{  
  ui.drawTitleBarWithBackButton("Oscillator Two");
  ui.clearDisplaySpace();

  //
  // define and display 3 selection boxes, one with 2 choice, one with 3 choice 
  // and one with 4
  //

  SELECTION_BOX oscillatorTwoBox;
  oscillatorTwoBox.labelText = "Oscillator 2";
  oscillatorTwoBox.value = oscillatorTwoSelection;
  oscillatorTwoBox.choice0Text = "Sine";
  oscillatorTwoBox.choice1Text = "Triangle";
  oscillatorTwoBox.choice2Text = "Saw";
  oscillatorTwoBox.choice3Text = "Pulse";
  oscillatorTwoBox.centerX = ui.displaySpaceCenterX;
  oscillatorTwoBox.centerY = 79;
  oscillatorTwoBox.width = 250;
  oscillatorTwoBox.height = 33;
  ui.drawSelectionBox(oscillatorTwoBox);
  
  //
  // process touch events
  //
  while(true)
  {
    ui.getTouchEvents();

    //
    // process touch events in the selection boxes
    //
    if (ui.checkForSelectionBoxTouched(oscillatorTwoBox)) {
      setControlChange(CCosc2,oscillatorTwoBox.value);
    }

    //
    // when the "Back" button is pressed, save all the values selected
    // by the user
    //
    if (ui.checkForBackButtonClicked())
    {
      oscillatorTwoSelection = oscillatorTwoBox.value;
      return;
    }
  }
}

//
// toggle used to enable / disable "Self Destruct Mode"
//
// Sometimes it is easier to use a single button to configure something, rather than
// having a separate screen with setting choices.  This can be done using a "Toggle"
// button.  
//
static byte enableSelfDestruct = false;

void enableSelfDestructCallback(void)
{
  //
  // check if menu is requesting that the state be changed (can have more than 2 states)
  //
  if (ui.toggleSelectNextStateFlg)
  {
    //
    // select the next state
    //
    enableSelfDestruct = !enableSelfDestruct;
  }
  
  //
  // here is where you update stuff depending on the choice made by the user
  // This assumes that you have wired a LED to pin 31
  //
  pinMode(31, OUTPUT);
  
  if (enableSelfDestruct)
    digitalWrite(31, HIGH);
  else
    digitalWrite(31, LOW);
 
  //
  // send back an indication of the current state, this text is written on the button
  //
  if(enableSelfDestruct)
    ui.toggleText = "On";
  else
    ui.toggleText = "Off";
}


//
// storage locations in EEPROM for configuration values settable below
//
const int EEPROM_AMP_ATTACK = 0;                        // int requires 5 bytes of EEPROM storage
const int EEPROM_AMP_DECAY = EEPROM_AMP_ATTACK + 5;      // int requires 5 bytes of EEPROM storage
const int EEPROM_AMP_SUSTAIN = EEPROM_AMP_DECAY + 5;      // int requires 5 bytes of EEPROM storage
const int EEPROM_AMP_RELEASE = EEPROM_AMP_SUSTAIN + 5;      // int requires 5 bytes of EEPROM storage
const int EEPROM_MIXER_OSC1 = EEPROM_AMP_RELEASE + 5;      // int requires 5 bytes of EEPROM storage
const int EEPROM_MIXER_OSC2 = EEPROM_MIXER_OSC1 + 5;      // int requires 5 bytes of EEPROM storage
const int EEPROM_MIXER_NOISE = EEPROM_MIXER_OSC2 + 5;      // int requires 5 bytes of EEPROM storage
const int EEPROM_MIXER_SUB = EEPROM_MIXER_NOISE + 5;      // int requires 5 bytes of EEPROM storage

//
// defaults for configuration values, these values are used if they have never been set before
//
const int   DEFAULT_AMP_ATTACK   = 1; 
const int   DEFAULT_AMP_DECAY   = 0;
const int   DEFAULT_AMP_SUSTAIN   = 127;
const int   DEFAULT_AMP_RELEASE   = 1;
const int   DEFAULT_MIXER_OSC1   = 127;
const int   DEFAULT_MIXER_OSC2   = 127;
const int   DEFAULT_MIXER_NOISE   = 0;
const int   DEFAULT_MIXER_SUB   = 127;


//
// menu command that demonstrates how to prompt the user for info then save it
//
void mixerChoices(void)
{
  const int numberBoxWidth = 145;
  const int numberBoxHeight = 34;


  //
  // read initial values from the EEPROM, if the EEPROM has never been set, use default values
  //
  int initialMixerOsc1 = ui.readConfigurationInt(EEPROM_MIXER_OSC1, DEFAULT_MIXER_OSC1);;
  int initialMixerOsc2 = ui.readConfigurationInt(EEPROM_MIXER_OSC2, DEFAULT_MIXER_OSC2);
  int initialMixerNoise = ui.readConfigurationInt(EEPROM_MIXER_NOISE, DEFAULT_MIXER_NOISE);
  int initialMixerSub = ui.readConfigurationInt(EEPROM_MIXER_SUB, DEFAULT_MIXER_SUB);


  //
  // draw title bar without a "Back" button
  //
  ui.drawTitleBar("Mixer");
  ui.clearDisplaySpace();
  

  //
  // define and display number boxes for setting calibration constants
  //
  NUMBER_BOX mixerOsc1_NumberBox;
  mixerOsc1_NumberBox.labelText    = "Oscillator 1";
  mixerOsc1_NumberBox.value        = initialMixerOsc1;
  mixerOsc1_NumberBox.minimumValue = 0;
  mixerOsc1_NumberBox.maximumValue = 127;
  mixerOsc1_NumberBox.stepAmount   = 10;
  mixerOsc1_NumberBox.centerX      = ui.displaySpaceCenterX - 80;
  mixerOsc1_NumberBox.centerY      = 83;
  mixerOsc1_NumberBox.width        = numberBoxWidth;
  mixerOsc1_NumberBox.height       = numberBoxHeight;
  ui.drawNumberBox(mixerOsc1_NumberBox);

  NUMBER_BOX mixerOsc2_NumberBox;
  mixerOsc2_NumberBox.labelText            = "Oscillator 2";
  mixerOsc2_NumberBox.value                = initialMixerOsc2;
  mixerOsc2_NumberBox.minimumValue         = 0;
  mixerOsc2_NumberBox.maximumValue         = 127;
  mixerOsc2_NumberBox.stepAmount           = 10;
  mixerOsc2_NumberBox.centerX              = ui.displaySpaceCenterX + 80;
  mixerOsc2_NumberBox.centerY              = 83;
  mixerOsc2_NumberBox.width                = numberBoxWidth;
  mixerOsc2_NumberBox.height               = numberBoxHeight;
  ui.drawNumberBox(mixerOsc2_NumberBox);


  NUMBER_BOX mixerNoise_NumberBox;
  mixerNoise_NumberBox.labelText    = "Noise";
  mixerNoise_NumberBox.value        = initialMixerNoise;
  mixerNoise_NumberBox.minimumValue = 0;
  mixerNoise_NumberBox.maximumValue = 127;
  mixerNoise_NumberBox.stepAmount   = 10;
  mixerNoise_NumberBox.centerX      = ui.displaySpaceCenterX + 80;
  mixerNoise_NumberBox.centerY      = 157;
  mixerNoise_NumberBox.width        = numberBoxWidth;
  mixerNoise_NumberBox.height       = numberBoxHeight;
  ui.drawNumberBox(mixerNoise_NumberBox);
  
  NUMBER_BOX mixerSub_NumberBox;
  mixerSub_NumberBox.labelText    = "Sub";
  mixerSub_NumberBox.value        = initialMixerSub;
  mixerSub_NumberBox.minimumValue = 0;
  mixerSub_NumberBox.maximumValue = 127;
  mixerSub_NumberBox.stepAmount   = 10;
  mixerSub_NumberBox.centerX      = ui.displaySpaceCenterX - 80;
  mixerSub_NumberBox.centerY      = 157;
  mixerSub_NumberBox.width        = numberBoxWidth;
  mixerSub_NumberBox.height       = numberBoxHeight;
  ui.drawNumberBox(mixerSub_NumberBox);

  //
  // define and display "OK" and "Cancel" buttons
  //
  BUTTON okButton        = {"OK",     ui.displaySpaceCenterX,   ui.displaySpaceBottomY-27,  100 , numberBoxHeight};
  ui.drawButton(okButton);


  //
  // process touch events
  //
  while(true)
  {
    ui.getTouchEvents();

    //
    // process touch events on the Number Boxes
    //
    if (ui.checkForNumberBoxTouched(mixerOsc1_NumberBox)){
      setControlChange(CCmixer1, mixerOsc1_NumberBox.value);
    }
    if (ui.checkForNumberBoxTouched(mixerOsc2_NumberBox)){
      setControlChange(CCmixer2, mixerOsc2_NumberBox.value);
    }
    if (ui.checkForNumberBoxTouched(mixerNoise_NumberBox)){
      setControlChange(CCmixer3, mixerNoise_NumberBox.value);
    }
    if (ui.checkForNumberBoxTouched(mixerSub_NumberBox)){
      setControlChange(CCmixer4, mixerSub_NumberBox.value);
    }

    //
    // check for touch events on the "OK" button
    //
    if (ui.checkForButtonClicked(okButton))
    {
      //
      // save the values set by the user to EEPROM, then return to the menu
      //
      ui.writeConfigurationInt(EEPROM_MIXER_OSC1, mixerOsc1_NumberBox.value);
      ui.writeConfigurationInt(EEPROM_MIXER_OSC2, mixerOsc2_NumberBox.value);
      ui.writeConfigurationInt(EEPROM_MIXER_NOISE, mixerNoise_NumberBox.value);
      ui.writeConfigurationInt(EEPROM_MIXER_SUB, mixerSub_NumberBox.value);
      return;
    }
  }
}

//
// menu command that demonstrates how to prompt the user for info then save it
//
void ampADSRChoices(void)
{
  const int numberBoxWidth = 145;
  const int numberBoxHeight = 34;


  //
  // read initial values from the EEPROM, if the EEPROM has never been set, use default values
  //
  int initialAmpAttack = ui.readConfigurationInt(EEPROM_AMP_ATTACK, DEFAULT_AMP_ATTACK);
  int initialAmpDecay = ui.readConfigurationInt(EEPROM_AMP_DECAY, DEFAULT_AMP_DECAY);
  int initialAmpSustain = ui.readConfigurationInt(EEPROM_AMP_SUSTAIN, DEFAULT_AMP_SUSTAIN);
  int initialAmpRelease = ui.readConfigurationInt(EEPROM_AMP_RELEASE, DEFAULT_AMP_RELEASE);


  //
  // draw title bar without a "Back" button
  //
  ui.drawTitleBar("Amp ADSR");
  ui.clearDisplaySpace();
  

  //
  // define and display number boxes for setting calibration constants
  //
  NUMBER_BOX ampAttack_NumberBox;
  ampAttack_NumberBox.labelText    = "Attack";
  ampAttack_NumberBox.value        = initialAmpAttack;
  ampAttack_NumberBox.minimumValue = 0;
  ampAttack_NumberBox.maximumValue = 127;
  ampAttack_NumberBox.stepAmount   = 1;
  ampAttack_NumberBox.centerX      = ui.displaySpaceCenterX - 80;
  ampAttack_NumberBox.centerY      = 83;
  ampAttack_NumberBox.width        = numberBoxWidth;
  ampAttack_NumberBox.height       = numberBoxHeight;
  ui.drawNumberBox(ampAttack_NumberBox);

  NUMBER_BOX ampDecay_NumberBox;
  ampDecay_NumberBox.labelText            = "Decay";
  ampDecay_NumberBox.value                = initialAmpDecay;
  ampDecay_NumberBox.minimumValue         = 0;
  ampDecay_NumberBox.maximumValue         = 127;
  ampDecay_NumberBox.stepAmount           = 1;
  ampDecay_NumberBox.centerX              = ui.displaySpaceCenterX - 80;
  ampDecay_NumberBox.centerY              = 157;
  ampDecay_NumberBox.width                = numberBoxWidth;
  ampDecay_NumberBox.height               = numberBoxHeight;
  ui.drawNumberBox(ampDecay_NumberBox);


  NUMBER_BOX ampSustain_NumberBox;
  ampSustain_NumberBox.labelText    = "Sustain";
  ampSustain_NumberBox.value        = initialAmpSustain;
  ampSustain_NumberBox.minimumValue = 0;
  ampSustain_NumberBox.maximumValue = 127;
  ampSustain_NumberBox.stepAmount   = 1;
  ampSustain_NumberBox.centerX      = ui.displaySpaceCenterX + 80;
  ampSustain_NumberBox.centerY      = 83;
  ampSustain_NumberBox.width        = numberBoxWidth;
  ampSustain_NumberBox.height       = numberBoxHeight;
  ui.drawNumberBox(ampSustain_NumberBox);

  NUMBER_BOX ampRelease_NumberBox;
  ampRelease_NumberBox.labelText            = "Release";
  ampRelease_NumberBox.value                = initialAmpRelease;
  ampRelease_NumberBox.minimumValue         = 0;
  ampRelease_NumberBox.maximumValue         = 127;
  ampRelease_NumberBox.stepAmount           = 1;
  ampRelease_NumberBox.centerX              = ui.displaySpaceCenterX + 80;
  ampRelease_NumberBox.centerY              = 157;
  ampRelease_NumberBox.width                = numberBoxWidth;
  ampRelease_NumberBox.height               = numberBoxHeight;
  ui.drawNumberBox(ampRelease_NumberBox);


  //
  // define and display "OK" and "Cancel" buttons
  //
  BUTTON okButton        = {"OK",     ui.displaySpaceCenterX,   ui.displaySpaceBottomY-27,  100 , numberBoxHeight};
  ui.drawButton(okButton);


  //
  // process touch events
  //
  while(true)
  {
    ui.getTouchEvents();

    //
    // process touch events on the Number Boxes
    //
    if (ui.checkForNumberBoxTouched(ampAttack_NumberBox)){
      setControlChange(CCattack, ampAttack_NumberBox.value);
    }
    if (ui.checkForNumberBoxTouched(ampDecay_NumberBox)){
      setControlChange(CCdecay, ampDecay_NumberBox.value);
    }
    if (ui.checkForNumberBoxTouched(ampSustain_NumberBox)){
      setControlChange(CCsustain, ampSustain_NumberBox.value);
    }
    if (ui.checkForNumberBoxTouched(ampRelease_NumberBox)){
      setControlChange(CCrelease, ampRelease_NumberBox.value);
    }

    //
    // check for touch events on the "OK" button
    //
    if (ui.checkForButtonClicked(okButton))
    {
      //
      // save the values set by the user to EEPROM, then return to the menu
      //
      ui.writeConfigurationInt(EEPROM_AMP_ATTACK, ampAttack_NumberBox.value);
      ui.writeConfigurationInt(EEPROM_AMP_DECAY, ampDecay_NumberBox.value);
      ui.writeConfigurationInt(EEPROM_AMP_SUSTAIN, ampSustain_NumberBox.value);
      ui.writeConfigurationInt(EEPROM_AMP_RELEASE, ampRelease_NumberBox.value);
      return;
    }
  }
}
