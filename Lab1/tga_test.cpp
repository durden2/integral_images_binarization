#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>

#include "Ttiming.h"
#include "TPGM.h"


TTiming tt;//klasa do mierzenia czasu wykonywania sie poszczegolnych funkcji

void buildIntegralImages(unsigned char** image, int h, int w) {
	unsigned char **integral = new unsigned char *[h];
	integral[0] = new unsigned char[w];
	for (int i = 1; i < h; i++)
		integral[i] = new unsigned char[w];

	for (int y = 0; y < h - 1; y++) {
		for (int x = 0; x < w - 1; x++) {
			if (x == 0) {
				if (y != 0) {
					integral[x][y] = integral[x - 1][w - 1] + image[x][y];
				}
				else {
					integral[0][0] = image[0][0];
				}
			} else {
				printf(" %d %d %d", image[y][x], x, y);
				printf("integral: %d", integral[0][0]);
				integral[x][y] = integral[x-1][y] + image[y][x];
			}
		}
	}
}

bool replace(std::string &str, const std::string &from, const std::string &to) {
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}


int main(int argc, char **argv) {
	int h, w; // height, width
	int max_color;
	int hpos, i, j;

	std::string infname = "maly.pgm";

	if ((hpos = readPGMB_header(infname.c_str(), &h, &w, &max_color)) <= 0) exit(1);

	unsigned char **a = new unsigned char *[h];
	a[0] = new unsigned char[h * w];
	for (int i = 1; i < h; i++)
		a[i] = a[i - 1] + w;

	if (readPGMB_data(a[0], infname.c_str(), hpos, h, w, max_color) == 0) exit(1);

	//przygotowanie czarno-bialej tablicy wyjsciowej

	unsigned char **b = new unsigned char *[h];
	b[0] = new unsigned char[h * w];
	for (int i = 1; i < h; i++)
		b[i] = b[i - 1] + w;

	tt.Begin();        //start to measure the time

					   /*    for (i=0; i< h; ++i){
					   for (j=0; j< w; ++j){

					   b[i][j] = (a[i][j] > 128 ) ?  255 : 0;

					   }
					   }*/

					   // lab2
					   //    memset(b[0], 255, w * h);
					   //    int r = 13;
					   //    float k = 0.75;
					   //    int n = (2 * r + 1) * (2 * r + 1);
					   //
					   //    for (int x = r; x < h - r; ++x) {
					   //        for (int y = r; y < w - r; ++y) {
					   //            int sum = 0;
					   //            for (i = x - r; i <= x + r; ++i) {
					   //                for (j = y - r; j <= y + r; ++j) {
					   //                    sum += a[i][j];
					   //                }
					   //            }
					   //            int mean = sum / n;
					   //            if (a[x][y] < k * mean)
					   //                b[x][y] = 0;
					   //        }
					   //
					   //    }
					   //lab3

	memset(b[0], 255, w * h);
	int r = 25;
	float k = 0.12;
	int MAX_STANDARD_DEVIATION = 128;
	int n = (2 * r + 1) * (2 * r + 1);

	for (int x = r; x < h - r; ++x) {
		for (int y = r; y < w - r; ++y) {
			int sum = 0;
			int sumSqrt = 0;
			for (i = x - r; i <= x + r; ++i) {
				for (j = y - r; j <= y + r; ++j) {
					sum += a[i][j];
					sumSqrt += a[i][j] * a[i][j];
				}
			}
			float mean = sum / (float)n;
			float variance = sqrt((sumSqrt - n*mean*mean) / n);
			float treshold = mean * (1 + k*(variance / MAX_STANDARD_DEVIATION - 1));

			if (a[x][y] < treshold) {
				b[x][y] = 0;
			}
		}
	}

	buildIntegralImages(a, h, w);

	double elapsed = tt.End();    //stop and read elapsed time in ms (miliseconds)

	std::string outfname = infname;
	replace(outfname, ".pgm", "_simple.pgm");

	if (writePGMB_image(outfname.c_str(), b[0], h, w, 255) == 0) exit(1);

	delete[] a[0];
	delete[] a;
	delete[] b[0];
	delete[] b;

	printf("czas binaryzacji : %f ms", elapsed);
	getchar();

	return 0;
}