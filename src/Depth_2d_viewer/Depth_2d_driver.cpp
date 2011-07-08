/*!
  \file
  \brief OpenNI で指定した X-Y 平面の深度情報を取得するためのクラス

  \author Satofumi KAMIMURA

  $Id$
*/

#include "Depth_2d_driver.h"
#include <XnCppWrapper.h>
#include "math_utilities.h"


namespace
{
    enum {
        Min_distance = 500,
        Max_distance = 10000,

	Default_min_step = 0,
	Default_max_step = 640,
    };

    const char* Config_file = "../SamplesConfig.xml";

    const double Vertical_viewing_deg_ = 57.0;
}


struct Depth_2d_driver::pImpl
{
    xn::Context context_;
    xn::DepthGenerator depth_;
  bool is_open_;
  int min_step_;
  int max_step_;


  pImpl(void) : is_open_(false),
		min_step_(Default_min_step), max_step_(Default_max_step)
  {
  }


    bool open(void)
    {
        // 途中で初期化に失敗したときに、適切にクリーンアップされるようにすべき
	XnStatus status = context_.InitFromXmlFile(Config_file);
	if (status != XN_STATUS_OK) {
	    fprintf(stderr, "InitFromXmlFile() failed.\n");
	    return false;
	}

	status = context_.FindExistingNode(XN_NODE_TYPE_DEPTH, depth_);
	if (status != XN_STATUS_OK) {
	    fprintf(stderr, "FindExistingNode() failed.\n");
	    return false;
	}

	return true;
    }
};


Depth_2d_driver::Depth_2d_driver(void) : pimpl(new pImpl)
{
}


Depth_2d_driver::~Depth_2d_driver(void)
{
}


const char* Depth_2d_driver::what(void) const
{
    // !!!
    return "Not implemented.";
}


bool Depth_2d_driver::open(const char* device_name, long baudrate,
                           connection_type_t type)
{
    static_cast<void>(device_name);
    static_cast<void>(baudrate);
    static_cast<void>(type);

    pimpl->is_open_ = pimpl->open();
    return pimpl->is_open_;
}


void Depth_2d_driver::close(void)
{
    if (pimpl->is_open_) {
        // !!!
    }
    pimpl->is_open_ = false;
}


bool Depth_2d_driver::is_open(void) const
{
    return pimpl->is_open_;
}


void Depth_2d_driver::set_timeout_msec(int msec)
{
    (void)msec;
    // !!!
}


bool Depth_2d_driver::laser_on(void)
{
    // !!!
    return false;
}


bool Depth_2d_driver::laser_off(void)
{
    // !!!
    return false;
}


void Depth_2d_driver::reboot(void)
{
    // !!!
}


void Depth_2d_driver::sleep(void)
{
    // !!!
}


void Depth_2d_driver::wakeup(void)
{
    // !!!
}


bool Depth_2d_driver::is_stable(void)
{
    // !!!
    return false;
}


bool Depth_2d_driver::start_measurement(measurement_type_t type,
                                        int scan_times, int skip_scan)
{
    static_cast<void>(type);
    static_cast<void>(scan_times);
    static_cast<void>(skip_scan);

    // !!!

    return false;
}


bool Depth_2d_driver::get_distance(std::vector<long>& data, long *time_stamp)
{
    static_cast<void>(time_stamp);

    pimpl->context_.WaitAndUpdateAll();

    xn::DepthMetaData depth_meta_data;
    pimpl->depth_.GetMetaData(depth_meta_data);

    // センサを上から見たときと同じデータ配置になるように格納して返す
    // !!! Mirror の仕組みを使うことを検討する
    int width = depth_meta_data.XRes();

    // 中心の高さのデータを返す
    int scan_height = depth_meta_data.YRes() / 2;

    data.clear();
    data.reserve(width);
    for (int x = width - 1; x >= 0; --x) {
        long l = depth_meta_data(x, scan_height);
	double radian = index2rad(x);
        data.push_back(l / cos(radian));
    }

    return true;
}


bool Depth_2d_driver::get_distance_intensity(std::vector<long>& data,
                                             std::vector<unsigned short>&
                                             intensity, long *time_stamp)
{
    (void)data;
    (void)intensity;
    (void)time_stamp;
    // !!!
    return false;
}


bool Depth_2d_driver::get_multiecho(std::vector<long>& data_multi,
                                    long* time_stamp)
{
    (void)data_multi;
    (void)time_stamp;
    // !!!
    return false;
}


bool Depth_2d_driver::get_multiecho_intensity(std::vector<long>& data_multiecho,
                                              std::vector<unsigned short>&
                                              intensity_multiecho,
                                              long* time_stamp)
{
    (void)data_multiecho;
    (void)intensity_multiecho;
    (void)time_stamp;
    // !!!
    return false;
}


bool Depth_2d_driver::set_scanning_parameter(int first_step, int last_step,
                                             int skip_step)
{
    (void)first_step;
    (void)last_step;
    (void)skip_step;
    // !!!
    return false;
}


void Depth_2d_driver::stop_measurement(void)
{
    // !!!
}


bool Depth_2d_driver::set_sensor_time_stamp(long time_stamp)
{
    (void)time_stamp;
    // !!!
    return false;
}


double Depth_2d_driver::index2rad(int index) const
{
  return (index - (pimpl->max_step_ / 2)) * Vertical_viewing_deg_ / pimpl->max_step_ * M_PI / 180.0;
}


double Depth_2d_driver::index2deg(int index) const
{
    return index2rad(index) * 180.0 / M_PI;
}


int Depth_2d_driver::rad2index(double radian) const
{
    (void)radian;
    // !!!
    return 0;
}


int Depth_2d_driver::deg2index(double degree) const
{
    return deg2index(degree * M_PI / 180.0);
}


int Depth_2d_driver::rad2step(double radian) const
{
    (void)radian;
    // !!!
    return 0;
}


int Depth_2d_driver::deg2step(double degree) const
{
    (void)degree;
    // !!!
    return 0;
}


double Depth_2d_driver::step2rad(int step) const
{
    (void)step;
    // !!!
    return 0.0;
}


double Depth_2d_driver::step2deg(int step) const
{
    (void)step;
    // !!!
    return 0.0;
}


int Depth_2d_driver::step2index(int step) const
{
    (void)step;
    // !!!
    return 0;
}


int Depth_2d_driver::min_step(void) const
{
    return pimpl->min_step_;
}


int Depth_2d_driver::max_step(void) const
{
    return pimpl->max_step_;
}


long Depth_2d_driver::min_distance(void) const
{
    return Min_distance;
}


long Depth_2d_driver::max_distance(void) const
{
    return Max_distance;
}


long Depth_2d_driver::scan_usec(void) const
{
    // !!!
    return 0;
}


int Depth_2d_driver::max_data_size(void) const
{
    return pimpl->max_step_;
}


int Depth_2d_driver::max_echo_size(void) const
{
    return 1;
}


const char* Depth_2d_driver::product_type(void) const
{
    // !!!
    return "Not implemented.";
}


const char* Depth_2d_driver::firmware_version(void) const
{
    // !!!
    return "Not implemented.";
}


const char* Depth_2d_driver::serial_id(void) const
{
    // !!!
    return "Not implemented.";
}


const char* Depth_2d_driver::status(void) const
{
    // !!!
    return "Not implemented.";
}


const char* Depth_2d_driver::state(void) const
{
    // !!!
    return "Not implemented.";
}


int Depth_2d_driver::raw_write(const char* data, size_t data_size)
{
    (void)data;
    (void)data_size;
    // !!!
    return 0;
}


int Depth_2d_driver::raw_read(char* data, size_t max_data_size, int timeout)
{
    (void)data;
    (void)max_data_size;
    (void)timeout;
    // !!!
    return 0;
}


int Depth_2d_driver::raw_readline(char* data, size_t max_data_size, int timeout)
{
    (void)data;
    (void)max_data_size;
    (void)timeout;
    // !!!
    return 0;
}
