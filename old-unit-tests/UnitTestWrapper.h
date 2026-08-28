#pragma once

#include "global.h"
#include "UnitTest.h"
#include "UnitTestManager.h"

class UnitTestWrapper
{
public:
	UnitTestWrapper(UnitTest* UnitTest):
		m_unit_test(UnitTest)
	{
		UnitTestManager::get_instance()->register_wrapper(this);
	}

	~UnitTestWrapper()
	{
		if(m_unit_test != NULL)
			delete m_unit_test;
	}

public:
	UnitTest* get_unit_test()
	{
		return m_unit_test;
	}

private:
	UnitTest* m_unit_test;
};
