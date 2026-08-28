#pragma once

#include "global.h"
#include "UnitTestManager.h"

class UnitTestFailed
{
};

class UnitTest
{
public:
	UnitTest(const string& test_name):
		m_name(test_name)
	{
	}

	virtual ~UnitTest()
	{

	}

public:
	virtual void run(int subcase) = 0;

	virtual int get_count() const
	{
		return 1;
	}

protected:
	template<class T>
	void check(const T& have, const T& want, const string& message = "")
	{
		if(have == want)
			return;

		UnitTestManager* UnitTestManager = UnitTestManager::get_instance();

		UnitTestManager->report_check_failure(convert_to_string(have), convert_to_string(want), message);

		throw UnitTestFailed();
	}

	void fail(const string& message)
	{
		UnitTestManager* UnitTestManager = UnitTestManager::get_instance();

		UnitTestManager->report_failure(message);

		throw UnitTestFailed();
	}

private:
	template<class T>
	string convert_to_string(const T& data)
	{
		ostringstream oss;

		oss.precision(20);
		oss << data;

		return oss.str();
	}

	template<class T>
	string convert_to_string(const vector<T>& data)
	{
		ostringstream oss;

		oss << "{ ";
		for(auto &it: data)
			oss << convert_to_string(it) << " ";
		oss << "}";

		return oss.str();
	}

public:
	string get_name()
	{
		return m_name;
	}

protected:
	string m_name;
};

#define KACTL_AUTOREGISTER_TEST(x) UnitTestWrapper* g__kactl__temp__##x = new UnitTestWrapper(new x())
#include "UnitTestWrapper.h"

#ifndef KACTL_UNITTEST_BATCH
#	include "UnitTestManager.cpp"
#endif
