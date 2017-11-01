
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_TSL2561_U.h>
#include <ESP.h>
#include <base64.h>

// All specific changes needed for ESP8266 need be made in hal.cpp if possible
// Include ESP environment definitions in lmic.h (lmic/limic.h) if needed
#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <Wire.h>
   
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

// LoRaWAN Application identifier (AppEUI)
static const u1_t APPEUI[8]  = { 0x40, 0x43, 0x45, 0x4E, 0x40, 0x00, 0x6F, 0x49 };

// LoRaWAN DevEUI, unique device ID (LSBF)
static const u1_t DEVEUI[8]  = { 0x4D, 0x40, 0x45, 0x46, 0x4F, 0xBE, 0xAD, 0xDE };

// LoRaWAN NwkSKey, network session key 
// Use this key for The Things Network
static const u1_t DEVKEY[16] = { 0xDD, 0x7D, 0xBD, 0xDD, 0x6F, 0x04, 0xF7, 0xDD, 0xDD, 0x4D, 0xE8, 0xAC, 0xEA, 0x44, 0x1F, 0x47 };

// LoRaWAN AppSKey, application session key
// Use this key to get your data decrypted by The Things Network
static const u1_t ARTKEY[16] = { 0x01, 0xF2, 0xE3, 0x64, 0x75, 0x95, 0x06, 0x57, 0x6F, 0x06, 0x1C, 0x58, 0x2C, 0x2D, 0xCD, 0xAA };

// LoRaWAN end-device address (DevAddr)
// See http://thethingsnetwork.org/wiki/AddressSpace
static const u4_t DEVADDR = 0x26031F39; // <-- Change this address for every node! ESP8266 node 0x01

unsigned long previousMillis = 0;
uint8_t mydata[8] = {0};
static osjob_t sendjob;

uint16_t light_val = 0;
uint16_t pressure_val = 0;
uint16_t temp_val = 0;
uint16_t altitude_val = 0;

/**
 * Displays some basic information on this sensor from the unified
 * sensor API sensor_t type (see Adafruit_Sensor for more information)
 */
void displaySensorDetails(void)
{
    sensor_t sensor;
    bmp.getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.print  ("Sensor:       "); Serial.println(sensor.name);
    Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
    Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
    Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" hPa");
    Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" hPa");
    Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" hPa");  
    Serial.println("------------------------------------");
    Serial.println("");
    delay(500);
    
    tsl.getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.print  ("Sensor:       "); Serial.println(sensor.name);
    Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
    Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
    Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" lux");
    Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" lux");
    Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" lux");  
    Serial.println("------------------------------------");
    Serial.println("");
    delay(500);
}

/**
 * 
 * 
 * 
 */
void configureSensor(void)
{
    /* You can also manually set the gain or enable auto-gain support */
    // tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
    // tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
    tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
    
    /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
    tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
    // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
    // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */
    
    /* Update these values depending on what you've set above! */  
    Serial.println("------------------------------------");
    Serial.print  ("Gain:         "); Serial.println("Auto");
    Serial.print  ("Timing:       "); Serial.println("13 ms");
    Serial.println("------------------------------------");
}

/**
 * provide application router ID (8 bytes, LSBF) 
 */
void os_getArtEui (u1_t* buf)
{
    memcpy(buf, APPEUI, 8);
}

/** 
 * provide device ID (8 bytes, LSBF)
 */
void os_getDevEui (u1_t* buf)
{
    memcpy(buf, DEVEUI, 8);
}

/**
 * provide device key (16 bytes) 
 */
void os_getDevKey (u1_t* buf)
{
    memcpy(buf, DEVKEY, 16);
}

// Pin mapping
// XXX We have to see whether all these pins are really used
// if not, we can use them for real sensor work.
lmic_pinmap pins = {
    .nss = 15,          // Make D8/GPIO15, is nSS on ESP8266
    .rxtx = 2,          // D4/GPIO2. For placeholder only,
                        // Do not connected on RFM92/RFM95
    .rst = 0,           // Make D0/GPIO16, Needed on RFM92/RFM95? (probably not)
    .dio = {5, 4, 3},   // Specify pin numbers for DIO0, 1, 2
                        // D1/GPIO5,D2/GPIO4,D3/GPIO3 are usable pins on ESP8266
                        // NOTE: D3 not really usable when UART not connected
                        // As it is used during bootup and will probably not boot.
                        // Leave D3 Pin unconnected for sensor to work
};

/**
 * 
 * 
 */
void onEvent (ev_t ev)
{
    //debug_event(ev);
    
    switch(ev) {
        // scheduled data sent (optionally data received)
        // note: this includes the receive window!
        case EV_TXCOMPLETE:
            // use this event to keep track of actual transmissions
            Serial.print("Event EV_TXCOMPLETE, time: ");
            Serial.println(millis() / 1000);
            if (LMIC.dataLen) { // data received in rx slot after tx
                //debug_buf(LMIC.frame+LMIC.dataBeg, LMIC.dataLen);
                Serial.println("Data Received!");
            }
            break;
        default:
            break;
    }
}

/**
 * 
 * 
 */
void do_send(osjob_t* j)
{
    Serial.print("Time: ");
    Serial.println(millis() / 1000);
    // Show TX channel (channel numbers are local to LMIC)
    Serial.print("Send, txCnhl: ");
    Serial.println(LMIC.txChnl);
    Serial.print("Opmode check: ");
    
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & (1 << 7)) {
        Serial.println("OP_TXRXPEND, not sending");
    } else {
        Serial.println("ok");
        // Prepare upstream data transmission at the next possible time.
        mydata[0] = light_val & 0xFF;
        mydata[1] = (light_val >> 2) & 0xFF;
        mydata[2] = pressure_val & 0xFF;
        mydata[3] = (pressure_val >> 2) & 0xFF;
        mydata[4] = temp_val & 0xFF;
        mydata[5] = (temp_val >> 2) & 0xFF;
        mydata[6] = altitude_val & 0xFF;
        mydata[7] = (altitude_val >> 2) && 0xFF;

        LMIC_setTxData2(1, mydata, sizeof(mydata), 0);
    }
    // Schedule a timed job to run at the given timestamp (absolute system time)
    os_setTimedCallback(j, os_getTime()+sec2osticks(60), do_send);       
}

/**
 * 
 * 
 */
void setup(void) 
{
    Wire.begin();
    Serial.begin(57600);
    Serial.println("Pressure Sensor Test");
    Serial.println("");
    
    /* Initialise the sensor */
    if (!bmp.begin()) {
        /* There was a problem detecting the BMP085 ... check your connections */
        Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
        while(1);
    }
    
    /* Initialise the sensor */
    if (!tsl.begin()) {
        /* There was a problem detecting the ADXL345 ... check your connections */
        Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
        while(1);
    }
    
    /* Display some basic information on this sensor */
    displaySensorDetails();
    
    // LMIC init
    os_init();
    Serial.println("os_init() finished");
    
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();
    Serial.println("LMIC_reset() finished");
    
    // Set static session parameters. Instead of dynamically establishing a session 
    // by joining the network, precomputed session parameters are be provided.
    LMIC_setSession (0x1, DEVADDR, (uint8_t*)DEVKEY, (uint8_t*)ARTKEY);
    Serial.println("LMIC_setSession() finished");
    
    // Disable data rate adaptation
    LMIC_setAdrMode(0);
    Serial.println("LMICsetAddrMode() finished");
    
    // Disable link check validation
    LMIC_setLinkCheckMode(0);
    // Disable beacon tracking
    LMIC_disableTracking ();
    // Stop listening for downstream data (periodical reception)
    LMIC_stopPingable();
    // Set data rate and transmit power (note: txpow seems to be ignored by the library)
    LMIC_setDrTxpow(DR_SF10,14);
    //
    //Serial.flush();
    Serial.println("Init done");
}

/**
 * 
 * 
 */
void loop(void) 
{
    do_send(&sendjob);
    delay(10);

    while (1) {
        os_runloop_once();

        /* Get a new sensor event */ 
        sensors_event_t event;
        bmp.getEvent(&event);
        
        /* Display the results (barometric pressure is measure in hPa) */
        if (event.pressure)
        {
            Serial.print("Pressure:    ");
            Serial.print(event.pressure);
            Serial.println(" hPa");
            pressure_val = (uint16_t) ((event.pressure / 2) * 100);
        
            /* Calculating altitude with reasonable accuracy requires pressure   *
            * sea level pressure for your position at the moment the data is     *
            * converted, as well as the ambient temperature in degress           *
            * celcius.  If you don't have these values, a 'generic' value of     *
            * 1013.25 hPa can be used (defined as SENSORS_PRESSURE_SEALEVELHPA   *
            * in sensors.h), but this isn't ideal and will give variable         *
            * results from one day to the next.                                  *
            *                                                                    *
            * You can usually find the current SLP value by looking at weather   *
            * websites or from environmental information centers near any major  *
            * airport.                                                           *
            *                                                                    *
            * For example, for Paris, France you can check the current mean      *
            * pressure and sea level at: http://bit.ly/16Au8ol                   */
            
            /* First we get the current temperature from the BMP085 */
            float temperature;
            bmp.getTemperature(&temperature);
            Serial.print("Temperature: ");
            Serial.print(temperature);
            Serial.println(" C");
            temp_val = (uint16_t) (temperature * 100);
            
            /* Then convert the atmospheric pressure, and SLP to altitude         */
            /* Update this next line with the current SLP for better results      */
            float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
            Serial.print("Altitude:    "); 
            Serial.print(bmp.pressureToAltitude(seaLevelPressure, event.pressure)); 
            Serial.println(" m");
            Serial.println("");
            altitude_val = (uint16_t) ((bmp.pressureToAltitude(seaLevelPressure, event.pressure)/2) * 100);
        } else {
            Serial.println("Sensor error");
        }
        
        tsl.getEvent(&event);
        
        /* Display the results (light is measured in lux) */
        if (event.light) {
            Serial.print(event.light);
            Serial.println(" lux");
            light_val = (uint16_t) event.light;
        } else {
            /* If event.light = 0 lux the sensor is probably saturated
            and no reliable data could be generated! */
            Serial.println("Sensor overload");
        }
        delay(1000);
    }
}
