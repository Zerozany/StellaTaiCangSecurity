#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    std::invoke(&Widget::init_window, this);
    std::invoke(&Widget::ui_layout, this);
    std::invoke(&Widget::signals_connect_slots, this);
    std::invoke(static_cast<void(QTimer::*)(int)>(&QTimer::start), async_recv_timer, 500);
}

void Widget::signals_connect_slots()
{
    /// @brief 绑定客户端连接至服务器按钮
    connect(connect_server_button, &QPushButton::clicked, this, &Widget::connect_server_slot);

    /// @brief 获取图像数据按钮
    connect(get_image_button, &QPushButton::clicked, this, &Widget::get_data_slot);

    /// @brief 绑定发送数据按钮
    connect(send_button, &QPushButton::clicked, this, &Widget::send_data_slot);

    /// @brief 绑定撤回按钮
    connect(revoke_button, &QPushButton::clicked, this, &Widget::__draw_invoke_slot);

    /// @brief 删除车道按钮
    connect(delete_line_button, &QPushButton::clicked, this, &Widget::delete_lane_slot);

    /// @brief 保存车道按钮
    connect(save_lane_button, &QPushButton::clicked, this, &Widget::save_lane_slot);

    /// @brief 车道ID 关联
    connect(lanes_id_combox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Widget::lane_and_auto_slot);

    /// @brief 截面ID 关联
    connect(section_sec_combox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Widget::section_and_auto_slot);

    /// @brief 删除截面所选线
    connect(delete_section_button, &QPushButton::clicked, this, &Widget::delete_section_slot);

    /// @brief 保存截面
    connect(save_section_button, &QPushButton::clicked, this, &Widget::save_section_slot);

    /// @brief 开启异步监听请求线程
    connect(async_recv_timer, &QTimer::timeout, this, &Widget::async_timer_slot);

    /// @brief 监听信息
    connect(&client.m_client_socket, &QTcpSocket::readyRead, [this]() {
        client.recv_request();
    });
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    std::invoke(&Widget::save_line, this , event, client.m_area);
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    /// @brief 鼠标点击事件跟踪
    if (!recv_image.isNull())
    {
        std::invoke(&Widget::save_points, this, event);
    }

    /// @brief 文本框焦点跟踪
    if (event->button() == Qt::LeftButton && event->pos() != area_number_edit->pos())
    {
        area_number_edit->clearFocus();
    }

    /// @brief 文本框焦点跟踪
    if (event->button() == Qt::LeftButton && event->pos() != load_number_edit->pos())
    {
        load_number_edit->clearFocus();
    }

    /// @brief 文本框焦点跟踪
    if (event->button() == Qt::LeftButton && event->pos() != ip_edit->pos())
    {
        ip_edit->clearFocus();
    }
}

void Widget::paintEvent(QPaintEvent *event)
{
    /// @brief 绘图事件
    if (!recv_image.isNull())
    {
        std::invoke(&Widget::draw_image, this, client.m_area);
    }
}

void Widget::resizeEvent(QResizeEvent *event)
{
    /// @brief 实时检测窗口分辨率变化
    if (!recv_image.isNull())
    {
        scaled_image = recv_image.scaled(image_win_label->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        image_win_label->setPixmap(QPixmap::fromImage(scaled_image));
    }
    QWidget::resizeEvent(event);
}

bool Widget::eventFilter(QObject *obj, QEvent *event)
{
    /// @brief 鼠标样式跟踪
    if (obj == image_win_label && event->type() == QEvent::Enter)
    {
        if (!recv_image.isNull())
        {
            this->setCursor(Qt::CrossCursor);
            image_win_label->setFocus();
        }
        return true;
    }
    else if (obj == image_win_label && event->type() == QEvent::Leave)
    {
        this->setCursor(Qt::ArrowCursor);
        image_win_label->clearFocus();
        return true;
    }
    return QObject::eventFilter(obj, event);
}
