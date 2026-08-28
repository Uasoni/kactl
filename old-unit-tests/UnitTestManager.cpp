#include "global.h"
#include "UnitTestManager.h"
#include "UnitTest.h"
#include "UnitTestWrapper.h"

//#include <direct.h>

UnitTestManager::UnitTestManager():
	m_success_count(0),
	m_failure_count(0)
{
}

UnitTestManager::~UnitTestManager()
{
	for(auto &it: m_unit_test_wrappers)
		delete it.second;

	m_unit_test_wrappers.clear();
}

UnitTestManager* UnitTestManager::get_instance()
{
	static UnitTestManager s_unit_test_manager;

	return &s_unit_test_manager;
}

void UnitTestManager::report_check_failure(const string& have,
										 const string& want,
										 const string& message)
{
	fprintf(stderr, "FAILED\n");
	fprintf(stderr, "\t_wanted:   \"%s\"\n", want.c_str());
	fprintf(stderr, "\t_returned: \"%s\"\n", have.c_str());
	if(!message.empty())
		fprintf(stderr, "\t_message: %s\n", message.c_str());
	fflush(stderr);
}


void UnitTestManager::report_failure(const string& message)
{
	fprintf(stderr, "FAILED\n");
	if(!message.empty())
		fprintf(stderr, "\t_message: %s\n", message.c_str());
	fflush(stderr);
}

void UnitTestManager::register_wrapper(UnitTestWrapper* UnitTestWrapper)
{
	if(UnitTestWrapper == NULL || UnitTestWrapper->get_unit_test() == NULL)
		return;

	m_unit_test_wrappers.insert(make_pair(UnitTestWrapper->get_unit_test()->get_name(), UnitTestWrapper));
}

void UnitTestManager::run_all()
{
	for(auto &it: m_unit_test_wrappers)
		run_test(it.second);
}

void UnitTestManager::run_test(const string& name)
{
	map<string, UnitTestWrapper*>::iterator it = m_unit_test_wrappers.find(name);

	if(it == m_unit_test_wrappers.end())
	{
		fprintf(stderr, "ERROR: couldn't find test \"%s\"!", name.c_str());
		fflush(stderr);
		return;
	}

	run_test(it->second);
}

void
UnitTestManager::print_statistics() const
{
	fprintf(stderr, "\n_statistics:\n");
	fprintf(stderr, "\t_successes: %d\n", m_success_count);
	fprintf(stderr, "\t_failures:  %d\n", m_failure_count);
	fprintf(stderr, "\t_total:     %d\n", m_success_count + m_failure_count);
	fflush(stderr);
}

void UnitTestManager::run_test(UnitTestWrapper* UnitTestWrapper)
{
	UnitTest* UnitTest = UnitTestWrapper->get_unit_test();
	int count = UnitTest->get_count();
	string name = UnitTest->get_name();

	for (int i = 0; i < (count); ++i)
	{
		fprintf(stderr, "running test \"%s\" <%d, %d>... ",
			name.c_str(), i, UnitTest->get_count());
		fflush(stderr);

		try
		{
			UnitTest->run(i);

			m_success_count++;
			fprintf(stderr, "OK\n");
			fflush(stderr);
		}
		catch(const UnitTestFailed&)
		{
			m_failure_count++;
		}
		catch(const exception& e)
		{
			m_failure_count++;
			fprintf(stderr, "FAILED\n");
			fprintf(stderr, "\t_exception: %s\n", e.what());
			fflush(stderr);
		}
		catch(...)
		{
			m_failure_count++;
			fprintf(stderr, "FAILED\n");
			fprintf(stderr, "\t_unknown exception caught!\n");
			fflush(stderr);
		}
	}
}

int main(int argc, char** argv)
{
	if(argc >= 2)
	{
		for (int i = 1; i < (argc); ++i)
			UnitTestManager::get_instance()->run_test(argv[i]);
	}
	else
	{
		UnitTestManager::get_instance()->run_all();
	}

	UnitTestManager::get_instance()->print_statistics();

	return 0;
}
