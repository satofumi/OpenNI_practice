/*!
  \file
  \brief 計測データの取得

  \author Satofumi KAMIMURA

  $Id$
*/

#include "Length_receive_thread.h"
#include "Lidar.h"

using namespace qrk;
using namespace std;


struct Length_receive_thread::pImpl
{
    Lidar& lidar_;


    pImpl(Lidar& lidar) : lidar_(lidar)
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
    // データの取得
    vector<long> data;
    pimpl->lidar_.get_distance(data, NULL);

    // データの描画
    // !!!
#if 0
    void set_draw_data(const long* data, int data_size,
                       long timestamp, const QColor& color,
                       bool is_intensity = false);
#endif
}
