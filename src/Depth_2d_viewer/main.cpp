/*!
  \file
  \brief Kinect センサの X-Y 平面データの 2D 表示

  \author Satofumi KAMIMURA

  $Id$
*/

#include "Depth_2d_viewer_window.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Depth_2d_viewer_window window;
    window.show();

    return app.exec();
}
