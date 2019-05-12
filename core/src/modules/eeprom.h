#include <EEPROM.h>


struct data_calibration
{
    float ecCal;
    float phCal;
};

data_calibration calibrationData;
int station = 1;

class EEPROM_Manager
{
  public:
    EEPROM_Manager(){};
    static int init_byte;
    static int calibration_byte;
    static int station_byte;
    static void InitEEPROM()
    {

#if !defined(ARDUINO_ARCH_AVR)
        // EEPROM.begin(EEPROM_SIZE);
        EEPROM.begin(2048);
#endif
        byte init;
        EEPROM.get(init_byte, init);
        if (init == 2)
        {
            
            Serial.println("INFO-GETMEMORY");
            GetAll();
        }
        else
        {
            Serial.println("INFO-CREATEMEMORY");
            data_calibration cal = {1.0, 1.0};
            EEPROM.put(calibration_byte, cal);
            EEPROM.put(station_byte, 1);
            EEPROM.put(init_byte, 2);
            Commit();
            GetAll();
        }
    }
    
    static void UpdateCalibration()
    {
        Serial.println("Update Calibraition: " + String(calibrationData.ecCal) + " " + String(calibrationData.phCal));
        EEPROM.put(calibration_byte, calibrationData);
        Commit();
    }
    
    
    static void UpdateStation()
    {
        Serial.println("Update station: " + String(station));
        EEPROM.put(station_byte, station);
        Commit();
    }

    static void GetAll()
    {
        EEPROM.get(calibration_byte, calibrationData);
        EEPROM.get(station_byte, station);
        Serial.println("Get Calibraition: " + String(calibrationData.ecCal) + " " + String(calibrationData.phCal));
        Serial.println("station: " + String(station));
    }

    static void Commit()
    {
#if !defined(ARDUINO_ARCH_AVR)
        EEPROM.commit();
#endif
    }
};
int EEPROM_Manager::init_byte = 0;
int EEPROM_Manager::calibration_byte = 20;
int EEPROM_Manager::station_byte = 10;