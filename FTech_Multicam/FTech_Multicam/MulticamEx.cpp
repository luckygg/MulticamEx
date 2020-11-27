#include "stdafx.h"
#include "MulticamEx.h"
//#include "StringConvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



//Board Control Class
CMulticamEx::CMulticamEx()
{
	m_hChannel	= 0;
	m_nSize		= 0;
	m_pbyBuffer = NULL;
	m_bOpened   = false;
	m_strErrMsg = _T("");
	m_hGrabDone = NULL;
	m_pBmpInfo = NULL;
	
	OpenDriver();

	SetErrorHandling(EMsgBox);
}

CMulticamEx::~CMulticamEx()
{
	if (m_bOpened == true)	
	{
		OnStopAcquisition();
		DeleteChannel();
	}

	if (m_pbyBuffer != NULL)
	{
		delete []m_pbyBuffer;
		m_pbyBuffer = NULL;
	}

	if (m_pBmpInfo != NULL)
	{
		delete []m_pBmpInfo;
		m_pBmpInfo = NULL;
	}

	CloseDriver();
}

bool CMulticamEx::GetNumberOfBoard(int &nValue)
{
	int value=0;
	MCSTATUS result;
	result = McGetParamNmInt(MC_CONFIGURATION, "BoardCount", &value);
	if (result != MC_OK) { nValue = -1; return false;}

	nValue = value;
	
	return true;
}

bool CMulticamEx::GetBoardName(int nIdx, CString &strValue)
{
	char value[MAX_PATH] = {0,};
	MCSTATUS result;
	result = McGetParamStr(MC_BOARD+nIdx, MC_BoardName, value, MAX_PATH);;
	if (result != MC_OK) { strValue = _T(""); return false;}

	strValue = (CString)value;
	return true;
}

bool CMulticamEx::GetBoardType(int nIdx, CString &strValue)
{
	char value[MAX_PATH] = {0,};

	MCSTATUS result;
	result = McGetParamStr(MC_BOARD+nIdx, MC_BoardType, value, MAX_PATH);
	if (result != MC_OK) { strValue = _T(""); return false;}

	strValue = (CString)value;
	return true;
}

bool CMulticamEx::GetSerialNumber(int nIdx, CString &strValue)
{
	char value[MAX_PATH] = {0,};

	MCSTATUS result;
	result = McGetParamStr(MC_BOARD+nIdx, MC_SerialNumber, value, MAX_PATH);;
	if (result != MC_OK) { strValue = _T(""); return false;}

	strValue = (CString)value;
	return true;
}

void CMulticamEx::SetErrorHandling(EErrorType eType)
{
	switch(eType)
	{
	case ENone :
		McSetParamInt (MC_CONFIGURATION, MC_ErrorHandling, MC_ErrorHandling_NONE);
		break;
	case EMsgBox :
		McSetParamInt (MC_CONFIGURATION, MC_ErrorHandling, MC_ErrorHandling_MSGBOX);
		break;
	case EException :
		McSetParamInt (MC_CONFIGURATION, MC_ErrorHandling, MC_ErrorHandling_EXCEPTION);
		break;
	case EMsgException :
		McSetParamInt (MC_CONFIGURATION, MC_ErrorHandling, MC_ErrorHandling_MSGEXCEPTION);
		break;
	}
	McSetParamStr (MC_CONFIGURATION, MC_ErrorLog, "error.log");
}

bool CMulticamEx::OpenDriver()
{
	MCSTATUS result;
	result = McOpenDriver(NULL);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	return true;
}

bool CMulticamEx::CloseDriver()
{
	MCSTATUS result;
	result = McCloseDriver();
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	return true;
}

bool CMulticamEx::DeleteChannel()
{
	MCSTATUS result;
	result = McDelete(m_hChannel);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	return true;
}

bool CMulticamEx::SetPicoloOutputConfig(int nChannel, Picolo::EOutputStyle eStyle)
{
	MCSTATUS result;
	result = McSetParamInt(MC_BOARD, MC_OutputConfig + nChannel, MC_OutputConfig_SOFT);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	switch (eStyle)
	{
	case Picolo::SSRLY : result = McSetParamInt(MC_BOARD, MC_OutputStyle + nChannel, MC_OutputStyle_SSRLY);	break;
	case Picolo::TTL   : result = McSetParamInt(MC_BOARD, MC_OutputStyle + nChannel, MC_OutputStyle_TTL	 );	break;
	}
	
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	return true;
}

bool CMulticamEx::SetDominoOutputConfig(int nChannel, Domino::EOutputStyle eStyle)
{
	MCSTATUS result;
	result = McSetParamInt(MC_BOARD, MC_OutputConfig + nChannel, MC_OutputConfig_SOFT);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	switch (eStyle)
	{
	case Domino::TTL   : result = McSetParamInt(MC_BOARD, MC_OutputStyle + nChannel, MC_OutputStyle_TTL	 );	break;
	case Domino::SSRLY : result = McSetParamInt(MC_BOARD, MC_OutputStyle + nChannel, MC_OutputStyle_SSRLY);	break;
	case Domino::OPTO  : result = McSetParamInt(MC_BOARD, MC_OutputStyle + nChannel, MC_OutputStyle_OPTO );	break;
	}

	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	return true;
}

bool CMulticamEx::SetGrablinkOutputConfig(int nChannel, Grablink::EOutputStyle eStyle)
{
	MCSTATUS result;
	result = McSetParamInt(MC_BOARD, MC_OutputConfig + nChannel, MC_OutputConfig_SOFT);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	switch (eStyle)
	{
	case Grablink::TTL  : result = McSetParamInt(MC_BOARD, MC_OutputStyle + nChannel, MC_OutputStyle_TTL );	break;
	case Grablink::ITTL : result = McSetParamInt(MC_BOARD, MC_OutputStyle + nChannel, MC_OutputStyle_ITTL);	break;
	case Grablink::LVDS : result = McSetParamInt(MC_BOARD, MC_OutputStyle + nChannel, MC_OutputStyle_LVDS);	break;
	case Grablink::OC	: result = McSetParamInt(MC_BOARD, MC_OutputStyle + nChannel, MC_OutputStyle_OC	 );	break;
	case Grablink::IOE	: result = McSetParamInt(MC_BOARD, MC_OutputStyle + nChannel, MC_OutputStyle_IOE );	break;
	case Grablink::IOC	: result = McSetParamInt(MC_BOARD, MC_OutputStyle + nChannel, MC_OutputStyle_IOC );	break;
	case Grablink::OPTO	: result = McSetParamInt(MC_BOARD, MC_OutputStyle + nChannel, MC_OutputStyle_OPTO);	break;
	}

	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	return true;
}

bool CMulticamEx::SetOutputStatus(int nChannel, bool bStatus)
{
	if (bStatus == true)
	{
		MCSTATUS result = McSetParamInt(MC_BOARD, MC_OutputState + nChannel, MC_OutputState_HIGH);
		if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }
	}
	else
	{
		MCSTATUS result = McSetParamInt(MC_BOARD, MC_OutputState + nChannel, MC_OutputState_LOW);
		if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }
	}

	return true;
}

bool CMulticamEx::OnInitPicoloBoard(int nIdx, Picolo::EConnector connector, Picolo::ETopology topology, CString strPath)
{
	MCSTATUS result;

	switch (topology)
	{
		case Picolo::E1_01_2	: result = McSetParamInt(MC_BOARD + nIdx, MC_BoardTopology, MC_BoardTopology_1_01_2	); break;
		case Picolo::E1_11_0	: result = McSetParamInt(MC_BOARD + nIdx, MC_BoardTopology, MC_BoardTopology_1_11_0	); break;
		case Picolo::E1_2_0		: result = McSetParamInt(MC_BOARD + nIdx, MC_BoardTopology, MC_BoardTopology_1_2_0	); break;
		case Picolo::EUniform	: result = McSetParamInt(MC_BOARD + nIdx, MC_BoardTopology, MC_BoardTopology_UNIFORM); break;
	}
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	result = McCreate(MC_CHANNEL, &m_hChannel);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	result = McSetParamInt(m_hChannel, MC_DriverIndex, nIdx);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	//VID1~VID16 및 YC1~YC4는 McParams.h 에서 순서대로 정의되어 있다.
	if (connector >= Picolo::EVID1 && connector <= Picolo::EVID16)
	{
		int index = connector - Picolo::EVID1;
		result = result = McSetParamInt(m_hChannel, MC_Connector, MC_Connector_VID1+index);
	}
	else if (connector >= Picolo::YC1 && connector <= Picolo::YC4)
	{
		int index = connector - Picolo::YC1;
		result = result = McSetParamInt(m_hChannel, MC_Connector, MC_Connector_YC1+index);
	}
	else
	{
		result = result = McSetParamInt(m_hChannel, MC_Connector, MC_Connector_YC);
	}
	
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }
	
	char path[MAX_PATH] = {0,};
	
	size_t szCvt = 0;
	wcstombs_s(&szCvt, path, strPath.GetLength()+1, strPath, _TRUNCATE);

	result = McSetParamStr(m_hChannel, MC_CamFile, path);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	bool ret=false;
	int w=0,h=0,bpp=0;
	ret = GetWidth(w);
	if (ret == false) return false;

	ret = GetHeight(h);
	if (ret == false) return false;

	ret = GetBpp(bpp);
	if (ret == false) return false;

	if (m_pbyBuffer != NULL)
	{
		delete []m_pbyBuffer;
		m_pbyBuffer = NULL;
	}

	m_nSize = w*h*bpp/8;

	m_pbyBuffer = new BYTE[m_nSize];
	ZeroMemory(m_pbyBuffer, m_nSize);

	OnCreateBmpInfo(w,h,bpp);
	
	result = McSetParamInt(m_hChannel, MC_SurfaceCount, EURESYS_BUFFER_COUNT);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }
	
	result = McSetParamInt(m_hChannel, MC_SignalEnable + MC_SIG_START_ACQUISITION_SEQUENCE, MC_SignalEnable_ON);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }
	result = McSetParamInt(m_hChannel, MC_SignalEnable + MC_SIG_SURFACE_PROCESSING, MC_SignalEnable_ON);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }
	result = McSetParamInt(m_hChannel, MC_SignalEnable + MC_SIG_END_CHANNEL_ACTIVITY, MC_SignalEnable_ON);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }
	result = McSetParamInt(m_hChannel, MC_SignalEnable + MC_SIG_ACQUISITION_FAILURE, MC_SignalEnable_ON);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }
	
	result = McRegisterCallback(m_hChannel, GlobalCallback, this);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }
	
	m_hGrabDone = CreateEvent(NULL,TRUE,FALSE,NULL);
	ResetEvent(m_hGrabDone);
	
	m_bOpened = true;
	
	return true;	
}

bool CMulticamEx::OnInitDominoBoard(int nIdx, Domino::EConnector connector, Domino::ETopology topology, CString strPath)
{
	MCSTATUS result;

	switch (topology)
	{
	case Domino::E1			: result = McSetParamInt(MC_BOARD + nIdx, MC_BoardTopology, MC_BoardTopology_1		); break;
	case Domino::E1_1		: result = McSetParamInt(MC_BOARD + nIdx, MC_BoardTopology, MC_BoardTopology_1_1	); break;
	case Domino::E2_1		: result = McSetParamInt(MC_BOARD + nIdx, MC_BoardTopology, MC_BoardTopology_2_1	); break;
	case Domino::E2_2		: result = McSetParamInt(MC_BOARD + nIdx, MC_BoardTopology, MC_BoardTopology_2_2	); break;
	case Domino::E1_11		: result = McSetParamInt(MC_BOARD + nIdx, MC_BoardTopology, MC_BoardTopology_1_11	); break;
	case Domino::E2_11		: result = McSetParamInt(MC_BOARD + nIdx, MC_BoardTopology, MC_BoardTopology_2_11	); break;
	case Domino::E11_11		: result = McSetParamInt(MC_BOARD + nIdx, MC_BoardTopology, MC_BoardTopology_11_11	); break;
	case Domino::E1_1_1_1	: result = McSetParamInt(MC_BOARD + nIdx, MC_BoardTopology, MC_BoardTopology_1_1_1_1); break;
	case Domino::E3_0		: result = McSetParamInt(MC_BOARD + nIdx, MC_BoardTopology, MC_BoardTopology_3_0	); break;
	}
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	result = McCreate(MC_CHANNEL, &m_hChannel);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	result = McSetParamInt(m_hChannel, MC_DriverIndex, nIdx);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	switch (connector)
	{
	case Domino::EX		: result = McSetParamInt(m_hChannel, MC_Connector, MC_Connector_X	 ); break;
	case Domino::EY		: result = McSetParamInt(m_hChannel, MC_Connector, MC_Connector_Y	 ); break;
	case Domino::EXBIS	: result = McSetParamInt(m_hChannel, MC_Connector, MC_Connector_XBIS); break;
	case Domino::EYBIS	: result = McSetParamInt(m_hChannel, MC_Connector, MC_Connector_YBIS); break;
	case Domino::EA		: result = McSetParamInt(m_hChannel, MC_Connector, MC_Connector_A	 ); break;
	case Domino::EB		: result = McSetParamInt(m_hChannel, MC_Connector, MC_Connector_B	 ); break;
	case Domino::EC		: result = McSetParamInt(m_hChannel, MC_Connector, MC_Connector_C	 ); break;
	case Domino::ED		: result = McSetParamInt(m_hChannel, MC_Connector, MC_Connector_D	 ); break;
	}
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	char path[MAX_PATH] = {0,};
	
	size_t szCvt = 0;
	wcstombs_s(&szCvt, path, strPath.GetLength()+1, strPath, _TRUNCATE);

	result = McSetParamStr(m_hChannel, MC_CamFile, path);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	bool ret=false;
	int w=0,h=0,bpp=0;
	ret = GetWidth(w);
	if (ret == false) return false;

	ret = GetHeight(h);
	if (ret == false) return false;

	ret = GetBpp(bpp);
	if (ret == false) return false;

	if (m_pbyBuffer != NULL)
	{
		delete []m_pbyBuffer;
		m_pbyBuffer = NULL;
	}

	m_nSize = w*h*bpp/8;

	m_pbyBuffer = new BYTE[m_nSize];
	ZeroMemory(m_pbyBuffer, m_nSize);

	OnCreateBmpInfo(w,h,bpp);

	result = McSetParamInt(m_hChannel, MC_SurfaceCount, EURESYS_BUFFER_COUNT);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	result = McSetParamInt(m_hChannel, MC_SignalEnable + MC_SIG_START_ACQUISITION_SEQUENCE, MC_SignalEnable_ON);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }
	result = McSetParamInt(m_hChannel, MC_SignalEnable + MC_SIG_SURFACE_PROCESSING, MC_SignalEnable_ON);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }
	result = McSetParamInt(m_hChannel, MC_SignalEnable + MC_SIG_END_CHANNEL_ACTIVITY, MC_SignalEnable_ON);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }
	result = McSetParamInt(m_hChannel, MC_SignalEnable + MC_SIG_ACQUISITION_FAILURE, MC_SignalEnable_ON);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	result = McRegisterCallback(m_hChannel, GlobalCallback, this);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	m_hGrabDone = CreateEvent(NULL,TRUE,FALSE,NULL);
	ResetEvent(m_hGrabDone);

	m_bOpened = true;

	return true;	
}

bool CMulticamEx::OnInitGrablinkBoard(int nIdx, Grablink::EConnector eConnector, Grablink::ETopology eTopology, CString strPath)
{
	MCSTATUS result;
	
	switch (eTopology)
	{
		case Grablink::EMONO		: result = McSetParamInt(MC_BOARD + nIdx, MC_BoardTopology, MC_BoardTopology_MONO		); break;
		case Grablink::EMONO_SLOW	: result = McSetParamInt(MC_BOARD + nIdx, MC_BoardTopology, MC_BoardTopology_MONO_SLOW	); break;
		case Grablink::EMONO_DECA	: result = McSetParamInt(MC_BOARD + nIdx, MC_BoardTopology, MC_BoardTopology_MONO_DECA	); break;
		case Grablink::EDUO			: result = McSetParamInt(MC_BOARD + nIdx, MC_BoardTopology, MC_BoardTopology_DUO		); break;
		case Grablink::EDUO_SLOW	: result = McSetParamInt(MC_BOARD + nIdx, MC_BoardTopology, MC_BoardTopology_DUO_SLOW	); break;
	}
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	result = McCreate(MC_CHANNEL, &m_hChannel);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	result = McSetParamInt(m_hChannel, MC_DriverIndex, nIdx);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	switch (eConnector)
	{
		case Grablink::EM: result = McSetParamInt(m_hChannel, MC_Connector, MC_Connector_M); break;
		case Grablink::EA: result = McSetParamInt(m_hChannel, MC_Connector, MC_Connector_A); break;
		case Grablink::EB: result = McSetParamInt(m_hChannel, MC_Connector, MC_Connector_B); break;
	}
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	bool ret=false;

	ret = SetValueString("CamFile",strPath);
	if (ret == false) return false;
	
	int w=0,h=0,bpp=0;
	ret = GetWidth(w);
	if (ret == false) return false;

	ret = GetHeight(h);
	if (ret == false) return false;

	ret = GetBpp(bpp);
	if (ret == false) return false;

	if (m_pbyBuffer != NULL)
	{
		delete []m_pbyBuffer;
		m_pbyBuffer = NULL;
	}

	m_nSize = w*h*bpp/8;

	m_pbyBuffer = new BYTE[m_nSize];
	ZeroMemory(m_pbyBuffer, m_nSize);

	OnCreateBmpInfo(w,h,bpp);

	result = McSetParamInt(m_hChannel, MC_SurfaceCount, EURESYS_BUFFER_COUNT);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	result = McSetParamInt(m_hChannel, MC_SignalEnable + MC_SIG_START_ACQUISITION_SEQUENCE, MC_SignalEnable_ON);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }
	result = McSetParamInt(m_hChannel, MC_SignalEnable + MC_SIG_SURFACE_PROCESSING, MC_SignalEnable_ON);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }
	result = McSetParamInt(m_hChannel, MC_SignalEnable + MC_SIG_END_CHANNEL_ACTIVITY, MC_SignalEnable_ON);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }
	result = McSetParamInt(m_hChannel, MC_SignalEnable + MC_SIG_ACQUISITION_FAILURE, MC_SignalEnable_ON);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	result = McRegisterCallback(m_hChannel, GlobalCallback, this);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	m_hGrabDone = CreateEvent(NULL,TRUE,FALSE,NULL);
	ResetEvent(m_hGrabDone);

	m_bOpened = true;

	return true;	
}

void WINAPI CMulticamEx::GlobalCallback(PMCSIGNALINFO SigInfo)
{
	if (SigInfo && SigInfo->Context)
	{
		CMulticamEx* pApp = (CMulticamEx*) SigInfo->Context;
		pApp->Callback (SigInfo);
	}
}


void CMulticamEx::Callback(PMCSIGNALINFO SigInfo)
{
	switch(SigInfo->Signal)
	{
	case MC_SIG_SURFACE_PROCESSING:
		McGetParamPtr(SigInfo->SignalInfo, MC_SurfaceAddr, &m_pBuffer); 
		memcpy(m_pbyBuffer, m_pBuffer, m_nSize);
		SetEvent(m_hGrabDone);
		
		break;

	case MC_SIG_END_CHANNEL_ACTIVITY :
		ResetEvent(m_hGrabDone);
		break;

	case MC_SIG_ACQUISITION_FAILURE:
		ResetEvent(m_hGrabDone);
		break;
	case MC_SIG_START_ACQUISITION_SEQUENCE :
		ResetEvent(m_hGrabDone);
		
		break;
	default:
		break;
	}
}

bool CMulticamEx::OnStartAcquisition()
{
	return SetValueString("ChannelState", _T("ACTIVE"));
}

bool CMulticamEx::OnStopAcquisition()
{
	return SetValueString("ChannelState", _T("IDLE"));
}

bool CMulticamEx::OnTriggerEvent()
{
	return SetValueString("ForceTrig", _T("TRIG"));
}

bool CMulticamEx::GetWidth(int &nValue)
{
	int value=0;
	bool ret=false;
	ret = GetValueInt("ImageSizeX", value);
	if (ret == false) return false;

	nValue = value;
	return true;
}

bool CMulticamEx::GetHeight(int &nValue)
{
	int value=0;
	bool ret=false;
	ret = GetValueInt("ImageSizeY", value);
	if (ret == false) return false;

	nValue = value;
	return true;
}

bool CMulticamEx::GetBufferPitch(int &nValue)
{
	int value=0;
	bool ret=false;
	ret = GetValueInt("BufferPitch", value);
	if (ret == false) return false;

	nValue = value;
	return true;
}

bool CMulticamEx::GetFrameRate(double &dValue)
{
	double value=0;
	bool ret=false;
	ret = GetValueDouble("PerSecond_Fr", value);
	if (ret == false) return false;

	dValue = value;
	return true;
}

bool CMulticamEx::GetBufferSize(int &nValue)
{
	int value=0;
	bool ret=false;
	ret = GetValueInt("BufferSize", value);
	if (ret == false) return false;

	nValue = value;
	return true;
}

bool CMulticamEx::GetBpp(int &nValue)
{
	CString format=_T("");
	bool ret = GetColorFormat(format);

	if (ret == false) return false;

	if (format == _T("Y8"))
	{
		nValue = 8;
	}
	else if (format == _T("RGB24"))
	{
		nValue = 24;
	}
	else if (format == _T("BAYER8"))
	{
		nValue = 8;
	}
	else
		nValue = 0;

	return true;
}

bool CMulticamEx::GetColorFormat(CString &strValue)
{
	CString value=_T("");
	bool ret=false;
	ret = GetValueString("ColorFormat", value);
	if (ret == false) return false;

	strValue = value;
	return true;
}

bool CMulticamEx::GetTrigMode(CString &strValue)
{
	CString value=_T("");
	bool ret=false;
	ret = GetValueString("TrigMode", value);
	if (ret == false) return false;

	strValue = value;
	return true;
}

bool CMulticamEx::GetNextTrigMode(CString &strValue)
{
	CString value=_T("");
	bool ret=false;
	ret = GetValueString("NextTrigMode", value);
	if (ret == false) return false;

	strValue = value;
	return true;
}

bool CMulticamEx::GetSeqLength_Fr(int &nValue)
{
	int value=0;
	bool ret=false;
	ret = GetValueInt("SeqLength_Fr", value);
	if (ret == false) return false;

	nValue = value;
	return true;
}

bool CMulticamEx::SetWindowX_Px(int nValue)
{
	return SetValueInt("WindowX_Px", nValue);
}

bool CMulticamEx::SetFrameRate_mHz(int nValue)
{
	return SetValueInt("FrameRate_mHz", nValue);
}

bool CMulticamEx::SetWindowY_Ln(int nValue)
{
	return SetValueInt("WindowY_Ln", nValue);
}

bool CMulticamEx::SetSeqLength_Fr(int nValue)
{
	return SetValueInt("SeqLength_Fr", nValue);
}

bool CMulticamEx::SetSoftwareTriggerMode()
{
	bool ret=false;
	ret = SetValueString("TrigMode", _T("SOFT"));
	if (ret == false) return false;
	ret = SetValueString("NextTrigMode", _T("SOFT"));
	if (ret == false) return false;

	return true;
}

bool CMulticamEx::SetHardwareTriggerMode()
{
	bool ret=false;
	ret = SetValueString("TrigMode", _T("HARD"));
	if (ret == false) return false;
	ret = SetValueString("NextTrigMode", _T("HARD"));
	if (ret == false) return false;

	return true;
}

bool CMulticamEx::SetImmediateMode()
{
	bool ret=false;
	ret = SetValueString("TrigMode", _T("IMMEDIATE"));
	if (ret == false) return false;
	ret = SetValueString("NextTrigMode", _T("IMMEDIATE"));
	if (ret == false) return false;

	return true;
}

bool CMulticamEx::SetTriggerLine(ETriggerLine eLine)
{
	switch (eLine)
	{
		case EDIN1 : return SetValueString("TrigLine", _T("DIN1"));
		case EDIN2 : return SetValueString("TrigLine", _T("DIN2"));
		case EIIN1 : return SetValueString("TrigLine", _T("IIN1"));
		case EIIN2 : return SetValueString("TrigLine", _T("IIN2"));
		case EIIN3 : return SetValueString("TrigLine", _T("IIN3"));
		case EIIN4 : return SetValueString("TrigLine", _T("IIN4"));
		default: return false;
	}
}

bool CMulticamEx::SetCombinedMode()
{
	bool ret=false;
	ret = SetValueString("TrigMode", _T("COMBINED"));
	if (ret == false) return false;
	ret = SetValueString("NextTrigMode", _T("COMBINED"));
	if (ret == false) return false;

	return true;
}

bool CMulticamEx::SetExpose_us(int nValue)
{
	return SetValueInt("Expose_us", nValue);
}

bool CMulticamEx::SetSurfaceCount(int nValue)
{
	return SetValueInt("SurfaceCount", nValue);
}

bool CMulticamEx::SetGrabWindow(bool bEnable)
{
	if (bEnable == true)
		return SetValueString("GrabWindow", _T("MAN"));
	else
		return SetValueString("GrabWindow", _T("NOBLACK"));
}

bool CMulticamEx::SetFlipX(bool bEnable)
{
	if (bEnable == true)
		return SetValueString("ImageFlipX", _T("ON"));
	else
		return SetValueString("ImageFlipX", _T("OFF"));
}

bool CMulticamEx::SetFlipY(bool bEnable)
{
	if (bEnable == true)
		return SetValueString("ImageFlipY", _T("ON"));
	else
		return SetValueString("ImageFlipY", _T("OFF"));
}

void CMulticamEx::OnCreateBmpInfo(int nWidth, int nHeight, int nBpp)
{
	if (m_pBmpInfo != NULL)
	{
		delete []m_pBmpInfo;
		m_pBmpInfo = NULL;
	}

	if (nBpp == 8)
		m_pBmpInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO) + 255*sizeof(RGBQUAD)];
	else if (nBpp == 24)
		m_pBmpInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO)];

	m_pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBmpInfo->bmiHeader.biPlanes = 1;
	m_pBmpInfo->bmiHeader.biBitCount = nBpp;
	m_pBmpInfo->bmiHeader.biCompression = BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage = 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biClrUsed = 0;
	m_pBmpInfo->bmiHeader.biClrImportant = 0;

	if (nBpp == 8)
	{
		for (int i = 0 ; i < 256 ; i++)
		{
			m_pBmpInfo->bmiColors[i].rgbBlue = (BYTE)i;
			m_pBmpInfo->bmiColors[i].rgbGreen = (BYTE)i;
			m_pBmpInfo->bmiColors[i].rgbRed = (BYTE)i;
			m_pBmpInfo->bmiColors[i].rgbReserved = 0;
		}
	}

	m_pBmpInfo->bmiHeader.biWidth = nWidth;
	m_pBmpInfo->bmiHeader.biHeight = -nHeight;
}

bool CMulticamEx::OnSaveImage(CString strPath)
{
	if (m_bOpened == false) return false;
	if (strPath.IsEmpty() == true) return false;

	// DIB 파일의 내용을 구성한다.
	BITMAPFILEHEADER dib_format_layout;
	ZeroMemory(&dib_format_layout, sizeof(BITMAPFILEHEADER));
	dib_format_layout.bfType = 0x4D42;//*(WORD*)"BM";
	
	dib_format_layout.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(m_pBmpInfo->bmiColors) * 256;
	
	dib_format_layout.bfSize = dib_format_layout.bfOffBits + m_pBmpInfo->bmiHeader.biSizeImage;

	// DIB 파일을 생성한다.
	char path[MAX_PATH] = {0,};
	size_t szCvt = 0;
	wcstombs_s(&szCvt, path, strPath.GetLength()+1, strPath, _TRUNCATE);

	FILE *p_file;
	fopen_s(&p_file,path, "wb");

	int w=0, h=0, bpp=m_pBmpInfo->bmiHeader.biBitCount;

	GetWidth(w);
	GetHeight(h);
	BYTE* pBuffer = GetImageBuffer();
	if (pBuffer == NULL) return false;

	if(p_file != NULL)
	{
		fwrite(&dib_format_layout, 1, sizeof(BITMAPFILEHEADER), p_file);
		fwrite(m_pBmpInfo, 1, sizeof(BITMAPINFOHEADER), p_file);
		fwrite(m_pBmpInfo->bmiColors, sizeof(m_pBmpInfo->bmiColors), 256, p_file);
		fwrite(pBuffer, 1, w*h*bpp/8, p_file);
		fclose(p_file);
	}

	return true;
}

CString CMulticamEx::GetErrorMsg(MCSTATUS ErrCode)
{
	CString strErrMsg=_T("");
	switch(ErrCode)
	{
	case MC_NO_BOARD_FOUND				:	strErrMsg = L"No Board Found"				 ; break; 
	case MC_BAD_PARAMETER				:	strErrMsg = L"Bad Parameter"				 ; break; 
	case MC_IO_ERROR					:	strErrMsg = L"I/O Error"					 ; break; 
	case MC_INTERNAL_ERROR				:	strErrMsg = L"Internal Error"				 ; break; 
	case MC_NO_MORE_RESOURCES			:	strErrMsg = L"No More Resources"			 ; break; 
	case MC_IN_USE						:	strErrMsg = L"Object still in use"			 ; break; 
	case MC_NOT_SUPPORTED				:	strErrMsg = L"Operation not supported"		 ; break; 
	case MC_DATABASE_ERROR				:	strErrMsg = L"Parameter database error"		 ; break; 
	case MC_OUT_OF_BOUND				:	strErrMsg = L"Value out of bound"			 ; break; 
	case MC_INSTANCE_NOT_FOUND			:	strErrMsg = L"Object instance not found"	 ; break; 
	case MC_INVALID_HANDLE				:	strErrMsg = L"Invalid Handle"				 ; break; 
	case MC_TIMEOUT						:	strErrMsg = L"Timeout"						 ; break; 
	case MC_INVALID_VALUE				:	strErrMsg = L"Invalid Value"				 ; break; 
	case MC_RANGE_ERROR					:	strErrMsg = L"Value not in range"			 ; break; 
	case MC_BAD_HW_CONFIG				:	strErrMsg = L"Invalid hardware configuration"; break; 
	case MC_NO_EVENT					:	strErrMsg = L"No	Event"					 ; break; 
	case MC_LICENSE_NOT_GRANTED			:	strErrMsg = L"License not granted"			 ; break; 
	case MC_FATAL_ERROR					:	strErrMsg = L"Fatal error"					 ; break; 
	case MC_HW_EVENT_CONFLICT			:	strErrMsg = L"Hardware event conflict"		 ; break; 
	case MC_FILE_NOT_FOUND				:	strErrMsg = L"File not found"				 ; break; 
	case MC_OVERFLOW					:	strErrMsg = L"Overflow"						 ; break; 
	case MC_INVALID_PARAMETER_SETTING	:	strErrMsg = L"Parameter inconsistency"		 ; break; 
	case MC_PARAMETER_ILLEGAL_ACCESS	:	strErrMsg = L"Illegal operation"			 ; break; 
	case MC_CLUSTER_BUSY				:	strErrMsg = L"Cluster busy"					 ; break; 
	case MC_SERVICE_ERROR				:	strErrMsg = L"MultiCam service error"		 ; break; 
	case MC_INVALID_SURFACE				:	strErrMsg = L"Invalid surface"				 ; break; 
	}

	return strErrMsg;
}

bool CMulticamEx::GetValueString(char* pNodeName, CString &strValue)
{
	char value[MAX_PATH] = {0,};

	MCSTATUS result;
	result = McGetParamNmStr(m_hChannel,pNodeName,value,MAX_PATH);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	strValue = (CString)value;

	return true;
}

bool CMulticamEx::SetValueString(char* pNodeName, CString strValue)
{
	char value[MAX_PATH] = {0,};

	size_t szCvt = 0;
	wcstombs_s(&szCvt, value, strValue.GetLength()+1, strValue, _TRUNCATE);

	MCSTATUS result;
	result = McSetParamNmStr(m_hChannel,pNodeName,value);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	return true;
}

bool CMulticamEx::GetValueInt(char* pNodeName, int &nValue)
{
	int value=0;

	MCSTATUS result;
	result = McGetParamNmInt(m_hChannel,pNodeName,&value);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	nValue = value;

	return true;
}

bool CMulticamEx::SetValueInt(char* pNodeName, int nValue)
{
	MCSTATUS result;
	result = McSetParamNmInt(m_hChannel,pNodeName,nValue);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	return true;
}

bool CMulticamEx::GetValueDouble(char* pNodeName, double &dValue)
{
	double value=0;

	MCSTATUS result;
	result = McGetParamNmFloat(m_hChannel,pNodeName,&value);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	dValue = value;

	return true;
}

bool CMulticamEx::SetValueDouble(char* pNodeName, double dValue)
{
	MCSTATUS result;
	result = McSetParamNmFloat(m_hChannel,pNodeName,dValue);
	if(result != MC_OK) { m_strErrMsg = GetErrorMsg(result); return false; }

	return true;
}
