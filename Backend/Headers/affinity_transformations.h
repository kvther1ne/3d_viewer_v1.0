#ifndef _AFFINITY_TRANSFORMATIONS_H_
#define _AFFINITY_TRANSFORMATIONS_H_
#include "parser.h"

/*!
\brief Перемещение модели
\details Функция move_to_xyz используется для перемещения модели в трехмерном
пространстве (3D) в указанную позицию, заданную координатами X, Y и Z. Она
принимает указатель на структуру Model и значения X, Y и Z в качестве
параметров.
\param[in] model Указатель на структуру Model, представляющую
перемещаемую модель
\param[in] x_value Значение типа double, представляющее
координату X целевой позиции
\param[in] y_value Значение типа double, представляющее координату Y целевой
позиции
\param[in] z_value Значение типа double, представляющее координату Z целевой
позиции
*/
void move_to_xyz(Model* model, double x_value, double y_value, double z_value);
/*!
\brief Вращение модели
\details Функция rotate_model используется для поворота модели в трехмерном
пространстве (3D) вокруг осей X, Y и Z на указанные углы. Она принимает
указатель на структуру Model и значения углов X, Y и Z в качестве параметров.
\param[in] model Указатель на структуру Model, представляющую
поворачиваемую модель
\param[in] x_value Значение типа double, представляющее
угол поворота вокруг оси X целевой позиции
\param[in] y_value Значение типа double, представляющее угол поворота вокруг оси
Y целевой позиции
\param[in] z_value Значение типа double, представляющее угол
поворота вокруг оси Z целевой позиции
*/
void rotate_model(Model* model, double x_angle, double y_angle, double z_angle);

#endif  //_AFFINITY_TRANSFORMATIONS_H_
