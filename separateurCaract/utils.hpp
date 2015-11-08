#ifndef UTILS_HPP_INCLUDED
#define UTILS_HPP_INCLUDED

#include <opencv2/opencv.hpp>


#define D(n) cout<<#n<<" = "<<(n)<<endl

typedef std::vector<float> Histogram;


float somme(cv::Mat m);

Histogram histogramCols(cv::Mat m, unsigned n);

Histogram histogramRows(cv::Mat m, unsigned n);


void graphicHisto(Histogram hist, cv::Mat histImage);

#endif // UTILS_HPP_INCLUDED
