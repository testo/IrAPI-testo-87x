
#include <irapi/Image.h>

#include <string>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

std::string convertTimestamp(uint64_t u64TimeStamp)
{
  // convert into readable time
  time_t t = time(NULL);
  memcpy(&t, &u64TimeStamp, sizeof(t));
  struct tm* tm = localtime(&t);
  char date[40];
  strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", tm);
  return std::string(date);
}

void loadIrImageFile(const std::string& strFileName)
{
  // get preview image from buffer
  std::cout << "get preview image (ir BGR) from buffer\n";

  std::cout << "get preview image (ir BGR) from buffer\n";
  cv::Mat3b matPreview;
  try
  {
    matPreview = irapi::Image::getIrImagePreview(strFileName);
  }
  catch (std::exception& e)
  {
    std::cout << "Error open BMT file: " << e.what() << std::endl;
    return;
  }

  std::cout << "preview image [0,0] B:" << int(matPreview(0, 0)[0])
    << " G:" << int(matPreview(0, 0)[1]) << " R:" << int(matPreview(0, 0)[2])
    << std::endl << std::endl;

  // load image file from buffer
  irapi::Image image(strFileName);

  std::cout << "serial      : " << image.getDeviceSerialNumber() << std::endl;
  std::cout << "device type : " << image.getDeviceName() << std::endl;
  std::cout << "timestamp   : " << image.getFileDateTime() << std::endl;
  std::cout << "record time : " << convertTimestamp(image.getFileDateTime()) << std::endl;

  // get ir image data
  cv::Mat3b matBrgIrImage(image.getIrImageBgr());
  // Note: All image temperature information is given in degree celsius
  cv::Mat_<float> matRadImage(image.getIrImageData());

  std::cout << "ir image [0,0] B:" << int(matPreview(0, 0)[0])
    << " G:" << int(matPreview(0, 0)[1]) << " R:" << int(matPreview(0, 0)[2])
    << " = Temperature: " << matRadImage(0, 0) << " Grad Celsius\n\n" << std::endl;

  const char szWindowNameIr[] = "BMT EXAMPLE IMAGE IR";
  cv::imshow(szWindowNameIr, matBrgIrImage);
  cv::moveWindow(szWindowNameIr, 20, 20);
  const char szWindowNameVis[] = "BMT EXAMPLE IMAGE VIS";
  cv::imshow(szWindowNameVis, image.getSmallVisualImage());
  cv::moveWindow(szWindowNameVis, matBrgIrImage.cols + 40, 20);
  cv::waitKey(10000);
  cv::destroyAllWindows();
}

bool existsFile(const std::string& strFilename)
{
  std::ifstream ifs(strFilename, std::ifstream::in);
  return ifs.is_open();
}

int main(int argc, char* argv[])
{
  // This program shows a simple useage if irapi::Image

  std::string strBmtFile;
  if (argc > 1)
  {
    // If provided, use the command line argument
    strBmtFile = argv[1];
  }
  else
  {
    strBmtFile = "IR_EXAMPLE.BMT";
    if (!existsFile(strBmtFile))
    {
      // If not, try to find IR_EXAMPLE.BMT in the source directory
      strBmtFile = __FILE__;
      strBmtFile = strBmtFile.substr(0, strBmtFile.rfind("\\") + 1) + "IR_EXAMPLE.BMT";

      if (!existsFile(strBmtFile))
      {
        std::cout << "Could not default example BMT file: IR_EXAMPLE.BMT!\n";
        std::cout << "Please provide a bmt file as call parameter.\n";
      }
    }

  }
  std::cout << "Opening image file " << strBmtFile << std::endl;

  loadIrImageFile(strBmtFile);
  std::cout << "\nPress ENTER to exit!\n>";
  std::cin.ignore();
}
