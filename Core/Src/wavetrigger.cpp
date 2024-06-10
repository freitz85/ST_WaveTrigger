#include "wavetrigger.h"


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
			m_parserResult = ERROR;
		}
		break;
	case WAIT_SOM2:
		if(data == SOM2)
		{
			m_parserState = GET_LENGTH;
		}
		else
		{
			m_parserResult = ERROR;
		}
		break;
	case GET_LENGTH:
		m_length = data - 4;
		m_parserState = GET_CMD;
		break;
	case GET_CMD:
		m_command = data;
		m_parserState = GET_DATA;
		break;
	case GET_DATA:
		m_data[m_dataIndex] = data;
		if(m_dataIndex == m_length + 1)
		{
			m_parserState = WAIT_EOM;
		}
		else
		{
			m_dataIndex++;
		}
		break;
	case WAIT_EOM:
		if(data == EOM)
		{
			m_parserResult = FINISHED;
		}
		else
		{
			m_parserResult = ERROR;
		}
		break;
	default:
		m_parserResult = ERROR;
	}


	return m_parserResult;
}

void Wavetrigger::DoCommand()
{
	switch(m_command)
	{
	case CMD_GET_VERSION:

		break;
	case CMD_GET_SYS_INFO:

		break;
	case CMD_TRACK_CONTROL:

		break;
	case CMD_STOP_ALL:

		break;
	case CMD_TRACK_VOLUME:

		break;
	case CMD_TRACK_FADE:

		break;
//#define CMD_RESUME_ALL_SYNC     0x0b
#define CMD_SAMPLERATE_OFFSET   0x0c
//#define CMD_TRACK_CONTROL_EX    0x0d
#define CMD_SET_REPORTING       0x0e
	void CmdGetVersion();
	void CmdGetSysInfo();
	void CmdTrackControl();
	void CmdStopAll();
	void CmdTrackVolume();
	void CmdTrackFade();
	void CmdSamplerateOffset();
	void CmdSetReporting();
	default:
		// do nothing
	}
}

void Wavetrigger::CmdGetVersion(){

}

void Wavetrigger::CmdGetSysInfo(){

}

void Wavetrigger::CmdTrackControl(){

}

void Wavetrigger::CmdStopAll(){

}

void Wavetrigger::CmdTrackVolume(){

}

void Wavetrigger::CmdTrackFade(){

}

void Wavetrigger::CmdSamplerateOffset(){

}

void Wavetrigger::CmdSetReporting(){

}
