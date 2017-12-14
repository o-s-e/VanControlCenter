#include "WifiInterface.h"

void WifiInterfaceClass::init() {
    //Init
    rxBuffer.resize(WIFI_RX_BUFFER_SIZE);

    //Load cfg
    Configuration cfg;

    //No handler
    gpsHandler = NULL;

    //Reset module
    digitalWrite(WIFI_RST_PIN, HIGH);
}

void WifiInterfaceClass::update() {
    /*
    while (1){
        while (BL_SERIAL.available()){
            Log << Hex << BL_SERIAL.read() << " ";
        }
    }
    */

    //Read all bytes on serial
    if (WIFI_SERIAL.available()) {
        while (WIFI_SERIAL.available() && rxBuffer.getSize() < WIFI_RX_BUFFER_SIZE) {
            rxBuffer.append(WIFI_SERIAL.read());
        }

        Log << "buffer size: " << rxBuffer.getSize() << Endl;
        Log << "buffer: " << Hex << Log.array<byte>(rxBuffer.data(), rxBuffer.getSize()) << Endl;

        if (parsePacket(INFO_PACKET, reinterpret_cast<byte*>(&info), sizeof(InfoData))) {
            if (info.ack == getAck(reinterpret_cast<byte*>(&info), sizeof(InfoData) - 1)) {
                channelsBuffer.setValue(CanID::DATE, reinterpret_cast<byte*>(info.date), sizeof(info.date));
                channelsBuffer.setValue(CanID::TIME, reinterpret_cast<byte*>(info.time), sizeof(info.time));

                Log << "parsed info" << Endl;
                Log << "buffer size: " << rxBuffer.getSize() << Endl;
                Log << "buffer: " << Hex << Log.array<byte>(rxBuffer.data(), rxBuffer.getSize()) << Endl;
            }
            else {
                Log.e(WIFI_TAG) << F("INVALID INFO DATA") << Endl;
                Log.e(WIFI_TAG) << Hex << Log.array<byte>(reinterpret_cast<byte*>(&info), sizeof(InfoData)) << Endl;
            }
        }

        if (parsePacket(GPS_PACKET, reinterpret_cast<byte*>(&gps), sizeof(GpsData))) {
            if (gps.ack == getAck(reinterpret_cast<byte*>(&gps), sizeof(GpsData) - 1)) {
                channelsBuffer.setValue<double>(CanID::GPS_LATITUDE, gps.latitude);
                channelsBuffer.setValue<double>(CanID::GPS_LONGITUDE, gps.longitude);
                channelsBuffer.setValue<double>(CanID::GPS_ALTITUDE, gps.altitude);
                channelsBuffer.setValue<double>(CanID::GPS_SPEED, gps.speed);
                channelsBuffer.setValue<double>(CanID::GPS_ACCURACY, gps.accuracy);

                //Invoke gps handler
                if (gpsHandler != NULL) {
                    gpsHandler(gps);
                }

                Log << "parsed gps" << Endl;
                Log << "buffer size: " << rxBuffer.getSize() << Endl;
                Log << "buffer: " << Hex << Log.array<byte>(rxBuffer.data(), rxBuffer.getSize()) << Endl;
            }
            else {
                Log.e(WIFI_TAG) << F("INVALID GPS DATA") << Endl;
                Log.e(WIFI_TAG) << Hex << Log.array<byte>(reinterpret_cast<byte*>(&gps), sizeof(GpsData)) << Endl;
            }
        }

        if (parsePacket(TEMP_PACKET, reinterpret_cast<byte*>(&temp), sizeof(TempData))) {
            if (temp.ack == getAck(reinterpret_cast<byte*>(&temp), sizeof(TempData) - 1)) {
                channelsBuffer.setValue<double>(CanID::TEMP, temp.temp);
                channelsBuffer.setValue<uint8_t>(CanID::HEATER_STATUS, temp.status);

                Log << "parsed temp" << Endl;
                Log << "buffer size: " << rxBuffer.getSize() << Endl;
                Log << "buffer: " << Hex << Log.array<byte>(rxBuffer.data(), rxBuffer.getSize()) << Endl;
            }
            else {
                Log.e(WIFI_TAG) << F("INVALID TEMP DATA") << Endl;
                Log.e(WIFI_TAG) << Hex << Log.array<byte>(reinterpret_cast<byte*>(&temp), sizeof(TempData)) << Endl;
            }
        }

        if (parsePacket(LIGHT_PACKET, reinterpret_cast<byte*>(&light), sizeof(LightData))) {
            if (light.ack == getAck(reinterpret_cast<byte*>(&light), sizeof(LightData) - 1)) {
                channelsBuffer.setValue<uint8_t>(CanID::LIGHT_1, light.light1);
                channelsBuffer.setValue<uint8_t>(CanID::LIGHT_2, light.light2);
                channelsBuffer.setValue<uint8_t>(CanID::LIGHT_3, light.light3);
                channelsBuffer.setValue<uint8_t>(CanID::LIGHT_4, light.light4);
                channelsBuffer.setValue<uint8_t>(CanID::LIGHT_5, light.light5);
                channelsBuffer.setValue<uint8_t>(CanID::LIGHT_6, light.light6);

                Log << "parsed light" << Endl;
                Log << "buffer size: " << rxBuffer.getSize() << Endl;
                Log << "buffer: " << Hex << Log.array<byte>(rxBuffer.data(), rxBuffer.getSize()) << Endl;
            }
            else {
                Log.e(WIFI_TAG) << F("INVALID TEMP DATA") << Endl;
                Log.e(WIFI_TAG) << Hex << Log.array<byte>(reinterpret_cast<byte*>(&temp), sizeof(TempData)) << Endl;
            }
        }

        Log << "buffer size: " << rxBuffer.getSize() << Endl;
        Log << "buffer: " << Hex << Log.array<byte>(rxBuffer.data(), rxBuffer.getSize()) << Endl;
    }
}

boolean WifiInterfaceClass::parsePacket(const char* header, byte* buffer, int size) {
    int headerSize = strlen(header);

    if (rxBuffer.getSize() >= size + headerSize) {
        int index = rxBuffer.indexOf(header);
        if (index != -1) {
            index += headerSize;
            if (rxBuffer.getSize() - index >= size) {
                for (int i = 0; i < size; i++) {
                    buffer[i] = rxBuffer.at(index + i);
                }
                rxBuffer.remove(index - headerSize, size + headerSize);
                return true;
            }
        }
    }

    return false;
}

void WifiInterfaceClass::setGpsDataHandler(GpsDataHandler gpsHandler) {
    this->gpsHandler = gpsHandler;
}

WifiInterfaceClass wifiInterface;