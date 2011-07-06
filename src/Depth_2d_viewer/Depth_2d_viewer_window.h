#ifndef DEPTH_2D_VIEWER_WINDOW_H
#define DEPTH_2D_VIEWER_WINDOW_H

/*!
  \file
  \brief Kinect センサの X-Y 平面データの 2D 表示

  \author Satofumi KAMIMURA

  $Id$
*/

#include "ui_Depth_2d_viewer_window_form.h"
#include <QMainWindow>
#include <memory>


class Depth_2d_viewer_window
    : public QMainWindow, private Ui::Depth_2d_viewer_window_form
{
    Q_OBJECT;

public:
    Depth_2d_viewer_window(void);
    ~Depth_2d_viewer_window(void);

private slots:
    void run_button_clicked(void);

private:
    struct pImpl;
    std::auto_ptr<pImpl> pimpl;
};


#endif /* !DEPTH_2D_VIEWER_WINDOW_H */
