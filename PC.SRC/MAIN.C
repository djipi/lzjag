/*
 * LZJAG.EXE - Data Compression Utility for Jaguar
 * Copyright ©1994 Atari Corp.
 * All Rights Reserved
 * Written by Scott Sanders
 *
 *    Module: MAIN.C - Primary Source Routines
 * Revisions: 9/19/94 - SDS: Created 
 */                                              
 
//#include <DOS.H>
#include <STDIO.H>
#include <STDLIB.H>
#include <STRING.H>
#include "bitio.h"
#include "dirent.h"

#define FILE_NO_ERROR		0
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

int main( int argc, char *argv[] )
{       
	FILE *fp;
	BIT_FILE *bfp;
 	short i;
	//unsigned ret = 0;
 	//char *cp;

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

	char* f;
	while ((f = strrchr(filename, '\\')) ? (*f = '/') : 0);

	DIR* dirinfo = opendir( filename);
	if (dirinfo)
	{
		struct dirent* fileinfo;

		while ((fileinfo = readdir(dirinfo)))
		{
			if (strcmp(fileinfo->d_name, ".") && strcmp(fileinfo->d_name, ".."))
			{
				strcpy(infile, filename);
				sprintf(infile, "%s/%s", filename, fileinfo->d_name);

				sprintf(outfile, "%s/%s", filename, fileinfo->d_name);
				char* cp = strrchr(outfile, '.');
				cp ? (*cp = 0) : 0;

				switch (op)
				{
				case COMPRESS:
					if (!strrchr(fileinfo->d_name, '.LZJ'))
					{
						printf("Compressing: %s ", fileinfo->d_name);

						strcat(outfile, ".LZJ");
						fp = fopen(infile, "rb");
						if (fp == NULL)
							return error(FILE_OPEN_ERR);

						bfp = OpenOutputBitFile(outfile);
						if (bfp == NULL)
							return error(FILE_WRITE_ERR);
						CompressFile(fp, bfp);
						CloseOutputBitFile(bfp);
						fclose(fp);
						printf("\n");
					}
					break;

				case EXPAND:
					if (strrchr(fileinfo->d_name, '.LZJ'))
					{
						printf("Expanding: %s ", fileinfo->d_name);

						strcat(outfile, ".BIN");
						fp = fopen(outfile, "wb");
						if (fp == NULL)
							return error(FILE_OPEN_ERR);

							bfp = OpenInputBitFile(infile);
							if (bfp == NULL)
								return error(FILE_WRITE_ERR);

						ExpandFile(bfp, fp);
						CloseInputBitFile(bfp);
						fclose(fp);

						printf("\n");
					}
					break;
				}
			}
		}

		closedir(dirinfo);

		printf("Operation Successful!\n");

		return FILE_NO_ERROR;
	}
}     

void banner( void )
{
	printf( "\n                ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿\n" );
	printf( "                ³  Jaguar LZSS Compression Utility  ³\n" );
	printf( "                ³   Copyright (c)1994 Atari Corp.   ³\n" );
	printf( "                ³            Version: 1.0           ³\n" );
	printf( "                ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ\n\n" );
}                                                                  

void usage( void )
{
	printf( "ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿\n" );
	printf( "³                                  USAGE                                     ³\n" );
	printf( "ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ\n\n" );
	
	printf( "lzjag directory -c|-x\n\n" );
	printf( "     -c: Compress each file, found in the directory, to .LZJ files.\n" );
	printf( "     -x: Expand each LZJ's files, found in the directory, to .BIN files.\n" );
}               

short error( short err )
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
		default:
			printf("Error unknown.\n");
			break;
	}
	return err;
}
