*** settings ***
Library    SerialLibrary      encoding=ascii
Library    String
Resource     ../resources/Serial.robot
Suite Setup        Open Serial Port    ${SERIAL_PORT_1}    ${SERIAL_PORT_2}
#Suite Setup        Serial.Open Serial Port    ${SERIAL_PORT_1}    
Suite Teardown     Serial.Close Serial Port

*** Variables ***
${SERIAL_PORT_1}    /dev/ttyUSB0
${SERIAL_PORT_2}  /dev/ttyUSB1
${VERBOSE}    1

*** test cases ***

Secured Connection Test
    [Timeout]     60s
    # inital sleep to allow device time to boot up after programming
    sleep    5
    Serial.send   pin 1 1234\n    ${SERIAL_PORT_2}
    sleep    2
    Serial.send    pin 1 1234\n    ${SERIAL_PORT_1}
    Serial.Expect And Timeout    >>> Pairing completed successfully <<<    25    ${SERIAL_PORT_1}

Write Characteristic Test
    [Timeout]     30s
    Serial.send    btn 2 l\n    ${SERIAL_PORT_1}
    Serial.Expect And Timeout    hello back    5    ${SERIAL_PORT_1}    

Write Secure Characteristic Test
    [Timeout]     30s
    Serial.send    btn 2 m\n    ${SERIAL_PORT_1}
    Serial.Expect And Timeout    Notification from secure data service    5    ${SERIAL_PORT_1}    


Phy Switching Test 
    [Timeout]     30s
    Serial.send    btn 2 s\n    ${SERIAL_PORT_1}
    Serial.Expect And Timeout    DM_PHY_UPDATE_IND - RX: 2, TX: 2    5    ${SERIAL_PORT_1}    


Speed Test  
    [Timeout]     120s
    Serial.send    btn 2 x\n    ${SERIAL_PORT_1}
    Serial.Expect And Timeout     bits transferred in    120    ${SERIAL_PORT_1}



