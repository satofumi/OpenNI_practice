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


namespace
{
  const char* Config_file = "../SamplesConfig.xml";
  const char* Sphere_file = "sphere.png";

  xn::Context context_;
  xn::ImageGenerator image_;
  xn::UserGenerator user_;

  IplImage* camera_ = NULL;
  IplImage* sphere_ = NULL;


  bool initialize(void)
  {
    // OpenNI の初期化
    XnStatus status = context_.InitFromXmlFile(Config_file);
    if (status != XN_STATUS_OK) {
      return false;
    }

    // イメージジェネレータの作成
    status = context_.FindExistingNode(XN_NODE_TYPE_IMAGE, image_);
    if (status != XN_STATUS_OK) {
      return false;
    }

    // デプスジェネレータの作成
    xn::DepthGenerator depth;
    status = context_.FindExistingNode(XN_NODE_TYPE_DEPTH, depth);
    if (status != XN_STATUS_OK) {
      return false;
    }
      
    // デプスの座標をイメージに合わせる
    depth.GetAlternativeViewPointCap().SetViewPoint(image_);

    // ユーザの作成
    status = context_.FindExistingNode(XN_NODE_TYPE_USER, user_);
    if (status != XN_STATUS_OK) {
      return false;
    }

    // カメラからイメージを作成
    XnMapOutputMode output_mode;
    image_.GetMapOutputMode(output_mode);
    camera_ = ::cvCreateImage(cvSize(output_mode.nXRes, output_mode.nYRes),
			      IPL_DEPTH_8U, 3);
    if (!camera_) {
      return false;
    }

    // 画像の読み出し
    sphere_ = cvLoadImage(Sphere_file);
    if (!sphere_) {
      return false;
    }
  }


  void receive_data(void)
  {
    context_.WaitAndUpdateAll();

    // 画像の取得
    // !!!

    // ユーザデータの取得
    // !!!
  }


  void draw(void)
  {
    // !!!
  }
}


int main(int argc, char *argv[])
{
  static_cast<void>(argc);
  static_cast<void>(argv);

  if (!initialize()) {
    return 1;
  }

  bool quit = false;
  while (quit) {
    // データの取得
    receive_data();

    // Sphere の移動処理
    // !!!

    // 人の検出
    // !!!

    // 位置を考慮した描画
    // !!!

    char key = cvWaitKey(10);
    if (key == 'q') {
      quit = true;
    }
  }

  return 0;
}  
