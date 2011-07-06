/*!
  \file
  \brief OpenNI で指定した X-Y 平面の深度情報を取得するためのクラス

  \author Satofumi KAMIMURA

  $Id$
*/

#include "Depth_2d_driver.h"
#include <XnCppWrapper.h>
#include <opencv/cv.h>


namespace
{
    enum {
      Min_distance = 500,
      Max_distance = 10000,
    };


    const char* Config_file = "../SampleConfig.xml";
}


struct Depth_2d_driver::pImpl
{
    IplImage* camera_;
    bool is_initialized_;
    xn::DepthGenerator depth_;


    pImpl(void) : camera_(NULL)
    {
    }


    bool open(void)
    {
        // 途中で初期化に失敗したときに、適切にクリーンアップされるようにすべき
        xn::Context context;
	XnStatus status = context.InitFromXmlFile(Config_file);
	if (status != XN_STATUS_OK) {
	    return false;
	}

#if 0
	xn::ImageGenerator image;
	status = context.FindExistingNode(XN_NODE_TYPE_IMAGE, image);
	if (status != XN_STATUS_OK) {
	    return false;
	}
#endif

	status = context.FindExistingNode(XN_NODE_TYPE_DEPTH, depth_);
	if (status != XN_STATUS_OK) {
	    return false;
	}

	return true;
    }
};


Depth_2d_driver::Depth_2d_driver(void) : pimpl(new pImpl)
{
    // !!!
}


Depth_2d_driver::~Depth_2d_driver(void)
{
    // !!!
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

    return pimpl->open();
}


void Depth_2d_driver::close(void)
{
    // !!!
}


bool Depth_2d_driver::is_open(void) const
{
    // !!!
    return false;
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
    (void)scan_times;
    (void)skip_scan;
    // !!!
    return false;
}


bool Depth_2d_driver::get_distance(std::vector<long>& data, long *time_stamp)
{
    (void)data;
    (void)time_stamp;
    // !!!
    return false;
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
    (void)index;
    // !!!
    return 0.0;
}


double Depth_2d_driver::index2deg(int index) const
{
    (void)index;
    // !!!
    return 0.0;
}


int Depth_2d_driver::rad2index(double radian) const
{
    (void)radian;
    // !!!
    return 0;
}


int Depth_2d_driver::deg2index(double degree) const
{
    (void)degree;
    // !!!
    return 0;
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
    // !!!
    return 0;
}


int Depth_2d_driver::max_step(void) const
{
    // !!!
    return 0;
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
    // !!!
    return 0;
}


int Depth_2d_driver::max_echo_size(void) const
{
    // !!!
    return 0;
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
