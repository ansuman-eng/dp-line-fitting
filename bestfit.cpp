/**
* @file         bestfit.cpp
* @brief        Basic functionality.
* @detail       This file has all the classes and functions needed to find the bestfit line.
                It is called by the Python script everytime the slider is changed.
* @author       Ansuman
*/

#include <bits/stdc++.h> 
using namespace std; 
#include <iostream>
#include <string>

/**
@brief A GLOBAL VARIABLE HOLDING THE NUMBER OF POINTS
*/
int num_points;

/**
@brief A GLOBAL VARIABLE TO STORE THE SIZE OF DATASET
*/
int D_size;
/**
@brief A GLOBAL VARIABLE FOR VALUE OF PENALTY C
*/
float c;
/**
@brief A GLOBAL POINTER TO STORE THE 1-D ARRAY FOR DP
*/
float * OP;
/**
@brief A POINTER TO STORE THE ARRAY FOR THE PATH RECONSTRUCTION
*/
int * minarr;


/**
* @class   point
* @brief   A class for point
* @detail  The coordinates and a constructor
*/
class point 
{ 
    public: 
  
    float x,y; 
  
  	point(float a , float b)
  	{
    /**
    * @brief  Intialize the values for point
    */  
  		x = a;
  		y = b;
	  }
}; 

/**
* @class   errorxy
* @brief   A class for error
* @detail  The slope,intercept and error values
*/
class errorxy
{
	public:
		
		float slope , intercept , e;
};

/**
@brief A GLOBAL POINTER TO POINTERS TO FORM THE 2-D ARRAY
*/
errorxy ** perr;


/**
* @class   Dataset
* @brief   A class for the dataset
* @detail  A constructor to initialize with number of points and an array to store points.
*/
class Dataset 
{ 
    public: 
  
    int index=0;
    point ** arr;
    
    Dataset(){
    /**
    * @brief  Defines an array of required size
    */  
    arr = (point ** )malloc(num_points * sizeof(point*));
	}
    
  
    void new_point(float x,float y) 
    { 
    /**
    * @brief Adds a point to the dataset
    */  
    	point *newpt;
    	newpt = new point(x,y);
    	arr[index]=newpt;
    	index++;
    } 
}; 


/*CALCULATE THE BEST FIT LINE BETWEEN ANY TWO POINTS*/
void err(Dataset* temp)
{
    /**
    * @brief  Utility function for finding out the error of best fit line between any two points
    *         , Output = A 2d matrix with error values
    */  
	for(int i = 0 ; i < temp->index;i++)
	{
		for(int j = i ; j < temp->index; j++)
		{
			float sumx,sumx2,sumy,sumxy;
			int n = j-i+1;
			sumx=sumx2=sumy=sumxy=0;
			for(int p=i;p<=j;p++)
    		{
    			point *p1 = temp->arr[p];
    			float x = p1->x;
    			float y = p1->y;
    	    	sumx=sumx +x;
        		sumx2=sumx2 +x*x;
	       		sumy=sumy +y;
    	    	sumxy=sumxy +x*y;
    		}
    		//printf("%f %f %f\n", sumx , sumx2 , sumxy);
    		float a=((n*sumxy-sumx*sumy)*1.0/(n*sumx2-sumx*sumx)*1.0);
    		float b=(sumy-a*sumx)/n;
    		float e=0;
    		//printf("%f %f\n",a,b);
			for(int p=i;p<=j;p++)
    		{
    			point *p1 = temp->arr[p];
    			float x = p1->x;
    			float y = p1->y;
    	    	e=e+(y-a*x-b)*(y-a*x-b);
    		}
    		//printf("%f \n",e);
    		errorxy * terror;
    		terror = new errorxy();
    		terror->slope=a;
    		terror->intercept=b;
    		terror->e=e;
    		perr[i][j]=*terror;
		}
	}
}

void readFile(Dataset* D)
{
    /*READ POINTS FROM THE FILE*/
    /**
    * @brief  Utility function to read the file and add points to the Dataset 
    *         
    */  
    fstream file; 
    string  word,points_file; 
    points_file = "input_points.txt"; 
    file.open(points_file.c_str()); 

    char char_array[70];

    while (getline(file,word)) 
    { 
        strcpy(char_array, word.c_str());
        
        char *token = strtok(char_array, ","); 
        float temp[2];
        int i =0;
        while (token != NULL) 
        { 
            temp[i]=atof(token);
            i++;
            token = strtok(NULL, ","); 
            
            
        } 
        D->new_point(temp[0],temp[1]);
        D_size++;
        
    }
}
int main(int argc, char** argv) 
{ 
    /*TAKE THE VALUE OF C AS COMMAND LINE ARGUMENT INPUT*/
    if(argc!=3)
    {
        printf("WRONG NUMBER OF ARGUMENTS\n");
        return 0;
    }
    
    /*FEED IN THE VALUES*/
    num_points = atoi(argv[2]);
    c = atof(argv[1]);
    
    /*READ POINTS INTO FILE*/
    Dataset *D;
    D = new Dataset();
    readFile(D);


    /*DECLARE THE ERROR 2-D MATRIX*/
	perr = (errorxy **)malloc(D_size* sizeof(errorxy *)); 
	for (int i=0; i<D_size; i++) 
    	perr[i] = (errorxy *)malloc(D_size * sizeof(errorxy));
    
    /*CALCULATE THE ERROR FOR EACH PAIR OF POINTS*/	
	err(D);
	
    /*MAKE ERRORS OF SAME POINTS TO BE ZERO*/
	for(int i = 0 ; i<D->index;i++)
		perr[i][i].e=0;            
		
		
    /*DEFINE THE DYNAMIC PROGRAMMING ARRAY*/
    OP = (float *)malloc(sizeof(float)*(D->index+1));
    for(int i = 0 ; i <=D->index;i++)
        OP[i]=-1;
    OP[0]=0;
    
    /*MINARR HELPS IN FINDING THE PATH*/
	minarr= (int *)malloc(sizeof(int)*(D->index+1));
	for(int i = 0 ; i <= D->index;i++)
	   minarr[i]=-1;
 
	

    //findsegJ(D->index);
    /*ITERATIVE DYNAMIC PROGRAMMING*/
    clock_t start,end;
    start = clock();
    for(int j=1;j<=D->index;j++)
        {
            int min=1000000;
            int min_idx=-1;
            for(int i=1;i<=j;i++)
            {
                if(OP[i-1]+c+perr[i-1][j-1].e<min)
                {
                    min=OP[i-1]+c+perr[i-1][j-1].e;
                    min_idx=i;
                }
            }
            OP[j]=min;
            //cout<<j<<" "<<min<<" "<<min_idx<<endl;
            minarr[j]=min_idx;
        }
    end = clock();
    std::ofstream outfile;
    outfile.open("time.txt", std::ios_base::app);  

    double time_taken = double(end - start) / double(CLOCKS_PER_SEC); 
    cout<<time_taken<<endl;
    printf("Writing into file\n");
    outfile << time_taken <<endl;
    outfile.close();

    int curr_point = D->index;

    ofstream op_points;
    op_points.open("output_points.txt");



    while(curr_point!=1)
    {
        //cout<<"CURRENT "<<curr_point<<" PREVIOUS "<<minarr[curr_point]<<endl;
        float f1,f2;
        f1=perr[minarr[curr_point]-1][curr_point-1].slope;
        f2=perr[minarr[curr_point]-1][curr_point-1].intercept;
        //cout<<"SLOPE "<<f1<<" INTERCEPT "<<f2<<endl;
        curr_point=minarr[curr_point];
        op_points<<f1<<" "<<f2<<endl;
    }


    return 0; 
    
} 
