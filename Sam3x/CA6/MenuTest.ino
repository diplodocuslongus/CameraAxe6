
void MenuTestInit(uint8 modId)
{
  const uint8 sDataMenu[] PROGMEM = {
  20,0,PID_MENU_HEADER,1,0,2,0,'U','I',' ','T','e','s','t',' ','M','e','n','u',0,  // MENU_HEADER 1 2 "UI Test Menu"
  15,0,PID_TEXT_STATIC,'S','t','a','t','i','c',' ','T','e','x','t',0,  // TEXT_STATIC "Static Text"
  18,0,PID_TEXT_DYNAMIC,0,0,'D','y','n','a','m','i','c',' ','T','e','x','t',0,  // TEXT_DYNAMIC 0 "Dynamic Text"  **gClientHostId_0**
  30,0,PID_BUTTON,1,0,17,'T','h','i','s',' ','i','s',' ','a',' ','b','u','t','t','o','n',0,'B','u','t','t','o','n',0,  // BUTTON 0 1 1 "This is a button" "Button"  **gClientHostId_1**
  26,0,PID_CHECK_BOX,2,1,0,'T','h','i','s',' ','i','s',' ','a',' ','c','h','e','c','k',' ','b','o','x',0,  // CHECK_BOX 1 0 "This is a check box"  **gClientHostId_2**
  35,0,PID_DROP_SELECT,3,2,1,'T','h','i','s',' ','i','s',' ','a',' ','d','r','o','p',' ','s','e','l','e','c','t',0,'n','o','|','y','e','s',0,  // DROP_SELECT 2 1 "This is a drop select" "no|yes"  **gClientHostId_3**
  41,0,PID_EDIT_NUMBER,4,0,50,0,0,0,0,159,134,1,0,80,195,0,0,'T','h','i','s',' ','i','s',' ','a','n',' ','e','d','i','t',' ','n','u','m','b','e','r',0,  // EDIT_NUMBER 0 2 3 0 99999 50000 "This is an edit number"  **gClientHostId_4**
  32,0,PID_TIME_BOX,5,0,223,24,59,122,62,125,144,1,'T','h','i','s',' ','i','s',' ','a',' ','t','i','m','e',' ','b','o','x',0,  // TIME_BOX 0 1 1 1 1 1 0 99 59 40 999 500 400 "This is a time box"  **gClientHostId_5**
  3,0,PID_SCRIPT_END,  // SCRIPT_END
  };  // Total Bytes = 220

  const uint8 sDataActive[] PROGMEM = {
  20,0,PID_MENU_HEADER,1,0,2,0,'U','I',' ','T','e','s','t',' ','M','e','n','u',0,  // MENU_HEADER 1 2 "UI Test Menu"
  3,0,PID_SCRIPT_END,  // SCRIPT_END
  };  // Total Bytes = 23
  
  g_ctx.modules[modId].modStore.menuSoundData.nextSendUpdate = 0;
  CAU::pinMode(g_ctx.modules[modId].modStore.menuSoundData.ppSound, ANALOG_INPUT);
  g_ctx.modules[modId].modStore.menuSoundData.ppSound = CAU::getModulePin(modId, 0);
  uint8 *data = 0;
  g_ctx.packetHelper.writeMenu(sDataMenu, 220);
}

// Sends packets to host
void MenuTestSendPackets(uint8 modId) {
}

// Receives packets from host
void MenuTestReceivePackets(uint8 modId, uint8 *packet) {
}

void MenuTestActiveInit(uint8 modId) {
}

// Handle the actual triggering
uint8 MenuTestTriggerCheck(uint8 modId) {
  return 0;
}

