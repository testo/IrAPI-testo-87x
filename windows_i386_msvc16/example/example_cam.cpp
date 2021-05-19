
#include <irapi/Cam.h>
#include <irapi/Image.h>

#include <string>
#include <time.h>
#include <stdio.h>
#include <iostream>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

std::string convertTimestamp(uint64_t u64TimeStamp)
{
    // convert into readable time
  time_t t = time(NULL);
  memcpy(&t, &u64TimeStamp, sizeof(t));
  struct tm *tm = localtime(&t);
  char date[40];
  strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", tm);
  return std::string(date);
}

void loadIrImageFile(const std::vector<char>& vecData)
{
  // get preview image from buffer
  std::cout << "get preview image (ir BGR) from buffer\n";
  cv::Mat3b matPreview(irapi::Image::getIrImagePreview(vecData));
  std::cout << "preview image [0,0] B:" << int(matPreview(0,0)[0])
            << " G:" << int(matPreview(0,0)[1]) << " R:" << int(matPreview(0,0)[2])
            << std::endl << std::endl;

  // load image file from buffer
  irapi::Image image(vecData);

  std::cout << "serial      : " << image.getDeviceSerialNumber() << std::endl;
  std::cout << "device type : " << image.getDeviceName() << std::endl;
  std::cout << "timestamp   : " << image.getFileDateTime() << std::endl;
  std::cout << "record time : " << convertTimestamp(image.getFileDateTime()) << std::endl;

  // get ir image data
  cv::Mat3b matBrgIrImage(image.getIrImageBgr());
  // Note: All image temperature information is given in degree celsius
  cv::Mat_<float> matRadImage(image.getIrImageData());

  std::cout << "ir image [0,0] B:" << int(matPreview(0,0)[0])
            << " G:" << int(matPreview(0,0)[1]) << " R:" << int(matPreview(0,0)[2])
            << " = Temperature: " << matRadImage(0,0) << " Grad Celsius\n\n" << std::endl;
}

void streamIr(irapi::Cam* pCam)
{
  const char* szWindowName = "LiveImageData (opencv palette)";
  const char* szWindowName2 = "Scale";
  int nXpos = 100;
  int nYpos = 100;

  std::cout << "Press any key to end live view! (fokus stream window)" << std::endl;
  int nKey(-1);

  for (int i = 0; i < 1000; i++)
  {
    irapi::IrFrame frame;

    try
    {
      frame = pCam->captureLiveIr();
    }
    catch (std::exception& e)
    {
      // camera was disconnected?
      std::cout << "got error: " << e.what();
      return;
    }

    // use bgr image with camera palette
    cv::imshow(szWindowName, frame.matIrBgr);
    cv::moveWindow(szWindowName, nXpos, nYpos);

    /* simple scale element */
    cv::Mat3b matScaleBase;
    cv::Mat3b matScale;
    // flip scale colors since lowest value should be at the button
    cv::flip(frame.matScaleGradient, matScaleBase, 0);
    // resize scale to make fit to the image
    cv::resize(matScaleBase, matScale, cv::Size(30, frame.matIrBgr.rows));
    std::string strMax = std::to_string(frame.fScaleMax);
    if (strMax.size() > 4) strMax = strMax.substr(0, 4);
    std::string strMin = std::to_string(frame.fScaleMin);
    if (strMin.size() > 4) strMin = strMin.substr(0, 4);
    cv::putText(matScale, strMax, cv::Point(0, 15), cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar::all(0));
    cv::putText(matScale, strMin, cv::Point(0, matScale.rows - 15), cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar::all(255));

    cv::imshow(szWindowName2, matScale);
    cv::moveWindow(szWindowName2, nXpos + frame.matIrBgr.cols + 20, nYpos);


    nKey = cv::waitKey(10);
    if (nKey != -1) break; // end loop early
  }

  pCam->stopLiveIr();
  cv::destroyAllWindows();
}



int main(int argc,char *argv[])
{
  // This program tries to connect to a testo ir camera and load on image from the memory

  // NOTE: In order to run successfully the WLAN / USB-RNDIS connection must already be established!
  // NOTE: Also your firewall setup must allow this program a connection to the camera.
  // Normally one the first run the firewall prompt will pop up and ask you to allow this connection.

  // create a camera object and wait until it is connected
  irapi::Cam cam;

  // poll object unitl it is conencted or timeout reached
  // alternative: if this thread should not block
  // isConnected() can be used to determine the connection state
  if(!cam.waitUntilConnected(6000))
  {
    std::cout << "could not connect to a camera! --> end programm here";
    return -1;
    // Note: the return will call the destructor of the cam object
    //       the destructor will wait until the conenction thread is ended.
    //       Due to OS dependent timeout on TCPIP sockets it could block for a while.
  }

  // print camera information
  std::cout << "Connected to camera" << std::endl;
  std::cout << "Serial: " << cam.getDeviceSerialNumber() << std::endl;
  std::cout << "DeviceType: " << cam.getDeviceType() << std::endl;

  // get image file list from the camera
  std::map<std::string, uint64_t> mapFiles(cam.listDirectoryContent());

  if(mapFiles.size() == 0U)
  {
    // if there is no file take one and get the list again
    // this command is asychon and only for test purpose
    // so we have to wait apx. 2 sec. to be shure a image is taken
    std::cout << "capture image..." << std::endl;
    cam.triggerImageCapture();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "done..." << std::endl;
    mapFiles = cam.listDirectoryContent();
  }


  // list image files on camera
  std::string strLastFileName;
  std::cout << "Filename      --  UTC stamp -- local time string\n";
  for (auto item : mapFiles)
  {
    std::cout << item.first << "  --  " << item.second  << " -- " << convertTimestamp(item.second) << std::endl;
    strLastFileName = item.first;
  }
  std::cout << std::endl;


  // load last listed image from the camera
  if(!strLastFileName.empty())
  {
    // load preview image..
    auto matPreview = cam.getIrFilePreview(strLastFileName);
    std::cout << "got preview image pixelcount: " << matPreview.total() << std::endl;
    // load last file
    std::vector<char> vecData = cam.getFileContent(strLastFileName);
    std::cout << "\nLoad image: " << strLastFileName << std::endl << std::endl;
    loadIrImageFile(vecData);
  }

  // test simple ir streaming
  streamIr(&cam);

#if 0
    std::cout << "\nPress ENTER to exit!\n>";
    std::cin.ignore();
#endif
}
