#include "stdafx.h"
#include "DAQmxTasksAndCodes.h"

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
CDAQmxTaskDynamicCardDigital32::CDAQmxTaskDynamicCardDigital32 ()
    : CNiDAQmxTask(_T("CDAQmxTaskDynamicCardDigital32"))
{
    this->Configure();
}
void CDAQmxTaskDynamicCardDigital32::Configure ()
{
	unsigned int NbrSamples = 100000;
    CNiDAQmxDOChannel ch = this->DOChannels.CreateChannel(_T("PXI1Slot4/port0_32"), _T("DigitalOut"), DAQmxOneChannelForAllLines);
    ch.InvertLines = false;

	this->Timing.ConfigureSampleClock(_T(""), Sequence::m_SampleRate, DAQmxSampleClockActiveEdgeRising, DAQmxSampleQuantityModeContinuousSamples, NbrSamples);
}
void CDAQmxTaskDynamicCardDigital32::Configure (unsigned int NbrSamples)
{
	this->Timing.ConfigureSampleClock(_T(""), Sequence::m_SampleRate, DAQmxSampleClockActiveEdgeRising, DAQmxSampleQuantityModeFiniteSamples, NbrSamples);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
CDAQmxTaskDynamicCardAnalog1::CDAQmxTaskDynamicCardAnalog1 ()
    : CNiDAQmxTask(_T("CDAQmxTaskDynamicCardAnalog1"))
{
    this->Configure();
}
void CDAQmxTaskDynamicCardAnalog1::Configure ()
{
  	unsigned int NbrSamples = 100000;
    this->AOChannels.CreateVoltageChannel(_T("PXI1Slot2/ao0"), _T("VoltageOut0"), -10, 10, DAQmxAOVoltageUnitsVolts);
    this->AOChannels.CreateVoltageChannel(_T("PXI1Slot2/ao1"), _T("VoltageOut1"), -10, 10, DAQmxAOVoltageUnitsVolts);
    this->AOChannels.CreateVoltageChannel(_T("PXI1Slot2/ao2"), _T("VoltageOut2"), -10, 10, DAQmxAOVoltageUnitsVolts);
    this->AOChannels.CreateVoltageChannel(_T("PXI1Slot2/ao3"), _T("VoltageOut3"), -10, 10, DAQmxAOVoltageUnitsVolts);
    this->AOChannels.CreateVoltageChannel(_T("PXI1Slot2/ao4"), _T("VoltageOut4"), -10, 10, DAQmxAOVoltageUnitsVolts);
    this->AOChannels.CreateVoltageChannel(_T("PXI1Slot2/ao5"), _T("VoltageOut5"), -10, 10, DAQmxAOVoltageUnitsVolts);
    this->AOChannels.CreateVoltageChannel(_T("PXI1Slot2/ao6"), _T("VoltageOut6"), -10, 10, DAQmxAOVoltageUnitsVolts);
    this->AOChannels.CreateVoltageChannel(_T("PXI1Slot2/ao7"), _T("VoltageOut7"), -10, 10, DAQmxAOVoltageUnitsVolts);

	this->Timing.ConfigureSampleClock(_T(""), Sequence::m_SampleRate, DAQmxSampleClockActiveEdgeRising, DAQmxSampleQuantityModeContinuousSamples, NbrSamples);
}

void CDAQmxTaskDynamicCardAnalog1::Configure (unsigned int NbrSamples)
{
	this->Timing.ConfigureSampleClock(_T(""), Sequence::m_SampleRate, DAQmxSampleClockActiveEdgeRising, DAQmxSampleQuantityModeFiniteSamples, NbrSamples);
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
CDAQmxTaskDynamicCardAnalog2::CDAQmxTaskDynamicCardAnalog2()
    : CNiDAQmxTask(_T("CDAQmxTaskDynamicCardAnalog2"))
{
    this->Configure();
}
void CDAQmxTaskDynamicCardAnalog2::Configure ()
{
  	unsigned int NbrSamples = 100000;
    this->AOChannels.CreateVoltageChannel(_T("PXI_Analog_Out_2/ao0"), _T("VoltageOut0"), -10, 10, DAQmxAOVoltageUnitsVolts);
    this->AOChannels.CreateVoltageChannel(_T("PXI_Analog_Out_2/ao1"), _T("VoltageOut1"), -10, 10, DAQmxAOVoltageUnitsVolts);
    this->AOChannels.CreateVoltageChannel(_T("PXI_Analog_Out_2/ao2"), _T("VoltageOut2"), -10, 10, DAQmxAOVoltageUnitsVolts);
    this->AOChannels.CreateVoltageChannel(_T("PXI_Analog_Out_2/ao3"), _T("VoltageOut3"), -10, 10, DAQmxAOVoltageUnitsVolts);
    this->AOChannels.CreateVoltageChannel(_T("PXI_Analog_Out_2/ao4"), _T("VoltageOut4"), -10, 10, DAQmxAOVoltageUnitsVolts);
    this->AOChannels.CreateVoltageChannel(_T("PXI_Analog_Out_2/ao5"), _T("VoltageOut5"), -10, 10, DAQmxAOVoltageUnitsVolts);
    this->AOChannels.CreateVoltageChannel(_T("PXI_Analog_Out_2/ao6"), _T("VoltageOut6"), -10, 10, DAQmxAOVoltageUnitsVolts);
    this->AOChannels.CreateVoltageChannel(_T("PXI_Analog_Out_2/ao7"), _T("VoltageOut7"), -10, 10, DAQmxAOVoltageUnitsVolts);

	this->Timing.ConfigureSampleClock(_T(""), Sequence::m_SampleRate, DAQmxSampleClockActiveEdgeRising, DAQmxSampleQuantityModeContinuousSamples, NbrSamples);
}

void CDAQmxTaskDynamicCardAnalog2::Configure (unsigned int NbrSamples)
{
	this->Timing.ConfigureSampleClock(_T(""), Sequence::m_SampleRate, DAQmxSampleClockActiveEdgeRising, DAQmxSampleQuantityModeFiniteSamples, NbrSamples);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
CDAQmxTaskStaticCardDigital32::CDAQmxTaskStaticCardDigital32 ()
    : CNiDAQmxTask(_T("CDAQmxTaskStaticCardDigital32"))
{
    this->Configure();
}
void CDAQmxTaskStaticCardDigital32::Configure ()
{
    CNiDAQmxDOChannel ch = this->DOChannels.CreateChannel(_T("PXI1Slot4/port0_32"), _T("SortieNumérique"), DAQmxOneChannelForAllLines);
    ch.InvertLines = false;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
CDAQmxTaskStaticCardAnalog1::CDAQmxTaskStaticCardAnalog1 ()
    : CNiDAQmxTask(_T("CDAQmxTaskStaticCardAnalog1"))
{
    this->Configure();
}
void CDAQmxTaskStaticCardAnalog1::Configure ()
{
    this->AOChannels.CreateVoltageChannel(_T("PXI1Slot2/ao0"), _T("VoltageOut0"), -10, 10, DAQmxAOVoltageUnitsVolts);
    this->AOChannels.CreateVoltageChannel(_T("PXI1Slot2/ao1"), _T("VoltageOut1"), -10, 10, DAQmxAOVoltageUnitsVolts);
    this->AOChannels.CreateVoltageChannel(_T("PXI1Slot2/ao2"), _T("VoltageOut2"), -10, 10, DAQmxAOVoltageUnitsVolts);
    this->AOChannels.CreateVoltageChannel(_T("PXI1Slot2/ao3"), _T("VoltageOut3"), -10, 10, DAQmxAOVoltageUnitsVolts);
    this->AOChannels.CreateVoltageChannel(_T("PXI1Slot2/ao4"), _T("VoltageOut4"), -10, 10, DAQmxAOVoltageUnitsVolts);
    this->AOChannels.CreateVoltageChannel(_T("PXI1Slot2/ao5"), _T("VoltageOut5"), -10, 10, DAQmxAOVoltageUnitsVolts);
    this->AOChannels.CreateVoltageChannel(_T("PXI1Slot2/ao6"), _T("VoltageOut6"), -10, 10, DAQmxAOVoltageUnitsVolts);
    this->AOChannels.CreateVoltageChannel(_T("PXI1Slot2/ao7"), _T("VoltageOut7"), -10, 10, DAQmxAOVoltageUnitsVolts);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
CDAQmxTaskStaticCardAnalog2::CDAQmxTaskStaticCardAnalog2 ()
    : CNiDAQmxTask(_T("CDAQmxTaskStaticCardAnalog2"))
{
    this->Configure();
}
void CDAQmxTaskStaticCardAnalog2::Configure ()
{
    this->AOChannels.CreateVoltageChannel(_T("PXI_Analog_Out_2/ao0"), _T("VoltageOut0"), -10, 10, DAQmxAOVoltageUnitsVolts);
    this->AOChannels.CreateVoltageChannel(_T("PXI_Analog_Out_2/ao1"), _T("VoltageOut1"), -10, 10, DAQmxAOVoltageUnitsVolts);
    this->AOChannels.CreateVoltageChannel(_T("PXI_Analog_Out_2/ao2"), _T("VoltageOut2"), -10, 10, DAQmxAOVoltageUnitsVolts);
    this->AOChannels.CreateVoltageChannel(_T("PXI_Analog_Out_2/ao3"), _T("VoltageOut3"), -10, 10, DAQmxAOVoltageUnitsVolts);
    this->AOChannels.CreateVoltageChannel(_T("PXI_Analog_Out_2/ao4"), _T("VoltageOut4"), -10, 10, DAQmxAOVoltageUnitsVolts);
    this->AOChannels.CreateVoltageChannel(_T("PXI_Analog_Out_2/ao5"), _T("VoltageOut5"), -10, 10, DAQmxAOVoltageUnitsVolts);
    this->AOChannels.CreateVoltageChannel(_T("PXI_Analog_Out_2/ao6"), _T("VoltageOut6"), -10, 10, DAQmxAOVoltageUnitsVolts);
    this->AOChannels.CreateVoltageChannel(_T("PXI_Analog_Out_2/ao7"), _T("VoltageOut7"), -10, 10, DAQmxAOVoltageUnitsVolts);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
CDAQmxTaskDynamicCardAnalogInput::CDAQmxTaskDynamicCardAnalogInput () :
	CNiDAQmxTask(_T("CDAQmxTaskDynamicCardAnalogInput"))
{
//	this->Configure();
}
void CDAQmxTaskDynamicCardAnalogInput::Configure(int NumChanel, double FromV /* = -10.0 */, double ToV /* = 10.0 */)
{
	if(NumChanel == 0)
		this->AIChannels.CreateVoltageChannel(_T("PXI_Analog_In_1/ai0") , _T("Voltage0") , DAQmxAITerminalConfigurationRse, FromV, ToV, DAQmxAIVoltageUnitsVolts);
	if(NumChanel == 1)
		this->AIChannels.CreateVoltageChannel(_T("PXI_Analog_In_1/ai1") , _T("Voltage1") , DAQmxAITerminalConfigurationRse, FromV, ToV, DAQmxAIVoltageUnitsVolts);
	if(NumChanel == 2)
		this->AIChannels.CreateVoltageChannel(_T("PXI_Analog_In_1/ai2") , _T("Voltage2") , DAQmxAITerminalConfigurationRse, FromV, ToV, DAQmxAIVoltageUnitsVolts);
	if(NumChanel == 3)
		this->AIChannels.CreateVoltageChannel(_T("PXI_Analog_In_1/ai3") , _T("Voltage3") , DAQmxAITerminalConfigurationRse, FromV, ToV, DAQmxAIVoltageUnitsVolts);
	if(NumChanel == 4)
		this->AIChannels.CreateVoltageChannel(_T("PXI_Analog_In_1/ai4") , _T("Voltage4") , DAQmxAITerminalConfigurationRse, FromV, ToV, DAQmxAIVoltageUnitsVolts);
	if(NumChanel == 5)
		this->AIChannels.CreateVoltageChannel(_T("PXI_Analog_In_1/ai5") , _T("Voltage5") , DAQmxAITerminalConfigurationRse, FromV, ToV, DAQmxAIVoltageUnitsVolts);
	if(NumChanel == 6)
		this->AIChannels.CreateVoltageChannel(_T("PXI_Analog_In_1/ai6") , _T("Voltage6") , DAQmxAITerminalConfigurationRse, FromV, ToV, DAQmxAIVoltageUnitsVolts);
	if(NumChanel == 7)
		this->AIChannels.CreateVoltageChannel(_T("PXI_Analog_In_1/ai7") , _T("Voltage7") , DAQmxAITerminalConfigurationRse, FromV, ToV, DAQmxAIVoltageUnitsVolts);
	if(NumChanel == 8)
		this->AIChannels.CreateVoltageChannel(_T("PXI_Analog_In_1/ai8") , _T("Voltage8") , DAQmxAITerminalConfigurationRse, FromV, ToV, DAQmxAIVoltageUnitsVolts);
	if(NumChanel == 9)
		this->AIChannels.CreateVoltageChannel(_T("PXI_Analog_In_1/ai9") , _T("Voltage9") , DAQmxAITerminalConfigurationRse, FromV, ToV, DAQmxAIVoltageUnitsVolts);
	if(NumChanel == 10)
		this->AIChannels.CreateVoltageChannel(_T("PXI_Analog_In_1/ai10"), _T("Voltage10"), DAQmxAITerminalConfigurationRse, FromV, ToV, DAQmxAIVoltageUnitsVolts);
	if(NumChanel == 11)
		this->AIChannels.CreateVoltageChannel(_T("PXI_Analog_In_1/ai11"), _T("Voltage11"), DAQmxAITerminalConfigurationRse, FromV, ToV, DAQmxAIVoltageUnitsVolts);
	if(NumChanel == 12)
		this->AIChannels.CreateVoltageChannel(_T("PXI_Analog_In_1/ai12"), _T("Voltage12"), DAQmxAITerminalConfigurationRse, FromV, ToV, DAQmxAIVoltageUnitsVolts);
	if(NumChanel == 13)
		this->AIChannels.CreateVoltageChannel(_T("PXI_Analog_In_1/ai13"), _T("Voltage13"), DAQmxAITerminalConfigurationRse, FromV, ToV, DAQmxAIVoltageUnitsVolts);
	if(NumChanel == 14)
		this->AIChannels.CreateVoltageChannel(_T("PXI_Analog_In_1/ai14"), _T("Voltage14"), DAQmxAITerminalConfigurationRse, FromV, ToV, DAQmxAIVoltageUnitsVolts);
	if(NumChanel == 15)
		this->AIChannels.CreateVoltageChannel(_T("PXI_Analog_In_1/ai15"), _T("Voltage15"), DAQmxAITerminalConfigurationRse, FromV, ToV, DAQmxAIVoltageUnitsVolts);
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
CNiUInt32Matrix DataDigital32;
CNiReal64Matrix DataAnalog1;
CNiReal64Matrix DataAnalog2;
////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
//		You can use this DAQmx user code to perform a continuous multichannel digital write.
//      N is the number of samples per channel configured in the task.
//

CDAQmxDynamicCardDigital32::CDAQmxDynamicCardDigital32() :
	m_task(),
    m_taskWriter(m_task.Stream)
{
}

void CDAQmxDynamicCardDigital32::SendNiMatrixDataToTheCard(CNiUInt32Matrix& data)
{
    m_taskWriter.WriteMultiSamplePort(false, data);
}

CNiUInt32Matrix& CDAQmxDynamicCardDigital32::GenerateNiMatrixDataForTheCard(unsigned long* p_Buffer, int numberOfSamples)
{
	int numChannels = m_task.DOChannels.Count;

	DataDigital32.SetSize(numChannels, numberOfSamples, CNiMatrix::NiMatrixDoNotInit);
	
	for(int i = 0; i < numChannels; i++)
		for(int j = 0; j < numberOfSamples; j++)
			DataDigital32(i, j) = p_Buffer[j];

return DataDigital32;
}

CDAQmxTaskDynamicCardDigital32& CDAQmxDynamicCardDigital32::GetTask()
{
    return m_task;
}

const CDAQmxTaskDynamicCardDigital32& CDAQmxDynamicCardDigital32::GetTask() const
{
    return m_task;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
//		You can use this DAQmx user code to perform an N-sample multichannel analog write.
//      N is the number of samples per channel configured in the task.
//

CDAQmxDynamicCardAnalog1::CDAQmxDynamicCardAnalog1() :
	m_task(),
    m_taskWriter(m_task.Stream)
{
}

void CDAQmxDynamicCardAnalog1::SendNiMatrixDataToTheCard(CNiReal64Matrix& data)
{
    m_taskWriter.WriteMultiSample(false, data);
}

CNiReal64Matrix& CDAQmxDynamicCardAnalog1::GenerateNiMatrixDataForTheCard(double** p_Buffer, int numberOfSamples)
{
	int numChannels = m_task.AOChannels.Count;

	DataAnalog1.SetSize(numChannels, numberOfSamples, CNiMatrix::NiMatrixDoNotInit);

	for(int i = 0; i < numChannels; i++)
		for(int j = 0; j < numberOfSamples; j++)
			DataAnalog1(i, j) = p_Buffer[i][j];

return DataAnalog1;
}

CDAQmxTaskDynamicCardAnalog1& CDAQmxDynamicCardAnalog1::GetTask()
{
    return m_task;
}

const CDAQmxTaskDynamicCardAnalog1& CDAQmxDynamicCardAnalog1::GetTask() const
{
    return m_task;
}

CDAQmxDynamicCardAnalog2::CDAQmxDynamicCardAnalog2() :
	m_task(),
    m_taskWriter(m_task.Stream)
{
}

void CDAQmxDynamicCardAnalog2::SendNiMatrixDataToTheCard(CNiReal64Matrix& data)
{
    m_taskWriter.WriteMultiSample(false, data);
}

CNiReal64Matrix& CDAQmxDynamicCardAnalog2::GenerateNiMatrixDataForTheCard(double** p_Buffer, int numberOfSamples)
{
	int numChannels = m_task.AOChannels.Count;

	DataAnalog2.SetSize(numChannels, numberOfSamples, CNiMatrix::NiMatrixDoNotInit);

	for(int i = 0; i < numChannels; i++)
		for(int j = 0; j < numberOfSamples; j++)
			DataAnalog2(i, j) = p_Buffer[i][j];

return DataAnalog2;
}

CDAQmxTaskDynamicCardAnalog2& CDAQmxDynamicCardAnalog2::GetTask()
{
    return m_task;
}

const CDAQmxTaskDynamicCardAnalog2& CDAQmxDynamicCardAnalog2::GetTask() const
{
    return m_task;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
//		You can use this DAQmx user code to write a single digital port to one or more channels.
//

CDAQmxStaticCardDigital32::CDAQmxStaticCardDigital32() :
	m_task(),
	m_taskWriter(m_task.Stream)
{
}

void CDAQmxStaticCardDigital32::SendNiMatrixDataToTheCard(CNiUInt32Vector& data)
{
	m_taskWriter.WriteSingleSamplePort(true, data);
}

CNiUInt32Vector CDAQmxStaticCardDigital32::GenerateNiMatrixDataForTheCard(int DigOut[])
{
	m_task.Control(NI::DAQmxTaskVerify);
	long size = m_task.Stream.NumberOfOutputChannels;
	CNiUInt32Vector data(size);

	unsigned int bit = 1;
	unsigned int Le32bit = 0;
	for(int i = 0; i < 32/*NUMBER_DIG_OUTPUT*/; i++)
	{
		int iemeDigVal = DigOut[i];
		Le32bit += bit * iemeDigVal;
		bit *= 2;
	}
	data[0] = Le32bit;
	return data;
}

CDAQmxTaskStaticCardDigital32& CDAQmxStaticCardDigital32::GetTask()
{
    return m_task;
}

const CDAQmxTaskStaticCardDigital32& CDAQmxStaticCardDigital32::GetTask() const
{
    return m_task;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
//		You can use this DAQmx user code to write a single analog point to one or more channels.
//

CDAQmxStaticCardAnalog1::CDAQmxStaticCardAnalog1() :
	m_task(),
	m_taskWriter(m_task.Stream)
{
}

void CDAQmxStaticCardAnalog1::SendNiMatrixDataToTheCard(CNiReal64Vector& data)
{
	m_taskWriter.WriteSingleSample(true, data);
}

CNiReal64Vector CDAQmxStaticCardAnalog1::GenerateNiMatrixDataForTheCard(int AnaOut[])
{
	m_task.Control(NI::DAQmxTaskVerify);
	long size = m_task.Stream.NumberOfOutputChannels;
	CNiReal64Vector data(size);

	for(int i=0; i < size; i++)
	{
		double iemeAnaVal = AnaOut[i];
		data[i] = iemeAnaVal MILLI_VOLT;
	}
	return data;
}

CDAQmxTaskStaticCardAnalog1& CDAQmxStaticCardAnalog1::GetTask()
{
    return m_task;
}

const CDAQmxTaskStaticCardAnalog1& CDAQmxStaticCardAnalog1::GetTask() const
{
    return m_task;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
//		You can use this DAQmx user code to write a single analog point to one or more channels.
//

CDAQmxStaticCardAnalog2::CDAQmxStaticCardAnalog2() :
	m_task(),
	m_taskWriter(m_task.Stream)
{
}

void CDAQmxStaticCardAnalog2::SendNiMatrixDataToTheCard(CNiReal64Vector& data)
{
	m_taskWriter.WriteSingleSample(true, data);
}

CNiReal64Vector CDAQmxStaticCardAnalog2::GenerateNiMatrixDataForTheCard(int AnaOut[])
{
	m_task.Control(NI::DAQmxTaskVerify);
	long size = m_task.Stream.NumberOfOutputChannels;
	CNiReal64Vector data(size);

	for(int i=0; i < size; i++)
	{
		double iemeAnaVal = AnaOut[i];
		data[i] = iemeAnaVal MILLI_VOLT;
	}
	return data;
}

CDAQmxTaskStaticCardAnalog2& CDAQmxStaticCardAnalog2::GetTask()
{
    return m_task;
}

const CDAQmxTaskStaticCardAnalog2& CDAQmxStaticCardAnalog2::GetTask() const
{
    return m_task;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
//		You can use this DAQmx user code to acquire a single analog point from one or more channels.
//

CDAQmxDynamicCardAnalogInput::CDAQmxDynamicCardAnalogInput() :
	m_task(),
	m_taskReader(m_task.Stream)
{
}

void CDAQmxDynamicCardAnalogInput::GetData(CNiReal64Matrix& data)
{
	m_taskReader.ReadMultiSample(10000, data);
}

CDAQmxTaskDynamicCardAnalogInput& CDAQmxDynamicCardAnalogInput::GetTask()
{
    return m_task;
}

const CDAQmxTaskDynamicCardAnalogInput& CDAQmxDynamicCardAnalogInput::GetTask() const
{
    return m_task;
}
