#ifndef LENGTH_DRAW_WIDGET_H
#define LENGTH_DRAW_WIDGET_H

/*!
  \file
  \brief 距離データの表示

  \author Satofumi KAMIMURA

  $Id$
*/

#include <QGLWidget>
#include <vector>
#include <memory>

namespace qrk
{
    class Lidar;
}


class Length_draw_widget : public QGLWidget
{
    Q_OBJECT;

public:
    Length_draw_widget(qrk::Lidar& lidar, QWidget* parent = NULL);
    ~Length_draw_widget(void);

    void reset_form(void);
    void start_measurement(void);
    void stop_measurement(void);

    void update_zoom_ratio(int steps);
    void set_zoom_ratio(double pixel_per_mm);
    double zoom_ratio(void);

    void set_intensity_only(bool is_intensity_only);
    void set_max_value_if_error(bool active);

    void set_step_line(bool active, double radian);

    void set_draw_data(const long* data, int data_size,
                       long timestamp, const QColor& color,
                       bool is_intensity = false);

    //! 強度データの登録用
    void set_draw_data(const unsigned short* data, int data_size,
                       long timestamp, const QColor& color);
    void redraw(void);


public slots:
    void set_scan_data(const long* length, int length_size,
                       const unsigned short* intensity, int intensity_size,
                       long timestamp);


protected:
    void initializeGL(void);
    void resizeGL(int width, int height);
    void paintGL(void);

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);


signals:
    void clicked_position(bool active, long x_mm, long y_mm);
    void left_clicked_position(bool active, long x_mm, long y_mm);


private:
    Length_draw_widget(const Length_draw_widget& rhs);
    Length_draw_widget& operator = (const Length_draw_widget& rhs);

    struct pImpl;
    std::auto_ptr<pImpl> pimpl;
};

#endif /* !LENGTH_DRAW_WIDGET_H */
