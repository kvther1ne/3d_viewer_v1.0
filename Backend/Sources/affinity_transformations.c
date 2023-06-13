// #include "../Headers/parser.h"
#include "../Headers/affinity_transformations.h"

void move_to_xyz(Model* model, double x_value, double y_value, double z_value) {
  for (size_t i = 1; i <= model->count_of_peaks; i++) {
    for (size_t j = 0; j < 3; j++) {
      model->matrix_peaks[i][j] += j == 1   ? y_value * 0.05
                                   : j == 0 ? x_value * 0.05
                                            : z_value * 0.05;
    }
  }
}

void rotate_model(Model* model, double x_angle, double y_angle,
                  double z_angle) {
  if (x_angle) {
    for (size_t i = 1; i <= model->count_of_peaks; i++) {
      double temp_y = model->matrix_peaks[i][1];
      double temp_z = model->matrix_peaks[i][2];
      model->matrix_peaks[i][1] = cos(x_angle) * temp_y - sin(x_angle) * temp_z;
      model->matrix_peaks[i][2] = sin(x_angle) * temp_y + cos(x_angle) * temp_z;
    }
  }
  if (y_angle) {
    for (size_t i = 1; i <= model->count_of_peaks; i++) {
      double temp_x = model->matrix_peaks[i][0];
      double temp_z = model->matrix_peaks[i][2];
      model->matrix_peaks[i][0] = cos(y_angle) * temp_x + sin(y_angle) * temp_z;
      model->matrix_peaks[i][2] = cos(y_angle) * temp_z - sin(y_angle) * temp_x;
    }
  }
  if (z_angle) {
    for (size_t i = 1; i <= model->count_of_peaks; i++) {
      double temp_x = model->matrix_peaks[i][0];
      double temp_y = model->matrix_peaks[i][1];
      model->matrix_peaks[i][0] = cos(z_angle) * temp_x + sin(z_angle) * temp_y;
      model->matrix_peaks[i][1] = cos(z_angle) * temp_y - sin(z_angle) * temp_x;
    }
  }
}
