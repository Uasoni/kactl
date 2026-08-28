#pragma once

class UnitTest;
class UnitTestWrapper;

class UnitTestManager
{
	friend class UnitTest;
	friend class UnitTestWrapper;

private:
	UnitTestManager();
	~UnitTestManager();

public:
	// retrieves singleton instance.
	static UnitTestManager* get_instance();

protected:
	// reports a "check" failure.
	void report_check_failure(const string& have,
	                        const string& want,
	                        const string& message = "");

	// reports a failure.
	void report_failure(const string& message = "");

protected:
	// registers a test wrapper.
	void register_wrapper(UnitTestWrapper* UnitTestWrapper);

public:
	// runs all tests.
	void run_all();

	// runs a specific test.
	void run_test(const string& name);

	// print statistics.
	void print_statistics() const;

private:
	// runs a specific test.
	void run_test(UnitTestWrapper* UnitTestWrapper);

private:
	// maps test names to test wrapper objects.
	map<string, UnitTestWrapper*> m_unit_test_wrappers;

	// statistics.
	int m_success_count;
	int m_failure_count;
};
