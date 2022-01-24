#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct PGMFile {
    char type[3];
    int row;
    int col;
    int max_gray;
    int **data;
} PGMFile;

void show_menu();
void image_options(PGMFile *pgm, const char filename[], int option);

/**
 * Open PGM file
 * 
 * @param fp pointer to file passed on argument
 * @param pgm_data pointer to pgm data
 * @param filename name of input argument file
 * @return true (1) if file success open (0) if file failed open
 */
int open_pgm_file(FILE *fp, PGMFile *pgm_data, const char filename[]);

/**
 * Generate a thresholding pgm file of the pgm image
 * 
 * @param pgm_data pointer to pgm data 
 * @param filename name of input argument file
 * @return true (1) if the image is successfully thresholded (0) if it was not possible to do the thresholding
 */
int threshold_image(PGMFile *pgm, const char filename[]);
/**
 * Generate a negative pgm file of the pgm image
 * 
 * @param pgm_data pointer to pgm data 
 * @param filename name of input argument file
 * @return true (1) if the image is successfully negative (0) if it was not possible to do the negative
 */
int negative_image(PGMFile *pgm, const char filename[]);

/**
 * Generate a histogram of the pgm image
 * 
 * @param pgm_data pointer to pgm data 
 * @param filename name of input argument file
 * @return true (1) if the histogram is successfully generated (0) if it was not possible to do the histogram
 */
int generate_histogram(PGMFile *pgm, const char filename[]);

/**
 * Set base head file
 * 
 * @param pgm_data pointer to pgm data 
 * @param filename name of input argument file
 */
void set_pgm_file_head(FILE *fp, PGMFile *data);
int main(int argc, char *argv[]) {

    PGMFile *pgm = malloc(sizeof(PGMFile));
    FILE *fp;
    int option;
    const char *filename;
 
    // handle with image name args
    if (argc == 2)
        filename = argv[1];
    else
        filename = "exemplo.pgm";


    // open file verification
    if (open_pgm_file(fp, pgm, filename)) {
        // menu
        show_menu();
        scanf("%d", &option);

        // take option and execute proccess
        image_options(pgm, filename, option);
    } else 
        printf("Não foi possível abrir o arquivo!");

    fclose(fp);
    
    printf("\nFinalizando ...\n");

    return 0;
}
int open_pgm_file(FILE *fp, PGMFile *pgm_data, const char filename[]) {

    int i, j;

    fp = fopen(filename, "rb");

    if (fp == NULL) {
        printf("Falha ao abrir o arquivo");
        return 0;
    }

    // get file type
    fgets(pgm_data->type, sizeof(pgm_data->type), fp);

    // get file info
    fscanf(fp, "%d", &pgm_data->col);
    fscanf(fp, "%d", &pgm_data->row);
    fscanf(fp, "%d", &pgm_data->max_gray);

    // allocate data matrix
    pgm_data->data = (int **)malloc(pgm_data->row * sizeof(int *));

    for (i = 0; i < pgm_data->row; i++){
        pgm_data->data[i] = (int *)malloc(pgm_data->col * sizeof(int));
    }

    // get image data pixels
    for (i = 0; i < pgm_data->row; ++i) {
        for (j = 0; j < pgm_data->col; ++j) {
            fscanf(fp, "%d", &pgm_data->data[i][j]);
        }
    }

    return 1;
}
void show_menu() {
    printf("-------------------------------------------\n");
    printf("O que voce deseja fazer com a imagem? \n");
    printf("Digite: \n");
    printf("\n");
    printf("(1) Limiarizacao \n");
    printf("(2) Negativo \n");
    printf("(3) Gerar Histogram\n");
    printf("(4) Gerar todos os arquivos acima\n");
    printf("\nOpcao: ");
}

void image_options(PGMFile *pgm, const char filename[], int option) {

    int res;

    switch (option) {

        case 1:
            res = threshold_image(pgm, filename);
            break;

        case 2:
            res = negative_image(pgm, filename);
            break;
    
        case 3:
            res = generate_histogram(pgm, filename);
            break;

        case 4:
            res = threshold_image(pgm, filename) == negative_image(pgm, filename) == generate_histogram(pgm, filename);
            break;
    
        default:
            break;
    }

    printf(res ? "\nImagem gerada com sucesso!" : "\nFalha ao gerar imagem");
}
int threshold_image(PGMFile *pgm, const char filename[]) {

    int i, j;
    FILE *fp;

    fp = fopen("threshold-exemple.pgm", "wb");

    if (fp == NULL) {
        printf("\nFalha ao criar novo arquivo!");
        return 0;
    } else 
        printf("\nGerando limiarizacao...");

    // set base archive head
    set_pgm_file_head(fp, pgm);

    // set converted pixels
    for (i = 0; i < pgm->row; ++i) {
        for (j = 0; j < pgm->col; ++j) {
            fprintf(fp, "%d ", (pgm->data[i][j] > 150) ? pgm->max_gray : 0); 
        }
    }
    
    fclose(fp);

    return 1;
}
int negative_image(PGMFile *pgm, const char filename[]) {

    int i, j;
    FILE *fp;

    fp = fopen("negative-exemple.pgm", "wb");

    if (fp == NULL) {
        printf("\nFalha ao criar novo arquivo!");
        return 0;
    } else 
        printf("\nGerando negativo...");

    // set base archive head
    set_pgm_file_head(fp, pgm);

    // set converted pixels
    for (i = 0; i < pgm->row; ++i) {
        for (j = 0; j < pgm->col; ++j) {
            fprintf(fp, "%d ", (pgm->max_gray - pgm->data[i][j])); 
        }
    }
    
    fclose(fp);

    return 1;
}
int generate_histogram(PGMFile *pgm, const char filename[]) {

    int i, j;
    int *histogram_arr;
    FILE *fp;

    if (fp == NULL) {
        printf("\nFalha ao criar novo arquivo!");
        return 0;
    } else 
        printf("\nGerando histograma...");

    // allocate histogram array
    histogram_arr = (int *)malloc(pgm->max_gray * sizeof(int));
    
    for (i = 0; i <= pgm->max_gray; i++)
        histogram_arr[i] = 0;

    // create txt file
    fp = fopen("histogram-exemple.txt", "wb");

    for (i = 0; i < pgm->row; ++i) {
        for (j = 0; j < pgm->col; ++j) {
            histogram_arr[pgm->data[i][j]]++;
        }
    }

    fprintf(fp, "tonal -- count \n"); 

    for (i = 0; i <= pgm->max_gray; i++)
        fprintf(fp, "%d -- %d \n", i, histogram_arr[i]); 

    fclose(fp);
    free(histogram_arr);

    return 1;
}
void set_pgm_file_head(FILE *fp, PGMFile *data) {
    fprintf(fp, "%s\n", data->type);
    fprintf(fp, "%d %d\n", data->row, data->col);
    fprintf(fp, "%d\n", data->max_gray);
}