/*	N'intervient pas dans ce programme, mais doit etre présent pour compiler...	*/

//-----------------------------------------------------------------------------
//  Company:  Basler Vision Technologies
//  Section:  Vision Components
//  Project:  1394 Driver
//  <b> Visual SourceSafe Information:</b>
//  $Archive: /Product/CFI/Dev/PC/ReviewSensorSupport/BCAM/Software_VS6/src/bcamapi/Bcam.cpp $
//  $Author: Happe, A.$
//  $Revision: 51$
//  $Date: 10/16/03 10:10:40 AM$
//-----------------------------------------------------------------------------
/**
 * \file     Bcam.cpp
 *
 *
 * \brief   Implementation of the user api
*/
//-----------------------------------------------------------------------------


#include <stdafx.h>
#pragma warning (disable: 4786) // identifier was truncated to '255' characters in the debug information
#pragma warning (disable: 4355) // 'this' used in base member initializer list
#pragma warning (disable: 4100 )
#pragma warning (disable: 4239 )
#pragma warning (disable: 4702) // unreachable code

#include <bcam.h>
#include <atlbase.h>
#include <bcamutility.h>
#include <setupapi.h>
#include <md5.h>

#ifndef BCAM_GUID_INITIALIZED
#include <initguid.h>
#include <bcam_guid.h>
#endif

#include <malloc.h>
#include <bcamerror.h>
#include <bcamversion.h>

#ifndef TRACE
/// In case we are using WTL replace TRACE calls by AtlTrace
#define TRACE AtlTrace
#endif

/******************************************************************************/
using namespace Bcam;

static CThread    s_MessageLoopThread;   ///< thread which runs the message loop for the message-only window

/*
* Synchronization of all camera objects
*/
static Bcam::CCriticalSection s_TheCriticalSection;

typedef CAutoLock<Bcam::CCriticalSection> CLock;

///////////////////////////////////////////////////////////////////////////////
//
// struct BcamOL
//
///////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
// struct BcamOL 
// Author: Hartmut Nebelung
//------------------------------------------------------------------------------
/**
* \brief   An extended overlapped structure used for kernel-mode calls
*
* This structure is passed to DeviceIoControl-calls to the kernel-mode
* and is collected at the completion port (in case of asynchronous calls).
* 
* \see GetNewBcamOL, Transmit
*/
//------------------------------------------------------------------------------
struct Bcam::BcamOL
{
  OVERLAPPED ol;            ///< Win32-Api defined structure, needed for all calls
  FunctionCode_t function;  ///< Function code to identify the function called
  PVOID pContext;           ///< Pointer to user provided context 
};

//------------------------------------------------------------------------------
// static BcamOL* GetNewBcamOL( FunctionCode_t func, PVOID pContext )
// Author: Hartmut nebelung
//------------------------------------------------------------------------------
/**
* \brief Create a BcamOL structure on the heap and initializes it.
*
* \param     func         The function code to identify the call.
* \param     pContext     The user defined context, may be NULL.
* \return    
*
* Returns an initialized BcamOL structure.
* 
*/
//------------------------------------------------------------------------------
static BcamOL* GetNewBcamOL( FunctionCode_t func, PVOID pContext )
{
  BcamOL* p = new BcamOL;
  if (NULL == p)
    throw BcamException( Bvc::ErrorNumber( E_OUTOFMEMORY ), _T( "GetBcamOL" ));
  
  p->ol.hEvent = NULL;
  p->ol.Internal = 0;
  p->ol.InternalHigh  = 0;
  p->ol.Offset = 0;
  p->ol.OffsetHigh = 0;
  p->pContext = pContext;
  p->function = func;
  
  return p;
}

///////////////////////////////////////////////////////////////////////////////
//
// class CDriverVersionCheck
//
///////////////////////////////////////////////////////////////////////////////
class CDriverVersionCheck
{
public:
  static bool Match();
  static bool s_DriverMatched;
};

bool CDriverVersionCheck::s_DriverMatched;

///////////////////////////////////////////////////////////////////////////////
//
// class BcamUtility
//
///////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// CString BcamUtility::VideoResolutionName(DCSVideoFormat VideoFormat, DCSVideoMode VideoMode)
// Author: A.Happe
//------------------------------------------------------------------------------
/**
* \brief Looks up the video resolution.
* It is restricted to formats 0 to 2.
* If the combination is not supported a BcamException with code 
* BCAM_E_UNSUPPORTED_VIDEO_MODE is thrown.
*
* \param     VideoFormat Code of the video format
* \param     VideoMode Code of the video mode
* \return    
*
* Returns a string representation of the video resolution.
* 
* \todo      
*/
//------------------------------------------------------------------------------
CString BcamUtility::VideoResolutionName(DCSVideoFormat VideoFormat, DCSVideoMode VideoMode)
{
  switch ( VideoFormat )
  {
  case DCS_Format0:
    {
      switch ( VideoMode )
      {
      case DCS_Mode0:
        return _T( "160 x 120, YUV(4:4:4)" );
      case DCS_Mode1:
        return _T( "320 x 240, YUV(4:2:2)" );
      case DCS_Mode2:
        return _T( "640 x 480, YUV(4:1:1)" );
      case DCS_Mode3:
        return _T( "640 x 480, YUV(4:2:2)" );
      case DCS_Mode4:
        return _T( "640 x 480, RGB" );
      case DCS_Mode5:
        return _T( "640 x 480, Mono8" );
      case DCS_Mode6:
        return _T( "640 x 480, Mono16" );
      default:
        throw BcamException(BCAM_E_UNSUPPORTED_VIDEO_MODE, _T( "VideoResolutionName()" ), &VideoMode);
      }
      break;
    }
  case DCS_Format1:
    {
      switch ( VideoMode )
      {
      case DCS_Mode0:
        return _T( "800 x 600, YUV(4:2:2)" );
      case DCS_Mode1:
        return _T( "800 x 600, RGB" );
      case DCS_Mode2:
        return _T( "800 x 600, Mono8" );
      case DCS_Mode3:
        return _T( "1024 x 768, YUV(4:2:2)" );
      case DCS_Mode4:
        return _T( "1024 x 768, RGB" );
      case DCS_Mode5:
        return _T( "1024 x 768, Mono8" );
      case DCS_Mode6:
        return _T( "800 x 600, Mono16" );
      case DCS_Mode7:
        return _T( "1024 x 768, Mono16" );
      default:
        throw BcamException(BCAM_E_UNSUPPORTED_VIDEO_MODE, _T( "VideoResolutionName()" ), &VideoMode);
      }
      break;
    }
  case DCS_Format2:
    {
      switch ( VideoMode )
      {
      case DCS_Mode0:
        return _T( "1280 x 960, YUV(4:2:2)" );
      case DCS_Mode1:
        return _T( "1280 x 960, RGB" );
      case DCS_Mode2:
        return _T( "1280 x 960, Mono8" );
      case DCS_Mode3:
        return _T( "1600 x 1200, Mono16" );
      case DCS_Mode4:
        return _T( "1600 x 1200, RGB" );
      case DCS_Mode5:
        return _T( "1600 x 1200, Mono8" );
      case DCS_Mode6:
        return _T( "1280 x 960, Mono16" );
      case DCS_Mode7:
        return _T( "1600 x 1200, Mono16" );
      default:
        throw BcamException(BCAM_E_UNSUPPORTED_VIDEO_MODE, _T( "VideoResolutionName()" ), &VideoMode);
      }
      break;
    }
  default:
    throw BcamException(BCAM_E_UNSUPPORTED_VIDEO_FORMAT, _T( "VideoResolutionName()" ), &VideoFormat);
  }
}

//------------------------------------------------------------------------------
// CString BcamUtility::VideoFrameRateName(DCSVideoFrameRate VideoFrameRate)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get a string representation of the framerate.
*
* \param     VideoFrameRate
* \return    
*
* Returns a string representation of the framerate
* 
*/
//------------------------------------------------------------------------------
CString BcamUtility::VideoFrameRateName(DCSVideoFrameRate VideoFrameRate)
{
  switch ( VideoFrameRate )
  {
  case DCS_1_875fps : return _T( "1.875 fps" );
  case DCS_3_75fps : return _T( "3.75 fps" );
  case DCS_7_5fps : return _T( "7.5 fps" );
  case DCS_15fps : return _T( "15 fps" );
  case DCS_30fps : return _T( "30 fps" );
  case DCS_60fps : return _T( "60 fps" );
  case DCS_120fps : return _T( "120 fps" );
  case DCS_240fps : return _T( "240 fps" );
  default:
    throw BcamException(Bvc::ErrorNumber( E_INVALIDARG ), _T( "VideoFrameRateName" ) );
  }
}

//------------------------------------------------------------------------------
// CString BcamUtility::FeatureName(BcamFeatureID id)
// Author: A.Happe
//------------------------------------------------------------------------------
/**
* \brief Get a string representation of the feature id.
*
* \param     id Identifier of the feature.
* \return    
*
* Returns a string representation of the feature
* 
*/
//------------------------------------------------------------------------------
CString BcamUtility::FeatureName(BcamFeatureID id)
{
  switch ( id )
  {
  case FeatureID_Brightness : 
    return _T( "Brightness" );
  case FeatureID_AutoExposure : 
    return _T( "Auto Exposure" );
  case FeatureID_Sharpness :
    return _T( "Sharpness" );
  case FeatureID_WhiteBalance :
    return _T( "WhiteBalance" );
  case FeatureID_Hue :
    return _T( "Hue" );
  case FeatureID_Saturation :
    return _T( "Saturation" );
  case FeatureID_Gamma :
    return _T( "Gamma" );
  case FeatureID_Shutter :
    return _T( "Shutter" );
  case FeatureID_Gain :
    return _T( "Gain" );
  case FeatureID_Iris :
    return _T( "Iris" );
  case FeatureID_Focus :
    return _T( "Focus" );
  case FeatureID_Temparature :
    return _T( "Temperature" );
  case FeatureID_Trigger :
    return _T( "Trigger" );
  case FeatureID_Zoom  :
    return _T( "Zoom" );
  case FeatureID_Pan :
    return _T( "Pan" );
  case FeatureID_Tilt :
    return _T( "Tilt" );
  case FeatureID_OpticalFilter :
    return _T( "Optical Filter" );
  case FeatureID_CaptureSize :
    return _T( "Capture Size" );
  case FeatureID_CaptureQuality :
    return _T( "Capture Quality" );
  }
  throw BcamException(BCAM_E_INVALID_OR_UNKNOWN_FEATURE_ID, _T( "BcamUtility::FeatureName()" ) );
}

//------------------------------------------------------------------------------
// unsigned long BcamUtility::BytePerPacket(DCSVideoFormat VideoFormat, DCSVideoMode VideoMode, DCSVideoFrameRate VideoFramerate)
// Author: AHappe
//------------------------------------------------------------------------------
/**
* \brief Computes the size of the isocronous packets.
*
* \param     VideoFormat 
* The code of the video format.
* \param     VideoMode
* The code of the video mode.
* \param     VideoFramerate
* The code of the video framerate.
* \return    
*
* Returns the size of the isochronous packet. If the combination of VideoFormat, VideoMode and VideoFramerate is not
* supported a BcamException( BCAM_E_UNSUPPORTED_VIDEO_MODE ) is thrown.
* 
* \see       DCS 2.1.2, AllocateResources
* \todo      
*/
//------------------------------------------------------------------------------
unsigned long BcamUtility::BytePerPacket(DCSVideoFormat VideoFormat, DCSVideoMode VideoMode, DCSVideoFrameRate VideoFramerate)
{
    unsigned long BytePerPacket = 0;
    const unsigned long shiftval = DCS_240fps - VideoFramerate;
    switch ( VideoFormat )
    {
    case 0:
        switch ( VideoMode )
        {
        case 0:
            BytePerPacket =  3*640 >> shiftval;
            break;
        case 1:
            BytePerPacket = 2*2560 >> shiftval;
            break;
        case 2:
            BytePerPacket = 3*10240/2 >> shiftval;
            break;
        case 3:
            BytePerPacket = 2*10240 >> shiftval;
            break;
        case 4:
            BytePerPacket = 3*10240 >> shiftval;
            break;
        case 5:
            BytePerPacket = 10240 >> shiftval;
            break;
        case 6:
            BytePerPacket = 2*10240 >> shiftval;
            break;
        default: 
            throw BcamException(BCAM_E_UNSUPPORTED_VIDEO_MODE, _T( "BcamUtility::BytePerPacket()" ), &VideoMode);
        }
        if ( BytePerPacket>4000 || BytePerPacket<40)
            throw BcamException(BCAM_E_UNSUPPORTED_VIDEO_FORMAT, _T( "BcamUtility::BytePerPacket()" ), &VideoFormat);
      break;
    case 1:
        switch ( VideoMode ) 
        {
        case 0:
            if ( VideoFramerate == DCS_1_875fps )
                throw BcamException(BCAM_E_UNSUPPORTED_VIDEO_FORMAT, _T( "BcamUtility::BytePerPacket()" ), &VideoFormat);
            BytePerPacket = 2*16000 >> shiftval;
            break;
        case 1:
            if ( VideoFramerate < DCS_7_5fps || VideoFramerate > DCS_120fps )
                throw BcamException(BCAM_E_UNSUPPORTED_VIDEO_FORMAT, _T( "BcamUtility::BytePerPacket()" ), &VideoFormat);
            BytePerPacket = 3*16000 >> shiftval;
            break;
        case 2:
            if ( VideoFramerate < DCS_7_5fps  )
                throw BcamException(BCAM_E_UNSUPPORTED_VIDEO_FORMAT, _T( "BcamUtility::BytePerPacket()" ), &VideoFormat);
            BytePerPacket = 16000 >> shiftval;
            break;
        case 3:
            BytePerPacket = 2*24576 >> shiftval;
            break;
        case 4:
            BytePerPacket = 3*24576 >> shiftval;
            break;
        case 5:
            BytePerPacket = 24576 >> shiftval;
            break;
        case 6:
            if ( VideoFramerate == DCS_1_875fps  )
                throw BcamException(BCAM_E_UNSUPPORTED_VIDEO_FORMAT, _T( "BcamUtility::BytePerPacket()" ), &VideoFormat);
            BytePerPacket = 2*16000 >> shiftval;
            break;
        case 7:
            BytePerPacket = 2*24576 >> shiftval;
            break;
        default: 
            throw BcamException(BCAM_E_UNSUPPORTED_VIDEO_MODE, _T( "BcamUtility::BytePerPacket()" ), &VideoMode);
        }
        if ( BytePerPacket>4000 )
            throw BcamException(BCAM_E_UNSUPPORTED_VIDEO_FORMAT, _T( "BcamUtility::BytePerPacket()" ), &VideoFormat);
        break;
    case 2:
        switch ( VideoMode )
        {
        case 0:
            BytePerPacket = 2*40960 >> shiftval;
            break;
        case 1:
            BytePerPacket = 3*40960 >> shiftval;
            break;
        case 2:
            BytePerPacket = 40960 >> shiftval;
            break;
        case 3:
            BytePerPacket = 2*64000 >> shiftval;
            break;
        case 4:
            BytePerPacket = 3*64000 >> shiftval;
            break;
        case 5:
            BytePerPacket = 64000 >> shiftval;
            break;
        case 6:
            BytePerPacket = 2*40960 >> shiftval;
            break;
        case 7:
            BytePerPacket = 2*64000 >> shiftval;
            break;
        default: 
            throw BcamException(BCAM_E_UNSUPPORTED_VIDEO_MODE, _T( "BcamUtility::BytePerPacket()" ), &VideoMode);
        }
        if ( BytePerPacket>4000 )
            throw BcamException(BCAM_E_UNSUPPORTED_VIDEO_FORMAT, _T( "BcamUtility::BytePerPacket()" ), &VideoFormat);
        break;
    default:
        throw BcamException(BCAM_E_UNSUPPORTED_VIDEO_MODE, _T( "BcamUtility::BytePerPacket()" ), &VideoMode);
    }

    return BytePerPacket;
}


//------------------------------------------------------------------------------
// CSize BcamUtility::ImageSize(DCSVideoFormat VideoFormat, DCSVideoMode VideoMode)
// Author: A.Happe
//------------------------------------------------------------------------------
/**
* \brief Looks up the image size.
*
* \param     VideoFormat The code of the video format
* \param     VideoMode   The code of the video mode
* \return    
*
* Returns the image size of the combination of video format and mode
* 
* \see       DCAM
*/
//------------------------------------------------------------------------------
CSize BcamUtility::ImageSize(DCSVideoFormat VideoFormat, DCSVideoMode VideoMode)
{
  switch (VideoFormat)
  {
  case 0:
    switch (VideoMode)
    {
    case 0: return CSize(160, 120);
    case 1: return CSize(320, 240);
    case 2: 
    case 3: 
    case 4:
    case 5:
    case 6: return CSize(640, 480);
    default: throw BcamException( BCAM_E_UNSUPPORTED_VIDEO_MODE, _T( "BcamUtility::ImageSize" ), &VideoMode);
    }
    case 1:
      switch (VideoMode)
      {
      case 0:
      case 1:
      case 2: return CSize(800, 600);
      case 3: 
      case 4:
      case 5: return CSize(1024, 768);
      case 6: return CSize(800, 600);
      case 7: return CSize(1024, 768);
      default: throw BcamException( BCAM_E_UNSUPPORTED_VIDEO_MODE, _T( "BcamUtility::ImageSize" ), &VideoMode);
      }
      
      case 2:
        switch (VideoMode)
        {
        case 0:   
        case 1:
        case 2: return CSize(1280, 960);
        case 3:
        case 4:
        case 5: return CSize(1600, 1200);
        case 6: return CSize(1280, 960);
        case 7: return CSize(1600, 1200);
        default: throw BcamException( BCAM_E_UNSUPPORTED_VIDEO_MODE, _T( "BcamUtility::ImageSize" ), &VideoMode);
        }
        default:
          throw BcamException( BCAM_E_UNSUPPORTED_VIDEO_FORMAT, _T( "BcamUtility::ImageSize" ), &VideoFormat);
          
  }
}

//------------------------------------------------------------------------------
// DCSColorCode BcamUtility::ColorCode(DCSVideoFormat VideoFormat, DCSVideoMode VideoMode)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Look up the color code.
*
* \param     VideoFormat
* The code of the video format.
* \param     VideoMode
* The code of the video mode.
* \return    
*
* Returns the color code of the combination of video format and video mode.
* 
* \see       DCAM 2.1.2
* \todo      
*/
//------------------------------------------------------------------------------
DCSColorCode BcamUtility::ColorCode(DCSVideoFormat VideoFormat, DCSVideoMode VideoMode)
{
  switch (VideoFormat)
  {
  case 0:
    switch (VideoMode)
    {
    case 0:
      return DCSColor_YUV8_4_4_4;
    case 1:
      return DCSColor_YUV8_4_2_2;
    case 2:
      return DCSColor_YUV8_4_1_1;
    case 3:
      return DCSColor_YUV8_4_2_2;
    case 4:
      return DCSColor_RGB8;
    case 5:
      return DCSColor_Mono8;
    case 6:
      return DCSColor_Mono16;
    default: throw BcamException(BCAM_E_UNSUPPORTED_VIDEO_MODE, _T( "BcamUtility::ColorMode" ), &VideoMode);
    }
    
    case 1:
      switch (VideoMode)
      {
      case 0: return DCSColor_YUV8_4_2_2;
      case 1: return DCSColor_RGB8;
      case 2: return DCSColor_Mono8;
      case 3: return DCSColor_YUV8_4_2_2;
      case 4: return DCSColor_RGB8;
      case 5: return DCSColor_Mono8;
      case 6:
      case 7: return DCSColor_Mono16;
      default: throw BcamException( BCAM_E_UNSUPPORTED_VIDEO_MODE, _T( "BcamUtility::ColorMode" ), &VideoMode);
      }
      case 2:
        switch (VideoMode)
        {
        case 0: return DCSColor_YUV8_4_2_2;
        case 1: return DCSColor_RGB8;
        case 2: return DCSColor_Mono8;
        case 3: return DCSColor_YUV8_4_2_2;
        case 4: return DCSColor_RGB8;
        case 5: return DCSColor_Mono8;
        case 6: return DCSColor_Mono16;
        case 7: return DCSColor_Mono16;
        default: throw BcamException( BCAM_E_UNSUPPORTED_VIDEO_MODE, _T( "BcamUtility::ColorMode" ), &VideoMode);
        }
        default:
          throw BcamException( BCAM_E_UNSUPPORTED_VIDEO_FORMAT, _T( "BcamUtility::ColorMode" ), &VideoFormat);
  }
}

//------------------------------------------------------------------------------
// CString BcamUtility::ColorCodeName(DCSColorCode ColorCode)
// Author: A.Happe
//------------------------------------------------------------------------------
/**
* \brief Get a string representation of the color code.
*
* \param     ColorCode
* \return    
*
* Returns a string representation of the color code.
*
* \throw BcamException BCAM_E_UNSUPPORTED_COLOR_CODE when passing an invalid color code to the function
* 
*/
//------------------------------------------------------------------------------
CString BcamUtility::ColorCodeName(DCSColorCode ColorCode)
{
  switch ( ColorCode )
  {
  case DCSColor_Mono8       : return _T( "Mono8" );
  case DCSColor_YUV8_4_1_1  : return _T( "YUV(4:1:1)" );
  case DCSColor_YUV8_4_2_2  : return _T( "YUV(4:2:2)" );
  case DCSColor_YUV8_4_4_4  : return _T( "YUV(4:4:4)" );
  case DCSColor_Mono16      : return _T( "Mono16(unsigned)" );
  case DCSColor_SMono16     : return _T( "Mono16(signed)" );
  case DCSColor_RGB8        : return _T( "RGB8" );
  case DCSColor_RGB16       : return _T( "RGB16(unsigned)" );
  case DCSColor_SRGB16      : return _T( "RGB16(signed)" );
  case DCSColor_Raw8        : return _T( "Raw8" );
  case DCSColor_Raw16       : return _T( "Raw16" );
  default:  
    if (DCSColor_VendorSpecific0 <= ColorCode && DCSColor_VendorSpecific127 >= ColorCode)
    {
      int i = ColorCode - DCSColor_VendorSpecific0;
      CString codename;
      codename.Format( _T("Vendor Specific Color Code %i" ), i );
      
      return codename;
    }
    else
      throw BcamException( BCAM_E_UNSUPPORTED_COLOR_CODE, _T( "BcamUtility::ColorCodeName" ), &ColorCode);
  }
}

//------------------------------------------------------------------------------
// unsigned short BcamUtility::BitsPerPixel(DCSColorCode ColorCode)
// Author: A.Happpe
//------------------------------------------------------------------------------
/**
* \brief Looks up the pixel size of a color code
*
* \param     ColorCode
* \return    
*
* Returns the number of bits used for a pixel.
* If the color code is vendor specific zero is returned.
*
* \throw BcamException BCAM_E_UNSUPPORTED_COLOR_CODE when passing an invalid color code to the function
*
* 
* \see       DCAM
*/
//------------------------------------------------------------------------------
unsigned short BcamUtility::BitsPerPixel(DCSColorCode ColorCode)
{
  switch ( ColorCode )
  {
  case DCSColor_Mono8       : return 8;
  case DCSColor_YUV8_4_1_1  : return 12;
  case DCSColor_YUV8_4_2_2  : return 16;
  case DCSColor_YUV8_4_4_4  : return 24;
  case DCSColor_Mono16      : return 16;
  case DCSColor_SMono16     : return 16;
  case DCSColor_RGB8        : return 24;
  case DCSColor_RGB16       : return 48;
  case DCSColor_SRGB16      : return 48;
  case DCSColor_Raw8        : return 8;
  case DCSColor_Raw16       : return 16;
  default:                    
    if (DCSColor_VendorSpecific0 <= ColorCode && DCSColor_VendorSpecific127 >= ColorCode)
      return 0;
    else
      throw BcamException( BCAM_E_UNSUPPORTED_COLOR_CODE, _T( "BcamUtility::BitsPerPixel" ), &ColorCode);
  }
}


//------------------------------------------------------------------------------
// bool BcamUtility::IsFullDeviceName(CString DeviceName)
// Author: 
// Date: 26.10.2002
//------------------------------------------------------------------------------
/**
 * \brief is a given device name a "full" device name, i.e. does it contain the driver interface GUID?
 * 
 * Check if a given device name is a full device name (as it is passed in by the plug and play manager) 
 * or if it is a friendly device name not containing the driver interface GUID
 *
 * \param     DeviceName device name to check
 * \return    true, if the device name is not a friendly device name
 *
 */
//------------------------------------------------------------------------------
bool BcamUtility::IsFullDeviceName(CString DeviceName)
{
  int first = DeviceName.Find('#');
  assert(first > 0);
  int l = DeviceName.GetLength();
  int second  = DeviceName.Right(l - (first+1)).Find('#') + first + 1;
  return(second > first);
}

//------------------------------------------------------------------------------
// CString BcamUtility::FullDeviceName(CString DeviceName)
// Author: 
// Date: 26.10.2002
//------------------------------------------------------------------------------
/**
 * \brief given a friendly device name return a full device name
 *
 * The Bcam API is dealing with friendly device names. Friendly device names don't contain 
 * the driver's interface GUID. This function converts a friendly device name into a full device name, 
 * as it is used by the system. 
 *
 * \param     DeviceName a friendly device name
 * \return    the full device name
 *
 */
//------------------------------------------------------------------------------
CString BcamUtility::FullDeviceName(CString DeviceName)
{

  if (!IsFullDeviceName(DeviceName))
  {
    struct _GUID guid = GUID_FILE_DEVICE_BCAM_1394;
    CString guidString;
    guidString.Format( _T("#{%08lX-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}"), 
      guid.Data1, guid.Data2, guid.Data3, 
      guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], 
      guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]); 
    return _T( "\\\\?\\1394#" ) + DeviceName + guidString;
  }
  else return DeviceName;
}


//------------------------------------------------------------------------------
// CString BcamUtility::FriendlyDeviceName(CString DeviceName)
// Author: 
// Date: 26.10.2002
//------------------------------------------------------------------------------
/**
 * \brief Retrieves the friendly device name for a given full device name
 *
 * The Bcam API is dealing with friendly device names. Friendly device names don't contain 
 * the driver's interface GUID. This function converts a full device named used by the system into a 
 * friendly device name
 *
 *  \param   DeviceName the full device name
 * \return   the friendly device name 
 *
 */
//------------------------------------------------------------------------------
CString BcamUtility::FriendlyDeviceName(CString DeviceName)
{
  if (IsFullDeviceName(DeviceName))
  {
    int first = DeviceName.Find('#');
    assert(first > 0);
    int l = DeviceName.GetLength();
    int second = DeviceName.Right(l - (first+1)).Find('#') + first + 1;
    assert(second > 0);
    int third = DeviceName.Right(l - (second+1)).Find('#') + second + 1;
    assert(third > 0);
    assert(DeviceName[third+1] == '{');
    return DeviceName.Left(third).Right(third-(first+1));
  }
  else return DeviceName;
}


///////////////////////////////////////////////////////////////////////////////
//
// class CBcam
//
///////////////////////////////////////////////////////////////////////////////

// static member initialization
HDEVNOTIFY CBcam::s_hDevInterfaceNotify = INVALID_HANDLE_VALUE;
CBcam::BcamMap_t CBcam::s_BcamMap;
HWND CBcam::s_hWndNotify = NULL;
HWND CBcam::s_hWndMsgOnly = NULL;
WNDPROC CBcam::s_lpPrevWndFunc = NULL;
ULONG CBcam::s_CompletionKey = 0;
//CThread CBcam::s_MessageLoopThread;
CBcam::CCleanUp CBcam::s_CleanUp;


///////////////////////////////////////////////////////////////////////////////
//
// CBcam::CBoolControl 
//
//

//------------------------------------------------------------------------------
// bool CBcam::CBoolControl::operator ()
// Author: 
//------------------------------------------------------------------------------
/**
 * \brief Inquire the feature is on or off.
 *
 * \retval  true if it is on    
 * \retval  false if it is not on
 */
//------------------------------------------------------------------------------
bool CBcam::CBoolControl::operator ()()
{
  throw BcamException( Bvc::ErrorNumber( E_NOTIMPL ), _T( "CBcam::CBoolControl::operator()" ) );
}

//------------------------------------------------------------------------------
// void CBcam::CBoolControl::operator =( bool OnOff )
// Author: 
//------------------------------------------------------------------------------
/**
 * \brief Turn a feature on or off
 *
 * \param     OnOff  true is on, false is off
 */
//------------------------------------------------------------------------------
void CBcam::CBoolControl::operator =( bool /*OnOff*/ )
{
  throw BcamException( Bvc::ErrorNumber( E_NOTIMPL ), _T( "CBcam::CBoolControl::operator=()" ) );
}
///////////////////////////////////////////////////////////////////////////////
//
// class CBcam::CScalarProperty
//
//

//------------------------------------------------------------------------------
// CBcam::CScalarProperty::CScalarProperty(  FunctionCode_t AsyncFuncCodeRaw,  FunctionCode_t AsyncFuncCodeAbs,  BcamFeatureID FuncId,  CBcam* pBcam ) : m_FunctionId( FuncId),  m_pBcam( FuncId, pBcam ), AbsControl( FuncId, mAbsControl, pBcam ), AutoMode( FuncId, mAutoMode, pBcam ), OnOff( FuncId, mOnOff, pBcam ), Raw( AsyncFuncCodeRaw, FuncId, pBcam ), Abs( AsyncFuncCodeAbs, FuncId, pBcam ){  /* NOP */ }
// Author: 
//------------------------------------------------------------------------------
/**
 * \brief Contruct a scalar property.
 *
 * \param     AsyncFuncCodeRaw Function code for asynchronous calls to raw register
 * \param     AsyncFuncCodeAbs Function code for asynchronous calls to absolute register
 * \param     FuncId Feature identifier
 * \param     pBcam Backpointer to the communication object
 */
//------------------------------------------------------------------------------
CBcam::CScalarProperty::CScalarProperty( 
                                        FunctionCode_t AsyncFuncCodeRaw, 
                                        FunctionCode_t AsyncFuncCodeAbs, 
                                        BcamFeatureID FuncId, 
                                        CBcam* pBcam ) 
: m_FunctionId( FuncId), 
  m_pBcam( pBcam ),
  AbsControl( FuncId, mAbsControl, pBcam ),
  AutoMode( FuncId, mAutoMode, pBcam  ),
  OnOff( FuncId, mOnOff, pBcam  ),
  Raw( AsyncFuncCodeRaw, FuncId, pBcam ),
  Abs( AsyncFuncCodeAbs, FuncId, pBcam )
{ 
  /* NOP */ 
}

//------------------------------------------------------------------------------
// bool CBcam::CScalarProperty::IsSupported(Supported_t Inquiry)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Inquire wether a feature is supported.
*
* \param     Inquiry Code of the requested feature.
* \return    
*
* Returns whether the feature is supported by the camera
* If the feature is not specified in DCAM then a BcamException with the
* error code BCAM_E_INVALID_INQUIRY is thrown.
* \exception BcamException The value of \c ::GetLastError() is thrown
*/
//------------------------------------------------------------------------------
bool CBcam::CScalarProperty::IsSupported(Supported_t Inquiry)
{
  ArgQueryDCSFeatureInq arg;
  arg.FeatId = m_FunctionId;
  ResQueryDCSFeatureInq res;
  m_pBcam->TryTransmit( _T( "CBcam::CScalarProperty::IsSupported()" ), IOCTL_BCAM_QUERY_DCS_FEATURE_INQ, &arg, sizeof(arg), &res, sizeof(res));
  switch ( Inquiry )
  {
  case inqPresent:
    return res.Scalar.Presence_Inq;
  case inqAbsControl:
    return res.Scalar.Abs_Control_Inq;
  case inqOnePush:
    return res.Scalar.One_Push_Inq;
  case inqReadOut:
    return res.Scalar.Read_Out_Inq;
  case inqOnOff:
    return res.Scalar.On_Off_Inq;
  case inqAuto:
    return res.Scalar.Auto_Inq;
  case inqManual:
    return res.Scalar.Manual_Inq;
  default:
    throw BcamException( BCAM_E_INVALID_INQUIRY, _T( "CBcam::CScalarProperty::IsSupported()" ), &Inquiry);
    
  }
  // return true;
}

//------------------------------------------------------------------------------
// void CBcam::CScalarProperty::OnePush()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Activate the OnePush feature.
* \exception BcamException The value of \c ::GetLastError() is thrown
*/
//------------------------------------------------------------------------------
void CBcam::CScalarProperty::OnePush()
{
  ArgSetDCSFeatureCSR arg;
  ZeroMemory(&arg, sizeof(arg));
  arg.FeatId = m_FunctionId;
  arg.FeatCSRDescriptor.Scalar.One_Push = 1;
  arg.Mask = DCS_FEATURE_CSR_SET_ONE_PUSH;
  m_pBcam->TryTransmit( _T( "CBcam::CScalarProperty::OnePush()" ), IOCTL_BCAM_SET_DCS_FEATURE_CSR, &arg, sizeof(arg) );
}

///////////////////////////////////////////////////////////////////////////////
// class CBcam::CScalarProperty::CBoolControl

//------------------------------------------------------------------------------
// CBcam::CScalarProperty::CBoolControl::CBoolControl( BcamFeatureID FuncId, Mode_t Mode, CBcam* pBcam ) : m_pBcam( pBcam ), m_FunctionId ( FuncId), m_Mode (Mode)
// Author: A. Happe
//------------------------------------------------------------------------------
/**
* \brief Create a switch for a part of a DCAM feature (e.g. Auto mode of Brightness)
*
* \param     FuncId Identifies the feature
* \param     Mode   Identifies the part of the feature
* \param     pBcam  Backpointer to the communication object 
*/
//------------------------------------------------------------------------------
CBcam::CScalarProperty::CBoolControl::CBoolControl( BcamFeatureID FuncId, Mode_t Mode, CBcam* pBcam ) 
: m_pBcam( pBcam ),
m_FunctionId ( FuncId),
m_Mode (Mode)
{
  /* NOP */ 
}

//------------------------------------------------------------------------------
// void CBcam::CScalarProperty::CBoolControl::operator=(bool Value)
// Author: Hartmut Nebelung
//------------------------------------------------------------------------------
/**
* \brief Set the value of the feature synchronously.
*
* \param     Value  The value to set, true means on and false off.
* 
* \throw BcamException The value of ::GetLastError() is thrown
*
* \b Example 
* \code
* m_pBcam->Brightness.AutoMode = true;
* m_pBcam->Brightness.AutoMode.operator =( true );
* \endcode
*/
//------------------------------------------------------------------------------
void CBcam::CScalarProperty::CBoolControl::operator=(bool Value)
{
  ArgSetDCSFeatureCSR arg;
  ZeroMemory(&arg, sizeof(arg));
  arg.FeatId = m_FunctionId;
  switch ( m_Mode )
  {
  case mOnOff:
    arg.FeatCSRDescriptor.Scalar.ON_OFF = Value;
    arg.Mask = DCS_FEATURE_CSR_SET_ON_OFF;
    break;
  case mAutoMode:
    arg.FeatCSRDescriptor.Scalar.A_M_Mode = Value;
    arg.Mask = DCS_FEATURE_CSR_SET_A_M_MODE;
    break;
  case mAbsControl:
    arg.FeatCSRDescriptor.Scalar.Abs_Control = Value;
    arg.Mask = DCS_FEATURE_CSR_SET_ABS_CONTROL;
    break;
  default:
    assert(false);
  }
  m_pBcam->TryTransmit( _T( "CBcam::CScalarProperty::CBoolControl::operator=()" ), IOCTL_BCAM_SET_DCS_FEATURE_CSR, &arg, sizeof(arg) );
}


//------------------------------------------------------------------------------
// bool CBcam::CScalarProperty::CBoolControl::operator()()
// Author: Hartmut Nebelung
//------------------------------------------------------------------------------
/**
* \brief Get the value of the feature synchronously.
*
* \return    
*
* Returns the current value. True means on and false off.
* 
* \throw BcamException The value of ::GetLastError() is thrown
*
* \b Example
* \code
* bool IsOn;
* IsOn = m_pBcam->Brightness.AutoMode();
* IsOn = m_pBcam->Brightness.AutoMode.operator()();
* \endcode
*/
//------------------------------------------------------------------------------
bool CBcam::CScalarProperty::CBoolControl::operator()()
{
  bool retVal = false;
  ArgGetDCSFeatureCSR arg;
  ZeroMemory(&arg, sizeof(arg));
  arg.FeatId = m_FunctionId;
  ResGetDCSFeatureCSR res;
  m_pBcam->TryTransmit( _T( "CBcam::CScalarProperty::CBoolControl::Operator()()" ),
    IOCTL_BCAM_GET_DCS_FEATURE_CSR, &arg, sizeof(arg), &res, sizeof(res) );
  switch ( m_Mode )
  {
  case mOnOff:
    retVal = res.Scalar.ON_OFF;
    break;
  case mAutoMode:
    retVal = res.Scalar.A_M_Mode;
    break;
  case mAbsControl:
    retVal = res.Scalar.Abs_Control;
    break;
  default:
    assert(false);
  }
  return retVal;
}

///////////////////////////////////////////////////////////////////////////////
// class CBcam::CScalarProperty::CRawRegister

//------------------------------------------------------------------------------
// CBcam::CScalarProperty::CRawRegister::CRawRegister( FunctionCode_t FuncCode, BcamFeatureID FuncId, CBcam* pBcam ): m_pBcam(pBcam),  m_FunctionId( FuncId), m_FunctionCode( FuncCode )
// Author: A. Happe
//------------------------------------------------------------------------------
/**
* \brief Create a raw register object 
*
* for accessing  a DCAM feature (e.g. Brightness)
*
* \param     FuncCode Function code used for asynchronous calls
* \param     FuncId   Feature identifier
* \param     pBcam    Backpointer to the communication object
*/
//------------------------------------------------------------------------------
CBcam::CScalarProperty::CRawRegister::CRawRegister( FunctionCode_t FuncCode, BcamFeatureID FuncId, CBcam* pBcam )
: m_pBcam(pBcam), 
m_FunctionId( FuncId),
m_FunctionCode( FuncCode )
{
  /* NOP */
}

//------------------------------------------------------------------------------
// unsigned long CBcam::CScalarProperty::CRawRegister::Min()
// Author: Hartmut Nebelung
//------------------------------------------------------------------------------
/**
* \brief Get the minimum of a DCAM feature synchronously.
*
* \return    
*
* Returns the minimum value.
* 
* \throw BcamException The value of \c ::GetLastError() is thrown
*
* \b Example
* \code
* unsigned long value;
* value = m_pBcam->Brightness.Raw.Min();
* \endcode
*/
//------------------------------------------------------------------------------
unsigned long CBcam::CScalarProperty::CRawRegister::Min()
{
  ArgQueryDCSFeatureInq arg;
  arg.FeatId = m_FunctionId;
  ResQueryDCSFeatureInq res;
  m_pBcam->TryTransmit( _T( "CBcam::CScalarProperty::CRawRegister::Min()" ), IOCTL_BCAM_QUERY_DCS_FEATURE_INQ, &arg, sizeof(arg), &res, sizeof(res) );
  return res.Scalar.Min_Value;
}

//------------------------------------------------------------------------------
// unsigned long CBcam::CScalarProperty::CRawRegister::Max()
// Author: Hartmut Nebelung
//------------------------------------------------------------------------------
/**
* \brief Get the maximum of a DCAM feature synchronously.
*
* \return    
*
* Returns the maximum value.
* 
* \throw BcamException The value of \c ::GetLastError() is thrown.
*
* \b Example
* \code
* unsigned long value;
* value = m_pBcam->Brightness.Raw.Max();
* \endcode
*/
//------------------------------------------------------------------------------
unsigned long CBcam::CScalarProperty::CRawRegister::Max()
{
  ArgQueryDCSFeatureInq arg;
  arg.FeatId = m_FunctionId;
  ResQueryDCSFeatureInq res;
  m_pBcam->TryTransmit( _T( "CBcam::CScalarProperty::CRawRegister::Max()" ), IOCTL_BCAM_QUERY_DCS_FEATURE_INQ, &arg, sizeof(arg), &res, sizeof(res) );
  return res.Scalar.Max_Value;
}

//------------------------------------------------------------------------------
// void CBcam::CScalarProperty::CRawRegister::operator=(unsigned long Value)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Set the value of a DCAM feature synchronously
*
* \param     Value Value to set
*
* \throw BcamException The value of ::GetLastError() is thrown
*
* \b Example
* \code
* const unsigned value=12;
* m_pBcam->Brightness.Raw = value;
* m_pBcam->Brightness.Raw.operator=( value );
* \endcode
*/
//------------------------------------------------------------------------------
void CBcam::CScalarProperty::CRawRegister::operator=(unsigned long Value)
{
  ArgSetDCSFeatureCSR arg;
  ZeroMemory(&arg, sizeof(arg));
  arg.FeatId = m_FunctionId;
  arg.FeatCSRDescriptor.Scalar.Value = Value;
  arg.Mask = DCS_FEATURE_CSR_SET_VALUE;
  m_pBcam->TryTransmit( _T( "CBcam::CScalarProperty::CRawRegister::operator=()" ), IOCTL_BCAM_SET_DCS_FEATURE_CSR, &arg, sizeof(arg) );
}

//------------------------------------------------------------------------------
// unsigned long CBcam::CScalarProperty::CRawRegister::operator()()
// Author: A.Happe
//------------------------------------------------------------------------------
/**
* \brief Get the value of a DCAM feature synchronously.
*
* \return    
*
* Returns the current value.
*
* \throw BcamException The value of \c ::GetLastError() is thrown
*
* \b Example
* \code
* unsigned value;
* value = m_pBcam->Brightness.Raw();
* value = m_pBcam->Brightness.Raw.operator()();
* \endcode
*/
//------------------------------------------------------------------------------
unsigned long CBcam::CScalarProperty::CRawRegister::operator()()
{
  ArgGetDCSFeatureCSR arg;
  ZeroMemory(&arg, sizeof(arg));
  arg.FeatId = m_FunctionId;
  ResGetDCSFeatureCSR res;
  m_pBcam->TryTransmit( _T( "CBcam::CScalarProperty::CRawRegister::Operator()()" ), IOCTL_BCAM_GET_DCS_FEATURE_CSR, &arg, sizeof(arg), &res, sizeof(res) );
  return res.Scalar.Value;
}

//------------------------------------------------------------------------------
// void CBcam::CScalarProperty::CRawRegister::SetAsync(unsigned long Value, void *pContext)
// Author: A.Happe
//------------------------------------------------------------------------------
/**
* \brief Set the value of a DCAM feature asynchronously.
*
* \param     Value     Value to set
* \param     *pContext Context that will be returned. (optional)
* \throw BcamException The value of \c ::GetLastError() is thrown
*
* \b Example
* \code
* const unsigned long newVal=12;
* m_pBcam->Brightness.Raw.SetAsync( newVal );
* m_pBcam->Brightness.Raw.SetAsync( newVal, &aInfoBlock );
* \endcode
*/
//------------------------------------------------------------------------------
void CBcam::CScalarProperty::CRawRegister::SetAsync(unsigned long Value, void *pContext)
{
  TRACE( _T( "BCAMApi: CBcam::CScalarProperty::CRawRegister::SetAsync(%lu, 0x%p)\n" ), Value, pContext);
  
  BcamOL *pOL = GetNewBcamOL(m_FunctionCode, pContext );
  ArgSetDCSFeatureCSR arg;
  ZeroMemory( &arg, sizeof arg );
  arg.FeatId = m_FunctionId;
  arg.FeatCSRDescriptor.Scalar.Value = Value;
  arg.Mask = DCS_FEATURE_CSR_SET_VALUE;
  if (! m_pBcam->Transmit( IOCTL_BCAM_SET_DCS_FEATURE_CSR, &arg, sizeof arg, pOL ))
  {
    DWORD error = ::GetLastError();
    if (error != ERROR_IO_PENDING)
      throw BcamException( ::GetLastError(), _T( "CBcam::CScalarProperty::CRawRegister::SetAsync()" ) );
  }
}

///////////////////////////////////////////////////////////////////////////////
// class CBcam::CScalarProperty::CAbsValue

//------------------------------------------------------------------------------
// CBcam::CScalarProperty::CAbsValue::CAbsValue( FunctionCode_t FuncCode, BcamFeatureID FuncId, CBcam* pBcam ) : m_pBcam( pBcam ), m_FunctionId( FuncId), m_FunctionCode( FuncCode ){  /* NOP */}double CBcam::CScalarProperty::CAbsValue::Min()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Create an absolute register object
*
* for accessing a DCAM feature using absolute values (e.g. Gain in dB).
*
* \param     FuncCode  Function code used for asynchronous calls
* \param     FuncId    Feature identifier
* \param     pBcam     Backpointer to the communication object
*/
//------------------------------------------------------------------------------
CBcam::CScalarProperty::CAbsValue::CAbsValue( FunctionCode_t FuncCode, BcamFeatureID FuncId, CBcam* pBcam ) 
: m_pBcam( pBcam ),
m_FunctionId( FuncId),
m_FunctionCode( FuncCode )
{ 
  /* NOP */
}

//------------------------------------------------------------------------------
// double CBcam::CScalarProperty::CAbsValue::Min()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the minimum of a DCAM feature synchronously.
*
* \return    
*
* Returns the minimum value. 
* 
* \throw BcamException the value of ::GetLastError() is thrown.
*
* \b Example
* \code 
* double value;
* value = m_pBcam->Gain.Abs.Min();
* \endcode
*/
//------------------------------------------------------------------------------
double CBcam::CScalarProperty::CAbsValue::Min()
{
  throw BcamException(Bvc::ErrorNumber( E_NOTIMPL ), _T( "CBcam::CScalarProperty::CAbsValue::Min()" ) );
}

//------------------------------------------------------------------------------
// double CBcam::CScalarProperty::CAbsValue::Max()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the maximum of a DCAM feature synchronously.
*
* \return    
*
* Returns the maximum value.
* 
* \throw BcamException The value of ::GetLastError() is thrown.
* 
* \b Example
* \code
* double value;
* value = m_pBcam->Gain.Abs.Max();
* \endcode
*/
//------------------------------------------------------------------------------
double CBcam::CScalarProperty::CAbsValue::Max()
{
  throw BcamException(Bvc::ErrorNumber( E_NOTIMPL ), _T( "CBcam::CScalarProperty::CAbsValue::Max()" ) );
}

//------------------------------------------------------------------------------
// void CBcam::CScalarProperty::CAbsValue::operator=(double Value)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Set the value of a DCAM feature synchronously.
*
* \param     Value Value to set
* \throw BcamException The value of ::GetLastError() is thrown
* 
* \b Example
* \code
* const double value = 3.5; // dB
* m_pBcam->Gain.Abs = value;
* m_pBcam->Gain.Abs.operatator=( value );

* \endcode
*/
//------------------------------------------------------------------------------
void CBcam::CScalarProperty::CAbsValue::operator=(double /*Value*/)
{
  throw BcamException( Bvc::ErrorNumber( E_NOTIMPL ), _T( "CBcam::CScalarProperty::CAbsValue::operator=()" ) );
}

//------------------------------------------------------------------------------
// double CBcam::CScalarProperty::CAbsValue::operator()()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the value of a DCAM feature synchronously.
*
* \return    
*
* Returns the current value
* 
* \throw BcamException The value of \c ::GetLastError() is thrown
* 
*  \b Example
* \code
* double value;
* value = m_pBcam->Gain.Abs()
* value = m_pBcam->Gain.Abs.operator()();
* \endcode
*/
//------------------------------------------------------------------------------
double CBcam::CScalarProperty::CAbsValue::operator()()
{
  throw BcamException( Bvc::ErrorNumber( E_NOTIMPL ), _T( "CBcam::CScalarProperty::CAbsValue::operator()()" ) );
}

//------------------------------------------------------------------------------
// void CBcam::CScalarProperty::CAbsValue::SetAsync(double Value, void *pContext)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Set the value of a DCAM feature asynchronously.
*
* \param     Value
* \param     *pContext
* \throw BcamException The value of ::GetLastError() is thrown.
*
* \b Example
* \code
* const double value = 3.5; // dB
* m_pBcam->Gain.Abs.SetAsync( value );
* m_pBcam->Gain.Abs.SetAsync( value, &InfoBlock );
* \endcode
*/
//------------------------------------------------------------------------------
void CBcam::CScalarProperty::CAbsValue::SetAsync(double /*Value*/, void * /*pContext*/)
{
  //  BcamOL *pOL = GetNewBcamOL( m_FunctionCode, pContext );
  //  ArgSetDCSFeatureCSR arg;
  //  ZeroMemory( &arg, sizeof arg );
  //  arg.FeatId = m_FunctionId;
  //  arg.FeatCSRDescriptor. = Value;
  //  arg.Mask = DCS_FEATURE_SET_ABS_CONTROL;
  //  if (! m_pBcam->Transmit( IOCTL_BCAM_SET_DCS_FEATURE_CSR, &arg, sizeof arg, pOL ))
  //    throw BcamException( ::GetLastError(), "CBcam::CScalarProperty::CAbsRegister::SetAsync()" );
  throw BcamException( Bvc::ErrorNumber( E_NOTIMPL ), _T( "CBcam::CScalarProperty::CAbsValue::SetAsync()" ) );

}

//------------------------------------------------------------------------------
// void CBcam::CScalarProperty::Save(const CPropertyBagPtr ptrBag)
// Author: 
// Date: 04.09.2002
//------------------------------------------------------------------------------
/**
 * Save settings to a property bag
 *
 * \param     ptrBag The property Bag
 */
//------------------------------------------------------------------------------
void CBcam::CScalarProperty::Save(const CPropertyBagPtr ptrBag)
{
  if ( IsSupported(inqOnOff) )
  {
    bool onOff = OnOff();
    ptrBag->WriteBool( _T( "OnOff" ), onOff);
    if ( ! onOff )
      return;  // feature is disabled, nothing left to save
  }

  if ( IsSupported(inqAuto) )
  {
    bool autoMode = AutoMode();
    ptrBag->WriteBool( _T( "AutoMode" ), autoMode);
    if ( autoMode )
      return; // feature is in auto mode, nothing left to save
  }

  if ( IsSupported(inqManual) && IsSupported(inqReadOut) )
  {
    bool absControl = false;
    if ( IsSupported(inqAbsControl) )
    {
      absControl = AbsControl();
      ptrBag->WriteBool( _T( "AbsControl" ), absControl);
    }
    if ( absControl )
      ptrBag->WriteFloat( _T( "AbsValue" ), (float) Abs());
    else
      ptrBag->WriteLong( _T( "RawValue" ), Raw());
  }

}

//------------------------------------------------------------------------------
// void CBcam::CScalarProperty::Restore(const CPropertyBagPtr ptrBag)
// Author: 
// Date: 04.09.2002
//------------------------------------------------------------------------------
/**
 * Restore settings from a property bag
 *
 * \param     ptrBag The property bag
 */
//------------------------------------------------------------------------------
void CBcam::CScalarProperty::Restore(const CPropertyBagPtr ptrBag)
{
  if ( IsSupported(inqOnOff) )
  {
    bool onOff = ptrBag->ReadBool( _T( "OnOff" ));
    OnOff = onOff;
    if ( ! onOff )
      return;  // feature is disabled, nothing to restore
  }

  if ( IsSupported(inqAuto) )
  {
    bool autoMode = ptrBag->ReadBool( _T( "AutoMode" ) );
    AutoMode = autoMode;
    if ( autoMode )
      return; // feature is in auto mode, noting to restore
  }

  if ( IsSupported(inqManual) && IsSupported(inqReadOut) )
  {
    bool absControl = false;
    if ( IsSupported(inqAbsControl) )
    {
      absControl = ptrBag->ReadBool( _T( "AbsControl" ));
    }
    if ( absControl )
      Abs = ptrBag->ReadFloat( _T( "AbsValue" ) );
    else
      Raw = ptrBag->ReadLong( _T( "RawValue" ) );
  }
}


///////////////////////////////////////////////////////////////////////////////
//
// class CBcam::CWhiteBalance
//
//

//------------------------------------------------------------------------------
// CBcam::CWhiteBalance::CWhiteBalance() 
//------------------------------------------------------------------------------
// Author: 
//------------------------------------------------------------------------------
/**
 * \brief Contruct a white balance feature
 *
 */
//------------------------------------------------------------------------------
CBcam::CWhiteBalance::CWhiteBalance()
: 
  AbsControl( mAbsControl, This() ),
  AutoMode( mAutoMode, This() ),
  OnOff( mOnOff, This() )
{ 
  /* NOP */ 
}

//------------------------------------------------------------------------------
// bool CBcam::CWhiteBalance::IsSupported(Supported_t Inquiry)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Inquire wether a feature is supported 
*
* \param     Inquiry Code of the requested feature.
* \return    
*
* Returns whether the feature is supported by the camera
* \exception BcamException The value of \c ::GetLastError() is thrown
* If the feature is not specified in DCAM then a BcamException with the
* error code BCAM_E_INVALID_INQUIRY is thrown.
* 
*/
//------------------------------------------------------------------------------
bool CBcam::CWhiteBalance::IsSupported(Supported_t Inquiry)
{
  ArgQueryDCSFeatureInq arg;
  arg.FeatId = FeatureID_WhiteBalance;
  ResQueryDCSFeatureInq res;
  This()->TryTransmit( _T( "CBcam::CWhiteBalance::IsSupported()" ),
    IOCTL_BCAM_QUERY_DCS_FEATURE_INQ, &arg, sizeof(arg), &res, sizeof(res) );
  switch ( Inquiry )
  {
  case inqPresent:
    return res.Scalar.Presence_Inq;
  case inqAbsControl:
    return res.Scalar.Abs_Control_Inq;
  case inqOnePush:
    return res.Scalar.One_Push_Inq;
  case inqReadOut:
    return res.Scalar.Read_Out_Inq;
  case inqOnOff:
    return res.Scalar.On_Off_Inq;
  case inqAuto:
    return res.Scalar.Auto_Inq;
  case inqManual:
    return res.Scalar.Manual_Inq;
  default:
    throw BcamException( BCAM_E_INVALID_INQUIRY, _T( "CBcam::WhiteBalance::IsSupported()" ), &Inquiry);
    
  }
}

//------------------------------------------------------------------------------
// void CBcam::CWhiteBalance::OnePush()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Activate the OnePush feature.
* \exception BcamException The value of \c ::GetLastError() is thrown
*/
//------------------------------------------------------------------------------
void CBcam::CWhiteBalance::OnePush()
{
  ArgSetDCSFeatureCSR arg;
  ZeroMemory(&arg, sizeof(arg));
  arg.FeatId = FeatureID_WhiteBalance;
  arg.FeatCSRDescriptor.Scalar.One_Push = 1;
  arg.Mask = DCS_FEATURE_CSR_SET_ONE_PUSH;
  This()->TryTransmit( _T( "CBcam::CWhiteBalance::OnePush()" ), IOCTL_BCAM_SET_DCS_FEATURE_CSR, &arg, sizeof(arg) );
}

///////////////////////////////////////////////////////////////////////////////
// class CBcam::CWhiteBalance::CBoolControl

//------------------------------------------------------------------------------
// CBcam::CWhiteBalance::CBoolControl::CBoolControl( Mode_t Mode, CBcam* pBcam )
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Create a switch for a part of a DCAM feature (e.g. Auto mode of Brightness)
*
* \param     FuncId Identifies the feature
* \param     Mode   Identifies the part of the feature
* \param     pBcam  Backpointer to the communication object 
*/
//------------------------------------------------------------------------------
CBcam::CWhiteBalance::CBoolControl::CBoolControl(Mode_t mode,  CBcam* pBcam ) 
: m_pBcam( pBcam ),
  m_Mode(mode)
{
  /* NOP */ 
}

//------------------------------------------------------------------------------
// void CBcam::CWhiteBalance::CBoolControl::operator=(bool Value)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Set the value of the feature synchronously.
*
* \param     Value  The value to set, true means on and false off.
* 
* \exception BcamException The value of \c ::GetLastError() is thrown
*
* \b Example 
* \code
* m_pBcam->WhiteBalance.AutoMode = true;
* m_pBcam->WhiteBalance.AutoMode.operator =( true );
* \endcode
*/
//------------------------------------------------------------------------------
void CBcam::CWhiteBalance::CBoolControl::operator=(bool Value)
{
  ArgSetDCSFeatureCSR arg;
  ZeroMemory(&arg, sizeof(arg));
  arg.FeatId = FeatureID_WhiteBalance;
  switch ( m_Mode )
  {
  case mOnOff:
    arg.FeatCSRDescriptor.WhiteBalance.ON_OFF = Value;
    arg.Mask = DCS_FEATURE_CSR_SET_ON_OFF;
    break;
  case mAutoMode:
    arg.FeatCSRDescriptor.WhiteBalance.A_M_Mode = Value;
    arg.Mask = DCS_FEATURE_CSR_SET_A_M_MODE;
    break;
  case mAbsControl:
    arg.FeatCSRDescriptor.WhiteBalance.Abs_Control = Value;
    arg.Mask = DCS_FEATURE_CSR_SET_ABS_CONTROL;
    break;
  default:
    assert(false);
  }
  m_pBcam->TryTransmit( _T( "CBcam::CWhiteBalance::CBoolControl::operator=()" ), IOCTL_BCAM_SET_DCS_FEATURE_CSR, &arg, sizeof(arg) );
}


//------------------------------------------------------------------------------
// bool CBcam::CWhiteBalance::CBoolControl::operator()()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the value of the feature synchronously.
*
* \return    
*
* Returns the current value. True means on and false off.
* 
* \exception BcamException The value of \c ::GetLastError() is thrown
*
* \b Example
* \code
* bool IsOn;
* IsOn = m_pBcam->WhiteBalance.AutoMode();
* IsOn = m_pBcam->WhiteBalance.AutoMode.operator()();
* \endcode
*/
//------------------------------------------------------------------------------
bool CBcam::CWhiteBalance::CBoolControl::operator()()
{
  bool retVal = false;
  ArgGetDCSFeatureCSR arg;
  ZeroMemory(&arg, sizeof(arg));
  arg.FeatId = FeatureID_WhiteBalance;
  ResGetDCSFeatureCSR res;
  m_pBcam->TryTransmit( _T( "CBcam::CWhiteBalance::CBoolControl::Operator()()" ), IOCTL_BCAM_GET_DCS_FEATURE_CSR, &arg, sizeof(arg), &res, sizeof(res) );
  switch ( m_Mode )
  {
  case mOnOff:
    retVal = res.WhiteBalance.ON_OFF;
    break;
  case mAutoMode:
    retVal = res.WhiteBalance.A_M_Mode;
    break;
  case mAbsControl:
    retVal = res.WhiteBalance.Abs_Control;
    break;
  default:
    assert(false);
  }
  return retVal;
}

///////////////////////////////////////////////////////////////////////////////
// class CBcam::CWhiteBalance::CRawRegister

//------------------------------------------------------------------------------
// CBcam::CWhiteBalance::CRawRegister::CRawRegister( ): 
// Author: 
//------------------------------------------------------------------------------
/**
 * default constructor
 */
//------------------------------------------------------------------------------
CBcam::CWhiteBalance::CRawRegister::CRawRegister( )
: UBValue(vUB, This()->This()),
  VRValue(vVR, This()->This())
{
  /* NOP */
}
  
//------------------------------------------------------------------------------
// unsigned long CBcam::CWhiteBalance::CRawRegister::Min()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the minimum of the white balance values
*
* \return    
*
* Returns the minimum value.
* 
* \throw BcamException The value of \c::GetLastError() is thrown
*
* \b Example
* \code
* unsigned long value;
* value = m_pBcam->WhiteBalance.Raw.Min();
* \endcode
*/
//------------------------------------------------------------------------------
unsigned long CBcam::CWhiteBalance::CRawRegister::Min()
{
  ArgQueryDCSFeatureInq arg;
  arg.FeatId = FeatureID_WhiteBalance;
  ResQueryDCSFeatureInq res;
  This()->This()->TryTransmit( _T( "CBcam::CWhiteBalance::CRawRegister::Min()" ), IOCTL_BCAM_QUERY_DCS_FEATURE_INQ, &arg, sizeof(arg), &res, sizeof(res) );
  return res.Scalar.Min_Value;
}

//------------------------------------------------------------------------------
// unsigned long CBcam::CWhiteBalance::CRawRegister::Max()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the maximum of the white balance values 
*
* \return    
*
* Returns the maximum value.
* 
* \throw BcamException The value of \c ::GetLastError() is thrown.
*
* \b Example
* \code
* unsigned long value;
* value = m_pBcam->Brightness.Raw.Max();
* \endcode
*/
//------------------------------------------------------------------------------
unsigned long CBcam::CWhiteBalance::CRawRegister::Max()
{
  ArgQueryDCSFeatureInq arg;
  arg.FeatId = FeatureID_WhiteBalance;
  ResQueryDCSFeatureInq res;
  This()->This()->TryTransmit( _T( "CBcam::CWhiteBalance::CRawRegister::Max()" ), IOCTL_BCAM_QUERY_DCS_FEATURE_INQ, &arg, sizeof(arg), &res, sizeof(res) );
  return res.Scalar.Max_Value;
}


///////////////////////////////////////////////////////////////////////////////
// class CBcam::CWhiteBalance::CRawRegister::CWhiteBalanceRawValue

//------------------------------------------------------------------------------
// CBcam::CWhiteBalance::CRawRegister::CWhiteBalanceRawValue::CWhiteBalanceRawValue(Value_t vt, CBcam* pBcam )
// Author: 
//------------------------------------------------------------------------------
/**
 * constructor
 *
 * \param     vt      Either vUB or vVR
 * \param     pBcam   Backpointer to the communication object 
 * \return    
 *
 * 
 */
//------------------------------------------------------------------------------
CBcam::CWhiteBalance::CRawRegister::CWhiteBalanceRawValue::CWhiteBalanceRawValue(Value_t vt, CBcam* pBcam )
: m_vt(vt),
  m_pBcam(pBcam) 
{
  /* NOP */
}

//------------------------------------------------------------------------------
// void CBcam::CWhiteBalance::CRawRegister::CWhiteBalanceRawValue::operator=(unsigned long Value)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Set the value of the white balance feature (either U/B or V/R value )
*
* \param     Value Value to set
*
* \exception BcamException The value of \c ::GetLastError() is thrown
*
* \b Example
* \code
* const unsigned value=12;
* m_pBcam->WhiteBalance.Raw.UBValue = value;
* m_pBcam->WhiteBalance.Raw.UBValue.operator=( value );
* \endcode
*/
//------------------------------------------------------------------------------
void CBcam::CWhiteBalance::CRawRegister::CWhiteBalanceRawValue::operator=(unsigned long Value)
{
  ArgSetDCSFeatureCSR arg;
  ZeroMemory(&arg, sizeof(arg));
  arg.FeatId = FeatureID_WhiteBalance;
  if ( m_vt == vUB )
  {
    arg.FeatCSRDescriptor.WhiteBalance.U_B = Value;
    arg.Mask = DCS_FEATURE_CSR_SET_UB_VALUE;
  }
  else
  {
    arg.FeatCSRDescriptor.WhiteBalance.V_R = Value;
    arg.Mask = DCS_FEATURE_CSR_SET_VR_VALUE;
  }
  m_pBcam->TryTransmit( _T( "CBcam::CWhiteBalance::CRawRegister::CWhiteBalanceRawValue::operator=()" ), IOCTL_BCAM_SET_DCS_FEATURE_CSR, &arg, sizeof(arg) );
}

//------------------------------------------------------------------------------
// unsigned long CBcam::CWhiteBalance::CRawRegister::CWhiteBalanceRawValue::operator()()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the value of either the white balance U/B or V/R value 
*
* \return    
*
* Returns the current value.
*
* \exception BcamException The value of \c ::GetLastError() is thrown
*
* \b Example
* \code
* unsigned value;
* value = m_pBcam->WhiteBalance.Raw.UBValue();
* value = m_pBcam->WhiteBalance.Raw.UBValue.operator()();
* \endcode
*/
//------------------------------------------------------------------------------
unsigned long CBcam::CWhiteBalance::CRawRegister::CWhiteBalanceRawValue::operator()()
{
  ArgGetDCSFeatureCSR arg;
  ZeroMemory(&arg, sizeof(arg));
  arg.FeatId = FeatureID_WhiteBalance;
  ResGetDCSFeatureCSR res;
  m_pBcam->TryTransmit( _T( "CBcam::CWhiteBalance::CRawRegister::CWhiteBalanceRawValue::Operator()()" ),
    IOCTL_BCAM_GET_DCS_FEATURE_CSR, &arg, sizeof(arg), &res, sizeof(res));
  return m_vt == vUB ? res.WhiteBalance.U_B : res.WhiteBalance.V_R;
}

//------------------------------------------------------------------------------
// void CBcam::CWhiteBalance::CRawRegister::CWhiteBalanceRawValue::SetAsync(unsigned long Value, void *pContext)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Set a white balance value asynchronously.
*
* \param     Value     Value to set
* \param     *pContext Context that will be returned. (optional)
* \exception BcamException The value of \c ::GetLastError() is thrown
*
* \b Example
* \code
* const unsigned long newVal=12;
* m_pBcam->WhiteBalance.Raw.UBValue.SetAsync( newVal );
* m_pBcam->WhiteBalance.Raw.UBValue.SetAsync( newVal, &aInfoBlock );
* \endcode
*/
//------------------------------------------------------------------------------
void CBcam::CWhiteBalance::CRawRegister::CWhiteBalanceRawValue::SetAsync(unsigned long Value, void *pContext)
{
  
  BcamOL *pOL;
  ArgSetDCSFeatureCSR arg;
  ZeroMemory( &arg, sizeof arg );
  arg.FeatId = FeatureID_WhiteBalance;
  if ( m_vt == vUB )
  {
    arg.FeatCSRDescriptor.WhiteBalance.U_B = Value;
    arg.Mask = DCS_FEATURE_CSR_SET_UB_VALUE;
    pOL = GetNewBcamOL(AsyncWhiteBalanceRawUBSet, pContext );
  }
  else
  {
    arg.FeatCSRDescriptor.WhiteBalance.V_R = Value;
    arg.Mask = DCS_FEATURE_CSR_SET_VR_VALUE;;
    pOL = GetNewBcamOL(AsyncWhiteBalanceRawVRSet, pContext );
  }

  if (! m_pBcam->Transmit( IOCTL_BCAM_SET_DCS_FEATURE_CSR, &arg, sizeof arg, pOL ))
  {
    DWORD error = ::GetLastError();
    if (error != ERROR_IO_PENDING)
      throw BcamException( ::GetLastError(), _T( "CBcam::CWhiteBalance::CRawRegister::CWhiteBalanceRawValue::SetAsync()" ) );
  }
}

///////////////////////////////////////////////////////////////////////////////
// class CBcam::CWhiteBalance::CAbsValue

//------------------------------------------------------------------------------
// CBcam::CWhiteBalance::CAbsValue::CAbsValue( )
// Author: 
//------------------------------------------------------------------------------
/**
 * default constructor
 *
 * 
 */
//------------------------------------------------------------------------------
CBcam::CWhiteBalance::CAbsValue::CAbsValue( )
: UBValue(vUB, This()->This()),
  VRValue(vVR, This()->This())
{
  /* NOP */
}


//------------------------------------------------------------------------------
// double CBcam::CWhiteBalance::CAbsValue::Min()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the minimum of a DCAM feature synchronously.
*
* \return    
*
* Returns the minimum value. 
* 
* \exception BcamException The value of \c ::GetLastError() is thrown
*
* \b Example
* \code 
* double value;
* value = m_pBcam->WhiteBalance.Abs.Min();
* \endcode
*/
//------------------------------------------------------------------------------
double CBcam::CWhiteBalance::CAbsValue::Min()
{
  throw BcamException(Bvc::ErrorNumber( E_NOTIMPL ), _T( "CWhiteBalance::CAbsValue::Min()" ) );
}

//------------------------------------------------------------------------------
// double CBcam::CWhiteBalance::CAbsValue::Max()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the maximum of a DCAM feature synchronously.
*
* \return    
*
* Returns the maximum value.
* 
* \exception BcamException The value of \c ::GetLastError() is thrown
* 
* \b Example
* \code
* double value;
* value = m_pBcam->WhiteBalance.Abs.Max();
* \endcode
*/
//------------------------------------------------------------------------------
double CBcam::CWhiteBalance::CAbsValue::Max()
{
  throw BcamException(Bvc::ErrorNumber( E_NOTIMPL ), _T( "CWhiteBalance::CAbsValue::Max()" ) );
}

///////////////////////////////////////////////////////////////////////////////
// class CBcam::CWhiteBalance::CAbsValue::::CWhiteBalanceAbsValue

//------------------------------------------------------------------------------
// CBcam::CWhiteBalance::CAbsValue::CWhiteBalanceAbsValue::CWhiteBalanceAbsValue(Value_t vt, CBcam* pBcam )
// Author: 
//------------------------------------------------------------------------------
/**
 * constructor
 *
 * \param     vt      Either vUB or vVR
 * \param     pBcam   Backpointer to the communication object 
 * \return    
 *
 * 
 */
//------------------------------------------------------------------------------
CBcam::CWhiteBalance::CAbsValue::CWhiteBalanceAbsValue::CWhiteBalanceAbsValue(Value_t vt, CBcam* pBcam )
: m_vt(vt),
  m_pBcam(pBcam)
{
  /* NOP */
}

//------------------------------------------------------------------------------
// void CBcam::CWhiteBalance::CAbsValue::CWhiteBalanceAbsValue::operator=(double Value)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Set the value of the feature synchronously.
*
* \param     Value Value to set
* \exception BcamException The value of \c ::GetLastError() is thrown
* 
* \b Example
* \code
* const double value = 3.5; // dB
* m_pBcam->WhiteBalance.Abs.UBValue = value;
* m_pBcam->WhiteBalance.Abs.UBValue.operatator=( value );
* \endcode
*/
//------------------------------------------------------------------------------
void CBcam::CWhiteBalance::CAbsValue::CWhiteBalanceAbsValue::operator=(double /*Value*/)
{
  throw BcamException( Bvc::ErrorNumber( E_NOTIMPL ), _T( "CBcam::CWhiteBalance::CAbsValue::CWhiteBalanceAbsValue::operator=()" ) );
}

//------------------------------------------------------------------------------
// double CBcam::CWhiteBalance::CAbsValue::CWhiteBalanceAbsValue::operator()()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the value of the feature synchronously.
*
* \return    
*
* Returns the current value
* 
* \throw BcamException The value of \c ::GetLastError() is thrown
* 
*  \b Example
* \code
* double value;
* value = m_pBcam->WhiteBalance.Abs.UBValue()
* value = m_pBcam->WhiteBalance.Abs.UBValue.operator()();
* \endcode
*/
//------------------------------------------------------------------------------
double CBcam::CWhiteBalance::CAbsValue::CWhiteBalanceAbsValue::operator()()
{
  throw BcamException( Bvc::ErrorNumber( E_NOTIMPL ), _T( "CBcam::CWhiteBalance::CAbsValue::CWhiteBalanceAbsValue::operator()()" ) );
}

//------------------------------------------------------------------------------
// void CBcam::CWhiteBalance::CAbsValue::SetAsync(double Value, void *pContext)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Set the value of the feature asynchronously.
*
* \param     Value
* \param     *pContext
* \exception BcamException The value of \c ::GetLastError() is thrown

*
* \b Example
* \code
* const double value = 3.5; // dB
* m_pBcam->WhiteBalance.Abs.SetAsync( value );
* m_pBcam->WhiteBalance.Abs.SetAsync( value, &InfoBlock );
* \endcode
*/
//------------------------------------------------------------------------------
void CBcam::CWhiteBalance::CAbsValue::CWhiteBalanceAbsValue::SetAsync(double /*Value*/, void* /*pContext*/)
{
  throw BcamException( Bvc::ErrorNumber( E_NOTIMPL ), _T( "CBcam::CWhiteBalance::CAbsValue::CWhiteBalanceAbsValue::SetAsync()" ) );
}

//------------------------------------------------------------------------------
// void CBcam::CWhiteBalance::Save(const CPropertyBagPtr ptrBag)
// Author: 
// Date: 04.09.2002
//------------------------------------------------------------------------------
/**
 * Save settings to a property bag
 *
 * \param     ptrBag The property Bag
 */
//------------------------------------------------------------------------------
void CBcam::CWhiteBalance::Save(const CPropertyBagPtr ptrBag)
{
  if ( IsSupported(inqOnOff) )
  {
    bool onOff = OnOff();
    ptrBag->WriteBool( _T( "OnOff" ), onOff);
    if ( ! onOff )
      return;  // feature is disabled, nothing left to save
  }

  if ( IsSupported(inqAuto) )
  {
    bool autoMode = AutoMode();
    ptrBag->WriteBool(_T( "AutoMode" ), autoMode);
    if ( autoMode )
      return; // feature is in auto mode, nothing left to save
  }

  if ( IsSupported(inqManual) && IsSupported(inqReadOut) )
  {
    bool absControl = false;
    if ( IsSupported(inqAbsControl) )
    {
      absControl = AbsControl();
      ptrBag->WriteBool( _T( "AbsControl"), absControl );
    }
    if ( absControl )
    {
      ptrBag->WriteFloat( _T( "AbsValueUB" ), (float) Abs.UBValue() );
      ptrBag->WriteFloat( _T( "AbsValueVR" ), (float) Abs.VRValue() );
    }
    else
    {
      ptrBag->WriteLong( _T( "RawValueUB" ), Raw.UBValue() );
      ptrBag->WriteLong( _T( "RawValueVR" ), Raw.VRValue() );
    }
  }

}

//------------------------------------------------------------------------------
// void CBcam::CWhiteBalance::Restore(const CPropertyBagPtr ptrBag)
// Author: 
// Date: 04.09.2002
//------------------------------------------------------------------------------
/**
 * Restore settings from a property bag
 *
 * \param     ptrBag The property bag
 */
//------------------------------------------------------------------------------
void CBcam::CWhiteBalance::Restore(const CPropertyBagPtr ptrBag)
{
  if ( IsSupported(inqOnOff) )
  {
    bool onOff = ptrBag->ReadBool( _T( "OnOff" ) );
    OnOff = onOff;
    if ( ! onOff )
      return;  // feature is disabled, nothing to restore
  }

  if ( IsSupported(inqAuto) )
  {
    bool autoMode = ptrBag->ReadBool( _T( "AutoMode" ) );
    AutoMode = autoMode;
    if ( autoMode )
      return; // feature is in auto mode, noting to restore
  }

  if ( IsSupported(inqManual) && IsSupported(inqReadOut) )
  {
    bool absControl = false;
    if ( IsSupported(inqAbsControl) )
    {
      absControl = ptrBag->ReadBool( _T( "AbsControl" ) );
    }
    if ( absControl )
    {
      Abs.UBValue = ptrBag->ReadFloat( _T( "AbsValueUB" ) );
      Abs.VRValue = ptrBag->ReadFloat( _T( "AbsValueVR") );
    }
    else
    {
      Raw.UBValue = ptrBag->ReadLong( _T( "RawValueUB" ) );
      Raw.VRValue = ptrBag->ReadLong( _T( "RawValueVR" ) );
    }
  }
}



///////////////////////////////////////////////////////////////////////////////
// class CBcam::CTestImage

//------------------------------------------------------------------------------
// bool CBcam::CTestImage::IsSupported(Supported_t Inquiry)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Inquire whether the feature is supported
*
* \param  Inquiry      Specifies the part of the feature
* 
* \retval true if feature is supported
* \retval false if feature is not supported.
* \exception BcamException The value of \c ::GetLastError() is thrown
*
* \b Example
* \code
* bool testImageFeatureSupported = m_pBcam->TestImage.IsSupported();
* bool testImageTwoSupported = m_pBcam->TestImage.IsSupported(inqTestImage_2);
* \endcode
* 
*/
//------------------------------------------------------------------------------
bool CBcam::CTestImage::IsSupported(Supported_t Inquiry)
{
  switch ( Inquiry )
  {
  case inqPresent:
  case inqTestImage_1:
  case inqTestImage_2:
  case inqTestImage_3:
  case inqTestImage_4:
  case inqTestImage_5:
  case inqTestImage_6:
  case inqTestImage_7:
    {
      ResGetTestImageCSR res;
      if (! This()->Transmit( IOCTL_BCAM_GET_TEST_IMAGE_CSR, NULL, 0, &res, sizeof(res)) )
      {
        DWORD error = ::GetLastError();
        if( (error & 0xdfffffff) == STATUS_BCAM_UNLOCK_ERROR   ||  // camera doesn't implement advanced features
            (error & 0xdfffffff) == STATUS_BCAM_TESTIMAGE_FEATURE_NOT_SUPPORTED  // camera doesn't implement the test imagefeature 
        )
        {

          return false;
        }
        // an other error has occured -> throw an exception
        throw BcamException( error, _T( "CBcam::CTestImage::IsSupported()" ) ); 
      }
      ULONG mask = *(PULONG) &res.TestImageCSR;
      return res.TestImageCSR.Presence_Inq && ( Inquiry == inqPresent || ( (0x01000000 >> Inquiry ) & mask )) ; 
    }
  default:
    throw BcamException( BCAM_E_INVALID_INQUIRY, _T( "CBcam::CTestImage::IsSupported()" ), &Inquiry);
  }
}

//------------------------------------------------------------------------------
// void CBcam::CTestImage::operator=(unsigned long Value)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Select a test image 
*
* \param  Value Number of the testimage ( TestImage_Disabled, TestImage_1, ... , TestImage_7)
* \exception BcamException The value of \c ::GetLastError() is thrown
*/
//------------------------------------------------------------------------------
void CBcam::CTestImage::operator=(ImageOnMode Value)
{
  ArgSetTestImageCSR arg;
  ZeroMemory(&arg, sizeof(arg) );
  arg.TestImageCSR.ImageOn = Value;
  if ( ! IsSupported( (Supported_t) Value ) )
    throw BcamException( BCAM_E_TESTIMAGE_NOT_SUPPORTED, _T( "CBcam::CTestImage::operator=()" ), &Value );
  This()->TryTransmit( _T( "CBcam::CTestImage::operator=()" ), IOCTL_BCAM_SET_TEST_IMAGE_CSR, &arg, sizeof(arg) );
}

//------------------------------------------------------------------------------
// unsigned long CBcam::CTestImage::operator()()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the selected image number
*
* \return    
*
* Returns the currently selected testimage number.
* 
* \exception BcamException The value of \c ::GetLastError() is thrown
*/
//------------------------------------------------------------------------------
ImageOnMode CBcam::CTestImage::operator()()
{
  ResGetTestImageCSR res;
  This()->TryTransmit( _T( "CBcam::CTestImage::operator()()" ), IOCTL_BCAM_GET_TEST_IMAGE_CSR, NULL, 0, &res, sizeof(res) );
  return (ImageOnMode) res.TestImageCSR.ImageOn;
}

//------------------------------------------------------------------------------
// void CBcam::CTestImage::Save(const CPropertyBagPtr ptrBag)
// Author: A.Happe
// Date: 04.09.2002
//------------------------------------------------------------------------------
/**
 * \brief Save the test image settings to a property bag
 *
 * \param     ptrBag The property bag
 */
//------------------------------------------------------------------------------
void CBcam::CTestImage::Save(const CPropertyBagPtr ptrBag)
{
  ptrBag->WriteLong( _T( "Value" ), operator()());
}

//------------------------------------------------------------------------------
// void CBcam::CTestImage::Restore(const CPropertyBagPtr ptrBag)
// Author: 
// Date: 04.09.2002
//------------------------------------------------------------------------------
/**
 * \brief Restore the test image settings from a property bag
 *
 * \param     ptrBag The property bag
 */
//------------------------------------------------------------------------------
void CBcam::CTestImage::Restore(const CPropertyBagPtr ptrBag)
{
  operator=((ImageOnMode) ptrBag->ReadLong( _T( "Value" ) ) );
}


///////////////////////////////////////////////////////////////////////////////
//
// class CBcam::CTrigger
//
//

//------------------------------------------------------------------------------
// CBcam::CTrigger::CTrigger(  FunctionCode_t AsyncFuncCodeRaw,  FunctionCode_t AsyncFuncCodeAbs,  CBcam* pBcam )
//------------------------------------------------------------------------------
// Author: 
//------------------------------------------------------------------------------
/**
 * \brief Contruct the trigger property.
 *
 * \param     AsyncFuncCodeRaw Function code for asynchronous calls to raw register
 * \param     AsyncFuncCodeAbs Function code for asynchronous calls to absolute register
 * \param     pBcam Backpointer to the communication object
 */
//------------------------------------------------------------------------------
CBcam::CTrigger::CTrigger( 
                            FunctionCode_t /*AsyncFuncCodeRaw*/, 
                            FunctionCode_t /*AsyncFuncCodeAbs*/, 
                            CBcam* /*pBcam*/ ) 
{ 
  /* NOP */ 
}

//------------------------------------------------------------------------------
// bool CBcam::CTrigger::IsSupported(Supported_t Inquiry)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Inquire whether a feature is supported
*
* \param  Inquiry Specifies the part of the feature
* \retval true if the feature is supported.
* \retval false if the feature is not supported.
* \exception BcamException The value of \c ::GetLastError() is thrown
*/
//------------------------------------------------------------------------------
bool CBcam::CTrigger::IsSupported(Supported_t Inquiry)
{
  ArgQueryDCSFeatureInq arg;
  arg.FeatId = FeatureID_Trigger;
  ResQueryDCSFeatureInq res;
  This()->TryTransmit( _T( "CBcam::CTrigger::IsSupported()" ),
    IOCTL_BCAM_QUERY_DCS_FEATURE_INQ, &arg, sizeof(arg), &res, sizeof(res) );
  switch ( Inquiry )
  {
  case inqPresent:
    return res.Trigger.Presence_Inq;
  case inqAbsControl:
    return res.Trigger.Abs_Control_Inq;
  case inqReadOut:
    return res.Trigger.Read_Out_Inq;
  case inqOnOff:
    return res.Trigger.On_Off_Inq;
  case inqPolarity:
    return res.Trigger.Polarity_Inq;
  default:
    throw BcamException( BCAM_E_INVALID_INQUIRY, _T( "CBcam::CTrigger::IsSupported()" ), &Inquiry );
  }
  // return true;
}

//------------------------------------------------------------------------------
// bool CBcam::CTrigger::IsModeSupported(DCSTriggerMode TriggerMode)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Inquire whether a trigger mode is supported
*
* \param  TriggerMode Code of the trigger mode
* \retval true if the mode is supported.
* \retval false if the mode is not supported.
* \exception BcamException The value of \c ::GetLastError() is thrown
*/
//------------------------------------------------------------------------------
bool CBcam::CTrigger::IsModeSupported(DCSTriggerMode TriggerMode)
{
  ArgQueryDCSFeatureInq arg;
  arg.FeatId = FeatureID_Trigger;
  ResQueryDCSFeatureInq res;
  This()->TryTransmit( _T( "CBcam::CTrigger::IsModeSupported()" ),
    IOCTL_BCAM_QUERY_DCS_FEATURE_INQ, &arg, sizeof(arg), &res, sizeof(res) );
  switch ( TriggerMode )
  {
    case DCS_TriggerMode0:
      return res.Trigger.Trigger_Mode0_Inq;
    case DCS_TriggerMode1:

      return res.Trigger.Trigger_Mode1_Inq;
    case DCS_TriggerMode2:
       return res.Trigger.Trigger_Mode2_Inq;
    case DCS_TriggerMode3:
      return res.Trigger.Trigger_Mode3_Inq;
    default:
      throw BcamException( BCAM_E_INVALID_INQUIRY, _T( "CBcam::CTrigger::IsModeSupported()" ), &TriggerMode );
  }
}

//------------------------------------------------------------------------------
// void CBcam::CTrigger::Save(const CPropertyBagPtr ptrBag)
// Author: A.Happe
// Date: 04.09.2002
//------------------------------------------------------------------------------
/**
 * \brief Save the trigger settings to a property bag
 *
 * \param     ptrBag The property bag
 */
//------------------------------------------------------------------------------

void CBcam::CTrigger::Save(const CPropertyBagPtr ptrBag)
{
  bool onOff = false;
  if ( IsSupported(inqOnOff) )
  {
    onOff = OnOff();
    ptrBag->WriteBool( _T( "OnOff" ), onOff );
  }
  if ( ! onOff )
    return; // feature is disabled, nothing left to save
  if ( IsSupported(inqReadOut)  )
  {
    ptrBag->WriteLong( _T( "Mode" ), Mode() );
    ptrBag->WriteLong( _T( "Parameter" ), Parameter() );
    if ( IsSupported(inqPolarity) )
      ptrBag->WriteLong( _T( "Polarity" ), Polarity() );
  }

}

//------------------------------------------------------------------------------
// void CBcam::CTrigger::Restore(const CPropertyBagPtr ptrBag)
// Author: A.Happe
// Date: 04.09.2002
//------------------------------------------------------------------------------
/**
 * \brief Restore the trigger settings from a property bag
 *
 * \param     ptrBag The property bag
 */
//------------------------------------------------------------------------------
void CBcam::CTrigger::Restore(const CPropertyBagPtr ptrBag)
{
  bool onOff = false;
  if ( IsSupported(inqOnOff) )
  {
    onOff = ptrBag->ReadBool( _T( "OnOff" ) );
    OnOff = onOff;
  }
  if ( ! onOff )
    return; // feature is disabled, nothing to restore
  if ( IsSupported(inqReadOut)  )
  {
    Mode = (DCSTriggerMode) ptrBag->ReadLong( _T( "Mode" ) );
    Parameter = (DCSTriggerMode) ptrBag->ReadLong( _T( "Parameter" ) );
    if ( IsSupported(inqPolarity) )
      Polarity = (Polarity_t) ptrBag->ReadLong( _T( "Polarity" ) );
  }
}



///////////////////////////////////////////////////////////////////////////////
// class CBcam::CTrigger::CMode


//------------------------------------------------------------------------------
// void CBcam::CTrigger::CMode::operator=(DCSTriggerMode Value)
// Author: 
//------------------------------------------------------------------------------
/**
 * \brief Set the mode of the trigger feature
 * \param     Value Value to set
 * \exception BcamException The value of \c ::GetLastError() is thrown 
 *
 * \b Example 
 * \code
 * const DCSTriggerMode = DCS_TriggerMode3; // internal trigger mode
 * m_pBcam->Trigger.Mode = mode;
 * m_pBcam->Trigger.Mode.operator ( mode );
 * \endcode
*/
//------------------------------------------------------------------------------
void CBcam::CTrigger::CMode::operator=(DCSTriggerMode Value)
{
  ArgSetDCSFeatureCSR arg;
  ZeroMemory(&arg, sizeof(arg));
  arg.FeatId = FeatureID_Trigger;
  arg.FeatCSRDescriptor.Trigger.Trigger_Mode = Value;
  arg.Mask = DCS_FEATURE_CSR_SET_TRIGGER_MODE;
  This()->This()->TryTransmit( _T( "CBcam::CTrigger::CMode::Operator=()" ), IOCTL_BCAM_SET_DCS_FEATURE_CSR, &arg, sizeof(arg) );
}

//------------------------------------------------------------------------------
// DCSTriggerMode CBcam::CTrigger::CMode::operator()()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the trigger mode.
*
* \return    
*
* Returns the number of the currently selected trigger mode.
* \exception BcamException The value of \c ::GetLastError() is thrown 
*
* \b Example
* \code
* DCSTriggerMode mode;
* mode = m_pBcam->Trigger.Mode();
* mode = m_pBcam->Trigger.Mode.operator()();
* \endcode
*/
//------------------------------------------------------------------------------
DCSTriggerMode CBcam::CTrigger::CMode::operator()()
{
  ArgGetDCSFeatureCSR arg;
  ZeroMemory(&arg, sizeof(arg));
  arg.FeatId = FeatureID_Trigger;
  ResGetDCSFeatureCSR res;
  This()->This()->TryTransmit( _T( "CBcam::CTrigger::CMode::Operator()()" ),
    IOCTL_BCAM_GET_DCS_FEATURE_CSR, &arg, sizeof(arg), &res, sizeof(res) );
  return (DCSTriggerMode) res.Trigger.Trigger_Mode;
}

///////////////////////////////////////////////////////////////////////////////
// class CBcam::CTrigger::CParameter

//------------------------------------------------------------------------------
// void CBcam::CTrigger::CParameter::operator=(unsigned long Value)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Set the trigger parameter
* \param     Value The value to set.
* \exception BcamException The value of \c ::GetLastError() is thrown
* \b Example:
* \code
* const unsigned long value = 3; // integration time from first to third trigger
* m_pBcam->Trigger.Parameter = value;
* m_pBcam->Trigger.Parameter.operator=( value );
* \endcode
*/
//------------------------------------------------------------------------------
void CBcam::CTrigger::CParameter::operator=(unsigned long Value)
{
  ArgSetDCSFeatureCSR arg;
  ZeroMemory(&arg, sizeof(arg));
  arg.FeatId = FeatureID_Trigger;
  arg.FeatCSRDescriptor.Trigger.Parameter = Value;
  arg.Mask = DCS_FEATURE_CSR_SET_PARAMETER;
  This()->This()->TryTransmit( _T( "CBcam::CTrigger::CParameter::operator=()" ),
    IOCTL_BCAM_SET_DCS_FEATURE_CSR, &arg, sizeof(arg) );
}

//------------------------------------------------------------------------------
// unsigned long CBcam::CTrigger::CParameter::operator()()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the  trigger parameter
*
* \return    
* Returns the current value.
* \exception BcamException The value of \c ::GetLastError() is thrown
* \b Example:
* \code
* unsigned long value;
* value = m_pBcam->Trigger.Parameter();
* value = m_pBcam->Trigger.Parameter.operator()();
* \endcode
*/
//------------------------------------------------------------------------------
unsigned long CBcam::CTrigger::CParameter::operator()()
{
  ArgGetDCSFeatureCSR arg;
  ZeroMemory(&arg, sizeof(arg));
  arg.FeatId = FeatureID_Trigger;
  ResGetDCSFeatureCSR res;
  This()->This()->TryTransmit( _T( "CBcam::CTrigger::CParameter::Operator()()" ),
    IOCTL_BCAM_GET_DCS_FEATURE_CSR, &arg, sizeof(arg), &res, sizeof(res) );
  return res.Trigger.Parameter;
}

///////////////////////////////////////////////////////////////////////////////
//
// CBcam::CTrigger::CBoolControl 
//
//

//------------------------------------------------------------------------------
// bool CBcam::CTrigger::CBoolControl::operator ()
// Author: 
//------------------------------------------------------------------------------
/**
 * \brief Inquire the feature is on or off.
 *
 * \retval  true if it is on    
 * \retval  false if it is not on
 * \exception BcamException The value of \c ::GetLastError() is thrown
 */
//------------------------------------------------------------------------------
bool CBcam::CTrigger::CBoolControl::operator ()()
{
  bool retVal = false;
  ArgGetDCSFeatureCSR arg;
  ZeroMemory(&arg, sizeof(arg));
  arg.FeatId = FeatureID_Trigger;
  ResGetDCSFeatureCSR res;
  This()->This()->TryTransmit( _T( "CBcam::CTrigger::CBoolControl::Operator()()" ),
    IOCTL_BCAM_GET_DCS_FEATURE_CSR, &arg, sizeof(arg), &res, sizeof(res) );
  switch ( m_Mode )
  {
  case mOnOff:
    retVal = res.Trigger.ON_OFF;
    break;
  case mAbsControl:
    retVal = res.Trigger.Abs_Control;
    break;
  default:
    assert(false);
  }
  return retVal;
}

//------------------------------------------------------------------------------
// void CBcam::CTrigger::CBoolControl::operator =( bool OnOff )
// Author: 
//------------------------------------------------------------------------------
/**
 * \brief Turn a feature on or off
 *
 * \param     OnOff  true is on, false is off
 * \exception The Value of \c ::GetLastError() is thrown.
 */
//------------------------------------------------------------------------------
void CBcam::CTrigger::CBoolControl::operator =( bool Value )
{
  ArgSetDCSFeatureCSR arg;
  ZeroMemory(&arg, sizeof(arg));
  arg.FeatId = FeatureID_Trigger;
  switch ( m_Mode )
  {
  case mOnOff:
    arg.FeatCSRDescriptor.Trigger.ON_OFF = Value;
    arg.Mask = DCS_FEATURE_CSR_SET_ON_OFF;
    break;
  case mAbsControl:
    arg.FeatCSRDescriptor.Trigger.Abs_Control = Value;
    arg.Mask = DCS_FEATURE_CSR_SET_ABS_CONTROL;
    break;
  default:
    assert(false);
  }
  This()->This()->TryTransmit( _T( "CBcam::CTrigger::CBoolControl::operator=()" ),
    IOCTL_BCAM_SET_DCS_FEATURE_CSR, &arg, sizeof(arg) );
}

///////////////////////////////////////////////////////////////////////////////
// class CBcam::CTrigger::CPolarity

//------------------------------------------------------------------------------
// void CBcam::CTrigger::CPolarity::operator=( Polarity_t Value )
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Set the polarity of the trigger signal.
*
* \param     Value Value to set
* \exception BcamException The value of \c ::GetLastError() is thrown
* \b Example
* \code
* Polarity_t polarity = LowActive;
* m_pBcam->Trigger.Polarity = polarity;
* m_pBcam->Trigger.Polarity.operator =( polarity );
* \endcode
*/
//------------------------------------------------------------------------------
void CBcam::CTrigger::CPolarity::operator=(Polarity_t Value)
{
  ArgSetDCSFeatureCSR arg;
  ZeroMemory(&arg, sizeof(arg));
  arg.FeatId = FeatureID_Trigger;
  arg.FeatCSRDescriptor.Trigger.Trigger_Polarity = Value;
  arg.Mask = DCS_FEATURE_CSR_SET_TRIGGER_POLARITY;
  This()->This()->TryTransmit( _T( "CBcam::CTrigger::CPolarity::operator=()" ),
    IOCTL_BCAM_SET_DCS_FEATURE_CSR, &arg, sizeof(arg) );
}

//------------------------------------------------------------------------------
// Polarity_t CBcam::CTrigger::CPolarity::operator()()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the polarity of the trigger signal
*
* \return    
* Returns the currently selected polarity
* \exception BcamException The value of \c ::GetLastError() is thrown
* \b Example
* \code
* Polarity_t polarity;
* polarity = m_pBcam->Trigger.Polarity;
* polarity = m_pBcam->Trigger.Polarity.operator()();
* \endcode
*/
//------------------------------------------------------------------------------
Polarity_t CBcam::CTrigger::CPolarity::operator()()
{
  ArgGetDCSFeatureCSR arg;
  ZeroMemory(&arg, sizeof(arg));
  arg.FeatId = FeatureID_Trigger;
  ResGetDCSFeatureCSR res;
  This()->This()->TryTransmit( _T( "CBcam::CTrigger::CPolarity::Operator()()" ),
    IOCTL_BCAM_GET_DCS_FEATURE_CSR, &arg, sizeof(arg), &res, sizeof(res) );

  return ( res.Trigger.Trigger_Polarity ) ? HighActive : LowActive;
}

///////////////////////////////////////////////////////////////////////////////
//
// class CBcam::COneShot
//
//

//------------------------------------------------------------------------------
// bool CBcam::COneShot::IsSupported()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Inquire whether the OneShot feature is supported
*
* \retval true if it is supported
* \retval false if it is not supported
*/
//------------------------------------------------------------------------------
bool CBcam::COneShot::IsSupported()
{
  ResGetBasicFuncInq res;
  This()->TryTransmit( _T( "CBcam::COneShot::IsSupported()" ),
    IOCTL_BCAM_GET_BASIC_FUNC_INQ, NULL, 0, &res, sizeof res );

  return ( res & 0x00001000 ) != 0;

}

//------------------------------------------------------------------------------
// void CBcam::COneShot::Set()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Performs a one shot 
* \see   GrabImage
* \exception BcamException The value of \c ::GetLastError() is thrown
*/
//------------------------------------------------------------------------------
void CBcam::COneShot::Set()
{
  static const ArgCameraControl arg = {CamCmd_SingleGrab, 0};
  This()->TryTransmit( _T( "CBcam::COneShot::Set" ), IOCTL_BCAM_CAMERA_CONTROL, (void *) &arg, sizeof arg );
}

//------------------------------------------------------------------------------
// void CBcam::COneShot::SetAsync(void *pContext)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Grabs a single image asynchronously.
*
* \param     *pContext To be returned after grabbing
* \throw BcamException The value of \c ::GetLastError() is thrown
*/
//------------------------------------------------------------------------------
void CBcam::COneShot::SetAsync(void *pContext)
{
  static const ArgCameraControl arg = {CamCmd_SingleGrab, 0};
  BcamOL *pOL = GetNewBcamOL( AsyncOneShot, pContext );
  if (! This()->Transmit( IOCTL_BCAM_CAMERA_CONTROL, (void *) &arg, sizeof arg, pOL ))
  {
    DWORD error = ::GetLastError();
    if (error != ERROR_IO_PENDING)
      throw BcamException( ::GetLastError(), _T( "CBcam::COneShot::SetAsync()" ) );
  }
}

///////////////////////////////////////////////////////////////////////////////
// class CBcam::CMultiShot

/*
bool CBcam::CMultiShot::IsSupported()
{
TRACE("BCAMApi: CBcam::CMultiShot::IsSupported()\n");
return true;
}

  void CBcam::CMultiShot::operator=(bool Value)
  {
  TRACE("BCAMApi: CBcam::CMultiShot::operator=(%lu)\n", Value);
  }
  
    bool CBcam::CMultiShot::operator()()
    {
    TRACE("BCAMApi: CBcam::CMultiShot::operator()()\n");
    return true;
    }
    
      void CBcam::CMultiShot::CCountNumber::operator=(unsigned long Value)
      {
      TRACE("BCAMApi: CBcam::CMultiShot::CCountNumber::operator=(%lu)\n", Value);
      }
      
        unsigned long CBcam::CMultiShot::CCountNumber::operator()()
        {
        TRACE("BCAMApi: CBcam::CMultiShot::CCountNumber::operator()()\n");
        return 12;
        }
*/

///////////////////////////////////////////////////////////////////////////////
// class CBcam::CContinuousShot

//------------------------------------------------------------------------------
// bool CBcam::CContinuousShot::IsSupported()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Inquire whether the ISO_EN/continous shot feature is supported.
*
* \retval true if it is supported. (It is required by DCAM)    
*
*/
//------------------------------------------------------------------------------
bool CBcam::CContinuousShot::IsSupported()
{
  
  // This is always true :-)
  return true;
}

//------------------------------------------------------------------------------
// void CBcam::CContinuousShot::operator=(bool Value)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Turns continuous grabbing on/off.
*
* \param     Value true is on, false is off
* \throw BcamException The value of \c ::GetLastError() is thrown.
*/
//------------------------------------------------------------------------------
void CBcam::CContinuousShot::operator=(bool Value)
{
  TRACE( _T( "BcamApi: Continuous Shot = %d\n" ), Value);
  ArgCameraControl Arg;
  Arg.Cmd = Value ? CamCmd_Start : CamCmd_Stop;
  This()->TryTransmit( _T( "CBcam::CContinuousShot::operator=()" ), IOCTL_BCAM_CAMERA_CONTROL, &Arg, sizeof(Arg) );
}

//------------------------------------------------------------------------------
// void CBcam::CContinuousShot::SetAsync(bool Value, void *pContext)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Turns continuous grabbing on/off asynchronously.
*
* \param     Value true is on, false is off
* \param     *pContext To be returned after enabling / disabling the camera
* \throw BcamException The value of \c ::GetLastError() is thrown
*/
//------------------------------------------------------------------------------
void CBcam::CContinuousShot::SetAsync(bool Value, void *pContext)
{
  ArgCameraControl Arg;
  Arg.Cmd = Value ? CamCmd_Start : CamCmd_Stop;
  BcamOL *pOL = GetNewBcamOL( AsyncContinuousShot, pContext );
  if (! This()->Transmit( IOCTL_BCAM_CAMERA_CONTROL, (void *) &Arg, sizeof Arg, pOL ))
  {
    DWORD error = ::GetLastError();
    if (error != ERROR_IO_PENDING)
      throw BcamException( ::GetLastError(), _T( "CBcam::CContinuousShot::SetAsync()" ) );
  }
}
//------------------------------------------------------------------------------
// bool CBcam::CContinuousShot::operator()()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Inquire whether the camera is grabbing
*
* \retval true if it is grabbing
* \retval false if it is not grabbing
*/
//------------------------------------------------------------------------------
bool CBcam::CContinuousShot::operator()()
{
  throw BcamException( Bvc::ErrorNumber( E_NOTIMPL ), _T( "CBcam::CContinuousShot::operator()()" ) );
}

///////////////////////////////////////////////////////////////////////////////
// class CBcam::CInfo

//------------------------------------------------------------------------------
// CString CBcam::CInfo::DeviceName()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the name of the camera device
*
* \return    
*
* Returns the device name
*/
//------------------------------------------------------------------------------
CString CBcam::CInfo::DeviceName()
{
  return This()->m_DeviceName;
}

//------------------------------------------------------------------------------
// CString CBcam::CInfo::NodeId()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the node identifier
*
* \return    
* Returns the node identifier.
* \exception BcamException The value of \c ::GetLastError() is thrown
*/
//------------------------------------------------------------------------------
CString CBcam::CInfo::NodeId()
{
  ResGetCameraID res;
  ZeroMemory(&res, sizeof(res));
  This()->TryTransmit( _T( "CBcam::CInfo::NodeId()" ),
    IOCTL_BCAM_GET_CAMERA_ID, NULL, 0, &res, sizeof(res) );

  ULONG VendorId = (ULONG) (( res.Id &  0xffffff0000000000i64 ) >> 40); 
  ULONGLONG ChipId = ( res.Id &  0x000000ffffffffffi64 ); 

  TCHAR buffer[256];
  _stprintf( buffer, _T( "0x%06x-%I64u" ), VendorId, ChipId );
  return buffer;
}

//------------------------------------------------------------------------------
// CString CBcam::CInfo::ModelName()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the camera model name
*
* \return    
* Returns the model name
* \exception BcamException The value of \c ::GetLastError() is thrown
*/
//------------------------------------------------------------------------------
CString CBcam::CInfo::ModelName()
{
  TCHAR res[256];
  ArgGetString arg;
  arg.Id = String_ModelName;
  This()->TryTransmit( _T( "CBcam::CInfo::ModelName()" ), IOCTL_BCAM_GET_STRING, &arg, sizeof(arg), res, sizeof(res) );
  return res;
}

//------------------------------------------------------------------------------
// CString CBcam::CInfo::VendorName()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the camera vendor name.
*
* \return    
* Returns the vendor name
* \exception BcamException The value of \c ::GetLastError() is thrown
* 
*/
//------------------------------------------------------------------------------
CString CBcam::CInfo::VendorName()
{
  TCHAR res[256];
  ArgGetString arg;
  arg.Id = String_VendorName;
  This()->TryTransmit( _T( "CBcam::CInfo::VendorName()" ), IOCTL_BCAM_GET_STRING, &arg, sizeof(arg), res, sizeof(res) );
  return res;
}


//------------------------------------------------------------------------------
// CString CBcam::CInfo::CameraFirmwareVersion()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the camera firmware version.
*
* \return    
* Returns the version.
* 
*/
//------------------------------------------------------------------------------
CString CBcam::CInfo::CameraFirmwareVersion()
{
  TCHAR res[256];
  ArgGetString arg;
  arg.Id = String_CameraVersion;
  This()->TryTransmit( _T( "CBcam::CInfo::CameraFirmwareVersion()" ), IOCTL_BCAM_GET_STRING, &arg, sizeof(arg), res, sizeof(res) );
  return res;
}

//------------------------------------------------------------------------------
// unsigned long CBcam::CInfo::CameraDcamVersion()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Returns the DCAM version the camera supports.
*
* \retval 256 version 1.04
* \retval 257 version 1.20
* \retval 258 version 1.30
* 
*/
//------------------------------------------------------------------------------
unsigned long CBcam::CInfo::CameraDcamVersion()
{
  unsigned long ver;
  This()->TryTransmit( _T( "CBcam::CInfo::CameraDcamVersion()" ),
                        IOCTL_BCAM_GET_DCAM_VERSION,
                        NULL, 
                        0, 
                        &ver, 
                        sizeof ver );
   return ver;
}

//------------------------------------------------------------------------------
// unsigned long CBcam::CInfo::DriverSoftwareVersion()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the version of the BCAM driver.
*
* \return    
* Returns the version.
* \exception BcamException The value of \c ::GetLastError() is thrown
*/
//------------------------------------------------------------------------------
CString CBcam::CInfo::DriverSoftwareVersion()
{
  TCHAR buf[BCAM_MAXIMUM_VERSION_STRING_SIZE];

  This()->TryTransmit( _T("CBcam::CInfo::DriverSoftwareVersion()" ),
                        IOCTL_BCAM_GET_DRIVER_VERSION_STRING,
                        NULL, 
                        0, 
                        buf, 
                        sizeof buf );
  return buf;
}

//------------------------------------------------------------------------------
// long CBcam::CInfo::IsoChannel()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the currently allocated Isochronous Channel.
*
* \return    
*
* Returns the channel number or -1 if no channel is allocated
* 
*/
//------------------------------------------------------------------------------
long CBcam::CInfo::IsoChannel()
{
  return This()->m_IsoChannel;
}

//------------------------------------------------------------------------------
// long CBcam::CInfo::IsoSpeed()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the selected Isochronous Speed.
*
* \return    
*
* Returns the current value
*/
//------------------------------------------------------------------------------
BcamIsoSpeed CBcam::CInfo::IsoSpeed()
{
  return This()->m_IsoSpeed;
}


/************************************************************************************************************/

//------------------------------------------------------------------------------
// CSize CBcam::CFormatSeven::_FormatSeven::MaxSize()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the maximum image size.
*
* \return    
*
* Returns the maximum size.
* 
* \b Example
* \code
*  CSize size;
*  size = m_pBcam->FormatSeven[0].MaxSize();
* \endcode
*/
//------------------------------------------------------------------------------
CSize CBcam::CFormatSeven::_FormatSeven::MaxSize()
{
  ArgFmt7GetModeDescriptor arg = {m_Mode};
  ResFmt7GetModeDescriptor res;
  This()->This()->TryTransmit( _T( "CBcam::FormatSeven::MaxSize()" ),
    IOCTL_BCAM_FMT7_GET_MODE_DESCRIPTOR, &arg, sizeof(arg), &res, sizeof(res) );
  return CSize( res.nTotalPixX, res.nTotalPixY );
}


/*----------------------------------------------------------------*/

//------------------------------------------------------------------------------
// void CBcam::CFormatSeven::_FormatSeven::CImagePosition::operator=(CPoint Value)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Set the image position.
*
* \param     Value The position to set.
*
* \b Example
* \code
* m_pBcam->FormatSeven[0].Position = CPoint( 100, 50 );
* \endcode
*/
//------------------------------------------------------------------------------
void CBcam::CFormatSeven::_FormatSeven::CImagePosition::operator=(CPoint Value)
{
  ArgFmt7SetAOI arg;
  arg.AOI.X0 = Value.x;
  arg.AOI.Y0 = Value.y;
  arg.Mask = BCAM_RECTCOMP_POSITION;
  arg.VideoMode = This()->m_Mode;
  This()->This()->This()->TryTransmit( _T( "CBcam::FormatSeven::CImagePosition::operator=()" ), IOCTL_BCAM_FMT7_SET_AOI, &arg, sizeof(arg));
}

//------------------------------------------------------------------------------
// CPoint CBcam::CFormatSeven::_FormatSeven::CImagePosition::operator()()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the image position
*
* \return    
*
* Returns the current image position
* 
* \b Example
* \code
* CPoint position = m_pBcam->FormatSeven[0].Position();
* \endcode
*/
//------------------------------------------------------------------------------
CPoint CBcam::CFormatSeven::_FormatSeven::CImagePosition::operator()()
{
  ArgFmt7GetAOI arg;
  ResFmt7GetAOI res;
  arg.VideoMode = This()->m_Mode;
  This()->This()->This()->TryTransmit( _T( "CBcam::FormatSeven::CImagePosition::operator()()" ), IOCTL_BCAM_FMT7_GET_AOI, &arg, sizeof(arg), &res, sizeof(res) );
  return CPoint(res.X0, res.Y0);
}

//------------------------------------------------------------------------------
// CSize CBcam::CFormatSeven::_FormatSeven::CImagePosition::Inc()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the unit size of the image position.
*
* \return    
*
* Returns the allowed increment.
* 
* \b Example
* \code 
* CSize unitsize = m_pBcam->FormatSeven[0].Position.Inc();
* \endcode
*/
//------------------------------------------------------------------------------
CSize CBcam::CFormatSeven::_FormatSeven::CImagePosition::Inc()
{
  ArgFmt7GetModeDescriptor arg = {This()->m_Mode};
  ResFmt7GetModeDescriptor res;
  This()->This()->This()->TryTransmit( _T( "CBcam::FormatSeven::CImagePosition::Inc()" ),
    IOCTL_BCAM_FMT7_GET_MODE_DESCRIPTOR, &arg, sizeof(arg),&res, sizeof(res) );
  CSize size;
  size.cx = res.nPosIncX != 0 ? res.nPosIncX : res.nSizeIncX;
  size.cy = res.nPosIncY != 0 ? res.nPosIncY : res.nSizeIncY;
  return size;
}

//------------------------------------------------------------------------------
// void CBcam::CFormatSeven::_FormatSeven::CImagePosition::SetAsync(CPoint Value, void *pContext)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Set the image position asynchronously
*
* \param     Value       The position to set.
* \param     *pContext   Pointer to context information, that will be returned after sending. (optional)
* \return    
*
* <type Return description here>
* 
* \b Example
* \code
* m_pBcam->FormaSeven[0].Position.SetAsync( CPoint( 100, 50 ), &InfoBlock );
* \endcode
*/
//------------------------------------------------------------------------------
void CBcam::CFormatSeven::_FormatSeven::CImagePosition::SetAsync(CPoint Value, void *pContext)
{
  ArgFmt7SetAOI arg;
  arg.AOI.X0 = Value.x;
  arg.AOI.Y0 = Value.y;
  arg.Mask = BCAM_RECTCOMP_POSITION;
  arg.VideoMode = This()->m_Mode;

  BcamOL *pOL = GetNewBcamOL(AsyncPositionSet, pContext );

  if (! This()->This()->This()->Transmit( IOCTL_BCAM_FMT7_SET_AOI, &arg, sizeof arg, pOL ))
  {
    DWORD error = ::GetLastError();
    if (error != ERROR_IO_PENDING)
      throw BcamException( ::GetLastError(), _T( "CBcam::FormatSeven::CImagePosition::SetAsync()" ) );
  }
}

/*----------------------------------------------------------------*/

//------------------------------------------------------------------------------
// void CBcam::CFormatSeven::_FormatSeven::CImageSize::operator=(CSize Value)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Set the image size synchronously
*
* \param     Value The image size to set

* 
* \b Example
* \code 
* m_pBcam->FormatSeven[0].Size = CSize( 300, 400 );
* \endcode
*/
//------------------------------------------------------------------------------
void CBcam::CFormatSeven::_FormatSeven::CImageSize::operator=(CSize Value)
{
  ArgFmt7SetAOI arg;
  arg.AOI.Xsize = Value.cx;
  arg.AOI.Ysize = Value.cy;
  arg.Mask = BCAM_RECTCOMP_SIZE;
  arg.VideoMode = This()->m_Mode;
  This()->This()->This()->TryTransmit( _T( "CBcam::FormatSeven::CImageSize::operator=()" ),
    IOCTL_BCAM_FMT7_SET_AOI, &arg, sizeof(arg) );
}

//------------------------------------------------------------------------------
// CSize CBcam::CFormatSeven::_FormatSeven::CImageSize::operator()()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the image size synchronously
*
* \return    
*
* Returns the current value;
* 
* \b Example
* \code
* CSize imgsize = m_pBcam->FormatSeven[0].Size();
* \endcode
*/
//------------------------------------------------------------------------------
CSize CBcam::CFormatSeven::_FormatSeven::CImageSize::operator()()
{
  ArgFmt7GetAOI arg;
  ResFmt7GetAOI res;
  arg.VideoMode = This()->m_Mode;
  This()->This()->This()->TryTransmit( _T( "CBcam::FormatSeven::CImageSize::operator()()" ),
    IOCTL_BCAM_FMT7_GET_AOI, &arg, sizeof(arg), 
    &res, sizeof(res) );
   return CSize(res.Xsize, res.Ysize);
}

//------------------------------------------------------------------------------
// CSize CBcam::CFormatSeven::_FormatSeven::CImageSize::Inc()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the unit size of the image size.
*
* \return    
*
* Returns the current unit size;
* 
* \b Example
* \code
* CSize unitsize = m_pBcam->FormatSeven[0].Size.Inc();
* \endcode
*/
//------------------------------------------------------------------------------
CSize CBcam::CFormatSeven::_FormatSeven::CImageSize::Inc()
{
  ArgFmt7GetModeDescriptor arg = {This()->m_Mode};
  ResFmt7GetModeDescriptor res;
  This()->This()->This()->TryTransmit( _T( "CBcam::FormatSeven::CImageSize.Inc()" ),
    IOCTL_BCAM_FMT7_GET_MODE_DESCRIPTOR, &arg, sizeof(arg), &res, sizeof(res));
  return CSize( res.nSizeIncX, res.nSizeIncY );
}

//------------------------------------------------------------------------------
// void CBcam::CFormatSeven::_FormatSeven::CImageSize::SetAsync(CSize Value, void *pContext)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Set the image size asynchronously.
*
* \param     Value       The image size to set
* \param     *pContext   Pointer to the context that will be returned afterwards. (optional)
* 
* \b Example
* \code
* m_pBcam->FormatSeven[0].Size.SetAsync( CSize( 100, 100 ), &InfoBlock );
* \endcode
*/
//------------------------------------------------------------------------------
void CBcam::CFormatSeven::_FormatSeven::CImageSize::SetAsync(CSize Value, void *pContext)
{
  ArgFmt7SetAOI arg;
  arg.AOI.Xsize = Value.cx;
  arg.AOI.Ysize = Value.cy;
  arg.Mask = BCAM_RECTCOMP_SIZE;;
  arg.VideoMode = This()->m_Mode;

  BcamOL *pOL = GetNewBcamOL(AsyncSizeSet, pContext );

  if (! This()->This()->This()->Transmit( IOCTL_BCAM_FMT7_SET_AOI, &arg, sizeof arg, pOL ))
  {
    DWORD error = ::GetLastError();
    if (error != ERROR_IO_PENDING)
      throw BcamException( ::GetLastError(), _T( "CBcam::FormatSeven::CImagePosition::SetAsync()" ) );
  }

}

/*----------------------------------------------------------------*/

//------------------------------------------------------------------------------
// bool CBcam::CFormatSeven::_FormatSeven::CColorCoding::IsSupported(DCSColorCode ColorCode)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Inquire whether the image type is supported.
*
* \param     ColorCode Code of the image type
* \retval true if it is supported
* \retval false if it is not supported
*/
//------------------------------------------------------------------------------
bool CBcam::CFormatSeven::_FormatSeven::CColorCoding::IsSupported(DCSColorCode ColorCode)
{
  ArgFmt7GetModeDescriptor arg = {This()->m_Mode};
  ResFmt7GetModeDescriptor res;
  This()->This()->This()->TryTransmit( _T( "CBcam::CFormatSeven::CColorCoding::IsSupported()" ),
    IOCTL_BCAM_FMT7_GET_MODE_DESCRIPTOR, &arg, sizeof(arg),&res, sizeof(res) );

  const int NumCodesPerQuadlet = 32;
  const int quad = ColorCode / NumCodesPerQuadlet;
  const int bit = ColorCode % NumCodesPerQuadlet;
  assert( quad < sizeof res.ColorCodingMask / sizeof res.ColorCodingMask[0] );
  return (res.ColorCodingMask[quad] & 0x80000000 >> bit) != 0; 
}

//------------------------------------------------------------------------------
// void CBcam::CFormatSeven::_FormatSeven::CColorCoding::operator=(DCSColorCode Value)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Set the image type
*
* \param     Value Code of the image type to set
*/
//------------------------------------------------------------------------------
void CBcam::CFormatSeven::_FormatSeven::CColorCoding::operator=(DCSColorCode Value)
{
  assert( Value >= DCSColor_Mono8 && Value <= DCSColor_Raw16 || Value >= DCSColor_VendorSpecific0 && Value <= DCSColor_VendorSpecific127 );
  ArgFmt7SetColorMode arg;
  arg.VideoMode = This()->m_Mode;
  arg.ColorMode = Value;
  This()->This()->This()->TryTransmit( _T( "CBcam::FormatSeven::CColorCoding::operator=()" ), IOCTL_BCAM_FMT7_SET_COLOR_MODE, &arg, sizeof(arg) );
}

//------------------------------------------------------------------------------
// DCSColorCode CBcam::CFormatSeven::_FormatSeven::CColorCoding::operator()()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the image type
*
* \return    
*
* Returns the code of the currently selected image type.
* 
*/
//------------------------------------------------------------------------------
DCSColorCode CBcam::CFormatSeven::_FormatSeven::CColorCoding::operator()()
{
  ArgFmt7GetColorMode arg;
  ResFmt7GetColorMode res;
  arg.VideoMode = This()->m_Mode;

  This()->This()->This()->TryTransmit( _T( "CBcam::FormatSeven::CColorCoding::operator()()" ),
    IOCTL_BCAM_FMT7_GET_COLOR_MODE, &arg,  sizeof(arg), &res, sizeof(res) );

  if (res.ColorMode < DCSColor_VendorSpecific0 && (res.ColorMode < DCSColor_Mono8 || res.ColorMode > DCSColor_Raw16))
    throw BcamException(BCAM_E_UNSUPPORTED_COLOR_ID, _T( "CBcam::FormatSeven::ColorCoding::operator()" ), &res.ColorMode);

  return (DCSColorCode) res.ColorMode;
}

/*----------------------------------------------------------------*/

//------------------------------------------------------------------------------
// unsigned long CBcam::CFormatSeven::_FormatSeven::PixelPerFrame()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the number of pixel in a image
*
* \return    
*
* Returns the number of pixel.
*/
//------------------------------------------------------------------------------
unsigned long CBcam::CFormatSeven::_FormatSeven::PixelPerFrame()
{
  /* There is no way to ask the driver for this value") */

  return Size().cx * Size().cy;
}

//------------------------------------------------------------------------------
// unsigned long CBcam::CFormatSeven::_FormatSeven::BytePerFrame()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the number of bytes in an image
*
* \return    
*
* Returns the number of bytes.
*/

//------------------------------------------------------------------------------
unsigned long CBcam::CFormatSeven::_FormatSeven::BytePerFrame()
{
  ArgGetBandwidthInfo arg;
  ResGetBandwidthInfo res;
  arg.VideoMode = m_Mode;
  This()->This()->TryTransmit( _T( "CBcam::FormatSeven::BytePerFrame()" ),
    IOCTL_BCAM_GET_BANDWIDTH_INFO, &arg, sizeof(arg),  &res, sizeof(res) );
  return (unsigned long) res.nBytesTotalFrameSize;  
}

//------------------------------------------------------------------------------
// unsigned long CBcam::CFormatSeven::_FormatSeven::PacketsPerFrame()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the number of packets for an image
*
* \return    
*
* Returns the number packets.
*/
//------------------------------------------------------------------------------
unsigned long CBcam::CFormatSeven::_FormatSeven::PacketsPerFrame()
{
  ArgGetBandwidthInfo arg;
  ResGetBandwidthInfo res;
  arg.VideoMode = m_Mode;
  This()->This()->TryTransmit( _T( "CBcam::FormatSeven::PacketsPerFrame()" ),
    IOCTL_BCAM_GET_BANDWIDTH_INFO, &arg, sizeof(arg),&res, sizeof(res) );
  return res.nPacketsPerFrame;
}

/*----------------------------------------------------------------*/

//------------------------------------------------------------------------------
// unsigned long CBcam::CFormatSeven::_FormatSeven::CBytePerPacket::Inc()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the unit size for the packet size.
*
* \return    
*
* Returns the unit size.
* 
* \throw BcamException The value of \c ::GetLastError() is thrown.
* 
*/
//------------------------------------------------------------------------------
unsigned long CBcam::CFormatSeven::_FormatSeven::CBytePerPacket::Inc()
{
  ArgGetBandwidthInfo Arg;
  ResGetBandwidthInfo Res;
  Arg.VideoMode = This()->m_Mode;
  This()->This()->This()->TryTransmit( _T( "CBcam::CFormatSeven::CBytePerPacket::Inc()" ),
    IOCTL_BCAM_GET_BANDWIDTH_INFO, &Arg, sizeof(Arg), &Res, sizeof(Res) );
  return Res.nBytesPerPacketUnit;
}

//------------------------------------------------------------------------------
// unsigned long CBcam::CFormatSeven::_FormatSeven::CBytePerPacket::Max()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the maximum packet size.
*
* \return    
*
* Return the number of bytes
* 
* \throw BcamException The value of \c ::GetLastError() is thrown.
*/
//------------------------------------------------------------------------------
unsigned long CBcam::CFormatSeven::_FormatSeven::CBytePerPacket::Max()
{
  ArgGetBandwidthInfo Arg;
  ResGetBandwidthInfo Res;
  Arg.VideoMode = This()->m_Mode;
  This()->This()->This()->TryTransmit( _T( "CBcam::CFormatSeven::CBytePerPacket::Max()" ),
    IOCTL_BCAM_GET_BANDWIDTH_INFO, &Arg, sizeof(Arg), &Res, sizeof(Res) );
  return Res.nBytesPerPacketMax;
}

//------------------------------------------------------------------------------
// unsigned long CBcam::CFormatSeven::_FormatSeven::CBytePerPacket::Rec()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the recommended packetsize.
*
* \return    
*
* Returns the number of bytes.
* 
* \throw BcamException The value of \c ::GetLastError() is thrown
*/
//------------------------------------------------------------------------------
unsigned long CBcam::CFormatSeven::_FormatSeven::CBytePerPacket::Rec()
{
  ArgGetBandwidthInfo Arg;
  ResGetBandwidthInfo Res;
  Arg.VideoMode = This()->m_Mode;
  This()->This()->This()->TryTransmit( _T( "CBcam::CFormatSeven::CBytePerPacket::Rec()" ),
    IOCTL_BCAM_GET_BANDWIDTH_INFO, &Arg, sizeof(Arg), &Res, sizeof(Res) );
  if ( Res.nBytesPerPacketRec != 0 )
    return Res.nBytesPerPacketRec;
  else
    return Res.nBytesPerPacketMax;
}

//------------------------------------------------------------------------------
// void CBcam::CFormatSeven::_FormatSeven::CBytePerPacket::operator=(unsigned long Value)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Set the packet size, controls the framerate.
*
* \param     Value The number of bytes per packet. The value must be both a multiple of four and a 
*            multiple of #FormatSeven[mode].BytePerPacket.Inc()
*/
//------------------------------------------------------------------------------
void CBcam::CFormatSeven::_FormatSeven::CBytePerPacket::operator=(unsigned long Value)
{
  ArgSetCameraBytesPerPacket arg;
  arg.nBytesPerPacket = Value;
  arg.VideoMode = This()->m_Mode;

  This()->This()->This()->TryTransmit( _T( "CBcam::FormatSeven::CBytePerPacket::operator=()" ), IOCTL_BCAM_SET_CAMERA_BYTES_PER_PACKET, &arg, sizeof(arg) );
}

//------------------------------------------------------------------------------
// unsigned long CBcam::CFormatSeven::_FormatSeven::CBytePerPacket::operator()()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the packet size.
*
* \return    
*
* Returns the number of bytes per packet.
*/
//------------------------------------------------------------------------------
unsigned long CBcam::CFormatSeven::_FormatSeven::CBytePerPacket::operator()()
{
  ArgGetBandwidthInfo Arg;
  ResGetBandwidthInfo Res;
  Arg.VideoMode = This()->m_Mode;
  This()->This()->This()->TryTransmit( _T( "CBcam::CFormatSeven::CBytePerPacket::operator()()" ),
    IOCTL_BCAM_GET_BANDWIDTH_INFO, &Arg, sizeof(Arg), &Res, sizeof(Res) );
    return Res.nBytesPerPacketCurrent; 
}

//------------------------------------------------------------------------------
// void CBcam::CFormatSeven::_FormatSeven::CBytePerPacket::SetAsync(unsigned long Value, void *pContext)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Set the packet size asychronously
*
* \param     Value      The number of bytes per packet
* \param     *pContext  Pointer to context information that is returned afterwards.
*/
//------------------------------------------------------------------------------
void CBcam::CFormatSeven::_FormatSeven::CBytePerPacket::SetAsync(unsigned long Value, void *pContext)
{
  ArgSetCameraBytesPerPacket arg;
  arg.nBytesPerPacket = Value;
  arg.VideoMode = This()->m_Mode;

  BcamOL *pOL = GetNewBcamOL(AsyncBytePerPacketSet, pContext );
  
  if (! This()->This()->This()->Transmit( IOCTL_BCAM_SET_CAMERA_BYTES_PER_PACKET, &arg, sizeof arg, pOL ))
  {
    DWORD error = ::GetLastError();
    if (error != ERROR_IO_PENDING)
      throw BcamException( ::GetLastError(), _T( "CBcam::FormatSeven::CBytePerPacket::SetAsync()" ) );
  }
}

//------------------------------------------------------------------------------
// void CBcam::CFormatSeven::_FormatSeven::Save(const CPropertyBagPtr ptrBag)
// Author: A.Happe
// Date: 04.09.2002
//------------------------------------------------------------------------------
/**
 * \brief Save the format7 settings to a property bag
 *
 * \param     ptrBag
 */
//------------------------------------------------------------------------------

void CBcam::CFormatSeven::_FormatSeven::Save(const CPropertyBagPtr ptrBag)
{
  ptrBag->WriteLong( _T( "BytePerPacket" ), BytePerPacket());
  ptrBag->WriteLong( _T( "ColorCoding" ), ColorCoding());
  CPoint position = Position();
  CSize size = Size();
  ptrBag->WriteLong( _T( "Pos_x" ), position.x);
  ptrBag->WriteLong( _T( "Pos_y" ), position.y);
  ptrBag->WriteLong( _T( "Size_cx" ), size.cx);
  ptrBag->WriteLong( _T( "Size_cy" ), size.cy);
}

//------------------------------------------------------------------------------
// void CBcam::CFormatSeven::_FormatSeven::Restore(const CPropertyBagPtr ptrBag)
// Author: A.Happe 
// Date: 04.09.2002
//------------------------------------------------------------------------------
/**
 * \brief Restore the format7 settings from a property bag
 *
 * \param     ptrBag
 */
//------------------------------------------------------------------------------

void CBcam::CFormatSeven::_FormatSeven::Restore(const CPropertyBagPtr ptrBag)
{
  BytePerPacket = ptrBag->ReadLong( _T( "BytePerPacket" ) );
  ColorCoding = (DCSColorCode) ptrBag->ReadLong( _T( "ColorCoding" ) );
  Position = CPoint( ptrBag->ReadLong( _T( "Pos_x" ) ), ptrBag->ReadLong( _T( "Pos_y" ) ) );
  Size = CSize( ptrBag->ReadLong( _T( "Size_cx" ) ), ptrBag->ReadLong( _T( "Size_cy" ) ) );
}

/************************************************************************************************************/


/************************************************************************************************************/



/**********************************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CBcam

//------------------------------------------------------------------------------
// CBcam::CBcam()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Constructs a camera object without opening the driver.
*
*/
//------------------------------------------------------------------------------
CBcam::CBcam()
  : m_hCamera( INVALID_HANDLE_VALUE ),
  m_hDevHandleNotify( INVALID_HANDLE_VALUE ),
  m_hDevHandleNotifyOpt( INVALID_HANDLE_VALUE),
  m_hWndHandleNotify( NULL ),
  m_hCompletionPort( NULL ),
  m_CompletionKey( s_CompletionKey++ ),
  m_IsoSpeed( IsoSpeed_Unknown ),
  m_IsoChannel( -1 ),
  m_OnRemoveCompleteCallback(NULL),
  m_pvOnRemoveComplete(NULL),
  m_OnRemoveRequestCallback(NULL),
  m_pvOnRemoveRequest(NULL),
  Shutter( AsyncShutterRawSet, AsyncShutterAbsSet,FeatureID_Shutter, this ),
  Brightness( AsyncBrightnessRawSet, AsyncBrightnessAbsSet, FeatureID_Brightness, this ),
  Gain( AsyncGainRawSet, AsyncGainAbsSet, FeatureID_Gain, this ),
  Hue( AsyncHueRawSet, AsyncHueAbsSet, FeatureID_Hue, this),
  Saturation( AsyncSaturationRawSet, AsyncSaturationAbsSet, FeatureID_Saturation, this),
  Gamma( AsyncGammaRawSet, AsyncGammaAbsSet, FeatureID_Gamma, this),
  Trigger( AsyncTriggerRawSet, AsyncTriggerAbsSet, this)
{
  TRACE( _T( "BCAMApi: CBcam::CBcam()\n" ) );
}

//------------------------------------------------------------------------------
// CBcam::~CBcam()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Destructor - closes the driver
*
*/
//------------------------------------------------------------------------------
CBcam::~CBcam()
{
  TRACE( _T( "BCAMApi: CBcam::~CBcam()\n" ) );
  Close();
}

// CBcamDevInfo: Helper class used by CBcam::DeviceNames()

//------------------------------------------------------------------------------
// class CBcamDevInfo 
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Helps with the list of devices.
*
*/
//------------------------------------------------------------------------------
class CBcamDevInfo 
{
  HDEVINFO m_hDevInfo; ///< Handle to the device information
public:
  
  //------------------------------------------------------------------------------
  // CBcamDevInfo( GUID& classguid )
  // Author: 
  //------------------------------------------------------------------------------
  /**
  * \brief Create a device list.
  *
  * \param     classguid Identifies the device interface
  * \throw BcamException The value of \c ::GetLastError() is thrown.
  */
  //------------------------------------------------------------------------------
  CBcamDevInfo( GUID& classguid )
    : m_hDevInfo( ::SetupDiGetClassDevs( 
    &classguid, 
    NULL, 
    NULL, 
    DIGCF_DEVICEINTERFACE | DIGCF_PRESENT) )
    
  {
    if (m_hDevInfo == INVALID_HANDLE_VALUE)
      throw BcamException( ::GetLastError(), _T( "CBcamDevInfo" ) );
  }
  //------------------------------------------------------------------------------
  // ~CBcamDevInfo()
  // Author: 
  //------------------------------------------------------------------------------
  /**
  * \brief Destroy the device list
  */
  //------------------------------------------------------------------------------
  ~CBcamDevInfo()
  {
    if (m_hDevInfo != INVALID_HANDLE_VALUE) 
    {
      SetupDiDestroyDeviceInfoList( m_hDevInfo );
      m_hDevInfo = INVALID_HANDLE_VALUE;
    }
  }
  /// Return the handle to the device list.
  operator HDEVINFO() { return m_hDevInfo; }
};


//------------------------------------------------------------------------------
// std::list<CString> CBcam::DeviceNames()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Create a list of camera devices
*
* \return    
*
* Returns a list of devices.

* 
* \throw BcamException The value of \c ::GetLastError() is thrown.
*/
//------------------------------------------------------------------------------
std::list<CString> CBcam::DeviceNames()
{
  return DeviceNames( std::list<CString>() );
}

//------------------------------------------------------------------------------
// std::list<CString>& CBcam::DeviceNames( std::list<CString>& deviceNames )
// Author: 
// Date: 05.09.2002
//------------------------------------------------------------------------------
/**
 * \brief Fill a list of camera device names
 *
 * \param     deviceNames
 * \return    
 * Returns a list of devices.
 * 
 * \throw BcamException The value of \c ::GetLastError() is thrown.
 */
//------------------------------------------------------------------------------
std::list<CString>& CBcam::DeviceNames( std::list<CString>& deviceNames )
{
  GUID classguid = GUID_FILE_DEVICE_BCAM_1394;
  CBcamDevInfo hDevInfo( classguid );
  
  CDriverVersionCheck::Match();

  deviceNames.clear();
  
  DWORD i=0;
  DWORD res = ERROR_SUCCESS;
  do
  {
    SP_DEVICE_INTERFACE_DATA DevInterfaceData;
    DevInterfaceData.cbSize = sizeof DevInterfaceData;
    if (::SetupDiEnumDeviceInterfaces( hDevInfo, NULL, &classguid, i, &DevInterfaceData ))
    {
      DWORD reqSize;
      ::SetupDiGetDeviceInterfaceDetail( hDevInfo, &DevInterfaceData, NULL, 0, &reqSize, NULL );
      if (ERROR_INSUFFICIENT_BUFFER != (res = ::GetLastError()) )
        throw BcamException( res, _T( "CBcamDevMgr::DeviceNames" ) );
      
      char * const pBuffer = (char*) ::_alloca( reqSize );
      PSP_DEVICE_INTERFACE_DETAIL_DATA pDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA) pBuffer;
      pDetail->cbSize = sizeof *pDetail;
      if (::SetupDiGetDeviceInterfaceDetail( hDevInfo, &DevInterfaceData, pDetail, reqSize, &reqSize, NULL ))
      {
        deviceNames.push_back( BcamUtility::FriendlyDeviceName(pDetail->DevicePath) );
        i++;
      }
    }
  } while (res = GetLastError(), res == ERROR_SUCCESS);
  
  if (res != ERROR_NO_MORE_ITEMS )
    throw BcamException( res, _T( "CBcam::DeviceNames" ) );

  return deviceNames;
  
}
//------------------------------------------------------------------------------
// void CBcam::RegisterClient(HWND hwnd)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Registers a client window to recieve device notifications
*
* This function must be called before opening a device, otherwise an exception 
* will be fired.
*
* \param     hwnd Handle to the client window
* 
* \throw BcamException The value of \c ::GetLastError() is thrown.
*/
//------------------------------------------------------------------------------
void CBcam::RegisterClient(HWND hwnd)
{
  CLock lck( s_TheCriticalSection );
  if ( s_BcamMap.size() != 0 )
  {
    // Register client must be called before opening a device!!
    throw BcamException(BCAM_E_REGISTER_CLIENT_NOT_ALLOWED);
  }

  if ( hwnd == NULL )
    return;

  // deregister a previously registered window
  if ( s_hWndNotify != NULL )
    UnregisterClient();

  // subclass the window (in order to issue the callbacks)
  s_lpPrevWndFunc = (WNDPROC) SetWindowLong(hwnd, GWL_WNDPROC, (LONG) BcamWindowProc);
  if ( s_lpPrevWndFunc == NULL )
  {
    DWORD err = GetLastError();
    throw BcamException( BCAM_E_REGISTER_DEVICE_NOTIFICATION_FAILED, _T( "CBcam::RegisterClient()" ), &err );
  }

  // register window for device notifications

  DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
  ZeroMemory( &NotificationFilter, sizeof(NotificationFilter) );
  NotificationFilter.dbcc_size = 
    sizeof(DEV_BROADCAST_DEVICEINTERFACE);
  NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
  NotificationFilter.dbcc_classguid = GUID_FILE_DEVICE_BCAM_1394;
  s_hWndNotify = hwnd;
  s_hDevInterfaceNotify = ::RegisterDeviceNotification( hwnd, &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE) ;
  if( ! s_hDevInterfaceNotify ) 
  {
    DWORD err = GetLastError();
    throw BcamException( BCAM_E_REGISTER_DEVICE_NOTIFICATION_FAILED, _T( "CBcam::RegisterClient()" ), &err );
  }
}

//------------------------------------------------------------------------------
// void CBcam::UnregisterClient()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Unregister the window registered to receive device notifications
*
* The function must not be called if there are any opened devices. 
*
* \throw BcamException The value of \c ::GetLastError() is thrown.
*/
//------------------------------------------------------------------------------
void CBcam::UnregisterClient()
{
  CLock lck( s_TheCriticalSection );
  if ( s_BcamMap.size() != 0 )
  {
    // Unregister client must be called after closing all devices!
    throw BcamException(BCAM_E_UNREGISTER_CLIENT_NOT_ALLOWED);
  }

  InternalUnregisterClient();
}

//------------------------------------------------------------------------------
// void CBcam::CleanUp()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Do some clean up
*
* This clean up function unregisters the window registered to receive device notifications. 
* If a hidden message-only window was registered, it will be closed and its associated 
* background thread driving the message loop of this window will terminate.
*
* When using the Bcam API in a dynamic link library, it is recommended to call CBcam::CleanUp() 
* before unloading the dll. The function must not be called from the dll's DllMain() function!
*
* If the Bcam API is not used in a dynamic link library, there is no need to call CleanUp(). 
* The function will be called automatically. 
*
* Calling CleanUp() when there are any opened device is not allowed and will issue a BcamException.
*
*  
*/
//------------------------------------------------------------------------------
void CBcam::CleanUp()
{
  CLock lck( s_TheCriticalSection );
  // Unregister client windows
  InternalUnregisterClient();

  // Close an existing hidden message-only window. Closing it will also
    // cause the message loop thread to terminate.
  if ( s_hWndMsgOnly != NULL )
  {
    assert( (HANDLE) s_MessageLoopThread != NULL );
    PostMessage(s_hWndMsgOnly, WM_CLOSE, 0, 0);
    s_MessageLoopThread.Release(50);  // Wait for the thread's termination. Enforce termination if necessary.
    s_hWndMsgOnly = NULL;
  }
}

//------------------------------------------------------------------------------
// void CBcam::SetCurrentThreadPriority(unsigned long priority)
// Author: 
//------------------------------------------------------------------------------
/**
 * \brief Set the priority level of the current thread
 * 
 * Use this function to raise the priority level to a real-time priority level ( > 15 ), without
 * being forced to change the priority class of the process (which would raise the 
 * priorities of all other threads in the process too).
 *
 * When manipulating the current thread's priority, be very careful to ensure that the
 * high-priority thread does not consume all of the available CPU time. A thread with a 
 * priority level above 11 interferes with the normal operation of the operating system. 
 * Using values greater than 15 may cause disk caches to not flush, hang the mouse, and so on. 
 *
 * It is not allowed to use this function for threads of REALTIME_PRIORITY_CLASS processes (an 
 * BcamException will be thrown). For REALTIME_PRIORITY_CLASS processes the WinApi SetThreadPriority
 * must be used instead.
 *
 * \param     priority New priority level of the current Thread [16..31]. 
 * \return    void
 * \throw BcamException The value of GetLastError, if the driver call fails
 * \throw BcamException BCAM_E_NOT_ALLOWED_FOR_REALTIME_CLASS when calling the function for a REALTIME_PRIORITY_CLASS process
 * 
 */
//------------------------------------------------------------------------------
void CBcam::SetCurrentThreadPriority(unsigned long priority)
{
  if ( priority <16 || priority > 31)
    throw BcamException( ERROR_INVALID_PARAMETER, _T( "CBcam::SetCurrentThreadPriority" ) );

  // don't allow this function for REALTIME_PRIORITY_CLASS processes
  HANDLE hProcess = GetCurrentProcess();
  int priorityClass = GetPriorityClass(hProcess);
  if ( priorityClass == REALTIME_PRIORITY_CLASS )
  {
    throw BcamException( BCAM_E_NOT_ALLOWED_FOR_REALTIME_CLASS, _T( "CBcam::SetCurrentThreadPriority" ) );
  } 

  // Before the driver is able to raise the priority level above 15, 
  // we must set the thread's priority to 15
  if ( ! SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL) )
  {
    throw BcamException( ::GetLastError(), _T( "CBcam::SetCurrentThreadPriority" ) );
  }

  // Now the driver will lift the priority level to the desired value
  ArgSetUserThreadPriority arg = priority;
  TryTransmit( _T( "CBcam::SetCurrentThreadPriority" ),
    IOCTL_BCAM_SET_USER_THREAD_PRIORITY, &arg, sizeof ArgSetUserThreadPriority );
}

//------------------------------------------------------------------------------
// unsigned long CBcam::SetQueueServerPriority(unsigned long priority)
// Author: 
// Date: 11.09.2002
//------------------------------------------------------------------------------
/**
 * \brief Set the priority of the driver's queue server thread
 *
 * The BCAM driver's queue server thread dispatches the I/O requests. 
 * #SetQueueServerPriority offers the possibility to raise 
 * the priority level of this thread above other user threads. The queue server thread
 * runs at least at a priority level of 16. Setting values lower than 16 is not allowed.
 * Normally there is no need to manipulate the queue server's thread priority. Only when
 * a user thread is set to a priority level higher than the queue server thread's one, 
 * the queue server thread's priority should be raised. 
 *
 * Use the function 
 * #CBcam::GetQueueServerPriority() to determine the current priority.
 *
 * \param     priority New priority level of the queue server thread [16..31]
 * \return    The previous priority level of the queue server thread
 * 
 */
//------------------------------------------------------------------------------

unsigned long CBcam::SetQueueServerPriority(unsigned long priority)
{
  ArgSetQueueServerPriority arg = priority;
  ResSetQueueServerPriority res;
  TryTransmit( _T( "CBcam::SetQueueServerPriority" ),
    IOCTL_BCAM_SET_QUEUE_SERVER_PRIORITY,
    &arg, sizeof ArgSetQueueServerPriority, 
    &res, sizeof ResSetQueueServerPriority );
  return res;
}


//------------------------------------------------------------------------------
// unsigned long CBcam::GetQueueServerPriority();
// Author: 
// Date: 26.09.2002
//------------------------------------------------------------------------------
/**
 * \brief Retrieves the current tpriority level of the driver's queue server thread
 *
 * \param     priority
 * \return    The priority level
 *
 */
//------------------------------------------------------------------------------
unsigned long CBcam::GetQueueServerPriority()
{
  ResSetQueueServerPriority res;
  TryTransmit( _T( "CBcam::GetQueueServerPriority" ),
    IOCTL_BCAM_GET_QUEUE_SERVER_PRIORITY,NULL, 0, &res, sizeof ResGetQueueServerPriority );
 
  return res;
}

//------------------------------------------------------------------------------
// LRESULT CBcam::OnDeviceChange(WPARAM wParam, LPARAM lParam)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Filter for WM_DEVICECHANGE messages.
*
* \param     wParam wParam of the WM_DEVICECHANGE message
* \param     lParam lParam of the WM_DEVICECHANGE message
* \return    
*
* This function is out of date and exists for compatibility reasons only. 
* In previous versions of CBcam, calling this function was neccessary
* to issue the RemoveRequest and RemoveComplete callbacks. In the current version, these
* callbacks are issued without calling this function.
* 
* Returns TRUE if information concerns one of our opened devices.
* 
*/
//------------------------------------------------------------------------------
LRESULT CBcam::OnDeviceChange(WPARAM wParam, LPARAM lParam)
{
  CLock lck( s_TheCriticalSection );
  PDEV_BROADCAST_HDR  devHdr;
  PDEV_BROADCAST_HANDLE devHandle;
  
  devHdr = (PDEV_BROADCAST_HDR)lParam;
  devHandle = (PDEV_BROADCAST_HANDLE) lParam;
  
  switch (wParam) 
  {
  case DBT_DEVICEQUERYREMOVE: 
    if ( devHdr->dbch_devicetype != DBT_DEVTYP_HANDLE )
      return FALSE;  // we are only interested in handle related device notification
    if ( s_BcamMap.find(devHandle->dbch_handle) != s_BcamMap.end() )
      return TRUE;
    break;
  case DBT_DEVICEQUERYREMOVEFAILED: 
    if ( devHdr->dbch_devicetype != DBT_DEVTYP_HANDLE )
      return FALSE;  // we are only interested in handle related device notification
    if ( s_BcamMap.find((HANDLE) devHandle->dbch_handle) != s_BcamMap.end() )
      return TRUE;
    break;
  case DBT_DEVICEREMOVECOMPLETE: 
    if ( devHdr->dbch_devicetype != DBT_DEVTYP_HANDLE )
      return FALSE;  // we are only interested in handle related device notification
    if ( s_BcamMap.find((HANDLE) devHandle->dbch_handle) != s_BcamMap.end() )
      return TRUE;
    break;
  case DBT_DEVICEREMOVEPENDING: 
    if ( devHdr->dbch_devicetype == DBT_DEVTYP_HANDLE )
      return TRUE;  // we are only interested in handle related device notification
    return FALSE;
  default: 
    return FALSE;   // we did not process the notification
  }
  
  return FALSE;
}

//------------------------------------------------------------------------------
// void CBcam::SetOnRemoveRequestCallback( BcamCallbackFunc f, void pv )
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Set a callback function which will called if the device is to be removed.
*
*        When entering the callback function, the device's file handle is still
*        valid, although the camera device already is removed 
*        from the 1394 bus. For example, in the callback function a client 
*        might remove pending I/O packets from the completion port.
*
* \param     f pointer to callback function
* \param     pv void pointer which will reached in to the callback function
* 
*/
//------------------------------------------------------------------------------
void CBcam::SetOnRemoveRequestCallback( BcamCallbackFunc f, void* pv )
{
  m_OnRemoveRequestCallback = f;
  m_pvOnRemoveRequest = pv;
}

//------------------------------------------------------------------------------
// void CBcam::SetOnRemoveCompleteCallback( BcamCallbackFunc f, void pv )
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Set a callback function which will called if the device has been removed
*
* When entering the callback function, the device has been closed. 
*
* \param     f pointer to callback function
* \param     pv void pointer which will reached in to the callback function
* 
*/
//------------------------------------------------------------------------------
void CBcam::SetOnRemoveCompleteCallback( BcamCallbackFunc f, void* pv )
{
  m_OnRemoveCompleteCallback = f;
  m_pvOnRemoveComplete = pv;
}


//------------------------------------------------------------------------------
// bool CBcam::IsOpen()
// Author: 
// Date: 04.09.2002
//------------------------------------------------------------------------------
/**
 * \brief Retrieve open status
 *
 * \return    
 *
 * True if the device is already open
 * 
 */
//------------------------------------------------------------------------------
bool CBcam::IsOpen()
{
  return IsValid();
}

//------------------------------------------------------------------------------
// void CBcam::Open(CString DeviceName, HWND hWndNotify)
// Author: 
//------------------------------------------------------------------------------
/**
*
* \brief Opens a camera and registers a client window for device notifications
*
*  Both full device names (as delivered by the plug and play manager) as friendly device names 
*  (as returned by the CBcam::DeviceNames() method) can be used to open the camera.
*
* \param     DeviceName name of the device (either full or friendly device name)
* \param     hWndNotify handle to client window (optional)
* \throw BcamException The value of \c ::GetLastError() is thrown.
*/
//------------------------------------------------------------------------------
void CBcam::Open(CString DeviceName, HWND hWndNotify)
{
  TRACE( _T( "BCAMApi: CBcam::Open(%s)\n" ), (LPCTSTR) DeviceName);
  
  CDriverVersionCheck::Match();

  CString fullDeviceName = BcamUtility::FullDeviceName(DeviceName);
  CString friendlyDeviceName = BcamUtility::FriendlyDeviceName(DeviceName);

  if (IsOpen())
  {
    if ( friendlyDeviceName == m_DeviceName )
      throw BcamException( BCAM_E_CAMERA_ALREADY_OPEN, _T( "CBcam::Open" ) );
    else
      Close();
  }
  
  assert(m_hCamera == INVALID_HANDLE_VALUE);
  CLock lck( s_TheCriticalSection );

  // If no window has been specified to be registered for device notifications (i.e. there has been 
  // no call to RegisterClient up to now), register a message-only window. If neccessary, a message-only 
  // window will be created. The message loop for the message-only window is driven by a seperate thread.
  if ( s_hWndNotify == NULL )
  {
    if ( s_hWndMsgOnly == NULL)
    {
      // Up to now no window exists, so create a message window.
      // The creation must be done by our message loop thread.
      assert ( (HANDLE) s_MessageLoopThread  == NULL );
      CEvent evt;  // The thread will signal this event if the window has been created
      evt.Create();
      DWORD error = 0;
      if ( s_MessageLoopThread.Create(MessageLoop, &evt) == TRUE )
      {
        switch ( WaitForSingleObject(evt, 20000) ) 
        {
        case WAIT_OBJECT_0: 
          assert ( s_hWndMsgOnly != NULL );
          break;
        case WAIT_TIMEOUT:
          error = WAIT_TIMEOUT;
          break;
        default:
          error = GetLastError();
        }
      }
      else
      {
        error = ::GetLastError();
      }
      if ( error != NULL )
      {
        throw BcamException( BCAM_E_REGISTER_DEVICE_NOTIFICATION_FAILED, _T( "CBcam::Open()" ), &error );
      }
    }
    RegisterClient(s_hWndMsgOnly);  
    s_hWndNotify = s_hWndMsgOnly;
  }
  m_hCamera = ::CreateFile(
    fullDeviceName,
    GENERIC_READ | GENERIC_WRITE,
    FILE_SHARE_READ | FILE_SHARE_WRITE,
    NULL, // security
    OPEN_EXISTING,
    FILE_FLAG_OVERLAPPED,
    NULL
    );
  if (INVALID_HANDLE_VALUE == m_hCamera)
    throw BcamException( ::GetLastError(), DeviceName );
  TRACE( _T( "BCAMApi: Handle = %x\n" ), m_hCamera);
  
  // add handle to BcamMap
  s_BcamMap.insert(BcamMap_t::value_type(m_hCamera, this));
  
  m_DeviceName = friendlyDeviceName;

  // Register the notification window for file handle related device notifications
  m_hDevHandleNotify = RegisterDeviceNotifcation(s_hWndNotify);
  
  // If the user wants to register an additional window register it for handle related device notifications
  if(hWndNotify)
    m_hDevHandleNotifyOpt = RegisterDeviceNotifcation(hWndNotify);

  // Create the completion port resp. associate an existing completion port with the new file handle
  m_hCompletionPort = ::CreateIoCompletionPort(
    m_hCamera,
    m_hCompletionPort,
    (ULONG)m_CompletionKey,
    0 );
  if (NULL == m_hCompletionPort)
  {
    throw BcamException( ::GetLastError(), DeviceName );
  }
  
}

static unsigned char H2B(TCHAR h)
{
  if ( h  < 58 )
    return  unsigned char(h - 48); 
  else if (  h < 71 ) 
    return unsigned char(h - 55); 
  else 
    return unsigned char(h - 87); 
}


//------------------------------------------------------------------------------
// bool CBcam::Authorize(const CString& Name, const CString& Id)
// Author: A.Happe
//------------------------------------------------------------------------------
/**
*
* \brief Authorize for restricted features.
*
* \param     Name Case sensitive user name
* \param     Id User ID
* \return true if the authorization succeeds, false otherwise
* \throw BcamException The value of \c ::GetLastError() is thrown.
*/
bool CBcam::Authorize(const CString& Name, const CString& Id)
{
  if (! IsValid())
    throw BcamException( BCAM_E_CAMERA_NOT_OPENED, _T( "CBcam::Authorize()" ) );
  if ( Id.GetLength() != sizeof MD5Sum * 2 )
    return false;
  
  USES_CONVERSION;
  const char* name = (char*) T2CA((LPCTSTR) Name);

  MD5Sum id;
  for ( int i = 0; i < sizeof MD5Sum; ++i )
  {
    id[i] = unsigned char( ( H2B(Id[i*2]) << 4 )  + H2B(Id[i*2+1]) );
  }

  BcamArg arg;
  BcamResult res;
  bool authorized = false;

  TryTransmit( _T( "CBcam::Authorize()" ),
    IOCTL_BCAM_GET_AUTH_CHALLENGE, 
   (void*) name, 
   strlen(name), 
   res.AuthChallenge, 
   sizeof res.AuthChallenge );

  MD5Context md5_ctxt;
  MD5Init(&md5_ctxt);
  MD5Update(&md5_ctxt, (unsigned char*) name, strlen(name));
  MD5Update(&md5_ctxt, id, sizeof(MD5Sum));
  MD5Update(&md5_ctxt, res.AuthChallenge, sizeof res.AuthChallenge);
  MD5Final(arg.Authorize, &md5_ctxt);

  if ( Transmit(
    IOCTL_BCAM_AUTHORIZE,
    (void*) arg.Authorize,
    sizeof arg.Authorize,
    NULL,
    0) )
  {
    authorized = true;
  }
  else
  {
    DWORD error = GetLastError() & 0xdfffffff;
    if ( error != STATUS_BCAM_PRIVILEGE_DENIED )
      throw BcamException( error, _T( "CBcam::Authorize()" ) );
  }

  return authorized;
}


//------------------------------------------------------------------------------
// void CBcam::CloseFileHandle()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Closes the device handle
*
* The file handle and related notification handles will be closed. The completion port
* remains opened.
* 
* \throw BcamException The errorcode of WaitForCompletion is returned.
* \todo      
*/
//------------------------------------------------------------------------------

void CBcam::CloseFileHandle()
{
  CLock lck( s_TheCriticalSection );
  DWORD error = 0;
  if ( INVALID_HANDLE_VALUE != m_hCamera )
  {
    if ( ! ::CloseHandle( m_hCamera ) )
      error = ::GetLastError();
    TRACE( _T( "BCAMApi: Handle %x for %s closed\n" ), m_hCamera, (LPCTSTR) m_DeviceName);
    s_BcamMap.erase(m_hCamera) ;
    m_hCamera = INVALID_HANDLE_VALUE;
    // Unregister the device notifcation handle(s)
    UnregisterDeviceNotification(m_hDevHandleNotify);
    m_hDevHandleNotify = INVALID_HANDLE_VALUE;
    if ( m_hDevHandleNotifyOpt != INVALID_HANDLE_VALUE )
    {
      UnregisterDeviceNotification(m_hDevHandleNotifyOpt);
      m_hDevHandleNotifyOpt = INVALID_HANDLE_VALUE;
    }
  }
  if ( error )
    throw BcamException( error, _T( "CBcam::CloseFileHandle(" ) + m_DeviceName + _T( ")" ) );
}


//------------------------------------------------------------------------------
// void CBcam::Close()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Close the Bcam device 
*
* The file handle, the notification handle(s) and the completion port handle 
* will be closed.
* 
* \throw BcamException The errorcode of WaitForCompletion is returned.
* \todo      
*/
//------------------------------------------------------------------------------
void CBcam::Close()
{
  CloseFileHandle();  // close file and notification handles

  // close completion port
  if ( m_hCompletionPort != NULL )
  {
    ::CloseHandle( m_hCompletionPort );
    m_hCompletionPort = NULL;
  }

}

//------------------------------------------------------------------------------
// void CBcam::Initialize()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Initializes the camera.
*
* \throw BcamException The value of \c ::GetLastError() is thrown.
*/
//------------------------------------------------------------------------------
void CBcam::Initialize()
{
  TryTransmit( _T( "CBcam::Initialize()" ), IOCTL_BCAM_INIT_CAMERA, NULL, 0 );
}


//------------------------------------------------------------------------------
// unsigned long CBcam::AllocateResources(unsigned long MaxBuffers, unsigned long MaxBufferSize, unsigned long BytePerPacket)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Allocate resource for isochronous transfer
*
* \param     MaxBuffers     maximum number of buffers
* \param     MaxBufferSize  maximum buffer size
* \param     BytePerPacket  number of byte per packet to allocate. If set to 0, 
             the value is read from the camera ( format 7 ) or calculated according to the current mode
* \return    Unused number of byte per packet remaining after allocation
* \throw BcamException The value of \c ::GetLastError() is returned.
*/
//------------------------------------------------------------------------------
unsigned long CBcam::AllocateResources(unsigned long MaxBuffers, unsigned long MaxBufferSize, unsigned long BytePerPacket)
{
  
  // Allocate Bandwith
  ArgAllocateResources arg;
  ResAllocateResources res;
  DCSVideoFormat VideoFormat;
  DCSVideoMode VideoMode;
  DCSVideoFrameRate VideoFramerate;
  
  GetVideoMode(&VideoFormat, &VideoMode, &VideoFramerate);

  arg.nBytesPerPacket = BytePerPacket ? BytePerPacket : (VideoFormat != DCS_Format7) ?
    BcamUtility::BytePerPacket(VideoFormat, VideoMode, VideoFramerate) : FormatSeven[VideoMode].BytePerPacket();
  arg.nMaxBufferSize = MaxBufferSize;
  arg.nNumberOfBuffers = MaxBuffers;
  
  TRACE( _T( "BCAMApi: Allocate Resources: BytesPerPacket: %d,  NumberOfBuffers: %d, MaxBufferSize: %d\n" ), arg);
  TryTransmit( _T( "CBcam::AllocateResources()" ), IOCTL_BCAM_ALLOCATE_RESOURCES, &arg, sizeof arg, &res, sizeof res );
  m_IsoSpeed = res.Speed;
  m_IsoChannel = res.Channel;
  return res.nBytesPerPacketRemain;
}

//------------------------------------------------------------------------------
// void CBcam::FreeResources()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Frees the allocated resources
*
* \throw BcamException The value of \c ::GetLastError() is returned.
*/
//------------------------------------------------------------------------------
void CBcam::FreeResources()
{
  TRACE( _T( "BCAMApi: FreeResources()...." ) );
  m_IsoChannel = -1;
  Sleep(100);
  TryTransmit( _T( "CBcam::FreeResources()" ), IOCTL_BCAM_FREE_RESOURCES, NULL, 0 );
  TRACE( _T( "ok.\n" ) );
}



//------------------------------------------------------------------------------
// void CBcam::GrabImage( void *pBuffer, size_t BufferSize, Timespan Timeout, bool OneShot )
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Grabs an image synchronously.
*
* \param     *pBuffer    Pointer to the image buffer
* \param     BufferSize  Size of the image buffer in bytes
* \param     Timeout     Number of milliseconds to wait (optional)
* \param     OneShot     Issues a one shot command (optional)
* \throw BCamException 
\li BCAM_E_TIMEOUT_GRAB  Timeout occurred
\li BCAM_E_UNEXPECTED_BUFFER Received a different image buffer
*/
//------------------------------------------------------------------------------
void CBcam::GrabImage( void *pBuffer, size_t BufferSize, Timespan Timeout, bool OneShot )
{

  if (! IsValid())
    throw BcamException( BCAM_E_CAMERA_NOT_OPENED, _T( "CBcam::GrabImage()" ) );

  assert(! IsBadWritePtr(pBuffer, BufferSize) );

  HANDLE hEvent;
  hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  if ( hEvent == NULL )
    throw BcamException( ::GetLastError(), _T( "CBcam::GrabImage()" ) );
  
  OVERLAPPED Overlapped;
  ZeroMemory(&Overlapped, sizeof(Overlapped));
  Overlapped.hEvent = (HANDLE) ( ( (unsigned long) hEvent)  | 0x1 );  // setting low order bit will prevent completion port notification

  // issue receive data command
  DWORD BytesTransmitted;
  DWORD error = 0;

  if ( ! DeviceIoControl( m_hCamera, DWORD( IOCTL_BCAM_RECEIVE_DATA ), NULL, 0, pBuffer, BufferSize, &BytesTransmitted, &Overlapped) )
  {
    if ( GetLastError() != ERROR_IO_PENDING )
    {
      error = GetLastError();
    }
  }

  if ( ! error && OneShot )
  {
    // issue one shot command 
    ArgCameraControl arg = {CamCmd_SingleGrab, 0};
    if ( ! Transmit(IOCTL_BCAM_CAMERA_CONTROL, &arg, sizeof arg, NULL, 0, Timeout) )
    {
      error = GetLastError();
      if ( error == ERROR_SEM_TIMEOUT )
      {
        error = BCAM_E_TIMEOUT_GRAB;
      }
    }
  }

  // Wait for the buffer
  if ( ! error || error == ERROR_BAD_COMMAND )
  {
    // errorr == ERROR_BAD_COMMAND is true, when issuing the one shot
    // command triggers an invalid state transistion. In this case, the driver has
    // rejected the receive buffer command. Nevertheless, retrieve the result of this io 
    // operation and returns it to the user.
    switch ( WaitForSingleObject(Overlapped.hEvent, Timeout) )
    {
    case WAIT_OBJECT_0: 
      if ( ! GetOverlappedResult(m_hCamera, &Overlapped, &BytesTransmitted, FALSE) )
      {
        error = GetLastError();
      }
      break;
    case WAIT_TIMEOUT:
      // timeout -> cancel 
      CancelIo(m_hCamera);
      error = BCAM_E_TIMEOUT_GRAB;
      break;
    default:
      error = GetLastError();
    }
  }

  // cleanup
  CloseHandle(hEvent);
///////////////////////////////////// 0 && : Gael sinon error
  if ( 0 && error )
    throw BcamException( error, _T( "CBcam::GrabImage()" ) );
}

//------------------------------------------------------------------------------
// void CBcam::GrabImages( void *pBuffer[], size_t NumBuffers, size_t BufferSize, Timespan Timeout)
// Author: 
//------------------------------------------------------------------------------
/**
* <type function description here>
*
* \param     *pBuffer[]
* \param     NumBuffers
* \param     BufferSize
* \param     Timeout
* \return    
*
* <type Return description here>
* 
* \see       <delete line if not used>
* \todo      Implementation
*/
//------------------------------------------------------------------------------
void CBcam::GrabImages( void * /*pBuffer*/[], size_t /*NumBuffers*/, size_t /*BufferSize*/, Timespan /*Timeout*/)
{
  throw BcamException( Bvc::ErrorNumber( E_NOTIMPL ), _T( "GrabImages()" ) );
}

//------------------------------------------------------------------------------
// void CBcam::GrabImageAsync( void *pBuffer, size_t BufferSize, void *pContext, bool OneShot)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Grabs an image asynchronously.
*
* \param     *pBuffer    Pointer to the image buffer
* \param     BufferSize  Size of image buffer in bytes
* \param     *pContext   Pointer to context information that will be returned afterwards (optional)
* \param     OneShot     Issues a one shot command (optional)
* \throw     BcamException The value of \c ::GetLastError() is returned.
* \todo OneShot
*/
//------------------------------------------------------------------------------
void CBcam::GrabImageAsync( void *pBuffer, size_t BufferSize, void *pContext,  bool OneShot)
{
  BcamOL *pOL = GetNewBcamOL( AsyncGrabImage, pContext );
  if ( ! Transmit(IOCTL_BCAM_RECEIVE_DATA, NULL, NULL, pBuffer, BufferSize, pOL) )
  {
    DWORD error = ::GetLastError();
    if (error != ERROR_IO_PENDING)
      throw BcamException( ::GetLastError(), _T( "CBcam::GrabImageAsync()" ) );
  }
  if ( OneShot )
  {
    this->OneShot.SetAsync();
  }
}


//------------------------------------------------------------------------------
// void CBcam::WaitForCompletion(FunctionCode_t *pFunktionCode,  unsigned long *pErrorCode,  void* *ppContext,  Timespan Timeout)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Wait for completed requestes
*
* \param     *pFunctionCode  Takes the received function code
* \param     *pErrorCode     Takes the received error code
* \param     *ppContext      Takes the receive context pointer
* \param     Timeout         Number of milliseconds to wait
*/
//------------------------------------------------------------------------------
void CBcam::WaitForCompletion(FunctionCode_t *pFunctionCode, 
                              unsigned long *pErrorCode, 
                              void* *ppContext, 
                              Timespan Timeout)
{
  if ( NULL == m_hCompletionPort )
    CreateCompletionPort();  // lazy creation of the completion port

  unsigned long transferredBytes = 0;
  BcamOL *pOL = NULL;
  unsigned long key;
  
  if ( ppContext )
    *ppContext = NULL;
  if ( pErrorCode )
    *pErrorCode = 0;
  
  if (! ::GetQueuedCompletionStatus( m_hCompletionPort, &transferredBytes, &key, (LPOVERLAPPED*)&pOL, Timeout ))
  {
    DWORD error = ::GetLastError();
    if (pErrorCode)
      *pErrorCode = error & 0xdfffffff; // clear custom code bit!
  }
  else
  {
    assert( key == m_CompletionKey );
    assert( pOL != NULL );
  }
  if ( pOL != NULL )
  {
    if (ppContext)
      *ppContext = pOL->pContext;
    if (pFunctionCode)
      *pFunctionCode = pOL->function;
    delete pOL;
  }
}


//------------------------------------------------------------------------------
// void CBcam::Notify(FunctionCode_t FunctionCode, void *pContext)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Notifies the completion port
*
* \param     FunctionCode Code to send
* \param     *pContext    Pointer to Context information (optional)
* \throw BcamException  The value of \c ::GetLastError() is thrown
*/
//------------------------------------------------------------------------------
void CBcam::Notify(FunctionCode_t FunctionCode, void *pContext)
{
	if ( NULL == m_hCompletionPort )
		CreateCompletionPort();  // lazy creation of the completion port

	BcamOL* pOL = GetNewBcamOL(FunctionCode, pContext);
	if ( ! PostQueuedCompletionStatus(m_hCompletionPort, 0, m_CompletionKey, &pOL->ol) )
		throw BcamException( ::GetLastError(), _T( "CBcam::Notify()" ) );
}

//------------------------------------------------------------------------------
// void CBcam::Cancel()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Cancel all requestes 
*
* \param     *pContext    Pointer to Context information (optional)
* \throw BcamException  The value of \c ::GetLastError() is thrown
*/
//------------------------------------------------------------------------------
void CBcam::Cancel()
{
  if (! IsValid())
    throw BcamException( BCAM_E_CAMERA_NOT_OPENED, _T( "CBcam::Cancel()" ) );

  TryTransmit( _T( "CBcam::Cancel()" ), IOCTL_BCAM_FLUSH, NULL, 0 );
  TRACE( _T( "BCAMApi:  Bcam::Cancel()" ) );
}


//------------------------------------------------------------------------------
// void CBcam::SetVideoMode(DCSVideoFormat VideoFormat, DCSVideoMode VideoMode, DCSVideoFrameRate VideoFrameRate )
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Set the combination of video format, mode and frame rate.
*
* \param     VideoFormat     The video format to set
* \param     VideoMode       The video mode to set
* \param     VideoFrameRate  The frame rate to set. Ignored in case of video format 7.
* \throw BcamException The value of \c ::GetLastError() is thrown.
*/
//------------------------------------------------------------------------------
void CBcam::SetVideoMode(DCSVideoFormat VideoFormat, DCSVideoMode VideoMode, DCSVideoFrameRate VideoFrameRate )
{
  ArgSetVideoFormatModeFrameRate  ArgFM;
  ArgFM.FrameRate = VideoFrameRate;
  ArgFM.VideoFormat = VideoFormat;
  ArgFM.VideoMode = VideoMode;
  TryTransmit( _T( "CBcam::SetVideoMode()" ),
    IOCTL_BCAM_SET_VIDEO_FORMAT_MODE_FRAME_RATE, &ArgFM, sizeof(ArgFM) );
}

//------------------------------------------------------------------------------
// void CBcam::GetVideoMode( DCSVideoFormat *pVideoFormat, DCSVideoMode *pVideoMode, DCSVideoFrameRate *pFrameRate )
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Get the video format, mode and framerate.
*
* \param     *pVideoFormat Takes the video format
* \param     *pVideoMode   Takes the video mode
* \param     *pFrameRate   Takes the frame rate
* 
* \todo      Call the driver
*/
//------------------------------------------------------------------------------
void CBcam::GetVideoMode( DCSVideoFormat *pVideoFormat, DCSVideoMode *pVideoMode, DCSVideoFrameRate *pFrameRate )
{
  
  ResGetVideoFormatModeFrameRate res;
  ZeroMemory(&res, sizeof(res));
  TryTransmit( _T( "CBcam::GetVideoMode()" ),
    IOCTL_BCAM_GET_VIDEO_FORMAT_MODE_FRAME_RATE, NULL, 0, &res, sizeof(res) );
  if (pVideoFormat) *pVideoFormat = (DCSVideoFormat) res.VideoFormat;
  if (pVideoMode) *pVideoMode = (DCSVideoMode) res.VideoMode;
  if (pFrameRate) *pFrameRate = (DCSVideoFrameRate) res.FrameRate;
}

//------------------------------------------------------------------------------
// bool CBcam::IsVideoModeSupported( DCSVideoFormat VideoFormat, DCSVideoMode VideoMode, DCSVideoFrameRate VideoFrameRate )
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Inquire whether the combination of video format, video mode and frame rate is supported
*
* \param     VideoFormat     The video format
* \param     VideoMode       The video mode
* \param     VideoFrameRate  The frame rate
* \retval true   if it is supported
* \retval false  if it is not supported
* \todo call the driver
*/
//------------------------------------------------------------------------------
bool CBcam::IsVideoModeSupported( DCSVideoFormat VideoFormat, DCSVideoMode VideoMode, DCSVideoFrameRate VideoFrameRate )
{
  
  BOOL supported = TRUE;
  
  ResGetVideoFormatMask res;
  TryTransmit( _T( "CBcam::IsVideoModeSupported()"), IOCTL_BCAM_GET_VIDEO_FORMAT_MASK, NULL, 0, &res, sizeof(res) );
  
  ULONG mask = 0x80000000 >> VideoFormat;
  supported = res.VideoFormatMask & mask;
  
  if ( supported && VideoMode != DCS_IgnoreVideoMode )
  {
    ResGetVideoModeMask res;
    ArgGetVideoModeMask arg;
    arg.VideoFormat = VideoFormat;
    TryTransmit( _T( "CBcam::IsVideoModeSupported()" ),
      IOCTL_BCAM_GET_VIDEO_MODE_MASK, &arg, sizeof(arg), &res, sizeof(res) );
    mask = 0x80000000 >> VideoMode;
    supported = res.VideoMode & mask;
  }
  
  if ( supported && VideoFrameRate != DCS_IgnoreFrameRate )
  {
    ResGetFrameRateMask res;
    ArgGetFrameRateMask arg;
    arg.VideoFormat = VideoFormat;
    arg.VideoMode = VideoMode;
    TryTransmit( _T( "CBcam::IsVideoModeSupported()" ),
      IOCTL_BCAM_GET_FRAME_RATE_MASK, &arg, sizeof(arg), &res, sizeof(res) );
    mask = 0x80000000 >> VideoFrameRate;
    supported = res.FrameRateMask & mask;
  }
  
#pragma warning ( push )
#pragma warning (disable:4800) // forcing value to bool 'true' or 'false' (performance warning)
  return supported;
#pragma warning ( pop )
}

//------------------------------------------------------------------------------
// void CBcam::SaveSettingsToFile(const CString& FileName)
// Author: A.Happe
// Date: 04.09.2002
//------------------------------------------------------------------------------
/**
 * \brief Save the device's current settings to a file
 *
 * For cameras supporting shading correction, the shading correction related
 * settings will *not* be saved.
 *
 * \throw BcamException If an error occures when saving the configuration
 *
 * \param     FileName The name of the configuration file
 */
//------------------------------------------------------------------------------
void CBcam::SaveSettingsToFile(const CString& FileName)
{
  SaveSettings( CIniFilePropertyBag::Create( FileName, _T( "BCAM" ) ) );
}

//------------------------------------------------------------------------------
// void CBcam::SaveSettingsToRegistry(const CString& KeyName)
// Author: 
// Date: 06.09.2002
//------------------------------------------------------------------------------
/**
 * \brief Save the device's current settings to the registry
 *
 * The settings will be saved under a specified registry key. 
 * The registry key will be created below HKEY_CURRENT_USER.
 *
 * For cameras supporting shading correction, the shading correction related
 * settings will *not* be saved.
 *
 * Example: 
 * \code
 * pBcam->SaveSettingsToRegistry("Software\\MyCompany\\MyApp\\" + pBcam->Info.NodeId());
 * \endcode
 *
 * \param     KeyName Name of the registry key
 * \throw BcamException If an error occures when saving the configuration
 */
//------------------------------------------------------------------------------
void CBcam::SaveSettingsToRegistry(const CString& KeyName)
{
  SaveSettings( CRegistryPropertyBag::Create( KeyName ) );
}


//------------------------------------------------------------------------------
// void CBcam::SaveSettings(const CPropertyBagPtr ptrBag)
// Author: A.Happe
// Date: 06.09.2002
//------------------------------------------------------------------------------
/**
 * \brief Save the device's current settings to a property bag
 *
 * For cameras supporting shading correction, the shading correction related
 * settings will *not* be saved.
 *
 * This function is used by \code SaveSettingsToRegistry \endcode and 
 * \code SaveSettingsToFile \endcode. But it also can be used for customized 
 * property bag implementations.
 *
 * \param     ptrBag smart pointer to the property bag
 * \throw BcamException If an error occures when saving the configuration
 *
 */
//------------------------------------------------------------------------------
void CBcam::SaveSettings(const CPropertyBagPtr ptrBag)
{
  // save model name
  ptrBag->WriteString( _T( "Model" ), Info.ModelName());

  // save video mode, video format etc.
  DCSVideoFormat format;
  DCSVideoMode mode;
  DCSVideoFrameRate rate;
  GetVideoMode(&format, &mode, &rate);
  ptrBag->WriteLong( _T( "VideoFormat" ), format);
  ptrBag->WriteLong( _T( "VideoMode" ), mode);
  if ( format != DCS_Format7 )
  {
    ptrBag->WriteLong( _T( "VideoFrameRate" ), rate);
  }
  else
  {
    FormatSeven[mode].Save( ptrBag->CreateBag( _T( "FormatSeven" ) ) );
  }

  // save trigger settings
  if ( Trigger.IsSupported() )
    Trigger.Save( ptrBag->CreateBag( _T( "Trigger" ) ) );

  // save testimage settings
  if ( TestImage.IsSupported() )
    TestImage.Save( ptrBag->CreateBag( _T( "TestImage" ) ) );

  // save scalar properties
  if ( Brightness.IsSupported() )
    Brightness.Save( ptrBag->CreateBag( _T( "Brightness" ) ) );
  if ( Shutter.IsSupported() )
    Shutter.Save( ptrBag->CreateBag( _T( "Shutter" ) ) );
  if ( Gain.IsSupported() )
    Gain.Save( ptrBag->CreateBag( _T( "Gain" ) ) );
  if ( Gamma.IsSupported() )
    Gamma.Save( ptrBag->CreateBag( _T( "Gamma" ) ) );
  if ( Hue.IsSupported() )
    Hue.Save( ptrBag->CreateBag( _T( "Hue" ) ) );
  if ( Saturation.IsSupported() )
    Saturation.Save( ptrBag->CreateBag( _T( "Saturation" ) ) );
  if ( WhiteBalance.IsSupported() )
    WhiteBalance.Save( ptrBag->CreateBag( _T( "WhiteBalance" ) ) );

}


//------------------------------------------------------------------------------
// void CBcam::RestoreSettingsFromFile(const CString& FileName)
// Author: A.Happe
// Date: 04.09.2002
//------------------------------------------------------------------------------
/**
 * \brief Restore the device's settings from a file.
 *
 * For cameras supporting shading correction, the shading correction related
 * settings will *not* be restored.
 *
 * \throw BcamException If an error occures when restoring the configuration
 *
 * \param     FileName The name of the configuration file
 */
//------------------------------------------------------------------------------
void CBcam::RestoreSettingsFromFile(const CString& FileName)
{
  RestoreSettings( CIniFilePropertyBag::Open( FileName, _T( "BCAM" ) ) );
}

//------------------------------------------------------------------------------
// void CBcam::RestoreSettingsFromRegistry(const CString& KeyName)
// Author: 
// Date: 06.09.2002
//------------------------------------------------------------------------------
/**
 * \brief Restore the device's settings from the registry.
 *
 *
 * For cameras supporting shading correction, the shading correction related
 * settings will *not* be restored.
 *
 * \param     KeyName Name of the registry key
 *
 * \throw BcamException If an error occures when restoring the configuration
 * 
 */
//------------------------------------------------------------------------------
void CBcam::RestoreSettingsFromRegistry(const CString& KeyName)
{
  RestoreSettings(CRegistryPropertyBag::Open(KeyName));
}

//------------------------------------------------------------------------------
// void CBcam::RestoreSettings(const CPropertyBagPtr ptrBag)
// Author: 
// Date: 06.09.2002
//------------------------------------------------------------------------------
/**
 * \brief Restore the devive's settings from a property bag
 *
 * This function is used by \code RestoreSettingsFromRegistry \endcode and 
 * \code RestoreSettingsFromFile \endcode. But it also can be used for customized 
 * property bag implementations.
 *
 * \param     ptrBag smart pointer to the property bag
 * \throw BcamException If an error occures when restoring the configuration
 */
//------------------------------------------------------------------------------
void CBcam::RestoreSettings(const CPropertyBagPtr ptrBag)
{
  // check if the stored configuration matches the current device
  if ( ptrBag->ReadString( _T( "Model" ) ) != Info.ModelName() )
    throw BcamException( BCAM_E_CONFIGURATION_MISMATCH, _T( "CBcam::RestoreConfiguration()" ) );

  // restore the video mode
  DCSVideoFormat format = (DCSVideoFormat) ptrBag->ReadLong( _T( "VideoFormat" ) );
  DCSVideoMode mode = (DCSVideoMode) ptrBag->ReadLong( _T( "VideoMode" ) );
  DCSVideoFrameRate rate;
  if ( format != DCS_Format7 )
  {
    rate = (DCSVideoFrameRate) ptrBag->ReadLong( _T( "VideoFrameRate" ) );
  }
  else
  {
    rate = DCS_IgnoreFrameRate;
  }
  SetVideoMode(format, mode, rate);
  if ( format == DCS_Format7 )
  {
    FormatSeven[mode].Restore( ptrBag->GetBag( _T( "FormatSeven" ) ) );
  }

  // restore trigger settings
  if ( Trigger.IsSupported() )
    Trigger.Restore( ptrBag->GetBag( _T( "Trigger" ) ) );

  // restore test image 
  if ( TestImage.IsSupported() )
    TestImage.Restore( ptrBag->GetBag( _T( "TestImage" ) ) );

  // restore scalar properties
  // save scalar properties
  if ( Brightness.IsSupported() )
    Brightness.Restore( ptrBag->GetBag( _T( "Brightness" ) ) );
  if ( Shutter.IsSupported() )
    Shutter.Restore( ptrBag->GetBag( _T( "Shutter" ) ) );
  if ( Gain.IsSupported() )
    Gain.Restore(ptrBag->GetBag( _T( "Gain" ) ) );
  if ( Gamma.IsSupported() )
    Gamma.Restore( ptrBag->GetBag( _T( "Gamma" ) ) );
  if ( Hue.IsSupported() )
    Hue.Restore( ptrBag->GetBag( _T( "Hue" ) ) );
  if ( Saturation.IsSupported() )
    Saturation.Restore( ptrBag->GetBag( _T( "Saturation" ) ) );
  if ( WhiteBalance.IsSupported() )
    WhiteBalance.Restore( ptrBag->GetBag( _T( "WhiteBalance" ) ) );
}

//------------------------------------------------------------------------------
// void CBcam::CreateCompletionPort()
// Author: 
//------------------------------------------------------------------------------
/**
 * \brief Creates the completion port
 * A completion port is created which is not associated with any file handle. 
 * Opening the Bcam device will attach the completion port to the device's file handle.
 *
 * \return void
 * \throw BcamException The value of ::GetLastError() is thrown 
 *    when failed to create the completion port
 * 
 */
//------------------------------------------------------------------------------
void CBcam::CreateCompletionPort()
{
  assert( NULL == m_hCompletionPort );
  if ( NULL == m_hCompletionPort )
  {
    m_hCompletionPort = ::CreateIoCompletionPort(
      INVALID_HANDLE_VALUE,
      NULL, 
      0,
      0 );
    if ( NULL == m_hCompletionPort )
    {
      // failed to create completion port
      m_hCompletionPort = INVALID_HANDLE_VALUE;  
      throw BcamException( ::GetLastError(), _T("CBcam::CreateCompletionPort()"));
    }
  }
}

//------------------------------------------------------------------------------
// void CBcam::TryTransmit(const char * context, DWORD IoCtl, void *pArg, size_t argSize , void *pRes, size_t resSize, Timespan timeout )
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Tries to transmit I/O control commands to the driver, throws on failure
*
* pseudo synchronous transmit
*
*
* \param     context  context of the exception thrown on failure
* \param     IoCtl    The device I/0 control code
* \param     *pArg    Pointer to the input parameters
* \param     argSize  Size of the input
* \param     *pRes    Pointer to the output buffer(optional)
* \param     resSize  Size of the output buffer(optional)
* \param     timeout  Number of milliseconds to wait(optional) 
* \retval    void
* \exception  BcamException with BCAM_E_CAMERA_NOT_OPENED is thrown if object is void
*             BcamException with ::GetlastError and context is thrown, if Transmit fails
*
*/
//------------------------------------------------------------------------------
void CBcam::TryTransmit( LPCTSTR context, DWORD IoCtl, void *pArg, size_t argSize , void *pRes, size_t resSize, Timespan timeout )
{
  if (! Transmit( IoCtl, pArg, argSize , pRes, resSize, timeout ))
    throw BcamException( ::GetLastError(), context );
}




//------------------------------------------------------------------------------
// bool CBcam::Transmit( DWORD IoCtl, void *pArg, size_t argSize , void *pRes, size_t resSize, Timespan timeout )
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Transmits I/O control commands to the driver
*
* pseudo synchronous transmit
*
*
* \param     IoCtl    The device I/0 control code
* \param     *pArg    Pointer to the input parameters
* \param     argSize  Size of the input
* \param     *pRes    Pointer to the output buffer(optional)
* \param     resSize  Size of the output buffer(optional)
* \param     timeout  Number of milliseconds to wait(optional) 
* \retval    true  if succeeded
* \retval    false if failed, use GetLastError() to retreive further information
* \exception  BcamException with BCAM_E_CAMERA_NOT_OPENED is thrown if object is void
*
*/
//------------------------------------------------------------------------------
bool CBcam::Transmit( DWORD IoCtl, void *pArg, size_t argSize , void *pRes, size_t resSize, Timespan timeout )
{
  DWORD error = 0;
  
  if (! IsValid())
    throw BcamException( BCAM_E_CAMERA_NOT_OPENED, _T( "CBcam::Transmit()" ) );
  
  HANDLE hEvent;
  hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  if ( hEvent == NULL )
    return false;
  
  OVERLAPPED Overlapped;
  ZeroMemory(&Overlapped, sizeof(Overlapped));
  Overlapped.hEvent = (HANDLE) ( ( (unsigned long) hEvent)  | 0x1 );  // setting low order bit will prevent completion port notification

  DWORD BytesTransmitted;
  if ( ! ::DeviceIoControl( m_hCamera, IoCtl, pArg, argSize, pRes, resSize, &BytesTransmitted, &Overlapped) )
  {
    if ( GetLastError() != ERROR_IO_PENDING )
    {
      error = GetLastError();
    }
  }
  if ( ! error )
  {
    switch ( WaitForSingleObject(Overlapped.hEvent, timeout) )
    {
    case WAIT_OBJECT_0: 
      if ( ! GetOverlappedResult(m_hCamera, &Overlapped, &BytesTransmitted, FALSE) )
      {
        error = GetLastError();
      }
      break;
    case WAIT_TIMEOUT:
      // timeout -> cancel 
      CancelIo(m_hCamera);
      error = ERROR_SEM_TIMEOUT;
      break;
    default:
      error = GetLastError();
    }
  }
  CloseHandle(hEvent);
  SetLastError(error);
  return error == 0;
}



//------------------------------------------------------------------------------
// bool CBcam::Transmit( DWORD IoCtl, void *pArg, size_t argSize , BcamOL* pOL )
// Author: 
//------------------------------------------------------------------------------
/**
* \overload
* \brief Transmits I/O control commands to the driver
*
* \param     IoCtl    The device I/O control code
* \param     *pArg    Pointer to the input parameter
* \param     argSize  Size of the input parameter
* \param     pOL      Pointer to an overlapped structure
* \exception  BcamException with BCAM_E_CAMERA_NOT_OPENED is thrown if object is void
*/
//------------------------------------------------------------------------------
bool CBcam::Transmit( DWORD IoCtl, void *pArg, size_t argSize , BcamOL* pOL )
{
  if (! IsValid())
    throw BcamException( BCAM_E_CAMERA_NOT_OPENED, _T( "CBcam::Transmit()" ) );
  
  DWORD BytesTransmitted;
  return  ::DeviceIoControl( m_hCamera, IoCtl, pArg, argSize, NULL, 0, &BytesTransmitted, &pOL->ol ) != FALSE;
}

//------------------------------------------------------------------------------
// \overload bool CBcam::Transmit( DWORD IoCtl, void *pArg, size_t argSize, void* pRes, size_t resSize, BcamOL* pOL )
// Author: 
//------------------------------------------------------------------------------
/**
* \overload
* \brief Transmits I/O control commands to the driver
*
* \param     IoCtl    The device I/O control code
* \param     *pArg    Pointer to the input parameter
* \param     argSize  Size of the input parameter
* \param     pRes     Pointer to the result buffer
* \param     resSize  Size of the result buffer
* \param     pOL      Pointer to an overlapped structure
* \exception  BcamException with BCAM_E_CAMERA_NOT_OPENED is thrown if object is void
*/
//------------------------------------------------------------------------------
bool CBcam::Transmit( DWORD IoCtl, void *pArg, size_t argSize, void* pRes, size_t resSize, BcamOL* pOL )
{
  if (! IsValid())
    throw BcamException( BCAM_E_CAMERA_NOT_OPENED, _T( "CBcam::Transmit()" ) );
  
  DWORD BytesTransmitted;
  return ::DeviceIoControl( m_hCamera, IoCtl, pArg, argSize, pRes, resSize, &BytesTransmitted, &pOL->ol ) != FALSE;
}


//------------------------------------------------------------------------------
// bool CBcam::IsValid()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief check whether the device is still valid.
*
* \retval  true  if it is ok
* \retval  false if it is not ok
*/
//------------------------------------------------------------------------------
bool CBcam::IsValid()
{
  return m_hCamera != NULL && m_hCamera != INVALID_HANDLE_VALUE;
}

//------------------------------------------------------------------------------
// HDEVNOTIFY CBcam::RegisterDeviceNotifcation( HWND hWndNotify )
// Author: 
//------------------------------------------------------------------------------
/**
 * \brief Registers a client window for handle related device notifications.
 *
 * \param     hWndNotify Handle to the client window
 * \return 
 *
 * Returns a device notification handle
 *
 * \throw BcamException BCAM_E_REGISTER_DEVICE_NOTIFICATION_FAILED
 */
//------------------------------------------------------------------------------
HDEVNOTIFY CBcam::RegisterDeviceNotifcation( HWND hWndNotify )
{
  HDEVNOTIFY hNotify = INVALID_HANDLE_VALUE;
  if ( NULL != hWndNotify )
  {
    if ( ! IsWindow(hWndNotify) )
    {
      DWORD Err = ERROR_INVALID_WINDOW_HANDLE;
      throw BcamException( BCAM_E_REGISTER_DEVICE_NOTIFICATION_FAILED, _T( "CBcam::RegisterDeviceNotifications()"), &Err );
    }
    DEV_BROADCAST_HANDLE NotificationFilter;
    ZeroMemory( &NotificationFilter, sizeof(NotificationFilter) );
    NotificationFilter.dbch_size = 
      sizeof(DEV_BROADCAST_HANDLE);
    NotificationFilter.dbch_devicetype = DBT_DEVTYP_HANDLE;   // we are interested in handle related notifications
    NotificationFilter.dbch_handle = m_hCamera;
    hNotify = ::RegisterDeviceNotification( hWndNotify,
      &NotificationFilter,
      DEVICE_NOTIFY_WINDOW_HANDLE
      );
    
    if( ! hNotify  ) 
    {
      DWORD Err = GetLastError();
      throw BcamException( BCAM_E_REGISTER_DEVICE_NOTIFICATION_FAILED, _T( "CBcam::RegisterDeviceNotifcation()" ), &Err);
    }
    
  }

  return hNotify;
}

//------------------------------------------------------------------------------
// void CBcam::UnregisterDeviceNotification(HDEVNOTIFY hNotify)
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Unregister client window from handle device notificatios
* \param hNotify Notification handle to unregister
*
* \throw BcamException The value of \c ::GetLastError() is thrown
*/
//------------------------------------------------------------------------------
void CBcam::UnregisterDeviceNotification(HDEVNOTIFY hNotify)
{
  if ( hNotify != INVALID_HANDLE_VALUE )
  {
    if ( ! ::UnregisterDeviceNotification( hNotify ) )
    {
      throw BcamException( GetLastError(), _T( "CBcam::UnregisterDeviceNotification()" ) );
    }
  }
}


//------------------------------------------------------------------------------
// void CBcam::InternalUnregisterClient()
// Author: 
// Date: 26.09.2002
//------------------------------------------------------------------------------
/**
 * \brief Unregister the client window from device interface related notifications
 *
 */
//------------------------------------------------------------------------------
void CBcam::InternalUnregisterClient()
{
  if ( s_hDevInterfaceNotify != INVALID_HANDLE_VALUE )
  {
    ::UnregisterDeviceNotification(s_hDevInterfaceNotify);
    s_hDevInterfaceNotify = INVALID_HANDLE_VALUE;
    // Redo window subclassing
    if ( s_hWndNotify != NULL && IsWindow(s_hWndNotify) )
    {
      SetWindowLong(s_hWndNotify, GWL_WNDPROC, (LONG) s_lpPrevWndFunc);
      s_lpPrevWndFunc = NULL;
      s_hWndNotify = NULL;
    }
  }

}

//------------------------------------------------------------------------------
// DWORD WINAPI CBcam::MessageLoop(LPVOID lpThreadParameter)
// Author: 
// Date: 19.09.2002
//------------------------------------------------------------------------------
/**
 * \Brief Window Thread procedure of the thread running the message-only window's message loop.
 *
 * \param     lpThreadParameter A pointer to a CEvent, which is set by the thread procedure
 *            when the message-only window has been created
 * \return    
 *
 * Exit code of the thread function
 * 
 */
//------------------------------------------------------------------------------

DWORD WINAPI CBcam::MessageLoop(LPVOID lpThreadParameter)
{

  // create the message-only window
  CEvent* pEvent = reinterpret_cast<CEvent*>(lpThreadParameter);

  HINSTANCE hInstance = NULL;
  s_hWndMsgOnly = CreateWindowEx(0, _T( "Message" ), _T( "BcamMessageOnly" ), 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, hInstance, NULL);
  if ( s_hWndMsgOnly == NULL )
  {
    return 1;
  }

  // set our own window procedure for the hidden window
  SetLastError(0);
  SetWindowLong(s_hWndMsgOnly, GWL_WNDPROC, (LONG) HiddenWindowProc);
  if ( GetLastError() != 0 )
    return 1; 

  pEvent->Set();  // signal that we have created the window

  // run the message loop

  MSG msg;
  BOOL bRet;

  while( (bRet = GetMessage( &msg, NULL, 0, 0 )) != 0)
  { 
    if (bRet != -1)
    {
        DispatchMessage(&msg); 
    }
    else
    {
      // error
      return 1;
    }
  }

  return 0;

}

//------------------------------------------------------------------------------
// LRESULT CALLBACK CBcam::HiddenWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
// Author: 
// Date: 25.09.2002
//------------------------------------------------------------------------------
/**
 * \brief The window procedure of the hidden message-only window
 *
 * \param     hWnd window handle
 * \param     uMsg message type
 * \param     wParam wParam
 * \param     lParam lParam
 * \return    0
 *
 */
//------------------------------------------------------------------------------
LRESULT CALLBACK CBcam::HiddenWindowProc(HWND hWnd, UINT uMsg, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
  switch ( uMsg )
  {
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  case WM_CLOSE:  
    DestroyWindow(hWnd);
    break;
  }

  return 0;
}
  
//------------------------------------------------------------------------------
// LRESULT CALLBACK CBcam::BcamWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
// Author: 
// Date: 19.09.2002
//------------------------------------------------------------------------------
/**
 * \brief The new window procedure for a window registered for device notifications.
 *
 * When a window gets registered for device notifications using CBcam::RegisterClient(), 
 * its window procedure is replaced by this one, i.e. the BcamApi sublcasses a window
 * registered for device notifications.
 *
 * If the new window procedure receives a WM_DEVICECHANGE message, it checks if the message
 * is related to a opened Bcam device. If the message signals that a opened Bcam device has 
 * been removed, the BcamWindowProc performes the following steps:
 * - call the OnRemoveRequest callback function, if any is registered
 * - close the device (this is necessary, otherwise the device object would not be removed)
 * - call the OnRemoveComplete callback function, if any is registered
 * - pass the message to the original window procedure.
 *
 * All other messages than WM_DEVICECHANGE are passed to the original window procedure.
 *
 * \param     hWnd handle to the window
 * \param     uMsg message type
 * \param     wParam wParam
 * \param     lParam lParam
 * \return    The return value of the original winow procedure
 * 
 */
//------------------------------------------------------------------------------

LRESULT CALLBACK CBcam::BcamWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  PDEV_BROADCAST_HDR  devHdr;
  devHdr = (PDEV_BROADCAST_HDR)lParam;
  PDEV_BROADCAST_HANDLE devHandle;
  devHandle = (PDEV_BROADCAST_HANDLE) lParam;

  if ( uMsg == WM_DEVICECHANGE )
  {
    // handle device notifications
    switch (wParam) 
    {
    case DBT_DEVICEQUERYREMOVE: 
      {
        if ( devHdr->dbch_devicetype != DBT_DEVTYP_HANDLE )
          break;  // we are only interested in handle related device notification
        TRACE( _T( "BCAMApi: CBCAM:  DBT_DEVICEQUERYREMOVE. Handle: %x \n" ), devHandle->dbch_handle);
        BcamMap_t::iterator p;
        p = s_BcamMap.find(devHandle->dbch_handle);
        assert(p != s_BcamMap.end() );
        if ( p != s_BcamMap.end() )
        {
          if ( p->second->m_hCamera != INVALID_HANDLE_VALUE && p->second->m_OnRemoveRequestCallback != NULL )
            p->second->m_OnRemoveRequestCallback(*p->second, p->second->m_pvOnRemoveRequest);
          p->second->CloseFileHandle();
        }
      }
      break;
    case DBT_DEVICEQUERYREMOVEFAILED: 
      {
        if ( devHdr->dbch_devicetype != DBT_DEVTYP_HANDLE )
          break;  // we are only interested in handle related device notification
        TRACE( _T( "BCAMApi: CBCAM:  DBT_DEVICEQUERYREMOVEFAILED\n" ) );
        BcamMap_t::iterator p;
        p = s_BcamMap.find((HANDLE) devHandle->dbch_handle);
        if ( p != s_BcamMap.end() )
        {
          p->second->Reopen();
        }
      }
      break;
    case DBT_DEVICEREMOVECOMPLETE: 
      {
        if ( devHdr->dbch_devicetype != DBT_DEVTYP_HANDLE )
          break;  // we are only interested in handle related device notification
        TRACE( _T( "BCAMApi: CBCAM:  DBT_DEVICEREMOVECOMPLETE. Handle: %x\n" ), devHandle->dbch_handle);
        BcamMap_t::iterator p;
        p = s_BcamMap.find((HANDLE) devHandle->dbch_handle);
        if ( p != s_BcamMap.end() )
        {
          CBcam* pBcam = p->second;
          // inform the client that a device is to be removed
          if ( pBcam->m_hCamera != INVALID_HANDLE_VALUE && pBcam->m_OnRemoveRequestCallback != NULL )
            pBcam->m_OnRemoveRequestCallback(*pBcam, pBcam->m_pvOnRemoveRequest);
          // close file handle 
          pBcam->CloseFileHandle();
          // Inform the client that a device has been removed
          // The client is allowed to delete the object in the OnRemoveCompleteCallback function,
          // so we don't access the this pointer any longer!!!!
          if ( pBcam->m_OnRemoveCompleteCallback != NULL )
            pBcam->m_OnRemoveCompleteCallback(*pBcam, pBcam->m_pvOnRemoveComplete);
        } 
        else
        {
          // we got signaled a remove complete event for a Bcam device already was closed, 
          // for example as reaction of a remove query.
          // Anyway unregister device notifications for the handle
          ::UnregisterDeviceNotification((HDEVNOTIFY) devHandle->dbch_hdevnotify);
        }
      }
      break;
    }
  }

  // pass the message to the original window procedure
  assert( s_lpPrevWndFunc != NULL);
  if ( s_lpPrevWndFunc != NULL )
    return CallWindowProc(s_lpPrevWndFunc, hWnd, uMsg, wParam, lParam);
  else
    return 0;

}



//------------------------------------------------------------------------------
// void CBcam::Reopen()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Close device and open it again
*/
//------------------------------------------------------------------------------
void CBcam::Reopen()
{
  assert(m_hCamera == INVALID_HANDLE_VALUE);
  // unregister device notification for old handle
  UnregisterDeviceNotification(m_hDevHandleNotify);
  Open(m_DeviceName, m_hWndHandleNotify);
}




///////////////////////////////////////////////////////////////////////////////
//
// CBcam::CShadingCorrection
//
//

//------------------------------------------------------------------------------
// CBcam::CShadingCorrection::IsSupported( Supported_t Inquiry )
// Author: 
//------------------------------------------------------------------------------
/**

 * \brief Inquire whether a feature is supported
 *
 * \param     Inquiry specifies the feature
 * \retval  true  if feature is supported
 * \retval  false if feature is not supported
 * \exception BcamException The value of \c ::GetLastError() is thrown
 */
//------------------------------------------------------------------------------
bool CBcam::CShadingCorrection::IsSupported( Supported_t Inquiry )
{
  ResGetShadingInq res;
  switch ( Inquiry )
  {
  case inqPresent :
  case inqSCM_1_3 :
  case inqSCM_1_5 : 
    if (! This()->Transmit( IOCTL_BCAM_GET_SHADING_INQUIRY, NULL, 0, &res, sizeof(res)) )
    {
      DWORD error = ::GetLastError();
      if 
        ( (error & 0xdfffffff) == STATUS_BCAM_UNLOCK_ERROR   ||  // camera doesn't implement advanced features
          (error & 0xdfffffff) == STATUS_BCAM_SHADING_FEATURE_NOT_SUPPORTED  // camera doesn't implement the shading correction feature 
        )
      {
        // camera doesn't implement the shading correction feature 
        return false;
      }
      // an other error has occured -> throw an exception
      throw BcamException( error, _T( "CBcam::CShadingCorrection::IsSupported()" ) );
    }
    return res.Present && ( Inquiry == inqPresent || ( (0x80000000 >> ( Inquiry - 1 ) ) & res.ModeFlags )) ; 
    break;
  default:
    throw BcamException( BCAM_E_INVALID_INQUIRY, _T( "CBcam::CShadingCorrection::IsSupported()" ), &Inquiry );
  }
}

//------------------------------------------------------------------------------
// void CBcam::CShadingCorrection::SetImage( void* pBuffer, size_t BufferSize, Timespan timeout,
// BcamProgressCallbackFunc pCallback , int BlockSize,  void* pContext )
// Author: 
//------------------------------------------------------------------------------
/**
 * Upload an image for shading correction
 *
 * \param     pBuffer     Pointer to the buffer
 * \param     BufferSize  Size of the buffer in bytes
 * \param     timeout     Number of milliseconds to wait(optional)
 * \param     pCallback   method to be called on Progress (optional) 
 * \param     BlockSize   size of one data block (optional) 
 * \param     pContext    second parameter of the callback method (optional) 
 * \exception BcamException The value of \c ::GetLastError() is thrown
 */
//------------------------------------------------------------------------------
void CBcam::CShadingCorrection::SetImage(  void* pBuffer, size_t BufferSize, Timespan timeout, 
                   BcamProgressCallbackFunc pCallback , int BlockSize,  void* pContext )
{
  BcamArg arg;
  DWORD error = 0;

  if ( BlockSize == 0 ) 
    BlockSize = BufferSize;

  // set write enable
  arg.SetShadingCtrl.ShadingCtrl.WriteEnable = TRUE;
  arg.SetShadingCtrl.Mask = BCAM_SHADING_CTRL_WRITEENABLE;
  This()->TryTransmit( _T( "CBcam::CShadingCorrection::SetImage()" ), IOCTL_BCAM_SET_SHADING_CTRL, &arg, sizeof arg.SetShadingCtrl );

  // upload data
  int written = 0;
  int c;
  bool cancelled = false;
  while ( written < (int) BufferSize && ! error )
  {
    c =  min( (int)BlockSize, (int)BufferSize - written);
    if ( ! This()->Transmit(IOCTL_BCAM_UPLOAD_SHADING_DATA, (unsigned char*) pBuffer + written,c,NULL,0,timeout) )
      error = GetLastError();
    else
    {
      written += c;
      if ( pCallback != 0 )
      {
        if ( ! (*pCallback)(written, pContext) )
        {
          cancelled = true;
          break;
        }
      }
    }
  }

  // We must at least send 256 bytes to the camera. 
  if ( ! error && written < 256 && ! cancelled)
  {
    int cDummy = 256 - written;
    unsigned char* pDummy = new unsigned char[cDummy];
    ZeroMemory(pDummy, cDummy);
    if ( ! This()->Transmit(IOCTL_BCAM_UPLOAD_SHADING_DATA, pDummy, cDummy,NULL,0,timeout) )
      error = GetLastError();
    else
    {
      written += cDummy;
      if ( pCallback != 0 )
        (*pCallback)(written, pContext);
    }
  }

  // delete write enable bit
  arg.SetShadingCtrl.ShadingCtrl.WriteEnable = FALSE;
  arg.SetShadingCtrl.Mask = BCAM_SHADING_CTRL_WRITEENABLE;
  This()->TryTransmit( _T( "CBcam::CShadingCorrection::SetImage()" ),
    IOCTL_BCAM_SET_SHADING_CTRL, &arg, sizeof arg.SetShadingCtrl );

  if ( error )
    throw BcamException( error, _T( "CBcam::CShadingCorrection::SetImage()" ) );

}

//------------------------------------------------------------------------------
// void CBcam::CShadingCorrection::operator=(SCM_Mode_t Value)
// Author: 
//------------------------------------------------------------------------------
/**
 * \brief Set the shading correction mode
 *
 * \param     Value new shading correction mode
 * \exception BcamException The value of \c ::GetLastError() is thrown
 */
//------------------------------------------------------------------------------
void CBcam::CShadingCorrection::operator=(SCMode_t Value)
{
  ArgSetShadingCtrl arg;
  arg.ShadingCtrl.ShadingMode = Value;
  arg.ShadingCtrl.Test = Value == SCM_Test;
  arg.Mask = BCAM_SHADING_CTRL_SHADINGMODE | BCAM_SHADING_CTRL_TEST;
  This()->TryTransmit( _T( "CBcam::CShadingCorrection::operator=()" ),
    IOCTL_BCAM_SET_SHADING_CTRL, &arg, sizeof arg );
}

//------------------------------------------------------------------------------
// SCM_Mode_t CBcam::CShadingCorrection::operator()()
// Author: 
//------------------------------------------------------------------------------
/**
 * \brief Retrieve the current shading correction mode 
 *
 * \return    Current shading correction mode
 * \exception BcamException The value of \c ::GetLastError() is thrown
 */
//------------------------------------------------------------------------------
SCMode_t CBcam::CShadingCorrection::operator()()
{
  ResGetShadingCtrl res;
  This()->TryTransmit( _T( "CBcam::CShadingCorrection::operator()()" ),
    IOCTL_BCAM_GET_SHADING_CTRL, NULL, 0, &res, sizeof res );
  if ( res.Test )
    return SCM_Test;
  return (SCMode_t) res.ShadingMode;
}


//------------------------------------------------------------------------------
// CBcam::CCleanUp::~CCleanUp()
// Author: 
// Date: 26.09.2002
//------------------------------------------------------------------------------
/**
 * \brief Do some cleanup
 *
 * \return    
 *
 * 
 */
//------------------------------------------------------------------------------
CBcam::CCleanUp::~CCleanUp()
{
  // If still a client window is registered for device interface related notifications unregister it
  InternalUnregisterClient();

  // close an existing message-only window
  CleanUp();
} 

//------------------------------------------------------------------------------
/**
* \brief Retrieve the file version of the installed driver.
* \return Returns the most significant 32 bits of the file's binary version number.
* \retval 0 if no driver is installed
* \exception BcamException Throws the value of \c ::GetLastError().
*/
//------------------------------------------------------------------------------
DWORD CBcam::InstalledDriverVersion()
{
  CString m_DriverPath;

  HKEY hKey;
  DWORD ret;
  CString path( _T( "" ), MAX_PATH+1), expanded_path( _T( "" ), MAX_PATH+1 );
  DWORD pathlen = MAX_PATH;
  const CString query = _T( "System\\CurrentControlSet\\Services\\bcam" );

  // look up path in service entry
  ret = ::RegOpenKeyEx( 
    HKEY_LOCAL_MACHINE,
    query,
    0, 
    KEY_QUERY_VALUE, 
    &hKey );
  if (ret != ERROR_SUCCESS)
    //throw BcamException( BCAM_E_DRIVER_NOT_INSTALLED );
    return false;

  ret = ::RegQueryValueEx( hKey, TEXT( "ImagePath" ), NULL, NULL, (LPBYTE)(LPCTSTR)path, &pathlen );
  if (ret != ERROR_SUCCESS)
  { 
    const DWORD e = ::GetLastError();
    ::RegCloseKey( hKey );
    throw BcamException( e );
  }
  ::RegCloseKey( hKey );


  // replace environment strings in path
  pathlen = sizeof path;
  ret = ::ExpandEnvironmentStrings( path, expanded_path.GetBuffer( MAX_PATH ), MAX_PATH );
  expanded_path.ReleaseBuffer();
  assert( ret > 0 );

  // if path is relative prepend the windows directory
  if (
    ((expanded_path[0] >= 'a' && expanded_path[0] <= 'z') ||
    (expanded_path[0] >= 'A' && expanded_path[0] <= 'Z') ) && 
    ! (expanded_path[1] == ':')
    )
  {
    ::GetWindowsDirectory( path.GetBuffer( MAX_PATH ), MAX_PATH );
    path.ReleaseBuffer();
    int l = path.GetLength() - 1;
    if (path[l] != _T( '\\' ))
      path += _T( "\\" );
    path += expanded_path;

    m_DriverPath = path;
  }
  else
    m_DriverPath = expanded_path;


  // get the version of the driver
  DWORD dwDummyHandle; 
  DWORD len = GetFileVersionInfoSize( (LPTSTR)(LPCTSTR)m_DriverPath, &dwDummyHandle );
  if (len == 0) throw BcamException( ::GetLastError() );
  PBYTE buf = (PBYTE) _alloca( len );
  assert( buf != NULL );
  if (! buf)
    throw BcamException( Bvc::ErrorNumber( E_OUTOFMEMORY ) );
  ::GetFileVersionInfo( (LPTSTR)(LPCTSTR)m_DriverPath, 0, len, &buf[0] );
  unsigned int ver_length;
  LPVOID lpvi;
  BOOL b = ::VerQueryValue( &buf[0], _T( "\\" ), &lpvi, &ver_length );
  if (! b) throw BcamException( Bvc::ErrorNumber( E_UNEXPECTED ) );
  VS_FIXEDFILEINFO fileInfo;
  fileInfo = *(VS_FIXEDFILEINFO*)lpvi;

  return fileInfo.dwFileVersionMS;
}
//------------------------------------------------------------------------------
// CDriverVersionCheck::Match()
// Author: 
// Date: 10.12.2002
//------------------------------------------------------------------------------
/**
 * \brief Check if the versions of the BCAM API and BCAM Driver match.
 *
 * \return    
 *
 * return true if the versions match, false if they don't
 * 
 */
//------------------------------------------------------------------------------


bool CDriverVersionCheck::Match()
{
#if OMIT_VERSION_CHECK
  return true;
#else
  if (! s_DriverMatched)
  {

    const DWORD version = CBcam::InstalledDriverVersion();
    s_DriverMatched = HIWORD( version ) == BCAM_VERSION_MAJOR && LOWORD( version ) == BCAM_VERSION_MINOR;
    if (! s_DriverMatched)
    {

      CString v,u;
      v.Format( _T("%d.%d"), HIWORD( version ), LOWORD( version ) ); 
      u.Format( _T("%d.%d"), BCAM_VERSION_MAJOR, BCAM_VERSION_MINOR );
      const _TCHAR *vaargs[] = { (LPCTSTR) u, (LPCTSTR) v };
//      throw BcamException( BCAM_E_DRIVER_VERSION, _T( "CDriverVersionCheck::Match" ), vaargs );
    }

  }

  return s_DriverMatched;
#endif // _DEBUG

}  
