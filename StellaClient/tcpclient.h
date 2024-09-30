#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QFile>
#include <QThread>
#include <QDataStream>
#include <QByteArray>
#include <mutex>
#include <QTextEdit>
#include <QDateTime>
#include <QApplication>
#include <QComboBox>
#include <atomic>
#include "StellaSerialize.h"

/// @brief 客户端类
class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QTextEdit* _log_textedit, QObject *parent = nullptr);
    ~TcpClient();

    /// @brief 获取ip地址
    void get_ip(const QString& _ip);

    /// @brief 获取当前时间
    const QString get_time();

    /// @brief 连接服务器
    void connect_server();

    /// @brief 断开连接
    void stop();

    /// @brief 返回客户端连接状态
    bool get_connect_status();

    /// @brief 发送请求
    void send_request(const Client_t::InfoHandle& _info_handle);

    /// @brief 接收请求
    void recv_request();

    /// @brief 处理请求
    void resolve_request(const Server_t::InfoHandle& _info_handle);

    /// @brief 接收图片
    bool recv_image();

    /// @brief 接收结构体
    bool recv_stu();

    /// @brief 接收map
    bool recv_map();

    /// @brief 发送结构体
    bool send_stu();

    /// @brief 发送map
    bool send_map();

    /// @brief 删除缓存图片
    void delete_image();

public:
    /// @brief 日志
    QTextEdit* m_log_textedit{};

    /// @brief 客户端套接字
    QTcpSocket m_client_socket{};

    /// @brief 图片存储路径
    QString m_image_path{QCoreApplication::applicationDirPath() + "/stellaLine.jpg"};

    /// @brief 数据缓存区
    st_tf::Area m_area{};

    /// @brief 配置参数容器
    std::map<std::string, std::string> m_params_buff{};

    /// @brief 绘制表格(ui)句柄
    std::atomic<bool> table_handle{false};

private:
    /// @brief 服务器ip地址
    QString m_host{};

    /// @brief 服务器端口
    quint16 m_port{13000};

    /// @brief 客户端互斥锁
    std::mutex m_mtx{};
};

#endif // TCPCLIENT_H
