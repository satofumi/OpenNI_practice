/*!
  \file
  \brief Kinect センサの X-Y 平面データの 2D 表示

  \author Satofumi KAMIMURA

  $Id$
*/

#include "Depth_2d_viewer_window.h"
#include "Length_draw_widget.h"
#include "Length_value_view_widget.h"
#include "Depth_2d_driver.h"
#include <QSettings>


namespace
{
    enum {
        Minimum_width = 100,
        Minimum_height = 100,
    };


    const char* Organization_ = "Hyakure-soft.";
    const char* Application_ = "Depth_2d_viewer";
}


struct Depth_2d_viewer_window::pImpl
{
    Depth_2d_viewer_window* widget_;
    Depth_2d_driver lidar_;
    Length_draw_widget length_draw_widget_;
    Length_value_view_widget length_value_view_widget_;


    pImpl(Depth_2d_viewer_window* widget)
        : widget_(widget), length_draw_widget_(lidar_)
    {
    }


    void initialize_form(void)
    {
        // Widget の配置
        enum { Maximum_width = 150, };
        length_value_view_widget_.setMaximumWidth(Maximum_width);

        length_draw_widget_.
            setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        length_draw_widget_.setMinimumSize(Minimum_width, Minimum_height);

        widget_->main_layout_->addWidget(&length_value_view_widget_);
        widget_->main_layout_->addWidget(&length_draw_widget_);

        // メニュー設定
        connect(widget_->action_quit_, SIGNAL(triggered()),
                widget_, SLOT(close()));
    }


    void load_settings(void)
    {
        QSettings settings(Organization_, Application_);

        // サイズ設定
        widget_->restoreGeometry(settings.value("geometry").toByteArray());
    }


    void save_settings(void)
    {
        QSettings settings(Organization_, Application_);

        // サイズ設定
        settings.setValue("geometry", widget_->saveGeometry());
    }
};


Depth_2d_viewer_window::Depth_2d_viewer_window(void)
    : pimpl(new pImpl(this))
{
    setupUi(this);
    pimpl->initialize_form();
    pimpl->load_settings();
}


Depth_2d_viewer_window::~Depth_2d_viewer_window(void)
{
    pimpl->save_settings();
}
