#ifndef WIDGET2PAINT_H
#define WIDGET2PAINT_H
#include "widget.h"
#include "DataStructure.h"

namespace ui2paint
{

/// @brief 读取结构体进行绘制
void draw_image(QImage _scaled_image, const st_tf::Area& _area, QImage *_recv_image, QLabel *_labal);

/// @brief 撤回至上一步操作
void draw_invoke();

/// @brief 缓存最后一次操作
void add_final_line(st_tf::Area *_area, QComboBox *_combox);

}

#endif // WIDGET2PAINT_H
