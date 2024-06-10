#include "wavetrigger.h"
#include <cstring>

PARSER_RESULT Wavetrigger::Parse(uint8_t data) {

	switch(m_parserState)
	{
	case WAIT_SOM1:
		if(data == SOM1)
		{
			m_parserResult = PARSING;
			m_parserState = WAIT_SOM2;
		}
		else
		{
			m_parserResult = PARSER_ERROR;
		}
		break;
	case WAIT_SOM2:
		if(data == SOM2)
		{
			m_parserState = GET_LENGTH;
		}
		else
		{
			m_parserResult = PARSER_ERROR;
		}
		break;
	case GET_LENGTH:
		m_rxLength = data - 4;
		m_parserState = GET_CMD;
		break;
	case GET_CMD:
		m_command = data;
		m_parserState = GET_DATA;
		break;
	case GET_DATA:
		m_rxData[m_rxDataIndex] = data;
		if(m_rxDataIndex == m_rxLength + 1)
		{
			m_parserState = WAIT_EOM;
		}
		else
		{
			m_rxDataIndex++;
		}
		break;
	case WAIT_EOM:
		if(data == EOM)
		{
			m_parserResult = FINISHED;
		}
		else
		{
			m_parserResult = PARSER_ERROR;
		}
		break;
	default:
		m_parserResult = PARSER_ERROR;
	}


	return m_parserResult;
}

void Wavetrigger::DoCommand()
{
	switch(m_command)
	{
	case CMD_GET_VERSION:
		CmdGetVersion();
		break;
	case CMD_GET_SYS_INFO:
		CmdGetSysInfo();
		break;
	case CMD_TRACK_CONTROL:
		CmdTrackControl();
		break;
	case CMD_STOP_ALL:
		CmdStopAll();
		break;
	case CMD_TRACK_VOLUME:
		CmdTrackVolume();
		break;
	case CMD_TRACK_FADE:
		CmdTrackFade();
		break;
	case CMD_SAMPLERATE_OFFSET:
		CmdSamplerateOffset();
		break;
	case CMD_SET_REPORTING:
		CmdSetReporting();
		break;
	default:
		// do nothing
		break;
	}


}

void Wavetrigger::SendTrackReport(uint16_t trackNumber, uint8_t voice, uint8_t state)
{
	char info[4];
	info[0] = (uint8_t)m_numTracks;
	info[1] = m_numTracks>>8;
	info[2] = voice;
	info[3] = state;

	SendResponse(RSP_TRACK_REPORT, info, sizeof(info));
}

voice* Wavetrigger::GetFreeVoice()
{
	voice* pVoice = nullptr;
	for(uint8_t n=0; n<NUM_VOICES; n++)
	{
		if((voices[n].playFlag == 0) && (voices[n].playFlag == 0))
		{
			pVoice = &voices[n];
			break;
		}
	}
	return pVoice;
}

voice* Wavetrigger::GetVoice(uint16_t trackNumber)
{
	voice* pVoice = nullptr;
	for(uint8_t n=0; n<NUM_VOICES; n++)
	{
		if(voices[n].trackNumber == trackNumber)
		{
			pVoice = &voices[n];
			break;
		}
	}
	return pVoice;
}

void Wavetrigger::SendResponse(uint8_t cmd, void* pData, uint8_t dataSize)
{
	m_txLength = dataSize + 5;
	m_txData[2] = m_txLength;
	m_txData[3] = cmd;
	memcpy(m_txData+4, pData, dataSize);
	m_txData[dataSize+5] = EOM;

	//HAL_UART_Transmit(m_pUart, m_txData, m_txLength, 0);
}

void Wavetrigger::CmdGetVersion(){
	const char* version = "StWavetrigger         ";
	SendResponse(RSP_VERSION_STRING, const_cast<char*>(version), strlen(version));
}

void Wavetrigger::CmdGetSysInfo(){
	char info[3];
	info[0] = NUM_VOICES;
	info[1] = (uint8_t)m_numTracks;
	info[2] = m_numTracks>>8;

	SendResponse(RSP_SYSTEM_INFO, info, sizeof(info));
}

void Wavetrigger::CmdTrackControl(){
	uint16_t trackNumber = m_rxData[1] + (m_rxData[2] << 8);
	uint8_t lockFlag = m_rxData[3];

	switch(m_rxData[0])
	{
	case TRK_PLAY_SOLO:
		break;
	case TRK_PLAY_POLY:
		break;
	case TRK_PAUSE:
		break;
	case TRK_RESUME:
		break;
	case TRK_STOP:
		break;
	case TRK_LOOP_ON:
		break;
	case TRK_LOOP_OFF:
		break;
	case TRK_LOAD:
		break;
	default:
		break;
	};
}

void Wavetrigger::CmdStopAll(){
	// for each track in list, disable play flag
}

void Wavetrigger::CmdTrackVolume(){

}

void Wavetrigger::CmdTrackFade(){

}

void Wavetrigger::CmdSamplerateOffset(){

}

void Wavetrigger::CmdSetReporting(){
	m_reportingEnabled = m_rxData[0];
}
