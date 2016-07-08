#pragma once
#include "Sequence.h"

class CDAQmxTaskDynamicCardDigital32 : public CNiDAQmxTask  
{
    public:
        void Configure ();
        void Configure (unsigned int NbrSamples);
    public:
        CDAQmxTaskDynamicCardDigital32 ();
};
class CDAQmxTaskDynamicCardAnalog1 : public CNiDAQmxTask  
{
    public:
        void Configure ();
        void Configure (unsigned int NbrSamples);
    public:
        CDAQmxTaskDynamicCardAnalog1 ();
};
class CDAQmxTaskDynamicCardAnalog2 : public CNiDAQmxTask  
{
    public:
        void Configure ();
        void Configure (unsigned int NbrSamples);
    public:
        CDAQmxTaskDynamicCardAnalog2 ();
};
class CDAQmxTaskStaticCardDigital32 : public CNiDAQmxTask  
{
    public:
        void Configure ();
    public:
        CDAQmxTaskStaticCardDigital32 ();
};
class CDAQmxTaskStaticCardAnalog1 : public CNiDAQmxTask  
{
    public:
        void Configure ();
    public:
        CDAQmxTaskStaticCardAnalog1 ();
};
class CDAQmxTaskStaticCardAnalog2 : public CNiDAQmxTask  
{
    public:
        void Configure ();
    public:
        CDAQmxTaskStaticCardAnalog2 ();
};
class CDAQmxTaskDynamicCardAnalogInput : public CNiDAQmxTask  
{
    public:
        void Configure(int NumChanel, double FromV = -10.0, double ToV = 10.0);
    public:
        CDAQmxTaskDynamicCardAnalogInput ();
};
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
//		You can use this DAQmx user code to perform a continuous multichannel digital write.
//      N is the number of samples per channel configured in the task.
//
// How to use the DAQmx user code programmatically without the dialog
//		1.  Declare an instance of the class in your code.
//          Call it's GenerateNiMatrixDataForTheCard method to run the task, as shown in the following example:
//		 
//			CDAQmxDynamicCardDigital32 userCode;
//			userCode.GenerateNiMatrixDataForTheCard();

class CDAQmxDynamicCardDigital32
{
	public:
		CDAQmxDynamicCardDigital32();

		CNiUInt32Matrix& GenerateNiMatrixDataForTheCard(unsigned long* p_Buffer, int numberOfSamples);	
										// modifié à ma guise pour générer les "data" à partir de mon "buffer"
										// il faudra envisager de simplifier cette partie si besoin est
		void SendNiMatrixDataToTheCard(CNiUInt32Matrix& data);
        CDAQmxTaskDynamicCardDigital32& GetTask();
        const CDAQmxTaskDynamicCardDigital32& GetTask() const;
        __declspec(property(get=GetTask)) CDAQmxTaskDynamicCardDigital32& Task;
    
	private:
		CDAQmxTaskDynamicCardDigital32 m_task;
		CNiDAQmxDigitalMultiChannelWriter m_taskWriter;
};
// Description:
//		You can use this DAQmx user code to perform an N-sample multichannel analog write.
//      N is the number of samples per channel configured in the task.
//
// How to use the DAQmx user code programmatically without the dialog
//		1.  Declare an instance of the class in your code.
//          Call it's GenerateNiMatrixDataForTheCard method to run the task, as shown in the following example:
//		 
//			CDAQmxDynamicCardAnalog1 userCode;
//			userCode.GenerateNiMatrixDataForTheCard();

class CDAQmxDynamicCardAnalog1
{
	public:
		CDAQmxDynamicCardAnalog1();

		CNiReal64Matrix& GenerateNiMatrixDataForTheCard(double** p_Buffer, int numberOfSamples);
										// modifié à ma guise pour générer les "data" à partir de mon "buffer"
										// il faudra envisager de simplifier cette partie si besoin est
		void SendNiMatrixDataToTheCard(CNiReal64Matrix& data);
        CDAQmxTaskDynamicCardAnalog1& GetTask();
        const CDAQmxTaskDynamicCardAnalog1& GetTask() const;
        __declspec(property(get=GetTask)) CDAQmxTaskDynamicCardAnalog1& Task;
    
	private:
		CDAQmxTaskDynamicCardAnalog1 m_task;
		CNiDAQmxAnalogMultiChannelWriter m_taskWriter;
};

class CDAQmxDynamicCardAnalog2
{
	public:
		CDAQmxDynamicCardAnalog2();

		CNiReal64Matrix& GenerateNiMatrixDataForTheCard(double** p_Buffer, int numberOfSamples);
										// modifié à ma guise pour générer les "data" à partir de mon "buffer"
										// il faudra envisager de simplifier cette partie si besoin est
		void SendNiMatrixDataToTheCard(CNiReal64Matrix& data);
        CDAQmxTaskDynamicCardAnalog2& GetTask();
        const CDAQmxTaskDynamicCardAnalog2& GetTask() const;
        __declspec(property(get=GetTask)) CDAQmxTaskDynamicCardAnalog2& Task;
    
	private:
		CDAQmxTaskDynamicCardAnalog2 m_task;
		CNiDAQmxAnalogMultiChannelWriter m_taskWriter;
};
// Description:
//		You can use this DAQmx user code to write a single digital port to one or more channels.
//
// How to use the DAQmx user code programmatically without the dialog
//		1.  Declare an instance of the class in your code.
//          Call it's GenerateNiMatrixDataForTheCard method to run the task, as shown in the following example:
//		 
//			CDAQmxStaticCardDigital32 userCode;
//			userCode.GenerateNiMatrixDataForTheCard();

class CDAQmxStaticCardDigital32
{
	public:
		CDAQmxStaticCardDigital32();

		void SendNiMatrixDataToTheCard(CNiUInt32Vector& data);
		CNiUInt32Vector GenerateNiMatrixDataForTheCard(int DigOut[]);
        CDAQmxTaskStaticCardDigital32& GetTask();
        const CDAQmxTaskStaticCardDigital32& GetTask() const;
        __declspec(property(get=GetTask)) CDAQmxTaskStaticCardDigital32& Task;
    
	private:
		CDAQmxTaskStaticCardDigital32 m_task;
		CNiDAQmxDigitalMultiChannelWriter m_taskWriter;
};
// Description:
//		You can use this DAQmx user code to write a single analog point to one or more channels.
//
// How to use the DAQmx user code programmatically without the dialog
//		1.  Declare an instance of the class in your code.
//          Call it's GenerateNiMatrixDataForTheCard method to run the task, as shown in the following example:
//		 
//			CDAQmxStaticCardAnalog1 userCode;
//			userCode.GenerateNiMatrixDataForTheCard();

class CDAQmxStaticCardAnalog1
{
	public:
		CDAQmxStaticCardAnalog1();

		void SendNiMatrixDataToTheCard(CNiReal64Vector& data);
		CNiReal64Vector GenerateNiMatrixDataForTheCard(int AnaOut[]);
        CDAQmxTaskStaticCardAnalog1& GetTask();
        const CDAQmxTaskStaticCardAnalog1& GetTask() const;
        __declspec(property(get=GetTask)) CDAQmxTaskStaticCardAnalog1& Task;
    
	private:
		CDAQmxTaskStaticCardAnalog1 m_task;
		CNiDAQmxAnalogMultiChannelWriter m_taskWriter;
};
// Description:
//		You can use this DAQmx user code to write a single analog point to one or more channels.
//
// How to use the DAQmx user code programmatically without the dialog
//		1.  Declare an instance of the class in your code.
//          Call it's GenerateNiMatrixDataForTheCard method to run the task, as shown in the following example:
//		 
//			CDAQmxStaticCardAnalog2 userCode;
//			userCode.GenerateNiMatrixDataForTheCard();

class CDAQmxStaticCardAnalog2
{
	public:
		CDAQmxStaticCardAnalog2();

		void SendNiMatrixDataToTheCard(CNiReal64Vector& data);
		CNiReal64Vector GenerateNiMatrixDataForTheCard(int AnaOut[]);
        CDAQmxTaskStaticCardAnalog2& GetTask();
        const CDAQmxTaskStaticCardAnalog2& GetTask() const;
        __declspec(property(get=GetTask)) CDAQmxTaskStaticCardAnalog2& Task;
    
	private:
		CDAQmxTaskStaticCardAnalog2 m_task;
		CNiDAQmxAnalogMultiChannelWriter m_taskWriter;
};
// Description:
//		You can use this DAQmx user code to acquire a single analog point from one or more channels.
//
// How to use the DAQmx user code programmatically without the dialog
//		1.  Declare an instance of the class in your code.
//          Call it's GetData method to run the task, as shown in the following example:
// 
//          CNiReal64Vector data;
//			CDAQmxDynamicCardAnalogInput userCode;
//          m_userCode->GetData(data);

class CDAQmxDynamicCardAnalogInput
{
	public:
		CDAQmxDynamicCardAnalogInput();

		void GetData(CNiReal64Matrix& data);
        CDAQmxTaskDynamicCardAnalogInput& GetTask();
        const CDAQmxTaskDynamicCardAnalogInput& GetTask() const;
        __declspec(property(get=GetTask)) CDAQmxTaskDynamicCardAnalogInput& Task;
    
	private:
		CDAQmxTaskDynamicCardAnalogInput m_task;
	public:
		CNiDAQmxAnalogMultiChannelReader m_taskReader;
};