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
    xn::DepthGenerator depth_;

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
        status = context_.FindExistingNode(XN_NODE_TYPE_DEPTH, depth_);
        if (status != XN_STATUS_OK) {
            cerr << "FindExistingNode(XN_NODE_TYPE_DEPTH) failed." << endl;
            return false;
        }
      
        // デプスの座標をイメージに合わせる
        depth_.GetAlternativeViewPointCap().SetViewPoint(image_);

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


    void draw(void)
    {
        enum {
            Wall_x_start = 120,
            Wall_x_end = 520,
            Wall_y_start = 40,
            Wall_y_end = 440,
            Wall_depth = 2000,
        };

        xn::DepthMetaData depth_meta_data;
        depth_.GetMetaData(depth_meta_data);

        // 壁の手前にある物体を描画する
        char* dest = camera_->imageData;
        for (size_t y = 0; y < image_meta_data_.YRes(); ++y) {
            for (size_t x = 0; x < image_meta_data_.XRes(); ++x) {

                if ((x > Wall_x_start) && (x < Wall_x_end) &&
                    (y > Wall_y_start) && (y < Wall_y_end)) {
                    int depth = depth_meta_data(x, y);
                    if (depth > Wall_depth) {
                        dest[0] = 255;
                        dest[1] = 255;
                        dest[2] = 255;
                        dest += 3;
                        continue;
                    }
                }

                XnRGB24Pixel rgb = image_meta_data_.RGB24Map()(x, y);
                dest[0] = rgb.nRed;
                dest[1] = rgb.nGreen;
                dest[2] = rgb.nBlue;
                dest += 3;
            }
        }

        ::cvCvtColor(camera_, camera_, CV_BGR2RGB);
        ::cvShowImage("depth_wall", camera_);
    }


    void cleanup(void)
    {
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

    bool quit = false;
    while (!quit) {
        // データの取得
        receive_data();

        // 描画
        draw();

        char key = cvWaitKey(10);
        if (key == 'q') {
            quit = true;
        }
    }

    cleanup();

    return 0;
}  
