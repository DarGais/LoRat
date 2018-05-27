    #include <RHReliableDatagram.h>
    #include <RH_RF95.h>
    #include <SPI.h>
    
    #define SERVER_ADDRESS 1
    RH_RF95 driver(8,7);
    RHReliableDatagram manager(driver, SERVER_ADDRESS);

    void setup()
    {
      Serial.begin(9600);
      while (!Serial) ; // Wait for serial port to be available
      if (!manager.init())
        Serial.println("init failed");
        Serial.println("Setting freq");
        driver.setFrequency(915.0);
        driver.setTxPower(23, false);
        driver.setModemConfig(RH_RF95::Bw31_25Cr48Sf512);
        manager.setTimeout(4000);
        //Serial.println("get");
    }

    //uint8_t data[] = "Pong";
    uint8_t data[32];
    int i = 0;
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    int s = 0;

    void loop()
    {
      if (manager.available())
      {
        int i = 0;
        uint8_t len = sizeof(buf);
        uint8_t from;
        if (manager.recvfromAck(buf, &len, &from))
        {
          Serial.print("got reply from : 0x");
          Serial.print(from, HEX);
          Serial.print(" : RSSI ");
          Serial.print(driver.lastRssi());
          Serial.print(" : ");
          Serial.println((char*)buf);
          if(s == 0){
            Serial.println("get");
            while(!Serial.available()){}
            while (Serial.available()){
              delay(60);  //delay to allow buffer to fill 
              if (Serial.available() >0){
                char c = Serial.read();  //gets one byte from serial buffer
                data[i] = c;
                i++;
              }
            }
          }

          Serial.println((char*)data);

          if (!manager.sendtoWait(data, sizeof(data), from)){
            Serial.println("No ACK-ACK");
            s = 1;
            //while(!manager.sendtoWait(data, sizeof(data), from));
          }
          else s = 0;
        }
      }
      Serial.flush();
      delay(5000);
    }

