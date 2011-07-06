#ifndef DEPTH_2D_DRIVER_H
#define DEPTH_2D_DRIVER_H

/*!
  \file
  \brief OpenNI で指定した X-Y 平面の深度情報を取得するためのクラス

  \author Satofumi KAMIMURA

  $Id$
*/

#include <vector>
#include <cstddef>
#include <memory>
#include "Lidar.h"


class Depth_2d_driver : public qrk::Lidar
{
public:
    Depth_2d_driver(void);
    ~Depth_2d_driver(void);

    const char* what(void) const;
    bool open(const char* device_name, long baudrate, connection_type_t type);
    void close(void);
    bool is_open(void) const;

    void set_timeout_msec(int msec);

    bool laser_on(void);
    bool laser_off(void);

    void reboot(void);

    void sleep(void);
    void wakeup(void);
    bool is_stable(void);

    bool start_measurement(measurement_type_t type,
                           int scan_times, int skip_scan);

    bool get_distance(std::vector<long>& data, long *time_stamp);
    bool get_distance_intensity(std::vector<long>& data,
                                std::vector<unsigned short>& intensity,
                                long *time_stamp);

    bool get_multiecho(std::vector<long>& data_multi, long* time_stamp);

    bool get_multiecho_intensity(std::vector<long>& data_multiecho,
                                 std::vector<unsigned short>&
                                 intensity_multiecho, long* time_stamp);

    bool set_scanning_parameter(int first_step, int last_step, int skip_step);

    void stop_measurement(void);

    bool set_sensor_time_stamp(long time_stamp);

    double index2rad(int index) const;
    double index2deg(int index) const;
    int rad2index(double radian) const;
    int deg2index(double degree) const;
    int rad2step(double radian) const;
    int deg2step(double degree) const;
    double step2rad(int step) const;
    double step2deg(int step) const;
    int step2index(int step) const;

    int min_step(void) const;
    int max_step(void) const;
    long min_distance(void) const;
    long max_distance(void) const;
    long scan_usec(void) const;
    int max_data_size(void) const;
    int max_echo_size(void) const;

    const char* product_type(void) const;
    const char* firmware_version(void) const;
    const char* serial_id(void) const;
    const char* status(void) const;
    const char* state(void) const;

    int raw_write(const char* data, size_t data_size);
    int raw_read(char* data, size_t max_data_size, int timeout);
    int raw_readline(char* data, size_t max_data_size, int timeout);

private:
    struct pImpl;
    std::auto_ptr<pImpl> pimpl;
};

#endif /* !DEPTH_2D_DRIVER_H */
