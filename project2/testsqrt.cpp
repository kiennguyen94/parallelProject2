#include <iostream>
#include <cstdlib>
using namespace std;

double sqrt(double n)
{
    if(n == 0 || n == 1) return n ; 
    double pres = 0.0001 ;
    double x_0 = n/4 ; 
    double x_1 = x_0-   (((x_0*x_0)-n)/(2*x_0)) ; 
    while((x_1 - x_0 > pres) || (x_1 - x_0 < -pres)){
        x_0 = x_1 ;
    x_1 = x_0-   (((x_0*x_0)-n)/(2*x_0)) ; 
    }
    return x_1 ; 
}
int main(int argc, char** argv) {
    for(float i = 1 ; i < 15 ; i+=1){
        // float z = i*i ; 
        cout<<"sqrt["<<i<<"] = "<<sqrt(3.3345) << endl ; 
    }
    return 0;
}