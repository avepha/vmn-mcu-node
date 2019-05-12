
#include "header.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);


const char *ssid = "vmnnetwork";
const char *password = "raspberry";
const char *host = "192.168.4.1";


String CNT = "CNT";
String RST = "P";


#include "./modules/lcd.h"
#include "./modules/wifi.h"


TaskManager taskManager;


void coreTask(void *pvParameters)
{

   EEPROM_Manager::InitEEPROM();
   String taskMessage = "Task running on core ";
   taskMessage = taskMessage + xPortGetCoreID();
   VmnClient::instance();

   delay(2000);

   while (true)
   {
       delay(3000);

       // Use WiFiClient class to create TCP connections
       WiFiClient client;
       const int httpPort = 80;


       //connecting time ~ 4-5 ms
       Serial.println(">>> Client connecting !");
       if (!client.connect(host, httpPort))
       {
           Serial.println("connection failed");
       }
       Serial.println(">>> Client connected !");

       float ec = ECSensor::instance()->GetEC();
       float vol = LoadCellSensor::instance()->getVal();
       String url = "{" + String(station) + "," + String(ec) + "," + String(vol) + "}";
       client.print(url);

       unsigned long timeout = millis();
       while (client.available() == 0)
       {
           if (millis() - timeout > 3000)
           {
               Serial.println(">>> Client Timeout !");
               client.stop();
               break;
           }
       }

       // Read all the lines of the reply from server and print them to Serial
       while (client.available())
       {
           String line = client.readStringUntil('\r');
       }

       Serial.println();
       Serial.println("closing connection");
   }
}
void setup()
{
    taskManager.StartTask(LcdHandler::instance());
    EEPROM_Manager::InitEEPROM();
    taskManager.StartTask(VmnClient::instance());
    Serial.begin(115200);
    taskManager.StartTask(WaterSensor::instance());
    taskManager.StartTask(ECSensor::instance());
    taskManager.StartTask(LoadCellSensor::instance());


   xTaskCreatePinnedToCore(
       coreTask,   /* Function to implement the task */
       "coreTask", /* Name of the task */
       10000,      /* Stack size in words */
       NULL,       /* Task input parameter */
       0,          /* Priority of the task */
       NULL,       /* Task handle. */
       0);
}
void loop()
{
    taskManager.Loop();
}
