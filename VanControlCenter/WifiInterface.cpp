
#include "WifiInterface.h"


void WifiInterfaceClass::init(){
	//Init
	rxBuffer.resize(WIFI_RX_BUFFER_SIZE);
	
	//Load cfg
	Configuration cfg;
	if (cfg.loadFromFile(WIFI_CFG_FILE) == FILE_VALID){
		phoneToCall = cfg.getProperty(PHONE_NUM).asString();
	}
	else{
		phoneToCall = DEFAULT_PHONE_NUM;
		consoleForm.println(cfg.getErrorMsg());
		Log.e(WIFI_TAG) << cfg.getErrorMsg() << Endl;
	}
	
	//No handler
	gpsHandler = NULL;

	//Reset module
	digitalWrite(WIFI_RST_PIN, HIGH);
}

void WifiInterfaceClass::update(){
	/*
	while (1){
		while (BL_SERIAL.available()){
			Log << Hex << BL_SERIAL.read() << " ";
		}
	}
	*/

	//Read all bytes on serial
	if (WIFI_SERIAL.available()){
		while (WIFI_SERIAL.available() && rxBuffer.getSize() < WIFI_RX_BUFFER_SIZE){
			rxBuffer.append(WIFI_SERIAL.read());
		}

		//Log << "buffer size: " << rxBuffer.getSize() << Endl;
		//Log << "buffer: " << Hex << Log.array<byte>(rxBuffer.data(), rxBuffer.getSize()) << Endl;

		if (parsePacket(INFO_PACKET, (byte*)&info, sizeof(InfoData))){
			if (info.ack == getAck((byte*)&info, sizeof(InfoData) - 1)){
				channelsBuffer.setValue(CanID::DATE, (byte*)info.date, sizeof(info.date));
				channelsBuffer.setValue(CanID::TIME, (byte*)info.time, sizeof(info.time));
				
				//Log << "parsed info" << Endl;
				//Log << "buffer size: " << rxBuffer.getSize() << Endl;
				//Log << "buffer: " << Hex << Log.array<byte>(rxBuffer.data(), rxBuffer.getSize()) << Endl;
			}
			else{
				Log.e(WIFI_TAG) << F("INVALID INFO DATA") << Endl;
				Log.e(WIFI_TAG) << Hex << Log.array<byte>((byte*)&info, sizeof(InfoData)) << Endl;
			}			
		}

		if (parsePacket(GPS_PACKET, (byte*)&gps, sizeof(GpsData))){
			if (gps.ack == getAck((byte*)&gps, sizeof(GpsData) - 1)){
				channelsBuffer.setValue<double>(CanID::GPS_LATITUDE,	gps.latitude);
				channelsBuffer.setValue<double>(CanID::GPS_LONGITUDE,	gps.longitude);
				channelsBuffer.setValue<double>(CanID::GPS_ALTITUDE,	gps.altitude);
				channelsBuffer.setValue<double>(CanID::GPS_SPEED,		gps.speed);
				channelsBuffer.setValue<double>(CanID::GPS_ACCURACY,	gps.accuracy);

				//Invoke gps handler
				if (gpsHandler != NULL){
					gpsHandler(gps);
				}

				//Log << "parsed gps" << Endl;
				//Log << "buffer size: " << rxBuffer.getSize() << Endl;
				//Log << "buffer: " << Hex << Log.array<byte>(rxBuffer.data(), rxBuffer.getSize()) << Endl;
			}
			else{
				Log.e(WIFI_TAG) << F("INVALID GPS DATA") << Endl;
				Log.e(WIFI_TAG) << Hex << Log.array<byte>((byte*)&gps, sizeof(GpsData)) << Endl;
			}
		}

		if (parsePacket(TEMP_PACKET, (byte*)&temp, sizeof(TempData))){
			if (temp.ack == getAck((byte*)&temp, sizeof(TempData) - 1)){
				channelsBuffer.setValue<double>(CanID::TEMP,		temp.temp);
				channelsBuffer.setValue<byte>(CanID::TEMP_STATUS,	temp.status);
				
				//Log << "parsed acc" << Endl;
				//Log << "buffer size: " << rxBuffer.getSize() << Endl;
				//Log << "buffer: " << Hex << Log.array<byte>(rxBuffer.data(), rxBuffer.getSize()) << Endl;
			}
			else{
				Log.e(WIFI_TAG) << F("INVALID ACC DATA") << Endl;
				Log.e(WIFI_TAG) << Hex << Log.array<byte>((byte*)&temp, sizeof(TempData)) << Endl;
			}
		}

		
		if (parsePacket(CALL_PACKET, (byte*)&call, sizeof(CallData))){
			channelsBuffer.setValue<boolean>(CanID::CALL_STATUS,	call.status);
		}
		

		//Log << "buffer size: " << rxBuffer.getSize() << Endl;
		//Log << "buffer: " << Hex << Log.array<byte>(rxBuffer.data(), rxBuffer.getSize()) << Endl;

	}
}

void WifiInterfaceClass::startCall(){
	WIFI_SERIAL.print('\n');
	WIFI_SERIAL.print(CALL_CMD);
	WIFI_SERIAL.print(phoneToCall);
	WIFI_SERIAL.print('\n');
}

boolean WifiInterfaceClass::parsePacket(const char* header, byte* buffer, int size){
	int index;
	int headerSize = strlen(header);

	if (rxBuffer.getSize() >= size + headerSize){
		index = rxBuffer.indexOf(header);
		if (index != -1){
			index += headerSize;
			if (rxBuffer.getSize() - index >= size){
				for (int i = 0; i < size; i++){
					buffer[i] = rxBuffer.at(index + i);
				}
				rxBuffer.remove(index - headerSize, size + headerSize);
				return true;
			}
		}
	}
	
	return false;
}

void WifiInterfaceClass::setGpsDataHandler(GpsDataHandler gpsHandler){
	this->gpsHandler = gpsHandler;
}


WifiInterfaceClass wifiInterface;