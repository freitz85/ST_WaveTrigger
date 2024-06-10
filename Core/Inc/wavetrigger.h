#ifndef WAVETRIGGER_H
#define WAVETRIGGER_H

#include <cstdint>
//#include "stm32f4xx_hal_uart.h"

// Message format: ( SOM1, SOM2, length, message code, data * n , EOM )
//	where SOM1=0xf0, SOM2=0xaa,  EOM=0x55

#define CMD_GET_VERSION         0x01
#define CMD_GET_SYS_INFO        0x02
#define CMD_TRACK_CONTROL       0x03
#define CMD_STOP_ALL          	0x04
#define CMD_MASTER_VOLUME       0x05
#define CMD_TRACK_VOLUME        0x08
#define CMD_AMP_POWER         	0x09
#define CMD_TRACK_FADE          0x0a
#define CMD_RESUME_ALL_SYNC     0x0b
#define CMD_SAMPLERATE_OFFSET   0x0c
#define CMD_TRACK_CONTROL_EX    0x0d
#define CMD_SET_REPORTING       0x0e
#define CMD_SET_TRIGGER_BANK    0x0f

#define TRK_PLAY_SOLO         	0
#define TRK_PLAY_POLY         	1
#define TRK_PAUSE           	2
#define TRK_RESUME            	3
#define TRK_STOP            	4
#define TRK_LOOP_ON           	5
#define TRK_LOOP_OFF          	6
#define TRK_LOAD            	7

#define RSP_VERSION_STRING      0x81
#define RSP_SYSTEM_INFO         0x82
#define RSP_STATUS            	0x83
#define RSP_TRACK_REPORT        0x84

#define MAX_MESSAGE_LEN         32
#define MAX_NUM_VOICES          14
#define VERSION_STRING_LEN      21

#define SOM1  					0xf0
#define SOM2  					0xaa
#define EOM   					0x55

#define NUM_VOICES				8


enum PARSER_STATE { WAIT_SOM1, WAIT_SOM2, GET_LENGTH, GET_CMD, GET_DATA, WAIT_EOM };
enum PARSER_RESULT { IDLE, PARSING, PARSER_ERROR, FINISHED };

struct voice
{
	uint16_t trackNumber;
	uint32_t offset;
	uint8_t playFlag;
	uint8_t lockFlag;
	int16_t gain;
};

class Wavetrigger
{
private:
	enum PARSER_STATE m_parserState;
	enum PARSER_RESULT m_parserResult;
	uint8_t m_command;
	uint8_t m_rxLength;
	uint8_t m_txLength;
	uint8_t m_rxDataIndex;
	uint8_t m_rxData[255];
	uint8_t m_txData[255];
	uint16_t m_numTracks;
	uint8_t m_reportingEnabled;
	voice voices[NUM_VOICES];

	//UART_HandleTypeDef* m_pUart;

public:
	Wavetrigger() { Reset(); }
	void Reset() {
		m_parserState = WAIT_SOM1;
		m_parserResult = IDLE;
		m_rxLength = 0;
		m_txLength = 0;
		m_rxDataIndex = 0;
		m_txData[0] = SOM1;
		m_txData[1] = SOM2;
	}

	PARSER_RESULT Parse(uint8_t data);
	void DoCommand();
	void SetNumTracks(uint16_t numTracks) { m_numTracks = numTracks; }
	uint8_t GetReportingEnabled() { return m_reportingEnabled; }
	void SendTrackReport(uint16_t trackNumber, uint8_t voice, uint8_t state);

private:
	voice* GetFreeVoice();
	voice* GetVoice(uint16_t trackNumber);
	void SendResponse(uint8_t cmd, void* pData, uint8_t dataSize);
	void CmdGetVersion();
	void CmdGetSysInfo();
	void CmdTrackControl();
	void CmdStopAll();
	void CmdTrackVolume();
	void CmdTrackFade();
	void CmdSamplerateOffset();
	void CmdSetReporting();
};


#endif
