#ifndef LENVTH_VALUE_VIEW_WIDGET_H
#define LENVTH_VALUE_VIEW_WIDGET_H

/*!
  \file
  \brief 距離データの値を表示

  \author Satofumi KAMIMURA

  $Id$
*/

#include "ui_Length_value_view_widget_form.h"
#include <memory>

namespace qrk
{
    class Lidar;
}


class Length_value_view_widget
    : public QWidget, private Ui::Length_value_view_widget_form
{
    Q_OBJECT;

public:
    Length_value_view_widget(QWidget* parent = NULL);
    ~Length_value_view_widget(void);

    void reset_form(void);
    void start_measurement(void);
    void stop_measurement(void);

    void set_sensor_parameter(qrk::Lidar& lidar);
    void update_view_data(void);

    void focus_step_index(int index);


signals:
    void current_cell_changed(bool active, int current_index);


public slots:
    void data_received(const long* length, int length_size,
                       const unsigned short* intensity, int intensity_size,
                       long timestamp);


private slots:
    void update_button_clicked(void);
    void current_cell_changed_slot(void);


private:
    Length_value_view_widget(const Length_value_view_widget& rhs);
    Length_value_view_widget& operator = (const Length_value_view_widget& rhs);

    struct pImpl;
    std::auto_ptr<pImpl> pimpl;
};

#endif /* !LENVTH_VALUE_VIEW_WIDGET_H */
