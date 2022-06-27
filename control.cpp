
#define TILE_SIZE_A 64
#define TILE_SIZE_X 32
#define TILE_SIZE_W 32

// #define BUF_SIZE (TILE_SIZE_X*TILE_SIZE_X)
#define BUF_SIZE 4096

extern "C" {
void control(float* A, float* X, float* W, float* C) {

	float buf_A[16384];

	float buf_B[65536];

	float buf_X[16384];

	float buf_W[8192];

	float buf_C[16384];

#pragma HLS bind_storage variable=buf_A type=ram_2p impl=bram
#pragma HLS bind_storage variable=buf_B type=ram_2p impl=uram
#pragma HLS bind_storage variable=buf_X type=ram_2p impl=bram
#pragma HLS bind_storage variable=buf_W type=ram_2p impl=bram
#pragma HLS bind_storage variable=buf_C type=ram_2p impl=bram

readA:
    for (int i = 0; i < BUF_SIZE; i++) {
#pragma HLS PIPELINE II = 1
    	buf_A[i] = A[i];
    	buf_X[i] = X[i];
    	buf_W[i] = W[i];
    }

initB:
    for (int i = 0; i < BUF_SIZE; i++) {
#pragma HLS PIPELINE II = 1
        buf_B[i] = 0.0;
        buf_C[i] = 0.0;
    }

  	for (int i = 0; i < BUF_SIZE; i++) {
#pragma HLS PIPELINE II = 1
  		if (i % 32 == 0) {
  			buf_B[i] = 1.0;
        	buf_C[i] = 1.0;
  		} else if (i % 64 == 0) {
  			buf_B[i] = 2.0;
        	buf_C[i] = 2.0;
  		} else if (i % 128 == 0) {
  			buf_B[i] = 4.0;
        	buf_C[i] = 4.0;
  		} else if (i % 256 == 0) {
  			buf_B[i] = 8.0;
        	buf_C[i] = 8.0;
  		}
    }

writeC:
    for (int i = 0; i < BUF_SIZE; i++) {
#pragma HLS PIPELINE II = 1
        C[i] = buf_C[i];
    }
}

}