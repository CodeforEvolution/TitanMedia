#include "EMVideoBitmapScaler.h"
//#include "EMGlobals.h"


EMVideoBitmapScaler::EMVideoBitmapScaler()
{
	m_opDestinationBitMap = NULL;
	m_vLocalHeight = 0;
	m_vLocalWidth = 0;
	m_vLocalSourceHeight = 0;
	m_vLocalSourceWidth = 0;
}

EMVideoBitmapScaler::~EMVideoBitmapScaler()
{
}

void* EMVideoBitmapScaler::Scale(void *src, EMMediaFormat *srcFormat, EMMediaFormat *destFormat)
{
	unsigned long *spixptr, *spix1, *spix2;
	unsigned char *dpixptr, *dpix;
	unsigned long slb, dlb;
	int64 i, j; //Register until here
	float p, q, p1, q1;
	float src_y_f;
	int64 src_y_i, src_x_i;
	unsigned char *a, *b, *c, *d; //was also registered
	float rred, rgreen, rblue;
	float yfac_inv;
	float transparency;
	


	//TODO: EM_new calls possible memory leaks! Fix!!? ... Don't new-delete all the time either!



	long srows, scols;
	float xFactor, yFactor = 0;
	float x_f;
	float xfac_inv;

	srows = srcFormat -> m_vHeight;//src->Bounds().IntegerHeight()-1; //ALIENS FILMEN
	scols = srcFormat -> m_vWidth;//src->Bounds().IntegerWidth()-1; //ALIENS FILMEN
	m_vDcols = destFormat -> m_vWidth;//*format.u.raw_video.display.bytes_per_row;
	m_vDrows = destFormat -> m_vHeight;
/*	;//cout_commented_out_4_release<<"srows"<<srows<<endl;
	;//cout_commented_out_4_release<<"scols"<<scols<<endl;
	;//cout_commented_out_4_release<<"m_vDcols"<<m_vDcols<<endl;
	;//cout_commented_out_4_release<<"m_vDrows"<<m_vDrows<<endl;
*/

//	if(m_opDestinationBitMap == NULL)
//	{
		if(	srows == m_vDrows && scols == m_vDcols)
			return NULL;
//	}
	
	yFactor=((float)m_vDrows)/(float)srows;
		
	xFactor=(float)m_vDcols/(float)scols; //This row is non proportinal


	m_vxfac = xFactor;
	m_vyfac = yFactor;	

	
	xfac_inv = 1.0 / m_vxfac;


	if(m_opDestinationBitMap == NULL)
	{
		m_vLocalHeight = m_vDrows;
		m_vLocalWidth = m_vDcols;
		m_vLocalSourceHeight = srows;
		m_vLocalSourceWidth = scols;
		int32 vSizeOfDestinationClipInBytes = m_vDcols*4*m_vDrows;
		m_opDestinationBitMap = EM_new unsigned char[vSizeOfDestinationClipInBytes];

		// Allocate buffer for storing the values of ma and m1
		m_opR = EM_new EMRowValues* [m_vDcols];
		m_opRptr = m_opR;
		for(int32 vIndex = 0; vIndex < m_vDcols; vIndex++)
		{
			m_opR[vIndex]	= EM_new EMRowValues();
		}

		// Fill up the buffer once, to be used for each row
		for (i=0; i < m_vDcols; i++)
		{
			x_f = i*xfac_inv;
			m_opRptr[i]->x_i = (long)x_f;
			m_opRptr[i]->p = x_f - (float) m_opRptr[i]->x_i;
			m_opRptr[i]->p1 = 1.0 - m_opRptr[i]->p;
			//m_opRptr++;
		}
	}
	else if(m_vLocalHeight != m_vDrows || m_vLocalWidth != m_vDcols || m_vLocalSourceHeight != srows || m_vLocalSourceWidth != scols)
	{
		delete [] m_opDestinationBitMap;
		m_opDestinationBitMap = EM_new unsigned char[m_vDcols*4*m_vDrows];

		for(int32 vIndex = 0; vIndex < m_vLocalWidth; vIndex++)
		{
			delete m_opR[vIndex];
		}
		delete [] m_opR;
		// Allocate buffer for storing the values of ma and m1
		m_opR = EM_new EMRowValues* [m_vDcols];
		m_opRptr = m_opR;
		for(vIndex = 0; vIndex < m_vDcols; vIndex++)
		{
			m_opR[vIndex]	= EM_new EMRowValues();
		}

		// Fill up the buffer once, to be used for each row
		for (i=0; i < m_vDcols; i++)
		{
			x_f = i*xfac_inv;
			m_opRptr[i]->x_i = (long)x_f;
			m_opRptr[i]->p = x_f - (float) m_opRptr[i]->x_i;
			m_opRptr[i]->p1 = 1.0 - m_opRptr[i]->p;
			//m_opRptr++;
		}
		m_vLocalHeight = m_vDrows;
		m_vLocalWidth = m_vDcols;
		m_vLocalSourceHeight = srows;
		m_vLocalSourceWidth = scols;
	}

//	void* dest = m_opDestinationBitMap;
		
	
	yfac_inv = 1.0 / m_vyfac;

	dpixptr = (unsigned char *)m_opDestinationBitMap;//->Bits();
	dpix = (unsigned char *)m_opDestinationBitMap;//dpixptr;
	spixptr = (unsigned long *)src;//->Bits();
	slb = srcFormat ->m_vBytesPerRow/4 ;//scols-1;//*4; //Should be x Color_Space()
//	;//cout_commented_out_4_release<<"srcFormat ->m_vBytesPerRow:"<<srcFormat ->m_vBytesPerRow<<endl;
	dlb = destFormat ->m_vBytesPerRow;//m_vDcols*4;//dest->BytesPerRow();
//	;//cout_commented_out_4_release<<"destFormat ->m_vBytesPerRow:"<<destFormat ->m_vBytesPerRow<<endl;
	
	// Perform the scaling by inverse mapping from dest to source
	// That is, for each point in the destination, find the 
	// corresponding point in the source.
	// Adding +1 to m_vDrows because the demands from the GUI
	// is actually a value not counting zero as a pixel
	//If we don't fo this there will be a white line at the bottom
	for (i = 0; i<m_vDrows-1; i++)
	{
		src_y_f = i * yfac_inv;
		src_y_i = (long) src_y_f;
		
		q = src_y_f - (float)src_y_i;
		q1 = 1.0 - q;
		spix1 = spixptr + src_y_i*slb;		//Hamnar på första pixeln i raden, slb=antalet bytes per rad
/*		if(i < m_vDrows-2)
			spix2 = spixptr + (src_y_i +1)*slb; //src_y_i=raden, hamnar på sista pixeln i raden
		else
*/
			spix2 = spixptr + (src_y_i +1)*slb; //src_y_i=raden, hamnar på sista pixeln i raden
//			;//cout_commented_out_4_release<<"Starting to read from byte:"<<(src_y_i +1)*slb<<endl;
//		;//cout_commented_out_4_release<<"spixptr + src_y_i*slb:"<<src_y_i*slb<< endl;
		m_opRptr = m_opR;

		transparency = 0.0;
		if(i+1 == m_vDrows-1)
			m_vDcols --;
		for (j = 0; j<m_vDcols; j++)
		{
			src_x_i = m_opRptr[j] -> x_i;
			p = m_opRptr[j] -> p;
			p1 = m_opRptr[j] -> p1;
			
/*			if((src_y_i +1)*slb == 327040)
				;//cout_commented_out_4_release<<"src_x_i"<<src_x_i<<endl;
*/
			// Get the four corner pixels
			a = (unsigned char *)(spix1 + src_x_i);
			b = (unsigned char *)(spix1 + src_x_i + 1);
			c = (unsigned char *)(spix2 + src_x_i);
			d = (unsigned char *)(spix2 + src_x_i + 1);

			
			// Compute the interpolated pixel value
/*			if((src_x_i > 637) && ((src_y_i +1)*slb > 325760))
			{
				;//cout_commented_out_4_release<<"rblue:"<<rblue<<endl;
				;//cout_commented_out_4_release<<"rgreen:"<<rgreen<<endl;
				;//cout_commented_out_4_release<<"rred:"<<rred<<endl;
				;//cout_commented_out_4_release<<"p:"<<p<<endl;
				;//cout_commented_out_4_release<<"p1:"<<p1<<endl;
				;//cout_commented_out_4_release<<"q:"<<q<<endl;
				;//cout_commented_out_4_release<<"d:"<<(float)d[0]<<endl;
				;//cout_commented_out_4_release<<"Furthest BYTE:"<<(src_y_i +1)*slb*4+src_x_i*4+1+2<<endl;
				;//cout_commented_out_4_release<<"(float)d[2]*p)*q:"<<((float)d[2]*p)*q<<endl;
			}
*/
			rblue = (((float)a[0]*p1 + (float)b[0]*p)*q1 + ((float)c[0]*p1 + (float)d[0]*p)*q);
			rgreen = (((float)a[1]*p1 + (float)b[1]*p)*q1 + ((float)c[1]*p1 + (float)d[1]*p)*q);
			rred = (((float)a[2]*p1 + (float)b[2]*p)*q1 + ((float)c[2]*p1 + (float)d[2]*p)*q);

			
			dpix[0] = static_cast<unsigned char>( rblue);//(rblue * (1.0 - transparency));// + (rblue2 * transparency));
			dpix[1] = static_cast<unsigned char>( rgreen);//(rgreen * (1.0 - transparency));// + (rgreen2 * transparency));
			dpix[2] = static_cast<unsigned char>( rred);//(rred * (1.0 - transparency));// + (rred2 * transparency));

			dpix += 4;

		}		
		
		// Advance to the next destination line
		dpix = (dpixptr += dlb);
	}
 return m_opDestinationBitMap;
}
