/******************************************************************************
*   Copyright 2021 Politecnico di Milano
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*******************************************************************************/
#include "user.h"
extern "C" {
#include "simple_random.h"
}
#include "data/iris_dataset.h"

#include <algorithm>
#include <cmath>
#include <vector>
#include <map>

MEASURE_GLOBAL_VARIABLES()
class DataPoint{
    public:
        DataPoint();
        DataPoint(double * data_point):
                sepal_length(data_point[0]),
                sepal_width(data_point[1]),
                petal_length(data_point[2]),
                petal_width(data_point[3]),
                label(data_point[4])
                {}
        DataPoint(double sepal_length,double sepal_width,double petal_length,double petal_width):
                sepal_length(sepal_length),
                sepal_width(sepal_width),
                petal_length(petal_length),
                petal_width(petal_width),
                label(-1)
                {}
        double distance(DataPoint & d){
            return sqrt(pow(sepal_length-d.sepal_length,2)+
                        pow(sepal_width-d.sepal_width,2)+
                        pow(petal_length-d.petal_length,2)+
                        pow(petal_width-d.petal_width,2));
        }
        int get_label(){
            return label;
        }
        
        bool operator < (const DataPoint & d) const
        {
            return true;
        }
        friend class kNN_classifier;

    private:
        double sepal_length,sepal_width,petal_length,petal_width;
        int label; //0-Iris Setosa,1-Iris Versicolor, 2-Iris Virginica
};

class kNN_classifier{
    public:
        kNN_classifier(int k, double  data_points[][5], int data_len):
            k(k)
            {
                for(int i=0;i<data_len;i++){
                    data.emplace_back(data_points[i]);
                }
                if(k>data_len){
                    k=data_len;
                }
            };

        int classify(DataPoint & observation){
            std::vector<std::pair<double,DataPoint>> distances;
            for(auto & d: data){
                distances.emplace_back(observation.distance(d),d);
            }

            std::sort(distances.begin(),distances.end());
            distances.erase(distances.begin()+k+1,distances.end());

            std::map<int,int> k_neighbour_classes;
            for(auto & p: distances){
                k_neighbour_classes[p.second.label]++;
            }

            int observation_label, max_rep=-1;
            for (auto it = k_neighbour_classes.begin(); it != k_neighbour_classes.end(); it++){
                if(it->second>max_rep){
                    max_rep=it->second;
                    observation_label=it->first;
                }
            }
            observation.label=observation_label;
            return observation_label;
        }
        
    private:
        int k; 
        std::vector<DataPoint> data;
        
};
 
/**
 * @brief Actual quadratic equation solver implementation
 * 
 * @param a 
 * @param b 
 * @param c 
 */
static void knn_routine(int k,double data_points[][5], int data_len, std::vector<DataPoint> & observations){

    kNN_classifier classifier(k,data_points,data_len);

    for(auto & obs : observations){
        classifier.classify(obs);
    }
    
}

/**
 * @brief It computes complex roots of a random quadratic equation . The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 */
extern "C" void knn(){
    std::vector<DataPoint> observations;
    
    for(int i=0;i<ARRAY_LENGTH;i++){
        observations.emplace_back(random_get(),random_get(),random_get(),random_get());
    }
    MEASURE_START();
    
    knn_routine(5,iris_dataset,IRIS_DATASET_LEN,observations);
    
    MEASURE_STOP();

    
}