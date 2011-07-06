/*!
  \file
  \brief 計測データの取得

  \author Satofumi KAMIMURA

  $Id$
*/

#include "Length_receive_thread.h"
#include "Lidar.h"

#include <cstdio>

using namespace qrk;
using namespace std;


struct Length_receive_thread::pImpl
{
    Lidar& lidar_;
    bool stop_;


    pImpl(Lidar& lidar) : lidar_(lidar), stop_(false)
    {
    }
};


Length_receive_thread::Length_receive_thread(qrk::Lidar& lidar)
    : pimpl(new pImpl(lidar))
{
}


Length_receive_thread::~Length_receive_thread(void)
{
}


void Length_receive_thread::run(void)
{
    pimpl->stop_ = false;

    // データの取得
    vector<long> data;
    while (!pimpl->stop_) {
        pimpl->lidar_.get_distance(data, NULL);

	// データの描画
	// !!!
	fprintf(stderr, "%d\n", data.size());
#if 0
	void set_draw_data(const long* data, int data_size,
			   long timestamp, const QColor& color,
			   bool is_intensity = false);
#endif
    }
}
