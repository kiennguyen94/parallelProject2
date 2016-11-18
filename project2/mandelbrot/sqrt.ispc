
export void sqrt_ispc(uniform int N, uniform float x[], uniform float out[]){
	foreach(i = 0 ... N){
		float n = x[i];
	    if(n == 0 || n == 1) out[i] = n ; 
	    double pres = 0.0001 ;
	    double x_0 = n/4 ; 
	    double x_1 = x_0 - (((x_0*x_0)-n)/(2*x_0)) ; 
	    while((x_1 - x_0 > pres) || (x_1 - x_0 < -pres)){
	        x_0 = x_1 ;
	    	x_1 = x_0 - (((x_0*x_0)-n)/(2*x_0)) ; 
	    }
	    out[i] = x_1;
	}
}