#include <Modbus.h>
#include "mcp_can.h"
#include <ModbusIP.h>


#include <Ethernet.h>
#include <SPI.h>

const int SPI_CS_PIN = 9;
MCP_CAN CAN(SPI_CS_PIN);
const int CAN_INT_PIN = 2;  // Interrupt pini (D2'ye bağla)




const int canMSG_ID_ir = 100;  //holding register
 int canMSG_ir = 101;  //holding register

//Modbus IP Object
ModbusIP mb;

void setup() {
  Serial.begin(115200);


  // The media access control (ethernet hardware) address for the shield
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  
  // The IP address for the shield
  byte ip[] = { 192, 168, 1, 120 };   
  //Config Modbus IP 
  mb.config(mac, ip);

  if (CAN.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) == CAN_OK)
      Serial.println("CAN BUS Başlatildi");
    else {
      Serial.println("CAN BUS Başlatilamadi");
      while (1);
    }

  CAN.setMode(MCP_NORMAL);  // Normal mod
  //attachInterrupt(digitalPinToInterrupt(CAN_INT_PIN), CAN_isr, LOW);

  mb.addHreg(canMSG_ID_ir);  //ad input register
  mb.addHreg(101);  //ad input register
   mb.addHreg(102);  //ad input register
  mb.addHreg(103);  //ad input register
  mb.addHreg(104);  //ad input register
  mb.addHreg(105);  //ad input register
  mb.addHreg(106);  //ad input register
  mb.addHreg(107);  //ad input register
  mb.addHreg(108);  //ad input register


  

}





uint16_t RXid_sent;
uint16_t RX_sent;

uint16_t RXid_sent_highPart;
uint16_t RXid_sent_lowPart;

uint16_t RX_sent_highPart;
uint16_t RX_sent_lowPart;





void loop() {

  mb.task(); // task ı yap
  

  long unsigned int rxId;
    unsigned char len = 0;
    unsigned char rxBuf[8];

    if (CAN_MSGAVAIL == CAN.checkReceive())
    {
      CAN.readMsgBuf(&rxId, &len, rxBuf);

      // C# uyumlu seri formatta gönder
      Serial.print("ID: ");
      //Serial.print(rxId, HEX);  // C# kodunda bu alınacak
      RXid_sent = (uint16_t)rxId;  //RX Sent string ve HEX formatına çevirildi
 /*     
      RXid_sent_highPart = (RXid_sent >> 16) & 0xFFFF; // Üst 16-bit (0x1A2B)
      RXid_sent_lowPart = RXid_sent & 0xFFFF;         // Alt 16-bit (0x3C4D)
      mb.Hreg(canMSG_ID_ir + 1, RXid_sent_highPart);
      mb.Hreg(canMSG_ID_ir + 2, RXid_sent_lowPart);
*/

      mb.Hreg(canMSG_ID_ir,  RXid_sent);
      Serial.print(RXid_sent);
/*
Serial.print("High part for id : 0x -->");
  Serial.print(RXid_sent_highPart, HEX);
  Serial.print("  Low part for id : 0x-->");
  Serial.println(RXid_sent_lowPart, HEX);

      Serial.print("32-bit veri yazıldı (canMSG_ID_ir)");
      Serial.print(canMSG_ID_ir + 1);
      Serial.print(" ve ");
      Serial.print(canMSG_ID_ir + 2);
      Serial.print("): 0x");
      Serial.println(RXid_sent, HEX);
*/

      Serial.print(" DATA: ");
        canMSG_ir = 101;

      for (int i = 0; i < len; i++) {
        if (rxBuf[i] < 0x10) Serial.print("0"); // Tek haneliler için 0 ekle
        //Serial.print(rxBuf[i], HEX);
        RX_sent = (uint16_t)(rxBuf[i]);  //RX Sent string ve HEX formatına çevirildi
        
        Serial.print(" -- ((canMSG_ir)) : ");
        Serial.print(canMSG_ir);
        Serial.print("  -  Mesaj: ");
        Serial.print(rxBuf[i] );

        mb.Hreg(canMSG_ir , RX_sent);
        canMSG_ir  = canMSG_ir+1;


        

        Serial.print(" ");
      }

  
      Serial.println();
      delay(1);
    }


    //mb.Ireg (canMSG_ir,  AllSent);  //input register a data object i eşleştirilir.

 
}



