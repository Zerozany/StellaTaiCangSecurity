#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H
#include <QPoint>
#include <vector>
#include <opencv2/opencv.hpp>

/// @brief 服务器发送协议
namespace Server_t
{
enum struct InfoHandle : std::int8_t
{
    READY_SEND_IMAGE = 0,    // 服务器回应：已准备发送图片
    READY_SEND_DATA,         // 服务器回应：已准备发送数据
    READY_SEND_MAP,          // 服务器回应：已准备发送map
    SERVER_READY_RECV_DATA,  // 服务器回应：已准备接收数据
    SERVER_REQUEST_RECV_MAP, // 服务器请求：接收数据
    TRANSPORT_ERROR,         // 传输错误
};
}

/// @brief 客户端发送协议
namespace Client_t
{
enum struct InfoHandle : std::int8_t
{
    REQUEST_RECEIVE_IMAGE = 0, // 客户端请求：接收图片
    REQUEST_RECEIVE_DATA,      // 客户端请求：接收数据
    REQUEST_RECEIVE_MAP,       // 客户端请求：接收map
    CLIENT_REQUEST_SEND_DATA,  // 客户端请求：发送数据
    CLIENT_READY_SEND_MAP,     // 客户端回应：已准备发送数
    TRANSPORT_ERROR,           // 传输错误
};
}

/// @brief 数据结构
namespace st_tf
{
    enum DirectionType
    {
        NORTH = 0,
        EAST,
        SOUTH,
        WEST,
        DIR_MAX_VALUE
    };

    enum AreaType
    {
        ENTRY = 0,
        EXIT,
        AREA_MAX_VALUE
    };

    enum ArrowType
    {
        ARROW_UNKNOWN = 0,
        FRONT,
        LEFT,
        RIGHT,
        FRONT_LEFT,
        FRONT_RIGHT,
        BACKWORD_FRONT,
        BACKWORD_LEFT,
        BACKWORD_RIGHT,
        BACKWORD_FRONT_LEFT,
        BACKWORD_FRONT_RIGHT,
        ARROW_MAX_VALUE
    };

    enum LaneID
    {
        LANE_UNKNOWN = 0,
        LANE_FIRST,
        LANE_SECOND,
        LANE_THIRD,
        LANE_FOURTH,
        LANE_FIFTH,
        LANE_SIXTH,
        LANE_SEVENTH,
        LANE_EIGHTH,
        LANE_MAX_VALUE
    };

    struct Line
    {
        std::vector<cv::Point> points;
        int line_id;
    };

    struct Lane
    {
        int line_ids[2];
        std::vector<cv::Point> mask_points;

        ArrowType arrow = ARROW_UNKNOWN;
        LaneID lane_id = LANE_UNKNOWN;
    };

    struct Section
    {
        int line_id;
        LaneID lane_id;
        unsigned char sec_no = 0;
    };

    struct Area
    {
        std::vector<Lane> lanes;
        std::vector<Section> sections;
        std::map<int, Line> line_map;

        unsigned char area_no = 0; // 区域号   通过写入txt获取 这个一个路口应该是一个
        unsigned char junc_no = 0; // 路口号   通过写入txt获取  一个路口应该是一个

        AreaType entry_or_exit = ENTRY;		  // 0进 1出 -1未知 进出口类型 通过写入txt获取
        DirectionType direction_type = NORTH; // 方向号 一个区域的方向号  北 东 南 西 0 1 2 3
    };
}

#endif // DATASTRUCTURE_H

