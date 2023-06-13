#include "../Headers/parser.h"

// int main(int argc, char const *argv[])
// {
// 	const char* file = "../../models/cube.obj";
// 	Model* a = (Model*)malloc(1*sizeof(Model));
// 	printf("res = %d\n", parser(a, file));

// 	// for (int i = 1; i <= a->count_of_peaks; i++) {
// 	// 	for (int j = 0; j < 3; j++) {
// 	// 		printf("%f ", a->matrix_peaks[i][j]);
// 	// 	}
// 	// 	printf("\n");
// 	// }

// 	for (int i = 1; i <= a->count_of_poligons; i++) {
// 		for (int j = 0; j < a->array_for_edjes[i]; j++) {
// 			printf("%u ", a->matrix_poligons[i][j]);
// 		}
// 		printf("\n");
// 	}

// 	for (int i = 0; i <= a->count_of_poligons; i++) {
// 		printf("%u ", a->array_for_edjes[i]);
// 	}
// 	printf("\n");

// 	free_model(a);
// 	return 0;
// }

int parser(Model* model, const char* file) {
	setlocale(LC_NUMERIC, "C"); // Установка локали со стандартным форматом чисел
	// fprintf(stderr, "%s\n", file);
    int res = 0, count = 1, error = kOK;
	FILE* fp = NULL;
	char* line = NULL;
    size_t len = 0, counter = 1;
	fp = fopen(file, "r");
	if (fp) {
		peaks_and_poligons_counter(model, fp);
		rewind(fp); //вернули каретку в начало файла
		model = allocation_and_initialization(model, &error);
		if (!error) {
			while (getline(&line, &len, fp) != -1) {
				if (!memcmp(line, "v ", 2)) {
					if ((res = sscanf(line + 1, "%lf %lf %lf", PEAK(0), PEAK(1), PEAK(2))) != 3) {
						// free_model(model);
						// fprintf(stderr, "Incorrect values of peaks\n");
						error = kINCORRECT_PEAKS;
					}
					// если строка была не корректной то нужно фришить массив для model->count_of_peaks + 1 (так как память под новую строку была выделена но не заполнена)
                    count++;
				} else if (!memcmp(line, "f ", 2)) {
					model = get_poligon(model, line, &counter);
				} else {
					continue;
				}
			}
		} else {
			free_model(model);
		}
		fclose(fp);
		free(line);
	} else {
		// fprintf(stderr, "File read error");
		error = kFILE_READ_ERROR;
	}
	// fprintf(stderr, "peaks = %lu\n", model->count_of_peaks);
	return error;
}

void peaks_and_poligons_counter(Model* model, FILE* fp) {
	char* line = NULL;
	size_t buff = 0, peaks_count = 0, poligons_count = 0;
	for (; getline(&line, &buff, fp) != -1;) {
		if (!memcmp(line, "v ", 2))
			peaks_count++;
		if (!memcmp(line, "f ", 2))
			poligons_count++;
	}
	model->count_of_peaks = peaks_count;
	model->count_of_poligons = poligons_count;
	free(line);
}

void free_model(Model* model) {
	for (size_t i = 0; i <= model->count_of_peaks; i++) {
		free(model->matrix_peaks[i]);
	}
	free(model->matrix_peaks);

	for (size_t i = 0; i <= model->count_of_poligons; i++) {
		free(model->matrix_poligons[i]);		
	}
	free(model->matrix_poligons);
	free(model->array_for_edjes);
	free(model);
}

Model* get_poligon(Model* model, char* line, size_t* counter) {
	size_t n = vertexes_counter_for_poligon(line), i = 0;
	model->array_for_edjes[*counter] = n * 2;
	model->matrix_poligons[*counter] = (unsigned*)calloc(n * 2, sizeof(unsigned));
	char* buff_for_tokens = NULL;
	buff_for_tokens = strtok(line + 1, " \n");
	while (buff_for_tokens) {
		long res = atol(buff_for_tokens) - 1;
		if (i == 0) {
			model->matrix_poligons[*counter][i] = res;
			i++;
		} else {
			model->matrix_poligons[*counter][i] = res;
			model->matrix_poligons[*counter][i+1] = res;
			i += 2;
		}
		buff_for_tokens = strtok(NULL, " \n");
	}
	model->matrix_poligons[*counter][i] = model->matrix_poligons[*counter][0];
	(*counter)++;
	return model;
}

size_t vertexes_counter_for_poligon(char* line) {
	char* buff = (char *)calloc(strlen(line) + 1, sizeof(char));
	strcpy(buff, line);
	buff[strlen(buff)] = '\0';
	char* tok = strtok(buff + 1, " \n");
	size_t n = 0;
	while (tok) {
		n++;
		tok = strtok(NULL, " \n");
	}
	free(buff);
	return n;
}

Model* allocation_and_initialization(Model* model, int* error) {
	model = allcation_for_peaks_matrix(model, error);
	model = allcation_for_poligons(model, error);
	return model;
}

Model* allcation_for_poligons(Model* model, int* error) {
	if ((model->matrix_poligons = (unsigned**)calloc(model->count_of_poligons + 1, sizeof(unsigned*))) == NULL)
		*error = kMEMORY_ALLOCATION_ERROR;
	if ((model->array_for_edjes = (size_t*)calloc(model->count_of_poligons + 1, sizeof(size_t))) == NULL)
		*error = kMEMORY_ALLOCATION_ERROR;
	return model;
}

Model* allcation_for_peaks_matrix(Model* model, int* error) {
	if ((model->matrix_peaks = (double**)calloc(model->count_of_peaks + 1, sizeof(double*))) == NULL) 
		*error = kMEMORY_ALLOCATION_ERROR;
	if (!(*error)) {
		for (size_t i = 1; i < model->count_of_peaks + 1; i++) {
			if ((model->matrix_peaks[i] = (double*)calloc(3, sizeof(double))) == NULL) {
				*error = kMEMORY_ALLOCATION_ERROR;
				break;
			}
		}
	}
	return model;
}
