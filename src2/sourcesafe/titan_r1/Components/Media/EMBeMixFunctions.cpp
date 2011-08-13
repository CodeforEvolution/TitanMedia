#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#include "EMBeMixFunctions.h"
#include "EMBeMediaUtility.h"

#include <iostream>
#include <MediaKit.h>
#include <string>
#include <TranslationUtils.h>

BBitmap* EMBeMixFunctions::myPicture = NULL;
bool EMBeMixFunctions::agust = true;
float EMBeMixFunctions::transparency = 0.0;
bool EMBeMixFunctions::svaj = true;

EMBeMixFunctions::EMBeMixFunctions()
{
	EMBeMediaUtility::push(this, "EMBeMixFunctions");
}

EMBeMixFunctions::~EMBeMixFunctions()
{

	EMBeMediaUtility::pop("EMBeMixFunctions");
//	EMBeMediaUtility::objects--;
}

EMMediaDataBuffer* EMBeMixFunctions::VideoPrioritizer(list<EMMediaDataBuffer*>* p_opList)
{
	EMMediaDataBuffer* opTarget = p_opList -> front();
	EMMediaDataBuffer* opSource = NULL;
	EMMediaDataBuffer* opSourceTemp = NULL;
//	return opTarget;
	//;//cout_commented_out_4_release<<"LIST SIZE:"<<p_opList -> size()<<endl;
	//;//cout_commented_out_4_release<<"PRIORITY1:"<<opTarget->m_vPriority<<endl;
	p_opList -> pop_front();
	while(p_opList -> size() > 0)
	{
		opSource = p_opList -> front();
		//;//cout_commented_out_4_release<<"PRIORITY2:"<<opSource->m_vPriority<<endl;
		if(! opSource -> m_vIsSilence)
		{
			opSourceTemp = opSource;
		}
		p_opList -> pop_front();
		
		if(opSource -> m_vIsSilence)
		{
			if(opSource -> m_vShouldBeDeleted)
				delete opSource;
			else
				opSource -> Recycle();
		} 
		else if(opTarget -> m_vIsSilence)
		{
			if(opTarget -> m_vShouldBeDeleted)
				delete opTarget;
			else
				opTarget -> Recycle();
			opTarget = opSource;				
		} 
		else if(opTarget -> m_vPriority > opSource -> m_vPriority)
		{
			if(opTarget -> m_vShouldBeDeleted)
				delete opTarget;
			else
				opTarget -> Recycle();
			opTarget = opSource;
		}
		else
		{
			if(opSource -> m_vShouldBeDeleted)
				delete opSource;
			else
				opSource -> Recycle();
		}
	}
	//;//cout_commented_out_4_release<<"opTarget -> m_vPriority:"<<opTarget -> m_vPriority<<endl;
	return 	opTarget;

	if(agust)
	{
		myPicture = static_cast<BBitmap*> (BTranslationUtils::GetBitmap("/boot/home/Desktop/ClapHands2.tga", NULL));
		//myPicture = new BBitmap(BRect(0,0, 720, 576), B_RGB32, false, true); //
		agust = false;
	}

	unsigned char *spixptr;
	unsigned char *dpixptr;

	dpixptr = static_cast<unsigned char*> (opTarget -> Data());
	if(opSource != NULL && opSource != opTarget)
	{
		spixptr = static_cast<unsigned char*> (opSourceTemp -> Data());
	}
	else
		spixptr = static_cast<unsigned char*> (myPicture -> Bits());


	unsigned char *a; //was also registered
	unsigned char *e; //was also registered

	int16 vColorValue;

	if(transparency < 1.0 && svaj)
		transparency += 0.01;
	else
	{
		svaj = false;
	}

	if(transparency > 0 && !svaj)
		transparency -= 0.01;
	else
	{
		svaj = true;
	}

		if(transparency > 1.0)
			transparency = 1.0;
		if(transparency < 0)
			transparency = 0.0;

	//transparency = 0.5;

	for (int i=0; i<(opTarget ->m_oFormat.m_vHeight); i++)
	{

		for (int j =0; j<((opTarget ->m_oFormat.m_vWidth*4)); j+=4)
		{
			a = static_cast<unsigned char *>(dpixptr+(i*opTarget ->m_oFormat.m_vWidth*4)+j);
			e = static_cast<unsigned char *>(spixptr+(i*opSourceTemp ->m_oFormat.m_vWidth*4)+j);

			vColorValue = static_cast<int16> (a[0] * (1.0 - transparency) + (e[0] * transparency));
			if(vColorValue < 0)
			{
				a[0] = 0;
			}
			if(vColorValue < 256)
				a[0] = static_cast<unsigned char>(vColorValue); //B
			else
			{
				a[0] = 255;
			}

			vColorValue = static_cast<int16> (a[1] * (1.0 - transparency) + (e[1] * transparency));
			if(vColorValue < 0)
			{
				a[1] = 0;
			}
			if(vColorValue < 256)
				a[1] = static_cast<unsigned char>(vColorValue); //G
			else
			{
				a[1] = 255;
			}

			vColorValue = static_cast<int16> (a[2] * (1.0 - transparency) + (e[2] * transparency));
			if(vColorValue < 0)
			{
				a[2] = 0;
			}
			if(vColorValue < 256)
			{
				a[2] = static_cast<unsigned char>(vColorValue); //R
			}
			else
			{
				a[2] = 255;
			}

		}
	
	}
	//opTarget -> Recycle();
	return 	opTarget;
}

#endif
