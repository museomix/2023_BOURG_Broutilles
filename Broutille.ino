#include <SoftwareSerial.h>

#include <DFMiniMp3.h>
class Mp3Notify;

const int buttonPin = 4 ;	Replaced by PIR sensor

int buttonState = 0;
int buttonStateOld = 0;
int playState = 0;
int volumeLevel = 30;
int isPlaying=0;

SoftwareSerial secondarySerial(10, 11); // RX, TX
typedef DFMiniMp3<SoftwareSerial, Mp3Notify> DfMp3;
DfMp3 dfmp3(secondarySerial);

void setup()
{
	Serial.begin(115200);

	dfmp3.begin();
	dfmp3.reset();
	dfmp3.setVolume(volumeLevel);

	//  dfmp3.playMp36FolderTrack(1); // random of all folders on sd

	Serial.print("buttonPin : ");
	Serial.print(buttonPin);
	pinMode(buttonPin,INPUT);
}

void loop()
{
	dfmp3.loop();
	buttonState = digitalRead(buttonPin); 
	if ( buttonStateOld != buttonState && !isPlaying )
	{
       buttonStateOld = buttonState ;
		// check if the pushbutton is pressed If it is, the buttonState is HIGH:
		if (buttonState == HIGH)
		{
			// turn LED on:
			Serial.println("Button HIGH");
	        playState = 1 - playState ;   // If switch for tests
	        playState = 1 ;
	        if ( playState )
	        {
				dfmp3.playGlobalTrack(1);
				isPlaying=1;
	        }
	        else  dfmp3.pause();
		}
		else
		{
			// turn LED off:
			Serial.println("Button LOW");
		}
       delay(200);
	}
}

class Mp3Notify
{
	public:
		static void PrintlnSourceAction(DfMp3_PlaySources source, const char* action)
		{
			if (source & DfMp3_PlaySources_Sd)
			{
				Serial.print("SD Card, ");
			}
			if (source & DfMp3_PlaySources_Usb)
			{
				Serial.print("USB Disk, ");
			}
			if (source & DfMp3_PlaySources_Flash)
			{
				Serial.print("Flash, ");
			}
			Serial.println(action);
		}
		static void OnError([[maybe_unused]] DfMp3& mp3, uint16_t errorCode)
		{
			// see DfMp3_Error for code meaning
			Serial.println();
			Serial.print("Com Error ");
			Serial.println(errorCode);
		}
		static void OnPlayFinished([[maybe_unused]] DfMp3& mp3, [[maybe_unused]] DfMp3_PlaySources source, uint16_t track)
		{
			Serial.print("Play finished for #");
			Serial.println(track);
			isPlaying=0;
			// start next track
			track += 1;
			// this exFolderTrack(track);  // sd:/mp3/0001.mp3, sd:/mp3/0002.mp3, sd:/mp3/0003.mp3
		}
		static void OnPlaySourceOnline([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source)
		{
			PrintlnSourceAction(source, "online");
		}
		static void OnPlaySourceInserted([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source)
		{
			PrintlnSourceAction(source, "inserted");
		}
		static void OnPlaySourceRemoved([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source)
		{
			PrintlnSourceAction(source, "removed");
		}
};
