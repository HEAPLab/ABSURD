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


#include <cmath>
#include <vector>
#include <functional>
#include <algorithm>
#include <random>

#define RESULT 2.38629

MEASURE_GLOBAL_VARIABLES()



class ZO_AdaMM_Optimizer{

    public:
    ZO_AdaMM_Optimizer(std::vector<double> & x,
                       std::function<double(std::vector<double>&)>  process,
                           double beta_1=0.9,
                           double beta_2=0.3,
                           double mu=0.0001,
                           double lr=0.05,
                           int q=10,
                           int epoch=2*ITERATIONS):
        process(process),
        beta_1(beta_1),
        beta_2(beta_1),
        mu(mu),
        lr(lr),
        q(q),
        epoch(epoch),
        v(x.size(),0.0000001),
        v_hat(x.size(),0.0000001),
        m(x.size()),
        distribution(0,100)
        
    {
        std::random_device rd{};
        gen=std::mt19937{rd()};
    }
    
    void optimize( std::vector<double> &x){
            
            for(int i=0;i<epoch;i++){
                std::vector<double> grad(x.size());
                gradient_estimation(x,grad);
                for(int j=0;j<x.size();j++){
                    m[j]=beta_1*m[j]+(1-beta_1)*grad[j];
                    v[j]=beta_2*v[j]+(1-beta_2)*grad[j]*grad[j];
                    v_hat[j]=std::max(v[j],v_hat[j]);
                    x[j]=x[j] - lr * m[j]/sqrt(v_hat[j]);
                    
                }
                
            }
        }
    private:

        void get_direction( std::vector<double> &x, std::vector<double> &u){
           
            double norm=0;

            for(int i=0;i<x.size();i++){
                auto r=distribution(gen);
                u.push_back(r);
                norm+=(r*r);
            }
            
            norm=sqrt(norm);
            for(int i=0;i<x.size();i++){
                u[i]=u[i]/norm;
            }

        }
        void gradient_estimation(std::vector<double> &x, std::vector<double> &grad){
            
            double f_0=process(x);
            
            for(int i=0;i<q;i++){
                std::vector<double> u;
                get_direction(x,u);
                std::vector<double> x_tmp(x.size());
                for(int j=0;j<x.size();j++){
                    
                    x_tmp[j]= x[j]+mu*u[j];
                }
                double f_tmp=process(x_tmp);

                for(int j=0;j<x.size();j++){
                    grad[j]= grad[j]+((x.size()*(f_tmp-f_0)*u[j])/(q*mu));
                }
            }
            
        }

        
        std::function<double(std::vector<double>&)> process;
        double beta_1;
        double beta_2;
        
        double mu;
        
        double lr;
        int q;
        int epoch;

        std::vector<double> v_hat;
        std::vector<double> v;
        std::vector<double> m;


        std::mt19937 gen;
        std::normal_distribution<double> distribution;

};

double f(std::vector<double> &x){
    return x[0]*x[0]+x[1]*x[1]+4*exp(-x[0]*x[0]-x[1]*x[1]);
}
/**
 * @brief Actual ZO-AdaMM implementation
 */
static void zo_adamm_routine(std::vector<double> &x){
    
    ZO_AdaMM_Optimizer optimizer(x,f);
    optimizer.optimize(x);
}

extern "C" void zo_adamm(){

    std::vector<double> x(2,0);
    
    MEASURE_START();
    
    zo_adamm_routine(x);
    
    MEASURE_STOP();

    CHECK_RESULT(std::abs(f(x)-RESULT)<CLASS_PRECISION);

}
