#include "utils.hpp"

using namespace cv;

float somme(cv::Mat m){
    float s = 0;
    for(int i = 0; i<m.rows; ++i){
        for(int j = 0; j<m.cols; ++j){
           s += m.at<uchar>(i, j) == 0?1:0;
        }
    }
    return s;
}

Histogram histogramCols(cv::Mat m, unsigned n){
    float pas = m.cols/n;
    Histogram hist(n);
    for(unsigned k=0; k<n; ++k){
        cv::Mat m1(m, cv::Range::all(), cv::Range(k*pas, (k+1)*pas));
        long s = somme(m1);
        hist[k] = s/(pas*m.rows);
    }
    return hist;
}

Histogram histogramRows(cv::Mat m, unsigned n){
    float pas = m.rows/n;
    Histogram hist(n);
    for(unsigned k=0; k<n; ++k){
        cv::Mat m1(m, cv::Range(k*pas, (k+1)*pas), cv::Range::all());
        long s = somme(m1);
        hist[k] = s/(pas*m.cols);
    }
    return hist;
}


void graphicHisto(Histogram hist, cv::Mat histImage){
    int hist_r = histImage.rows;

    for(unsigned i = 0; i<hist.size(); ++i){
        for(int j = 0; j<hist[i]*hist_r; ++j){
            histImage.at<uchar>(j, i) = 255;
        }
    }
}
