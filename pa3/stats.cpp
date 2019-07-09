
#include "stats.h"

stats::stats(PNG & im){
    hist.resize(im.width(), vector<vector<int>> (im.height(), vector<int> (36, 0)));     // initializing all the vectors to be the correct size (size of PNG)
    sumHueX.resize(im.width(), vector<double>(im.height(), 0.0));
    sumHueY.resize(im.width(), vector<double>(im.height(), 0.0));
    sumSat.resize(im.width(), vector<double>(im.height(), 0.0));
    sumLum.resize(im.width(), vector<double>(im.height(), 0.0));
    
    
   
    initializeSums(im);   // This will initialize the first row and column in order to allow for the recursion in the next step to work
    

    for (int j = 1; j < (int)im.height(); j++){
        for (int i = 1; i < (int)im.width(); i++){
            HSLAPixel * curr = im.getPixel(i,j);
            double sat = getSums(sumSat[i-1][j], sumSat[i][j-1], sumSat[i-1][j-1], curr->s);
            double lum = getSums(sumLum[i-1][j], sumLum[i][j-1], sumLum[i-1][j-1], curr->l);
            double hueX = getSums(sumHueX[i-1][j], sumHueX[i][j-1], sumHueX[i-1][j-1], cos(curr->h * PI / 180));
            double hueY = getSums(sumHueY[i-1][j], sumHueY[i][j-1], sumHueY[i-1][j-1], sin(curr->h * PI / 180));   

            sumSat[i][j] = sat;
            sumLum[i][j] = lum;
            sumHueX[i][j] = hueX;
            sumHueY[i][j] = hueY;
        }
    }
    
    initializeHist(im);
     

}

void stats::initializeHist(PNG & im){

    for(int j = 0; j < (int)im.height(); j++){
        for (int i = 0; i < (int)im.width(); i++){
            
            
            HSLAPixel * curr = im.getPixel(i,j);
            int k = curr->h / 10;
            
            hist[i][j][k] += 1;

            

            if (i > 0){
                for (int a = i-1; a >= 0; a--){
                    HSLAPixel * temp = im.getPixel(a,j);
                    int t = temp->h / 10;
                    hist[i][j][t] += 1;
                }
            }

           

            if (j > 0){
                for (int b = j-1; b >= 0; b--){
                    HSLAPixel * temp = im.getPixel(i,b);
                    int t = temp->h / 10;
                    hist[i][j][t] += 1;
                    
                }
            }

             

            if (i > 0 && j > 0){
                for (k = 0; k < 36; k++){
                    hist[i][j][k] += hist[i-1][j-1][k];
                }
            }
            

            
        }
    }
    //cout << count <<endl;
    
}


void stats::initializeSums(PNG & im){
    HSLAPixel * start = im.getPixel(0,0);

    sumHueX[0][0] = cos(start->h * PI / 180);
    sumHueY[0][0] = sin(start->h * PI / 180);
    sumLum[0][0] = start->l;
    sumSat[0][0] = start->s;

    for (int i = 1; i < (int)im.width(); i++){
        HSLAPixel * curr = im.getPixel(i,0);
        sumHueX[i][0] = cos(curr->h * PI / 180) + sumHueX[i-1][0];
        sumHueY[i][0] = sin(curr->h * PI / 180) + sumHueY[i-1][0];
        sumLum[i][0] = curr->l + sumLum[i-1][0];
        sumSat[i][0] = curr->s + sumSat[i-1][0];
    }

    for (int j = 1; j < (int)im.height(); j++){
        HSLAPixel * curr = im.getPixel(0,j);
        sumHueX[0][j] = cos(curr->h * PI / 180) + sumHueX[0][j-1];
        sumHueY[0][j] = sin(curr->h * PI / 180) + sumHueY[0][j-1];
        sumLum[0][j]= curr->l + sumLum[0][j-1];
        sumSat[0][j] = curr->s + sumSat[0][j-1];

    }
    
}

double stats::getSums(double sumX, double sumY, double minus, double currVal){
    return sumX + sumY + currVal - minus;
}

long stats::rectArea(pair<int,int> ul, pair<int,int> lr){

    return (lr.second - ul.second + 1) * (lr.first - ul.first + 1);

}

HSLAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){
    double fullRectSat = sumSat[lr.first][lr.second];
    double fullRectLum = sumLum[lr.first][lr.second];
    double fullRectHueX = sumHueX[lr.first][lr.second];
    double fullRectHueY = sumHueY[lr.first][lr.second];

   

    int count = 0;
 
    if (ul.second > 0) {
       
        fullRectSat -= sumSat[lr.first][ul.second-1];
        fullRectLum -= sumLum[lr.first][ul.second-1];
        fullRectHueX -= sumHueX[lr.first][ul.second-1];
        fullRectHueY -= sumHueY[lr.first][ul.second-1];
        
        count++;
    }

   

    if (ul.first > 0) {
        fullRectSat -= sumSat[ul.first - 1][lr.second];
        fullRectLum -= sumLum[ul.first - 1][lr.second];
        fullRectHueX -= sumHueX[ul.first - 1][lr.second];
        fullRectHueY -= sumHueY[ul.first - 1][lr.second];
        
        if (count == 1){                   // THIS WILL ADD BACK THE ORIGIN IF IT WAS ALREADY SUBTRACTED IN THE PREVIOUS FOR LOOP
            fullRectSat += sumSat[ul.first - 1][ul.second - 1];
            fullRectLum += sumLum[ul.first - 1][ul.second - 1];
            fullRectHueX += sumHueX[ul.first - 1][ul.second - 1];
            fullRectHueY += sumHueY[ul.first - 1][ul.second - 1];
        }

    }

     

    double averageSat = fullRectSat / rectArea(ul,lr);
    double averageLum = fullRectLum / rectArea(ul,lr);
    
    double averageHueX = fullRectHueX / rectArea(ul,lr);
    double averageHueY = fullRectHueY / rectArea(ul,lr);
    double averageHue = atan2(averageHueY,averageHueX) * 180 / PI;

    if (averageHue < 0)
        averageHue += 360;

    HSLAPixel moneyTeam = HSLAPixel(averageHue, averageSat, averageLum, 1.0);
    return moneyTeam;

}

vector<int> stats::buildHist(pair<int,int> ul, pair<int,int> lr){
    
    vector<int> fullHist = hist[lr.first][lr.second];
     


    int counter = 0;

    if (ul.first > 0){                               // first BUG, had lr.second > 0 instead of
        for (int k = 0 ; k < 36 ; k++){
            fullHist[k] -= hist[ul.first - 1][lr.second][k];    
        }
        counter++;
    }

    if (ul.second > 0){
        for (int k = 0 ; k < 36 ; k++){
            fullHist[k] -= hist[lr.first][ul.second - 1][k];
            
            if (counter == 1)
                fullHist[k] += hist[ul.first - 1][ul.second - 1][k];
        }
    }


    return fullHist;

}

// takes a distribution and returns entropy
double stats::entropy(vector<int> & distn,int area){

    double entropy = 0;
    for (int i = 0; i < 36; i++) {
        if (distn[i] > 0 ) 
            entropy += ((double) distn[i]/(double) area) 
                                    * log2((double) distn[i]/(double) area);
    }
    return  -1 * entropy;

}

double stats::entropy(pair<int,int> ul, pair<int,int> lr){

    vector<int> distn =  buildHist(ul,lr);
    int area = rectArea(ul,lr);
    return  entropy(distn,area);

}

