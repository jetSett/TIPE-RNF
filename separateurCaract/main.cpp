#include <iostream>

#include "utils.hpp"


using namespace cv;
using namespace std;


void seuil(Mat m, unsigned char seuilNoir){
    for(int i = 0; i<m.rows; ++i){
        for(int j = 0; j<m.cols; ++j){
            m.at<uchar>(i, j) = m.at<uchar>(i, j)>=seuilNoir?255:0;
        }
    }
}

std::vector<Mat> lignes(Mat m, Histogram histR){
    std::vector<Mat> result;
    enum Etat{ WHITE, BLACK };
    Etat state = BLACK;
    unsigned a = 0, b = 0;
    for(unsigned i = 0; i<histR.size(); ++i){
        if(histR[i] != 0 and state == BLACK ){
            a = i;
            state = WHITE;
        }else if(histR[i] == 0 and state == WHITE){
            b = i;
            result.push_back(Mat(m, Range(a, b), Range::all()));
            state = BLACK;
        }
    }
    if(state == WHITE){
            result.push_back(Mat(m, Range(a, histR.size()-1), Range::all()));
    }
    return result;
}

std::vector<Mat> caracts(Mat m, Histogram histC){
    std::vector<Mat> result;
    enum Etat{ WHITE, BLACK };
    Etat state = BLACK;
    unsigned a = 0, b = 0;
    for(unsigned i = 0; i<histC.size(); ++i){
        if(histC[i] != 0 and state == BLACK ){
            a = i;
            state = WHITE;
        }else if(histC[i] == 0 and state == WHITE){
            b = i;
            result.push_back(Mat(m, Range::all(), Range(a, b)));
            state = BLACK;
        }
    }
    if(state == WHITE){
            result.push_back(Mat(m, Range::all(), Range(a, histC.size()-1)));
    }
    return result;
}

void writeCaracts(std::vector<Mat> caracteres){
    int i = 0;
    bool maj = false;
    for(auto c : caracteres){
        ostringstream oss;

        if(!maj)
            oss << char('a' + i);
        else
            oss << char('A' + i);

        if(i == 25){
            i = -1;
            maj = true;
        }

        imwrite("../Samples/ComicSansMs/"+oss.str()+".png", c);
        i++;
    }
}


int main()
{

    Mat m = imread("../lettresComicSansMs.png", CV_LOAD_IMAGE_GRAYSCALE);
    seuil(m, 200);

    vector<Mat> lettres;
    ///1 -> on extrait les lignes
    auto tabLignes = lignes(m, histogramRows(m, m.rows));
    ///2 -> on extrait les caractères de charques lignes
    for(Mat l : tabLignes){
        auto c = caracts(l, histogramCols(l, l.cols));
        for(Mat carac : c){
            lettres.push_back(carac);
        }
    }

    writeCaracts(lettres);

    return 0;
}
