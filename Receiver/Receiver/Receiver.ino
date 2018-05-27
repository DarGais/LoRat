    #include <RHReliableDatagram.h>
    #include <RH_RF95.h>
    #include <SPI.h>

    #define CLIENT_ADDRESS 101
    #define SERVER_ADDRESS 1

    RH_RF95 driver(8,7);
    RHReliableDatagram manager(driver, CLIENT_ADDRESS);

    void setup()
    {
      Serial.begin(9600);
      Serial1.begin(9600); 
      while (!Serial){
      }
      Serial.println("initializing");
      if (!manager.init())
        Serial.println("init failed");
        Serial.println("Setting freq");
        driver.setFrequency(915.0);
        driver.setTxPower(23, false);
        driver.setModemConfig(RH_RF95::Bw31_25Cr48Sf512);
        manager.setTimeout(4000);
    }
    uint8_t data[] = "This is Gateway.";
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];

    void loop()
    {    
      //Serial.println((char*)data);
      if (manager.sendtoWait(data, sizeof(data), SERVER_ADDRESS))
      {
        uint8_t len = sizeof(buf);
        uint8_t from; 
        //delay(2000);  
        if (manager.recvfromAckTimeout(buf, &len, 15000, &from))
        {
          //delay(2000);
          //Serial.print("Reply from,0x");
          //Serial.print("0x");
          //Serial.print(from, HEX);
          //Serial.print(",RSSI");
          //Serial.print(",");
          //Serial.print(driver.lastRssi());
          //Serial.print(",");
          //Serial.println((char*)buf);
          Serial.write(buf, len);
          //RH_RF95::printBuffer("Sent: ", buf, len);
          
        }
      }
      else
        Serial.println("No ACK");
      Serial.flush();
    }

