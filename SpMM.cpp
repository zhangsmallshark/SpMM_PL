#define HIDDEN_DIM 128
#define NNZS = 16

extern "C" {
void SpMM(int* indices, float* data, float* B, float* C) {
	int indices_buf[NNZS];
	float data_buf[NNZS];
	float B_buf[HIDDEN_DIM];
	float C_buf[HIDDEN_DIM];

readA:
    for (int i = 0; i < NNZS; i++) {
#pragma HLS PIPELINE II = 1
        indices_buf[i] = indices[i];
        data_buf[i] = data[i];
    }

readB:
    for (int i = 0; i < HIDDEN_DIM; i++) {
#pragma HLS PIPELINE II = 1
        B_buf[i] = B[i];
    }

compute:
	for (int i0 = 0; i0 < NNZS; i0++) {
#pragma HLS LOOP_TRIPCOUNT min = NNZS max = NNZS
#pragma HLS PIPELINE II = 1
		for (int i1 = 0; i1 < HIDDEN_DIM; i1++) {
// #pragma HLS LOOP_TRIPCOUNT min = HIDDEN_DIM max = HIDDEN_DIM
// #pragma HLS UNROLL
			C_buf[i1] += data_buf[i0] * B_buf[i1];
		}
	}

writeC:
    for (int i = 0; i < HIDDEN_DIM; i++) {
#pragma HLS PIPELINE II = 1
        C[i] = C_buf[i];
    }
}

}
