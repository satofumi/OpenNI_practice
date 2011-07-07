/*!
  \file
  \brief 計測データの取得

  \author Satofumi KAMIMURA

  $Id$
*/

#include "Length_receive_thread.h"
#include "Length_draw_widget.h"
#include "Lidar.h"
#include <QColor>

#include <cstdio>

using namespace qrk;
using namespace std;


struct Length_receive_thread::pImpl
{
    Lidar& lidar_;
    Length_draw_widget& length_draw_widget_;
    bool stop_;


    pImpl(Lidar& lidar, Length_draw_widget& length_draw_widget)
      : lidar_(lidar), length_draw_widget_(length_draw_widget), stop_(false)
    {
    }
};


Length_receive_thread::Length_receive_thread(qrk::Lidar& lidar,
					     Length_draw_widget&
					     length_draw_widget)
    : pimpl(new pImpl(lidar, length_draw_widget))
{
}


Length_receive_thread::~Length_receive_thread(void)
{
}


void Length_receive_thread::run(void)
{
    pimpl->stop_ = false;
    pimpl->length_draw_widget_.start_measurement();

    // データの取得
    vector<long> data;

    while (!pimpl->stop_) {
        pimpl->lidar_.get_distance(data, NULL);

	// データの登録
	QColor color(0, 0, 255);
#if 0
	pimpl->length_draw_widget_.
	  set_draw_data(&data[0], data.size(), 0, color);
	//pimpl->length_draw_widget_.redraw();
#else
	emit data_received(&data[0], data.size(), NULL, 0, 0);
#endif
    }
}
