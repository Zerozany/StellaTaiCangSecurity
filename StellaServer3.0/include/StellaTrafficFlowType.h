_Pragma("once");
#include <vector>
#include <opencv4/opencv2/opencv.hpp>

using namespace std;

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
		vector<cv::Point> points;
		int line_id;
	};

	struct Lane
	{
		int line_ids[2];
		vector<cv::Point> mask_points;

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
		vector<Lane> lanes;
		vector<Section> sections;
		map<int, Line> line_map;

		unsigned char area_no = 0; // 区域号   通过写入txt获取 这个一个路口应该是一个
		unsigned char junc_no = 0; // 路口号   通过写入txt获取  一个路口应该是一个

		AreaType entry_or_exit = ENTRY;		  // 0进 1出 -1未知 进出口类型 通过写入txt获取
		DirectionType direction_type = NORTH; // 方向号 一个区域的方向号  北 东 南 西 0 1 2 3
	};

	struct DTime
	{
		unsigned char year = 0; // 两位数的年份
		unsigned char month = 0;
		unsigned char day = 0;
		unsigned char hour = 0;
		unsigned char minute = 0;
		unsigned char second = 0;
	};

	// 临时定义存放  单个车道方向跟车流量
	struct QueueAttribute
	{
		/* data */
		ArrowType type = ARROW_UNKNOWN;
		LaneID lane_id = LANE_UNKNOWN;
		;
		unsigned char queue_veh_num = 0;   // 检测区实时车辆数
		unsigned char queue_len = 0;	   // 排队长度
		unsigned char queue_start_pos = 0; // 排队起始位
		unsigned char veh_speed = 0;	   // 平均车速
	};
	// 多个车道跟对应车流量 1.3.1
	struct CrossroadQueueInfo
	{
		/* data */
		std::vector<QueueAttribute> lanes_queue_info;
		DTime det_time;					// 检测时间
		unsigned char area_no = 0;		// 区域号   通过写入txt获取 这个一个路口应该是一个
		unsigned char junc_no = 0;		// 路口号   通过写入txt获取  一个路口应该是一个
		DirectionType dir_no = NORTH;	// 方向号 一个区域的方向号  北 东 南 西 0 1 2 3
		AreaType entry_or_exit = ENTRY; // 0进 1出 -1未知 进出口类型 通过写入txt获取
	};

	// 1.3.2 输出模块
	struct VehiclePassInfo
	{
		DTime pass_time;				   // 每个车过车时间
		unsigned int occ_time = 0;		   // 每个车占用时间
		unsigned char veh_speed = 0;	   // 车速
		unsigned char veh_type = 0;		   // 车类型
		unsigned char sec_no = 0;		   // 单个检测断面
		LaneID sec_lane_no = LANE_UNKNOWN; // 车道号 检测断面对应的车道号
		unsigned char area_no = 0;		   // 区域号   通过写入txt获取 这个一个路口应该是一个
		unsigned char junc_no = 0;		   // 路口号   通过写入txt获取  一个路口应该是一个
		DirectionType dir_no = NORTH;	   // 方向号 一个区域的方向号  北 东 南 西 0 1 2 3
		AreaType entry_or_exit = ENTRY;	   // 0进 1出 -1未知 进出口类型 通过写入txt获取
										   // int all_car_no = 0;  // 为了测试用 统计断面车流量
										   // float all_speed_no = 0;  // 为来测试用 统计断面平均车速
	};

	// 1.3.3 输出模块
	struct VehicleStopInfo
	{
		DTime stop_begin_time;			// 每个车停止开始时间
		unsigned char area_no = 0;		// 区域号   通过写入txt获取 这个一个路口应该是一个
		unsigned char junc_no = 0;		// 路口号   通过写入txt获取  一个路口应该是一个
		DirectionType dir_no = NORTH;	// 方向号 一个区域的方向号  北 东 南 西 0 1 2 3
		AreaType entry_or_exit = ENTRY; // 0进 1出 -1未知 进出口类型 通过写入txt获取
		LaneID lane_no = LANE_UNKNOWN;	// 车道号 非检测断面的车道号
		unsigned char stop_pos = 0;		// 每个车的停止位置
		unsigned char stop_times = 0;	// 每个车的停止次数
		unsigned int stop_occ_time = 0; // 每个车停止占用时间
	};

	// 1.3.4 输出模块
	// 单个车道方向跟车流量
	struct LaneFlowInfo
	{
		LaneID lane_id = LANE_UNKNOWN;
		unsigned char flow = 0;		 // 检测区车道车流量
		unsigned char avespeed = 0;	 // 每个车道平均车速
		unsigned char occupancy = 0; // 车道占有率 用
									 // unsigned char sec_no = 0; //这个车道在第几断面
	};

	struct CrossroadFlowInfo
	{
		DTime det_time;				  // 目前暂定写系统一帧运行时间
		unsigned char area_no = 0;	  // 区域号   通过写入txt获取 这个一个路口应该是一个
		unsigned char junc_no = 0;	  // 路口号   通过写入txt获取  一个路口应该是一个
		DirectionType dir_no = NORTH; // 方向号 一个区域的方向号  北 东 南 西 0 1 2 3
		unsigned char sec_no = 0;	  //  mul检测断面号
		std::vector<LaneFlowInfo> lanes_flow_info;
	};
}
