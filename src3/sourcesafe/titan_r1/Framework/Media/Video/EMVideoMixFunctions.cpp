#include "EMGlobals.h"


#include "EMVideoMixFunctions.h"

EMVideoMixFunctions::EMVideoMixFunctions()
{
}

EMVideoMixFunctions::~EMVideoMixFunctions()
{
}


EMMediaDataBuffer* EMVideoMixFunctions::VideoPrioritizer(list<EMMediaDataBuffer*>* p_opList)
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
}