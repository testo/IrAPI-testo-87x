/***************************************************************************
* Copyright: Testo SE & Co. KGaA, Testo-Straße 1, 79849 Lenzkirch
***************************************************************************/
/**@file
@brief<b>Description: </b> ir image interface class

***************************************************************************/

#ifndef IR_API_IMAGE_H
#define IR_API_IMAGE_H

/***************************************************************************
* Includes
***************************************************************************/

// standard include for testo project defines basic data types from std lib, decl
#include <ircam2020/irapi/IrapiConfig.h>

#include <vector>
#include <string>
#include <map>

#include <opencv2/core/core.hpp>

#include "IrTypes.h"



/**
**************************************************************************
Forward declaration of private member
**************************************************************************/
namespace testo
{
  namespace exp
  {
    class IrImage;
  }

  namespace vision
  {
    class ColorPaletteConverter;
  }
}

namespace ridgid
{
  class IrImageRidgid;
}

namespace irapi
{
  /**
  **************************************************************************
  @brief imager interface

  Note: all float values with temperatures use degree Celsius
  Note: getIrImagePreview() from image object has been removed
        use getIrImageBgr() (the size of the image is the same)

  \ingroup interfaces
  **************************************************************************/
  class IRCAM2020_IRAPI_DECL Image
  {
  public:
    /**
    **************************************************************************
    Constructor

    @param [in] strFileName  full path and file name to the ir image
    on the local file system
    ***************************************************************************/
    Image(const std::string& strFileName);

    /**
    **************************************************************************
    Constructor

    @param [in] vecFileData  binar data of the ir image
    ***************************************************************************/
    Image(const std::vector<char>& vecFileData);

    /**
    **************************************************************************
    Destructor

    The destuctor will not save changes on the image! Use save or export function.
    ***************************************************************************/
    ~Image();

    /**
    **************************************************************************
    Disable copy constructor & assignment operator

    Image object can not be copied since open file access must be exclusive
    (only one file handle should have write access at a time)
    ***************************************************************************/
    Image(const Image& other) = delete;
    Image& operator= (const Image& rhs) = delete;

    /**
    **************************************************************************
    IMAGE DATA
    ***************************************************************************/

    /**
    **************************************************************************
    PREVIEW IMAGE DATA

    Fast static function that only read the preview information from the file.
    Can be used for a gallary with image previews
    ***************************************************************************/

    /**
    *************************************************************************
    Get visual image preview (fast version, only load jpeg from file)
    @param [in] strFilename filename
    @return bgr data from visual image for preview
    ************************************************************************/
    static cv::Mat3b getVisualImagePreview(const std::string& strFilename);

    /**
    *************************************************************************
    Get visual image preview (fast version, only load jpeg from file)
    @param [in] vecImageData binary image data (in case the image is already in RAM)
    @return bgr data from visual image for preview
    ************************************************************************/
    static cv::Mat3b getVisualImagePreview(const std::vector<char>& vecImageData);

    /**
    *************************************************************************
    Get visual image preview from open image object
    @return bgr data from visual image for preview
    ************************************************************************/
    cv::Mat3b getVisualImagePreview() const;

    /**
    *************************************************************************
    Get ir image preview (fast version, only load jpeg from file)
    @param [in] strFilename filename
    @return bgr data from ir image for preview
    ************************************************************************/
    static cv::Mat3b getIrImagePreview(const std::string& strFilename);

    /**
    *************************************************************************
    Get ir image preview (fast version, only load jpeg from file)
    @param [in] vecImageData binary image data (in case the image is already in RAM)
    @return bgr data from ir image for preview
    ************************************************************************/
    static cv::Mat3b getIrImagePreview(const std::vector<char>& vecImageData);

    /**
    *************************************************************************
    Get ir image preview from open image object
    @return BGR data from ir image for preview
    ************************************************************************/
    cv::Mat3b getIrImagePreview() const;

    /**
    *************************************************************************
    Get color ir image useing current palette
    @return ir image in BGR format
    ************************************************************************/
    cv::Mat3b getIrImageBgr() const;

    /**
    *************************************************************************
    get image data
    note:
    In standard mode this functions returns temperature values in degree Celsius!
    In humidity mode this function returns surface moisture values in %rH!
    @return ir image as cv::Mat_<float>
    ************************************************************************/
    cv::Mat_<float> getIrImageData() const;

    /**
    *************************************************************************
    Get full sized visual image (current resolution 3MP)
    @return visual image in BGR format
    ************************************************************************/
    cv::Mat3b getFullVisualImage() const;

    /**
    *************************************************************************
    Get small visual image. Can be use for a faster display on mobil device.
    // intern note use getVisualImageJpeg convert to BGR!
    @return visual image in BGR format
    ************************************************************************/
    cv::Mat3b getSmallVisualImage() const;



    /**
    **************************************************************************
    BASIC IR IMAGE PARAMETER
    **************************************************************************/

    /**
    *************************************************************************
    Get device serial number
    @return uint64_t serial number
    ************************************************************************/
    uint64_t getDeviceSerialNumber() const;

    /**
    *************************************************************************
    Get device name (e.g. "testo 872")
    @return device name
    ************************************************************************/
    std::string getDeviceName() const;

    /**
    *************************************************************************
    Get the user temperature unit that was set last time with this image
    Note: the interface unit is always degree Celsius
    and does not depend on the user settings

    @return TemperatureUnit enum Celsius, Fahrenheit
    ************************************************************************/
    TemperatureUnit getUserTemperatureUnit() const;

    /**
    *************************************************************************
    Set the user temperature unit
    Note: the interface unit is always degree Celsius
    and does not depend on the user settings

    @param [in] unit enum celsius, fahrenheit
    ************************************************************************/
    void setUserTemperatureUnit(TemperatureUnit unit);

    /**
    *************************************************************************
    Get date and time from camera
    @return timestamp (uint64_t eqates to time_t)
            the timestamp data eqates to time_t integral value
            representing the number of seconds elapsed since
            00:00 hours, Jan 1, 1970 UTC

            example convert the timestamp in a readable sting.
              time_t t = time(NULL);
              memcpy(&t, &u64TimeStamp, sizeof(t));
              struct tm *tm = localtime(&t);
              char date[50];
              strftime(date, sizeof(date), "%m/%d%Y %H:%M:%S", tm);
              std::cout << "date and time : " << date << std::endl;
    ************************************************************************/
    uint64_t getFileDateTime() const;

    /**
    *************************************************************************
    Set reflected temperature
    (throws if value is outside the sensor curve range -100  ... 1000 degree Celsius)
    Attention: This function recalculate temperatures

    @param [in] temperature reflected temperature in degree Celsius
    ************************************************************************/
    void setReflectedTemperature(float fTemperature);
    /**
    *************************************************************************
    Get reflected temperature
    @return reflected temperature in degree Celsius
    ************************************************************************/
    float getReflectedTemperature() const;

    /**
    *************************************************************************
    Get list of all defined materials for emissivity settings
    @return list (uses std::vector)
    ************************************************************************/
    static std::vector<std::string> listEmissivityMaterials();

    /**
    *************************************************************************
    Set user defiend emissivity
    Attention: This function recalculate temperatures
    (this set functions actiavtes the "user defined mode" the previous selected
    material is deleted)
    @param [in] fEmissivity emissivity of whole image [range 0...1]
    ************************************************************************/
    void setEmissivity(const float& fEmissivity);

    /**
    *************************************************************************
    Get emissivity
    @return emissivity
    ************************************************************************/
    float getEmissivity() const;


    /**
    *************************************************************************
    Set emissivity material
    (setting the material will result in a predefined emissivity value)
    @param [in] strMaterial material name
    ************************************************************************/
    void setEmissivityMaterial(const std::string& strMaterial);

    /**
    *************************************************************************
    Get emissivity material
    @return emissivity material name or "user defined"
    ************************************************************************/
    std::string getEmissivityMaterial() const;



    /**
    **************************************************************************
    COLOR PALETTE
    **************************************************************************/
    static std::vector<std::string> getAvailableColorPalettes();

    /**
    *************************************************************************
    Get active palette name
    @return palette name
    ************************************************************************/
    std::string getPalette() const;

    /**
    *************************************************************************
    Set active color palette

    Throws if palette is not available!

    @param [in] strPalette color palette name
    ************************************************************************/
    void setPalette(const std::string& strPalette);

    /**
    *************************************************************************
    Get a color gradient of active palette in a 1D cv::Mat3b (RGB format).
    The gradient represents the values between scale bottom and scale top (liniar).
    This function can be used to create a scale element displaying the
    temperature / surface moisture and the corresponding color

    @param [in] nGradientLength length of gradient
    (the current palette is interpoleted to target length)



    @return color gradient in RGB format
    ************************************************************************/
    cv::Mat3b getPaletteColors(int nGradientLength);


    /**
    **************************************************************************
    SCALING
    **************************************************************************/


    /**
    *************************************************************************
    Get scaling mode
    posible return values: "Auto", "Manual", "ScaleAssist", "Humidity"
    @return scaling mode
    ************************************************************************/
    std::string getScalingMode() const;

    /***********************************************************************
    get scaling top
    @return scale top in degree Celsius (normal mode) %rH (humidity mode fix 100%rH)
    ************************************************************************/
    float getScaleTop();

    /***********************************************************************
    get scaling bottom
    @return scale bottom in degree Celsius (normal mode) %rH (humidity mode fix 0%rH)
    ************************************************************************/
    float getScaleBottom();

    /***********************************************************************
    get measrange top
    Can be used in manual mode to define maximal scale top.
    // intern note: returns measrange + tolerance range
    @return measrange top in degree Celsius
    ************************************************************************/
    float getMeasrangeTop();

    /***********************************************************************
    get measrange bottom
    Can be used in manual mode to define minimal scale bottom.
    @return measrange bottom in degree Celsius
    ************************************************************************/
    float getMeasrangeBottom();

    /**
    *************************************************************************
    Set manual scale mode with bottom and top scale temperature
    (throws if min or max is outside the current extended measurement range)

    note: Manual scale can not be used in humidity mode!

    @param [in] fTemperatureBottom in degree Celsius
    @param [in] fTemperatureTop in degree Celsius
    ************************************************************************/
    void setScalingToManualScale(float fTemperatureBottom, float fTemperatureTop);

    /**
    *************************************************************************
    Set to auto scale mode

    Scale top and bottom will be calculated by the minimal and maximal values
    ************************************************************************/
    void setScalingToAutoScale();

    /**
    *************************************************************************
    setScaleAssist

    note: Scale assist can not be used in humidity mode!
    @param [in] fTemperatureInside     scaling In temperature
    @param [in] fTemperatureOutside    scaling Out temperature
    ************************************************************************/
    void setScaleToScaleAssist(float fTemperatureInside, float fTemperatureOutside);

    /**
    *************************************************************************
    getScaleAssistTemperatureInside
    @return temperature inside for scale assist mode
    ************************************************************************/
    float getScaleAssistTemperatureInside();

    /**
    *************************************************************************
    getScaleAssistTemperatureOutside
    @return temperature outside for scale assist mode
    ************************************************************************/
    float getScaleAssistTemperatureOutside();

    /**
    **************************************************************************
    MEASUREMENT POINTS
    **************************************************************************/

    /**
    *************************************************************************
    listAvailableMeasPoints
    get a list of all available measurement point keys (active and inactive)
    e.g. {'CenterSpot', HotSpot', 'ColdSpot'}
    ************************************************************************/
    std::vector<std::string> listAvailableMeasPoints() const;

    /**
    *************************************************************************
    getActiveMeasPoints
    @return get all active meas points names, position and values
    (value unit: degree celcius or surface moisture)
    e.g. <Laser, irapi::MeasPoint(60,75, 22.5 degree Celsius)>
    ************************************************************************/
    std::map<std::string, irapi::MeasPoint> getActiveMeasPoints() const;

    /**
    *************************************************************************
    setMeasPointState
    activate or deactivate a measurement point
    activate measurement points will appar in getActiveMeasPoints

    @param [in] strKey measurement point
    @param [in] bEnable enable state
    ************************************************************************/
    void setMeasPointState(const std::string& strKey, bool bEnable);


    /**
    *************************************************************************
    setMeasPoint
    Set a new measpoint. Overwriting a standard measpoint is protected
    and will cause an std::exception.
    Note: to set the measpoint only the coordinates must be defined of an
          irapi::MeasPoint struct. The value will be replaced by this function!

    @param [in] strKey measurement point
    @param [in/out] measpoint struct (coordinates must be set to desired position)
                    the measpoint value is calculated
    ************************************************************************/
    void setMeasPoint(const std::string& strKey, irapi::MeasPoint& measpoint);


    /**
    **************************************************************************
    MEASUREMENT AREA
    **************************************************************************/

    /**
    *************************************************************************
    listAvailableMeasAreas
    list available measurement areas

    note: for t87x there is only one area that is defined by the camera.
          Additional areas can be added but are not visible on the camera image browser.
    ************************************************************************/
    std::vector<std::string> listAvailableMeasAreas() const;

    /**
    *************************************************************************
    getMeasAreas
    @return get all active meas areas (return empty vector if no area is not active)

    note: for rt87x ir cameras there is only one or none active area
          that is defined by the camera. Additional areas can be added.
    ************************************************************************/
    std::map<std::string, MeasArea> getActiveMeasAreas() const;

    /**
    *************************************************************************
    setAreaActive changes the area active state
    @param [in] strKey measurement area
    @param [in] flag area active
    ************************************************************************/
    void setMeasAreaState(const std::string& strKey, bool bEnable);


    /**
    *************************************************************************
    removeMeasArea
    Delete a measurement area. Only added measurement areas can be removed.

    @param [in] strKey measurement area
    ************************************************************************/
    void removeMeasArea(const std::string& strKey);

    /**
    *************************************************************************
    setMeasArea
    Set a new measurement area. The standard area 'CenterArea' is protected.
    Overwriting will cause an std::exception.

    @param [in] strKey - measurement point
    @param [in/out] area -  area struct (coordinates must be set to desired position)
                    the measurement point values are calculated
    ************************************************************************/
    void setMeasArea(const std::string& strKey, MeasArea& area);





    /**
    *************************************************************************
    removeMeasPoint
    Delete a measpoint. Only optional measurement points can be removed.

    @param [in] strKey measurement point
    ************************************************************************/
    void removeMeasPoint(const std::string& strKey);
    /**
    **************************************************************************
    HUMDITY MODE

    In humidity mode the temperatures of the image are converted in
    surface moisture values.

    This values are calcualted using a reference humidity value and ambient
    temperature. The reference humidity ambient temperature can be measured
    with a probe connected to the camera or set by manual imput.
    ***************************************************************************/

    /**
    *************************************************************************
    Set humidity mode active
    @param [in] bActive humidity mode active
    ************************************************************************/
    void setHumidityModeActive(bool bActive);

    /**
    *************************************************************************
    Get humidity mode active status
    @return true if humidity mode is active
    ************************************************************************/
    bool getHumidityModeActive() const;

    /**
    *************************************************************************
    Set reference humidity
    @param [in] fHumidity relative humidity in percent at reference ambient temperature
    (range 0 ...100%)
    ************************************************************************/
    void setHumidity(float fHumidity);

    /**
    *************************************************************************
    Get reference humidity at current ambient temperature
    @return reference humidity
    ************************************************************************/
    float getHumidity() const;

    /**
    *************************************************************************
    Set ambient temperature
    @param [in] temperature ambient temperature in degree Celsius
    ************************************************************************/
    void setAmbientTemperature(float fTemperature);
    /**
    *************************************************************************
    Get ambient temperature
    @return ambient temperature in degree Celsius
    ************************************************************************/
    float getAmbientTemperature() const;





    /**
    *************************************************************************
    Save current state of the image to a file
    ************************************************************************/
    void save(const std::string& strFilename);

    /**
    *************************************************************************
    encode current state if the image to BMT format
    @return binary data from BMT
    ************************************************************************/
    std::vector<char> exportData();

    /**
    *************************************************************************
    isSuperResolutionActive

    @return true if super resolution is active
    ************************************************************************/
    bool isSuperResolutionActive();

    /**
    *************************************************************************
    getActiveMeasapplicationValue get values for any additional measurement application
    that was recorded during image capture (like manual input values or
    values from an connected BT sensor)

    @return return pair of measurement {application name, value}
            application names:
            {None, Current, Humidity, RadiationIntensity, Power, Voltage}
            the measurement value is set coressponding to measurement application name
            using only basic unit Ampare, Watt, Voltage...
            Note: in case of a Humitiy sensor the values can be also extracted
            by using the getHumidity and getAmbientTemperature function
    ************************************************************************/
    std::pair<std::string, float> getActiveMeasapplicationValue();

    friend ridgid::IrImageRidgid;
  private:
    testo::exp::IrImage* getImageClass();

    testo::exp::IrImage* m_pImage;
    testo::vision::ColorPaletteConverter* m_pConverter;

  };
}

#endif