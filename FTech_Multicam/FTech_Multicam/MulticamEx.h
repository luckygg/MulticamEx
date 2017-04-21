//----------------------------------------------------------
// String Conversion Functions
//----------------------------------------------------------
// Programmed by William Kim
//----------------------------------------------------------
// Last Update : 2017-03-16 08:52
// Modified by William Kim
//----------------------------------------------------------

#pragma once

#include <multicam.h>
#pragma comment (lib,"multicam")

#define		EURESYS_BUFFER_COUNT 4

#define		MC_NO_BOARD_FOUND				-1	
#define		MC_BAD_PARAMETER				-2	
#define		MC_IO_ERROR						-3	
#define		MC_INTERNAL_ERROR				-4	
#define		MC_NO_MORE_RESOURCES			-5	
#define		MC_IN_USE						-6	
#define		MC_NOT_SUPPORTED				-7	
#define		MC_DATABASE_ERROR				-8	
#define		MC_OUT_OF_BOUND					-9	
#define		MC_INSTANCE_NOT_FOUND			-10
#define		MC_INVALID_HANDLE				-11
#define		MC_TIMEOUT						-12
#define		MC_INVALID_VALUE				-13
#define		MC_RANGE_ERROR					-14
#define		MC_BAD_HW_CONFIG				-15
#define		MC_NO_EVENT						-16
#define		MC_LICENSE_NOT_GRANTED			-17
#define		MC_FATAL_ERROR					-18
#define		MC_HW_EVENT_CONFLICT			-19
#define		MC_FILE_NOT_FOUND				-20
#define		MC_OVERFLOW						-21
#define		MC_INVALID_PARAMETER_SETTING	-22
#define		MC_PARAMETER_ILLEGAL_ACCESS		-23
#define		MC_CLUSTER_BUSY					-24
#define		MC_SERVICE_ERROR				-25
#define		MC_INVALID_SURFACE				-26

enum EErrorType{ENone, EMsgBox, EException, EMsgException};
enum EColorFormat{EY8, EBayer8, ERGB24};
enum ETriggerLine{EDIN1, EDIN2, EIIN1, EIIN2, EIIN3, EIIN4};

namespace Picolo
{
	enum EConnector{EVID1, EVID2, EVID3, EVID4, EVID5, EVID6, EVID7, EVID8, EVID9, EVID10, EVID11, EVID12, EVID13, EVID14, EVID15, EVID16, YC, YC1, YC2, YC3, YC4};
	enum ETopology{E1_01_2, E1_11_0, E1_2_0, EUniform};
	enum EOutputStyle{SSRLY, TTL};
}

namespace Domino
{
	enum EConnector{EX, EY, EXBIS, EYBIS, EA, EB, EC, ED};
	enum ETopology{E1, E1_1, E2_1, E2_2, E1_11, E2_11, E11_11, E1_1_1_1, E3_0};
	enum EOutputStyle{TTL, SSRLY, OPTO};
}

namespace Grablink
{
	enum EConnector{EM, EA, EB};
	enum ETopology{EMONO, EMONO_SLOW, EMONO_DECA, EDUO, EDUO_SLOW};
	enum EOutputStyle{TTL, ITTL, LVDS, OC, IOE, IOC, OPTO};
}


class CMulticamEx
{
public :
	//******************************************************************************************************************
	/// \brief					Board ���� Ȯ�� �Լ�.
	/// \param [out] nValue		Board �� Ȯ��.
	/// \param bool				��� ��ȯ.
	static bool GetNumberOfBoard(int &nValue);
	//******************************************************************************************************************
	/// \brief					Board Name Ȯ�� �Լ�.
	/// \param [in] nIdx		Board Index �Է�.
	/// \param [out] nValue		Board Name Ȯ��.
	/// \param bool				��� ��ȯ.
	static bool GetBoardName	(int nIdx, CString &strValue);
	//******************************************************************************************************************
	/// \brief					Board Type Ȯ�� �Լ�.
	/// \param [in] nIdx		Board Index �Է�.
	/// \param [out] nValue		Board Type Ȯ��.
	/// \param bool				��� ��ȯ.
	static bool GetBoardType	(int nIdx, CString &strValue);
	//******************************************************************************************************************
	/// \brief					Serial Number Ȯ�� �Լ�.
	/// \param [in] nIdx		Board Index �Է�.
	/// \param [out] nValue		Serial Number Ȯ��.
	/// \param bool				��� ��ȯ.
	static bool GetSerialNumber (int nIdx, CString &strValue);

public:
	CMulticamEx();
	~CMulticamEx();
	//******************************************************************************************************************
	/// \brief					Error Handling ���� �Լ�.
	/// \param [in] nCount		Error Type �Է�. (ENone, EMsgBox, EException, EMsgException)
	/// \param bool				��� ��ȯ.
	void SetErrorHandling(EErrorType eType);
	//******************************************************************************************************************
	/// \brief					Board �ʱ�ȭ �Լ�.
	/// \param [in] nIdx		Board Index �Է�.
	/// \param [in] eConnector	Connector Type �Է�.
	/// \param [in] eTopology	Topology Type �Է�.
	/// \param [in] strPath		Camfile ��� �Է�.
	/// \param bool				��� ��ȯ.
	bool OnInitPicoloBoard(int nIdx, Picolo::EConnector connector, Picolo::ETopology topology, CString camfilePath);
	bool OnInitDominoBoard(int nIdx, Domino::EConnector connector, Domino::ETopology topology, CString camfilePath);
	bool OnInitGrablinkBoard(int nIdx, Grablink::EConnector eConnector, Grablink::ETopology eTopology, CString strPath);
	//******************************************************************************************************************
	/// \brief					���� ��� ���� �Լ�.
	/// \param bool				��� ��ȯ.	 
	bool OnStartAcquisition();
	//******************************************************************************************************************
	/// \brief					���� ��� ���� �Լ�.
	/// \param bool				��� ��ȯ.	 
	bool OnStopAcquisition();
	//******************************************************************************************************************
	/// \brief					Software Trigger Event �߻� �Լ�.
	/// \param bool				��� ��ȯ.
	bool OnTriggerEvent();	
	//******************************************************************************************************************
	/// \brief					�̹��� ���� �Լ�.
	/// \param [in] strPath		���� ��� �Է�.
	/// \param bool				��� ��ȯ.
	bool OnSaveImage(CString strPath);
	//******************************************************************************************************************
	/// \brief					��� ��ȣ ���� �Լ�.
	/// \param [in] nChannel	��� ��ȣ ��ȣ �Է�.
	/// \param [in] eStyle		��� ��ȣ Style Type �Է�.
	/// \param bool				��� ��ȯ.
	bool SetPicoloOutputConfig(int nChannel, Picolo::EOutputStyle eStyle);
	bool SetDominoOutputConfig(int nChannel, Domino::EOutputStyle eStyle);
	bool SetGrablinkOutputConfig(int nChannel, Grablink::EOutputStyle eStyle);
	//******************************************************************************************************************
	/// \brief					��� ��ȣ ���� ���� �Լ�.
	/// \param [in] nChannel	��� ��ȣ ��ȣ �Է�.
	/// \param [in] bStatus		��� ��ȣ ���� ����. (true : High, false : Low)
	/// \param bool				��� ��ȯ.
	bool SetOutputStatus(int nChannel, bool bStatus);
	//******************************************************************************************************************
	/// \brief					���� �ػ� ȹ�� �Լ�.
	/// \param [out] nValue		���� �ػ� Ȯ��.
	/// \param bool				��� ��ȯ.
	bool GetWidth				(int &nValue);
	//******************************************************************************************************************
	/// \brief					���� �ػ� ȹ�� �Լ�.
	/// \param [out] nValue		���� �ػ� Ȯ��.
	/// \param bool				��� ��ȯ.
	bool GetHeight				(int &nValue);
	//******************************************************************************************************************
	/// \brief					�ȼ� ���� ȹ�� �Լ�.
	/// \param [out] nValue		�ȼ� ���� Ȯ��.
	/// \param bool				��� ��ȯ.
	bool GetBpp					(int &nValue);
	//******************************************************************************************************************
	/// \brief					Buffer Pitch ȹ�� �Լ�.
	/// \param [out] nValue		Buffer Pitch Ȯ��.
	/// \param bool				��� ��ȯ.
	bool GetBufferPitch			(int &nValue);
	//******************************************************************************************************************
	/// \brief					Sequence Length ȹ�� �Լ�. (Frame Count)
	/// \param [out] nValue		Sequence Length Ȯ��.
	/// \param bool				��� ��ȯ.
	bool GetSeqLength_Fr		(int &nValue);
	//******************************************************************************************************************
	/// \brief					Buffer Size ȹ�� �Լ�.
	/// \param [out] nValue		Buffer Size Ȯ��.
	/// \param bool				��� ��ȯ.
	bool GetBufferSize			(int &nValue);
	//******************************************************************************************************************
	/// \brief					Frame Rate ȹ�� �Լ�.
	/// \param [out] nValue		Frame Rate Ȯ��.
	/// \param bool				��� ��ȯ.
	bool GetFrameRate			(double &dValue);
	//******************************************************************************************************************
	/// \brief					Color Format ȹ�� �Լ�.
	/// \param [out] nValue		Color Format Ȯ��.
	/// \param bool				��� ��ȯ.
	bool GetColorFormat			(CString &strValue);
	//******************************************************************************************************************
	/// \brief					Trigger Mode ȹ�� �Լ�.
	/// \param [out] nValue		Trigger Mode Ȯ��.
	/// \param bool				��� ��ȯ.
	bool GetTrigMode			(CString &strValue);
	//******************************************************************************************************************
	/// \brief					Next Trigger Mode ȹ�� �Լ�.
	/// \param [out] nValue		Next Trigger Mode Ȯ��.
	/// \param bool				��� ��ȯ.
	bool GetNextTrigMode		(CString &strValue);
	//******************************************************************************************************************
	/// \brief					Frame Rate ���� �Լ�. (Immediate Trigger Mode �� �� ���.)
	/// \param [in] nValue		Frame Rate �Է�.
	/// \param bool				��� ��ȯ.
	bool SetFrameRate_mHz		(int nValue);
	//******************************************************************************************************************
	/// \brief					Sequence Length �Է� �Լ�.
	/// \param [in] nValue		Sequence Length �Է�.
	/// \param bool				��� ��ȯ.
	bool SetSeqLength_Fr		(int nValue);
	//******************************************************************************************************************
	/// \brief					Surface Count �Է� �Լ�.
	/// \param [in] nValue		Surface Count �Է�.
	/// \param bool				��� ��ȯ.
	bool SetSurfaceCount		(int nValue);
	//******************************************************************************************************************
	/// \brief					Exposure Time �Է� �Լ�. (���� : us)
	/// \param [in] nValue		Exposure Time �Է�.
	/// \param bool				��� ��ȯ.
	bool SetExpose_us			(int nValue);
	//******************************************************************************************************************
	/// \brief					ROI WindowX ũ�� �Է� �Լ�. (Grab Window �� enable�� �� ���.)
	/// \param [in] nValue		ROI WindowX ũ�� �Է�.
	/// \param bool				��� ��ȯ.
	bool SetWindowX_Px			(int nValue);
	//******************************************************************************************************************
	/// \brief					ROI WindowY ũ�� �Է� �Լ�. (Grab Window �� enable�� �� ���.)
	/// \param [in] nValue		ROI WindowY ũ�� �Է�.
	/// \param bool				��� ��ȯ.
	bool SetWindowY_Ln			(int nValue);
	//******************************************************************************************************************
	/// \brief					ROI Window ��� ���� �Լ�.
	/// \param [in] bEnable		��� ���� �Է�.
	/// \param bool				��� ��ȯ.
	bool SetGrabWindow			(bool bEnable);
	//******************************************************************************************************************
	/// \brief					Flip X ��� ���� �Լ�.
	/// \param [in] bEnable		��� ���� �Է�.
	/// \param bool				��� ��ȯ.
	bool SetFlipX				(bool bEnable);
	//******************************************************************************************************************
	/// \brief					Flip Y ��� ���� �Լ�.
	/// \param [in] bEnable		��� ���� �Է�.
	/// \param bool				��� ��ȯ.
	bool SetFlipY				(bool bEnable);
	//******************************************************************************************************************
	/// \brief					Hardware Trigger Line ���� �Լ�.
	/// \param [in] eLine		Hardware Trigger Line Type �Է�.
	/// \param bool				��� ��ȯ.
	bool SetTriggerLine			(ETriggerLine eLine);
	//******************************************************************************************************************
	/// \brief					Software Trigger Mode ���� �Լ�.
	/// \param bool				��� ��ȯ.
	bool SetSoftwareTriggerMode	();
	//******************************************************************************************************************
	/// \brief					Hardware Trigger Mode ���� �Լ�.
	/// \param bool				��� ��ȯ.
	bool SetHardwareTriggerMode	();
	//******************************************************************************************************************
	/// \brief					Immediate Trigger Mode ���� �Լ�.
	/// \param bool				��� ��ȯ.
	bool SetImmediateMode		();
	//******************************************************************************************************************
	/// \brief					Combined Trigger Mode ���� �Լ�.
	/// \param bool				��� ��ȯ.
	bool SetCombinedMode		();
	//******************************************************************************************************************
	/// \brief					�̹��� ȹ���� �Ϸ�ƴ��� Ȯ���ϴ� Handle ��ȯ �Լ�.
	/// \param bool				�ڵ� ��ȯ.
	HANDLE GetHandleGrabDone() { return m_hGrabDone; }
	//******************************************************************************************************************
	/// \brief					�̹��� ȹ���� �Ϸ�ƴ��� Ȯ���ϴ� Handle�� �ʱ�ȭ(Reset)��Ű�� �Լ�.
	void ResetHandleGrabDone() { ResetEvent(m_hGrabDone); }
	//******************************************************************************************************************
	/// \brief					�̹��� ������ ȹ�� �Լ�.
	/// \param bool				�̹��� ������ ������ ��ȯ.
	BYTE* GetImageBuffer() { return m_pbyBuffer; }
	//******************************************************************************************************************
	/// \brief					���� �ʱ�ȭ Ȯ�� �Լ�.
	/// \param bool				���� �ʱ�ȭ ���� ��ȯ.
	bool IsOpened() { return m_bOpened; }
	//******************************************************************************************************************
	/// \brief					������ ���� �޽��� Ȯ�� �Լ�.
	/// \param CString			������ ���� �޽��� ��ȯ.
	CString GetLastErrorMsg() { return m_strErrMsg; }

private:
	CString		m_strErrMsg;
	MCHANDLE	m_hChannel;
	PVOID		m_pBuffer;
	BYTE*		m_pbyBuffer;
	BITMAPINFO* m_pBmpInfo;
	int			m_nSize;
	bool		m_bOpened;
	HANDLE		m_hGrabDone;

	static void WINAPI	GlobalCallback(PMCSIGNALINFO SigInfo);
	void Callback(PMCSIGNALINFO SigInfo);
	bool DeleteChannel();
	bool OpenDriver();
	bool CloseDriver();
	void OnCreateBmpInfo(int nWidth, int nHeight, int nBpp);
	CString GetErrorMsg(MCSTATUS ErrCode);
private :
	//******************************************************************************************************************
	/// \brief					�Ķ���� ���� Type���� Get/Set �ϴ� �Լ�.
	/// \param [in] pNodeName	�Ķ������ Node Name �Է�.
	/// \param [in]  Value		�Ķ������ �� �Է�.
	/// \param [out] Value		�Ķ������ �� Ȯ��.
	/// \param bool				��� ��ȯ.
	bool GetValueString(char* pNodeName, CString &strValue);
	bool SetValueString(char* pNodeName, CString strValue);
	bool GetValueInt(char* pNodeName, int &nValue);
	bool SetValueInt(char* pNodeName, int nValue);
	bool GetValueDouble(char* pNodeName, double &dValue);
	bool SetValueDouble(char* pNodeName, double dValue);
	bool ExecuteCommand(char* pNodeName);
};