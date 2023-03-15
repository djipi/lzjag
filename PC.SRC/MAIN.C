/*
 * LZJAG.EXE - Data Compression Utility for Jaguar
 * Copyright ©1994 Atari Corp.
 * All Rights Reserved
 * Written by Scott Sanders
 *
 *    Module: MAIN.C - Primary Source Routines
 * Revisions: 9/19/94 - SDS: Created 
 */                                              
 
#include <DOS.H>
#include <STDIO.H>
#include <STDLIB.H>
#include <STRING.H>
#include "bitio.h"

#define NO_ERROR		0
#define FILE_OPEN_ERR		1
#define FILE_READ_ERR		2
#define FILE_WRITE_ERR		3

#define UNKNOWN			0
#define COMPRESS		1
#define EXPAND			2
                        
/* Global Variables */
char filename[200];                                                                           
char infile[200];
char outfile[200];
short op = UNKNOWN;

/* Prototypes */
int main( int, char *[] );
void banner( void );
void usage( void );
short error( short );

extern void CompressFile( FILE *, BIT_FILE * );
extern void ExpandFile( BIT_FILE *, FILE * );           

int
main( int argc, char *argv[] )
{       
	FILE *fp;
	BIT_FILE *bfp;
 	short i;
	unsigned ret;
 	char *cp;
	struct _find_t fileinfo;

	filename[0] = 0;
	
 	banner();
 	if(argc != 3)    
 	{
 		usage(); 
 		return 0;
 	}
 	
	for( i = 1; i < argc; i++ )
	{
		if( argv[i][0] == '-' || argv[i][0] == '/' )
		{                                   
			switch( argv[i][1] )
			{       
				case '?':
				case 'h':
				case 'H':
					usage();
					return 0;
					break;
				case 'x':
				case 'X':
					if(op != UNKNOWN)
					{
						usage();
						return(0);
					}
					op = EXPAND;
					break;
				case 'c':
				case 'C':
					if(op != UNKNOWN)
					{
						usage();
						return(0);
					}
					op = COMPRESS;
					break;
				default:      
					usage();
					return 0;
					break;
			}
		}
		else
		{
			if(filename[0] != 0)	/* Two filenames */
			{
				usage();
				return 0;
			}
			
			strcpy( filename, argv[i] );
		}	
	}                                
	                    
	if( filename[0] == 0 || op == UNKNOWN )
	{
		usage();
		return 0;
	}

	ret = _dos_findfirst( filename, _A_RDONLY, &fileinfo );
	while( !ret )
	{
		if( op == COMPRESS )
		{
			printf( "Compressing: %s ", fileinfo.name );
			strcpy( infile, filename );
			cp = strrchr( infile, '\\' );
			if( cp )
				cp++;
			else
				cp = infile;

			strcpy( cp, fileinfo.name ); 
			strcpy( outfile, infile );
			cp = strrchr( outfile, '.' );
			if( cp )
				*cp = 0;
			else
				cp = infile;

			strcpy( cp, ".LZJ" );
                                        
                        fp = fopen( infile, "rb" );
			if( fp == NULL )
				return error( FILE_OPEN_ERR );
                                         
                        bfp = OpenOutputBitFile( outfile );
			if( bfp == NULL )
				return error( FILE_WRITE_ERR );
			CompressFile( fp, bfp );               
			CloseOutputBitFile( bfp );
			fclose( fp );
			printf( "\n" ); 
		}
		else
		{                      
			printf( "Expanding: %s ", fileinfo.name );
			strcpy( infile, filename );
			cp = strrchr( infile, '\\' );
			if( cp )
				cp++;
			else
				cp = infile;

			strcpy( cp, fileinfo.name ); 
			strcpy( outfile, infile );
			cp = strrchr( outfile, '.' );
			if( cp )
				*cp = 0;
			else
				cp = infile;

			strcpy( cp, ".BIN" );
                                          
			fp = fopen( outfile, "wb" );
			if( fp == NULL )
				return error( FILE_OPEN_ERR );

			bfp = OpenInputBitFile( infile );
			if( bfp == NULL )
				return error( FILE_WRITE_ERR );

			ExpandFile( bfp, fp );
			CloseInputBitFile( bfp );
			fclose( fp );
			
			printf( "\n" );
		}
		ret = _dos_findnext( &fileinfo );		
	}                              
	printf( "Operation Successful!\n" );
            
	return NO_ERROR;
}     

void
banner( void )
{
	printf( "\n                ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿\n" );
	printf( "                ³  Jaguar LZSS Compression Utility  ³\n" );
	printf( "                ³   Copyright (c)1994 Atari Corp.   ³\n" );
	printf( "                ³            Version: 1.0           ³\n" );
	printf( "                ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ\n\n" );
}                                                                  

void
usage( void )
{
	printf( "ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿\n" );
	printf( "³                                  USAGE                                     ³\n" );
	printf( "ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ\n\n" );
	
	printf( "lzjag filename -c|-x\n\n" );
	printf( "     -c: Compress filename to .LZJ\n" );
	printf( "     -x: Expand filename to .BIN\n" );
}               

short
error( short err )
{                  
	switch( err )
	{
		case FILE_OPEN_ERR:
			printf( "Error attempting to open %s.\n", filename );
			break;
		case FILE_READ_ERR:
			printf( "Error reading %s.\n", filename );
			break;
		case FILE_WRITE_ERR:
			printf( "Error writing %s.\n", filename );
			break;
		case NO_ERROR:
			break;
	}
	return err;
}               