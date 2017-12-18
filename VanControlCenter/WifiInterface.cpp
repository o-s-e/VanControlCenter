#include "WifiInterface.h"

void WifiInterfaceClass::init() {
    //Init
    rxBuffer_.resize(WIFI_RX_BUFFER_SIZE);

    //Load cfg
    Configuration cfg;

    //No handler
    gpsHandler_ = NULL;

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
        while (WIFI_SERIAL.available() && rxBuffer_.getSize() < WIFI_RX_BUFFER_SIZE) {
            rxBuffer_.append(WIFI_SERIAL.read());
        }

        Log << "buffer size: " << rxBuffer_.getSize() << Endl;
        Log << "buffer: " << Hex << Log.array<byte>(rxBuffer_.data(), rxBuffer_.getSize()) << Endl;

        if (parsePacket(INFO_PACKET, reinterpret_cast<byte*>(&info_), sizeof(infoData))) {
            if (info_.ack == getAck(reinterpret_cast<byte*>(&info_), sizeof(infoData) - 1)) {
                channelsBuffer.setValue(CanId::DATE, reinterpret_cast<byte*>(info_.date), sizeof(info_.date));
                channelsBuffer.setValue(CanId::TIME, reinterpret_cast<byte*>(info_.time), sizeof(info_.time));

                Log << "parsed info" << Endl;
                Log << "buffer size: " << rxBuffer_.getSize() << Endl;
                Log << "buffer: " << Hex << Log.array<byte>(rxBuffer_.data(), rxBuffer_.getSize()) << Endl;
            }
            else {
                Log.e(WIFI_TAG) << F("INVALID INFO DATA") << Endl;
                Log.e(WIFI_TAG) << Hex << Log.array<byte>(reinterpret_cast<byte*>(&info_), sizeof(infoData)) << Endl;
            }
        }

        if (parsePacket(GPS_PACKET, reinterpret_cast<byte*>(&gps_), sizeof(gpsData))) {
            if (gps_.ack == getAck(reinterpret_cast<byte*>(&gps_), sizeof(gpsData) - 1)) {
                channelsBuffer.setValue<double>(CanId::GPS_LATITUDE, gps_.latitude);
                channelsBuffer.setValue<double>(CanId::GPS_LONGITUDE, gps_.longitude);
                channelsBuffer.setValue<double>(CanId::GPS_ALTITUDE, gps_.altitude);
                channelsBuffer.setValue<double>(CanId::GPS_SPEED, gps_.speed);
                channelsBuffer.setValue<double>(CanId::GPS_ACCURACY, gps_.accuracy);

                //Invoke gps handler
                if (gpsHandler_ != NULL) {
                    gpsHandler_(gps_);
                }

                Log << "parsed gps" << Endl;
                Log << "buffer size: " << rxBuffer_.getSize() << Endl;
                Log << "buffer: " << Hex << Log.array<byte>(rxBuffer_.data(), rxBuffer_.getSize()) << Endl;
            }
            else {
                Log.e(WIFI_TAG) << F("INVALID GPS DATA") << Endl;
                Log.e(WIFI_TAG) << Hex << Log.array<byte>(reinterpret_cast<byte*>(&gps_), sizeof(gpsData)) << Endl;
            }
        }

        if (parsePacket(TEMP_PACKET, reinterpret_cast<byte*>(&temp_), sizeof(tempData))) {
            if (temp_.ack == getAck(reinterpret_cast<byte*>(&temp_), sizeof(tempData) - 1)) {
                channelsBuffer.setValue<double>(CanId::TEMP, temp_.temp);
                channelsBuffer.setValue<uint8_t>(CanId::HEATER_STATUS, temp_.status);

                Log << "parsed temp" << Endl;
                Log << "buffer size: " << rxBuffer_.getSize() << Endl;
                Log << "buffer: " << Hex << Log.array<byte>(rxBuffer_.data(), rxBuffer_.getSize()) << Endl;
            }
            else {
                Log.e(WIFI_TAG) << F("INVALID TEMP DATA") << Endl;
                Log.e(WIFI_TAG) << Hex << Log.array<byte>(reinterpret_cast<byte*>(&temp_), sizeof(tempData)) << Endl;
            }
        }

        if (parsePacket(LIGHT_PACKET, reinterpret_cast<byte*>(&light_), sizeof(lightData))) {
            if (light_.ack == getAck(reinterpret_cast<byte*>(&light_), sizeof(lightData) - 1)) {
                channelsBuffer.setValue<uint8_t>(CanId::LIGHT_1, light_.light1);
                channelsBuffer.setValue<uint8_t>(CanId::LIGHT_2, light_.light2);
                channelsBuffer.setValue<uint8_t>(CanId::LIGHT_3, light_.light3);
                channelsBuffer.setValue<uint8_t>(CanId::LIGHT_4, light_.light4);
                channelsBuffer.setValue<uint8_t>(CanId::LIGHT_5, light_.light5);
                channelsBuffer.setValue<uint8_t>(CanId::LIGHT_6, light_.light6);

                Log << "parsed light" << Endl;
                Log << "buffer size: " << rxBuffer_.getSize() << Endl;
                Log << "buffer: " << Hex << Log.array<byte>(rxBuffer_.data(), rxBuffer_.getSize()) << Endl;
            }
            else {
                Log.e(WIFI_TAG) << F("INVALID TEMP DATA") << Endl;
                Log.e(WIFI_TAG) << Hex << Log.array<byte>(reinterpret_cast<byte*>(&temp_), sizeof(tempData)) << Endl;
            }
        }

        Log << "buffer size: " << rxBuffer_.getSize() << Endl;
        Log << "buffer: " << Hex << Log.array<byte>(rxBuffer_.data(), rxBuffer_.getSize()) << Endl;
    }
}

boolean WifiInterfaceClass::parsePacket(const char* header, byte* buffer, int size) {
    int headerSize = strlen(header);

    if (rxBuffer_.getSize() >= size + headerSize) {
        int index = rxBuffer_.indexOf(header);
        if (index != -1) {
            index += headerSize;
            if (rxBuffer_.getSize() - index >= size) {
                for (int i = 0; i < size; i++) {
                    buffer[i] = rxBuffer_.at(index + i);
                }
                rxBuffer_.remove(index - headerSize, size + headerSize);
                return true;
            }
        }
    }

    return false;
}

void WifiInterfaceClass::setGpsDataHandler(gpsDataHandler gpsHandler) {
    this->gpsHandler_ = gpsHandler;
}

WifiInterfaceClass wifiInterface;