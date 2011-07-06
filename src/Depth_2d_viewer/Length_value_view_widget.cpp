/*!
  \file
  \brief 距離データの値を表示

  \author Satofumi KAMIMURA

  $Id$

  \todo 距離データの更新、ボタンがタブを切替えると押せるようになるのを修正する。
*/

#include "Length_value_view_widget.h"
#include "Lidar.h"


namespace
{
    enum {
        Default_column_size = 768 + 1,
        Default_minimum_length = 23,
        Default_maximum_length = 60000,
    };


    typedef enum {
        Length,
        Intensity,
    } value_type_t;
}


struct Length_value_view_widget::pImpl
{
    Length_value_view_widget* widget_;
    bool data_updated_;
    long minimum_length_;
    long maximum_length_;

    int previous_select_index_;


    pImpl(Length_value_view_widget* widget)
        : widget_(widget), data_updated_(false),
          minimum_length_(Default_minimum_length),
          maximum_length_(Default_maximum_length),
          previous_select_index_(0)
    {
    }


    void initialize_form(void)
    {
        enum { Horizontal_header_size = 2, };
        for (int i = 0; i < Horizontal_header_size; ++i) {
            widget_->table_->horizontalHeader()->
                setResizeMode(i, QHeaderView::Stretch);
        }
        widget_->table_->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

        // !!! set???Enable() を作ったときに、この呼び出しも含めること
        widget_->update_button_->setEnabled(true);

        connect(widget_->update_button_, SIGNAL(clicked()),
                widget_, SLOT(update_button_clicked()));
        connect(widget_->table_, SIGNAL(itemSelectionChanged()),
                widget_, SLOT(current_cell_changed_slot()));
    }


    void reset_form(void)
    {
        set_row_size(Default_column_size);
        minimum_length_ = Default_minimum_length;
        buttons_set_enabled(false);
    }


    void set_length(QTableWidgetItem* item, long length,
                    value_type_t type = Length)
    {
        QString value = QString("%1").arg(length);

        if (type == Length) {
            if (length < minimum_length_) {
                // 距離値がエラーのときは赤色で表示する
                item->setForeground(QBrush(Qt::red));
            } else {
                item->setForeground(QBrush(Qt::black));
            }
        }

        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        item->setText(value);
    }


    void set_row_size(size_t row_size)
    {
        QTableWidget* p = widget_->table_;
        p->clearContents();
        p->setRowCount(row_size);

        QStringList row_labels;
        QString zero_value = "0";
        for (size_t i = 0; i < row_size; ++i) {
            row_labels << QString("%1").arg(i);

            // 距離
            QTableWidgetItem* item = new QTableWidgetItem;
            set_length(item, 0);
            p->setItem(i, 0, item);

            // 強度
            item = new QTableWidgetItem;
            set_length(item, 0, Intensity);
            p->setItem(i, 1, item);
        }
        p->setVerticalHeaderLabels(row_labels);
    }


    void set_table_zero(void)
    {
        size_t row_size = widget_->table_->rowCount();
        QString zero_value = "0";
        for (size_t i = 0; i < row_size; ++i) {
            QTableWidgetItem* item = widget_->table_->item(i, 0);
            set_length(item, 0);

            item = widget_->table_->item(i, 1);
            set_length(item, 0, Intensity);
        }
    }


    void set_length_range(long minimum_length, long maximum_length)
    {
        minimum_length_ = minimum_length;
        maximum_length_ = maximum_length;
    }


    void buttons_set_enabled(bool enable)
    {
        widget_->update_button_->setEnabled(enable);
    }
};


Length_value_view_widget::Length_value_view_widget(QWidget* parent)
    : QWidget(parent), pimpl(new pImpl(this))
{
    setupUi(this);
    pimpl->initialize_form();
}


Length_value_view_widget::~Length_value_view_widget(void)
{
}


void Length_value_view_widget::reset_form(void)
{
    pimpl->reset_form();
}


void Length_value_view_widget::start_measurement(void)
{
    pimpl->buttons_set_enabled(true);
}


void Length_value_view_widget::stop_measurement(void)
{
    pimpl->buttons_set_enabled(false);
    pimpl->set_table_zero();
}


void Length_value_view_widget::set_sensor_parameter(qrk::Lidar& lidar)
{
    pimpl->set_row_size(lidar.max_data_size());
    pimpl->set_length_range(lidar.min_distance(), lidar.max_distance());
}


void Length_value_view_widget::update_view_data(void)
{
    pimpl->data_updated_ = false;
}


void Length_value_view_widget::focus_step_index(int index)
{
    // 選択中のセルを非選択にする
    QTableWidgetItem* previous_item =
        table_->item(pimpl->previous_select_index_, 0);
    if (previous_item) {
        previous_item->setSelected(false);
    }

    QTableWidgetItem* item = table_->item(index, 0);
    if (item) {
        item->setSelected(true);
        table_->scrollToItem(item);
        table_->setCurrentItem(item);
    }

    pimpl->previous_select_index_ = index;
}


void Length_value_view_widget::data_received(const long* length,
                                             int length_size,
                                             const unsigned short* intensity,
                                             int intensity_size,
                                             long timestamp)
{
    static_cast<void>(timestamp);

    if (pimpl->data_updated_) {
        return;
    }

    // 表示データの更新
    for (int i = 0; i < length_size; ++i) {
        QTableWidgetItem* item = table_->item(i, 0);
        if (!item) {
            break;
        }
        pimpl->set_length(item, length[i]);
    }

    for (int i = 0; i < intensity_size; ++i) {
        QTableWidgetItem* item = table_->item(i, 1);
        if (!item) {
            break;
        }
        pimpl->set_length(item, intensity[i], Intensity);
    }

    // データが登録されたら、ボタンを有効にする
    pimpl->data_updated_ = true;
    update_button_->setEnabled(true);
}


void Length_value_view_widget::update_button_clicked(void)
{
    // Length_receive_thread に対してデータの配置を要求する
    update_button_->setEnabled(false);
    pimpl->data_updated_ = false;
}


void Length_value_view_widget::current_cell_changed_slot(void)
{
    QTableWidgetItem* item = table_->currentItem();
    if (item) {
        int index = item->row();
        emit current_cell_changed(true, index);
    } else {
        emit current_cell_changed(false, 0);
    }
}
