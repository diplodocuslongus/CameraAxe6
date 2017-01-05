void caRunTests()
{
  //caTestTickTimer();
  caTestPackets();
  //caTestBlinkLed();
  //caTestPerf();
  //caTestModulePorts();
  //caTestLinkAndCamPorts();
  //caTestAuxPort();
  //caTestEeprom();
  //caTestAnalog();
  //caTestRFD();
  //caTestEsp8266();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// caTestRFD - Test communications with BLE module
// returns  - NA
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*void caTestRFD()
{
  // This test sends a packet from SAM to RFduino.  Then RFduino returns it with the current code there.
  uint8 bufSize;
  uint8 header;
  uint8 data[64];
  uint8 data2[64];
  CAPacket packetProcessor;
  PacketTextDynamic packTextDynamic;
  uint8 packetType;

  packTextDynamic.client_host_id = 1;
  packTextDynamic.text_string = "SAM456";
  packetProcessor.packTextDynamic(&packTextDynamic, data);
  g_ctx.bleSerial.write(data);
  delay(4000);

  if (g_ctx.bleSerial.read(&bufSize, &header, data))
  {
    packetProcessor.unpackTextDynamic((uint8*)data, &packTextDynamic, (char*)data2);
    if (strcmp((char*)packTextDynamic.text_string, "SAM456") != 0)
    {
      CAU::log("  Failed strcmp - %s\n", packTextDynamic.text_string);
    }
    else
    {
      CAU::log("Done - RFD\n");
    }
  }
}*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// caTestEsp8266 - Test communications with ESP8266 module
// returns  - NA
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void caTestEsp8266()
{
  char buf[64];
  static bool first = true;

  if (first) {
    first = false;
    Serial1.begin(9600);
  }

  if (Serial1.available()) {
    SerialUSB.write(Serial1.read());
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// caTestTickTimer - Tests the CATickTimer class
// returns  - NA
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void caTestTickTimer()
{
  uint64 ticks;
  CATickTimer bob(0);
  ticks = bob.convertTimeToTicks(0,0,0,100,0,0);
  bob.start(toggleLed, ticks, CA_TRUE);
  delay(5000);
  bob.stop();
}
void toggleLed()
{
  static uint8 toggle = 1;

  hwPortPin led = CAU::getOnboardDevicePin(LED_GREEN_PIN);
  CAU::pinMode(led, OUTPUT);

  if (toggle)
  {
    CAU::digitalWrite(led, HIGH);
    toggle = 0;
  }
  else
  {
    CAU::digitalWrite(led, LOW);
    toggle = 1;
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// caTestPackets - Tests packing and unpacking of data packets
// returns  - NA
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void caTestPackets()
{
  const uint8 sData[] PROGMEM = {
  17,PID_MENU_HEADER,1,2,'U','I',' ','T','e','s','t',' ','M','e','n','u',0,  // MENU_HEADER 1 2 "UI Test Menu"
  2,PID_NEW_ROW,  // NEW_ROW
  4,PID_NEW_CELL,40,0,  // NEW_CELL 40 0
  4,PID_NEW_CELL,10,2,  // NEW_CELL 10 2
  4,PID_NEW_CELL,50,1,  // NEW_CELL 50 1
  14,PID_TEXT_STATIC,'S','t','a','t','i','c',' ','T','e','x','t',0,  // TEXT_STATIC "Static Text"
  16,PID_TEXT_DYNAMIC,0,'D','y','n','a','m','i','c',' ','T','e','x','t',0,  // TEXT_DYNAMIC "Dynamic Text"  **gClientHostId_0**
  11,PID_BUTTON,1,17,'B','u','t','t','o','n',0,  // BUTTON 1 1 "Button"  **gClientHostId_1**
  4,PID_CHECK_BOX,2,0,  // CHECK_BOX 0  **gClientHostId_2**
  11,PID_DROP_SELECT,3,1,'n','o','|','y','e','s',0,  // DROP_SELECT 1 "no|yes"  **gClientHostId_3**
  16,PID_EDIT_NUMBER,4,50,0,0,0,0,159,134,1,0,80,195,0,0,  // EDIT_NUMBER 2 3 0 99999 50000  **gClientHostId_4**
  4,PID_COND_START,5,0,  // COND_START 0 0  **gClientHostId_5**
  11,PID_TIME_BOX,6,223,24,59,122,62,125,144,1,  // TIME_BOX 1 1 1 1 1 0 99 59 40 999 500 400  **gClientHostId_6**
  2,PID_COND_END,  // COND_END
  2,PID_SCRIPT_END,  // SCRIPT_END
  };  // Total Bytes = 122

  const uint8 BUF_SIZE = 122;
  uint8 data[BUF_SIZE];
  const uint8 *dataPtr = data;
  uint8 totalUnpackSize=0;
  // Move progmem data to a buffer for this test
  for(uint16 i=0; i<BUF_SIZE; ++i)
  {
   data[i] = pgm_read_byte_near(sData+i);
  }

  uint8 dataA[256];
  uint8 packType, packSize, unpackSize;
  memset(dataA, 0, 256);
  CAPacket unpack0(STATE_UNPACKER, data, BUF_SIZE);
  CAPacket pack0(STATE_PACKER, dataA, 256);

  {  // MENU_HEADER 1 2 "UI Test Menu"
    CAPacketMenuHeader unpack1(unpack0);          // Update per type
    CAPacketMenuHeader pack1(pack0);              // Update per type
  
    pack1.set(1, 2, "UI Test Menu");              // Update per type
    unpackSize = unpack0.unpackSize();
    packType = unpack0.unpackType();
    unpack1.unpack();
    packSize = pack1.pack();
    totalUnpackSize += unpackSize;
    if (memcmp(data, dataA, totalUnpackSize) != 0 ||
        packSize != unpackSize ||
        packType != PID_MENU_HEADER ||            // Update per type
        unpack1.getMajorVersion() != 1 ||
        unpack1.getMinorVersion() != 2 ||
        strcmp(unpack1.getMenuName().c_str(), "UI Test Menu") != 0) {
      CAU::log("ERROR - MENU_HEADER test failed\n");
    }
  }

  {  // NEW_ROW
    CAPacketNewRow unpack1(unpack0);              // Update per type
    CAPacketNewRow pack1(pack0);                  // Update per type
  
    //pack1.set();                                // Update per type
    unpackSize = unpack0.unpackSize();
    packType = unpack0.unpackType();
    //unpack1.unpack();
    packSize = pack1.pack();
    totalUnpackSize += unpackSize;
    if (memcmp(data, dataA, totalUnpackSize) != 0 ||
        packSize != unpackSize ||
        packType != PID_NEW_ROW) {                // Update per type
      CAU::log("ERROR - NEW_ROW test failed\n");
    }
  }

  {  // NEW_CELL 40 0
    CAPacketNewCell unpack1(unpack0);             // Update per type
    CAPacketNewCell pack1(pack0);                 // Update per type
  
    pack1.set(40, 0);                             // Update per type
    unpackSize = unpack0.unpackSize();
    packType = unpack0.unpackType();
    unpack1.unpack();
    packSize = pack1.pack();
    totalUnpackSize += unpackSize;
    if (memcmp(data, dataA, totalUnpackSize) != 0 ||
        packSize != unpackSize ||
        packType != PID_NEW_CELL ||               // Update per type
        unpack1.getColumnPercentage() != 40 ||
        unpack1.getJustification() != 0) {
      CAU::log("ERROR - NEW_CELL 0 test failed\n");
    }
  }

  {  // NEW_CELL 10 2
    CAPacketNewCell unpack1(unpack0);             // Update per type
    CAPacketNewCell pack1(pack0);                 // Update per type
  
    pack1.set(10, 2);                             // Update per type
    unpackSize = unpack0.unpackSize();
    packType = unpack0.unpackType();
    unpack1.unpack();
    packSize = pack1.pack();
    totalUnpackSize += unpackSize;
    if (memcmp(data, dataA, totalUnpackSize) != 0 ||
        packSize != unpackSize ||
        packType != PID_NEW_CELL ||               // Update per type
        unpack1.getColumnPercentage() != 10 ||
        unpack1.getJustification() != 2) {
      CAU::log("ERROR - NEW_CELL 2 test failed\n");
    }
  }

  {  // NEW_CELL 50 1
    CAPacketNewCell unpack1(unpack0);             // Update per type
    CAPacketNewCell pack1(pack0);                 // Update per type
  
    pack1.set(50, 1);                             // Update per type
    unpackSize = unpack0.unpackSize();
    packType = unpack0.unpackType();
    unpack1.unpack();
    packSize = pack1.pack();
    totalUnpackSize += unpackSize;
    if (memcmp(data, dataA, totalUnpackSize) != 0 ||
        packSize != unpackSize ||
        packType != PID_NEW_CELL ||               // Update per type
        unpack1.getColumnPercentage() != 50 ||
        unpack1.getJustification() != 1) {
      CAU::log("ERROR - NEW_CELL 1 test failed\n");
    }
  }

  {  // TEXT_STATIC "Static Text"
    CAPacketTextStatic unpack1(unpack0);          // Update per type
    CAPacketTextStatic pack1(pack0);              // Update per type
  
    pack1.set("Static Text");                     // Update per type
    unpackSize = unpack0.unpackSize();
    packType = unpack0.unpackType();
    unpack1.unpack();
    packSize = pack1.pack();
    totalUnpackSize += unpackSize;
    if (memcmp(data, dataA, totalUnpackSize) != 0 ||
        packSize != unpackSize ||
        packType != PID_TEXT_STATIC ||            // Update per type
        strcmp(unpack1.getText().c_str(), "Static Text") != 0) {
      CAU::log("ERROR - TEXT_STATIC test failed\n");
    }
  }

  {  // TEXT_DYNAMIC "Dynamic Text"  **gClientHostId_0**
    CAPacketTextDynamic unpack1(unpack0);         // Update per type
    CAPacketTextDynamic pack1(pack0);             // Update per type
  
    pack1.set(0, "Dynamic Text");                 // Update per type
    unpackSize = unpack0.unpackSize();
    packType = unpack0.unpackType();
    unpack1.unpack();
    packSize = pack1.pack();
    totalUnpackSize += unpackSize;
    if (memcmp(data, dataA, totalUnpackSize) != 0 ||
        packSize != unpackSize ||
        packType != PID_TEXT_DYNAMIC ||           // Update per type
        unpack1.getClientHostId() != 0 ||
        strcmp(unpack1.getText().c_str(), "Dynamic Text") != 0) {
      CAU::log("ERROR - TEXT_DYNAMIC test failed\n");
    }
  }

  { // BUTTON 1 1 "Button"  **gClientHostId_1**
    CAPacketButton unpack1(unpack0);              // Update per type
    CAPacketButton pack1(pack0);                  // Update per type
  
    pack1.set(1, 1, 1, "Button");                 // Update per type
    unpackSize = unpack0.unpackSize();
    packType = unpack0.unpackType();
    unpack1.unpack();
    packSize = pack1.pack();
    totalUnpackSize += unpackSize;
    if (memcmp(data, dataA, totalUnpackSize) != 0 ||
        packSize != unpackSize ||
        packType != PID_BUTTON ||                 // Update per type
        unpack1.getClientHostId() != 1 ||
        unpack1.getType() != 1 ||
        unpack1.getValue() != 1 ||
        strcmp(unpack1.getText().c_str(), "Button") != 0) {
      CAU::log("ERROR - BUTTON test failed\n");
    }
  }

  {  // CHECK_BOX 0  **gClientHostId_2**
    CAPacketCheckBox unpack1(unpack0);            // Update per type
    CAPacketCheckBox pack1(pack0);                // Update per type
  
    pack1.set(2, 0);                              // Update per type
    unpackSize = unpack0.unpackSize();
    packType = unpack0.unpackType();
    unpack1.unpack();
    packSize = pack1.pack();
    totalUnpackSize += unpackSize;
    if (memcmp(data, dataA, totalUnpackSize) != 0 ||
        packSize != unpackSize ||
        packType != PID_CHECK_BOX ||              // Update per type
        unpack1.getClientHostId() != 2 ||
        unpack1.getValue() != 0 ) {
      CAU::log("ERROR - CHECK_BOX test failed\n");
    }
  }

  {  // DROP_SELECT 1 "no|yes"  **gClientHostId_3**
    CAPacketDropSelect unpack1(unpack0);          // Update per type
    CAPacketDropSelect pack1(pack0);              // Update per type
  
    pack1.set(3, 1, "no|yes");                    // Update per type
    unpackSize = unpack0.unpackSize();
    packType = unpack0.unpackType();
    unpack1.unpack();
    packSize = pack1.pack();
    totalUnpackSize += unpackSize;
    if (memcmp(data, dataA, totalUnpackSize) != 0 ||
        packSize != unpackSize ||
        packType != PID_DROP_SELECT ||            // Update per type
        unpack1.getClientHostId() != 3 ||
        unpack1.getValue() != 1 ||
        strcmp(unpack1.getText().c_str(), "no|yes") != 0) {
      CAU::log("ERROR - DROP_SELECT test failed\n");
    }
  }

  {  // EDIT_NUMBER 2 3 0 99999 50000  **gClientHostId_4**
    CAPacketEditNumber unpack1(unpack0);          // Update per type
    CAPacketEditNumber pack1(pack0);              // Update per type
  
    pack1.set(4, 2, 3, 0, 99999, 50000);          // Update per type
    unpackSize = unpack0.unpackSize();
    packType = unpack0.unpackType();
    unpack1.unpack();
    packSize = pack1.pack();
    totalUnpackSize += unpackSize;
    if (memcmp(data, dataA, totalUnpackSize) != 0 ||
        packSize != unpackSize ||
        packType != PID_EDIT_NUMBER ||            // Update per type
        unpack1.getClientHostId() != 4 ||
        unpack1.getDigitsBeforeDecimal() != 2 ||
        unpack1.getDigitsAfterDecimal() != 3 ||
        unpack1.getMinValue() != 0 ||
        unpack1.getMaxValue() != 99999 ||
        unpack1.getValue() != 50000 ) {
      CAU::log("ERROR - EDIT_NUMBER test failed\n");
    }
  }

  {  // COND_START 0 0  **gClientHostId_5**
    CAPacketCondStart unpack1(unpack0);           // Update per type
    CAPacketCondStart pack1(pack0);               // Update per type
  
    pack1.set(5, 0, 0);                           // Update per type
    unpackSize = unpack0.unpackSize();
    packType = unpack0.unpackType();
    unpack1.unpack();
    packSize = pack1.pack();
    totalUnpackSize += unpackSize;
    if (memcmp(data, dataA, totalUnpackSize) != 0 ||
        packSize != unpackSize ||
        packType != PID_COND_START ||             // Update per type
        unpack1.getClientHostId() != 5 ||
        unpack1.getModAttribute() != 0 ||
        unpack1.getValue() != 0 ) {
      CAU::log("ERROR - COND_START test failed\n");
    }
  }


  {  // TIME_BOX 1 1 1 1 1 0 99 59 40 999 500 400  **gClientHostId_6**
    CAPacketTimeBox unpack1(unpack0);             // Update per type
    CAPacketTimeBox pack1(pack0);                 // Update per type
  
    pack1.set(6, 0x1f, 99, 59, 40, 999, 500, 400);// Update per type
    unpackSize = unpack0.unpackSize();
    packType = unpack0.unpackType();
    unpack1.unpack();
    packSize = pack1.pack();
    totalUnpackSize += unpackSize;
    if (memcmp(data, dataA, totalUnpackSize) != 0 ||
        packSize != unpackSize ||
        packType != PID_TIME_BOX ||               // Update per type
        unpack1.getClientHostId() != 6 ||
        unpack1.getEnableMask() != 0x1f ||
        unpack1.getHours() != 99 ||
        unpack1.getMinutes() != 59 ||
        unpack1.getSeconds() != 40 ||
        unpack1.getMilliseconds() != 999 ||
        unpack1.getMicroseconds() != 500 ||
        unpack1.getNanoseconds() != 400 ) {
      CAU::log("ERROR - TIME_BOX test failed\n");
    }
  }

  {  // COND_END
    CAPacketCondEnd unpack1(unpack0);             // Update per type
    CAPacketCondEnd pack1(pack0);                 // Update per type
  
    //pack1.set();                                // Update per type
    unpackSize = unpack0.unpackSize();
    packType = unpack0.unpackType();
    //unpack1.unpack();
    packSize = pack1.pack();
    totalUnpackSize += unpackSize;
    if (memcmp(data, dataA, totalUnpackSize) != 0 ||
        packSize != unpackSize ||
        packType != PID_COND_END) {               // Update per type
      CAU::log("ERROR - COND_END test failed\n");
    }
  }


  {  // SCRIPT_END
    CAPacketScriptEnd unpack1(unpack0);           // Update per type
    CAPacketScriptEnd pack1(pack0);               // Update per type
  
    //pack1.set();                                // Update per type
    unpackSize = unpack0.unpackSize();
    packType = unpack0.unpackType();
    //unpack1.unpack();
    packSize = pack1.pack();
    totalUnpackSize += unpackSize;
    if (memcmp(data, dataA, totalUnpackSize) != 0 ||
        packSize != unpackSize ||
        packType != PID_SCRIPT_END) {             // Update per type
      CAU::log("ERROR - SCRIPT_END test failed\n");
    }
  }

  // The following are packet types that don't exist in the script files
  memset(dataA, 0, 256);
  CAPacket unpack10(STATE_UNPACKER, dataA, 256);
  CAPacket pack10(STATE_PACKER, dataA, 256);

  { // ACTIVATE Packet Test
    CAPacketActivate unpack11(unpack10);          // Update per type
    CAPacketActivate pack11(pack10);              // Update per type
    
    pack11.set(1);                                // Update per type
    uint8 packSize = pack11.pack();
    uint8 unpackSize = unpack10.unpackSize();
    uint8 packType = unpack10.unpackType();
    unpack11.unpack();
    if (packSize != unpackSize ||
          packType != PID_ACTIVATE ||             // Update per type
          unpack11.getActivate() != 1 ) {
      CAU::log("ERROR - ACTIVATE test failed");
    } 
  }
  
  { // LOG Packet Test
    CAPacketLog unpack11(unpack10);               // Update per type
    CAPacketLog pack11(pack10);                   // Update per type
    
    pack11.set("This is a log");                  // Update per type
    uint8 packSize = pack11.pack();
    uint8 unpackSize = unpack10.unpackSize();
    uint8 packType = unpack10.unpackType();
    unpack11.unpack();
    if (packSize != unpackSize ||
          packType != PID_LOG ||                  // Update per type
          strcmp(unpack11.getLog().c_str(), "This is a log") != 0) {
      CAU::log("ERROR - LOG test failed");
    } 
  }

/*
  // Pack/Unpack for ACTIVATE
  dataPtr = data;
  memset(&packActivate, 0, sizeof(packActivate));
  memset(&packActivate2, 0, sizeof(packActivate2));
  packActivate.active = 1;
  dataPtr2 = packetProcessor.packActivate(&packActivate, (uint8*)dataPtr);
  dataPtr = packetProcessor.getPacketSize(dataPtr, &packetSize);
  dataPtr = packetProcessor.getPacketType(dataPtr, &packetType);
  dataPtr2 = packetProcessor.unpackActivate(dataPtr, &packActivate2);
  if ((packetType != PID_ACTIVATE) || (memcmp(&packActivate, &packActivate2, sizeof(packActivate)) != 0))
  {
    CAU::log("ERROR - Pack/unpack ACTIVATE %d\n", packetType);
    return;
  }

  // Pack/Unpack for LOG
  dataPtr = data;
  memset(&packLog, 0, sizeof(packLog));
  memset(&packLog2, 0, sizeof(packLog2));
  packLog.log_string = "foo";
  dataPtr2 = packetProcessor.packLog(&packLog, (uint8*)dataPtr);
  dataPtr = packetProcessor.getPacketSize(dataPtr, &packetSize);
  dataPtr = packetProcessor.getPacketType(dataPtr, &packetType);
  dataPtr2 = packetProcessor.unpackLog(dataPtr, &packLog2, strBuf);
  if ((packetType != PID_LOG) || (memcmp(&packLog, &packLog2, sizeof(packLog)-4) != 0) || strcmp("foo", strBuf)!=0 )
  {
    CAU::log("ERROR - Pack/unpack LOG %d\n", packetType);
    return;
  }

  // Pack/Unpack for CAM_STATE
  dataPtr = data;
  memset(&packCamState, 0, sizeof(packCamState));
  memset(&packCamState2, 0, sizeof(packCamState2));
  packCamState.cam_multiplier = 0;
  packCamState.cam0_focus     = 1;
  packCamState.cam0_shutter   = 0;
  packCamState.cam1_focus     = 1;
  packCamState.cam1_shutter   = 1;
  packCamState.cam2_focus     = 0;
  packCamState.cam2_shutter   = 0;
  packCamState.cam3_focus     = 0;
  packCamState.cam3_shutter   = 1;
  packCamState.cam4_focus     = 1;
  packCamState.cam4_shutter   = 1;
  packCamState.cam5_focus     = 1;
  packCamState.cam5_shutter   = 1;
  packCamState.cam6_focus     = 1;
  packCamState.cam6_shutter   = 0;
  packCamState.cam7_focus     = 1;
  packCamState.cam7_shutter   = 0;
  dataPtr2 = packetProcessor.packCamState(&packCamState, (uint8*)dataPtr);
  dataPtr = packetProcessor.getPacketSize(dataPtr, &packetSize);
  dataPtr = packetProcessor.getPacketType(dataPtr, &packetType);
  dataPtr2 = packetProcessor.unpackCamState(dataPtr, &packCamState2);
  if ((packetType != PID_CAM_STATE) || (memcmp(&packCamState, &packCamState2, sizeof(packCamState)) != 0))
  {
    CAU::log("ERROR - Pack/unpack CAM_STATE %d\n", packetType);
    return;
  }

  // Pack/Unpack for CAM_SETTINGS
  dataPtr = data;
  memset(&packCamSettings, 0, sizeof(packCamSettings));
  memset(&packCamSettings2, 0, sizeof(packCamSettings2));
  packCamSettings.cam_port_number = 1;
  packCamSettings.mode = 2;
  packCamSettings.delay_hours = 877;
  packCamSettings.delay_minutes = 30;
  packCamSettings.delay_seconds = 55;
  packCamSettings.delay_milliseconds = 998;
  packCamSettings.delay_microseconds = 122;
  packCamSettings.duration_hours = 744;
  packCamSettings.duration_minutes = 22;
  packCamSettings.duration_seconds = 32;
  packCamSettings.duration_milliseconds = 31;
  packCamSettings.duration_microseconds = 333;
  packCamSettings.sequencer0 = 1;
  packCamSettings.sequencer1 = 1;
  packCamSettings.sequencer2 = 0;
  packCamSettings.sequencer3 = 0;
  packCamSettings.sequencer4 = 0;
  packCamSettings.sequencer5 = 0;
  packCamSettings.sequencer6 = 0;
  packCamSettings.sequencer7 = 0;
  packCamSettings.apply_intervalometer = 1;
  packCamSettings.smart_preview = 0;
  packCamSettings.mirror_lockup_enable = 1;
  packCamSettings.mirror_lockup_minutes = 45;
  packCamSettings.mirror_lockup_seconds = 33;
  packCamSettings.mirror_lockup_milliseconds = 525;
  dataPtr2 = packetProcessor.packCamSettings(&packCamSettings, (uint8*)dataPtr);
  dataPtr = packetProcessor.getPacketSize(dataPtr, &packetSize);
  dataPtr = packetProcessor.getPacketType(dataPtr, &packetType);
  dataPtr2 = packetProcessor.unpackCamSettings(dataPtr, &packCamSettings2);
  if ((packetType != PID_CAM_SETTINGS) || (memcmp(&packCamSettings, &packCamSettings2, sizeof(packCamSettings)) != 0))
  {
    CAU::log("ERROR - Pack/unpack CAM_SETTINGS %d\n", packetType);
    return;
  }

  // Pack/Unpack for INTERVALOMETER
  dataPtr = data;
  memset(&packIntervalometer, 0, sizeof(packIntervalometer));
  memset(&packIntervalometer2, 0, sizeof(packIntervalometer2));
  packIntervalometer.start_hours = 22;
  packIntervalometer.start_minutes = 55;
  packIntervalometer.start_seconds = 11;
  packIntervalometer.start_milliseconds = 322;
  packIntervalometer.start_microseconds = 422;
  packIntervalometer.interval_hours = 522;
  packIntervalometer.interval_minutes = 46;
  packIntervalometer.interval_seconds = 12;
  packIntervalometer.interval_milliseconds = 56;
  packIntervalometer.interval_microseconds = 43;
  packIntervalometer.repeats = 43999;
  dataPtr2 = packetProcessor.packIntervalometer(&packIntervalometer, (uint8*)dataPtr);
  dataPtr = packetProcessor.getPacketSize(dataPtr, &packetSize);
  dataPtr = packetProcessor.getPacketType(dataPtr, &packetType);
  dataPtr2 = packetProcessor.unpackIntervalometer(dataPtr, &packIntervalometer2);
  if ((packetType != PID_INTERVALOMETER) || (memcmp(&packIntervalometer, &packIntervalometer2, sizeof(packIntervalometer)) != 0))
  {
    CAU::log("ERROR - Pack/unpack INTERVALOMETER %d\n", packetType);
    return;
  }

  // Pack/Unpack for INTER_MODULE_LOGIC
  dataPtr = data;
  memset(&packInterModuleLogic, 0, sizeof(packInterModuleLogic));
  memset(&packInterModuleLogic2, 0, sizeof(packInterModuleLogic2));
  packInterModuleLogic.enable_latch = 0;
  packInterModuleLogic.logic = 3;
  dataPtr2 = packetProcessor.packInterModuleLogic(&packInterModuleLogic, (uint8*)dataPtr);
  dataPtr = packetProcessor.getPacketSize(dataPtr, &packetSize);
  dataPtr = packetProcessor.getPacketType(dataPtr, &packetType);
  dataPtr2 = packetProcessor.unpackInterModuleLogic(dataPtr, &packInterModuleLogic2);
  if ((packetType != PID_INTER_MODULE_LOGIC) || (memcmp(&packInterModuleLogic, &packInterModuleLogic2, sizeof(packInterModuleLogic)) != 0))
  {
    CAU::log("ERROR - Pack/unpack INTER_MODULE_LOGIC %d\n", packetType);
    return;
  }
  
  // Pack/Unpack for CONTROL_FLAGS
  dataPtr = data;
  memset(&packControlFlags, 0, sizeof(packControlFlags));
  memset(&packControlFlags2, 0, sizeof(packControlFlags2));
  packControlFlags.enable_slave_mode = 0;
  packControlFlags.enable_extra_messages = 1;
  dataPtr2 = packetProcessor.packControlFlags(&packControlFlags, (uint8*)dataPtr);
  dataPtr = packetProcessor.getPacketSize(dataPtr, &packetSize);
  dataPtr = packetProcessor.getPacketType(dataPtr, &packetType);
  dataPtr2 = packetProcessor.unpackControlFlags(dataPtr, &packControlFlags2);
  if ((packetType != PID_CONTROL_FLAGS) || (memcmp(&packControlFlags, &packControlFlags2, sizeof(packControlFlags)) != 0))
  {
    CAU::log("ERROR - Pack/unpack CONTROL_FLAGS %d\n", packetType);
    return;
  }
*/
  CAU::log("Done - testPackets\n");
  delay(2000);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// caTestBlinkLed - Test blinks LEDs.
// returns  - NA
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void caTestBlinkLed()
{
  hwPortPin led = CAU::getOnboardDevicePin(LED_GREEN_PIN);
  CAU::pinMode(led, OUTPUT);
  CAU::digitalWrite(led, HIGH);
  delay(1000);
  CAU::digitalWrite(led, LOW);
  delay(200);

  led = CAU::getOnboardDevicePin(LED_RED_PIN);
  CAU::pinMode(led, OUTPUT);
  CAU::digitalWrite(led, HIGH);
  delay(200);
  CAU::digitalWrite(led, LOW);
  delay(200);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// caTestPerf - Various performance tests for low level CA6 functions
// returns  - NA
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void caTestPerf()
{
  uint16 count = 0;
  uint8 val8=0;
  uint16 val16=0;
  uint32 startTime, endTime;
  hwPortPin pp0 = CAU::getModulePin(0, 2); // actually Mod0_SDA
  
  startTime = micros();
  for(count=0; count<10000; ++count)
  {
    val8 = CAU::digitalRead(pp0);
  }
  endTime = micros();
  CAU::log("  10,000 digitalRead() = %dus\n", endTime-startTime);

  startTime = micros();
  for(count=0; count<10000; ++count)
  {
    CAU::digitalWrite(pp0, LOW);
  }
  endTime = micros();
  CAU::log("  10,000 digitalWrite() = %dus\n", endTime-startTime);

  pp0 = CAU::getOnboardDevicePin(LV_DETECT_PIN);
  CAU::initializeAnalog();
  CAU::pinMode(pp0, ANALOG_INPUT);
  startTime = micros();
  for(count=0; count<10000; ++count)
  {
    val16 = CAU::analogRead(pp0);
  }
  endTime = micros();
  CAU::log("  10,000 analogRead() = %dus\n", endTime-startTime);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// caTestModulePorts - Tests all the module ports.  Reports which ones failed. - Requires a special test dongle.
// returns  - NA
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void caTestModulePorts()
{
  uint8 module;
  uint8 pin;

  for(module=0; module<4; ++module)
  {
    for(pin=0; pin<6; pin+=2)
    {
      hwPortPin pp0, pp1;
      pp0 = CAU::getModulePin(module, pin);
      pp1 = CAU::getModulePin(module, pin+1);
      if (!caTestTwoPins(pp0, pp1))
      {
        CAU::log("  Module Port %d:%d failed\n", module, pin);
      }
      
      if (!caTestTwoPins(pp1, pp0))
      {
        CAU::log("  Module Port %d:%d failed\n", module, pin+1);
      }
    }
  }
  CAU::log("Done - module ports\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// caTestLinkAndCamPorts - Tests link and camera ports.  Reports which ones failed. - Requires a special test dongle.
// returns  - NA
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void caTestLinkAndCamPorts()
{
  hwPortPin ppFocus, ppShutter;
  uint8 cam, i;
  hwPortPin linkFocus   = CAU::getLinkPin(FOCUS);
  hwPortPin linkShutter = CAU::getLinkPin(SHUTTER);
  uint8 val0, val1, val2, val3, val4, val5;
  
  for(cam=0; cam<8; ++cam)
  {
    caAllPortsLow(); // Must reset all cam ports here
    CAU::pinMode(linkFocus,   INPUT);
    CAU::pinMode(linkShutter, INPUT);

    delayMicroseconds(90);  // Without this delay optoisolator trigger time causes failure
    val0 = CAU::digitalRead(linkFocus);    // Should be high due to pullup resistor
    val1 = CAU::digitalRead(linkShutter);  // Should be high due to pullup resistor
    ppFocus   = CAU::getCameraPin(cam, FOCUS);
    ppShutter = CAU::getCameraPin(cam, SHUTTER);
    CAU::digitalWrite(ppFocus,   HIGH);
    CAU::digitalWrite(ppShutter, LOW);
    delayMicroseconds(90);  // Without this delay optoisolator trigger time causes failure
    val2 = CAU::digitalRead(linkFocus);    // Should be low
    val3 = CAU::digitalRead(linkShutter);  // Should be high due to pullup resistor
    CAU::digitalWrite(ppFocus,   LOW);
    CAU::digitalWrite(ppShutter, HIGH);
    delayMicroseconds(90);  // Without this delay optoisolator trigger time causes failure
    val4 = CAU::digitalRead(linkFocus);    // Should be high due to pullup resistor
    val5 = CAU::digitalRead(linkShutter);  // Should be low
  
    if ((val0 != HIGH) || (val1 != HIGH) ||
        (val2 != LOW)  || (val3 != HIGH) ||
        (val4 != HIGH) || (val5 != LOW))
    {
      CAU::log("  Camera/Link Port Cam:%d failed (%d,%d,%d,%d,%d,%d)\n", cam, val0, val1, val2, val3, val4, val5);
    }
  }
  
  CAU::log("Done - camera/linkports\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// caTestLinkAndCamPorts - Tests the aux port.  Reports which pins failed. - Requires a special test dongle.
// returns  - NA
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void caTestAuxPort()
{
  uint8 pin;
  hwPortPin pp0, pp1;

  for(pin=0; pin<46; pin+=2)
  {
    pp0 = CAU::getAuxPin(pin);
    pp1 = CAU::getAuxPin(pin+1);
    if (!caTestTwoPins(pp0, pp1))
    {
      CAU::log("  Aux Port %d failed\n", pin);
    }
    if (!caTestTwoPins(pp1, pp0))
    {
      CAU::log("  Aux Port %d failed\n", pin+1);
    }
  }
  
  CAU::log("Done - aux port\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// caTestEeprom - Tests the eeprom
// returns  - NA
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void caTestEeprom()
{
  char *str0 = "This is just a test string1.";
  char *str1 = "Blah blah blah blah 2.";
  uint8 buf[128];
  unioReadStatus statusReg;

  CAEeprom eeprom(CA_INTERNAL);
  
  if (!eeprom.statusWrite(EEPROM_WRITE_PROTECTED_ALL))
  {
    CAU::log("  Writing status register FAILED\n");
  }
  if(!eeprom.statusRead(&statusReg))
  {
    CAU::log("  Reading status register FAILED\n");
  }
  if (!eeprom.statusWrite(EEPROM_WRITE_PROTECTED_NONE))
  {
    CAU::log("  Writing status register 2 FAILED\n");
  }

  if (!eeprom.write((uint8*)str0, 0x20, strlen(str0)+1))
  {
    CAU::log("  Write1 FAILED\n");
  }
  memset(buf, 0, 128);
  if (!eeprom.read(buf, 0x20, strlen(str0)+1))
  {
    CAU::log("  Read1 FAILED\n");
  }
  if (!eeprom.write((uint8*)str1, 0x20, strlen(str1)+1))
  {
    CAU::log("  Write2 FAILED\n");
  }
  memset(buf, 0, 128);

  if (!eeprom.read(buf, 0x20, strlen(str1)+1))
  {
    CAU::log("  Read2 FAILED\n");
  }

  CAU::log("Done - eeprom\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// caTestAnalog - Tests the different analog pins
// returns  - NA
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void caTestAnalog()
{
  uint16 val;
  hwPortPin ppAn;   // Analog Port
  hwPortPin ppDig;  // Port used to change analog's voltage level

  CAU::initializeAnalog();

  ppAn = CAU::getModulePin(0, 0);
  ppDig = CAU::getModulePin(0, 1);
  caTestAnalogPin(ppAn, ppDig);
  ppAn = CAU::getModulePin(0, 1);
  ppDig = CAU::getModulePin(0, 0);
  caTestAnalogPin(ppAn, ppDig);
  ppAn = CAU::getModulePin(0, 2);
  ppDig = CAU::getModulePin(0, 3);
  caTestAnalogPin(ppAn, ppDig);
  ppAn = CAU::getModulePin(1, 0);
  ppDig = CAU::getModulePin(1, 1);
  caTestAnalogPin(ppAn, ppDig);
  ppAn = CAU::getModulePin(2, 0);
  ppDig = CAU::getModulePin(2, 1);
  caTestAnalogPin(ppAn, ppDig);
  ppAn = CAU::getModulePin(2, 1);
  ppDig = CAU::getModulePin(2, 0);
  caTestAnalogPin(ppAn, ppDig);
  ppAn = CAU::getModulePin(2, 2);
  ppDig = CAU::getModulePin(2, 3);
  caTestAnalogPin(ppAn, ppDig);
  ppAn = CAU::getModulePin(2, 4);
  ppDig = CAU::getModulePin(2, 5);
  caTestAnalogPin(ppAn, ppDig);
  ppAn = CAU::getModulePin(2, 5);
  ppDig = CAU::getModulePin(2, 4);
  caTestAnalogPin(ppAn, ppDig);
  ppAn = CAU::getModulePin(3, 0);
  ppDig = CAU::getModulePin(3, 1);
  caTestAnalogPin(ppAn, ppDig);
  ppAn = CAU::getAuxPin(33);
  ppDig = CAU::getAuxPin(32);
  caTestAnalogPin(ppAn, ppDig);
  ppAn = CAU::getAuxPin(34);
  ppDig = CAU::getAuxPin(35);
  caTestAnalogPin(ppAn, ppDig);
  ppAn = CAU::getAuxPin(35);
  ppDig = CAU::getAuxPin(34);
  caTestAnalogPin(ppAn, ppDig);
  ppAn = CAU::getAuxPin(36);
  ppDig = CAU::getAuxPin(37);
  caTestAnalogPin(ppAn, ppDig);

  ppAn = CAU::getOnboardDevicePin(LV_DETECT_PIN);
  CAU::pinMode(ppAn, ANALOG_INPUT);
  val = CAU::analogRead(ppAn); // expected value is 5V/4 = 1.25V -- 1.25*4095/3.3 =1551
  if ((val < 1350) || (val > 1650))
  {
    CAU::log("  Failed Analog Test -- LV_DETECT_PIN %d\n", val);
  }
  
  CAU::log("Done - analog\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Test helper functions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void caTestAnalogPin(hwPortPin ppAn, hwPortPin ppDig)
{
  uint valLow, valHigh;

  caAllPortsLow(); // Helps detect shorted pins
  CAU::pinMode(ppAn, ANALOG_INPUT);
  CAU::pinMode(ppDig, OUTPUT);

  CAU::digitalWrite(ppDig, LOW);
  delayMicroseconds(20);  // I think this is needed due to capacitance in ribbon cable
  valLow = CAU::analogRead(ppAn);
  CAU::digitalWrite(ppDig, HIGH);
  delayMicroseconds(20);  // I think this is needed due to capacitance in ribbon cable
  valHigh = CAU::analogRead(ppAn);
  if (valLow >= 70 || valHigh <=4050)
  {
    CAU::log("  Failed Analog Test pp(%d, %d) - %d %d\n", ppAn.port, ppAn.pin, valLow, valHigh);
  }
}

boolean caTestTwoPins(hwPortPin ppIn, hwPortPin ppOut)
{
  uint8 val0, val1;
  boolean ret = true;

  caAllPortsLow(); // Helps detect shorted pins
  CAU::pinMode(ppOut, OUTPUT);
  CAU::digitalWrite(ppOut, LOW);
  CAU::pinMode(ppIn, INPUT);
  val0 = CAU::digitalRead(ppIn);
  CAU::digitalWrite(ppOut, HIGH);
  val1 = CAU::digitalRead(ppIn);
  if ((val0 != LOW) || (val1 != HIGH))
  {
    ret = false;
  }
  
  return ret;
}

// Set all the ports to output low
void caAllPortsLow()
{
  hwPortPin pp0;
  uint8 i, j;

  // Aux Port
  for(i=0; i<46; ++i)
  {
    pp0 = CAU::getAuxPin(i);
    CAU::pinMode(pp0, OUTPUT);
    CAU::digitalWrite(pp0, LOW);
  }
  
  // Module Ports
  for(i=0; i<4; ++i)
  {
    for(j=0; j<6; ++j)
    {
      pp0 = CAU::getModulePin(i, j);
      CAU::pinMode(pp0, OUTPUT);
      CAU::digitalWrite(pp0, LOW);
    }
  }

  // Camera Ports
  for(i=0; i<8; ++i)
  {
    pp0 = CAU::getCameraPin(i, FOCUS);
    CAU::pinMode(pp0, OUTPUT);
    CAU::digitalWrite(pp0, LOW);
    pp0 = CAU::getCameraPin(i, SHUTTER);
    CAU::pinMode(pp0, OUTPUT);
    CAU::digitalWrite(pp0, LOW);
  }

  // Link Port
  pp0 = CAU::getLinkPin(FOCUS);
  CAU::pinMode(pp0, OUTPUT);
  CAU::digitalWrite(pp0, LOW);
  pp0 = CAU::getLinkPin(SHUTTER);
  CAU::pinMode(pp0, OUTPUT);
  CAU::digitalWrite(pp0, LOW);
}

