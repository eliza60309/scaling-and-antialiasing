//*****************************************************************************
//
// Image.cpp : Defines the class operations on images
//
// Author - Parag Havaldar
// Code used by students as starter code to display and modify images
//
//*****************************************************************************

#include "Image.h"
#include <iostream>
#include <cmath>
using namespace std;

// Constructor and Desctructors
MyImage::MyImage() 
{
	Data = NULL;
	Width = -1;
	Height = -1;
	ImagePath[0] = 0;
}

MyImage::~MyImage()
{
	if ( Data )
		delete []Data;
}


// Copy constructor
MyImage::MyImage( MyImage *otherImage)
{
	Height = otherImage->Height;
	Width  = otherImage->Width;
	Data   = new char[Width*Height*3];
	strcpy(otherImage->ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage->Data[i];
	}


}



// = operator overload
MyImage & MyImage::operator= (const MyImage &otherImage)
{
	Height = otherImage.Height;
	Width  = otherImage.Width;
	if (Data)
		delete[]Data;
	Data   = new char[Width*Height*3];
	strcpy( (char *)otherImage.ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage.Data[i];
	}
	
	return *this;

}


// MyImage::ReadImage
// Function to read the image given a path
bool MyImage::ReadImage()
{
	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
		return false;
	}

	// Create a valid output file pointer
	FILE *IN_FILE;
	IN_FILE = fopen(ImagePath, "rb");
	if ( IN_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	for (i = 0; i < Width*Height; i ++)
	{
		Rbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Gbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Bbuf[i] = fgetc(IN_FILE);
	}
	
	// Allocate Data structure and copy
	Data = new char[Width*Height*3];
	for (i = 0; i < Height*Width; i++)
	{
		Data[3*i]	= Bbuf[i];
		Data[3*i+1]	= Gbuf[i];
		Data[3*i+2]	= Rbuf[i];
	}

	// Clean up and return
	delete []Rbuf;
	delete []Gbuf;
	delete []Bbuf;
	fclose(IN_FILE);

	return true;

}



// MyImage functions defined here
bool MyImage::WriteImage()
{
	// Verify ImagePath
	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *OUT_FILE;
	OUT_FILE = fopen(ImagePath, "wb");
	if ( OUT_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Writing");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	for (i = 0; i < Height*Width; i++)
	{
		Bbuf[i] = Data[3*i];
		Gbuf[i] = Data[3*i+1];
		Rbuf[i] = Data[3*i+2];
	}

	
	// Write data to file
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Rbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Gbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Bbuf[i], OUT_FILE);
	}
	
	// Clean up and return
	delete []Rbuf;
	delete []Gbuf;
	delete []Bbuf;
	fclose(OUT_FILE);

	return true;

}




// Here is where you would place your code to modify an image
// eg Filtering, Transformation, Cropping, etc.
bool MyImage::Modify()
{

	// TO DO by student
	
	// sample operation
	for ( int i=0; i<Width*Height; i++ )
	{
		Data[3*i] = 0;
		Data[3*i+1] = 0;
	}

	return false;
}

int MyImage::Resize(long double scale)
{
	if (!scale)
		return 0;
	char* newData = new char[Width * scale * Height * scale * 3];
	char* oldData = Data;
	int newHeight = Height * scale;
	int newWidth = Width * scale;
	for (int i = 0; i < newHeight; i++)
	{
		for (int j = 0; j < newWidth; j++)
		{
			newData[3 * ((i * newWidth) + j)] = oldData[3 *((int)(i / scale) * Width + (int)(j / scale))];
			newData[3 * ((i * newWidth) + j) + 1] = oldData[3 * ((int)(i / scale) * Width + (int)(j / scale)) + 1];
			newData[3 * ((i * newWidth) + j) + 2] = oldData[3 * ((int)(i / scale) * Width + (int)(j / scale)) + 2];
		}
	}
	delete []Data;
	Data = newData;
	Height = newHeight;
	Width = newWidth;
	return 0;
}

int MyImage::Antialias(int level)
{
	char* source = Data;
	unsigned int* newdata = new unsigned int[3 * Height * Width];
	unsigned short* weight = new unsigned short[Height * Width];
	memset(weight, 0, Height * Width * sizeof(unsigned short));
	memset(newdata, 0, 3 * Height * Width * sizeof(unsigned int));
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			for (int k = i - level; k <= i + level; k++)
			{
				if (k < 0)continue;
				if (k >= Height)continue;
				for (int l = j - level; l <= j + level; l++)
				{
					if (l < 0)continue;
					if (l >= Width)continue;
					newdata[3 * ((i * Width) + j)] += (unsigned char)Data[3 * ((k * Width) + l)];
					newdata[3 * ((i * Width) + j) + 1] += (unsigned char)Data[3 * ((k * Width) + l) + 1];
					newdata[3 * ((i * Width) + j) + 2] += (unsigned char)Data[3 * ((k * Width) + l) + 2];
					weight[(i * Width) + j]++;
				}
			}
		}
	}
	Data = new char[Width * Height * 3];
	for (int i = 0; i < Width * Height; i++)
	{
		Data[3 * i] = newdata[3 * i] / weight[i];
		Data[3 * i + 1] = newdata[3 * i + 1] / weight[i];
		Data[3 * i + 2] = newdata[3 * i + 2] / weight[i];
	}
	delete []newdata;
	delete []weight;
	return 0;
}

int MyImage::SampleImg(int sampleid, int args)
{
	switch (sampleid)
	{
		case 1://black white stripes, args = stripe width
		{
			if (!args)
				args = 1;
			bool white = false;
			for (int i = 0; i < getHeight(); i++)
			{
				for (int j = 0; j < getWidth(); j++)
				{
					if (j % args == 0)
						white = !white;
					if (white)
					{
						getImageData()[3 * ((i * getWidth()) + j)] = 255;
						getImageData()[3 * ((i * getWidth()) + j) + 1] = 255;
						getImageData()[3 * ((i * getWidth()) + j) + 2] = 255;
					}
					else
					{
						getImageData()[3 * ((i * getWidth()) + j)] = 0;
						getImageData()[3 * ((i * getWidth()) + j) + 1] = 0;
						getImageData()[3 * ((i * getWidth()) + j) + 2] = 0;
					}
				}
			}
			break;
		}
		case 2://white box at the corner, args = edge length;
		{
			for (int i = 0; i < getHeight(); i++)
			{
				for (int j = 0; j < getWidth(); j++)
				{
					if (i < args && j < args)
					{
						getImageData()[3 * ((i * getWidth()) + j)] = 255;
						getImageData()[3 * ((i * getWidth()) + j) + 1] = 255;
						getImageData()[3 * ((i * getWidth()) + j) + 2] = 255;
					}
					else
					{
						getImageData()[3 * ((i * getWidth()) + j)] = 0;
						getImageData()[3 * ((i * getWidth()) + j) + 1] = 0;
						getImageData()[3 * ((i * getWidth()) + j) + 2] = 0;
					}
				}
			}
			break;
		}
		default:
			break;
	}
	return 0;
}

int MyImage::Magnify(int x, int y, int size, double scale, MyImage &background, MyImage &filter)
{
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			if (sqrt((i - y) * (i - y) + (j - x) * (j - x)) <= size * scale)
			{
				if (j - x + (int)(x * scale) < 0 || j - x + (int)(x * scale) >= Width * scale || i - y + (int)(y * scale) < 0 || i - y + (int)(y * scale) >= Height * scale)
				{
					Data[3 * ((i * Width) + j)] = 0;
					Data[3 * ((i * Width) + j) + 1] = 0;
					Data[3 * ((i * Width) + j) + 2] = 0;
				}
				else
				{
					Data[3 * ((i * Width) + j)] = filter.getImageData()[3 * ((int)(y * scale) * (int)(Width * scale) + (int)(x * scale) + ((i - y) * (int)(Width * scale) + (j - x)))];
					Data[3 * ((i * Width) + j) + 1] = filter.getImageData()[3 * ((int)(y * scale) * (int)(Width * scale) + (int)(x * scale) + ((i - y) * (int)(Width * scale) + (j - x))) + 1];
					Data[3 * ((i * Width) + j) + 2] = filter.getImageData()[3 * ((int)(y * scale) * (int)(Width * scale) + (int)(x * scale) + ((i - y) * (int)(Width * scale) + (j - x))) + 2];
				}
			}
			else
			{
				Data[3 * ((i * Width) + j)] = background.getImageData()[3 * ((i * Width) + j)];
				Data[3 * ((i * Width) + j) + 1] = background.getImageData()[3 * ((i * Width) + j) + 1];
				Data[3 * ((i * Width) + j) + 2] = background.getImageData()[3 * ((i * Width) + j) + 2];
			}
		}
	}
	return 0;
}

int MyImage::Blackalize(double fraction)
{
	for (int i = 0; i < 3 * Width * Height; i++)
		Data[i] = (unsigned char)Data[i] / fraction;
	return 0;
}