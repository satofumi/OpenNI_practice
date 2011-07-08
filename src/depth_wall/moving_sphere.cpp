/*!
  \file
  \brief 物体と人の描画サンプル

  "Kinect センサープログラミング" (秀和システム) の 5.1 光学迷彩を参考にした。

  \author Satofumi KAMIMURA

  $Id$
*/

#include <XnCppWrapper.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

using namespace std;


namespace
{
  const char* Config_file = "../SamplesConfig.xml";

  xn::Context context_;
  xn::ImageGenerator image_;

  xn::ImageMetaData image_meta_data_;

  IplImage* camera_ = NULL;


  bool initialize(void)
  {
    // OpenNI の初期化
    XnStatus status = context_.InitFromXmlFile(Config_file);
    if (status != XN_STATUS_OK) {
      cerr << "InitFromXmlFile() failed." << endl;
      return false;
    }

    // イメージジェネレータの作成
    status = context_.FindExistingNode(XN_NODE_TYPE_IMAGE, image_);
    if (status != XN_STATUS_OK) {
      cerr << "FindExistingNode(XN_NODE_TYPE_IMAGE) failed." << endl;
      return false;
    }

    // デプスジェネレータの作成
    xn::DepthGenerator depth;
    status = context_.FindExistingNode(XN_NODE_TYPE_DEPTH, depth);
    if (status != XN_STATUS_OK) {
      cerr << "FindExistingNode(XN_NODE_TYPE_DEPTH) failed." << endl;
      return false;
    }
      
    // デプスの座標をイメージに合わせる
    depth.GetAlternativeViewPointCap().SetViewPoint(image_);

    // カメラからイメージを作成
    XnMapOutputMode output_mode;
    image_.GetMapOutputMode(output_mode);
    camera_ = ::cvCreateImage(cvSize(output_mode.nXRes, output_mode.nYRes),
			      IPL_DEPTH_8U, 3);
    if (!camera_) {
      cerr << "cvCreateImage() failed." << endl;
      return false;
    }

    return true;
  }


  void receive_data(void)
  {
    context_.WaitAndUpdateAll();

    // 画像の取得
    image_.GetMetaData(image_meta_data_);
  }


  void draw(int sphere_x, int sphere_y);
  {
    // 球の表示
    // !!!

    // 球の手前にある物体を描画する
    char* dest = camera_->imageData;
    for (size_t y = 0; y < image_meta_data_.YRes(); ++y) {
      for (size_t x = 0; x < image_meta_data_.XRes(); ++x) {
	XnRGB24Pixel rgb = image_meta_data_.RGB24Map()(x, y);
	dest[0] = rgb.nRed;
	dest[1] = rgb.nGreen;
	dest[2] = rgb.nBlue;
	dest += 3;
      }
    }

    ::cvShowImage("moving_sphere", camera_);
  }


  void cleanup(void)
  {
    ::cvReleaseImage(&sphere_);
    ::cvReleaseImage(&camera_);
  }
}


int main(int argc, char *argv[])
{
  static_cast<void>(argc);
  static_cast<void>(argv);

  if (!initialize()) {
    return 1;
  }

  double sphere_radian = 0.0;
  bool quit = false;
  while (!quit) {
    // データの取得
    receive_data();

    // Sphere の移動処理
    const double Moving_degree = 3.0;
    const double Moving_radius = 1000.0;
    const double Moving_radius_y = 100.0;
    sphere_radian += Moving_degree * M_PI / 180.0;
    int sphere_depth = Moving_radius * cos(sphere_radian);
    int sphere_x = Moving_radius * cos(sphere_radian);
    int sphere_y = Moving_radius_y * sin(sphere_radian);
    // !!!
    cout << sphere_x << ", " << sphere_y << endl;


    // 位置を考慮した描画
    draw(sphere_x, sphere_y);

    char key = cvWaitKey(10);
    if (key == 'q') {
      quit = true;
    }
  }

  cleanup();

  return 0;
}  
