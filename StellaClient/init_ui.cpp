#include "widget.h"

constexpr static uint8_t ContentsMargins{1};
constexpr static uint32_t WINDOW_WIDTH{1150};
constexpr static uint32_t WINDOW_HEIGHT{750};

void Widget::init_window()
{
    this->setContentsMargins(ContentsMargins, ContentsMargins, ContentsMargins, ContentsMargins);
    this->setLayoutDirection(Qt::LeftToRight);
    this->setMinimumSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    this->setWindowState(Qt::WindowNoState);
    this->setWindowTitle("@StellaTaiCany-Client");
    this->setWindowIcon(QIcon(":/image/Pictogrammers-Material-Road-variant.128.png"));
    this->setMouseTracking(true);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setStyleSheet("font-family:'微软雅黑';");
    std::invoke(&Widget::hideOshow, this , false);
}

void Widget::client_layout()
{
    /// @brief 客户端布局
    QHBoxLayout *client_window_groupbox_hlayout{new QHBoxLayout(client_window_groupbox)};
    QVBoxLayout *label_client_vlayout{new QVBoxLayout()};
    QVBoxLayout *other_client_vlayout{new QVBoxLayout()};

    QLabel *connect_title_label{new QLabel(this)};
    QLabel *get_image_title_label{new QLabel(this)};
    QLabel *ip_title_label{new QLabel(this)};

    connect_server_button->setMinimumHeight(35);
    connect_server_button->setMinimumWidth(120);
    get_image_button->setMinimumHeight(35);
    get_image_button->setMinimumWidth(120);
    connect_server_button->setText("连接");
    get_image_button->setText("获取图像");
    connect_server_button->setIcon(QIcon(":/image/Bootstrap-Bootstrap-Bootstrap-wifi.24.png"));
    get_image_button->setIcon(QIcon(":/image/Microsoft-Fluentui-Emoji-Mono-Open-File-Folder.32.png"));
    connect_title_label->setPixmap(QPixmap(":/image/Fatcow-Farm-Fresh-Chart-pie-title.24.png"));
    get_image_title_label->setPixmap(QPixmap(":/image/Fatcow-Farm-Fresh-Chart-pie-title.24.png"));
    ip_title_label->setPixmap(QPixmap(":/image/Fatcow-Farm-Fresh-Chart-pie-title.24.png"));
    ip_edit->setStyleSheet("font-size:15px;border:none;");
    ip_edit->setPlaceholderText("ip地址");

    label_client_vlayout->addWidget(ip_title_label);
    label_client_vlayout->addWidget(connect_title_label);
    label_client_vlayout->addWidget(get_image_title_label);

    other_client_vlayout->addWidget(ip_edit);
    other_client_vlayout->addWidget(connect_server_button);
    other_client_vlayout->addWidget(get_image_button);

    client_window_groupbox_hlayout->addLayout(label_client_vlayout, 2);
    client_window_groupbox_hlayout->addLayout(other_client_vlayout, 1);
}

void Widget::stella_layout()
{
    QHBoxLayout *stella_groupbox_vlayout{new QHBoxLayout(stella_info_groupbox)};
    QLabel *stella_icon{new QLabel(this)};
    stella_icon->setPixmap(QPixmap(":/image/StellaLogo.png"));
    stella_groupbox_vlayout->addWidget(stella_icon);
}

void Widget::config_params_layout()
{
    /// @brief 配置参数布局
    QVBoxLayout *params_table_groupbox_vlayout{new QVBoxLayout(config_params_groupbox)};  // 改为 QVBoxLayout
    params_table->setFont(QFont("微软雅黑", 10));
    params_table->setColumnCount(2);
    params_table->setHorizontalHeaderLabels(QStringList() << "名称" << "数值");
    params_table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    params_table->resizeColumnsToContents();
    params_table->verticalHeader()->setHidden(true);
    params_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    params_table->horizontalHeader()->setVisible(true);
    params_table_groupbox_vlayout->addWidget(params_table);
}

void Widget::load_info_layout()
{
    /// @brief 路况信息布局
    QVBoxLayout *load_info_groupbox_vlayout{new QVBoxLayout(load_info_groupbox)};
    QHBoxLayout *load_info_hlayout{new QHBoxLayout()};
    QVBoxLayout *label_load_vlayout{new QVBoxLayout()};
    QVBoxLayout *text_load_vlayout{new QVBoxLayout()};
    QVBoxLayout *other_load_vlayout{new QVBoxLayout()};
    QHBoxLayout *button_load_hlayout{new QHBoxLayout()};

    QLabel *intersection_type_icon_label{new QLabel(this)};
    QLabel *area_number_icon_label{new QLabel(this)};
    QLabel *load_number_icon_label{new QLabel(this)};
    QLabel *area_directionr_icon_label{new QLabel(this)};

    QLabel *intersection_type_tip_label{new QLabel(this)};
    QLabel *area_number_tip_label{new QLabel(this)};
    QLabel *load_number_tip_label{new QLabel(this)};
    QLabel *area_direction_tip_label{new QLabel(this)};

    revoke_button->setMinimumHeight(35);
    send_button->setMinimumHeight(35);
    send_button->setMinimumWidth(120);
    revoke_button->setMinimumWidth(120);
    send_button->setText("发送");
    revoke_button->setText("撤回");
    revoke_button->setToolTip("撤回(r)");
    revoke_button->setShortcut(tr("r", "R"));

    intersection_type_icon_label->setPixmap(QPixmap(":/image/Bootstrap-Bootstrap-Bootstrap-sign-intersection.24.png"));
    area_number_icon_label->setPixmap(QPixmap(":/image/Simpleicons-Team-Simple-N26.24.png"));
    load_number_icon_label->setPixmap(QPixmap(":/image/Bootstrap-Bootstrap-Bootstrap-arrow-down-up.24.png"));
    area_directionr_icon_label->setPixmap(QPixmap(":/image/Icons8-Ios7-Maps-Center-Direction.24.png"));
    send_button->setIcon(QIcon(":/image/Iconoir-Team-Iconoir-Send.32.png"));
    revoke_button->setIcon(QIcon(":/image/Amitjakhu-Drip-Return.32.png"));

    intersection_type_tip_label->setStyleSheet("font-size:14px;");
    area_number_tip_label->setStyleSheet("font-size:14px;");
    load_number_tip_label->setStyleSheet("font-size:14px;");
    area_direction_tip_label->setStyleSheet("font-size:14px;");
    intersection_type_combox->setStyleSheet("font-size:14px; height:30px;");
    area_direction_combox->setStyleSheet("font-size:14px; height:30px;");
    area_number_edit->setStyleSheet("font-size:15px;border:none;");
    load_number_edit->setStyleSheet("font-size:15px;border:none;");

    intersection_type_tip_label->setText("出入口");
    area_number_tip_label->setText("区域编号");
    load_number_tip_label->setText("路口编号");
    area_direction_tip_label->setText("区域方向");

    intersection_type_combox->addItem(" 进口");
    intersection_type_combox->addItem(" 出口");
    intersection_type_combox->addItem(" 未知");
    area_direction_combox->addItem(" 北");
    area_direction_combox->addItem(" 东");
    area_direction_combox->addItem(" 南");
    area_direction_combox->addItem(" 西");

    label_load_vlayout->addWidget(intersection_type_icon_label, Qt::AlignCenter);
    label_load_vlayout->addWidget(area_directionr_icon_label, Qt::AlignCenter);
    label_load_vlayout->addWidget(area_number_icon_label, Qt::AlignCenter);
    label_load_vlayout->addWidget(load_number_icon_label, Qt::AlignCenter);

    text_load_vlayout->addWidget(intersection_type_tip_label, Qt::AlignCenter);
    text_load_vlayout->addWidget(area_direction_tip_label, Qt::AlignCenter);
    text_load_vlayout->addWidget(area_number_tip_label, Qt::AlignCenter);
    text_load_vlayout->addWidget(load_number_tip_label, Qt::AlignCenter);

    other_load_vlayout->addWidget(intersection_type_combox, Qt::AlignCenter);
    other_load_vlayout->addWidget(area_direction_combox, Qt::AlignCenter);
    other_load_vlayout->addWidget(area_number_edit, Qt::AlignCenter);
    other_load_vlayout->addWidget(load_number_edit, Qt::AlignCenter);

    load_info_hlayout->addLayout(label_load_vlayout, Qt::AlignCenter);
    load_info_hlayout->addLayout(text_load_vlayout, Qt::AlignCenter);
    load_info_hlayout->addLayout(other_load_vlayout, Qt::AlignCenter);

    button_load_hlayout->addWidget(revoke_button, 1, Qt::AlignCenter);
    button_load_hlayout->addWidget(send_button, 1, Qt::AlignCenter);

    load_info_groupbox_vlayout->addLayout(load_info_hlayout, Qt::AlignCenter);
    load_info_groupbox_vlayout->addLayout(button_load_hlayout, Qt::AlignCenter);
}

void Widget::image_layout()
{
    /// @brief 绘制窗口布局
    image_win_label->setStyleSheet("background-color:black;");
    image_win_label->setAlignment(Qt::AlignCenter);
    image_win_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    image_win_label->installEventFilter(this);
}

void Widget::config_lanes_layout()
{
    QVBoxLayout *lanes_window_groupbox_hlayout{new QVBoxLayout(line_window_groupbox)};
    QHBoxLayout *label_and_combox_hvlayout{new QHBoxLayout()};
    QVBoxLayout *label_lane_vlayout{new QVBoxLayout()};
    QVBoxLayout *combox_lane_vlayout{new QVBoxLayout()};
    QHBoxLayout *button_lane_hlayout{new QHBoxLayout()};

    QLabel *select1_tip_label{new QLabel(this)};
    QLabel *select2_tip_label{new QLabel(this)};
    /*QLabel lane_id_tip_label{new QLabel(this)};*/
    QLabel *arrow_tip_label{new QLabel(this)};

    select1_tip_label->setText("线号(1)");
    select2_tip_label->setText("线号(2)");
    lane_id_tip_label->setText("车道ID");
    arrow_tip_label->setText("路口方向");

    select1_tip_label->setStyleSheet("font-size:14px;");
    select2_tip_label->setStyleSheet("font-size:14px;");
    lane_id_tip_label->setStyleSheet("font-size:14px;");
    arrow_tip_label->setStyleSheet("font-size:14px;");

    select_line_combox1->setStyleSheet("font-size:14px; height:30px;");
    select_line_combox2->setStyleSheet("font-size:14px; height:30px;");
    lanes_id_combox->setStyleSheet("font-size:14px; height:30px;");
    intersection_direction_combox->setStyleSheet("font-size:14px; height:30px;");
    select_line_combox1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    select_line_combox2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    lanes_id_combox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    intersection_direction_combox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    lanes_id_combox->addItem(" 未知");
    lanes_id_combox->addItem(" 1");
    lanes_id_combox->addItem(" 2");
    lanes_id_combox->addItem(" 3");
    lanes_id_combox->addItem(" 4");
    lanes_id_combox->addItem(" 5");
    lanes_id_combox->addItem(" 6");
    lanes_id_combox->addItem(" 7");
    lanes_id_combox->addItem(" 8");
    lanes_id_combox->addItem(" 最大值");
    intersection_direction_combox->addItem(" 未知");
    intersection_direction_combox->addItem(" 直行");
    intersection_direction_combox->addItem(" 左转");
    intersection_direction_combox->addItem(" 右转");
    intersection_direction_combox->addItem(" 直行或左转");
    intersection_direction_combox->addItem(" 直行或右转");
    intersection_direction_combox->addItem(" 反向直行");
    intersection_direction_combox->addItem(" 反向左转");
    intersection_direction_combox->addItem(" 反向右转");
    intersection_direction_combox->addItem(" 反向直行或反向左转");
    intersection_direction_combox->addItem(" 反向直行或反向右转");

    delete_line_button->setMinimumHeight(35);
    save_lane_button->setMinimumHeight(35);
    delete_line_button->setMinimumWidth(120);
    save_lane_button->setMinimumWidth(120);

    delete_line_button->setText("删除车道");
    save_lane_button->setText("保存车道");

    delete_line_button->setIcon(QIcon(":/image/Custom-Icon-Design-Mono-General-1-Delete.24.png"));
    save_lane_button->setIcon(QIcon(":/image/Icons8-Windows-8-Sports-Finish-Flag.24.png"));

    label_lane_vlayout->addWidget(select1_tip_label);
    label_lane_vlayout->addWidget(select2_tip_label);
    label_lane_vlayout->addWidget(arrow_tip_label);
    label_lane_vlayout->addWidget(lane_id_tip_label);

    combox_lane_vlayout->addWidget(select_line_combox1);
    combox_lane_vlayout->addWidget(select_line_combox2);
    combox_lane_vlayout->addWidget(intersection_direction_combox);
    combox_lane_vlayout->addWidget(lanes_id_combox);

    label_and_combox_hvlayout->addLayout(label_lane_vlayout, 1);
    label_and_combox_hvlayout->addLayout(combox_lane_vlayout, 2);
    button_lane_hlayout->addWidget(delete_line_button, 1, Qt::AlignCenter);
    button_lane_hlayout->addWidget(save_lane_button, 1, Qt::AlignCenter);

    lanes_window_groupbox_hlayout->addLayout(label_and_combox_hvlayout);
    lanes_window_groupbox_hlayout->addLayout(button_lane_hlayout);
}

void Widget::config_sections_layout()
{
    QVBoxLayout *sections_window_groupbox_hlayout{new QVBoxLayout(section_window_groupbox)};
    QHBoxLayout *label_and_combox_vlayout{new QHBoxLayout()};
    QVBoxLayout *label_section_vlayout{new QVBoxLayout()};
    QVBoxLayout *combox_section_vlayout{new QVBoxLayout()};
    QHBoxLayout *button_section_hlayout{new QHBoxLayout()};

    QLabel *section_line_tip_label{new QLabel(this)};
    /*QLabel *section_sec_tip_label{new QLabel(this)};*/
    QLabel *auto_id_tip_label{new QLabel(this)};

    section_line_tip_label->setText("线号");
    section_sec_tip_label->setText("截断线序号");
    auto_id_tip_label->setText("所属车道ID");

    section_line_tip_label->setStyleSheet("font-size:14px;");
    section_sec_tip_label->setStyleSheet("font-size:14px;");
    auto_id_tip_label->setStyleSheet("font-size:14px;");

    section_line_combox->setStyleSheet("font-size:14px; height:30px;");
    section_sec_combox->setStyleSheet("font-size:14px; height:30px;");
    section_id_combox->setStyleSheet("font-size:14px; height:30px;");

    section_sec_combox->addItem(" 1");
    section_sec_combox->addItem(" 2");
    section_sec_combox->addItem(" 3");
    section_sec_combox->addItem(" 4");
    section_sec_combox->addItem(" 5");
    section_sec_combox->addItem(" 6");
    section_sec_combox->addItem(" 7");
    section_sec_combox->addItem(" 8");
    section_sec_combox->addItem(" 9");
    section_sec_combox->addItem(" 10");
    section_sec_combox->addItem(" 11");
    section_sec_combox->addItem(" 12");
    section_sec_combox->addItem(" 13");
    section_sec_combox->addItem(" 14");
    section_sec_combox->addItem(" 15");
    section_id_combox->addItem(" 未知");
    section_id_combox->addItem(" 1");
    section_id_combox->addItem(" 2");
    section_id_combox->addItem(" 3");
    section_id_combox->addItem(" 4");
    section_id_combox->addItem(" 5");
    section_id_combox->addItem(" 6");
    section_id_combox->addItem(" 7");
    section_id_combox->addItem(" 8");
    section_id_combox->addItem(" 最大值");

    delete_section_button->setMinimumHeight(35);
    save_section_button->setMinimumHeight(35);
    delete_section_button->setMinimumWidth(120);
    save_section_button->setMinimumWidth(120);

    delete_section_button->setText("删除断面");
    save_section_button->setText("保存断面");

    delete_section_button->setIcon(QIcon(":/image/Custom-Icon-Design-Mono-General-1-Delete.24.png"));
    save_section_button->setIcon(QIcon(":/image/Icons8-Windows-8-Sports-Finish-Flag.24.png"));

    label_section_vlayout->addWidget(section_line_tip_label);
    label_section_vlayout->addWidget(auto_id_tip_label);

    combox_section_vlayout->addWidget(section_line_combox);
    combox_section_vlayout->addWidget(section_id_combox);
    combox_section_vlayout->addWidget(section_sec_combox);
    label_section_vlayout->addWidget(section_sec_tip_label);

    button_section_hlayout->addWidget(delete_section_button);
    button_section_hlayout->addWidget(save_section_button);

    label_and_combox_vlayout->addLayout(label_section_vlayout, 1);
    label_and_combox_vlayout->addLayout(combox_section_vlayout, 2);

    sections_window_groupbox_hlayout->addLayout(label_and_combox_vlayout);
    sections_window_groupbox_hlayout->addLayout(button_section_hlayout);
}

void Widget::log_layout()
{
    QHBoxLayout *log_groupbox_vlayout{new QHBoxLayout(log_groupbox)};
    QTextCursor cursor{log_textedit->textCursor()};
    cursor.movePosition(QTextCursor::End);
    log_textedit->setReadOnly(true);
    log_textedit->setTextCursor(cursor);
    log_textedit->ensureCursorVisible();
    log_groupbox_vlayout->addWidget(log_textedit);
}

void Widget::ui_layout()
{
    std::invoke(&Widget::client_layout, this);
    std::invoke(&Widget::stella_layout, this);
    std::invoke(&Widget::config_params_layout, this);
    std::invoke(&Widget::load_info_layout, this);
    std::invoke(&Widget::image_layout, this);
    std::invoke(&Widget::config_lanes_layout, this);
    std::invoke(&Widget::config_sections_layout, this);
    std::invoke(&Widget::log_layout, this);

    QVBoxLayout *left_groupboxs_vlayout{new QVBoxLayout()};
    QVBoxLayout *right_groupboxs_vlayout{new QVBoxLayout()};
    QHBoxLayout *line_add_log_hlayout{new QHBoxLayout()};

    line_add_log_hlayout->addWidget(line_window_groupbox, 1);
    line_add_log_hlayout->addWidget(section_window_groupbox, 1);
    line_add_log_hlayout->addWidget(log_groupbox, 1);

    left_groupboxs_vlayout->addWidget(client_window_groupbox, 1);
    left_groupboxs_vlayout->addWidget(config_params_groupbox, 2);
    left_groupboxs_vlayout->addWidget(load_info_groupbox, 3);
    left_groupboxs_vlayout->addWidget(stella_info_groupbox, 5);

    right_groupboxs_vlayout->addWidget(image_win_label, 4);
    right_groupboxs_vlayout->addLayout(line_add_log_hlayout, 1);

    whole_hlayout->addLayout(left_groupboxs_vlayout, 1);
    whole_hlayout->addLayout(right_groupboxs_vlayout, 3);
}

void Widget::hideOshow(bool _handle)
{
    if(_handle)
    {
        config_params_groupbox->show();
        load_info_groupbox->show();
        line_window_groupbox->show();
        section_window_groupbox->show();
        stella_info_groupbox->hide();
    }
    else
    {
        config_params_groupbox->hide();
        load_info_groupbox->hide();
        line_window_groupbox->hide();
        section_window_groupbox->hide();
        stella_info_groupbox->show();
    }
}




