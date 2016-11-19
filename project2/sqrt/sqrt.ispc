
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

// start and stop are inclusive
task void do_sqrt(uniform int N, uniform float x[],
				uniform float out[], uniform int space){

	uniform int start = taskIndex * space;
	uniform int stop = min((taskIndex+1) * space, (unsigned int) N);
	foreach(i = start ... stop){
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

export void sqrt_ispc_task(uniform int N, uniform float x[],
							 uniform float out[], uniform int num_tasks){
	uniform int space = N / num_tasks + 1;
	if (N <= num_tasks){
		launch[1] do_sqrt(N, x, out, num_tasks);
	}
	else {
		launch[num_tasks] do_sqrt(N, x, out, space);
	}
}