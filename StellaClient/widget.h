#ifndef WIDGET_H
#define WIDGET_H

#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QMouseEvent>
#include <QTimer>
#include <QImage>
#include <QPainter>
#include <QTableWidget>
#include <QHeaderView>
#include "DataStructure.h"
#include "tcpclient.h"

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() = default;
private:
    /// @brief 初始化窗口
    void init_window();

    /// @brief 初始化ui
    void ui_layout();

    /// @brief 客户端ui
    void client_layout();

    /// @brief 公司简介ui
    void stella_layout();

    /// @brief 配置参数ui
    void config_params_layout();

    /// @brief 数据配置ui
    void load_info_layout();

    /// @brief 图像ui
    void image_layout();

    /// @brief 配置车道信息ui
    void config_lanes_layout();

    /// @brief 配置截面信息ui
    void config_sections_layout();

    /// @brief 日志以及参数配置ui
    void log_layout();

    /// @brief 连接信号与槽
    void signals_connect_slots();

    /// @brief ui -> Area全局配置
    void ui2stu(st_tf::Area& _area);

    /// @brief Area全局配置 -> ui
    void stu2ui(const st_tf::Area& _area);

    /// @brief ui -> map
    void ui2map(std::map<std::string, std::string>& _params);

    /// @brief map -> ui
    void map2ui(const std::map<std::string, std::string>& _params);

    /// @brief ui组件显示状态
    void hideOshow(bool _handle);

    /// @brief 保存绘制点
    void save_points(QMouseEvent *event);

    /// @brief 保存线段
    void save_line(QKeyEvent *event, st_tf::Area& _area);

    /// @brief 绘制线段
    void draw_image(const st_tf::Area& _area);

    /// @brief 撤回至上一步
    void draw_invoke();

    /// @brief 添加或删除线段索引
    void addOdelkey(const bool& handle, const st_tf::Area& _area);

public slots:
    /// @brief 连接至服务器槽函数
    void connect_server_slot();

    /// @brief 向服务器请求数据槽函数
    void get_data_slot();

    /// @brief 向服务器发送数据槽函数
    void send_data_slot();

    /// @brief 撤回绘制槽函数
    void __draw_invoke_slot();

    /// @brief 删除车道槽函数
    void delete_lane_slot();

    /// @brief 保存车道槽函数
    void save_lane_slot();

    /// @brief 关联车道ID槽函数
    void lane_and_auto_slot();

    /// @brief 关联车道ID槽函数
    void section_and_auto_slot();

    /// @brief 删除截断线按钮槽函数
    void delete_section_slot();

    /// @brief 保存截面按钮槽函数
    void save_section_slot();

    /// @brief 异步监听定时器槽函数
    void async_timer_slot();

private:
    /// @brief 窗口分辨率变化事件
    void resizeEvent(QResizeEvent *event) override;

    /// @brief 鼠标移动事件
    void mousePressEvent(QMouseEvent *event) override;

    /// @brief 对象跟踪事件
    bool eventFilter(QObject *obj, QEvent *event) override;

    /// @brief 绘画事件
    void paintEvent(QPaintEvent *event) override;

    /// @brief 键盘时间
    void keyPressEvent(QKeyEvent *event) override;

private:
    /// @brief 总布局
    QHBoxLayout *whole_hlayout{new QHBoxLayout(this)};

    /// @brief 客户端通讯box
    QGroupBox *client_window_groupbox{new QGroupBox(tr("客户端"), this)};
    /// @brief 服务器连接ip文本框
    QLineEdit *ip_edit{new QLineEdit(this)};
    /// @brief 连接服务器按钮
    QPushButton *connect_server_button{new QPushButton(this)};
    /// @brief 获取图像按钮
    QPushButton *get_image_button{new QPushButton(this)};

    /// @brief 公司遮挡简介ui
    QGroupBox *stella_info_groupbox{new QGroupBox(tr("公司简介"), this)};

    /// @brief 配置参数box
    QGroupBox *config_params_groupbox{new QGroupBox(tr("配置参数"), this)};
    /// @brief 配置参数表格
    QTableWidget *params_table{new QTableWidget(this)};

    /// @brief 路况信息box
    QGroupBox *load_info_groupbox{new QGroupBox(tr("路况信息"),this)};
    /// @brief 出入口类型下拉框
    QComboBox *intersection_type_combox{new QComboBox(this)};
    /// @brief 区域方向下拉框
    QComboBox *area_direction_combox{new QComboBox(this)};
    /// @brief 区域编号
    QLineEdit *area_number_edit{new QLineEdit(this)};
    /// @brief 路口编号
    QLineEdit *load_number_edit{new QLineEdit(this)};
    /// @brief 绘制撤回按钮
    QPushButton *revoke_button{new QPushButton(this)};
    /// @brief 发送数据按钮
    QPushButton *send_button{new QPushButton(this)};

    /// @brief 图片所在句柄标签
    QLabel *image_win_label{new QLabel(this)};
    /// @brief 接收图片
    QImage recv_image{};
    /// @brief 绘制图层
    QImage scaled_image{};

    /// @brief 车道线box
    QGroupBox *line_window_groupbox{new QGroupBox(tr("车道配置项"), this)};
    /// @brief 线段选择下拉框1
    QComboBox *select_line_combox1{new QComboBox(this)};
    /// @brief 线段选择下拉框2
    QComboBox *select_line_combox2{new QComboBox(this)};
    /// @brief 车道线ID下拉框
    QComboBox *lanes_id_combox{new QComboBox(this)};
    /// @brief 车道线ID下拉框label
    QLabel *lane_id_tip_label{new QLabel(this)};
    /// @brief 路口方向下拉框
    QComboBox *intersection_direction_combox{new QComboBox(this)};
    /// @brief 删除车道按钮
    QPushButton *delete_line_button{new QPushButton(this)};
    /// @brief 保存配置按钮
    QPushButton *save_lane_button{new QPushButton(this)};

    /// @brief 车道线box
    QGroupBox *section_window_groupbox{new QGroupBox(tr("截面配置项"), this)};
    /// @brief 截面所选线段下拉框
    QComboBox *section_line_combox{new QComboBox(this)};
    /// @brief 截面id box
    QComboBox *section_sec_combox{new QComboBox(this)};
    /// @brief 截面所在车道box
    QComboBox *section_id_combox{new QComboBox(this)};
    /// @brief 线号label
    QLabel *section_sec_tip_label{new QLabel(this)};
    /// @brief 删除截断线按钮
    QPushButton *delete_section_button{new QPushButton(this)};
    /// @brief 保存配置按钮
    QPushButton *save_section_button{new QPushButton(this)};

    /// @brief 日志box
    QGroupBox *log_groupbox{new QGroupBox(tr("日志信息"), this)};
    /// @brief 日志框
    QTextEdit *log_textedit{new QTextEdit(this)};

    /// @brief 定时器
    QTimer *async_recv_timer{new QTimer(this)};
    /// @brief 服务器
    TcpClient client{log_textedit};
};
#endif // WIDGET_H
