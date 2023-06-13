#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#define GL_SILENCE_DEPRECATION

#include <QtOpenGLWidgets/QOpenGLWidget>
#include <stddef.h>
#include <QOpenGLFunctions>
#include <QtOpenGL>
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QtWidgets/QWidget>
#include <iostream>
#include <new>
#include <vector>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QApplication>
#include <QScreen>
#include <QPixmap>
#include <QPushButton>
#include <QTime>
#include <QColor>

extern "C" {
#include "../Backend/Headers/affinity_transformations.h"
#include "./GIFCreation/gifImage/qgifglobal.h"
#include "./GIFCreation/gifImage/qgifimage.h"
#include "./GIFCreation/gifLib/gif_lib.h"
}

enum extension {
  kJpeg,
  kBmp,
};

class MyOpenGLWidget : public QOpenGLWidget {
  Q_OBJECT
 public:
    MyOpenGLWidget(QWidget* parrent = nullptr);

    Model* model;
    std::vector<double> array;
    float line_thickness;
    int vertex_thickness;
    int widgetWidth;
    int widgetHeight;
    float scale;
    int last_x = 0;
    int last_y = 0;
    int last_z = 0;
    QColor backgroundColor;
    QColor vertexColor;
    QColor linesColor;

    int status = 0;

    bool renderingPaused;

    QString file_path;
    QPoint lastMousePos; // Последняя позиция мыши

    int modelRotation_x = 0; // Угол вращения модели
    int modelRotation_y = 0; // Угол вращения модели
    int modelRotation_z = 0; // Угол вращения модели
      // Последняя позиция мыши
  float modelRotation = 0.0f;  // Угол вращения модели

#define kPROJORT 0
#define kPROJCEN 1

    char projection = kPROJORT;

#define kLINESOLID 1
#define kLINEDASHD 0

    char line_type = kLINESOLID;

#define kNOPOINT 0
#define kROUNDPOINT 1
#define kSQUAREPOINT 2

    char point_type = kNOPOINT;

    void matrix_to_array();

    void pauseRendering();
    void resumeRendering();

    float getLineThickness() const;
   int getVertexThickness() const;
   QColor getBackgroundColor() const;
   QColor getVertexColor() const;
   QColor getLinesColor() const;
   char getLineType() const;
   char getPointType() const;
    int getProjection() const;
    ~MyOpenGLWidget();

signals:
    void angleChanged_x(int angle);
    void angleChanged_y(int angle);
    void angleChanged_z(int angle);
    void colorChanged(int type, QColor t_color);
    void projectionChanged(int type);
    void lineThicknessChanged(int value);
    void vertexThicknessChanged(int value);
    void backgroundColorChanged(QColor color);
    void vertexColorChanged(QColor color);
    void linesColorChanged(QColor color);
    void lineTypeChanged(int value);
    void pointTypeChanged(int value);

public Q_SLOTS:
    void rotate_x(int angle);
    void rotate_y(int angle);
    void rotate_z(int angle);
    void translate_x(int angle);
    void translate_y(int angle);
    void translate_z(int angle);
    void scale_change(int number);
    void listening_to_a_lines_signal(bool state);
    void setProjection(int value);
    void setLineThickness(float value);
    void setVertexThickness(int value);
    void setBackgroundColor(QColor color);
    void setVertexColor(QColor color);
    void setLinesColor(QColor color);
    void setLineType(char value);
    void setPointType(char value);

private:
    int prev_angle_x = 0;
    int prev_angle_y = 0;
    int prev_angle_z = 0;
    int prev_trans_x = 0;
    int prev_trans_y = 0;
    int prev_trans_z = 0;
    QString file_mame;
    int vertex_amount;
    int lines_amount;
    void rotate_xyz(float angle_x, float angle_y, float angle_z);
    void translate_xyz(float angle_x, float angle_y, float angle_z);
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void initializeGL() override;
    void paintGL() override;
};

int NormalizeAngle(int angle);
int ModCelil(float num);
#endif // MYOPENGLWIDGET_H
