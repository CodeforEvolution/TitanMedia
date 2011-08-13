#include "EMExceptionHandler.h"

#include "EMException.h"

const int BUFFER_SIZE = 1024 * 1024 * 5; // 5 MB

EMExceptionHandler* EMExceptionHandler::m_opInstance = NULL;
char* EMExceptionHandler::m_vpBuffer = NULL;

EMExceptionHandler::EMExceptionHandler()
{
	m_vpBuffer = NULL;
/*
Reserve memory buffer is disabled in this version of Titan
	m_vpBuffer = EM_new char[BUFFER_SIZE];
	for(int vIndex = 0; vIndex < BUFFER_SIZE; vIndex++)
		m_vpBuffer[vIndex] = 'x';
	;//cout_commented_out_4_release << "Reserve memory allocated" << endl;
*/
}

EMExceptionHandler::~EMExceptionHandler()
{
	delete [] m_vpBuffer;
}

void EMExceptionHandler::DeleteInstance()
{
	delete m_opInstance;
}

void EMExceptionHandler::HandleException(EMException p_oException)
{
	switch(p_oException.GetType())
	{
	/*
	case EM_BAD_ALLOC:
		if(m_vpBuffer == NULL)
			;//cout_commented_out_4_release << "Out of memory (reserve buffer empty)" << endl;
		else
		{
			;//cout_commented_out_4_release << "Memory is low, releasing reserve buffer" << endl;
			delete m_vpBuffer;
			m_vpBuffer = NULL;
		}
		break;
	*/
	case EM_DIALOG_ALWAYS:
//			ErrorAlert(p_oException.GetMessage(), B_ERROR);
		eo << (*(p_oException.GetMessage())).c_str() << ef;
		break;
	case EM_DIALOG_SELECTABLE: // Works just like EM_DIALOG_ALWAYS for the moment
		if(p_oException.GetMessage() == NULL)
			;//cout_commented_out_4_release << "An exception has ocurred" << endl;
		else
			;//cout_commented_out_4_release << "Exception dialog:" << (*(p_oException.GetMessage())) << endl;
		break;
	default:
		break;
	}
}

EMExceptionHandler* EMExceptionHandler::Instance()
{
	if(m_opInstance != NULL)
		return m_opInstance;

	m_opInstance = EM_new EMExceptionHandler();
	return m_opInstance;
}

void EMExceptionHandler::NewHandler()
{
	if(m_vpBuffer != NULL)
	{
		delete [] m_vpBuffer;
		m_vpBuffer = NULL;
	}
	else
		throw bad_alloc();
}

