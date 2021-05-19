/***************************************************************************
* Copyright: Testo SE & Co. KGaA, Testo-Straße 1, 79849 Lenzkirch
***************************************************************************/
/**@file
@brief<b>Description: </b> ir image interface class

***************************************************************************/

#ifndef IR_API_IMAGE_TYPES_H
#define IR_API_IMAGE_TYPES_H

/***************************************************************************
* Includes
***************************************************************************/

// standard include for testo project defines basic data types from std lib, decl
#include <ircam2020/irapi/IrapiConfig.h>

#include <vector>
#include <string>
#include <map>

#include <opencv2/core/core.hpp>

namespace irapi
{
  /**
  **************************************************************************
  class MeasPoint
  **************************************************************************/
  struct IRCAM2020_IRAPI_DECL MeasPoint
  {
    /**
    **************************************************************************
    Default Constructor
    any value is set to zero
    ***************************************************************************/
    MeasPoint();

    /**
    **************************************************************************
    Constructor

    This constructor only sets coordinates. That can be used together with
    irapi::Image::setMeaspoint function. The value is extracted from the image
    during the function call.

    @param [in] nXinit
    @param [in] nYinit
    ***************************************************************************/
    MeasPoint(int nXinit, int nYinit);

    int nX;
    int nY;
    // fValue in degree Celsius or %rH if humidity mode is active
    float fValue;
  };

  /**
  **************************************************************************
  class MeasArea
  **************************************************************************/
  struct IRCAM2020_IRAPI_DECL MeasArea
  {
    MeasArea();

    /**
    **************************************************************************
    Constructor

    This constructor sets coordinates, size and active spots.
    It can be used together with irapi::Image::setMeasArea function.
    The measurement values and spot coordinates are extracted from the image
    during the function call.

    @param [in] nXinit x position
    @param [in] nYinit y position
    @param [in] nWinit width
    @param [in] nHinit height
    @param [in] bHotColdActive activate hotspot and coldspot
    ***************************************************************************/
    MeasArea(int nXinit, int nYinit, int nWinit, int nHinit, bool bHotColdActive);

    int nX;
    int nY;
    int nWidth;
    int nHeight;

    float fMeanValue;

    MeasPoint coldspot;
    MeasPoint hotspot;

    bool bColdspotActive;
    bool bHotSpotActive;
  };

  /**
  **************************************************************************
  Defines temperature unit for get/set user temperature unit

  Note: the interface unit is always defree Celsius
  and does not depend on the user settings
  **************************************************************************/
  enum class TemperatureUnit
  {
    Celsius,
    Fahrenheit
  };

  /**
  **************************************************************************
  class IrFrame
  **************************************************************************/
  struct IrFrame
  {
    /* matIrData holds the temperature data of the live image in degree celsius */
    cv::Mat_<float> matIrData;

    /* matIrBgr holds the colored image in BRG mode */
    cv::Mat3b matIrBgr;

    /* maximal temperature or humidty (always 100%) used for scaling and palettizing the image */
    float fScaleMax;

    /* minimal temperature or humidty (always 0%) used for scaling and palettizing the image */
    float fScaleMin;

    /* color gradient with liniar scaled values between fScaleMin and fScaleMax
       This information can be used to display the relation between color and temperature
       in a scale element */
    cv::Mat3b matScaleGradient;
  };

  /**
  **************************************************************************
  class Key defines common key strings for measurement point and areas names
  usage e.g.: image.setMeasPointState(irapi::Key::MeasPoint::HotSpot, true);
  **************************************************************************/
  struct IRCAM2020_IRAPI_DECL Key
  {
    struct IRCAM2020_IRAPI_DECL MeasPoint
    {
      static const char* HotSpot;
      static const char* ColdSpot;
      static const char* CenterSpot;
      static const char* DeltaT1;
      static const char* DeltaT2;
      static const char* LaserSpot;
    };

    struct IRCAM2020_IRAPI_DECL MeasArea
    {
      static const char* Center;
    };
  };

  class IRCAM2020_IRAPI_DECL CameraNotConnectedException : public std::exception
  {
  public:
    CameraNotConnectedException(const std::string& strMessage) throw();
    virtual ~CameraNotConnectedException();

    virtual const char* what() const throw();

  private:
    std::string m_strMessage;
  };

  class IRCAM2020_IRAPI_DECL ParameterException : public std::exception
  {
  public:
    ParameterException(const std::string& strMessage) throw();
    virtual ~ParameterException();

    virtual const char* what() const throw();

  private:
    std::string m_strMessage;
  };
}


#endif