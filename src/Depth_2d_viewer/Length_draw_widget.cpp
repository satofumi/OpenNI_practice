/*!
  \file
  \brief 距離データの表示

  \author Satofumi KAMIMURA

  $Id$
*/

#include "Length_draw_widget.h"
#include "math_utilities.h"
#include "Lidar.h"
#include <QVector2D>
#include <QWheelEvent>
#include <deque>
#include <algorithm>
#include <limits>

using namespace qrk;
using namespace std;


namespace
{
    typedef vector<QPoint> Points;


    class Draw_data
    {
    public:
        Points points;
        QColor color;
        long timestamp;


        Draw_data(void) : timestamp(0)
        {
        }
    };
}


struct Length_draw_widget::pImpl
{
    enum {
        PointLength = 30 * 1000,     // [mm]
        Default_draw_period = 100,   // [msec]
    };


    static const double Default_pixel_per_mm = 20.0;


    Length_draw_widget* widget_;
    Lidar& lidar_;
    QColor clear_color_;
    double pixel_per_mm_;
    size_t width_;
    size_t height_;
    QPoint view_center_;

    QColor current_color_;
    deque<Draw_data*> draw_data_;
    long last_timestamp_;
    long draw_period_;

    bool is_scanning_;
    long last_redraw_;
    QPoint clicked_position_;
    bool now_pressed_;

    bool is_step_line_active_;
    double step_line_radian_;

    bool is_intensity_only_;
    bool is_max_value_if_error_;


    pImpl(Lidar& lidar, Length_draw_widget* widget)
        : widget_(widget), lidar_(lidar),
          clear_color_(Qt::white), pixel_per_mm_(Default_pixel_per_mm),
          width_(1), height_(1), last_timestamp_(0),
          draw_period_(Default_draw_period), is_scanning_(false),
          last_redraw_(0), now_pressed_(false),
          is_step_line_active_(false), step_line_radian_(0.0),
          is_intensity_only_(false), is_max_value_if_error_(false)
    {
    }


    ~pImpl(void)
    {
        remove_data();
    }


    void initialize_form(void)
    {
        // !!!

        reset_form();
    }


    void reset_form(void)
    {
        // !!!
    }


    void draw_axis(void)
    {
        double ratio = zoom_ratio();
        QVector2D offset = point_offset(ratio);

        // 補助線の描画
        draw_sub_axis(offset);

        glColor3d(0.3, 0.3, 0.3);
        glBegin(GL_LINES);

        // Y 軸
        glVertex2d(-offset.x(), -1.0);
        glVertex2d(-offset.x(), +1.0);

        // X 軸
        double aspect = 1.0 * width_ / height_;
        glVertex2d(-aspect, -offset.y());
        glVertex2d(+aspect, -offset.y());

        // 背後 45 [deg] の補助線
        glVertex2d(-offset.x(), -offset.y());
        glVertex2d((-PointLength * ratio) - offset.x(),
                   (-PointLength * ratio) - offset.y());
        glVertex2d(-offset.x(), -offset.y());
        glVertex2d((+PointLength * ratio) - offset.x(),
                   (-PointLength * ratio) - offset.y());
        glEnd();

        // 円の補助線を描画
        for (int i = 0; i <= PointLength; i += 1000) {
            draw_sub_circle(i, offset);
        }

        // 単位の表示
        // !!!
    }


    QPoint draw_offset(double zoom_ratio)
    {
        return QPoint(-view_center_.x() * zoom_ratio,
                      -view_center_.y() * zoom_ratio);
    }


    // [mm] -> [pixel]
    double zoom_ratio(void)
    {
        return (1.0 / pixel_per_mm_ / (height_ / 2.0));
    }


    QVector2D point_offset(double ratio)
    {
        return QVector2D(-view_center_.x() * ratio,
                         -view_center_.y() * ratio);
    }


    void draw_sub_circle(int radius, const QVector2D& offset)
    {
        double pixel_radius = (radius / pixel_per_mm_) / (height_ / 2.0);

        glBegin(GL_LINE_STRIP);
        for (int theta = -45; theta <= (180 + 45); theta += 2) {

            double radian = M_PI * theta / 180.0;
            double x = pixel_radius * cos(radian);
            double y = pixel_radius * sin(radian);
            glVertex2d(x - offset.x(), y - offset.y());
        }
        glEnd();
    }


    void draw_sub_axis(const QVector2D& offset)
    {
        QVector2D center(width_ / 2, height_ / 2);
        QVector2D first(static_cast<int>((-center.x() * pixel_per_mm_)
                                         - view_center_.x()),
                        static_cast<int>((-center.y() * pixel_per_mm_)
                                         - view_center_.y()));
        QVector2D last(static_cast<int>((+center.x() * pixel_per_mm_)
                                        - view_center_.x()),
                       static_cast<int>((+center.y() * pixel_per_mm_)
                                        - view_center_.y()));

        const double threshold[] = { 4.0, 16.0, 32.0 };
        const int interval[] = { 10, 100, 1000 };
        const double color[] = { 0.8, 0.6, 0.3 };
        size_t n = sizeof(threshold) / sizeof(threshold[0]);
        for (size_t i = 0; i < n; ++i) {
            const double draw_threshold = threshold[i];
            if (pixel_per_mm_ > draw_threshold) {
                continue;
            }
            double alpha = 1.0 - (pixel_per_mm_ / draw_threshold);
            glColor4d(color[i], color[i], color[i], alpha);
            draw_sub_point_line(interval[i], offset, first, last);
        }
    }


    void draw_sub_point_line(int interval, const QVector2D& offset,
                             const QVector2D& first, const QVector2D& last)
    {
        glBegin(GL_LINES);
        for (int x = (first.x() / interval) - 1;
             x < ((last.x() / interval) + 1); ++x) {
            double draw_x = (interval * x / pixel_per_mm_) / (height_ / 2.0);
            glVertex2d(draw_x - offset.x(), -1.0);
            glVertex2d(draw_x - offset.x(), +1.0);
        }

        double aspect = 1.0 * width_ / height_;
        for (int y = (first.y() / interval) - 1;
             y < ((last.y() / interval) + 1); ++y) {
            double draw_y = (interval * y / pixel_per_mm_) / (height_ / 2.0);
            glVertex2d(-aspect, draw_y - offset.y());
            glVertex2d(+aspect, draw_y - offset.y());
        }
        glEnd();
    }


    void update_zoom_ratio(int steps)
    {
        double zoom = widget_->zoom_ratio();
        zoom *= pow(1.1, steps);

        if (zoom > 500.0) {
            zoom = 500.0;
        } else if (zoom < 0.05) {
            zoom = 0.05;
        }
        widget_->set_zoom_ratio(zoom);
    }


    void set_clicked_position(QMouseEvent* event)
    {
        clicked_position_ = event->pos();
    }


    void calculate_clicked_position(long& x_mm, long& y_mm, long x, long y)
    {
        int center_x = width_ / 2;
        int center_y = height_ / 2;

        x_mm = static_cast<int>(((x - center_x) * pixel_per_mm_)
                                - view_center_.x());
        y_mm = static_cast<int>((-(y - center_y) * pixel_per_mm_)
                                - view_center_.y());
    }


    void draw_data(void)
    {
#if 0
        // !!! タイムスタンプのローテーションに対応できていないので、
        // !!! １つ残して全てのデータを削除するようにする

        // 一定時間以上経過したデータを削除
        size_t data_size = draw_data_.size();
        for (size_t i = 0; i < data_size; ++i) {
            long diff = last_timestamp_ - draw_data_[0]->timestamp;
            if (diff < draw_period_) {
                break;
            }

            remove_front_data();
        }
#else
        // 一定時間以上経過したデータを削除
        // 強度と距離の２つのみ残す
        int remove_size = draw_data_.size() - 2;
        for (int i = 0; i < remove_size; ++i) {
            remove_front_data();
        }
#endif

        // !!! 時間が経過するほど、薄い色で表示するようにする

        double ratio = zoom_ratio();
        QVector2D offset = point_offset(ratio);

        // 注視するステップ位置に線を描画
        if (is_step_line_active_) {
            double radian = step_line_radian_ + (M_PI / 2.0);
            double x = 100000 * cos(radian);
            double y = 100000 * sin(radian);

            glColor4d(0.0, 1.0, 0.0, 0.9);

            glBegin(GL_LINES);
            glVertex2d(-offset.x(), -offset.y());
            glVertex2d(x - offset.x(), y - offset.y());
            glEnd();
        }

        // 中心から測定点への直線を描画
        glBegin(GL_LINES);
        for (deque<Draw_data*>::const_iterator scan_it = draw_data_.begin();
             scan_it != draw_data_.end(); ++scan_it) {

            QColor& color = (*scan_it)->color;
#if 0
            double diff = last_timestamp_ - (*scan_it)->timestamp;
            double alpha = (1.0 - (diff / draw_period_)) * 0.6;
#else
            double alpha = color.alpha() / 255.0;
#endif
            glColor4d(color.red() / 255.0,
                      color.green() / 255.0,
                      color.blue() / 255.0, alpha);

            vector<QPoint>& scan_data = (*scan_it)->points;
            vector<QPoint>::const_iterator end_it = scan_data.end();
            for (vector<QPoint>::const_iterator it = scan_data.begin();
                 it != end_it; ++it) {

                double x = it->x() * ratio;
                double y = it->y() * ratio;
                glVertex2d(-offset.x(), -offset.y());
                glVertex2d(x - offset.x(), y - offset.y());
            }
        }
        glEnd();

#if 0
        // 測定点の描画
        double base_size = 1.4;
        double mm_pixel = max(base_size / pixel_per_mm_, base_size);
        glPointSize(mm_pixel);

        glBegin(GL_POINTS);
        for (DataArray::iterator line_it = draw_data_.begin();
             line_it != draw_data_.end(); ++line_it) {

            // !!! 関数にする
            double diff = last_timestamp_ - line_it->timestamp;
            double alpha = 1.0 - (diff / draw_period_);
            glColor4d(1.0, 0.0, 0.0, alpha);

            vector<QPoint>::iterator end_it = line_it->point_data.end();
            for (vector<QPoint>::iterator it =
                     line_it->point_data.begin();
                 it != end_it; ++it) {

                double x = it->x * ratio;
                double y = it->y * ratio;
                glVertex2d(x - offset.x, y - offset.y);
            }
        }
        glEnd();
#endif
    }


    void remove_front_data(void)
    {
        if (draw_data_.empty()) {
            return;
        }

        delete draw_data_.front();
        draw_data_.pop_front();
    }


    void remove_data(void)
    {
        while (!draw_data_.empty()) {
            remove_front_data();
        }
    }
};


Length_draw_widget::Length_draw_widget(qrk::Lidar& lidar, QWidget* parent)
    : QGLWidget(parent), pimpl(new pImpl(lidar, this))
{
}


Length_draw_widget::~Length_draw_widget(void)
{
}


void Length_draw_widget::reset_form(void)
{
    pimpl->reset_form();
}


void Length_draw_widget::start_measurement(void)
{
    // !!!

    pimpl->is_scanning_ = true;
}


void Length_draw_widget::stop_measurement(void)
{
    // !!!

    // !!! データの削除とクリア
    pimpl->is_scanning_ = false;
    pimpl->remove_data();
    redraw();
}


void Length_draw_widget::initializeGL(void)
{
    qglClearColor(pimpl->clear_color_);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void Length_draw_widget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    double aspect = 1.0 * width / height;
    glOrtho(-1.0 * aspect, +1.0 * aspect, -1.0, +1.0,
            numeric_limits<int>::min(), numeric_limits<int>::max());

    glMatrixMode(GL_MODELVIEW);
    pimpl->width_ = width;
    pimpl->height_ = height;
}


void Length_draw_widget::paintGL(void)
{
    qglClearColor(pimpl->clear_color_);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 軸の描画
    pimpl->draw_axis();

    // データの描画
    pimpl->draw_data();
}


void Length_draw_widget::update_zoom_ratio(int steps)
{
    pimpl->update_zoom_ratio(steps);
}


void Length_draw_widget::set_intensity_only(bool is_intensity_only)
{
    pimpl->is_intensity_only_ = is_intensity_only;
}


void Length_draw_widget::set_max_value_if_error(bool active)
{
    pimpl->is_max_value_if_error_ = active;
}


void Length_draw_widget::set_zoom_ratio(double pixel_per_mm)
{
    pimpl->pixel_per_mm_ = pixel_per_mm;
    redraw();
}


double Length_draw_widget::zoom_ratio(void)
{
    return pimpl->pixel_per_mm_;
}


void Length_draw_widget::set_step_line(bool active, double radian)
{
    pimpl->is_step_line_active_ = active;
    if (active) {
        pimpl->step_line_radian_ = radian;
    }
}


void Length_draw_widget::set_draw_data(const long* data, int data_size,
                                       long timestamp, const QColor& color,
                                       bool is_intensity)
{
    Draw_data* draw_data = new Draw_data();
    draw_data->color = color;
    draw_data->timestamp = timestamp;
    pimpl->last_timestamp_ = timestamp;

    long max_distance = (is_intensity) ? 0xffff : pimpl->lidar_.max_distance();
    long min_distance = (is_intensity) ? 0 : pimpl->lidar_.min_distance();

    for (int i = 0; i < data_size; ++i) {
        long l = data[i];

        if (!is_intensity && pimpl->is_max_value_if_error_) {
            // エラー時の距離データを最大値にする
            if (l <= min_distance) {
                l = max_distance;
            }
        }

        double radian = pimpl->lidar_.index2rad(i) + (M_PI / 2.0);
        long x = static_cast<long>(l * cos(radian));
        long y = static_cast<long>(l * sin(radian));

        draw_data->points.push_back(QPoint(x, y));
    }

    pimpl->draw_data_.push_back(draw_data);
}


void Length_draw_widget::set_draw_data(const unsigned short* data,
                                       int data_size, long timestamp,
                                       const QColor& color)
{
    if (data_size <= 0) {
        return;
    }

    // !!! どうにかする
    //std::copy(&data[0], &data[data_size - 1], long_data.begin());
    vector<long> long_data;
    for (int i = 0; i < data_size; ++i) {
        long_data.push_back(data[i]);
    }
    set_draw_data(&long_data[0], data_size, timestamp, color, true);
}



void Length_draw_widget::redraw(void)
{
    //pimpl->last_redraw_ = ticks();
    updateGL();
}


void Length_draw_widget::set_scan_data(const long* length, int length_size,
                                       const unsigned short* intensity,
                                       int intensity_size, long timestamp)
{
    if (!pimpl->is_scanning_) {
        return;
    }

    // 強度データの登録
    if (intensity_size) {
        // !!! 色は const で定義する
        set_draw_data(intensity, intensity_size,
                      timestamp, QColor(255, 64, 255, 48));
    }

    if (!pimpl->is_intensity_only_) {
        // 距離データの登録
        if (length_size > 0) {
            // !!! 色は const で定義する
            set_draw_data(length, length_size, timestamp,
                          QColor(0, 0, 255, 80));
        }
    }

    // 描画
    redraw();
}


void Length_draw_widget::mousePressEvent(QMouseEvent* event)
{
    pimpl->now_pressed_ = true;

    pimpl->set_clicked_position(event);
    if (event->button() == Qt::LeftButton) {
        long x_mm;
        long y_mm;
        pimpl->calculate_clicked_position(x_mm, y_mm, event->x(), event->y());
        emit left_clicked_position(true, x_mm, y_mm);
    }
}


void Length_draw_widget::mouseMoveEvent(QMouseEvent* event)
{
    int x = event->x();
    int y = event->y();

    if (pimpl->now_pressed_) {
        int dx = x - pimpl->clicked_position_.x();
        int dy = y - pimpl->clicked_position_.y();

        pimpl->view_center_.setX(pimpl->view_center_.x() +
                                 (dx * pimpl->pixel_per_mm_));
        pimpl->view_center_.setY(pimpl->view_center_.y() -
                                 (dy * pimpl->pixel_per_mm_));

        pimpl->set_clicked_position(event);

#if 0
        // !!! ticks() を用意したら有効にする

        // 最後の更新が MinimumRedrawInterval [msec] 以前だったなら、
        // 再描画を行う
        enum { MinimumRedrawInterval = 25 };
        if ((pimpl->last_redraw_ + MinimumRedrawInterval) < ticks()) {
            redraw();
        }
#else
        redraw();
#endif
    }

    // カーソル位置の座標をシグナルで送信する
    if ((x < 0) || (x >= static_cast<int>(pimpl->width_)) ||
        (y < 0) || (y >= static_cast<int>(pimpl->height_))) {
        emit clicked_position(false, -1, -1);
        return;
    }

    long x_mm;
    long y_mm;
    pimpl->calculate_clicked_position(x_mm, y_mm, x, y);
    emit clicked_position(true, x_mm, y_mm);
}


void Length_draw_widget::mouseReleaseEvent(QMouseEvent* event)
{
    static_cast<void>(event);

    pimpl->now_pressed_ = false;
    redraw();
}


void Length_draw_widget::wheelEvent(QWheelEvent* event)
{
    int degrees = event->delta() / 8;
    int steps = degrees / 15;

    event->accept();
    update_zoom_ratio(steps);
}
