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
	/// \brief					Board 개수 확인 함수.
	/// \param [out] nValue		Board 수 확인.
	/// \param bool				결과 반환.
	static bool GetNumberOfBoard(int &nValue);
	//******************************************************************************************************************
	/// \brief					Board Name 확인 함수.
	/// \param [in] nIdx		Board Index 입력.
	/// \param [out] nValue		Board Name 확인.
	/// \param bool				결과 반환.
	static bool GetBoardName	(int nIdx, CString &strValue);
	//******************************************************************************************************************
	/// \brief					Board Type 확인 함수.
	/// \param [in] nIdx		Board Index 입력.
	/// \param [out] nValue		Board Type 확인.
	/// \param bool				결과 반환.
	static bool GetBoardType	(int nIdx, CString &strValue);
	//******************************************************************************************************************
	/// \brief					Serial Number 확인 함수.
	/// \param [in] nIdx		Board Index 입력.
	/// \param [out] nValue		Serial Number 확인.
	/// \param bool				결과 반환.
	static bool GetSerialNumber (int nIdx, CString &strValue);

public:
	CMulticamEx();
	~CMulticamEx();
	//******************************************************************************************************************
	/// \brief					Error Handling 설정 함수.
	/// \param [in] nCount		Error Type 입력. (ENone, EMsgBox, EException, EMsgException)
	/// \param bool				결과 반환.
	void SetErrorHandling(EErrorType eType);
	//******************************************************************************************************************
	/// \brief					Board 초기화 함수.
	/// \param [in] nIdx		Board Index 입력.
	/// \param [in] eConnector	Connector Type 입력.
	/// \param [in] eTopology	Topology Type 입력.
	/// \param [in] strPath		Camfile 경로 입력.
	/// \param bool				결과 반환.
	bool OnInitPicoloBoard(int nIdx, Picolo::EConnector connector, Picolo::ETopology topology, CString camfilePath);
	bool OnInitDominoBoard(int nIdx, Domino::EConnector connector, Domino::ETopology topology, CString camfilePath);
	bool OnInitGrablinkBoard(int nIdx, Grablink::EConnector eConnector, Grablink::ETopology eTopology, CString strPath);
	//******************************************************************************************************************
	/// \brief					영상 취득 시작 함수.
	/// \param bool				결과 반환.	 
	bool OnStartAcquisition();
	//******************************************************************************************************************
	/// \brief					영상 취득 종료 함수.
	/// \param bool				결과 반환.	 
	bool OnStopAcquisition();
	//******************************************************************************************************************
	/// \brief					Software Trigger Event 발생 함수.
	/// \param bool				결과 반환.
	bool OnTriggerEvent();	
	//******************************************************************************************************************
	/// \brief					이미지 저장 함수.
	/// \param [in] strPath		저장 경로 입력.
	/// \param bool				결과 반환.
	bool OnSaveImage(CString strPath);
	//******************************************************************************************************************
	/// \brief					출력 신호 설정 함수.
	/// \param [in] nChannel	출력 신호 번호 입력.
	/// \param [in] eStyle		출력 신호 Style Type 입력.
	/// \param bool				결과 반환.
	bool SetPicoloOutputConfig(int nChannel, Picolo::EOutputStyle eStyle);
	bool SetDominoOutputConfig(int nChannel, Domino::EOutputStyle eStyle);
	bool SetGrablinkOutputConfig(int nChannel, Grablink::EOutputStyle eStyle);
	//******************************************************************************************************************
	/// \brief					출력 신호 상태 설정 함수.
	/// \param [in] nChannel	출력 신호 번호 입력.
	/// \param [in] bStatus		출력 신호 상태 설정. (true : High, false : Low)
	/// \param bool				결과 반환.
	bool SetOutputStatus(int nChannel, bool bStatus);
	//******************************************************************************************************************
	/// \brief					가로 해상도 획득 함수.
	/// \param [out] nValue		가로 해상도 확인.
	/// \param bool				결과 반환.
	bool GetWidth				(int &nValue);
	//******************************************************************************************************************
	/// \brief					세로 해상도 획득 함수.
	/// \param [out] nValue		세로 해상도 확인.
	/// \param bool				결과 반환.
	bool GetHeight				(int &nValue);
	//******************************************************************************************************************
	/// \brief					픽셀 포맷 획득 함수.
	/// \param [out] nValue		픽셀 포맷 확인.
	/// \param bool				결과 반환.
	bool GetBpp					(int &nValue);
	//******************************************************************************************************************
	/// \brief					Buffer Pitch 획득 함수.
	/// \param [out] nValue		Buffer Pitch 확인.
	/// \param bool				결과 반환.
	bool GetBufferPitch			(int &nValue);
	//******************************************************************************************************************
	/// \brief					Sequence Length 획득 함수. (Frame Count)
	/// \param [out] nValue		Sequence Length 확인.
	/// \param bool				결과 반환.
	bool GetSeqLength_Fr		(int &nValue);
	//******************************************************************************************************************
	/// \brief					Buffer Size 획득 함수.
	/// \param [out] nValue		Buffer Size 확인.
	/// \param bool				결과 반환.
	bool GetBufferSize			(int &nValue);
	//******************************************************************************************************************
	/// \brief					Frame Rate 획득 함수.
	/// \param [out] nValue		Frame Rate 확인.
	/// \param bool				결과 반환.
	bool GetFrameRate			(double &dValue);
	//******************************************************************************************************************
	/// \brief					Color Format 획득 함수.
	/// \param [out] nValue		Color Format 확인.
	/// \param bool				결과 반환.
	bool GetColorFormat			(CString &strValue);
	//******************************************************************************************************************
	/// \brief					Trigger Mode 획득 함수.
	/// \param [out] nValue		Trigger Mode 확인.
	/// \param bool				결과 반환.
	bool GetTrigMode			(CString &strValue);
	//******************************************************************************************************************
	/// \brief					Next Trigger Mode 획득 함수.
	/// \param [out] nValue		Next Trigger Mode 확인.
	/// \param bool				결과 반환.
	bool GetNextTrigMode		(CString &strValue);
	//******************************************************************************************************************
	/// \brief					Frame Rate 설정 함수. (Immediate Trigger Mode 일 때 사용.)
	/// \param [in] nValue		Frame Rate 입력.
	/// \param bool				결과 반환.
	bool SetFrameRate_mHz		(int nValue);
	//******************************************************************************************************************
	/// \brief					Sequence Length 입력 함수.
	/// \param [in] nValue		Sequence Length 입력.
	/// \param bool				결과 반환.
	bool SetSeqLength_Fr		(int nValue);
	//******************************************************************************************************************
	/// \brief					Surface Count 입력 함수.
	/// \param [in] nValue		Surface Count 입력.
	/// \param bool				결과 반환.
	bool SetSurfaceCount		(int nValue);
	//******************************************************************************************************************
	/// \brief					Exposure Time 입력 함수. (단위 : us)
	/// \param [in] nValue		Exposure Time 입력.
	/// \param bool				결과 반환.
	bool SetExpose_us			(int nValue);
	//******************************************************************************************************************
	/// \brief					ROI WindowX 크기 입력 함수. (Grab Window 가 enable일 때 사용.)
	/// \param [in] nValue		ROI WindowX 크기 입력.
	/// \param bool				결과 반환.
	bool SetWindowX_Px			(int nValue);
	//******************************************************************************************************************
	/// \brief					ROI WindowY 크기 입력 함수. (Grab Window 가 enable일 때 사용.)
	/// \param [in] nValue		ROI WindowY 크기 입력.
	/// \param bool				결과 반환.
	bool SetWindowY_Ln			(int nValue);
	//******************************************************************************************************************
	/// \brief					ROI Window 사용 설정 함수.
	/// \param [in] bEnable		사용 여부 입력.
	/// \param bool				결과 반환.
	bool SetGrabWindow			(bool bEnable);
	//******************************************************************************************************************
	/// \brief					Flip X 사용 설정 함수.
	/// \param [in] bEnable		사용 여부 입력.
	/// \param bool				결과 반환.
	bool SetFlipX				(bool bEnable);
	//******************************************************************************************************************
	/// \brief					Flip Y 사용 설정 함수.
	/// \param [in] bEnable		사용 여부 입력.
	/// \param bool				결과 반환.
	bool SetFlipY				(bool bEnable);
	//******************************************************************************************************************
	/// \brief					Hardware Trigger Line 설정 함수.
	/// \param [in] eLine		Hardware Trigger Line Type 입력.
	/// \param bool				결과 반환.
	bool SetTriggerLine			(ETriggerLine eLine);
	//******************************************************************************************************************
	/// \brief					Software Trigger Mode 설정 함수.
	/// \param bool				결과 반환.
	bool SetSoftwareTriggerMode	();
	//******************************************************************************************************************
	/// \brief					Hardware Trigger Mode 설정 함수.
	/// \param bool				결과 반환.
	bool SetHardwareTriggerMode	();
	//******************************************************************************************************************
	/// \brief					Immediate Trigger Mode 설정 함수.
	/// \param bool				결과 반환.
	bool SetImmediateMode		();
	//******************************************************************************************************************
	/// \brief					Combined Trigger Mode 설정 함수.
	/// \param bool				결과 반환.
	bool SetCombinedMode		();
	//******************************************************************************************************************
	/// \brief					이미지 획득이 완료됐는지 확인하는 Handle 반환 함수.
	/// \param bool				핸들 반환.
	HANDLE GetHandleGrabDone() { return m_hGrabDone; }
	//******************************************************************************************************************
	/// \brief					이미지 획득이 완료됐는지 확인하는 Handle을 초기화(Reset)시키는 함수.
	void ResetHandleGrabDone() { ResetEvent(m_hGrabDone); }
	//******************************************************************************************************************
	/// \brief					이미지 데이터 획득 함수.
	/// \param bool				이미지 데이터 포인터 반환.
	BYTE* GetImageBuffer() { return m_pbyBuffer; }
	//******************************************************************************************************************
	/// \brief					보드 초기화 확인 함수.
	/// \param bool				보드 초기화 여부 반환.
	bool IsOpened() { return m_bOpened; }
	//******************************************************************************************************************
	/// \brief					마지막 에러 메시지 확인 함수.
	/// \param CString			마지막 에러 메시지 반환.
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
	/// \brief					파라미터 값을 Type별로 Get/Set 하는 함수.
	/// \param [in] pNodeName	파라미터의 Node Name 입력.
	/// \param [in]  Value		파라미터의 값 입력.
	/// \param [out] Value		파라미터의 값 확인.
	/// \param bool				결과 반환.
	bool GetValueString(char* pNodeName, CString &strValue);
	bool SetValueString(char* pNodeName, CString strValue);
	bool GetValueInt(char* pNodeName, int &nValue);
	bool SetValueInt(char* pNodeName, int nValue);
	bool GetValueDouble(char* pNodeName, double &dValue);
	bool SetValueDouble(char* pNodeName, double dValue);
	bool ExecuteCommand(char* pNodeName);
};