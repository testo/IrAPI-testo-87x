/***************************************************************************
* Copyright: Testo SE & Co. KGaA, Testo-Straße 1, 79849 Lenzkirch
***************************************************************************/
/**@file
@brief<b>Description: </b> camera interface class

***************************************************************************/

#ifndef IR_API_CAM_H
#define IR_API_CAM_H

/***************************************************************************
* Includes
***************************************************************************/

// standard include for testo project defines basic data types from std lib, decl
#include <ircam2020/interface/irapi/IrapiConfig.h>

#include <opencv2/core/core.hpp>

#include <map>
#include <string>
#include <vector>
#include <atomic>
#include <memory>
#include <thread>

#include "IrTypes.h"

// Forward declaration for intern private members
namespace testo
{
  namespace exp
  {
    namespace com
    {
      class IrCamMobileApp;
    }
  }
}



namespace irapi
{
    /**
    **************************************************************************
    @brief camera communication interface API for RIDGIG mobile app

    \ingroup interfaces
    **************************************************************************/
  class IRCAM2020_IRAPI_DECL Cam
  {
  public:
    /**
    **************************************************************************
    Constructor

    the interface class will start to try to connect to an ir camera
    over TCPIP/USB?(Windows) after construction

    The status of that connection can be read out with isConnected()
    ***************************************************************************/
    Cam(bool bActiveConnectMode = true);

    /**
    **************************************************************************
    Destructor

    closes open connection to the camera
    ***************************************************************************/
    ~Cam();

    /**
    **************************************************************************
    Disable copy constructor & assignment operator

    Object can not be copied since the connection to the camera is exclusive!
    Unique pointer could be used to hold a camera object.
    ***************************************************************************/
    Cam(const Cam& other) = delete;
    Cam& operator= (const Cam& rhs) = delete;

    /**
    **************************************************************************
    get connection status

    @return true as soon as a connection could be open to any ir camera
    **************************************************************************/
    bool isConnected();

    /**
    **************************************************************************
    wait until connected
    Note: This function is only useful in ActiveConnectMode

    @return true if connected false if timeout occurred
    **************************************************************************/
    bool waitUntilConnected(int nTimeoutMs);

    /**
    *************************************************************************
    Try to connect to a camera and return true if successful or false
    if not.

    Note: Use connect only if ActiveConnectMode is disabled!
    Functions throws std::exception if used with ActiveConnectHandling=True

    Note: The conenct is blocking the timeout depends on system socket connect
    timeout.

    @return true if successfully connected to a t87x camera
    ************************************************************************/
    bool connect();

    /**
    *************************************************************************
    Get device serial number
    @return uint64_t serial number
    ************************************************************************/
    uint64_t getDeviceSerialNumber();

    /**
    *************************************************************************
    Get device type name (e.g. "???")
    @return device type name
    ************************************************************************/
    std::string getDeviceType();

    /**
    *************************************************************************
    get a list of all file on the camera's image directory
    (throws if not connected std::exception)
    note: timestamp was added by request from ridgid

    @return list of all file names with its timestamps (data type std::map<std::string, uint64_t> )
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
            note: older Firmware version do not provide a timestamp (it will be set to zero)
    ************************************************************************/
    std::map<std::string, uint64_t> listDirectoryContent();

    /**
    *************************************************************************
    removeFile -- Removes an image file from the camera.
    Function throws if not connected std::exception but
    doesn't throw anything if file is not available.


    @param [in] strFileName file name (without path)
    ************************************************************************/
    void removeFile(const std::string& strFileName);

    /**
    *************************************************************************
    get the content of an image file
    (throws if not connected std::exception)

    @param [in] strFileName file name (without path)
    @return file content (binary data)
    can be stored to the disk to create the same file on the mobile device
    ************************************************************************/
    std::vector<char> getFileContent(const std::string& strFileName);

    /**
    *************************************************************************
    get ir preview image from a bmt file (does not work for other file types)
    functions throws if not connected std::exception or filename not exists
    Note: This function may not be available on older firmware versions
          and internal replaced by loading the complete file and extracting the preview

    @param [in] strFileName file name (without path)
    @return BGR mat
    ************************************************************************/
    cv::Mat3b getIrFilePreview(const std::string& strFileName);

    /**
    *************************************************************************
    trigger image capture on camera (image will be saved to camera memory)

    Function was added for test purpose only. It is asynchronous and will not wait
    until the image capture process is ready.
    ************************************************************************/
    void triggerImageCapture();

    /**
    *************************************************************************
    capture live ir image from the camera

    note: The first function call will activate the cameras streaming mode
          over wlan. To display the life images the function should be poll.
          It is essential to recall this function faster than the actual
          streaming framerate (4.5Hz) otherwise a delay will buil up.
          To stop the streaming mode the function stopLiveIr() should be called.

    @return IrFrame frame includes palletized image and the image temperature data
    ************************************************************************/
    IrFrame captureLiveIr();


    /**
    *************************************************************************
    stopLiveIr

    Call this function to stop the streaming mode.
    Note: The object destructor will also call the stop function if steaming
    is still active.
    ************************************************************************/
    void stopLiveIr();
  private:
    void connect_loop();

    void checkConnection() const;

    std::atomic<bool> m_bConnectLoopActive;
    bool m_bActiveConnectMode;

    std::unique_ptr<testo::exp::com::IrCamMobileApp> m_pDevice;
    std::thread m_thdScan;

    std::atomic<bool> m_bStreamActive;

  };
}


#endif