#include <iostream>
#include <vector>
#include <fstream>
#include "stb/im_mat.h"
#include "stb/mat_im.h"
#include "Gussian/gussian.h"
#include "sobel/sobel.h"
#include "non_maximum_suppression/nms.h"
#include "Threshold/threshold.h"
#include "hysteresis/hy.h"
#include "morphology/dilation.h"
#include "morphology/erosion.h"

using namespace std;



vector<vector<int>> image_to_matrix(const char* path) {
    return im_mat::get_matrix(path);
}

vector<vector<float>> inttofloat(
    const vector<vector<int>>& input
) {
    int H = input.size();
    int W = input[0].size();

    vector<vector<float>> out(H, vector<float>(W));
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            out[i][j] = static_cast<float>(input[i][j]);

    return out;
}

void save_matrix_to_csvf(
    const vector<vector<float>>& matrix,
    const char* filename
) {
    ofstream file(filename);
    if (!file.is_open()) return;

    int H = matrix.size();
    int W = matrix[0].size();

    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            file << matrix[i][j];
            if (j != W - 1) file << ",";
        }
        file << "\n";
    }
    file.close();
}

void save_matrix_to_csv(
    const vector<vector<int>>& matrix,
    const char* filename
) {
    ofstream file(filename);
    if (!file.is_open()) return;

    int H = matrix.size();
    int W = matrix[0].size();

    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            file << matrix[i][j];
            if (j != W - 1) file << ",";
        }
        file << "\n";
    }
    file.close();
}

void matrix_to_image(
    const vector<vector<int>>& data,
    const char* outPath
) {
    mat_im::matrix_to_image(data, outPath);
}




vector<vector<int>> apply_operation_for_detction(
    const vector<vector<float>>& input
) {
    auto blurred = img::gaussianBlur(input, 3);
    save_matrix_to_csvf(blurred, "training_data/gaussian.csv");

    img::EdgeResult grad = img::scharrEdge(blurred);
    save_matrix_to_csvf(grad.Mag,   "training_data/grad_mag.csv");
    save_matrix_to_csvf(grad.Angle, "training_data/grad_angle.csv");

    auto nms = img::nonMaximumSuppression(grad.Mag, grad.Angle);
    save_matrix_to_csvf(nms, "training_data/nms.csv");

    auto thresh = img::doubleThreshold(nms);
    save_matrix_to_csv(thresh, "training_data/threshold.csv");

    auto hyst = img::hysteresis(thresh);
    save_matrix_to_csv(hyst, "training_data/hysteresis.csv");

    auto dilated = img::dilation(hyst);
    auto final_edges = img::erosion(dilated);
    save_matrix_to_csv(final_edges, "training_data/final_edges.csv");

    return final_edges;
}



int main() {

    const char* inputPath  ="image\\Adeeb_.jpg";
;   
    const char* outputPath ="results\\output_edges.png";

    auto image_int = image_to_matrix(inputPath);
    if (image_int.empty()) return 0;

    save_matrix_to_csv(image_int, "training_data/orig.csv");

    auto image_float = inttofloat(image_int);

    auto final_edges =apply_operation_for_detction(image_float);

    matrix_to_image(final_edges, outputPath);

    cout << "DONE\n";
    return 0;
}
