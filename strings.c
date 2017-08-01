// Elaheh Rashedi



// CAUTIONS: the lenght of s1 should be devisible by p
// this program works fine with any number of processors

// the printf is used for debugging

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int match(char *a,char *b,int pos);

int main(int argc, char *argv[])
{

	// I used static memory allocation here
	int n1 = 1000 , n2 = 1000; 

	char * s1,*s2;

	int  nprocs; 
	int total_num=0; // for saving total number of matches
	int temp_num=0; // for saving temporal number of matches
	int p,pid ;
	int tagA=0 ; // these are the tags that I used for sending and receiving data
	int tagB=1 ;
	int tagC=2 ;
	int tagN1= 3 ;
	int tagN2= 4 ;


	MPI_Init(&argc,&argv);

	MPI_Comm_size(MPI_COMM_WORLD, &p); // obtaining number of processors
	MPI_Comm_rank(MPI_COMM_WORLD, &pid); // obtaining number of PID
        
	MPI_Status status;

	if (pid ==0 ) 
	{

		FILE *fp = fopen("strings.txt","r"); 
		s1 = (char*) malloc( n1 * sizeof( char* ));
		s2 = (char*) malloc( n2 *sizeof( char* ));	
		fgets( 	s1 , n1 , fp );
		fgets( s2, n2, fp );
 		n1= strlen(s1) - 1 ; // omit null from the lenght
 		n2= strlen(s2) - 1 ; // omit null from the lenght

		printf ( "n1 is : %d and n2 is %d and p is %d \n" , n1 , n2 , p ) ;
		for ( int dest = 1 ; dest < p ; dest ++ ) // p0 send data to all other processors
		{
			// first, i send the lenghts of s1 and s2
			MPI_Send(& n1 , 1   , MPI_INT, dest , tagN1, MPI_COMM_WORLD);
			MPI_Send(& n2 , 1   , MPI_INT, dest , tagN2, MPI_COMM_WORLD);
			
			// the lenght of the last processor is different from the others
			if (dest==p-1) 
				MPI_Send(& s1[n1/p*dest], n1 / p   , MPI_INT, dest , tagA, MPI_COMM_WORLD);
			else
				MPI_Send(& s1[n1/p*dest], n1 / p + n2-1  , MPI_INT, dest , tagA, MPI_COMM_WORLD);
			
			// the lenght of s2 is the same for all processors
			MPI_Send(& s2[0] , n2 , MPI_INT, dest , tagB, MPI_COMM_WORLD);



			
		}
		
		for ( int dest = 1 ; dest < p ; dest ++ ) 
		{
			// processor p0 collect all data from other processors, in a loop
			MPI_Recv(& temp_num, 1, MPI_INT,dest  , tagC, MPI_COMM_WORLD, &status);
			total_num +=temp_num;
		}

		// processor p0 should do the matches, like the others
		for (int pos=0; pos < n1/p ; pos++)
			total_num += match(s1,s2, pos);

		printf ("total number of matches is : %d \n" , total_num ) ;

		// p0 prints the total number of matches in a file
		FILE *out = fopen("strings_result.txt","w");
		if (out == NULL)
		{
    			printf("Error opening file!\n");
    			exit(1);
		}
		fprintf ( out , "Total match is %d \n " ,  total_num ) ; 
		fprintf ( out , "Thank you ") ;
		fclose ( out) ;
				
		
	}
	// all other processors should do different calculations	
	if (pid!=0)
	{
		
		//printf ( "pid is : %d \n " , pid ) ;
		// first, they receive the lenght of s1 and s2, from p0
		MPI_Recv(& n1 , 1, MPI_INT, 0  , tagN1, MPI_COMM_WORLD, &status);
		MPI_Recv(& n2 , 1, MPI_INT, 0  , tagN2, MPI_COMM_WORLD, &status);

		
				
		// the size of the last array for the last procesor is different from the others
		if (pid==p-1) 	
		{
			s1 = (char*) malloc( n1/p * sizeof( char* ));
			MPI_Recv(&s1[0] , n1 / p , MPI_INT, 0, tagA, MPI_COMM_WORLD, &status);
			s1[n1 / p] = '\0' ;
		}
		else 	
		{
			// all other processors (not p0 and p(p-1)) receive s1 with the same lenght
			s1 = (char*) malloc( (n1/p+n2-1) * sizeof( char* ));
			MPI_Recv(&s1[0] , n1 / p + n2 -1 , MPI_INT, 0, tagA, MPI_COMM_WORLD, &status);
			s1[n1 / p + n2 - 1] = '\0' ;
		}
		
		// the lenght of s2 is equal for all
		s2 = (char*) malloc( n2 *sizeof( char* ));
		MPI_Recv(&s2[0] ,n2, MPI_INT, 0, tagB, MPI_COMM_WORLD, &status);
		
		int lenght = strlen ( s1 ) ;
		printf ( "lenght is : %d \n " , lenght ) ;
		for (int pos=0; pos<= lenght ; pos++)
				temp_num += match(s1,s2, pos);

		// here, i send the temporal number of matches fro all processors
		MPI_Send(& temp_num, 1 , MPI_INT, 0, tagC, MPI_COMM_WORLD);
			

	}


	// finalizing the MPI
	MPI_Finalize();

	return 0;

}
int match (char *a,char *b,int pos)
{

 	int i = 0; 
	//printf (" string a : %s anb b : %s and pos : %d \n" , a , b , pos) ;

	int lenghtb = strlen (b) - 1 ;
	int num = 0 ;
	for (int i =0 ; i < lenghtb ; i++ )
		if ( a[pos+i] != '\0'  && b[i] != '\0' && a[pos+i] == b[i] )
			num ++ ;
	//printf ( "lenghtb is : %d and matches are : %d \n" , lenghtb , num );	
	if ( num == lenghtb )
		return 1;
	else
		return 0;



}