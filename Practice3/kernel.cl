#pragma OPENCL EXTENSION cl_khr_fp64 : enable 

#define TAM_KERNEL 3

__constant double THRESHOLD =  150;

__constant double hori[TAM_KERNEL][TAM_KERNEL] = { 
{-1, 0, 1} , 
{-1, 0, 1} ,
{-1, 0, 1} 
};

__constant double vert[TAM_KERNEL][TAM_KERNEL] = { 
{-1, -1, -1} , 
{ 0,  0,  0} ,
{ 1,  1,  1} 
};

__kernel void edge_detector(                                              
	__global double* input,                                           
	__global double* output,                                          
	const unsigned int width,                                         
	const unsigned int height)                                        
{                                                                    
	int w = get_global_id(0);  //imageSizeWidth                                        
	int h = get_global_id(1);  //imageSizeHeight
   
	int hg = 0;
	int vg = 0;

	// Compute horizontal gradient
	for (int x = w;  x < w + TAM_KERNEL; x++)
	{
		for (int y = h; y < h + TAM_KERNEL; y++)
		{
			hg += input[x*height + y] * hori[x-w][y-h];
		}
	}

	// Compute vertical gradient
	for (int x = w;  x < w + TAM_KERNEL; x++)
	{
		for (int y = h ; y < h + TAM_KERNEL; y++)
		{
			vg += input[x*height + y] * vert[x-w][y-h];
		}
	}
	
	if (hg < 0)
	{
		hg = hg * -1;
	}

	if (vg < 0)
	{
		vg = vg * -1;
	}
	
	if (hg + vg > THRESHOLD){
		output[(w)*height + (h)] = 255; // White
	}else{
		output[(w)*height + (h)] = 0; // Black
	}
}                                                                  
